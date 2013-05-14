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


typedef void(*tripperPtr)();


const int t_count = 5;
const int t1_idx  = 0;
const int t2_idx  = 1;
const int t3a_idx = 2;
const int t3b_idx = 3;
const int t3c_idx = 4;


const int pin[5]               = {     4,     5,     6,     7,     8 };  // digital i/o pins
const int min_open[5]          = {  2000,  125,     62,    62,    62 };  // minumum time to remain open
const int max_open[5]          = {  2000,  250,    250,   250,   250 };  // maximum time to remain open
const unsigned long refresh[5] = { 60000, 60000, 10000, 10000, 10000 };  // time to refresh tank
int state[5]                   = {     0,     0,     0,     0,     0 };  // current state
unsigned long last_open[5]     = {     0,     0,     0,     0,     0 };  // last time opened
int remain_open[5]             = {     0,     0,     0,     0,     0 };  // time to remain open


void all_off()
{
  digitalWrite(pin[t1_idx], LOW);
  state[t1_idx] = 0;
  digitalWrite(pin[t2_idx], LOW);
  state[t2_idx] = 0;
  digitalWrite(pin[t3a_idx], LOW);
  state[t3a_idx] = 0;
  digitalWrite(pin[t3b_idx], LOW);
  state[t3b_idx] = 0;
  digitalWrite(pin[t3c_idx], LOW);
  state[t3c_idx] = 0;
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
  }
}


void trip_all()
{
  // Serial.println("trip all");
  for (int idx = 0; idx < t_count; idx++) {
    trip(idx);
  }
}


void trip_t1()
{
  // Serial.println("trip t1");
  trip(t1_idx);
}


void trip_t2()
{
  // Serial.println("trip t2");
  trip(t2_idx);
}


void trip_t2_boomboom()
{
  unsigned long last = millis() - last_open[t2_idx];

  // Serial.println("trip t2 boomboom");

  all_off();

  if (last > refresh[t2_idx]) {
    delay(refresh[t2_idx]);
  }

  digitalWrite(pin[t2_idx], HIGH);
  delay(min_open[t2_idx]);
  digitalWrite(pin[t2_idx], LOW);
  delay(random(min_open[t2_idx], max_open[t2_idx]));
  digitalWrite(pin[t2_idx], HIGH);
  delay(max_open[t2_idx]);
  digitalWrite(pin[t2_idx], LOW);

  last_open[t2_idx] = millis();
}


void trip_t3()
{
  // Serial.println("trip t3");
  trip(t3a_idx);
  trip(t3b_idx);
  trip(t3c_idx);
}


void trip_t3_spin()
{
  // Serial.println("trip t3 spin");
  int pause = random(min_open[t3a_idx], max_open[t3a_idx]);
  int rounds = random(1, 5);
  int direction = random(0, 1);
  int i1 = t3a_idx, i2 = t3b_idx, i3 = t3c_idx;

  if (direction == 1) {
    i1 = t3c_idx;
    i2 = t3b_idx;
    i3 = t3a_idx;
  }

  all_off();

  for (int i = 0; i < rounds; i++) {
    digitalWrite(pin[i1], HIGH);
    delay(pause);
    digitalWrite(pin[i1], LOW);

    digitalWrite(pin[i2], HIGH);
    delay(pause);
    digitalWrite(pin[i2], LOW);

    digitalWrite(pin[i3], HIGH);
    delay(pause);
    digitalWrite(pin[i3], LOW);
  }
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


const int trippers_count = 7;
tripperPtr trippers[7] = {
  trip_all,
  trip_t1,        trip_t1,
  trip_t2,        trip_t2_boomboom,
  trip_t3,        trip_t3_spin
};


void setup()
{
  Serial.begin(57600);

  for (int idx = 0; idx < t_count; idx++) {
    pinMode(pin[idx], OUTPUT);
  }

  randomSeed(analogRead(0));
}


void loop()
{
  int tidx = random(0, trippers_count);
  // tidx = random(trippers_count - 2, trippers_count);
  // tidx = 4;
  static unsigned long pause = 7000;
  static unsigned long last = 0;
  unsigned long now = millis();

  if (now > last + pause) {
    (*trippers[tidx])();
    last = now;
    pause = random(30000, 200000);
    // Serial.print("pause ");
    // Serial.println(pause);
  }
  close_down();
}
 
