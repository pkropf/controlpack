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
const int pcount = 3;
const int duration = 125;                    // how long between pin activations
const int pins[pcount] = {
  9, 10, 11
};

int cpin = 0;

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  for (int i = 0; i < pcount; i++) {
    pinMode(i,  OUTPUT);
    digitalWrite(i, LOW);
  }
}


void loop()
{
  digitalWrite(pins[cpin],  HIGH);
  digitalWrite(ledPin, HIGH);
  delay(duration);

  digitalWrite(pins[cpin],  LOW);
  digitalWrite(ledPin, LOW);
  delay(duration);

  cpin += 1;

  if (cpin == pcount) {
    cpin = 0;
  }
}

