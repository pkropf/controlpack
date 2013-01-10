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


int range_lower =  125;   // low end for poof rate in milliseconds
int range_upper = 2000;   // upper end for poof rate in milliseconds
int halfway = 531;        // potentiometers are not linear in their sweep, 0 to 531 is the left 
                          //   side range and 532 to 1023 is the right side range of the dial
                          //   movement
int flatspot = 20;        // 
int duration = 31;        // 1/32 second open time
int potPin = A2;
int ledPin = 13;
int val = 0;


void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}


void loop() {
  int m = 0;
  val = analogRead(potPin);
  Serial.print(val);
  if (val <= halfway - flatspot) {
    Serial.print(" - left - ");
    m = map(val, 0, halfway - flatspot, range_lower, range_upper);
  } else {
    if (val > halfway + flatspot) {
      Serial.print(" - right - ");
      m = map(val, halfway + flatspot + 1, 1023, range_upper, range_lower);
    } else {
      Serial.print(" - flatspot - ");
      m = 0;
    }
  }
  Serial.println(m);

  digitalWrite(ledPin, HIGH);
  delay(.25); 
  digitalWrite(ledPin, LOW);
  delay(.25);
}

