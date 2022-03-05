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

# The MSP430 GNU Toolchain

The sources and prebuilt release of MSP430 GCC can be downloaded from https://www.ti.com/tool/MSP430-GCC-OPENSOURCE#downloads

Up to this tutorial written, the lastest version is '9.3.1.2' with released date '23 Jul 2021', you can download the sourcecodes or patchset to build the toolchain yourself using configuration options such as `./configure --prefix=/opt/msp430-gcc --target=msp430-elf`, it includes binutils/gcc/gdb/newlib.

Here we directly use the prebuilt release since it is well supported officially.

Download the prebuilt toolchain from http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/9_3_1_2/export/msp430-gcc-9.3.1.11_linux64.tar.bz2 and support files from http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/9_3_1_2/export/msp430-gcc-support-files-1.212.zip. If you use other arch, such as 32bit x86, you should download the 32bit linux release. for ARM, you have to build it yourself.

This prebuilt release requires libncusesw.so.5 instead of so.6 shipped by most mainstream dist. it's a little bit old but most dist should already provide a compat libs, such as 'ncurses-compat-libs' for fedora or 'libncurses5' for ubuntu, please install via YUM or APT first.


```
sudo mkdir -p /opt/msp430-gcc
sudo tar jxf <where you put>/msp430-gcc-9.3.1.11_linux64.tar.bz2 -C /opt/msp430-gcc --strip-components=1
cd /opt/msp43-gcc
sudo unzip <where you put>/msp430-gcc-support-files-1.212.zip -d .
sudo mv msp430-gcc-support-files/include/*.h /opt/msp430-gcc/msp430-elf/include
sudo mv msp430-gcc-support-files/include/*.ld /opt/msp430-gcc/msp430-elf/lib
sudo rm -rf msp430-gcc-support-files
```

Here also installed the support files include headers and linkers for different MCUs to `msp430-elf` dir, after that, you can use these headers anywhere, otherwise, you have to deal with include dir and pass linker script to `ld`, it's not very easy for beginners.






