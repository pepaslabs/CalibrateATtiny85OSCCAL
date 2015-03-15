# CalibrateATtiny85OSCCAL
Tweak the OSCCAL register to calibrate the internal oscillator of the ATtiny85.

## Background:

You can free up a few pins on the ATtiny85 by foregoing an external crystal oscillator in favor of the ATtiny's internal oscillator.  However, the internal oscillator is much less accurate than an external crystal.

## Problem:

Serial communications rely on the two devices having (relatively) synchornized clocks, otherwise the bytes they exchange will get corrupted.

## Solution:

You can tweak the internal oscillator of the ATTiny85 by using the OSCCAL register.

We will send a byte from device A to device B, and then from device B back to device A (where device A is probably a USB-to-serial adapter for your PC and device B is an ATtiny85).  We will use an oscilloscope to measure the time of the transmission going each way, then tweak OSCCAL until they match.

