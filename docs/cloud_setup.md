# 雲端平台設定指南

## 1. ThingSpeak 設定

### 建立 Channel
1. 登入 ThingSpeak → **Channels → New Channel**
2. 填入：
   - **Name**：`ESP32_DHT_Monitor`
   - **Field 1**：`Temperature (°C)`
   - **Field 2**：`Humidity (%RH)`

### 取得 API Key
1. Channel → **API Keys** 頁籤
2. 複製 **Write API Key** → 填入 `config.h` 的 `THINGSPEAK_WRITE_KEY`
3. 複製 **Channel ID** → 填入 `config.h` 的 `THINGSPEAK_CHANNEL_ID`

---

## 2. Blynk 設定

### 建立 Template
1. Blynk Console → **Developer Zone → My Templates → New Template**
2. 填入：Template Name `ESP32_DHT_Monitor`、Hardware `ESP32`、Connection `WiFi`

### 建立 Datastreams

| Virtual Pin | Name | Data Type | Min | Max | Unit |
|-------------|------|-----------|-----|-----|------|
| V1 | Temperature | Double | -40 | 80 | °C |
| V2 | Humidity | Double | 0 | 100 | %RH |
| V3 | Status | String | — | — | — |

### 建立 Events（警報）

| Event Code | 說明 | 通知 |
|------------|------|------|
| `humidity_alert` | 濕度超閾值 | Push Notification |
| `temp_high_alert` | 高溫警報 | Push Notification |
| `temp_low_alert` | 低溫警報 | Push Notification |

### 建立 Dashboard Widget

| Widget | Virtual Pin | 說明 |
|--------|-------------|------|
| Gauge | V1 | 即時溫度（-40 ~ 80°C） |
| Gauge | V2 | 即時濕度（0 ~ 100%RH） |
| SuperChart | V1 + V2 | 歷史趨勢折線圖 |
| Label | V3 | 系統狀態 |

### 取得 Token
1. **Devices → New Device → From Template**，選擇上方 Template
2. 複製 **Auth Token** → 填入 `config.h` 的 `BLYNK_AUTH_TOKEN`
3. 複製 **Template ID** → 填入 `config.h` 的 `BLYNK_TEMPLATE_ID`

---

## 3. 填入 config.h

完成上述步驟後，開啟 `src/config.h`，替換以下佔位字串：

```cpp
#define WIFI_SSID               "你的WiFi名稱"
#define WIFI_PASSWORD           "你的WiFi密碼"
#define BLYNK_TEMPLATE_ID       "TMPxxxxxx"
#define BLYNK_AUTH_TOKEN        "你的Blynk Auth Token"
#define THINGSPEAK_CHANNEL_ID   123456UL
#define THINGSPEAK_WRITE_KEY    "XXXXXXXXXXXXXXXX"
```
