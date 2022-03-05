# Opensource toolchain for TI MSP430
The MSP430 is a mixed-signal microcontroller family from Texas Instruments, first introduced on 14 February 1992. Built around a 16-bit CPU, the MSP430 is designed for low cost and, specifically, **low power consumption** embedded applications. 

For more information, please refer to https://en.wikipedia.org/wiki/TI_MSP430

# Hardware requirements
* A development board.
  - I prefer TI's MSP430 LaunchPad with EZFET jtag debugger integrated for beginners. If you have other minimum MSP430 system without FET, you may also need a FET(Flashing Emulation Tool) adapter (It's not cheap, even more expensive than a official LaunchPad).
  - If you really do not know which one to choose, buy the cheapest one such as G2/G2ET or more high end such as FR5994 or FR6989.


**NOTE**

- MSP430 support JTAG and a 2-wire JTAG(Spy-Bi-Wire) interface, but it's NOT compatible with ARM JTAG, so you can not use your ARM JTAG adapters with MSP430.
- Ti's Launchpad with EZFET usually have some jumpers between the EZFET and MCU, you can use the EZFET standalone to connect to other MSP430 development board.

# Toolchain overview
* Compiler, gcc with TI's official support.
* Debugger, mspdebug/gdb
* SDK, opensource headers/linker script provied by TI.
* Flashing tool, mspdebug/MSP430Flasher from TI.

**NOTE**

The Opensource toolchain is supported very well by opensource community and TI officially.

TI officially provide opensource gcc toolchains and MCU support files, and released its' MSP430Flasher and  msp-debug-stack under BSD license.


