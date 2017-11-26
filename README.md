# Arduino Internet/WAN Watchdog

This Arduino sketch pings external servers over the Internet to check if your Internet connection is up or down. If it's down, a relay attached to the Arduino reboots your router, cablemodem or any other CPE you're using, by cutting it's power supply for a second. Notice this is only useful for equipments that doesn't auto-reconnect to the Internet when they loose connection.

## Requirements

* Any Arduino-like capable board. Tested on Arduino Uno (official & clone) and Arduino Mega (clone).
* An Ethernet Shield using the W5100 chip, or any other compatible with the Ethernet library
* One relay, activated with LOW digital input
* [Ethernet](https://www.arduino.cc/en/Reference/Ethernet) and [ICMPPing](https://playground.arduino.cc/Code/ICMPPing) libraries; [PubSubClient](https://github.com/knolleary/pubsubclient) optional for MQTT support (MQTT not implemented yet)

## Features

* Multiple external server definition
* False positive tolerance
* Highly customizable
* Complete debug over Serial
* Two LED debug (OK/FAIL system), can use a bi-color LED
* MQTT debug, web interface & SD card support (coming soon)

## How does it work?

Arduino will pick a random server from the list of servers provided, but won't ping the same server twice in a row, avoiding false positives (when your Internet connection is UP but the server is down). If it can't ping the server, will add +1 to an "error" counter. When this counter reaches a certain ammount of errors (4 by default), CPE will reboot. This counter will be reset when CPE is rebooted, or after another ping is successful.

CPE **Vcc or GND from power supply should be connected between Normally Closed and Common pins of the relay**, so it will be ON by default, when relay is OFF. On the sketch, **relay pin is LOW for relay ON, and HIGH for relay OFF**. When Arduino reboots the CPE, will turn on the relay for a second and turn if off again, so will cut off power from CPE rebooting it. After rebooting the CPE, the system will wait for 2 minutes before resuming pings, giving enough time for CPE to reboot and reconnect to the Internet.

## Important about Ethernet Shield

Some network equipment (routers, switches...) doesn't work well with the W5100 Ethernet Shield, not working at all, or making a bit longer to establish link with it. On this last case, when using static IP on the Arduino, false positives will happen, since Arduino will get a static IP but won't be connected yet to the LAN. Using DHCP doesn't show this problem, since it has to get an IP from the DHCP server before starting pings.

