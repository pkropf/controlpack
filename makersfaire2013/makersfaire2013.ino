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


// 3 fire effects on towers
// t1 - first has a 3" pneumatic valve w/ a 5-ish gallon accumilator tank
// t2 - second has a 2" solenoid valve w/ a 7-ish gallon accumilator tank
// t3 - third has 3 fire extinguisher tank accumulators


const int t_count = 5;
const int t1_idx  = 0;
const int t2_idx  = 1;
const int t3a_idx = 2;
const int t3b_idx = 3;
const int t3c_idx = 4;


const int pin[5]               = {     4,     5,     6,     7,     8 };  // digital i/o pins
const int min_open[5]          = {  2000,  125,     62,    62,    62 };  // minumum time to remain open
const int max_open[5]          = {  2000,  500,    500,   500,   500 };  // maximum time to remain open
const unsigned long refresh[5] = { 60000, 60000, 10000, 10000, 10000 };  // time to refresh tank
int state[5]                   = {     0,     0,     0,     0,     0 };  // current state
unsigned long last_open[5]     = {     0,     0,     0,     0,     0 };  // last time opened
int remain_open[5]             = {     0,     0,     0,     0,     0 };  // time to remain open


void setup()
{
  Serial.begin(57600);

  for (int idx = 0; idx < t_count; idx++) {
    pinMode(pin[idx], OUTPUT);
  }

  randomSeed(analogRead(0));
}


void show_state(int idx)
{
  Serial.print(idx);
  Serial.print(" ");
  Serial.print(remain_open[idx]);
  Serial.print(" ");
  Serial.print(last_open[idx]);
  Serial.print(" ");
  Serial.print(now);
  Serial.print(" ");
  Serial.print(last);
  Serial.print(" ");
  Serial.print(refresh[idx]);
  Serial.println("");
}


void trip(int idx)
{
  if (state[idx] == 0) {
    unsigned long now = millis();
    unsigned long last = now - last_open[idx];
    if (last > refresh[idx]) {
      state[idx] = 1;
      remain_open[idx] = random(min_open[idx], max_open[idx]);
      last_open[idx] = millis();
      digitalWrite(pin[idx], HIGH);
    }
    show_state();
  }
}


void trip_all()
{
  for (int idx = 0; idx < t_count; idx++) {
    trip(idx);
  }
}


void trip_t1()
{
  trip(t1_idx);
}


void trip_t2()
{
  trip(t2_idx);
}


void trip_t3()
{
  trip(t3a_idx);
  trip(t3b_idx);
  trip(t3c_idx);
}


void trip_t3a()
{
  trip(t3a_idx);
}


void trip_t3b()
{
  trip(t3b_idx);
}


void trip_t3c()
{
  trip(t3c_idx);
}


void close_down()
{
  for (int idx = 0; idx < t_count; idx++) {
    if (state[idx] == 1) {
      if ((millis() - last_open[idx]) > remain_open[idx]) {
        digitalWrite(pin[idx], LOW);
        state[idx] = 0;
      }
    }
  }
}


void loop()
{
  trip_all();
  close_down();
}

