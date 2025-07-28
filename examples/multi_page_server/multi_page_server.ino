#include "multi_page_server.hpp"

String ssid     = "<Your SSID>";
String password = "<Your Password>";

custom_server* server;

void setup()
{
  Serial.begin(115200);
  delay(2000);

  server = new multi_page_server();
  server->start("Multi-Page Server", ssid, password);
}

void loop() 
{
}
