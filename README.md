# LoRa-Based Data Collection Project

This repository contains the files for a LoRa-based data collection project designed to gather environmental data such as atmospheric pressure, particulate matter (PM) readings, temperature, and humidity.

## 📖 Project Report

A detailed report is attached with this repository. In **Chapter 3**, we dive deep into the project, including testing and validation. The report contains the readings from various sensor modules, test outputs at each stage, and our troubleshooting process.

## 🛠️ Hardware & Modules Used

Throughout the project, we **experimented and worked with** a variety of LoRa modules and sensors, though they are **not all included in the final implementation**:

### LoRa Modules:
- **ESP32**
- **Arduino**
- **Heltec ESP32 LoRa Modules V3**
- **Seeed Studio Grove LoRa Modules**
- **Ra02 Modules** (used in the final working prototype)

### Sensors:
- **DHT11** (Temperature and Humidity)
- **BMP180** (Atmospheric Pressure)
- **PMS7003** (not used in the final project)
- **MQ4 and MQ7** (Gas Sensors)
- **DSM501A** (Dust Sensor)

## 📂 Project Structure

The final, tested, and working project files can be found in the folder named `FINAL`.

## 📡 Testing & Results

The project was tested over a distance of **1 km**, where the data from pressure, temperature, humidity sensors, and PM particle sensors was successfully recorded and transmitted to a receiver located 1 km away.

## ⚠️ Notes

- Many intermediate files and different implementation attempts are included.
- We explored communication using two ESP modules.
- Integration of gas sensors and conversion algorithms were also part of the project.
- Many files and alternative implementations are lost to time, sadly.

## 🎓 About

This was a college project, and through this journey, I gained substantial hands-on experience with LoRa technology, sensor integration, and wireless data communication.

---

Feel free to explore the code and report
