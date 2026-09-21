Dies ist die Firmware für einen LiPo - Pouchzellen Prüfstand.

Verwendet wird ein RP2350 sowie ein ADS1256 und 6x NTCLE213E3.

Der ads_driver.c stellt eine Struct mit allen Channel Spannungen bereit. Dieser Treiber ist komplett unabhängig vom Messaufbau und funktioniert universell.

volt_temp_calc.c wandelt die Spannunen mithilfe der Beta-Näherung in Temperaturen um.

Da sowohl Temperaturen als auch die Spannung der Zelle selbst geloggt werden müssen, 
sampled man zuerst alle Kanäle mit ADS_GET_VOLTAGES, und Konvertiert dann ausgewählte Werte in Temperaturen.
