# Deney 1 - 7-Segment Sayıcı

**Ogrenci:** Yusuf Kilic  
**Tarih:** Kasim 2025

## Ne Yapiyor

Iki buton ile 7-segment ekranda 0-9 arasi sayi gosteriliyor.

## Malzemeler

- Arduino Uno
- 1 adet 7-segment display (ortak katot)
- 2 adet buton (yukari ve asagi)
- Direncler

## Baglantilar

**7-Segment Pinleri:**
- D0 -> c segmenti
- D1 -> d segmenti  
- D2 -> e segmenti
- D3 -> b segmenti
- D4 -> a segmenti
- D5 -> f segmenti
- D6 -> g segmenti

**Butonlar:**
- D10 -> Yukari butonu
- D11 -> Asagi butonu

## Nasil Calisir

- Yukari butonuna basinca sayi 1 artar
- Asagi butonuna basinca sayi 1 azalir
- 9'dan sonra 0'a, 0'dan once 9'a doner
- Butona basili tutsan bile sadece 1 kere artar/azalir

## Onemli

- digitalWrite, digitalRead, pinMode kullanilmadi
- Port register'lari (DDRB, PORTB, PINB, DDRD, PORTD) kullanildi
- Butonlar icin dahili pull-up direnc aktif
