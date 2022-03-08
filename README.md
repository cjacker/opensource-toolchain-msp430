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
  - For rust toolchain, please refer to https://github.com/rust-embedded/msp430-quickstart
* Debugger, mspdebug/gdb
* SDK, opensource headers/linker script provied by TI.
* Flashing tool, mspdebug, MSP430Flasher from TI and bsl-scripter(UART BSL) from TI.

**NOTE**

The Opensource toolchain is supported very well by opensource community and TI officially.

TI officially provide opensource gcc toolchains and MCU support files, and released its' MSP430Flasher and  msp-debug-stack under BSD license.

# The MSP430 GNU Toolchain

The sources and prebuilt releases of MSP430 GCC can be downloaded from https://www.ti.com/tool/MSP430-GCC-OPENSOURCE#downloads, it includes binutils/gcc/gdb/newlib.

Up to this tutorial written, the lastest version is '9.3.1.2' with released date '23 Jul 2021', you can download the sourcecodes or patchset to build the toolchain yourself using configuration options such as `./configure --prefix=/opt/msp430-gcc --target=msp430-elf`.

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

After installation finished, please add `/opt/msp430-gcc/bin` to PATH env according to your shell.

# MSP Debug Stack
The MSP debug stack (MSPDS) for all MSP430™ microcontrollers (MCUs) and SimpleLink™ MSP432™ devices consists of a static library on the host system side as well as an embedded firmware that runs on debug tools including the MSP-FET, MSP-FET430UIF or on-board eZ debuggers. 

It is the bridging element between all PC software and all MSP430 and SimpleLink MSP432 microcontroller derivatives and handles tasks such as code download, stepping through code or break points. 

You can download MSP Debug Stack from https://www.ti.com/tool/MSPDS#downloads, up to this tutorial written, the latest version is '3.15.1.1' with release date '12 Jun 2020'. Building it need a patch for linux, the patch provied within this repo.

Building:
```
mkdir -p msp-debug-stack
wget https://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPDS/3_15_1_001/export/MSPDebugStack_OS_Package_3_15_1_1.zip
unzip MSPDebugStack_OS_Package_3_15_1_1.zip -d .
cat MSPDebugStack_build_linux.patch|patch -p1
make
```

Installation:
```
sudo install -m0755 libmsp430.so /usr/lib
sudo mkdir -p /usr/include/libmsp430
sudo cp DLL430_v3/include/* /usr/include/libmsp430
```

# SDKS

SDK for TI MSP430 acctually is a set of headers and linker scripts for various MSP430 models, we already install it with GCC toolchain.

Let's blink a LED on board, here I use fr6989 launchpad as example, please read the circuit diagram of your board to find the IO pin you should manupulate to turn on or toggle a LED, fr6989 have two LED on board, the codes below turn on one and blink another one.

```
// blink.c
#include <msp430fr6989.h>

int main(void)
{
    unsigned int i;

    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
    P1DIR |= BIT0;
    P1OUT |= BIT0;
    P9DIR |= BIT7;
    P9OUT |= BIT7;

    PM5CTL0 &= ~LOCKLPM5;

    while(1)
    {
      P9OUT |= BIT7;
      for(i = 0; i < 30000; i++);
      P9OUT &= ~BIT7;
      for(i = 0; i < 30000; i++);
    }

    return 0;
}
```

Compile it:
```
msp430-elf-gcc -g -mmcu=msp430fr6989 -o main main.c
```

If built successfully, 'main' elf generated, it means toolchain works. otherwise, please examine the steps of the toolchain installation.

# Flashing
There are 3 Flashing tools you can use with linux for MSP430, include mspdebug, msp-flasher(TI) and bsl scripter(TI)

**Note:**
* Not all models of MSP430 support BSL UART flashing, please read the datasheet first.
* TI officially provided a bsl-scripter tool and only works with TI MSP BSL rocket adapter and some adapter special designed for MSP430.
* **If you want to use general USB to TTL UART adapter, at least it should have RX/TX/DTR/RTS pins and need some patches to bsl scripter.**

## mspdebug
MSPDebug is a free debugger for use with MSP430 MCUs. 

It supports FET430UIF, eZ430, RF2500 and Olimex MSP430-JTAG-TINY programmers, as well as many other compatible devices. 

It can be used as a proxy for gdb or as an independent debugger with support for programming, disassembly and reverse engineering.

Installation:
```
git clone https://github.com/dlbeer/mspdebug.git
cd mspdebug
make
sudo make install
```

After installation finished, please connect the launchpad to PC linux with a USB cable, the devices '/dev/ttyACM[n]' should appear. and flashing above blink example as:

```
sudo mspdebug tilib -d /dev/ttyACM0 'prog main'
```

If you get some errors from `dmesg`, try to update the EZFET firmware with:
```
sudo mspdebug tilib -d /dev/ttyACM0 --allow-fw-update
```

and try flashing again.


## MSPFlasher provided by TI officially

MSPFlasher is an open-source, shell-based (command line) interface for programming MSP430™ and MSP432™ microcontrollers (MCUs) through JTAG and provides the most common programming functions. MSP Flasher can be used to download binary files directly to memory without the need of an IDE like CCS or IAR. It can also be used to extract firmware and lock JTAG access permanently. 

The source codes of MSPFlasher can be obtained from its' installer package.

```
wget http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430Flasher/1_03_20_00/exports/MSPFlasher-1_03_20_00-linux-x64-installer.zip
unzip MSPFlasher-1_03_20_00-linux-x64-installer.zip

chmod +x ./MSPFlasher-1.3.20-linux-x64-installer.run
./MSPFlasher-1.3.20-linux-x64-installer.run --mode unattended --prefix `pwd`/ti
rm -rf ti/libmsp430.so

cd ti/Source
make
cd ..
sudo install -m0755 MSP430Flasher /usr/bin
```

You may also need to create a udev rules to set EZFET devices to correct permissions if you do not want to use 'sudo' everytime.

Create a file `/etc/udev/rules.d/99-ti-launchpad.rules` with the contents:

```
ATTRS{idVendor}=="2047", ATTRS{idProduct}=="0010", MODE="0660", GROUP="dialout"
ATTRS{idVendor}=="2047", ATTRS{idProduct}=="0013", MODE="0660", GROUP="dialout"
ATTRS{idVendor}=="2047", ATTRS{idProduct}=="0203", MODE="0660", GROUP="dialout"
```
reload udev rules or just reboot:
```
sudo udevadm control --reload-rules
sudo udevadm trigger
```

and add current user to `dialout` group
```
usermod -a -G dialout <your user name>
```

Above steps about udev rules are optional and can be omitted, if you prepend `sudo` to `MSP430Flasher` everytime.

## UART BSL flashing
TI provide bsl scripter for some new MSP430 model, it support UART/I2C flashing but still need a special adatper or a special designed circuit. 

If you want to use general USB to TTL UART adapter, you should apply some patches to bsl scripter and rebuilt it by yourself.

The code changes is described at https://github.com/gbhug5a/CP2102-with-BSL-Scripter-for-MSP430 and I implement the TEST/RESET signal control for Linux, since the upstream codes from TI leave them unimplemented for Linux.

The BSL Scripter is a command line tool to communicate with the bootloader (BSL) on an MSP430™ or SimpleLink™ MSP432™ microcontroller. 

you can download the release package with source codes from https://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPBSL_Scripter/latest/exports/BSLScripter-3.4.0.2-linux64-installer.zip, the latest version up to this tutorial written is '3.4.0.2' and download 3 patches from this repo.

before building it, you should have boost/hidapi/libusb development package installed:

```
unzip BSLScripter-3.4.0.2-linux64-installer.zip
./BSLScripter-3.4.0.2-linux64-installer.run --mode unattended --prefix `pwd`/bsl-scripter
cd bsl-scripter
cat 001-bsl-scripter-build-linux.patch | patch -p1
cat 002-bsl-scripter-invoke.patch | patch -p1               
cat 003-bsl-scripter-test-reset-for-linux.patch | patch -p1
cd Source-Linux
make BIT64=1
sudo install -m0755 bsl-scripter-linux-64 /usr/bin/bsl-scripter
```

This tutorial use FR6989 Launchpad, please wire up USB2TTL adapter with Launchpad as:

```
RX->TX(P2.0), not the backchannel TX
TX->RX(P2.1), not the backchannel RX
DTR->SBWTDIO/RST
RTS->SBWTCK/TEST
GND->GND
VCC->VCC, if the board need power supply from USB2TTL adapter.
```

And use the `FRxx_uart` examples in `bsl-scripter/ScriptExampleLinux`, if you use general USB to TTL adapter, please change the `MODE` line of `script_FRxx_uart.txt` from
```
MODE FRxx UART 9600 /dev/ttyACM0
```
to
```
MODE INVOKE FRxx UART 9600 PARITY /dev/ttyUSB0
```
the **'INVOKE'** means a special workaround to enter BSL with general USB to TTL adapter.

the **'PARITY'** here is for my FT2232 adapter, you may not need to set this arg.

and change the device according to your adapter.

and try run:
```
sudo bsl-scripter script_FRxx_uart.txt
```

the output should looks like:

<img src="https://user-images.githubusercontent.com/1625340/157285979-3abd84aa-9436-469e-9c9e-6071801a4322.png" width="70%"/>


# Debugging

Here is a simple demo using above blink example, Launch a terminal and run:
```
sudo mspdebug tilib -d /dev/ttyACM0 gdb
```

the output looks like:
```
...
Using new (SLAC460L+) API
MSP430_Initialize: /dev/ttyACM0
Firmware version is 31501001
MSP430_VCC: 3000 mV
MSP430_OpenDevice
MSP430_GetFoundDevice
Device: MSP430FR6989 (id = 0x0160)
3 breakpoints available
MSP430_EEM_Init
Chip ID data:
  ver_id:         81a8
  ver_sub_id:     0000
  revision:       30
  fab:            55
  self:           5555
  config:         20
  fuses:          55
Bound to port 2000. Now waiting for connection...
```

And launch another terminal, run:
```
msp430-elf-gdb -q main
(gdb) target remote :2000
Remote debugging using :2000
0x00004402 in __crt0_start ()
(gdb) load
Loading section .upper.data, size 0x2 lma 0x4400
Loading section .text, size 0x9e lma 0x4402
Loading section __reset_vector, size 0x2 lma 0xfffe
Start address 0x00004402, load size 162
Transfer rate: 2 KB/sec, 54 bytes/write.
(gdb) break 22
(gdb) break 22
Breakpoint 1 at 0x447c: file main.c, line 22.
(gdb) c
Continuing.

Breakpoint 1, main () at main.c:22
22            P9OUT &= ~BIT7;

```

Beside as a proxy to gdb, mspdebug can also work as a debugger. please refer to mspdebug and gdb manuals for more information. 

# Project template

A project and Makefile templates provided within this repo, Please refer to the Makefile and change it according to your development board.

to flash with MSP430Flasher:
```
make flash_msp430flasher
```

to flash with mspdebug:
```
make flash_mspdebug
```


There are still a lot need to write, for example, various target file formats, etc. but this tutorial should be enough to help you start MSP430 development with Linux.


