#pragma once

#include "custom_server.hpp"

class multi_page_server: public custom_server
{
  public:

    multi_page_server(): custom_server()
    {
    }

  protected:

    void handle_get(WiFiClient& client)
    {
      if      (source == "/"     ) send_content(client);
      else if (source == "/page1") send_page(client, 1);
      else if (source == "/page2") send_page(client, 2);
      else if (source == "/page3") send_page(client, 3);
      else                         send_not_found_response(client);
    }

    void send_content(WiFiClient& client)
    {
      send_standard_header(client);
      client.println
      (
        "<!DOCTYPE html>"
        "<html>"
        "  <head>"
        "    <title>Multi-page Server</title>"
        "    <meta name='viewport' content='width=device-width, initial-scale=1'>"
        "  </head>"
        "  <body>"
        "    <h2>Mult-page Server</h2>"
        "    <p><a href='page1'>Page 1</a></p>"
        "    <p><a href='page2'>Page 2</a></p>"
        "    <p><a href='page3'>Page 3</a></p>"
        "    <p><a href='https://start.duckduckgo.com/'>DuckDuckGo</a></p>"
        "  </body>"
        "</html>"
      );
    }

    void send_page(WiFiClient& client, int n)
    {
      send_standard_header(client);
      client.println
      (
        "<!DOCTYPE html>"
        "<html>"
        "  <head>"
        "    <title>Page " + String(n) + "</title>"
        "    <meta name='viewport' content='width=device-width, initial-scale=1'>"
        "  </head>"
        "  <body>"
        "   <h2>Page " + String(n) + "</h2>"
        "    <p><a href='/'>Return to index</a></p>"
        "  </body>"
        "</html>"
      );
    }
};
