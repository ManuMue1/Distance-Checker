# Covid-Distance-Checker
a tool to measure the distance between people and, if necessary, play a warning sound when the distance is less than 1.5m

Me and my Team used Arduino unos, ultrasonic modules and 433 MHz high-frequency modules.
With ultrasonic signals we calculated the distance between two arduinos.
With high-frequncy signals and the logic of the SPI-bus the arduinos communicate. 
When two approach within 1.5m a warning signal sounds.

Via the high frequency signal it is possible to make devices known to each other, so that friends and acquaintances can be approached without a warning signal. This is possible, because each device has a unique ID and these IDs can be stored but also deleted again.
For unknown persons, a warning signal sounds, whereby the distance is checked again every 5 seconds. 
The system works with any number, because with the Master Slave principle the master is selected at random and makes all Arduinos in the vicinity to its slaves, which are then contacted via a Brodcast.



# Operating instructions

## Add / Remove Friend

Push Button connected to D3 (INPUT_PULLUP)
```
|     |
b  t  n
|     |
GND   D3
```
***
ID's will be stored in EEPROM.

Even after flasing new program ID's will still be in EEPROM.

To erase EEPROM set CLEAR_EEPROM_AT_START to true in util.h

***

#### To add a friend:

    1. Push BTN 3 times -> Pairing attempt starts (broadcast with PAIR_CODE getting send)
    2. On other uC hold BTN to confirm pairing
        If pairing was successful sound will play

#### To remove a friend:
    
    1. Connect D4 to Vcc(5V)
    2. Push BTN 3 times -> Unpairing attempt starts (broadcast with UNPAIR_CODE getting send)
    2. On other uC hold BTN to confirm unpairing
        If unpairing was successful sound will play
