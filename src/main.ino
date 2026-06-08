/*
 * ESP32 + DHT22 環境監測系統
 * 架構：DHT22 → ESP32 邊緣運算 → ThingSpeak（趨勢分析）+ Blynk（即時控制）
 *
 * 函式庫需求（Arduino Library Manager 安裝）：
 *   - DHT sensor library      (Adafruit)
 *   - Adafruit Unified Sensor (Adafruit)
 *   - ThingSpeak              (MathWorks)
 *   - Blynk                   (Blynk)
 */

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>
#include <BlynkSimpleEsp32.h>
#include "config.h"

// ─── 全域物件 ────────────────────────────────────────────
DHT        dht(DHT_PIN, DHT_TYPE);
WiFiClient wifiClient;
BlynkTimer timer;

/*
 * SLEEP_MODE_PIN（GPIO0）控制運作模式：
 *   HIGH（跳線不接）→ 連續上傳模式，適合室內市電場景
 *   LOW （跳線接地）→ 深度休眠模式，適合戶外電池場景
 */
#define SLEEP_MODE_PIN 0

// ─── 函式前宣告 ──────────────────────────────────────────
void connectWiFi();
void readAndUpload();
void checkAlerts(float temp, float humid);
void updateBlynk(float temp, float humid);
bool uploadThingSpeak(float temp, float humid);

// ═════════════════════════════════════════════════════════
void setup() {
    Serial.begin(115200);
    pinMode(SLEEP_MODE_PIN, INPUT_PULLUP);
    dht.begin();

    connectWiFi();
    Blynk.config(BLYNK_AUTH_TOKEN);
    Blynk.connect(3000);
    ThingSpeak.begin(wifiClient);

    Blynk.virtualWrite(VPIN_STATUS, "System Online");

    if (digitalRead(SLEEP_MODE_PIN) == HIGH) {
        timer.setInterval(UPLOAD_INTERVAL_SEC * 1000L, readAndUpload);
    }
}

// ═════════════════════════════════════════════════════════
void loop() {
    Blynk.run();

    if (digitalRead(SLEEP_MODE_PIN) == HIGH) {
        timer.run();
    } else {
        readAndUpload();
        Serial.println("Entering deep sleep...");
        esp_sleep_enable_timer_wakeup((uint64_t)DEEP_SLEEP_SEC * 1000000ULL);
        esp_deep_sleep_start();
    }
}

// ─── Wi-Fi 連線（含重試機制）────────────────────────────
void connectWiFi() {
    Serial.printf("Connecting to Wi-Fi: %s\n", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 20) {
        delay(500);
        Serial.print(".");
        retry++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("\nConnected. IP: %s\n", WiFi.localIP().toString().c_str());
    } else {
        Serial.println("\nWi-Fi failed. Rebooting...");
        ESP.restart();
    }
}

// ─── 主要採集 + 上傳流程 ─────────────────────────────────
void readAndUpload() {
    float temp  = dht.readTemperature();
    float humid = dht.readHumidity();

    if (isnan(temp) || isnan(humid)) {
        Serial.println("[ERROR] DHT22 read failed");
        Blynk.virtualWrite(VPIN_STATUS, "Sensor Error");
        return;
    }

    Serial.printf("[DATA] Temp: %.1f°C  Humid: %.1f%%RH\n", temp, humid);

    updateBlynk(temp, humid);
    bool ok = uploadThingSpeak(temp, humid);
    checkAlerts(temp, humid);

    Blynk.virtualWrite(VPIN_STATUS, ok ? "OK" : "Upload Failed");
}

// ─── 更新 Blynk 儀表板 ───────────────────────────────────
void updateBlynk(float temp, float humid) {
    Blynk.virtualWrite(VPIN_TEMPERATURE, temp);
    Blynk.virtualWrite(VPIN_HUMIDITY,    humid);
}

// ─── 上傳 ThingSpeak ─────────────────────────────────────
bool uploadThingSpeak(float temp, float humid) {
    ThingSpeak.setField(1, temp);
    ThingSpeak.setField(2, humid);
    int code = ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, THINGSPEAK_WRITE_KEY);

    if (code == 200) {
        Serial.println("[ThingSpeak] Upload OK");
        return true;
    }
    Serial.printf("[ThingSpeak] Upload failed, code: %d\n", code);
    return false;
}

// ─── 自動化閾值警報 ──────────────────────────────────────
void checkAlerts(float temp, float humid) {
    if (humid > HUMID_HIGH_THRESHOLD) {
        Blynk.logEvent("humidity_alert",
            String("濕度異常攀升！目前值：") + humid + " %RH");
    }
    if (temp > TEMP_HIGH_THRESHOLD) {
        Blynk.logEvent("temp_high_alert",
            String("高溫警告！目前值：") + temp + " °C");
    }
    if (temp < TEMP_LOW_THRESHOLD) {
        Blynk.logEvent("temp_low_alert",
            String("低溫警告！目前值：") + temp + " °C");
    }
}
