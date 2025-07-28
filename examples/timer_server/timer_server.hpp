#pragma once

#include "custom_server.hpp"

class timer_server: public custom_server
{
  public:

    timer_server(): custom_server()
    {
    }

  protected:

    void virtual handle_get(WiFiClient& client)
    {
      if (source == "/timer_request") send_timer_content(client);
      else                            send_content      (client);
    }

    void send_timer_content(WiFiClient& client)
    {
      send_standard_header(client);
      client.println("<p>The time in milliseconds is " + String(millis()) + "</p>");
    }

    void send_content(WiFiClient& client)
    {
      send_standard_header(client);
      client.println
      (
        "<!DOCTYPE html>"
        "<html>"
        "  <head>"
        "    <title>Timer Server</title>"
        "    <meta name='viewport' content='width=device-width, initial-scale=1'>"
        "  </head>"
        "  <body>"
        "    <div id='display'></div>"
        "    <p>Click buttons to start and stop the timer.</p>"
        "    <button type='button' onclick='start_timer(\"timer_request\", update_display, 500)'>Start</button>"
        "    <button type='button' onclick='stop_timer()'>Stop</button>"
      );
      send_ajax_request_script(client);
      send_timer_script(client);
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
