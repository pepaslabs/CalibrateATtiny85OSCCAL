/*
 CalibrateATtiny85OSCCAL: Tweak the OSCCAL register to calibrate the internal oscillator of the ATtiny85.
 Copyright Jason Pepas (Pepas Labs, LLC)
 Released under the terms of the MIT License.
 
see https://learn.sparkfun.com/tutorials/serial-communication
in particular: https://dlnmh9ip6v2uc.cloudfront.net/assets/1/8/d/c/1/51142c09ce395f0e7e000002.png
see also http://web.alfredstate.edu/weimandn/miscellaneous/ascii/ascii_index.html

consider ASCII '8':

'8' in binary: 00111000

serial is transmitted LSB first, so that would look backwards on
an oscilloscope: 00011100

the start bit is a 0, and the stop bit is a 1, so the entire byte
looks like: 0000111001

before the start bit arrives (when the line is idle), the line is
high.  so you can consider that to be an infinite seris of 1's before
the start bit:

...11111111111111110000111001

additionally, the line stays high after the stop bit, so on a scope
you'd really just see two 'dips', the first being 4 bits wide and the
second 2 bits wide.

...111111111111111100001110011111111111111111...

we'll briefly represent that this way:

...10000111001...

or, visually:

...-____---__-...

here are some interesting characters:

ch    binary     logical             oscilloscope
===   ========   =================   =================
NUL   00000000   ...10000000001...   ...-_________-...
'~'   01111110   ...10011111101...   ...-__------_-...
'x'   01111000   ...10000111101...   ...-____----_-...
'8'   00111000   ...10000111001...   ...-____---__-...
' '   00100000   ...10000001001...   ...-______-__-...
'@'   01000000   ...10000000101...   ...-_______-_-...
'U'   01010101   ...10101010101...   ...-_-_-_-_-_-...
*/

// ATiny85 pinout:
//
//                                      +--\/--+
//        (PCINT5/!RESET/ADC0/dW) PB5  -|1    8|-  VCC
// (PCINT3/XTAL1/CLKI/!OC1B/ADC3) PB3  -|2    7|-  PB2 (SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
//  (PCINT4/XTAL2/CLKO/OC1B/ADC2) PB4  -|3    6|-  PB1 (MISO/DO/AIN1/OC0B/OC1A/PCINT1)
//                                GND  -|4    5|-  PB0 (MOSI/DI/SDA/AIN0/OC0A/!OC1A/AREF/PCINT0)
//                                      +------+

#define ARDUINO_PIN_D0 0
#define ARDUINO_PIN_D1 1
#define ARDUINO_PIN_D2 2
#define ARDUINO_PIN_D3 3
#define ARDUINO_PIN_D4 4
#define ARDUINO_PIN_D5 5

#define ATTINY_PIN_1 ARDUINO_PIN_D5
#define ATTINY_PIN_2 ARDUINO_PIN_D3
#define ATTINY_PIN_3 ARDUINO_PIN_D4
#define ATTINY_PIN_5 ARDUINO_PIN_D0
#define ATTINY_PIN_6 ARDUINO_PIN_D1
#define ATTINY_PIN_7 ARDUINO_PIN_D2

// project pinout:
//
//        +--\/--+
//   D5  -|1    8|-  5V
//   D3  -|2    7|-  D2
//   D4  -|3    6|-  D1  TX_pin
//  GND  -|4    5|-  D0  RX_pin
//        +------+

#define TX_pin ARDUINO_PIN_D1
#define RX_pin ARDUINO_PIN_D0


#include <SoftwareSerial.h>
SoftwareSerial ss(RX_pin, TX_pin);

void setup()
{
//  OSCCAL = 162;
  
  pinMode(RX_pin, INPUT);
  pinMode(TX_pin, OUTPUT);
 
  ss.begin(9600);

  printOSCCAL();
}

void loop()
{
  while (!ss.available()) delay(1);
  char ch = ss.read();
  delay(1);
  
  switch(ch)
  {    
    case ']':
      OSCCAL = OSCCAL + 1;
      printOSCCAL();
      break;
    
    case '[':
      OSCCAL = OSCCAL - 1;
      printOSCCAL();
      break;
    
    case '?':
      printOSCCAL();
      break;
    
    case '0':
      printNx100Chars(10);
      break;
    
    default:
      ss.println(ch);
      break;
  }
}

void printOSCCAL()
{
  ss.print("OSCCAL: ");
  ss.println(OSCCAL);
}

void printNx100Chars(uint16_t n)
{
  for (uint16_t i=0; i<n; i++)
  {
    ss.print("********* ********* ********* ********* ********* ********* ********* ********* ********* *********!");  
  }
  ss.println(n*100);
}

