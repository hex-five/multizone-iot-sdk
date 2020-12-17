# multizone-iot-sdk

The MultiZone® IoT Firmware is the quick and safe way to build secure IoT applications with any RISC-V processor. It provides secure access to commercial and private IoT clouds, real-time monitoring, secure boot, and remote firmware updates. The built-in Trusted Execution Environment provides hardware-enforced separation to shield the execution of trusted applications from untrusted 3rd party libraries.

Complete IoT firmware optimized for RISC-V processors:

- Provides secure access to any IoT clouds, secure boot, remote firmware updates, etc.
- Works with any RISC-V processor: no need for proprietary TrustZone-like HW
- Rapid development: pre-integrated TEE, TCP/IP, TLS/ECC, MQTT, FreeRTOS, GCC, Eclipse
- Built-in Trusted Execution Environment RTOS providing up to 4 separated HW/SW “worlds”
- Commercial open source license: no GPL contamination, no royalties, priced per design

The MultiZone® IoT Firmware works with any 32-bit and 64-bit RISC-V processor with standard U-mode extension. For a quick start, we recommend the development kit based on the open source softcore X300 developed by Hex Five Security. It is an enhanced version of the E300 SoC (Rocket rv32) originally developed at U.C. Berkeley. Like the E300, the X300 is designed to be programmed onto a Xilinx Artix-7. The X300 bitstream is entirely free for commercial and non-commercial use.

This version of the MultiZone Secure IoT Firmware supports the following hardware development kits:

- [Xilinx Artix-7 Arty FPGA Evaluation Kit](https://www.xilinx.com/products/boards-and-kits/arty.html)

- [Microchip PolarFire SoC FPGA Icicle Kit](https://www.microsemi.com/existing-parts/parts/152514) (Coming soon ...)


The Arty FPGA Evaluation Kit requires the following open source softcore:

- [Hex Five X300 RV32ACIMU - Permissive license, free for any use.](https://github.com/hex-five/multizone-fpga)

For instructions on how to upload the bitstream to the ARTY board and how to connect the [Olimex debug head ARM-USB-TINY-H](https://www.olimex.com/Products/ARM/JTAG/ARM-USB-TINY-H/) see [Arty FPGA Dev Kit Getting Started Guide](https://sifive.cdn.prismic.io/sifive%2Fed96de35-065f-474c-a432-9f6a364af9c8_sifive-e310-arty-gettingstarted-v1.0.6.pdf)


### Installation ###

The MultiZone SDK works with any versions of Linux, Windows, and Mac capable of running Java 1.8 or greater. The directions in this readme have been verified with fresh installations of Ubuntu 20.04, Ubuntu 19.10, Ubuntu 18.04.5, and Debian 10.5. Other Linux distros are similar. Windows developers may want to install a Linux emulation environment like Cygwin or run the SDK in a Linux VM guest (2GB Disk, 2GB Ram).

**Linux prerequisites**

```
sudo apt update
sudo apt install git make default-jre libhidapi-dev libftdi1-2
sudo apt install gtkterm mosquitto-clients
```
_Note_: the package gtkterm is optional and required only to connect to the reference application via a local terminal. It is not required to build, debug, and load the MultiZone firmware or to connect to the target via Ethernet. Any other serial terminal application of choice would do.

_Note_: the package mosquitto-clients is optional and required only to test MQTT funcionality including telemetry and remote firmware updates. It is not required to build, debug, and load the MultiZone firmware or to connect to the target via Ethernet. Any other MQTT client application of choice would do.

Ubuntu 18.04 LTS additional dependency
```
sudo add-apt-repository "deb http://archive.ubuntu.com/ubuntu/ focal main universe"
sudo apt update
sudo apt install libncurses-dev
```

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
git clone --recursive https://github.com/hex-five/multizone-iot-sdk.git
cd multizone-iot-sdk
git apply -p1 ext/lwip.patch --directory=ext/lwip
git apply -p1 ext/freertos.patch --directory=ext/freertos
```
**_Important_**: make sure to apply the lwIP patch above. Without the lwIP patch the firmware is NOT secure!

**_Important_**: FreeRTOS is optional as the MultiZone TEE provides its own RTOS. If you intend to use FreeRTOS, make sure to apply the freertos patch above. 


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


### Connect the device to the MQTT Broker ###

Make sure switch SW3 is positioned close to the edge of the board.

Make sure the board is properly powered. An external power adapter 7-15V connected to J13 is recomended. USB hubs and weak computer USB ports will interfere with Ethernet operations and result in unexpected random behavior.     

Disconnect the JTAG connector if OpenOCD is not in use otherwise the CPU is permanently halted and the system won't boot.

Connect the UART port (ARTY micro USB J10) as indicated in the user manual. On your computer, start a serial terminal console (gtkterm) and connect to /dev/ttyUSB1 at 115200-8-N-1.

Connect the Ethernet port to an Internet router or to your computer if Internet sharing is enabled - see https://help.ubuntu.com/community/Internet/ConnectionSharing. The router should provide DHCP configuration including one DNS servers. There is no need to open inbound ports for the MQTT client to work. If your local network blocks outbound connections to the default MQTT/TLS port 8883, you can reconfigure the client to use the HTTPS/TLS port 443, which is usually open - see MQTT configuration file [mqtt_config.h](https://github.com/hex-five/multizone-iot-sdk/blob/master/zone1/mqtt_config.h) 

Press the reset button on the board and hit the enter key a few times on your serial terminal to synchronize the UART port.  

After a few seconds the client should connect to Hex Five's public MQTT broker:

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
 
Z1 > netif_status_callback: address 192.168.0.130
 
Z1 > dns_callback: mqtt-broker.hex-five.com 54.176.2.35
 
Z1 > sntp_process: 1608056327 Tue Dec 15 18:18:47 2020
 
Z1 > client_id: mzone-47194669 
 
Z1 > mqtt: connecting ... 
 
Z1 > mqtt: connected 
 
Z2 >
```

Hit enter on an empty line to show the list of commands available:

```
Z2 > Commands: yield send recv pmp load store exec dma stats timer restart
```

- **yield**: yield the CPU to the next zone showing the time taken to loop through all zones
- **send/recv**: exchange messages with any zones. Zone 1 is the gateway to the MQTT broker: messages sent to zone 1 are forwarded to the broker topic device-id/zone. Messages sent to the broker topic device-id/zone are forwarded to the respective zone
- **pmp**: show the separation policies for zone 2, which is the zone operating the local terminal
- **load/store**: read and write data from/to any arbitrary physical memory location
- **exec**: jump the execution of the zone to any arbitrary memory location
- **dma**: submit a protected DMA transfer request
- **stats**: repeat the yield command multiple times and print detailed kernel statistics
- **timer**: set the zone timer to current time plus a time delay expressed in milliseconds
- **restart**: jump the execution of this zone to the base address of the first pmp range restarting the zone

For a detailed explanation of the features of the MultiZone TEE see the [MultiZone TEE Reference Manual](https://github.com/hex-five/multizone-iot-sdk/blob/master/ext/multizone/manual.pdf)

_Note:_ take note of your randomly generated client id as you'll need it to interact with the target via MQTT messages published and subscribed to topics mzone-xxxxxxxx/zonex (mzone-47194669 in this example). The MQTT client id is generated randomly for each new MQTT session upon board reset.


### Send and receive MQTT messages ###
```
cd ~/multizone-iot-sdk
export MQTT=" --host mqtt-broker.hex-five.com"
export MQTT=$MQTT" --cafile pki/hexfive-ca.crt"
export MQTT=$MQTT" --cert pki/test.crt"
export MQTT=$MQTT" --key pki/test.key"
```
_Note:_ in the following examples replace "mzone-47194669" with your randomly generated client id.


Subscribe (listen) to all topics for your device - background process:
```
mosquitto_sub $MQTT -t mzone-47194669/# -v &
```

Publish (send) a "ping" message to zone #1:
```
mosquitto_pub $MQTT -t mzone-47194669/zone1 -m ping
```
Observe the "pong" reply received in the background.


### Deploy Remote Firmware Updates ###

Deploy the rainbow LED appication to zone #3 (binary zone3.1/zone3.bin):
```
mosquitto_pub $MQTT -t mzone-47194669/zone3 -f zone3.1/zone3.bin
```

Delopy the robot application to zone #4 (binary zone4.1/zone4.bin):
```
mosquitto_pub $MQTT -t mzone-47194669/zone4 -f zone4.1/zone4.bin
```

Optional: delopy the FreeRTOS-based version of the robot application (binary zone4.2/zone4.bin):
```
mosquitto_pub $MQTT -t mzone-47194669/zone4 -f zone4.2/zone4.bin
```

_Note:_ For a complete explanation of the functionality of each zone/application see the [MultiZone SDK Reference Manual](https://github.com/hex-five/multizone-iot-sdk/blob/master/ext/multizone/manual.pdf).


### Technical Specs ###

<table border=0 cellspacing=0 cellpadding=0 width=1122 style='width:841.25pt;border-collapse:collapse'>
 
<tr style='height:33.55pt'>
  <td width=246 valign=top style='width:184.25pt;border:solid #FFC000 1.0pt;
  border-right:none;background:#FFC000;padding:.05in .1in .05in .1in;
  height:33.55pt'>
  <p><b>Stack Component</b></p>
  </td>
  <td width=528 valign=top style='width:5.5in;border-top:solid #FFC000 1.0pt;
  border-left:none;border-bottom:solid #FFC000 1.0pt;border-right:none;
  background:#FFC000;padding:.05in .1in .05in .1in;height:33.55pt'>
  <p><b><span style='color:black'>Features</span></b></p>
  </td>
  <td width=96 valign=top style='width:71.75pt;border-top:solid #FFC000 1.0pt;
  border-left:none;border-bottom:solid #FFC000 1.0pt;border-right:none;
  background:#FFC000;padding:.05in .1in .05in .1in;height:33.55pt'>
  <p><b><span style='color:black'>Size</span></b></p>
  </td>
  <td width=252 valign=top style='width:189.25pt;border:solid #FFC000 1.0pt;
  border-left:none;background:#FFC000;padding:.05in .1in .05in .1in;height:
  33.55pt'>
  <p><b><span style='color:black'>License</span></b></p>
  </td>
 </tr>

 <tr style='height:47.65pt'>
  <td width=246 valign=top style='width:184.25pt;border-top:none;border-left:
  solid #FFC000 1.0pt;border-bottom:solid #FFC000 1.0pt;border-right:none;
  padding:.05in .1in .05in .1in;height:47.65pt'>
  <p><b>Reference Hardware</b></p>
  <p class=MsoListParagraphCxSpFirst style='text-indent:-.25in'>Digilent ARTY7 35T FPGA</p>
  <p style='text-indent:-.25in'>Hex Five X300 SoC IP</p>
  </td>
  <td width=528 valign=top style='width:5.5in;border:none;border-bottom:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:47.65pt'>
  <p class=MsoListParagraphCxSpMiddle>&nbsp;</p>
  <p style='text-indent:-.25in'>RISC-V core RV32ACIMU 4-way i-cahe 65MHz</p>
  <p style='text-indent:-.25in'>Ethernet: Xilinx EthernetLite Ethernet core</p>
  </td>
  <td width=96 valign=top style='width:71.75pt;border:none;border-bottom:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:47.65pt'></td>
  <td width=252 valign=top style='width:189.25pt;border-top:none;border-left:
  none;border-bottom:solid #FFC000 1.0pt;border-right:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:47.65pt'>
  <p><span lang=EN-GB>Apache 2.0 license</span></p>
  <p><span lang=EN-GB>permissive </span></p>
  <p>commercial use ok</p>
  </td>
 </tr>
 <tr style='height:44.0pt'>
  <td width=246 valign=top style='width:184.25pt;border-top:none;border-left:
  solid #FFC000 1.0pt;border-bottom:solid #FFC000 1.0pt;border-right:none;
  padding:.05in .1in .05in .1in;height:44.0pt'>
  <p><b>IDE &amp; Toolchain</b></p>
  <p class=MsoListParagraphCxSpFirst style='text-indent:-.25in'>Eclipse IDE + openOCD debug</p>
  <p style='text-indent:-.25in'>GNU GCC, GDB, …</p>
  </td>
  <td width=528 valign=top style='width:5.5in;border:none;border-bottom:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:44.0pt'>
  <p style='text-indent:-.25in'>GCC multi-lib rv32, rv32e, rv64, GDB, openOCD</p>
  <p style='text-indent:-.25in'>Hex Five pre-built GCC binaries (optional)</p>
  <p style='text-indent:-.25in'>Hex Five pre-built OpenOCD binaries (optional)</p>
  </td>
  <td width=96 valign=top style='width:71.75pt;border:none;border-bottom:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:44.0pt'></td>
  <td width=252 valign=top style='width:189.25pt;border-top:none;border-left:
  none;border-bottom:solid #FFC000 1.0pt;border-right:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:44.0pt'>
  <p>GNU General Public License version 3</p>
  </td>
 </tr>
 <tr style='height:50.2pt'>
  <td width=246 valign=top style='width:184.25pt;border-top:none;border-left:
  solid #FFC000 1.0pt;border-bottom:solid #FFC000 1.0pt;border-right:none;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p><b>TCP/IP library</b></p>
  <p class=MsoListParagraphCxSpFirst style='text-indent:-.25in'>LWIP 2.1.1</p>
  <p style='text-indent:-.25in'>Hex Five security extensions</p>
  </td>
  <td width=528 valign=top style='width:5.5in;border:none;border-bottom:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p style='text-indent:-.25in'>IP, ICMP, UDP, TCP, ARP, DHCP, DNS, SNTP, MQTT</p>
  <p style='text-indent:-.25in'>Light weight single threaded execution</p>
  <p style='text-indent:-.25in'>Fully integrated with SSL stack</p>
  </td>
  <td width=96 valign=top style='width:71.75pt;border:none;border-bottom:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p> 40KB ROM</p>
  <p> 16KB RAM</p>
  </td>
  <td width=252 valign=top style='width:189.25pt;border-top:none;border-left:
  none;border-bottom:solid #FFC000 1.0pt;border-right:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p>Modified BSD</p>
  <p>permissive</p>
  <p>commercial use ok</p>
  </td>
 </tr>
 <tr style='height:50.2pt'>
  <td width=246 valign=top style='width:184.25pt;border-top:none;border-left:
  solid #FFC000 1.0pt;border-bottom:solid #FFC000 1.0pt;border-right:none;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p><b>SSL library</b></p>
  <p class=MsoListParagraphCxSpFirst style='text-indent:-.25in'>mbed TLS 2.23.0</p>
  <p style='text-indent:-.25in'>Hex Five secure configuration</p>
  </td>
  <td width=528 valign=top style='width:5.5in;border:none;border-bottom:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p style='text-indent:-.25in'>TLSv1.2, Cipher TLS_AES_128_GCM_SHA256</p>
  <p style='text-indent:-.25in'>ECC: prime256v1, Private Key NIST CURVE: P-256</p>
  <p style='text-indent:-.25in'>Mutual authentication, Cert expiration verification, TLS large
  fragment </p>
  </td>
  <td width=96 valign=top style='width:71.75pt;border:none;border-bottom:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p> 64KB ROM</p>
  <p> 32KB RAM</p>
  </td>
  <td width=252 valign=top style='width:189.25pt;border-top:none;border-left:
  none;border-bottom:solid #FFC000 1.0pt;border-right:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p><span lang=EN-GB>Apache 2.0 license</span></p>
  <p><span lang=EN-GB>permissive </span></p>
  <p>commercial use ok</p>
  </td>
 </tr>
 <tr style='height:50.2pt'>
  <td width=246 valign=top style='width:184.25pt;border-top:none;border-left:
  solid #FFC000 1.0pt;border-bottom:solid #FFC000 1.0pt;border-right:none;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p><b>Real Time OS </b>(optional)</p>
  <p class=MsoListParagraphCxSpFirst style='text-indent:-.25in'>FreeRTOS 10.3.0</p>
  <p style='text-indent:-.25in'>Hex Five integration with TEE</p>
  </td>
  <td width=528 valign=top style='width:5.5in;border:none;border-bottom:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p style='text-indent:-.25in'>Secure unprivileged execution of kernel, tasks, and interrupt
  handlers</p>
  <p style='text-indent:-.25in'>No memory shared with TCP/IP and SSL library code</p>
  <p style='text-indent:-.25in'>No memory shared with other applications running in separate
  zones</p>
  </td>
  <td width=96 valign=top style='width:71.75pt;border:none;border-bottom:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p> 32KB ROM</p>
  <p> 16KB RAM</p>
  </td>
  <td width=252 valign=top style='width:189.25pt;border-top:none;border-left:
  none;border-bottom:solid #FFC000 1.0pt;border-right:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p>MIT open-source license</p>
  <p>permissive</p>
  <p>commercial use ok</p>
  </td>
 </tr>
 <tr style='height:50.2pt'>
  <td width=246 valign=top style='width:184.25pt;border-top:none;border-left:
  solid #FFC000 1.0pt;border-bottom:solid #FFC000 1.0pt;border-right:none;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p><b>Trusted Execution Environment</b></p>
  <p class=MsoListParagraphCxSpFirst style='text-indent:-.25in'>MultiZone Security TEE 2.0</p>
  <p style='text-indent:-.25in'>RISC-V secure DMA extension</p>
  <p style='text-indent:-.25in'>RISC-V shared PLIC extension</p>
  </td>
  <td width=528 valign=top style='width:5.5in;border:none;border-bottom:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p style='text-indent:-.25in'>4 separated Trusted Execution Environments (zones) enforced via
  PMP</p>
  <p style='text-indent:-.25in'>8 memory-mapped resources per zone – i.e. ram, rom, i/o, uart,
  gpio, eth, …</p>
  <p style='text-indent:-.25in'>Secure inter-zone messaging – no shared memory, no buffers, no
  stack, etc</p>
  <p style='text-indent:-.25in'>Protected user-mode interrupt handlers mapped to zones – plic /
  clint</p>
  </td>
  <td width=96 valign=top style='width:71.75pt;border:none;border-bottom:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p>   4KB ROM</p>
  <p>   4KB RAM</p>
  </td>
  <td width=252 valign=top style='width:189.25pt;border-top:none;border-left:
  none;border-bottom:solid #FFC000 1.0pt;border-right:solid #FFC000 1.0pt;
  padding:.05in .1in .05in .1in;height:50.2pt'>
  <p>Free evaluation</p>
  <p>Commercial license priced per design perpetual, no
  royalties, no GPL contamination</p>
  </td>
 </tr>
</table>


### Additional Resources ###

- [MultiZone IoT Firmware Quick Start Guide](https://hex-five.com/)
- [MultiZone IoT Firmware Datasheet](https://hex-five.com/)
- [MultiZone IoT Firmware Website](https://hex-five.com/multizone-secure-iot-firmware-riscv/)
- [MultiZone TEE Reference Manual](http://github.com/hex-five/multizone-sdk/blob/master/manual.pdf)
- [MultiZone TEE Datasheet](https://hex-five.com/wp-content/uploads/2020/01/multizone-datasheet-20200109.pdf)
- [MultiZone TEE Website](https://hex-five.com/multizone-security-sdk/)
- [Frequently Asked Questions](http://hex-five.com/faq/)
- [Contact Hex Five http://hex-five.com/contact](http://hex-five.com/contact)


### Legalities ###

Please remember that export/import and/or use of strong cryptography software, providing cryptography hooks, or even just communicating technical details about cryptography software is illegal in some parts of the world. So when you import this software to your country, re-distribute it from there or even just email technical suggestions or even source patches to the authors or other people you are strongly advised to pay close attention to any laws or regulations which apply to you. Hex Five Security, Inc. and the authors of the software included in this repository are not liable for any violations you make here. So be careful, it is your responsibility.

MultiZone and HEX-Five are registered trademarks of Hex Five Security, Inc.

MultiZone technology is patent pending US 16450826, PCT US1938774.
