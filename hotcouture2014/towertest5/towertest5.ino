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
const int pcount = 9;
const int don  = 125;     // how long the solenoid is on
const int doff = 125;     // how long till the next solenoid
const int sequence_pin = 1;             // pin to read when checking if the poofers are active
const int random_pin   = 0;             // pin to read when checking if the poofers are active

int cpin = 0;
int dran = 0;

void setup() {
  Serial.begin(9600);

  pinMode(sequence_pin, INPUT_PULLUP);
  pinMode(random_pin, INPUT_PULLUP);

  pinMode(ledPin, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(12, OUTPUT);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}


void next()
{
  switch (cpin) {
    case 0:
      digitalWrite(9,  HIGH);
      break;
    case 1:
      digitalWrite(10,  HIGH);
      break;
    case 2:
      digitalWrite(11,  HIGH);
      break;

    case 3:
      digitalWrite(2,  HIGH);
      break;
    case 4:
      digitalWrite(3,  HIGH);
      break;
    case 5:
      digitalWrite(4,  HIGH);
      break;

    case 6:
      digitalWrite(5,  HIGH);
      break;
    case 7:
      digitalWrite(6,  HIGH);
      break;
    case 8:
      digitalWrite(12,  HIGH);
      break;
      
  }
  digitalWrite(ledPin, HIGH);
  delay(don);

  switch (cpin) {
    case 0:
      digitalWrite(9,  LOW);
      break;
    case 1:
      digitalWrite(10,  LOW);
      break;
    case 2:
      digitalWrite(11,  LOW);
      break;
      
    case 3:
      digitalWrite(2,  LOW);
      break;
    case 4:
      digitalWrite(3,  LOW);
      break;
    case 5:
      digitalWrite(4,  LOW);
      break;

    case 6:
      digitalWrite(5,  LOW);
      break;
    case 7:
      digitalWrite(6,  LOW);
      break;
    case 8:
      digitalWrite(12,  LOW);
      break;
      
  }
  digitalWrite(ledPin, LOW);
  delay(doff);

  cpin += 1;

  if (cpin == pcount) {
    cpin = 0;
  }
}


void loop() {
  if (digitalRead(sequence_pin) == LOW) {       // we've got a button press
    dran = random(don / 2, doff);
    next();
  }

  if (digitalRead(random_pin) == LOW) {       // we've got a button press
    dran = random(don / 2, doff);
    cpin = random(0, pcount);
    next();
  }
}

