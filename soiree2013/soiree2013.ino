// Copyright (c) 2013 Peter Kropf. All rights reserved.
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


int range_lower =  31;   // low end for poof rate in milliseconds
int range_upper = 500;   // upper end for poof rate in milliseconds
int halfway = 531;        // potentiometers are not linear in their sweep, 0 to 531 is the left 
                          //   side range and 532 to 1023 is the right side range of the dial
                          //   movement
int flatspot = 20;        // define a range within which we'll be at the top of the potentiometer
int duration = 0;
int duration_low = 15;        // 1/32 second open time for a stack pin
int duration_high = 250;   
int potPin = A2;          // which pin to read the pot value
int ledPin = 13;          // ping to trip to show that we're doing something

int stack_count = 4;                       // number of poofers on the stacks
int east_stack_pins[4] = {35, 34, 33, 32};     // output pins for the east stack
int west_stack_pins[4] = {28, 29, 30, 31};     // output pins for the west stack
int stack_idx = 0;                         // current pin index
int stack_active_pin = 9;                  // pin to read when checking if the stack is active
int stack_high = false;                    // indicator for any stack pin being high
unsigned long stack_last = millis();       // when was the last time we tripped a pin?


void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);

  pinMode(stack_active_pin, INPUT);         // ping to look for the stack sequencer switch
  digitalWrite(stack_active_pin, HIGH);     // turn on pullup resistor

  for (int i = 0; i++; i < stack_count) {
    pinMode(east_stack_pins[i], OUTPUT);
    pinMode(west_stack_pins[i], OUTPUT);
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
  
  if (stack_high == false) {          // there are no stacks pins set high
    if ((now - stack_last) > await) {
      digitalWrite(east_stack_pins[stack_idx], HIGH);
      digitalWrite(west_stack_pins[stack_idx], HIGH);
      digitalWrite(ledPin, HIGH);
      stack_high = true;
      stack_last = millis();
      // Serial.println("stack high");
    }

  } else {
    if ((now - stack_last) > duration) {
      digitalWrite(east_stack_pins[stack_idx], LOW);
      digitalWrite(west_stack_pins[stack_idx], LOW);
      digitalWrite(ledPin, LOW);
      stack_high = false;
      stack_last = millis();
      // Serial.println("stack low");

      if (wait < 0) {
        if (stack_idx == 0) {
          stack_idx = stack_count - 1;
        } else {
          stack_idx -= 1;
        }
      } else {
        if (wait > 0) {
          if (stack_idx == stack_count - 1) {
            stack_idx = 0;
          } else {
            stack_idx += 1;
          }
        }
      }
    }
  }
}


void loop()
{
  if (digitalRead(stack_active_pin) == LOW) {       // we've got a button press
    trigger_next(transpose(analogRead(potPin)));

  } else {
    stack_idx = 0;

    for(int idx = 0; idx < stack_count; idx++) {
        digitalWrite(east_stack_pins[idx], LOW);
        digitalWrite(west_stack_pins[idx], LOW);
    }
    digitalWrite(ledPin, LOW);
  }
}

