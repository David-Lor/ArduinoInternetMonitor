/* --- WAN WATCHDOG 0.1 ---
 * by EnforcerZhukov - http://www.enforcerzhukov.xyz
 * 
 * MAIN Sketch: Settings and definition of constants and variables.
 */

#include <SPI.h> //EthernetShield
#include <Ethernet.h> //EthernetShield - https://www.arduino.cc/en/Reference/Ethernet
#include <ICMPPing.h> //Ping - https://playground.arduino.cc/Code/ICMPPing
//#include <PubSubClient.h> //MQTT will only be imported if MQTT_DEBUG == 1 - https://github.com/knolleary/pubsubclient

/* SETTINGS HERE
 * v v v v v v v v
 */

//Delay and timing Config (all time in ms)
#define DELAY_PING 1500 //Time between Pings
#define PING_MAX_ERRORS 4 //Number of continuous ping errors needed to reboot the CPE
#define DELAY_CPE_OFF2ON 1000 //Safe time between the relay cuts off the CPE power supply and turns on again
#define DELAY_CPE_REBOOT 120000 //Safe wait time after a CPE reboot, so it can boot again and connect to Internet
#define DELAY_DHCP_MAINTAIN 3600000 //Time between DHCP renewal check
#define DELAY_MQTT_RECONNECT 60000 //Time between MQTT reconnect tries, if previous one was unsuccessful

//LAN Config (EthernetShield)
#define DHCP_ON 1 //1=use DHCP; 0=use Static IP
#define DHCP_MAINTAIN 1 //1=check local IP renewal by DHCP server; 0=don't check this
const byte arduinoMAC[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x01}; //Arduino MAC (always required)
const byte arduinoIP[4] = {192, 168, 0, 155}; //Will be ignored if DHCP is ON
const byte arduinoGateway[4] = {192, 168, 0, 1}; //Will be ignored if DHCP is ON

//Debug Config (MQTT)
//Serial debug always on
#define MQTT_DEBUG 0
const byte mqttBrokerIP[4] = {192, 168, 0, 102};
#define MQTT_PORT 1883
const char* clientName = "WanWatchdog";
const char* outTopicJson = "json"; //Topic for JSON output. Topics will be = "clientName/outTopic"
const char* outTopicHuman = "status"; //Topic for 'human-readable' outputs

//Misc Config
#define BPS 9600 //Serial Baudrate

//Pinout Config
#define PIN_RELAY_MAIN 2 //Main relay (where CPE is connected). Pin LOW = CPE turns off
#define PIN_RELAY_AUX 3 //Aux relay
#define PIN_LED_OK 4 //Status=OK LED
#define PIN_LED_KO 5 //Status=Fail LED

/* ^ ^ ^ ^ ^ ^ ^ ^ ^
 * END OF SETTINGS
 */

//Variables: Millis
unsigned long prevMillis_lastPing = 0;
#if DHCP_ON == 1 && DHCP_MAINTAIN == 1
  unsigned long prevMillis_dhcpMaintain = 0;
#endif
#if MQTT_DEBUG == 1
  unsigned long prevMillis_mqttReconnect = 0; //if this == 0 -> no previous unsuccessful reconnection
#endif

//Variables: Ethernet Shield
#if DHCP_ON == 0
  IPAddress ip(arduinoIP[0], arduinoIP[1], arduinoIP[2], arduinoIP[3]);
  IPAddress gateway(arduinoGateway[0], arduinoGateway[1], arduinoGateway[2], arduinoGateway[3]);
#elif DHCP_ON == 1
  IPAddress prevIP;
#endif
EthernetClient eth;

//Variables: ICMPPing
SOCKET pingSocket = 0;
//char buffer [256];
ICMPPing ping(pingSocket, (uint16_t)random(0, 255));

//Variables: MQTT
#if MQTT_DEBUG == 1
  #include <PubSubClient.h> //MQTT library
  IPAddress mqttBroker(mqttBrokerIP[0], mqttBrokerIP[1], mqttBrokerIP[2], mqttBrokerIP[3]);
  PubSubClient mqtt(eth);
#endif

