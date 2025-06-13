#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// WiFi credentials
const char* ssid = "wifi";  //update this with your ssid
const char* password = "12345678";  //update this with your wifi password
const String serverName = "http://<your-server-ip>:5000/api/data";  //update this with your ip

// RGB LED pins
#define RED_PIN 14
#define GREEN_PIN 12
#define BLUE_PIN 13

String scrollText = "";
int16_t textX;
int16_t textY = 30;
unsigned long lastScrollTime = 0;
const unsigned long scrollDelay = 30;

void setup() {
  Serial.begin(115200);

  // Setup of RGB LED pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Setting LED colour at the beginning
  analogWrite(RED_PIN, 255);
  analogWrite(GREEN_PIN, 255);
  analogWrite(BLUE_PIN, 255);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Connecting\nWiFi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("WiFi\nConnected!");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextWrap(false);

  fetchData();  // Get initial data
}

void fetchData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println(payload);

      StaticJsonDocument<512> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        int absentees = doc["absentees"];
        int projects = doc["projects"];
        const char* performer = doc["best_performer"];
        int r = doc["led"]["r"];
        int g = doc["led"]["g"];
        int b = doc["led"]["b"];

        // Update RGB LED
        analogWrite(RED_PIN, r);
        analogWrite(GREEN_PIN, g);
        analogWrite(BLUE_PIN, b);

        scrollText = "Absentees: " + String(absentees) +
                     "  |  Projects: " + String(projects) +
                     "  |  Best Performer: " + String(performer) + "     ";

        int16_t x1, y1;
        uint16_t w, h;
        display.getTextBounds(scrollText, 0, 0, &x1, &y1, &w, &h);
        textX = SCREEN_WIDTH;
      } else {
        scrollText = "JSON Error     ";
        textX = SCREEN_WIDTH;
      }
    } else {
      scrollText = "HTTP Error     ";
      textX = SCREEN_WIDTH;
    }
    http.end();
  } else {
    scrollText = "WiFi Disconnected     ";
    textX = SCREEN_WIDTH;
  }
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastScrollTime >= scrollDelay) {
    lastScrollTime = currentMillis;

    display.clearDisplay();
    display.setCursor(textX, textY);
    display.print(scrollText);
    display.display();

    textX -= 3;

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(scrollText, 0, 0, &x1, &y1, &w, &h);
    if (textX + w < 0) {
      fetchData();
    }
  }
}
