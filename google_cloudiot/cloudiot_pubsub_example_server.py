# Copyright 2017 Google Inc. All rights reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
r"""Sample server that pushes configuration to Google Cloud IoT devices.

This example represents a server that consumes telemetry data from multiple
Cloud IoT devices. The devices report telemetry data, which the server consumes
from a Cloud Pub/Sub topic. The server then decides whether to turn on or off
individual devices fans.

This example requires the Google Cloud Pub/Sub client library. Install it with

  $ pip install --upgrade google-cloud-pubsub

If you are running this example from a Compute Engine VM, you will have to
enable the Cloud Pub/Sub API for your project, which you can do from the Cloud
Console. Create a pubsub topic, for example
projects/my-project-id/topics/my-topic-name, and a subscription, for example
projects/my-project-id/subscriptions/my-topic-subscription.

You can then run the example with

  $ python cloudiot_pubsub_example_server.py \
    --project_id=my-project-id \
    --pubsub_subscription=my-topic-subscription \
    --api_key=YOUR_API_KEY
"""

import argparse
import base64
import json
import sys
from threading import Lock
import time

from googleapiclient import discovery
from googleapiclient.errors import HttpError
from oauth2client.service_account import ServiceAccountCredentials
from google.cloud import pubsub_v1


API_SCOPES = ['https://www.googleapis.com/auth/cloud-platform']
API_VERSION = 'v1beta1'
DISCOVERY_API = 'https://cloudiot.googleapis.com/$discovery/rest'
#SERVICE_NAME = 'cloudiot'
SERVICE_NAME = 'e2e-example-service'

def discovery_url(api_key):
  """Construct the discovery url for the given api key."""
  return '{}?version={}&key={}'.format(DISCOVERY_API, API_VERSION, api_key)


class Server(object):
  """Represents the state of the server."""

  def __init__(self, service_account_json, api_key):
    credentials = ServiceAccountCredentials.from_json_keyfile_name(
        service_account_json, API_SCOPES)
    if not credentials:
      sys.exit('Could not load service account credential from {}'.format(
          service_account_json))

    self._service = discovery.build(
        SERVICE_NAME,
        API_VERSION,
        discoveryServiceUrl=discovery_url(api_key),
        credentials=credentials)

    # Used to serialize the calls to the
    # modifyCloudToDeviceConfig REST method. This is needed
    # because the google-api-python-client library is built on top
    # of the httplib2 library, which is not thread-safe. For more
    # details, see: https://developers.google.com/
    #     api-client-library/python/guide/thread_safety
    self._update_config_mutex = Lock()

  def _update_device_config(self, project_id, region, registry_id, device_id,
                            data):
    """Push the data to the given device as configuration."""
    config_data = None
    print 'The device ({}) has a temperature of: {}, a humidity of {}'.format(device_id,data['temperature'], data['humidity'])
    #print 'The device ({}) has a temperature of: {}'.format(device_id,data['temperature'])



    if data['temperature'] > 30:
      # Turn off the fan.
      config_data = {'RedLED': True, 'BlueLED':False}
      print 'Setting Red LED', device_id, 'to on.'
      
    elif data['temperature'] < 25 and data['temperature'] > 20:
      # Turn on the fan
      config_data = {'RedLED': False, 'BlueLED': False}
      print 'Setting Red LED', device_id, 'to off.'
      
    elif data['temperature'] <= 20:
      # Turn on the fan
      config_data = {'RedLED': False, 'BlueLED': True}
      print 'Setting Blue LED', device_id, 'to on.'
    elif data['temperature'] > 20 and data['temperature'] <= 25:
      # Turn on the fan
      config_data = {'RedLED': False, 'BlueLED': False}
      print 'Setting Blue LED', device_id, 'to off.'
    else:
      return
    #  config_data = {'RedLED': False, 'BlueLED': False}
    #  print 'Default'
      
    config_data_json = json.dumps(config_data)
    body = {
        # The device configuration specifies a version to update, which can be
        # used to avoid having configuration updates race. In this case, we
        # use the special value of 0, which tells Cloud IoT to always update the
        # config.
        'version_to_update': 0,
        'data': {
            # The data is passed as raw bytes, so we encode it as base64. Note
            # that the device will receive the decoded string, and so you do not
            # need to base64 decode the string on the device.
            'binary_data': base64.b64encode(config_data_json)
        }
    }

    device_name = 'projects/{}/locations/{}/registries/{}/devices/{}'.format(
        project_id, region, registry_id, device_id)

    request = self._service.projects().locations().registries().devices(
    ).modifyCloudToDeviceConfig(
        name=device_name, body=body)

    # The http call for the device config change is thread-locked so that we
    # don't have competing threads simultaneously using the httplib2 library,
    # which is not thread-safe.
    self._update_config_mutex.acquire()
    try:
      request.execute()
    except HttpError, e:
      # If the server responds with a HtppError, we log it here, but continue
      # so that the message does not stay NACK'ed on the pubsub channel.
      print 'Error executing ModifyCloudToDeviceConfig: {}'.format(e)
    finally:
      self._update_config_mutex.release()

  def run(self, project_id, pubsub_subscription):
    """The main loop. Consumes messages from the Pub/Sub subscription."""

    subscriber = pubsub_v1.SubscriberClient()
    subscription_path = subscriber.subscription_path(project_id,
                                                     pubsub_subscription)

    def callback(message):
      """Logic executed when a message is received from subscribed topic."""
      try:
        data = json.loads(message.data)
      except ValueError, e:
        print 'Loading Payload ({}) threw an Exception: {}.'.format(
            message.data, e)
        message.ack()
        return

      # Get the registry id and device id from the attributes. These are
      # automatically supplied by IoT, and allow the server to determine which
      # device sent the event.
      device_project_id = message.attributes['projectId']
      device_registry_id = message.attributes['deviceRegistryId']
      device_id = message.attributes['deviceId']
      device_region = message.attributes['deviceRegistryLocation']

      # Send the config to the device.
      self._update_device_config(device_project_id, device_region,device_registry_id, device_id, data)

      # Acknowledge the consumed message. This will ensure that they are not
      # redelivered to this subscription.
      message.ack()

    print 'Listening for messages on {}'.format(subscription_path)
    subscriber.subscribe(subscription_path, callback=callback)

    # The subscriber is non-blocking, so we must keep the main thread from
    # exiting to allow it to process messages in the background.
    while True:
      time.sleep(60)


def parse_command_line_args():
  """Parse command line arguments."""
  parser = argparse.ArgumentParser(
      description='Example of Google Cloud IoT registry and device management.')
  # Required arguments
  parser.add_argument(
      '--project_id', required=True, help='GCP cloud project name.')
  parser.add_argument(
      '--pubsub_subscription',
      required=True,
      help='Google Cloud Pub/Sub subscription name.')
  parser.add_argument('--api_key', required=True, help='Your API key.')

  # Optional arguments
  parser.add_argument(
      '--service_account_json',
      default='service_account.json',
      help='Path to service account json file.')

  return parser.parse_args()


def main():
  args = parse_command_line_args()

  server = Server(args.service_account_json, args.api_key)
  server.run(args.project_id, args.pubsub_subscription)


if __name__ == '__main__':
  main()
