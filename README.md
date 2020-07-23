[![MCHP](https://cldup.com/U0qhLwBijF.png)](https://www.microchip.com)

**Description**

This example demonstrates smart, secure and connected indoor Air Quality Monitor (AQM) using AVR IoT WG board powered by ATmega4808 MCU, secured by ATECC608A security chip and connected by WINC1510 WIFI module.

**Note**

Download/Clone the project at root of the directory to avoid build errors due to windows path length limitation.

**INTRODUCTION**

In this application demonstrator, the AQM system is designed and implemented using core independent peripherals (CIPs) and intelligent analog peripherals of the ATmega4808 microcontroller featuring the 8-bit AVR® processor. Microchip’s CryptoAuthentication™ secure element (ATECC608A) and a fully certified Wi-Fi® module (ATWINC1510) are used to securely connect, the smart AQM to Google Cloud IoT Core platform.

![Overall AQM System with Cloud and Webpage](Images/1.jpg)

​The application demo is realized using [AVR-IoT WG Development Board](https://www.microchip.com/DevelopmentTools/ProductDetails/AC164160) and several click boards from MikroElektronika™.

In this application, the MCU monitors humidity and temperature along with main airborne contaminants such Particulate Matter (PM2.5), Carbon dioxide (CO2) and Total Volatile Organic Components (TVOC). The MCU processes these acquired readings and calculates the Air quality Index (AQI) from the readings of PM2.5 sensor. The AQI and other acquired air quality parameters are stored on an external EEPROM and displayed on the OLED. If AQM finds internet connectivity, it uploads the AQI and other parameters to the Google Cloud. After that, the MCU enters in the sleep mode and wakes up periodically to monitor the sensors. MCU also wakes up by switch press event and displays the air quality parameters on the OLED display.

![Block Diagram](Images/2.jpg)

For more details refer to application note [AN3403 Indoor Air Quality Monitor: Concept and Implementation](https://www.microchip.com/DS00003403).



**SUPPORTED EVALUATION KIT**

ATmega4808 AVR IoT WG

**HARDWARE**

+ ATmega4808 AVR IoT WG
	+ ATmega4808
	+ ATWINC1510
	+ ATECC608A (pre-provisioned)
	+ TEMT6000 light sensor
	+ MCP9808 temperature sensor
	+ MCP73871 Battery Charger
	+ 2x push buttons
	+ 4x LEDs

+ Air Quality 3 Click
	+ CCS811 TVOC and eCO2 Sensor

+ OLED B Click

+ EEPROM3 Click
	+ AT24CM02 2 Mbit EEPROM chip

+ SPS30 PM Sensor

+ SHT31 Humidity and Temperature Sensor

+ Proto Click

+ Shuttle Click

+ mikroBUS Shuttles

**PERIPHRALS INVLOVED**

+ Analog-to-Digital Converter (ADC)

+ Universal Synchronous and Asynchronous Receiver and Transmitter (USART)

+ Serial Peripheral Interface (SPI)

+ Two Wire Interface (TWI): I2C compatible

+ Real-Time Counter (RTC)

+ Periodic Interrupt Timer (PIT)

+ Sleep Controller

+ Non Volatile Memory Access Control (NVM)

+ Event system

+ Configurable Custom Logic (CCL)

**RUNNING THE DEMO**

**Hardware Connection**

The AVR-IOT WG board has a mikroBUS slot. Click boards are connected to the board using shuttle click, which is a mikroBUS™ socket expansion board, placed over the mikroBUS slot.

Proto click is used to interface Humidity -Temperature sensor (SHT31-ARP) and PM sensor (SPS30) with the MCU as clicks boards for these sensors are not available. Below table shows pin connections of humidity and PM sensor with mikroBUS connector of proto click.

| SHT31-ARP     | mikroBUS connector of Proto Click | mikroBUS connector of Proto Click  |SHT31-ARP|SPS30|
| :---------: |:----------:|:-----------:|:---------:|:------------:|
| PIN 1 (Humidity)	| AN 	| PWM|	|			|		
|           		| RST 	| INT	|PIN 4 (Temperature)	|		|
|          		| CS 	|RX 	|			|PIN 3 (**TX**)|
|          		| SCK 	| TX 	|			|PIN 2 (**RX**)|
|           		| MISO 	| SCL 	|			|		|
|          		| MOSI 	| SDA	|			|		|
|PIN 5 (VDD)      	| 3.3V 	| 5V 	|			|PIN 1 (5V)	|
|PIN 2,3,7,8 (VSS) 	| GND 	| GND 	|			|PIN 5 (GND)	|




**Firmware generation and Programming**

1. Download the example.

2. Open the example  into Atmel Studio 7 by clicking on the .atsln file.

3. Connect AVR IoT WG board to PC using the micro USB cable.

4. Build the application and program the device. Press Start without debugging or use  CTRL+ALT+F5 hotkeys to run the application for programming the device.


**Operation**

1. Once programming is completed, “AQM” will be displayed on the OLED.

2. The LED array of the AVR-IoT WG Development Board will flash in the following order twice: Blue->Green->Yellow->Red

3. Now all the air quality parameters will start displaying one by one with predefined time (approx. 4 second). After all parameters are displayed OLED will be turned off. User can turn on the display by pressing switch SW0.

4. SW0 can also be used to display next parameter without waiting for the predefined time.

5. Following tasks are carried out according to predefined time

	+ Reading sensor parameters

	+ Sending data to the google cloud (if connection is available)

	+ Storing data in external EEPROM carries out according to predefined time.

6. Red LED will be turned on if there is any error in connection to the Wi-Fi and cloud.

7. Refer section **Wi-Fi Configuration** to visit dedicated webpage and configure Wi-Fi.

8. Check the LEDs indication for Wi-Fi connection (Blue) and cloud connection (Green). Yellow LED will flash during data upload to the google cloud.

**Wi-Fi Configuration**

1. Once plugged into to the PC, the board should also appear as a Removable Storage Device on the host PC. Double click the “CURIOSITY” drive to open it. Double click on the CLICK-ME.HTM file to go to the dedicated webpage.

2. When the connection has not been established, the lower left-hand corner of the webpage will show a wireless network connection window where the user can enter the credentials for the Wi-Fi network.

3. Once these details are entered, click the Download Configuration button. This will download the WIFI.CFG (text) file on the host PC.

4. From the WIFI.CFG’s download location, drag and drop the file to the CURIOSITY drive to update the Wi-Fi credentials of the board. The Blue LED will light up to show a successful connection.

5. Once after successful connection webpage starts showing the air quality sensor data.

**RELATED DOCUMENTS / APPLICATION NOTES**

+ [AN3403 Indoor Air Quality Monitor: Concept and Implementation](https://www.microchip.com/DS00003403)

+ [Indoor Air Quality Monitor UG](https://www.microchip.com/DS50002966)

+ [AN3417 Indoor Air Quality Monitor: Firmware Creation Using Atmel START and MPLAB® Code Configurator (MCC)](https://www.microchip.com/DS00003417)

+ [AVR IoT Development Board User Guide](https://www.microchip.com/DS50002809)
