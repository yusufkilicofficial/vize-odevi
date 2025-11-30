# Deney 3 - Servo Motor ile Isik Takibi

**Ogrenci:** Yusuf Kilic  
**Tarih:** Kasim 2025

## Ne Yapiyor

Iki LDR ile isigi algilayip servo motor isiga dogru donuyor.

## Malzemeler

- Arduino Uno
- 1 adet servo motor (SG90)
- 2 adet LDR
- 2 adet 10k direnç
- Kapasitor (mümkünse 0.22f) / veya servo için ayrı enerji kaynağı

## Baglantilar

**Servo Motor:**
- Sari kablo (sinyal) -> D9
- Kirmizi (+5V) -> 5V
- Kahverengi (GND) -> GND

**LDR'ler:**
- Sol LDR -> A2
- Sag LDR -> A3

## Nasil Calisir

- Sol taraftan isik gelirse servo sola doner
- Sag taraftan isik gelirse servo saga doner  
- Isik ortadaysa servo ortada durur
- LDR'ler arasina kucuk bir engel koy ki ayirt edebilsin

## Onemli

- Servo.h kutuphanesi kullanilmadi
- Timer1 register'lari (TCCR1A, TCCR1B, ICR1, OCR1A) kullanildi
- 50Hz PWM sinyali donanımsal olarak uretiliyor
- ADC ile LDR degerleri okunuyor
- analogWrite kullanilmadi
