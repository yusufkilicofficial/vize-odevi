# Birlesik Sistem - 3 Deney Ayni Anda

**Ogrenci:** Yusuf Kilic  
**Tarih:** Kasim 2025

## Ne Yapiyor

Uc deneyi ayni anda calistiriyor. Pinler cakismayacak sekilde ayarlanmis.

## Icerik

- `main/main.ino` - Ana dosya
- `deney1_lib.h` - 7-segment kutuphanesi
- `deney2_lib.h` - LCD ve sensor kutuphanesi  
- `deney3_lib.h` - Servo ve LDR kutuphanesi

## Pin Dagılımı

### Deney 1 (7-Segment + Butonlar)
- D0-D6: 7-segment
- D10: Yukari buton
- D11: Asagi buton

### Deney 2 (LCD + Sensorler)
- D7, D8, A4, A5: LCD data
- D12: LCD RS
- D13: LCD E
- A0: LDR
- A1: DHT11

### Deney 3 (Servo + LDR)
- D9: Servo
- A2: Sol LDR
- A3: Sag LDR

## Onemli Notlar

- Hic bir pin cakismasi yok
- digitalWrite, digitalRead, pinMode kullanilmadi
- Her deney kendi library'sinde calisir
- ADC deney2 ve deney3 tarafindan ortak kullanilir
- Kod tekrari yok, hersey moduler
