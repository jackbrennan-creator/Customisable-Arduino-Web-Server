#pragma once

#include "custom_server.hpp"

class form_server: public custom_server
{
  public:

    form_server(): custom_server()
    {
    }

  protected:

    void send_content(WiFiClient& client)
    {
      send_standard_header(client);
      client.println
      (
        "<!DOCTYPE html>"
        "<html>"
        "  <head>"
        "    <title>Form Server</title>"
        "    <meta name='viewport' content='width=device-width, initial-scale=1'>"
        "  </head>"
        "  <body>"
        "    <h2>Form Server</h2>"
        "    <form name='form1' method='post' action='/'>"
        "      <label>Name</label><br>"
        "      <input type='text' id='name' name='name' value=''> <br>"
        "      <label>Address</label><br>"
        "      <input type='text' id='address' name='address' value=''> <br>"
        "      <button type='submit' name='action' value='submit'>Submit</button>"
        "    </form>"
      );
      send_prevent_reposting_on_page_refresh_script(client);
      client.println
      (
        "  </body>"
        "</html>"
      );
    }

    void handle_post(WiFiClient& client)
    {
      String name    = get_content_parameter("name"   );
      String address = get_content_parameter("address");
      show_command_information();
      Serial.print("Name   : "); Serial.println(decode_url(name   ));
      Serial.print("Address: "); Serial.println(decode_url(address));
      send_content(client);
    }

};
