# Aquordion
MTL project

COLOUR SENSOR

- LED disabled by connecting LED pin to GND

PIEZO SENSORS

- Pulldown required (1M)
- main.pd is kinda broken
- Comport and pduino must open different serial ports
- Pinmode needs to be analog

/etc/rc.local starts pd at boot. Pd will freak out if input devices are enabled
