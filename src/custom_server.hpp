#pragma once

#include <WiFi.h>

class custom_server
{
  public:
  
    custom_server()
    {
    }

    void start(String server_name, String ssid, String password)
    {
      this->ssid     = ssid;
      this->password = password;
      initialise();
      while (true)
      {
        if (!WiFi.status() != WL_CONNECTED) connect();
        const int server_port = 80;
        WiFiServer server(server_port);
        this->server_name = server_name;
        WiFi.hostname(server_name);
        server.begin();
        while (WiFi.status() == WL_CONNECTED) serve_clients(server);
      }
    }

  protected:

    String ssid        = "";
    String password    = "";
    String server_name = "";
    String command     = "";
    String source      = "";
    String parameters  = "";
    String content     = "";

    void virtual initialise()
    {
    }

    void virtual before_connecting()
    {
      Serial.println("Connecting to Network");
    }

    void virtual connecting()
    {
      Serial.print(".");
    }

    void virtual connected(IPAddress ip)
    {
      Serial.println();
      Serial.println("Network connected");
      Serial.print("Server URL: http://"); Serial.println(ip); 
    }
    
    void virtual parse_header_line(String header_line)
    {
    }

    void virtual handle_get(WiFiClient& client)
    {
      send_content(client);
    }

    void virtual handle_post(WiFiClient& client)
    {
      send_content(client);
    }

    void virtual handle_head(WiFiClient& client)
    {
      send_null_response(client);
    }

    void virtual handle_put(WiFiClient& client)
    {
      send_null_response(client);
    }

    void virtual handle_delete(WiFiClient& client)
    {
      send_null_response(client);
    }
    
    void virtual send_content(WiFiClient& client)
    {
      send_standard_header(client);
      client.println
      (
        "<!DOCTYPE html>"
        "<html>"
        "  <head>"
        "    <title>" + server_name + "</title>"
        "    <meta name='viewport' content='width=device-width, initial-scale=1'>"
        "  </head>"
        "  <body>"
        "    <h2>" + server_name + "</h2>"
        "  </body>"
        "</html>"
      );
    }
    
    void send_standard_header(WiFiClient& client)
    {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
    }

    void send_prevent_reposting_on_page_refresh_script(WiFiClient& client)
    {
      client.println
      (
        "<script>"
        "  document.addEventListener('DOMContentLoaded', function() "
        "  {"
        "    if (window.history.replaceState) "
        "      window.history.replaceState({}, null, window.location.href);"
        "  });"
        "</script>"
      );
    }

    void send_ajax_request_script(WiFiClient& client)
    {
      client.println
      (
        "<script>"
        "  function send_ajax_request(source, action)"
        "  {"
        "    var xhttp = new XMLHttpRequest();"
        "    xhttp.onreadystatechange = function()"
        "    {"
        "      if (this.readyState == 4 && this.status == 200) action(this.responseText);"
        "    };"
        "    xhttp.open('GET', source, true);"
        "    xhttp.send();"
        "  }"
        "</script>"
      );
    }

    void send_timer_script(WiFiClient& client)
    {
      client.println
      (
        "<script>"
        "  var timer_source;"
        "  var timer_action;"
        "  var ajax_timer;"
        "  function start_timer(source, action, interval)"
        "  {"
        "    timer_source = source;"
        "    timer_action = action;"
        "    ajax_timer   = window.setInterval(ajax_request, interval);"
        "  }"
        "  function stop_timer()"
        "  {"
        "    window.clearInterval(ajax_timer);"
        "  }"
        "  function ajax_request()"
        "  {"
        "    send_ajax_request(timer_source, timer_action);"
        "  }"
        "</script>"
      );
    }
    
    void send_null_response(WiFiClient& client)
    {
      client.println("HTTP/1.1 204 No Content");
      client.println();
    }
    
    void send_not_found_response(WiFiClient& client)
    {
      client.println("HTTP/1.1 404 Not Found");
      client.println("Content-Type: text/html");
      client.println();
      client.println
      (
        "<html>"
        "  <head>"
        "   <title>Page Not Found</title>"
        "  </head>"
        "  <body>"
        "    <h1>404 Page Not Found!</h1>"
        "  </body>"
        "</html>"
      );
      client.println();
    }
    
    String get_header_line_description(String header_line)
    {
      int index = header_line.indexOf(": ");
      return header_line.substring(0, index);
    }
    
    String get_header_line_value(String header_line)
    {
      int index = header_line.indexOf(": ");
      return header_line.substring(index + 2);
    }

    String get_parameter(String name)
    {
      String all_parameters = "&" + parameters;
      String pattern        = "&" + name + "=";
      int    index          = all_parameters.indexOf(pattern);
      if (index >= 0)
      {
        String value = all_parameters.substring(index + pattern.length());
        int    eop   = value.indexOf("&");
        if (eop >= 0) value = value.substring(0, eop);
        return value;
      }
      else return "";
    }
    
    String virtual get_content(WiFiClient& client, int content_length)
    {
      String content = read_chars(client, content_length);
      return content;
    }

    String get_content_parameter(String name)
    {
      String all_parameters = "&" + content;
      String pattern        = "&" + name + "=";
      int    index          = all_parameters.indexOf(pattern);
      if (index >= 0)
      {
        String value = all_parameters.substring(index + pattern.length());
        int    eop   = value.indexOf("&");
        if (eop >= 0) value = value.substring(0, eop);
        return value;
      }
      else return "";
    }
        
    String decode_url(String encoded)
    { 
      String decoded = "";
      for (int i = 0; i < encoded.length(); i = i + 1)
      {
        char ch = encoded[i];
        if (ch == '%') 
        {
          int d1  = hex_digit_value(encoded[i + 1]);
          int d2  = hex_digit_value(encoded[i + 2]);
          i       = i + 2;
          ch      = (char)(16 * d1 + d2);
          decoded = decoded +  ch;
        } 
        else if (ch == '+') decoded = decoded + ' ';  
        else                decoded = decoded + ch;  
        yield();
      }
     return decoded;
    }
        
    String encode_url(String plain_text)
    { 
      String encoded = "";
      for (int i = 0; i < plain_text.length(); i = i + 1)
      {
        char ch = plain_text[i];
        if (is_special_char(ch)) 
        {
          char d1  = hex_char((int)ch / 16);
          char d2  = hex_char((int)ch % 16);
          encoded = encoded +  "%" + d1 + d2;
        } 
        else if (ch == ' ') encoded = encoded + '+';  
        else                encoded = encoded + ch;  
      }
     return encoded;
    }

    void virtual show_command_information()
    {
      Serial.print("Command: "   ); Serial.println(command   );
      Serial.print("Source: "    ); Serial.println(source    );
      Serial.print("Parameters: "); Serial.println(parameters);
      Serial.print("Content: "   ); Serial.println(content );
    }

  private:
  
    int content_length = 0;

    void connect()
    {
      before_connecting();
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED)
      {
        connecting();
        delay(500);
      }
      connected(WiFi.localIP());
    }

    void serve_clients(WiFiServer& server) 
    {
      command        = ""; 
      content_length = 0;
      content        = "";
      WiFiClient client = server.accept();
      if (client.connected() &&  client.available() > 0) 
      {   
        command   = read_line(client, "Command: ");
        parse_command();
        if (command != "") process_command(client, command);
      }
      else delay(100);
    }

    void process_command(WiFiClient& client, String command)
    {    
      String line = read_line(client, "Header: ");
      while (line != "" && client.connected() && client.available() > 0)
      {
        if (line.indexOf("Content-Length: ") >= 0)
          get_content_length(line);
        else
          parse_header_line(line);
        line = read_line(client, "Header: ");
        yield();
      }
      if (client.connected() && client.available() > 0 && content_length > 0) content = get_content(client, content_length);
      send_response(client);
    }
    
    void get_content_length(String header_line)
    {
      int index = header_line.indexOf(": ");
      String content_length_string = header_line.substring(index + 2);
      content_length        = content_length_string.toInt();
    }
    
    void send_response(WiFiClient& client)
    {
      if (command.startsWith("GET "))
        handle_get(client);
      else if (command.startsWith("POST "))
        handle_post(client);
      else if (command.startsWith("HEAD "))
        handle_head(client);
      else if (command.startsWith("PUT "))
        handle_put(client);
      else if (command.startsWith("DELETE "))
        handle_delete(client);
      else
        send_null_response(client);
      client.println();
      client.stop();
    }
    
    String read_line(WiFiClient& client, String kind)
    {
      String line = "";
      if (client.connected() && client.available() > 0)
      {
        char ch = client.read();
        while (ch != '\r'  && client.connected() && client.available() > 0)
        {
          if (ch != '\n') line = line + ch;
          ch = client.read();
        }
      }
      return line;
    } 
    
    String read_chars(WiFiClient& client, int chars)
    {
      String line = "";
      for (int i = 1; i <= chars; i = i + 1)
      {
        char ch = client.read();
        while (ch <= ' ') ch = client.read();
        line = line + ch;
        yield();
      }
      return line;
    } 

    void parse_command()
    {
      if (command.indexOf("?") >= 0) 
      {
        source     = enclosed_string(command, " ", "?");
        parameters = enclosed_string(command, "?", " HTTP");
      }
      else 
      {
        source     = enclosed_string(command, " ", " HTTP");
        parameters = "";
      }
    }

    int hex_digit_value(char ch)
    {
      if      ('0' <= ch && ch <= '9') return (int)(ch - '0'     );
      else if ('A' <= ch && ch <= 'F') return (int)(ch - 'A' + 10);
      else if ('a' <= ch && ch <= 'z') return (int)(ch - 'a' + 10);
      else                             return 0;
    }

    bool is_special_char(char ch)
    {
      if      ('0' <= ch && ch <= '9') return false;
      else if ('A' <= ch && ch <= 'Z') return false;
      else if ('a' <= ch && ch <= 'z') return false;
      else                             return ch != ' ';
    }

    char hex_char(int d)
    {
      int zero = (int)'0';
      int a    = (int)'A';
      return d < 10 ? zero + d : a + d - 10;
    }

    String enclosed_string(String input, String start_symbol, String end_symbol)
    {
      int from_index = input.indexOf(start_symbol) + start_symbol.length();
      int to_index   = input.indexOf(end_symbol, from_index);
      return input.substring(from_index, to_index);
    }
};
