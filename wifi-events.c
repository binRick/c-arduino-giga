#include "arduino_secrets.h"

#include <Arduino_ConnectionHandler.h>

#if defined(BOARD_HAS_WIFI)
WiFiConnectionHandler conMan(SECRET_SSID, SECRET_PASS);
#endif

void setup() {
  Serial.begin(9600);
  /* Give a few seconds for the Serial connection to be available */
  delay(4000);
#ifndef __AVR__
  setDebugMessageLevel(DBG_INFO);
#endif
  conMan.addCallback(NetworkConnectionEvent::CONNECTED, onNetworkConnect);
  conMan.addCallback(NetworkConnectionEvent::DISCONNECTED, onNetworkDisconnect);
  conMan.addCallback(NetworkConnectionEvent::ERROR, onNetworkError);
}

void loop() {
  /* The following code keeps on running connection workflows on our
   * ConnectionHandler object, hence allowing reconnection in case of failure
   * and notification of connect/disconnect event if enabled (see
   * addConnectCallback/addDisconnectCallback) NOTE: any use of delay() within
   * the loop or methods called from it will delay the execution of .update(),
   * which might not guarantee the correct functioning of the ConnectionHandler
   * object.
   */

  conMan.check();
}

void onNetworkConnect() {
  Serial.println(">>>> CONNECTED to network");
}

void onNetworkDisconnect() {
  Serial.println(">>>> DISCONNECTED from network");
}

void onNetworkError() {
  Serial.println(">>>> ERROR");
}

