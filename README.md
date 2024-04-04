# arduinoafsk
### Description
arduinoafsk is a FM focused afsk encoder and decoder based on an Arduino Nano.
I have used it with my trusty Baofeng, but any FM radio should work as this uses audio in and out.

RTTY is typically seen as AFSK or FSK. FSK (Frequency Shift Keying) actually keys the transmitter on two different frequencies. AFSK (Audio Frequency Shift Keying) sends an audio signal to the transmitter and plays two different tones. The inherit difficulty of AFSK is that the audio signal level needs to be correctly adjusted, where as FSK does not.

### TODOs
- [x] Encode Baudot Code into mark and space tones
- [x] Setup hardware tone generator with Timer1 instead of using `tone()`
- [ ] Decode mark and space tones into Baudot Code
- [ ] Make use of the Push-To-Talk instead of VOX

### Specifications
This program aims to implement basic AFSK RTTY for FM applications. Current interface is through a serial terminal to the Arduino then to the radio's audio interface.
- RTTY 45, 45.45 Baud (60 wpm)
- 2125 Hz mark tone with 170 Hz space offset
- ASCII input and output
- Serial Baud of 115200

### Reference Material
[ASCII,BAUDOT,And The Radio Amateur](https://www.navy-radio.com/rtty/hal/misc/hal-ascii-baudot.pdf)
Really helpful

[RTTY signal parameters](http://www.kloth.net/radio/rtty.php)
Less information than above source but also helpful
