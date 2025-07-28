#include "ajax_server.hpp"

String ssid     = "<Your SSID>";
String password = "<Your Password>";

custom_server* server;

void setup()
{
  Serial.begin(115200);
  delay(2000);

  server = new ajax_server();
  server->start("AJAX Server", ssid, password);
}

void loop() 
{
}
