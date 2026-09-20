# ADS1256_driver
Simple driver for the ADS1256 Devboard.

## RP2350-One: grüne Onboard-LED

`examples/led_green.c` ist ein eigenständiges C-Beispiel für die WS2812-LED
an GPIO16. Es verwendet das PIO-Programm des installierten Pico SDK 2.3.1
und sendet RGB-Daten mit einer grünen Helligkeit von 32/255.

```sh
cmake -S . -B build
cmake --build build --target led_green
~/.pico-sdk/picotool/2.3.1/picotool/picotool load build/led_green.uf2 -fx
```

Alternativ `build/led_green.uf2` im BOOTSEL-Modus auf das Board kopieren.
USB-stdio bleibt aktiv, damit nachfolgendes Flashen mit `picotool -f` möglich ist.
Das Beispiel wurde kompiliert, aber noch nicht auf Hardware getestet.

GPIO16 ist im ADC-Projekt aktuell als MISO eingetragen. Für diesen LED-Test
darf dort kein externer MISO-Ausgang angeschlossen sein. Für eine spätere
Integration muss die SPI-Pinbelegung angepasst werden.
