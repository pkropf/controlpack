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

// test control panel configuration and wiring


int relay[] = {
  30, 32, 34, 36,
  31, 33, 35, 37,
  22, 24, 26, 28,
  23, 25, 27, 29,
  38, 40, 42, 44,
  39, 41, 43, 45,
  47, 49, 46, 48,
};
int relay_count = sizeof(relay) / sizeof(relay[0]);    // mini solenoid count


void setup() {
  Serial.begin(57600);

  for (int idx = 0; idx < relay_count; idx++) {
    pinMode(relay[idx], OUTPUT);
    digitalWrite(relay[idx], HIGH);
  }
}


void trigger(int pin, int duration) {
  //Serial.println(pin);
  digitalWrite(pin, LOW);
  delay(duration);
  digitalWrite(pin, HIGH);
  //delay(duration);
}


void loop() {
  for (int idx = 0; idx < relay_count; idx++) {
    trigger(relay[idx], 125);
  }
}

