/* --- WAN WATCHDOG 0.1 ---
 * by EnforcerZhukov - http://www.enforcerzhukov.xyz
 * 
 * LOOP Sketch
 */

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - prevMillis_lastPing >= DELAY_PING) { //Launch a PING
    prevMillis_lastPing = currentMillis;
    launchPing();
  }

  #if DHCP_ON == 1 && DHCP_MAINTAIN == 1
    if (currentMillis - prevMillis_dhcpMaintain >= DELAY_DHCP_MAINTAIN) {
      Ethernet.maintain();
      IPAddress currentIP = Ethernet.localIP();
      if (currentIP != prevIP) {
        Serial.print(F("IP changed by DHCP pool, from "));
        Serial.print(prevIP);
        Serial.print(F(" to NEW IP: "));
        Serial.println(currentIP);
        prevIP = currentIP;
      }
    }
  #endif

  #if MQTT_DEBUG == 1
    if (!mqtt.connected() && (prevMillis_mqttReconnect == 0 || currentMillis - prevMillis_mqttReconnect >= DELAY_MQTT_RECONNECT) ) {
      Serial.print(F("Connecting MQTT broker on "));
      Serial.print(mqttBroker);
      Serial.print(F(" with client name '"));
      Serial.print(clientName);
      Serial.print(F("'..."));
      if (mqtt.connect(clientName)) {
        Serial.println(F(" CONNECTED!"));
        prevMillis_mqttReconnect = 0;
      } else {
        Serial.println(F(" ERROR! Couldn't connect to MQTT broker!"));
        prevMillis_mqttReconnect = currentMillis;
      }
    }
  #endif
  
}

