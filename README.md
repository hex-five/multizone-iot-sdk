# multizone-iot-sdk

The MultiZone® IoT Firmware is the quick and safe way to build secure IoT applications with any RISC-V processor. It provides secure access to commercial and private IoT clouds, real-time monitoring, secure boot, and remote firmware updates. The built-in Trusted Execution Environment provides hardware-enforced separation to shield the execution of trusted applications from untrusted 3rd party libraries.

Complete IoT firmware optimized for RISC-V:

- Complete IoT stack that shields trusted applications from untrusted 3rd party libraries
- Provides secure access to any IoT clouds, secure boot, remote firmware updates, etc.
- Works with any RISC-V processor: no need for proprietary TrustZone-like HW
- Rapid development: pre-integrated TEE, TCP/IP, TLS/ECC, FreeRTOS, GCC, Eclipse
- Built-in Trusted Execution Environment RTOS providing up to 4 separated HW/SW “worlds”
- Commercial open source license: no GPL contamination, no royalties, priced per design

The MultiZone® IoT Firmware works with any 32-bit and 64-bit RISC-V processor with standard U-mode extension. For a quick start, we recommend the development kit based on the open source softcore X300 developed by Hex Five Security. It is an enhanced version of the E300 SoC (Rocket rv32) originally developed at U.C. Berkeley. Like the E300, the X300 is designed to be programmed onto a Xilinx Artix-7 35T. The X300 bitstream is entirely free for commercial and non-commercial use.

This version of the MultiZone Secure IoT Firmware supports the following hardware development kits:

- [Xilinx Artix-7 Arty FPGA Evaluation Kit](https://www.xilinx.com/products/boards-and-kits/arty.html)

- [Microchip PolarFire SoC FPGA Icicle Kit](https://www.microsemi.com/existing-parts/parts/152514) (Coming soon ...)


The Arty FPGA Evaluation Kit requires the following open source softcore:

- [Hex Five X300 RV32ACIMU - Permissive license, free for any use.](https://github.com/hex-five/multizone-fpga)

For instructions on how to upload the bitstream to the ARTY board and how to connect the [Olimex debug head ARM-USB-TINY-H](https://www.olimex.com/Products/ARM/JTAG/ARM-USB-TINY-H/) see [Arty FPGA Dev Kit Getting Started Guide](https://sifive.cdn.prismic.io/sifive%2Fed96de35-065f-474c-a432-9f6a364af9c8_sifive-e310-arty-gettingstarted-v1.0.6.pdf)


### Installation ###

The MultiZone SDK works with any versions of Linux, Windows, and Mac capable of running Java 1.8 or greater. The directions in this readme have been verified with fresh installations of Ubuntu 20.04, Ubuntu 19.10, Ubuntu 18.04.5, and Debian 10.5. Other Linux distros are similar. Windows developers may want to install a Linux emulation environment like Cygwin or run the SDK in a Linux VM guest (2GB Disk, 2GB Ram)

**Linux prerequisites**

```
sudo apt update
sudo apt install gmake default-jre gtkterm libhidapi-dev libftdi1-2
```
Ubuntu 18.04 LTS additional dependency

```
sudo add-apt-repository "deb http://archive.ubuntu.com/ubuntu/ focal main universe"
sudo apt update
sudo apt install libncurses-dev
```
Note: GtkTerm is optional and required only to connect to the reference application via a local terminal. It is not required to build, debug, and load the MultiZone firmware or to connect to the target via Ethernet. Any other serial terminal application of choice would do.

**GNU RISC-V Toolchain**

Hex Five reference build: RISC-V GNU Toolchain Linux 64-bit June 13, 2020
```
cd ~
wget https://hex-five.com/wp-content/uploads/riscv-gnu-toolchain-20200613.tar.xz
tar -xvf riscv-gnu-toolchain-20200613.tar.xz
```

**OpenOCD on-chip debugger**

Hex Five reference build: RISC-V openocd Linux 64-bit June 13, 2020
```
cd ~
wget https://hex-five.com/wp-content/uploads/riscv-openocd-20200613.tar.xz
tar -xvf riscv-openocd-20200613.tar.xz
```

**Linux USB udev rules**

```
sudo vi /etc/udev/rules.d/99-openocd.rules

# Future Technology Devices International, Ltd FT2232C Dual USB-UART/FIFO IC
SUBSYSTEM=="tty", ATTRS{idVendor}=="0403",ATTRS{idProduct}=="6010", MODE="664", GROUP="plugdev"
SUBSYSTEM=="usb", ATTR{idVendor} =="0403",ATTR{idProduct} =="6010", MODE="664", GROUP="plugdev"

# Olimex Ltd. ARM-USB-TINY-H JTAG interface
SUBSYSTEM=="tty", ATTRS{idVendor}=="15ba",ATTRS{idProduct}=="002a", MODE="664", GROUP="plugdev"
SUBSYSTEM=="usb", ATTR{idVendor} =="15ba",ATTR{idProduct} =="002a", MODE="664", GROUP="plugdev"
```
Reboot for these changes to take effect.

**MultiZone IoT SDK**

```
cd ~
git clone --recursive git@github.com:hex-five/multizone-iot-sdk.git
cd multizone-iot-sdk
git apply -p1 ext/lwip.patch --directory=ext/lwip
git apply -p1 ext/freertos.patch --directory=ext/freertos
```
***Important***: make sure to apply the lwIP patch above. Without the lwIP patch the firmware is NOT secure!

***Important***: FreeRTOS is optional as the MultiZone TEE provides its own RTOS. If you intend to use FreeRTOS, make sure to apply the freertos patch above. 


### Build & load the MultiZone IoT firmware ###

Connect the target board to the development workstation as indicated in the user manual.
'ls multizone-iot-sdk/bsp' shows the list of supported targets: X300, PFSOC.
Assign one of these values to the BOARD variable - default is X300.

```
cd ~/multizone-iot-sdk
export RISCV=~/riscv-gnu-toolchain-20200613
export OPENOCD=~/riscv-openocd-20200613
export BOARD=X300
make 
make load
```
Note: With some older versions of the ftdi libraries, the first "make load" after powering the board may take a bit longer. If you don't want to wait, the simple workaround is to reset the FPGA board to abort the openOCD session. If you do this, make sure to kill the openocd process on your computer. Subsequent loads will work as expected and take approximately 10 seconds.

### Connect to the MQTT Broker ###

Make sure switch SW3 is positioned close to the edge of the board.
Open jumper JP2 (CK RST) to prevent system reset upon UART connection.

Make sure the board is properly powered. An external power adapter 7-15V connected to J13 is recomended. USB hubs and weak computer USB ports will interfere with Ethernet operations and result in unexpected random behavior.     

Disconnect the JTAG connector if OpenOCD is not in use otherwise the processor is permanently halted and the system won't boot.

Connect the UART port (ARTY micro USB J10) as indicated in the user manual. On your computer, start a serial terminal console (gtkterm) and connect to /dev/ttyUSB1 at 115200-8-N-1.

Connect the Ethernet port to an Internet router or to your computer if Internet sharing is enabled - see https://help.ubuntu.com/community/Internet/ConnectionSharing. The router should provide DHCP configuration including one DNS servers. There is no need to open inbound ports for the MQTT client to work. If your local network blocks outbound connections to the default MQTT/TLS port 8883, you can reconfigure the client to use the HTTPS/TLS port 443, which is likely not blocked.

Press the reset button

After 5 to 10 seconds the client should connect to the Hex Five's public MQTT broker

```
=====================================================================
      	             Hex Five MultiZone® Security                    
    Copyright© 2020 Hex Five Security, Inc. - All Rights Reserved    
=====================================================================
This version of MultiZone® Security is meant for evaluation purposes 
only. As such, use of this software is governed by the Evaluation    
License. There may be other functional limitations as described in   
the evaluation SDK documentation. The commercial version of the      
software does not have these restrictions.                           
=====================================================================
Machine ISA   : 0x40101105 RV32 ACIMU 
Vendor        : 0x0000057c Hex Five, Inc. 
Architecture  : 0x00000001 X300 
Implementation: 0x20181004 
Hart id       : 0x0 
CPU clock     : 64 MHz 
RTC clock     : 16 KHz 

Z1 > netif_link_callback: up

Z1 > netif_status_callback: address 192.168.0.141

Z1 > dns_callback: mqtt-broker.hex-five.com 54.176.2.35

Z1 > sntp_process: 1597618503 Sun Aug 16 22:55:03 2020

Z1 > client_id: mzone-3c77b58a 

Z1 > mqtt: connecting ... 

Z1 > mqtt: connected 

Z2 > 
```

### Remote Firmware Updates ###


### Enable LwIP Debug output ###

**1. lwipopts.h**

```
LWIP_DEBUG
ALTCP_MBEDTLS_DEBUG
ALTCP_MBEDTLS_LIB_DEBUG

/* [OPTIONAL - Second stage debugging] */

#define MEMP_OVERFLOW_CHECK 1
#define MEMP_SANITY_CHECK   1
#define MEM_OVERFLOW_CHECK  1
#define MEM_SANITY_CHECK 1
[Comment out] // #define LWIP_NOASSERT 1 // saves ~18K FLASH
```

**2. mbedtls_config.h**

```
#define MBEDTLS_DEBUG_C
#define MBEDTLS_SSL_DEBUG_ALL
```

**3. cc.h**

Redefine LWIP_PLATFORM_DIAG output

```
#define printf2(format, args...) { 
/* Multi-part printf() - non blocking / nocopy */ 
...						
}

#define LWIP_PLATFORM_DIAG(x) do {printf2 x;} while(0)
```


### Technical Specs (TBD) ###
| |
|---|
| Up to 8 hardware threads (zones) hardware-enforced, software-defined |
| Up to 8 memory mapped resources per zone – i.e. flash, ram, rom, i/o, etc. |
| Scheduler: preemptive, cooperative, round robin, configurable tick |
| Secure interzone communications based on messages – no shared memory |
| Built-in trap & emulation for privileged instructions – CSRR, CSRW, WFI, etc. |
| Support for secure user-mode interrupt handlers mapped to zones – up to 32 sources PLIC / CLIC|
| Support for Wait For Interrupt and CPU suspend mode for low power applications |
| Formally verifiable runtime ~2KB, 100% written in assembly, no 3rd-party dependencies |
| C library wrapper for protected mode execution – optional for high speed / low-latency |
| Hardware requirements: RV32, RV32e, RV64 processor with Memory Protection Unit | 
| System requirements: 4KB FLASH, 2KB RAM - CPU overhead < 0.01% | 
| Development environment: any versions of Linux, Windows, Mac running Java 1.8 |


### Additional Resources (TBD) ###

- [MultiZone Reference Manual](http://github.com/hex-five/multizone-sdk/blob/master/manual.pdf)
- [MultiZone Datasheet](https://hex-five.com/wp-content/uploads/2020/01/multizone-datasheet-20200109.pdf)
- [MultiZone Website](https://hex-five.com/multizone-security-sdk/)
- [Frequently Asked Questions](http://hex-five.com/faq/)
- [Contact Hex Five http://hex-five.com/contact](http://hex-five.com/contact)


### Legalities ###

Please remember that export/import and/or use of strong cryptography software, providing cryptography hooks, or even just communicating technical details about cryptography software is illegal in some parts of the world. So when you import this software to your country, re-distribute it from there or even just email technical suggestions or even source patches to the authors or other people you are strongly advised to pay close attention to any laws or regulations which apply to you. Hex Five Security, Inc. and the authors of the software included in this repository are not liable for any violations you make here. So be careful, it is your responsibility.

MultiZone and HEX-Five are registered trademarks of Hex Five Security, Inc.

MultiZone technology is patent pending US 16450826, PCT US1938774.
