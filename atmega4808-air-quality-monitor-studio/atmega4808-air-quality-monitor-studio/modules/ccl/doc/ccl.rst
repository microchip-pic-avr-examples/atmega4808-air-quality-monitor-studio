===========================================================
Digital Glue Logic based on Configurable Custom Logic (CCL)
===========================================================

The Configurable Custom Logic (CCL) is a programmable logic peripheral which
can be connected to the device pins, to events, or to other internal peripherals.
This allows the user to eliminate logic gates for simple glue logic functions
on the PCB. Custom logic driver offers a way to initialize on-chip programmable
logic units, so that a specific logic box is built. Then this "box" can be
connected to internal or external circuit to perform the logic operations.

NOTE : For ATmega408,ATmega808,"ATmega809",ATmega1608,ATmega1609,ATMega4809,ATMega4808,ATMega3208,ATMega3209, the CCL configurator supports only 2 clock source mode (CCL/IN2).

Features
--------
* Initialization

Applications
------------
* Connected to external circuit, as a logic operation box, e.g., as adder.

Dependencies
------------
* CLKCTRL for clocks
* PORT for I/O lines and connections
* CPUINT for interrupt
* EVSYS for events
* UPDI for debug

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