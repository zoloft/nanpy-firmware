#include "Display.h"
#include <Arduino.h>

static int hours = 0;
static int minutes = 0;
static int seconds = 0;

// fading transitions sketch for 8-tube board with default connections.
// based on 6-tube sketch by Emblazed

// 09/03/2010 - Added Poxin's 12 hour setting for removing 00 from hours when set to 12 hour time
// 06/16/2011 - 4-tube-itized by Dave B.
// 08/19/2011 - modded for six bulb board, hours, minutes, seconds by Brad L.
// 01/28/2013 - expanded to 8 digit crossfade by Brad L.
// 12/04/2014 - First V2 standardization version by Brad L.
// 01/07/2015 - Code Revision by Jeremy Howa

// SN74141 : Truth Table
//D C B A #
//L,L,L,L 0
//L,L,L,H 1
//L,L,H,L 2
//L,L,H,H 3
//L,H,L,L 4
//L,H,L,H 5
//L,H,H,L 6
//L,H,H,H 7
//H,L,L,L 8
//H,L,L,H 9

#define _DEBUG          0
#define _bUSE_COLONS    0
#define _BULB_COUNT     6
#define _bUSE_CROSSFADE 0

#define _BrightnessLevelDelay     7.14//  100.0f / 14.0f

// SN74141 (1)
static const int ledPin_0_a = 2;
static const int ledPin_0_b = 3;
static const int ledPin_0_c = 4;
static const int ledPin_0_d = 5;

// SN74141 (2)
static const int ledPin_1_a = 6;
static const int ledPin_1_b = 7;
static const int ledPin_1_c = 8;
static const int ledPin_1_d = 9;

// anode pins
// NOTE: V1 board has this order flipped
static const int ledPin_a_1 = 10;
static const int ledPin_a_2 = 11;
static const int ledPin_a_3 = 12;
static const int ledPin_a_4 = 13;

void _setup()
{
    pinMode(ledPin_0_a, OUTPUT);
    pinMode(ledPin_0_b, OUTPUT);
    pinMode(ledPin_0_c, OUTPUT);
    pinMode(ledPin_0_d, OUTPUT);

    pinMode(ledPin_1_a, OUTPUT);
    pinMode(ledPin_1_b, OUTPUT);
    pinMode(ledPin_1_c, OUTPUT);
    pinMode(ledPin_1_d, OUTPUT);

    pinMode(ledPin_a_1, OUTPUT);
    pinMode(ledPin_a_2, OUTPUT);
    pinMode(ledPin_a_3, OUTPUT);
    pinMode(ledPin_a_4, OUTPUT);

    // NOTE: Grounding on virtual pins 14 and 15 (analog pins 0 and 1) will set the Hour and Mins.

    pinMode( 14, INPUT ); // set the virtual pin 14 (pin 0 on the analog inputs )  SET HOURS
    digitalWrite(14, HIGH); // set pin 14 as a pull up resistor.

    pinMode( 15, INPUT ); // set the virtual pin 15 (pin 1 on the analog inputs )  SET MINUTES
    digitalWrite(15, HIGH); // set pin 15 as a pull up resistor.

    pinMode( 16, INPUT ); // set the virtual pin 16 (pin 1 on the analog inputs )  FREE TO USE AS INPUT OR OUTPUT
    digitalWrite(16, HIGH); // set pin 16 as a pull up resistor.

    pinMode( 17, INPUT ); // set the virtual pin 17 (pin 1 on the analog inputs )  FREE TO USE AS INPUT OR OUTPUT
    digitalWrite(17, HIGH); // set pin 17 as a pull up resistor.

    pinMode( 18, INPUT ); // set the virtual pin 18 (pin 1 on the analog inputs )  FREE TO USE AS INPUT OR OUTPUT
    digitalWrite(18, HIGH); // set pin 18 as a pull up resistor.

    pinMode( 19, INPUT ); // set the virtual pin 19 (pin 1 on the analog inputs )  FREE TO USE AS INPUT OR OUTPUT
    digitalWrite(19, HIGH); // set pin 19 as a pull up resistor.

    // Open serial communications:
    if( _DEBUG )
        Serial.begin(9600);
}

void SetSN74141Chips( int num2, int num1 )
{
    int a,b,c,d;

    // set defaults.
    a=0;b=0;c=0;d=0; // will display a zero.

    // Load the a,b,c,d.. to send to the SN74141 IC (1)
    switch( num1 )
    {
        case 0: a=0;b=0;c=0;d=0;break;
        case 1: a=1;b=0;c=0;d=0;break;
        case 2: a=0;b=1;c=0;d=0;break;
        case 3: a=1;b=1;c=0;d=0;break;
        case 4: a=0;b=0;c=1;d=0;break;
        case 5: a=1;b=0;c=1;d=0;break;
        case 6: a=0;b=1;c=1;d=0;break;
        case 7: a=1;b=1;c=1;d=0;break;
        case 8: a=0;b=0;c=0;d=1;break;
        case 9: a=1;b=0;c=0;d=1;break;
        default: a=1;b=1;c=1;d=1;
        break;
    }

    // Write to output pins.
    digitalWrite(ledPin_0_d, d);
    digitalWrite(ledPin_0_c, c);
    digitalWrite(ledPin_0_b, b);
    digitalWrite(ledPin_0_a, a);

    // Load the a,b,c,d.. to send to the SN74141 IC (2)
    switch( num2 )
    {
        case 0: a=0;b=0;c=0;d=0;break;
        case 1: a=1;b=0;c=0;d=0;break;
        case 2: a=0;b=1;c=0;d=0;break;
        case 3: a=1;b=1;c=0;d=0;break;
        case 4: a=0;b=0;c=1;d=0;break;
        case 5: a=1;b=0;c=1;d=0;break;
        case 6: a=0;b=1;c=1;d=0;break;
        case 7: a=1;b=1;c=1;d=0;break;
        case 8: a=0;b=0;c=0;d=1;break;
        case 9: a=1;b=0;c=0;d=1;break;
        default: a=1;b=1;c=1;d=1;
        break;
    }

    // Write to output pins
    digitalWrite(ledPin_1_d, d);
    digitalWrite(ledPin_1_c, c);
    digitalWrite(ledPin_1_b, b);
    digitalWrite(ledPin_1_a, a);
}

// Fade in/out times .. Values set for IN-17 Nixie Bulbs
static const float fadeMax    = 5.0f;
static const float fadeStep   = 0.3f;

int NumberArray[8]={0,0,0,0,0,0,0,0};
int currNumberArray[8]={0,0,0,0,0,0,0,0};
float NumberArrayFadeInValue[8]={0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
float NumberArrayFadeOutValue[8]={5.0f,5.0f,5.0f,5.0f,5.0f,5.0f,5.0f,5.0f};

void DisplayFadeNumberString()
{
    if( _bUSE_COLONS == 1 )
    {
        digitalWrite(ledPin_a_1, HIGH);
        SetSN74141Chips(0,0);
        delay(1.8f);
        SetSN74141Chips(1,1);
        delay(1.8f);
        SetSN74141Chips(2,2);
        delay(1.8f);
        SetSN74141Chips(3,3);
        delay(1.8f);
        digitalWrite(ledPin_a_1, LOW);
    }

    if( _bUSE_CROSSFADE == 1 )
    {
        // Anode channel 2 - numerals 1,5
        SetSN74141Chips(currNumberArray[1],currNumberArray[5]);
        digitalWrite(ledPin_a_2, HIGH);
            delay(NumberArrayFadeOutValue[1]);
        SetSN74141Chips(NumberArray[1],NumberArray[5]);
            delay(NumberArrayFadeInValue[1]);
        digitalWrite(ledPin_a_2, LOW);

        // Anode channel 3 - numerals 2,6
        SetSN74141Chips(currNumberArray[2],currNumberArray[6]);
        digitalWrite(ledPin_a_3, HIGH);
            delay(NumberArrayFadeOutValue[2]);
        SetSN74141Chips(NumberArray[2],NumberArray[6]);
            delay(NumberArrayFadeInValue[2]);
        digitalWrite(ledPin_a_3, LOW);

        // Anode channel 4 - numerals 3,7
        SetSN74141Chips(currNumberArray[3],currNumberArray[7]);
        digitalWrite(ledPin_a_4, HIGH);
            delay(NumberArrayFadeOutValue[3]);
        SetSN74141Chips(NumberArray[3],NumberArray[7]);
            delay(NumberArrayFadeInValue[3]);
        digitalWrite(ledPin_a_4, LOW);
    }
    else
    {
        // Anode channel 2 - numerals 1,5
        SetSN74141Chips(currNumberArray[1],currNumberArray[5]);
        digitalWrite(ledPin_a_2, HIGH);
            delay(_BrightnessLevelDelay);
        digitalWrite(ledPin_a_2, LOW);

        // Anode channel 3 - numerals 2,6
        SetSN74141Chips(currNumberArray[2],currNumberArray[6]);
        digitalWrite(ledPin_a_3, HIGH);
            delay(_BrightnessLevelDelay);
        digitalWrite(ledPin_a_3, LOW);

        // Anode channel 4 - numerals 3,7
        SetSN74141Chips(currNumberArray[3],currNumberArray[7]);
        digitalWrite(ledPin_a_4, HIGH);
            delay(_BrightnessLevelDelay);
        digitalWrite(ledPin_a_4, LOW);
    }

    if( _bUSE_CROSSFADE == 1 )
    {
        // Loop thru and update all the arrays, and fades.
        for( int i = 0 ; i < 8 ; i ++ ) //equal to & of digits
        {
            //if( NumberArray[i] != currNumberArray[i] )
            {
                NumberArrayFadeInValue[i] += fadeStep;
                NumberArrayFadeOutValue[i] -= fadeStep;

               if( NumberArrayFadeInValue[i] >= fadeMax )
               {
                   NumberArrayFadeInValue[i] = 0.0f;
                   NumberArrayFadeOutValue[i] = fadeMax; //affects the refresh cycle
                   currNumberArray[i] = NumberArray[i];
                }
            }
        }
    }
    else
    {
        for( int i = 0 ; i < 8 ; i ++ )
        {
            currNumberArray[i] = NumberArray[i];
        }
    }
}

// Defines
static const long SECS  = 1000;       // milliseconds in a Sec
static const long MINS  = 60;         // 60 Seconds in a Min.
static const long HOURS = 60 * MINS;  // 60 Mins in an hour.
static const long DAYS  = 12 * HOURS; // 24 Hours in a day. > Note: change the 24 to a 12 for non military time.

////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
void _loop() {
    // Get the high and low order values for hours,min,seconds.
    int lowerHours = hours % 10;
    int upperHours = hours - lowerHours;
    int lowerMins = minutes % 10;
    int upperMins = minutes - lowerMins;
    int lowerSeconds = seconds % 10;
    int upperSeconds = seconds - lowerSeconds;

    if( upperSeconds >= 10 )   upperSeconds = upperSeconds / 10;
    if( upperMins >= 10 )      upperMins = upperMins / 10;
    if( upperHours >= 10 )     upperHours = upperHours / 10;

    // Fill in the Number array used to display on the tubes.
    NumberArray[7] = upperHours;
    NumberArray[6] = lowerHours;
    NumberArray[5] = upperMins;

    // Slot four is used for dots .. should get them blinking ?? !!
    NumberArray[3] = lowerMins;
    NumberArray[2] = upperSeconds;
    NumberArray[1] = lowerSeconds;

    DisplayFadeNumberString();
}

nanpy::Display::Display(void) {}

nanpy::Display::~Display(void) {}

int nanpy::Display::getSeconds(void) {
  return seconds;
}

void nanpy::Display::setSeconds(int secs) {
  seconds = secs;
}

int nanpy::Display::getMinutes(void) {
  return minutes;
}

void nanpy::Display::setMinutes(int mins) {
  minutes = mins;
}

int nanpy::Display::getHours(void) {
  return hours;
}

void nanpy::Display::setHours(int hrs) {
  hours = hrs;
}

void nanpy::Display::setup(void) {
  _setup();
}

void nanpy::Display::loop(void) {
  _loop();
}
