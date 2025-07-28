#pragma once

#include "custom_server.hpp"

class ajax_server: public custom_server
{
  public:

    ajax_server(): custom_server()
    {
    }

  protected:

    void virtual handle_get(WiFiClient& client)
    {
      if      (source == "/hello"  ) send_hello_content  (client);
      else if (source == "/goodbye") send_goodbye_content(client);
      else if (source == "/reset"  ) send_reset_content  (client);
      else                           send_content        (client);
    }

    void send_hello_content(WiFiClient& client)
    {
      send_standard_header(client);
      client.println("<h1>Hello</h1><p>Say hello to the world!</p>");
    }

    void send_goodbye_content(WiFiClient& client)
    {
      send_standard_header(client);
      client.println("<h1>Goodbye</h1><p>Say goodbye for now!</p>");
    }

    void send_reset_content(WiFiClient& client)
    {
      send_standard_header(client);
      client.println("<p>Click buttons to change this text.</p>");
    }

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
        "    <div id='display'><p>Click buttons to change this text.</p></div>"
        "    <button type='button' onclick='send_ajax_request(\"hello\",   update_display)'>Hello</button>"
        "    <button type='button' onclick='send_ajax_request(\"goodbye\", update_display)'>Goodbye</button>"
        "    <button type='button' onclick='send_ajax_request(\"reset\",   update_display)'>Reset</button>"
      );
      send_ajax_request_script(client);
      send_update_script(client);
      send_prevent_reposting_on_page_refresh_script(client);
      client.println
      (
        "  </body>"
        "</html>"
      );
    }

    void send_update_script(WiFiClient& client)
    {
      client.println
      (
        "<script>"
        "  function update_display(response)"
        "  {"
        "    document.getElementById('display').innerHTML = response;"
        "  }"
        "</script>"
      );
    }

};
