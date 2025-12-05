#include <M5Cardputer.h>
#include <WiFi.h>
#include "splash.h"
#define MAX_CHANNELS 13
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 135

#define CHART_LEFT 10
#define CHART_RIGHT 230
#define CHART_BOTTOM 120
#define CHART_TOP 20
#define MAX_BAR_HEIGHT (CHART_BOTTOM - (CHART_TOP + 15))

#define BAR_SPACING 4

int channel_counts[MAX_CHANNELS + 1] = { 0 };
int total_networks = 0;

uint16_t BACKGROUND = BLACK;
uint16_t BAR_COLOR = CYAN;
uint16_t TEXT_COLOR = WHITE;
uint16_t AXIS_COLOR = LIGHTGREY;

unsigned long lastScan = 0;
const int SCAN_INTERVAL = 1000;


// ----------------------------------------------------
void setup() {
  auto cfg = M5.config();
  M5Cardputer.begin(cfg, true);
  splash();
  M5.Lcd.fillScreen(BACKGROUND);
  M5.Lcd.setTextColor(TEXT_COLOR);
  M5.Lcd.setTextSize(1);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);

  M5.Lcd.drawString("WIFI CHANNEL ANALYZER", 10, 5);
  M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, 20, BACKGROUND);
}

// ----------------------------------------------------
void scan_and_count() {
  // Limpia SOLO área superior de estado
  M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, 10, BACKGROUND);
  // --- calcular batería ---
  float batteryVoltage = (float)M5.Power.getBatteryVoltage() / 1000;
  int batteryPercent = (int)((batteryVoltage - 3.3) / (4.2 - 3.3) * 100);
  //batteryPercent = constrain(batteryPercent, 0, 100);

  // --- mostrar batería ---
  if (batteryPercent >= 100) {
    M5.Lcd.setCursor(162, 0);
  } else {
    M5.Lcd.setCursor(168, 0);
  }

  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.printf("%.2f V (%d%%)", batteryVoltage, batteryPercent);
  M5.Lcd.setCursor(166, 6);
  M5.Lcd.printf("------------");

  M5.Lcd.drawString("Scan...", 5, 0);

  for (int i = 1; i <= MAX_CHANNELS; i++) channel_counts[i] = 0;

  // **Modo estable**
  int n = WiFi.scanNetworks();  // ← ESTE no crashea

  total_networks = n;

  if (n <= 0) return;

  for (int i = 0; i < n; i++) {
    int ch = WiFi.channel(i);
    if (ch >= 1 && ch <= MAX_CHANNELS) channel_counts[ch]++;
  }
  M5.Lcd.fillRect(0, 10, SCREEN_WIDTH, 10, BACKGROUND);
  M5.Lcd.drawString(String("APs: ") + n, 5, 10);  // antes 10
}

// ----------------------------------------------------
void draw_chart() {
  // Borra solo el área del gráfico
  M5.Lcd.fillRect(0, CHART_TOP, SCREEN_WIDTH, SCREEN_HEIGHT - CHART_TOP, BACKGROUND);
  M5.Lcd.drawRect(0, CHART_TOP, SCREEN_WIDTH, SCREEN_HEIGHT - CHART_TOP, RED);
  if (total_networks <= 0) {
    M5.Lcd.drawString("No APs found!", 40, 60);
    return;
  }

  int chart_width = CHART_RIGHT - CHART_LEFT;
  int bar_width = (chart_width - MAX_CHANNELS * BAR_SPACING) / MAX_CHANNELS;
  if (bar_width < 3) bar_width = 3;

  // Eje
  M5.Lcd.drawLine(CHART_LEFT, CHART_BOTTOM, CHART_RIGHT, CHART_BOTTOM, AXIS_COLOR);

  int max_count = 1;
  for (int i = 1; i <= MAX_CHANNELS; i++)
    if (channel_counts[i] > max_count)
      max_count = channel_counts[i];

  for (int i = 1; i <= MAX_CHANNELS; i++) {
    int count = channel_counts[i];
    int bar_h = (count * MAX_BAR_HEIGHT) / max_count;
    int x = CHART_LEFT + (i - 1) * (bar_width + BAR_SPACING);

    // Draw bar
    if (bar_h > 0)
      M5.Lcd.drawRect(x, CHART_BOTTOM - bar_h, bar_width, bar_h, BAR_COLOR);

    // --- NEW: print AP count above bar ---
    if (count > 0) {
      int text_w = (count < 10) ? 6 : (count < 100 ? 12 : 18);
      int text_x = x + (bar_width / 2) - (text_w / 2);
      int text_y = CHART_BOTTOM - bar_h - 10;
      M5.Lcd.drawString(String(count), text_x, text_y);
    }

    // Draw channel number below bar
    int text_width_offset = (i < 10) ? 3 : 6;
    M5.Lcd.drawString(String(i), x + bar_width / 2 - text_width_offset, CHART_BOTTOM + 2);
  }
}

// ----------------------------------------------------
void loop() {
  unsigned long now = millis();
  if (now - lastScan > SCAN_INTERVAL) {
    lastScan = now;
    scan_and_count();
    draw_chart();
  }
}
