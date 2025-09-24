# Notes for Configuration in MPLAB X IDE
## 9/24
I've set it up to use avr-gcc instead of XC8 just because XC8 is proprietary and I'd like the final product to run in the Arduino IDE if possible. 
That should make it friendlier to being open source, should make a generic avr-gcc make file and avrdude script later. 
But I haven't used the avr-gcc from MicroChip, rather the avr-gcc that ships with Arduino IDE (version 2.3.5). 
Not 100% sure which avr-gcc version this is but MPLAB X read it as 1.0.0 (I don't think that's correct). 

Found how to change settings (like clock speed) it's `Window > Target Memory Views > Configuration Bits` or just `Production > Configuration Bits`.

Likely going to use https://www.nongnu.org/avr-libc/user-manual/index.html