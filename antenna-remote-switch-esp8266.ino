#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


const char* ssid = "wifiname";            // WIFI NAME
const char* password = "wifipass";      // WifiPass
const char* hostname = "ESP8266antenne";  // hostname

ESP8266WebServer server(80);


const int pins[] = { 5, 4, 13, 12, 14 };  // GPIO 5 ant 1, 4 ant 2 etc
const int numPins = sizeof(pins) / sizeof(pins[0]);

void setup() {
  Serial.begin(115200);


  for (int i = 0; i < numPins; i++) {
    pinMode(pins[i], INPUT);
  }
  WiFi.hostname(hostname);

  WiFi.begin(ssid, password);
  Serial.println("\nConnecting...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Hostname: ");
  Serial.println(hostname);

  server.on("/", handleRoot);

  server.begin();
  Serial.println("Server HTTP avviato!");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html lang=\"en\"><head>";
  html += "<meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "<meta http-equiv=\"refresh\" content=\"60\">";  // 60 second refresh
  html += "<title>Antenne esp8266</title>";
  html += "</head><body>";
  html += "<h1>Controller</h1><ul>";

  for (int i = 0; i < numPins; i++) {
    int state = digitalRead(pins[i]);
    html += "<li>ANTENNA " + String(i + 1) + ": " + (state == HIGH ? "ON" : "OFF") + "</li>";
  }

  html += "</ul></body></html>";
  server.send(200, "text/html", html);
}
