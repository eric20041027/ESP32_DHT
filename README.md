# ESP32 + DHT22 動態物聯網環境監測系統

基於 DHT22、ESP32 邊緣運算與雲端平台（ThingSpeak + Blynk）的端到端環境監測系統。

## 系統架構

```
DHT22 感測 → ESP32 邊緣運算 → ThingSpeak（趨勢分析）
                            → Blynk（即時推播 + 控制）
```

支援雙軌應用模式：
- **室內靜態**：連續上傳，智慧家居聯動
- **戶外動態**：深度休眠節能，可搭載無人機巡航

## 硬體規格

| 元件 | 規格 |
|------|------|
| 微控制器 | ESP32 DevKit V1（雙核 240 MHz，內建 Wi-Fi） |
| 感測器 | DHT22（溫度 ±0.1°C、濕度 ±2%RH） |
| 供電 | 3.3V / USB 5V / 鋰電池 |
| 深度休眠功耗 | < 50 μA（省電 99%） |

## 快速開始

### 1. 安裝 Arduino 函式庫

在 Arduino IDE → Library Manager 安裝：
- `DHT sensor library`（Adafruit）
- `Adafruit Unified Sensor`
- `ThingSpeak`（MathWorks）
- `Blynk`

### 2. 設定雲端平台

參考 [docs/cloud_setup.md](docs/cloud_setup.md) 建立 ThingSpeak Channel 與 Blynk Template。

### 3. 填入設定值

開啟 `src/config.h`，填入 Wi-Fi、Blynk Token、ThingSpeak API Key。

### 4. 接線

參考 [docs/wiring.md](docs/wiring.md)：DHT22 DATA → GPIO4，10kΩ 上拉電阻。

### 5. 燒錄

以 Arduino IDE 開啟 `src/main.ino`，選擇 Board `ESP32 Dev Module`，上傳。

## 目錄結構

```
ESP32_DHT/
├── src/
│   ├── main.ino        # 主程式
│   └── config.h        # 設定檔（Wi-Fi、API Keys、閾值）
├── docs/
│   ├── wiring.md       # 硬體接線說明
│   └── cloud_setup.md  # 雲端平台設定指南
├── project_proposal.md # 學術計畫書
└── README.md
```

## 警戒閾值（可在 config.h 調整）

| 事件 | 預設閾值 |
|------|---------|
| 高溫警報 | > 35°C |
| 低溫警報 | < 5°C |
| 高濕警報 | > 70%RH |
