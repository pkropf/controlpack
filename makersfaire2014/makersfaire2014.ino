// Copyright (c) 2014 Peter Kropf. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// control the 27 mini and 1 large solenoids for accumulator flame
// effects used at maker faire 2014.
//
// the 27 minis are used as a countdown timer for the 1 large
// poofer. the minis countdown by triggering n times, wait for n
// seconds, then trigger n-1 times, etc where n is the fibonacci
// sequence running in reverse from 610.


const int fibonacci[] = {610, 377, 233, 144, 89, 55, 34, 21, 13, 8, 5, 3, 2, 1, 1};

const int msolenoid_count = 27;    // mini solenoid count
int msolenoids[] = {               // mini solenoid pins
  22, 23, 24, 25, 26, 27, 28,
  29, 30, 31, 32, 33, 34, 35,
  36, 37, 38, 39, 40, 41, 42
};
const int mopen_min = 62;          // minimun duration an msolenoid should remain open in milliseconds
const int mopen_max = 200;         // maximum duration an msolenoid should remain open in milliseconds

const int bsolenoid = 4;           // big solenoid pin
const int bduration = 2000;        // duration big solenoid should remain open in milliseconds


void setup() {
  Serial.begin(57600);

  for (int idx = 0; idx < msolenoid_count; idx++) {
    pinMode(msolenoids[idx], OUTPUT);
  }

  pinMode(bsolenoid, OUTPUT);

  randomSeed(analogRead(0));
}


void trigger(int pin, int duration) {
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
}


void loop() {
  for (int fidx = 0; fidx < sizeof(fibonacci) / sizeof(fibonacci[0]); fidx++) {
    delay(fibonacci[fidx] * 1000);

    for (int cidx = 0; cidx < fibonacci[fidx]; cidx++) {
      int wait = random(mopen_min, mopen_max);
      trigger(random(0, sizeof(msolenoids) / sizeof(msolenoids[0])), wait);
      delay(wait);
    }
  }

  trigger(bsolenoid, bduration);
}

