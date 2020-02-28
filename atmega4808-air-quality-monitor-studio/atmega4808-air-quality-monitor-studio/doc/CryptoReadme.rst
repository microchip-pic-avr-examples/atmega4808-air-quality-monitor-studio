CryptoAuthLib - Microchip CryptoAuthentication Library
======================================================

Introduction
------------

This code base implements an object-oriented C library which supports
Microchip CryptoAuth devices. The family of devices supported currently
are:

-  `ATSHA204A <http://www.microchip.com/ATSHA204A>`__
-  `ATECC108A <http://www.microchip.com/ATECC108A>`__
-  `ATECC508A <http://www.microchip.com/ATECC508A>`__
-  `ATECC608A <http://www.microchip.com/ATECC608A>`__

Online documentation is at
https://microchiptech.github.io/cryptoauthlib/

Latest software and examples can be found at: -
http://www.microchip.com/SWLibraryWeb/product.aspx?product=CryptoAuthLib

Prerequisite skills: - strong C programming and code reading - Atmel
Studio familiarity - Knowledge of flashing microcontrollers with new
code - Familiarity with Microchip CryptoAuth device functionality


There are two major compiler defines that affect the operation of the
library. - ATCA\_NO\_POLL can be used to revert to a non-polling
mechanism for device responses. Normally responses are polled for after
sending a command, giving quicker response times. However, if
ATCA\_NO\_POLL is defined, then the library will simply delay the max
execution time of a command before reading the response. -
ATCA\_NO\_HEAP can be used to remove the use of malloc/free from the
main library. This can be helpful for smaller MCUs that don't have a
heap implemented. If just using the basic API, then there shouldn't be
any code changes required. The lower-level API will no longer use the
new/delete functions and the init/release functions should be used
directly.

Examples
--------

-  Watch `CryptoAuthLib
   Documents <http://www.microchip.com/design-centers/security-ics/cryptoauthentication/overview>`__
   for new examples coming online.
-  Node Authentication Example Using Asymmetric PKI is a complete,
   all-in-one example demonstrating all the stages of crypto
   authentication starting from provisioning the Crypto Authentication
   device ATECC608A/ATECC508A with keys and certificates to
   demonstrating an authentication sequence using asymmetric techniques.
   http://www.microchip.com/SWLibraryWeb/product.aspx?product=CryptoAuthLib
