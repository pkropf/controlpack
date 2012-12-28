/* -*- coding: utf-8 -*- */

/* Copyright (c) 2012 Peter Kropf. All rights reserved.

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <ControlPack.h>


const int relay_count = 4;                         // number of relays connected
int relays[relay_count] = {3, 4, 5, 6};             // relay pins
int state[relay_count] = {0, 0, 0, 0};             // expected state of the relays, 0 off, 1 on
unsigned long timers[relay_count] = {0, 0, 0, 0};  // duration timers for the relays


ControlPack cp(1, CP_MODEL_4PACK);


void all_off(uint8_t src, uint8_t dst)
{
  for (int x = 0; x < relay_count; x++) {
    state[x] = 0;
  }
}


void all_on(uint8_t src, uint8_t dst)
{
  for (int x = 0; x < relay_count; x++) {
    state[x] = 1;
  }
}


void all_timed(uint8_t src, uint8_t dst, uint16_t m)
{
  unsigned long now = millis();

  for (int x = 0; x < relay_count; x++) {
    state[x] = 1;
    timers[x] = now + m;
  }
}


void setup()
{
  Serial.begin(9600);

  for (int x = 0; x < relay_count; x++) {
    pinMode(relays[x], OUTPUT);
  }

  cp.scb_all_off(all_off);
  cp.scb_all_on(all_on);
  cp.scb_all_timed(all_timed);

  cp.send_version(CP_EVERYONE);
  cp.send_model(CP_EVERYONE);
}


void check_timers()
{
  unsigned long now = millis();

  for (int x = 0; x < relay_count; x++) {
    if (now > timers[x]) {
      state[x] == 0;
    }
  }
}


void update_relays()
{
  for (int x = 0; x < relay_count; x++) {
    if (state[x] == 1) {
      digitalWrite(relays[x], HIGH);
    } else {
      digitalWrite(relays[x], LOW);
    }
  }
}


void loop()
{
  cp.loop();
  check_timers();
  update_relays();
}

