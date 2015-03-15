# CalibrateATtiny85OSCCAL
Tweak the OSCCAL register to calibrate the internal oscillator of the ATtiny85.

## Background:

You can free up a few pins on the ATtiny85 by foregoing an external crystal oscillator in favor of the ATtiny's internal oscillator.  However, the internal oscillator is much less accurate than an external crystal.

## Problem:

Serial communications rely on the two devices having (relatively) synchornized clocks, otherwise the bytes they exchange will get corrupted.  When using the ATtiny's internal, you might get hit by this problem.

## Solution:

You can tweak the internal oscillator of the ATTiny85 by using the OSCCAL register.

We will send a byte from device A to device B, and then from device B back to device A (where device A is probably a USB-to-serial adapter for your PC and device B is an ATtiny85).  We will use an oscilloscope to measure the time of the transmission going each way, then tweak OSCCAL until they match.

![](/github%20media/IMG_1758.PNG)

This is what the ASCII 'U' character looks like on a serial connection (as 5V TTL) at 9600 baud, 8N1:

![](/github%20media/NewFile2.png)

We can increment or decrement the OSCCAL value by sending '[' or ']' characters.  Each time we change OSCCAL, the program will send 50 'U' characters across the serial connection, allowing us to see the byte corruption as it crops up:

![](/github%20media/decreasing_osccal.png)

A 'U' character has the most number of voltage changes, and a NUL character has the least, which makes it suitable for measurement.  Here's a NUL on the serial line (as 5V TTL):

![](/github%20media/NewFile0.png)

On a Rigol DC1102E oscilloscope, go Measure -> Time -> -Width to add "dwell" measurement.  Here, we see that an ASCII NUL from my PC to the ATtiny measures 944 microseconds:

![](/github%20media/NewFile1.png)

Similarly, an ASCII NUL going from the ATtiny back to my PC also measures 944 microseonds.  Both of these measurements matching up means both sides are transmitting / receiving at the same speed.  This means we shouldn't see any corrupted bytes.  (Also note how much cleaner the ATtiny's TX line is compared to the noisy TX line of my PC's USB-to-serial adapter!):

![](/github%20media/NewFile3.png)

If we bring the OSCCAL down from 162 to 153, we start to see some corrupted bytes.  This corresponds to a 'dwell' measurement of 984 microseconds: 

![](/github%20media/153osccalnul.png)
![](/github%20media/NewFile4.png)
