#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266Ping.h>
#include <Ticker.h>

#include "credentials.h"
#include "settings.h"
#include "prototypes.h"

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);

FASTLED_USING_NAMESPACE
CRGB leds[NUM_PIXELS];

Ticker ticker;

void setup() { 
  Serial.begin(115200);
  delay(300);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_PIXELS);

  Serial.println();
  Serial.println("Start ticker...");
  Serial.println();
  ticker.attach(1, loopCount);

  Serial.println("");
  startWIFI();
  startWebserver();
  blackOut();
}

void loop() { 
  //(re)connect wifi if not connected
  if(WiFiMulti.run() != WL_CONNECTED) {
    delay(1);
    startWIFI();
    return;
  }

  handlePing();
  server.handleClient();
  handleAnimation();
}

/**
   Start or reconnect the wifi
   by switching into an infinite loop
   as long as the connection is broken
*/
void startWIFI() {
  int loopcnt = 0;
  Serial.println("---");
  WiFi.mode(WIFI_STA);
  Serial.println("Connecting WIFI.");
  Serial.println("(Re)Connecting to Wifi-Network with following credentials:");
  Serial.print("SSID: ");
  //Serial.println(WifiCredentials[0].c_str());
  Serial.print("Key: ");
  //Serial.println(WifiCredentials[1].c_str());
  WiFiMulti.addAP(WifiCredentials[0].c_str(), WifiCredentials[1].c_str());

  while (WiFiMulti.run() != WL_CONNECTED) {
    loopcnt++;
    if(loopcnt < 10) {
      Serial.print(".");
    }
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address:");
  Serial.println(WiFi.localIP().toString());
}

/**
   Start webserver for handling
   incoming requests
*/
void startWebserver() {
  Serial.println("Starting HTTP-Server...");
  Serial.println("-- Registering routes.");
  server.on("/signal", HTTP_GET, []() {
    String r = server.arg("r");
    String g = server.arg("g");
    String b = server.arg("b");
    String t = server.arg("t");
    String s = server.arg("s");
    startAnimation(r.toInt(), g.toInt(), b.toInt(), t.toInt(), s.toInt());
  });

  server.on("/blackout", HTTP_GET, []() {
    endAnimation();
  });
  server.onNotFound(handleRequestNotFound);
  Serial.println("-- Launching server ...");
  server.begin();
  Serial.println("-- DONE.");
}

/**
   Handles pinging (send alive)
   to server
*/
void handlePing() {
  if(pingCnt > handlePingSecs) {
    Serial.println("Start pinging.");
    bool result = Ping.ping("www.google.com");

    if(!result) {
      Serial.println("Ping failed!");
    }

    Serial.println("[HTTP] end ping-pong.");
    Serial.println("---");
    pingCnt = 0;
  }
}

/**
   Tick, tick, tick
*/
void loopCount() {
  loopCnt++;
  pingCnt++;
}

/**
   Start animation on request
*/
void startAnimation(uint8_t r, uint8_t g, uint8_t b, uint8_t times, uint8_t speedMs) {
  server.send(200, "text/plain", "Start fading now.");
  rCurrent = r;
  gCurrent = g;
  bCurrent = b;
  ledBrightnessCurrent = 0;
  
  animationRepeats = animationRepeatsFallback;
  if(times > 0) animationRepeats = times;
  
  animationSpeed = animationSpeedFallback;
  if(speedMs > 0) animationSpeed = speedMs;
  animationActive = true;

  FastLED.setBrightness(0);
  Serial.println("Start animation!");
}

/**
 * Stop animation on request
 */
void endAnimation() {
  server.send(200, "text/plain", "No animation anymore. :(");
  blackOut();
}

/**
 * Reset animation
 */
void blackOut() {
  fill_solid(leds, NUM_PIXELS, CRGB::Black);
  FastLED.show();
  animationActive = false;
  animationRepeats = 0;
  animationSpeed = 0;
  animationRepeatCnt = 0;
  fadeDirection = 1;
  
  Serial.println("BLACKOUT animation!");
}

/**
 * Play animation
 */
void handleAnimation() {
  if(animationActive) {
    if(millis() % animationSpeed == 0) {
      FastLED.setBrightness(ledBrightnessCurrent);
      fill_solid(leds, NUM_PIXELS, CRGB(rCurrent,gCurrent,bCurrent));
      FastLED.show();

      if(fadeDirection > 0) {
        ledBrightnessCurrent++;
        if(ledBrightnessCurrent > LED_BRIGHTNESS) {
          fadeDirection = 0;
        }
      }else{
        ledBrightnessCurrent--;
        if(ledBrightnessCurrent <= 0) {
          fadeDirection = 1;
          animationRepeatCnt++;
        }
      }
      
      if(animationRepeatCnt >= animationRepeats) {
        blackOut();
      }
    }
  }
}

/**
   Unknown Route
   Send teapot.
*/
void handleRequestNotFound() {
  server.send(418, "text/plain", "I'm the stream marker. I mark streams.");
}
