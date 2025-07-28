#pragma once

#include "custom_server.hpp"

class led_server: public custom_server
{
  public:

    led_server(): custom_server()
    {
    }

  protected:

    const int led_pin = LED_BUILTIN;
    bool      led_on  = false;

    void initialise()
    {
      pinMode(led_pin, OUTPUT);
      enable_led(false);
    }

    void send_content(WiFiClient& client)
    {
      send_standard_header(client);
      client.println
      (
        "<!DOCTYPE html>"
        "<html>"
        "  <head>"
        "    <title>LED Server</title>"
        "    <meta name='viewport' content='width=device-width, initial-scale=1'>"
        "  </head>"
        "  <body>"
        "    <h2>LED Server</h2>"
        "    <p>The LED is " + (led_on ? String("on") : String("off")) + ".</p>"
        "    <form name='on_off' method='post' action='/'>"
        "      <button type='submit' name='action' value='off'>Off</button>"
        "      <button type='submit' name='action' value='on'>On</button>"
        "      <button type='submit' name='action' value='toggle'>Toggle</button>"
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
      String action = get_content_parameter("action");
      if (action == "on")
        enable_led(true);
      else if (action == "off")
        enable_led(false);
      else if (action == "toggle")
        toggle_led();
      send_content(client);
      show_command_information();
    }

    void toggle_led()
    {
      led_on = !led_on;
      digitalWrite(led_pin, led_on ? HIGH : LOW);
    }

    void enable_led(bool on)
    {
      led_on = on;
      digitalWrite(led_pin, led_on ? HIGH : LOW);
    }
};
