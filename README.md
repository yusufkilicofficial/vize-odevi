# Arduino Mikrodenetleyici Deney Projesi

**Ogrenci:** Yusuf Kilic  
**Numara:** 360522014
**Ders:** Mikrodenetleyiciler  
**Tarih:** Kasim 2025

## Proje Hakkinda

Bu proje, Arduino dersindeki vize odevi icin hazirlanmistir. Ucunu deney ayri ayri ve birlikte calisabilecek sekilde yapilmistir.
vize-odev-vide.mp4 içerisinde 3 deneyinde videosu bulunmaktadır. Video içerisinde once 7 segmentli display deneyi, daha sonra lcd ve sensörler deneyi , daha sonra ldr kontrollü servo deneyi gösterilmiştir. Videoyu indirmek için
## Klasor Yapisi

```
deneyler/
├── deney1/      # 7-segment sayici
├── deney2/    # LCD ekran ve sensorler
├── deney3/     # Servo ile isik takibi
└── birlesik_sistem/      # 3 deney birlikte
```

## Deneyler

### Deney 1: 7-Segment Sayici
Butonlarla kontrol edilen 0-9 sayici. Port register'lari ile yapildi.

### Deney 2: LCD ve Sensorler  
LCD ekranda sicaklik, nem ve isik gosterimi. ADC register'lari ve manuel LCD surucu kullanildi.

### Deney 3: Servo ile Isik Takibi
LDR sensorleri ile isigi takip eden servo motor. Timer1 PWM register'lari kullanildi.

### Birlesik Sistem
Ucunu deneyi ayni anda calistiran sistem. Pin cakismasi yok, moduler yapi.

## Onemli Kurallar

Her projede sunlar uygulanmistir:
- digitalWrite, digitalRead, pinMode kullanilmadi
- Port register'lari (DDRx, PORTx, PINx) kullanildi
- analogRead yerine ADC register'lari kullanildi
- analogWrite yerine Timer PWM register'lari kullanildi
- Servo.h, LiquidCrystal.h gibi kutuphaneler kullanilmadi

## Kullanılan Modüller

- Arduino Uno
- 7-segment display
- 16x2 LCD ekran
- DHT11 sensor
- 3 adet LDR
- Servo motor (SG90)
- Butonlar ve direncler
- Kapasitor (mümkünse 0.22f) / veya servo için ayrı enerji kaynağı
- Potansiyometre

## Yardimci Bilgiler

Her deneyin kendi klasorunde detayli README.md dosyasi var. Baglanti semasi ve aciklamalar orada.
