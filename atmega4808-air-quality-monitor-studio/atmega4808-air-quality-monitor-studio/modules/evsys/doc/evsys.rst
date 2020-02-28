====================
Event System driver
====================
The Event System (EVSYS) enables direct peripheral-to-peripheral communication and signaling. It
allows a change in one peripheral's state to trigger actions in other peripherals. 

Features
--------
* Initialization

Applications
------------
* Event System reduces CPU overhead and simplifies the application

Dependencies
------------
* CLKCTRL/CLK for clocks
* PORT for I/O lines and connections
* CPUINT/PMIC for Interrupt

Note
----
* ISR will be generated only when Global Interrupt checkbox and driver_isr Harness checkbox are enabled

Concurrency
-----------
N/A

Limitations
-----------
N/A

Knows issues and workarounds
----------------------------
N/A

