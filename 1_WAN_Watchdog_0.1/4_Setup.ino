/* --- WAN WATCHDOG 0.1 ---
 * by EnforcerZhukov - http://www.enforcerzhukov.xyz
 * 
 * SETUP Sketch
 */

void setup()
{
  //First stuff initialization
  Serial.begin(BPS);
  Serial.println();
  Serial.println(F("Arduino WAN Watchdog booting up..."));
  randomSeed(analogRead(0));

  //Pin initialization
  pinMode(PIN_RELAY_MAIN, OUTPUT);
  digitalWrite(PIN_RELAY_MAIN, HIGH);
  pinMode(PIN_RELAY_AUX, OUTPUT);
  digitalWrite(PIN_RELAY_AUX, HIGH);
  pinMode(PIN_LED_OK, OUTPUT);
  pinMode(PIN_LED_KO, OUTPUT);
  led(false);

  //Ethernet initialization
  #if DHCP_ON == 1
    Serial.print(F("Connecting using DHCP..."));
    bool ledStatus = true;
    while (Ethernet.begin(arduinoMAC) == 0) {
      Serial.print('.');
      led(ledStatus);
      ledStatus = !ledStatus;
    }
    Serial.println(F(" CONNECTED!"));
    Serial.print(F("Arduino IP: "));
    Serial.println(Ethernet.localIP());
    #if DHCP_MAINTAIN == 1
      prevIP = Ethernet.localIP();
    #endif
  #else
    Ethernet.begin(arduinoMAC, ip, gateway);
    Serial.print(F("Connected using static IP "));
    Serial.print(ip);
    Serial.print(F(" and Gateway "));
    Serial.print(gateway);
  #endif
  Serial.println();
  led(true);

  Serial.print(F("Arduino MAC: "));
  for (byte i=0; i<6; i++) {
    byte chunk = arduinoMAC[i];
    if (chunk < 10) {
      Serial.print('0');
    }
    Serial.print(chunk, HEX);
    if (i<5) {
      Serial.print(':');
    }
  }
  Serial.println();

  #if MQTT_DEBUG == 1
    mqtt.setServer(mqttBroker, MQTT_PORT);
  #endif
  
  Serial.println(F("System load successful! Starting now!"));
}
