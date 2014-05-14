// Copyright (c) 2014 Peter Kropf. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


const int range_lower =  31;           // low end for poof rate in milliseconds
const int range_upper = 500;           // upper end for poof rate in milliseconds
const int halfway = 531;               // potentiometers are not linear in their sweep, 0 to 531 is the left 
                                       //   side range and 532 to 1023 is the right side range of the dial
                                       //   movement
const int flatspot = 20;               // define a range within which we'll be at the top of the potentiometer
const int duration_low = 15;           // 1/32 second open time for a poofer pin
const int duration_high = 250;   
const int potPin = A2;                 // which pin to read the pot value
const int ledPin = 13;                 // pin to trip to show that we're doing something
const int poofer_count = 4;            // number of poofers

int duration = 0;
int poofer_pins[poofer_count] = {      // output pins for the poofers
  35, 34, 33, 32
};
int poofer_idx = 0;                    // current pin index
int poofer_active_pin = 9;             // pin to read when checking if the poofers are active
int poofer_high = false;               // indicator for any poofer pin being high
int poofer_all_pin = 10;               // pin to read when checking is all the poofers are on
int poofer_all_high = false;           // indicator for all the poofers being active
unsigned long poofer_last = millis();  // when was the last time we tripped a pin?


void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);

  pinMode(poofer_active_pin, INPUT_PULLUP);
  pinMode(poofer_all_pin, INPUT_PULLUP);
  
  for (int i = 0; i++; i < poofer_count) {
    pinMode(poofer_pins[i], OUTPUT);
  }
}


int transpose(int v)
{
  int m = 0;
  
  // Serial.print(v);

  if (v <= halfway - flatspot) {
    // Serial.print(" - left - ");
    m = 1 - map(v, 0, halfway - flatspot, range_lower, range_upper);
  } else {

    if (v > halfway + flatspot) {
      // Serial.print(" - right - ");
      m = map(v, halfway + flatspot + 1, 1023, range_upper, range_lower);
    } else {

      // Serial.print(" - flatspot - ");
      m = 0;
    }
  }
  // Serial.println(m);
  
  return m;
}


void trigger_next(int wait)
{
  int await = abs(wait);
  unsigned long now = millis();
  if (wait == 0) {
    return;
  }

  duration = map(await, range_lower, range_upper, duration_low, duration_high);
  // Serial.println(wait);
  
  if (poofer_high == false) {          // there are no poofer pins set high
    if ((now - poofer_last) > await) {
      digitalWrite(poofer_pins[poofer_idx], HIGH);
      digitalWrite(ledPin, HIGH);
      poofer_high = true;
      poofer_last = millis();
      // Serial.println("poofer high");
    }

  } else {
    if ((now - poofer_last) > duration) {
      digitalWrite(poofer_pins[poofer_idx], LOW);
      digitalWrite(ledPin, LOW);
      poofer_high = false;
      poofer_last = millis();
      // Serial.println("poofer low");

      if (wait < 0) {
        if (poofer_idx == 0) {
          poofer_idx = poofer_count - 1;
        } else {
          poofer_idx -= 1;
        }
      } else {
        if (wait > 0) {
          if (poofer_idx == poofer_count - 1) {
            poofer_idx = 0;
          } else {
            poofer_idx += 1;
          }
        }
      }
    }
  }
}


void loop()
{
  if (digitalRead(poofer_all_pin) == HIGH) {         // we've got a button press
    // Serial.println("poofer all high");
    if (poofer_all_high == false) {

      for(int idx = 0; idx < poofer_count; idx++) {
          digitalWrite(poofer_pins[idx], HIGH);
      }
      digitalWrite(ledPin, HIGH);
      poofer_all_high = true;
    }

  } else { // if poofer_all_pin
    // Serial.println("poofer all low");

    if (poofer_all_high == true) {
      for(int idx = 0; idx < poofer_count; idx++) {
          digitalWrite(poofer_pins[idx], LOW);
      }
      digitalWrite(ledPin, LOW);
      poofer_all_high = false;
    }

    if (digitalRead(poofer_active_pin) == LOW) {       // we've got a button press
      trigger_next(transpose(analogRead(potPin)));

    } else {
      poofer_idx = 0;

      for(int idx = 0; idx < poofer_count; idx++) {
          digitalWrite(poofer_pins[idx], LOW);
      }
      digitalWrite(ledPin, LOW);
    } // else poofer_active_pin
  } // else poofer_all_pin
}

