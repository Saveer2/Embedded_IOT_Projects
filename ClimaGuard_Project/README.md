** ClimaGuard — Arduino Temperature & Humidity Monitor with Alerts ** 

ClimaGuard is an Arduino-based project that continuously monitors temperature and humidity using a DHT11 sensor, displays values on an OLED screen, and provides visual + audio alerts with an RGB LED module, active buzzer, and passive buzzer.

** Components = 

1.  Arduino UNO
2.  DHT11 (temperature & humidity sensor)
3.  SSD1306 OLED display (128×64, I²C)
4.  RGB LED module (common cathode)
5.  3 × 220 Ω resistors (for R, G, B pins)
6.  Active buzzer
7.  Passive buzzer
8.  Breadboard + jumper wires

** Features

1.  OLED Display: Live temperature (°C) and humidity (%) values.
2.  RGB LED Alerts:
🔵 Blue → Temperature below 30 °C (Normal)
🟢 Green → Temperature between 30–35 °C (Warning)
🔴 Red → Temperature above 35 °C (Danger)
3.  Active Buzzer Alerts:
  * One short beep every 10 sec in Warning mode.
  * One short beep every 1 sec in Danger mode.
4.  Passive Buzzer Alerts: Plays an emergency melody if temperature exceeds 35 °C.
