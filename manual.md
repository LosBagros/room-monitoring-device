# Room Monitoring Device Manual

Link to [Czech version](manual_cz.md)

This document provides setup instructions and usage guidelines for the **Room Monitoring Device**, which continuously measures **CO₂ concentration**, **temperature**, and **humidity** in indoor environments.

## ⚠️ Important Safety Notice

> **Do not leave the device unattended.**
>
> Although the device has been stable and reliable in practice, **it is a custom electronic device and there is no absolute guarantee against hardware failure or fire risk**.
>
> Always place the device on a **non-flammable surface**, keep it **away from flammable materials**, and **periodically monitor its operation**.
>
> **Use only 5V power adapters**

## Setup Instructions

### 1. Power Supply

- Use a stable 5V USB-C power source (standard phone charger with USB-C output).
- Ensure uninterrupted power for continuous monitoring and reliable data transmission.

### 2. Wi-Fi Connectivity

- Place the device close to your Wi-Fi router to ensure strong and stable signal strength.
- Ensure your Wi-Fi network operates on the 2.4 GHz band. The device does not support 5 GHz Wi-Fi networks.
- When first powered on, the device will create a Wi-Fi hotspot named `RoomMonitorAP` with password `12345678`.
- Open captive portal by connecting to the hotspot and visiting `http://192.168.4.1` in a web browser.
- Enter your Wi-Fi SSID and password to connect the device to your Wi-Fi network.

### 3. Placement Recommendations

- Position the device at typical room height, away from:
  - Direct sunlight
  - Heating/cooling vents
  - Windows and doors
- Avoid enclosing the device in tight spaces to allow free airflow around the sensors.

## Live Dashboard

[**Dashboard link**](https://iot.bagros.eu/d/be7hw0wxuy1vkc/co2?orgId=1&from=now-3h&to=now&timezone=browser&kiosk)

The dashboard provides:

- **CO₂ concentration (ppm)**
- **Room temperature (°C)**
- **Relative humidity (%)**

## LED Indicator

| LED Color                | CO₂ Level (ppm)   | Action                             |
| ------------------------ | ----------------- | ---------------------------------- |
| 🟢 Good                  | `< 1000 ppm`      | No action needed                   |
| 🟡 Moderate              | `1000 - 2000 ppm` | Increase ventilation               |
| 🔴 Poor                  | `> 2000 ppm`      | Immediately improve ventilation    |
| 🔵 Sensor not ready      | `N/A`             | Waiting for CO₂ sensor to warmup   |
| ⚪ Not connected to wifi | `N/A`             | Wifi is not configured / connected |
