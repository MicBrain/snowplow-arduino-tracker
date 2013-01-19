/* 
 * SnowPlow Arduino Tracker
 *
 * @description Arduino tracker for SnowPlow
 * @version     0.0.1
 * @author      Alex Dean
 * @copyright   SnowPlow Analytics Ltd
 * @license     Apache License Version 2.0
 *
 * Copyright (c) 2012 SnowPlow Analytics Ltd. All rights reserved.
 *
 * This program is licensed to you under the Apache License Version 2.0,
 * and you may not use this file except in compliance with the Apache License Version 2.0.
 * You may obtain a copy of the Apache License Version 2.0 at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the Apache License Version 2.0 is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the Apache License Version 2.0 for the specific language governing permissions and limitations there under.
 */

#include <SPI.h>
#include "SnowPlowTracker.h"
#include <Ethernet.h>
#include <EthernetClient.h>

private:

// Initialize constants
const char* SnowPlowTracker::kUserAgent = "Arduino/2.0";
const char* SnowPlowTracker::kVersion = "arduino-0.1.0";

public:

/**
 * Constructor for SnowPlow class.
 *
 * @param aEthernet Pointer to the
 *        EthernetClass (initialised
 *        outside of this library)
 * @param aMac The MAC address of the
 *        Arduino's WiFi or Ethernet
 *        Shield
 * @param aAppId The SnowPlow application
 *        ID
 **/
SnowPlowTracker::SnowPlowTracker(EthernetClass *aEthernet, const byte* aMac, const String aAppId) {  
  this->ethernet = aEthernet;
  this->mac = aMac;
  this->appId = aAppId;
}

/**
 * Initializes the SnowPlow tracker to
 * talk to a collector hosted on
 * CloudFront.
 *
 * @param aCfSubdomain The subdomain
 *        of the CloudFront collector
 *        e.g. "d3rkrsqgmqf"
 */
void SnowPlowTracker::initCf(const String aCfSubdomain) {
  String domain = cfSubdomain + String(".cloudfront.net");
  this->init(domain);
}

/**
 * Initializes the SnowPlow tracker
 * to speak to a URL-based (self-
 * hosted) collector.
 *
 * @param aHost The hostname of the
 *        URL hosting the collector
 *        e.g. tracking.mysite.com
 */
void SnowPlowTracker::initUrl(const String aHost) {
  this->init(aHost);
}

/**
 * Sets the User Id for this Arduino.
 * Overrides the default User Id, which
 * is the Arduino's MAC address.
 *
 * @param @aUserId The new User Id
 */
void SnowPlowTracker::setUserId(const String aUserId) {
  this->userId = userId;
}

/**
 * Tracks a structured event to a
 * SnowPlow collector: version
 * where the value field is a float.
 *
 * @param aCategory The name you supply for
 *        the group of objects you want to track
 * @param aAction A string that is uniquely
 *        paired with each category, and commonly
 *        used to define the type of user
 *        interaction for the web object
 * @param aLabel An optional string
 *        to provide additional dimensions to the
 *        event data
 * @param aProperty An optional string
 *        describing the object or the action
 *        performed on it. This might be the
 *        quantity of an item added to basket
 * @param aValue A (typically numeric) value that
 *        you can use to provide numerical data
 *        about the user event
 * @return An integer indicating the success/failure
 *         of logging the event to SnowPlow
 */ 
int SnowPlowTracker::trackEvent(
  const String aCategory,
  const String aAction,
  const String aLabel,
  const String aProperty,
  const float aValue) const {

  // Cast aValue to String and call appropriate trackEvent
  trackEvent(aCategory, aAction, aLabel, aProperty, String(aValue));
}

/**
 * Tracks a structured event to a
 * SnowPlow collector: version
 * where the value field is an int.
 *
 * @param aCategory The name you supply for
 *        the group of objects you want to track
 * @param aAction A string that is uniquely
 *        paired with each category, and commonly
 *        used to define the type of user
 *        interaction for the web object
 * @param aLabel An optional string
 *        to provide additional dimensions to the
 *        event data
 * @param aProperty An optional string
 *        describing the object or the action
 *        performed on it. This might be the
 *        quantity of an item added to basket
 * @param aValue A (typically numeric) value that
 *        you can use to provide numerical data
 *        about the user event
 * @return An integer indicating the success/failure
 *         of logging the event to SnowPlow
 */ 
int SnowPlowTracker::trackEvent(
  const String aCategory,
  const String aAction,
  const String aLabel,
  const String aProperty,
  const int aValue) const {

  // Cast aValue to String and call appropriate trackEvent
  trackEvent(aCategory, aAction, aLabel, aProperty, String(aValue));
}

/**
 * Tracks a structured event to a
 * SnowPlow collector: version
 * where the value field is a String.
 *
 * @param aCategory The name you supply for
 *        the group of objects you want to track
 * @param aAction A string that is uniquely
 *        paired with each category, and commonly
 *        used to define the type of user
 *        interaction for the web object
 * @param aLabel An optional string
 *        to provide additional dimensions to the
 *        event data
 * @param aProperty An optional string
 *        describing the object or the action
 *        performed on it. This might be the
 *        quantity of an item added to basket
 * @param aValue A (typically numeric) value that
 *        you can use to provide numerical data
 *        about the user event
 * @return An integer indicating the success/failure
 *         of logging the event to SnowPlow
 */ 
int SnowPlowTracker::trackEvent(
  const String aCategory,
  const String aAction,
  const String aLabel,
  const String aProperty,
  const String aValue) const {

  Serial.println("Tracking event!");
  return 0;
}

private:

/**
 * Common initialization, called by
 * both initCf and initUrl.
 *
 * @param aHost The hostname of the
 *        URL hosting the collector
 *        e.g. tracking.mysite.com
 *        or d3rkrsqgmqf.cloudfront.net
 */
void SnowPlowTracker::init(const String aHost) {
  // Set trackerUrl and userId
  this->collectorUrl = aHost;
  this->userId = mac2String(this->mac);

  // Boot the Ethernet connection
  this->ethernet->begin(this->mac);
  delay(1000); // Wait 1 sec
  this->client = new EthernetClient();
}

/**
 * Converts a MAC address byte array
 * into a String. Generated String is
 * of the format: "00:01:0A:2E:05:0B"
 *
 * @param aMac The MAC address, in bytes,
 *             to convert
 * @return the MAC address as a String
 */
String SnowPlowTracker::mac2String(const byte* aMac) const {
  const int macLength = 6;
  String buffer = String();
  for (int i = 0; i < macLength; i++) {
    buffer += String(mac[i], HEX);
    if (i < macLength - 1) {
      buffer += ":";
    }
  }
  return buffer;
}


/*

int SnowPlowTracker::trackEvent(const String category, const String action, const String label, const String property, const float value) const {
  // TODO: fix this crap.
  char rxdata[150];
  int ret = 0;
  int stringPos = 0;
  boolean DataRx = false;
  boolean RxLoop = true;
  char c;
  unsigned long timeout_time = 0;
  unsigned long time_now = 0;
  unsigned long timeout = 3000; // 3 seconds
  String myDataString = ""; // Allocate for actual data sent

  if (client->connect(serverName,80)) {
    if (client->connected()) {

      client->println("GET /i HTTP/1.1");

      client->print("Host: ");
      client->println(this->collectorUrl);

      client->print("User-Agent: ");
      client->println(kUserAgent);

      client->println();

      
      // TODO: check if we need more headers.

      // Read from the nic
      //
      timeout_time = millis()+ timeout; 
      while ((timeout_time > time_now) && RxLoop) { 
        if (client->available()) {
          if (!DataRx)
            DataRx= true;          
          c = client->read();
          rxdata[stringPos] = c;          
          stringPos += 1;
        } else {
          rxdata[stringPos] = 0;

          if (DataRx) {
            DataRx= false;
            RxLoop = false;

            ret=1;
          }
        }//else
        time_now = millis();
      }// while ((timeout_time > time_now) && RxLoop) {

      client->stop();
    }
  }// if (client->connect(serverName,80)) {
  
  // Return updated status code
  return ret;
}

*/