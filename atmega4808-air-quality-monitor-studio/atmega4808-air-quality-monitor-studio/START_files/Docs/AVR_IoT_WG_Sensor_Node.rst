===============
Google IoT Core
===============
Google IoT Core is a secure, Wi-Fi connected solution for an IoT node. It enables users or developers
to acquire sensor data (e.g. from either a sensor, or a Click board) and push it to the Google Could Platform.
This IoT solution relies on three main components - Smart, Connectivity, Security. The smart part is
represented by a Microchip ATmega4808 8-bit high-performance AVR MCU, featuring a hardware multiplier,
Core Independent Peripherals, Event System, intelligent analog and maximum frequency of 20MHz.
Connectivity is represented by Microchip ATWINC1510, an IEEE 802.11 b/g/n IoT network controller,
an ideal add-on to existing MCU solutions, featuring a fully integrated Power Amplifier, LNA and Switch
and Power Management. The security part is represented by Microchip ATECC608A, a secure element integrating 
ECDH (Elliptic Curve Diffie Hellman) security protocol, an ultra-secure method to provide key agreement for
encryption/decryption, along with ECDSA (Elliptic Curve Digital Signature Algorithm) sign-verify authentication
for the Internet of Things (IoT) market.

`Go to AVR IoT Sensor Node page <https://avr-iot.com/>`__

========================================
Hardware (Google Field Engagement Board)
========================================
* ATmega4808
* ATWINC1510
* ATECC608A (pre-provisioned)
* TEMT6000 light sensor
* MCP9808 temperature sensor
* MCP73871 Battery Charger
* 2x push buttons
* 4x LEDs

============
Key features
============
* ATmega4808
	- Program memory size: 48KB
	- SRAM Bytes: 6144
	- Data EEPROM/HEF (bytes): 256
	- CPU Speed (MIPS/DMIPS): 20
	- Digital Communication Peripherals: 3-UART, 1-SPI, 1-I2C
	- Capture/Compare/PWM Peripherals: 12 Input Capture, 11PWM
	- Timers: 4 x 16-bit
	- Hardware multiplier
	- Three sleep modes: (Idle, Standby, Power Down)
 	- Event System
	
* ATWINC1510
	- Single-band 2.4GHz b/g/n
	- Extreme low power
	- Operating voltage: 3.0V to 4.2V
	- Serial host interface: SPI
	- Security protocols supported:  WPA/WPA2 Personal, TLS, SSL
	- Network services:  DHCP, DNS, TCP/IP (IPv4), UDP, HTTP, HTTPS

* ATECC608A
	- Cryptographic co-processor with secure hardware-based key storage
	- Protected storage for up to 16 Keys, certificates or data
	- Hardware support for asymmetric sign, verify, key agreement - ECDSA
	- Hardware support for symmetric algorithms (SHA-256, AES-128)
	- Secure boot support

=================
Related Documents
=================
* `ATmega4808 Product Page <https://www.microchip.com/wwwproducts/en/ATMEGA4808>`__
* `ATWINC1500 Product Page <https://www.microchip.com/wwwproducts/en/ATWINC1500>`__
* `ATECC608A Product Page <https://www.microchip.com/wwwproducts/en/ATECC608A>`__


=======================
Google IoT Core Example 
=======================
Example has all the components (middlewares and drivers) already added and configured for the project. The CryptoLib has been
configured for ATECC608A use. The example uses ADC and a scheduler that implements a one second timeout to read out the
sensor data from the light sensor and push it to the cloud.

===================
Running the Example
===================

IMPORTANT NOTICE:

* Personalizing and configuring the Wi-Fi and Secure elements are out of scope for this example; this implies:
	- ATECC608A has already been provisioned/personalized for Google Cloud, prior to using it in this example
	- WINC1510 is running the latest 19.6.1 firmware and it has Google Cloud certificates already programmed

NOTICE FOR MPLAB-X PROJECT:

* Add these symbols "ATCA_NO_HEAP;ATCA_NO_POLL;ATCA_HAL_I2C;ATCAPRINTF;TCPIP_BSD" to project settings->XC8 Compiler->Preprocessing and messages->Define macros.
* Add "-Wno-pragmas" to XC8 Global options>additional options
* The project needs to be compiled in "-Os" optimization which needs a licensed version.
	
1. Open the example in Atmel START

2. Export Project: Name and download to relevant IDE.

3. In Atmel Studio 7: File->Import->Atmel Start Project.

7. Connect ATmega4808 board to PC using the micro USB cable

8. Check the 20MHz Oscillator fuse is set. If not, then switch to 20MHz Oscillator Frequency Fuse on the Google Field Engagement Board:
	- In Studio, click Tools -> Device Programming
	- Select the EDBG Tool. Device will be already identified as ATmega4808.
	- Click Apply
	- Select Fuses on the left side
	- Set OSCCFG.FREQSEL to 20MHz
	- Click Program

9. Build and flash project to the Google Field Engagement Board board.
The LEDs shall start lighting up in this order:
- BLUE - node successfully connected to the Wi-Fi network
- GREEN - node successfully connected to Google Cloud
- YELLOW - data pushing; as soon as data is being pushed to the cloud, yellow LED shall start flashing
- RED - this LED will only light up in case there was a problem during the node connection, either to the Wi-Fi, or Google Cloud.

