#pragma once

// ─── Wi-Fi / Blynk / ThingSpeak ──────────────────────────
// 敏感資訊統一放在 secrets.h（已加入 .gitignore，不會上傳 GitHub）
// secrets.h 由 main.ino 最頂端優先 include，此處不重複引用

// ─── 硬體腳位 ────────────────────────────────────────────
#define DHT_PIN  4
#define DHT_TYPE DHT22

// ─── 警戒閾值 ────────────────────────────────────────────
#define TEMP_HIGH_THRESHOLD  35.0f   // 超過此值觸發高溫警報 (°C)
#define TEMP_LOW_THRESHOLD    5.0f   // 低於此值觸發低溫警報 (°C)
#define HUMID_HIGH_THRESHOLD 70.0f   // 超過此值觸發高濕警報 (%RH)

// ─── 採集與休眠 ──────────────────────────────────────────
#define UPLOAD_INTERVAL_SEC  30      // ThingSpeak 上傳間隔（秒，免費版最短 15 秒）
#define DEEP_SLEEP_SEC       30      // 深度休眠時間（秒，戶外電池模式）

// ─── Blynk Virtual Pins ──────────────────────────────────
#define VPIN_TEMPERATURE  V1
#define VPIN_HUMIDITY     V2
#define VPIN_STATUS       V3
