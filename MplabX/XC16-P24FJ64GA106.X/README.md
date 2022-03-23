#

<!--
# PIC16F1778 - Firmware Debug.

## Code.

- [PIC16F1778 - XC8 Firmware Debug.](https://github.com/tronixio/robot-tbot/blob/main/MplabX/XC8-P16F1778-DEBUG.X/main.c)

## Terminal.

<p align="center">
<img alt="EUSART TX" src="https://github.com/tronixio/robot-tbot/blob/main/MplabX/extras/eusart-0.png">
</p>

## MPLABX Linker Configuration.

- PIC-AS Linker > Custom linker options:
  - For Configuration & PWM: `-preset_vec=0000h, -pcinit=0005h, -pstringtext=3FC0h`

![MPLABX Configuration](https://github.com/tronixio/robot-tbot/blob/main/Code/extras/configuration-1.png)
-->

## Notes.

- TODO : Work in progress, prototype was not good, hardware, PCB and code can be rework.
- DRAFT : Prototype OK, last check schematic, PCB & code can be modify.

## DISCLAIMER.

THIS CODE IS PROVIDED WITHOUT ANY WARRANTY OR GUARANTEES.
USERS MAY USE THIS CODE FOR DEVELOPMENT AND EXAMPLE PURPOSES ONLY.
AUTHORS ARE NOT RESPONSIBLE FOR ANY ERRORS, OMISSIONS, OR DAMAGES THAT COULD
RESULT FROM USING THIS FIRMWARE IN WHOLE OR IN PART.

---
