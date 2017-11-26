/* --- WAN WATCHDOG 0.1 ---
 * by EnforcerZhukov - http://www.enforcerzhukov.xyz
 * 
 * PING Sketch: ping method and CPE restart
 */

byte lastServerIndex = 0;

bool doPing()
{ //Ping a server and return true=ping OK, false=ping KO
  byte serverIndex = lastServerIndex;
  while (serverIndex == lastServerIndex) {
    serverIndex = random(0, NSERVERS);
  }
  lastServerIndex = serverIndex;

  PingServer server = servers[serverIndex];
  IPAddress pingIP(server.ip1, server.ip2, server.ip3, server.ip4); //IP to ping over
  
  Serial.print(F("Pinging "));
  Serial.print(server.serverName);
  Serial.print(F(" ("));
  Serial.print(pingIP);
  Serial.print(F(")... "));

  ICMPEchoReply echoReply = ping(pingIP, 4); //Ping execution

  if (echoReply.status == SUCCESS)
  { //Ping OK
    byte pingTime = millis() - echoReply.data.time;
    Serial.print( F("OK - "));
    Serial.print(millis() - echoReply.data.time);
    Serial.println(F("ms"));
    led(true);
    return true;
  }
  else 
  { //Ping KO
    Serial.println(F("KO"));
    led(false);
    return false;
  }

}

byte pingErrors = 0;

void launchPing()
{ //Called from main sketch each X seconds. Do a ping and decide if reboot the CPE
  if (doPing()) {
    pingErrors = 0; //Reset error counter to 0
  } else {
    if (++pingErrors >= PING_MAX_ERRORS) { //We need to reboot CPE
      Serial.print(pingErrors);
      Serial.println(F(" ping errors reached, rebooting CPE!"));
      digitalWrite(PIN_RELAY_MAIN, LOW);
      delay(DELAY_CPE_OFF2ON);
      digitalWrite(PIN_RELAY_MAIN, HIGH);

      Serial.print(F("CPE rebooted, waiting "));
      Serial.print(DELAY_CPE_REBOOT/1000);
      Serial.println(F(" seconds before resuming pings..."));
      delay(DELAY_CPE_REBOOT);
      pingErrors = 0; //Reset error counter to 0 after rebooting CPE
    }
  }
}

void led(bool ok) { //true: turn on "ok" & turn off "ko" / false: turn on "ko" & turn off "ko"
  if (ok) { //turn on OK
    digitalWrite(PIN_LED_KO, LOW);
    digitalWrite(PIN_LED_OK, HIGH);
  } else { //turn on KO
    digitalWrite(PIN_LED_OK, LOW);
    digitalWrite(PIN_LED_KO, HIGH);
  }
}

