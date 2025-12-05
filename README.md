# 📡 WiFi Channel Analyzer for M5Cardputer

This program scans nearby WiFi networks and visualizes how many access points are operating on each channel (1–13) using the **M5Cardputer**.  
It refreshes every second and displays the results as a clean bar chart, helping identify congestion and select the best channel for WiFi configuration.

---

## ✨ Features

- Real-time WiFi scanning (1-second interval by default).
- Bar graph visualization of AP count per channel.
- Battery voltage and percentage display.
- Lightweight and optimized for M5Cardputer's display.
- Useful tool for on-site spectrum evaluation.

---

## 🧩 Requirements

Hardware:
- **M5Cardputer**

Libraries:
- `M5Cardputer.h`
- `WiFi.h`

---

## ⚙ Installation

1. Clone or download this repository.
2. Open the `.ino` file using **Arduino IDE** or **PlatformIO**.
3. Select **M5Cardputer** as the board.
4. Compile and upload to the device.

---

## ▶ How It Works

Once powered:
- The M5Cardputer continuously scans for nearby WiFi networks.
- It counts how many APs are using channels 1–13.
- A bar graph updates in real time.
- Battery voltage and percentage are shown at the top.

This allows you to:
- Detect crowded WiFi channels,
- Select optimal channels for routers,
- Perform quick and portable spectrum checks.

---

## 🧱 Code Structure Overview

| Function         | Purpose |
|-----------------|----------|
| `scan_and_count()` | Scans networks, extracts channels, counts occurrences |
| `draw_chart()`     | Draws bar graph and channel labels |
| `splash()`         | Intro splash screen |
| `loop()`           | Executes scan + chart updates |

---

## 🧭 Use Cases

- Home/office WiFi optimization.
- Field diagnostics for technicians.
- Educational demonstrations.
- Quick scan before deploying access points.

---

## ✍ Author

github.com/gvrubio

---



