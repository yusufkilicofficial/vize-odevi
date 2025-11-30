# Deney 2 - LCD Ekran ve Sensorler

**Ogrenci:** Yusuf Kilic  
**Tarih:** Kasim 2025

## Ne Yapiyor

LCD ekranda sicaklik, nem ve isik seviyesini gosteriyor.

## Malzemeler

- Arduino Uno
- 16x2 LCD ekran
- DHT11 sicaklik/nem sensoru
- LDR isik sensoru
- 10k direnç (LDR icin)
- potansiyometre

## Baglantilar

**LCD Pinleri:**
- RS -> D12
- E -> D13
- D4 -> D7
- D5 -> D8
- D6 -> A4
- D7 -> A5

**Sensorler:**
- LDR -> A0
- DHT11 -> A1

## Ekranda Ne Gorunur

Satir 1: `T:25C N:45%`  
Satir 2: `L:512`

T = sicaklik, N = nem, L = isik

## Onemli

- LCD 4-bit modda elle suruluyor
- digitalWrite, digitalRead, pinMode kullanilmadi
- ADC register'lari (ADMUX, ADCSRA) ile LDR okunuyor
- DHT11 elle bit-banging yontemiyle okunuyor
- Port register'lari kullanildi
