/* --- WAN WATCHDOG 0.1 ---
 * by EnforcerZhukov - http://www.enforcerzhukov.xyz
 * 
 * SERVERS Sketch: Define here external servers to ping over
 */

#define NSERVERS 27 //Total ammount of servers defined here
#define SERVERNAME_NCHARS 14 //Max. ammount of characters we'll use for server names

typedef struct PingServer {
  byte ip1;
  byte ip2;
  byte ip3;
  byte ip4;
  char serverName[15+1];
};

const PROGMEM PingServer servers[NSERVERS] = {
  /* Add each server as a new element of this array
   * Example of a server line: { 8,8,8,8, "GoogleDNS 1" },
   * Use commas to separate each server, but last server without comma!
   * IMPORTANT! NSERVERS must be the number of servers defined here!
   * Limited to 256 servers.
   */

  { 8,8,8,8, "GoogleDNS 1" },
  { 8,8,4,4, "GoogleDNS 2" },
  { 208,67,220,220, "OpenDNS 1" },
  { 208,67,222,222, "OpenDNS 2" },
  { 213,60,205,175, "R 1" },
  { 213,60,205,174, "R 2 PO/OU" },
  { 213,60,205,173, "R 3 CO/LU" },
  { 194,224,52,36, "Movistar 1" },
  { 194,224,52,37, "Movistar 2" },
  { 194,179,1,100, "Movistar 3" },
  { 62,42,230,24, "ONO" },
  { 4,2,2,1, "Level3 1" },
  { 4,2,2,2, "Level3 2" },
  { 4,2,2,3, "Level3 3" },
  { 4,2,2,4, "Level3 4" },
  { 4,2,2,5, "Level3 5" },
  { 4,2,2,6, "Level3 6" },
  { 212,55,8,132, "Euskaltel 1" },
  { 212,142,144,66, "Euskaltel 2" },
  { 212,55,8,133, "Euskaltel 3" },
  { 217,11,108,234, "Iberbanda" },
  { 77,88,8,8, "Yandex 1" },
  { 77,88,8,1, "Yandex 2" },
  { 216,146,35,35, "Dyn 1" },
  { 216,146,36,36, "Dyn 2" },
  { 64,6,64,6, "Verisign 1" },
  { 64,6,65,6, "Verisign 2" }

};

