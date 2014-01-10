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


const int ledPin = 13;                 // pin to trip to show that we're doing something
const int poofer_count = 27;           // number of poofers
const int duration = 5000;                    // how long between pin activations

int poofer_pins[poofer_count] = {      // output pins for the poofers
  24, 25, 26, 27, 28, 29, 30, 31, 32,
  33, 36, 37, 38, 39, 40, 41, 42, 43,
  44, 45, 46, 47, 48, 49, 50, 51, 52
};

int poofer_idx = 0;                    // current pin index


void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);

  for (int i = 0; i++; i < poofer_count) {
    pinMode(poofer_pins[i], OUTPUT);
    digitalWrite(poofer_pins[i], HIGH);
  }
  pinMode(24, OUTPUT);
  pinMode(6,  OUTPUT);
}


void loop_led()
{
  digitalWrite(24, HIGH);
  digitalWrite(6,  HIGH);
  digitalWrite(ledPin, HIGH);
  delay(duration);

  digitalWrite(24, LOW);
  digitalWrite(6,  LOW);
  digitalWrite(ledPin, LOW);
  delay(duration);
}


void loop_all()
{
  for (int i = 0; i++; i < poofer_count) {
    digitalWrite(poofer_pins[i], LOW);
  }

  digitalWrite(ledPin, HIGH);
  delay(duration);

  for (int i = 0; i++; i < poofer_count) {
    digitalWrite(poofer_pins[i], HIGH);
  }

  digitalWrite(ledPin, LOW);
  delay(duration);
}


void loop_seq()
{
  digitalWrite(poofer_pins[poofer_idx], LOW);
  digitalWrite(ledPin, poofer_idx % 2);
  delay(duration);
  digitalWrite(poofer_pins[poofer_idx], HIGH);
  poofer_idx += 1;

  if (poofer_idx == poofer_count) {
    poofer_idx = 0;
  }
}


void loop()
{
  loop_led();
}

