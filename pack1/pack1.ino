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
#include "quadpack.h"


#define UP 1
#define DOWN -1
#define INACTIVE 0


const int relay_count = 4;                         // number of relays connected
int relays[relay_count] = {3, 4, 5, 6};            // relay pins
int state[relay_count] = {0, 0, 0, 0};             // expected state of the relays, 0 off, 1 on
unsigned long timers[relay_count] = {0, 0, 0, 0};  // timers for the relays

int sequence_active = INACTIVE;                    // a sequence is active
int sequence_current = 0;                          // current sequence relay
unsigned long sequence_millis = 0;                 // millis between sequence relays
unsigned long sequence_timer = 0;                  // time till current sequence step is complete

unsigned long last_heartbeat = millis();           // last time we heard a heartbeat from the coordinator
const int heartbeat_led_pin =  13;                 // led to flash when we're receiving a heartbeat
int heartbeat_led_state = LOW;                     // state of the heartbeat led
long heartbeat_led_previous = 0;                   // last time the heartbeat led was updated
long heartbeat_led_interval = 500;                 // interval at which to blink the heartbeat led


ControlPack cp(QUADPACK_ID, CP_MODEL_4PACK);


void kill_sequence()
{
  if (sequence_active != INACTIVE) {
    sequence_active = INACTIVE;

    for (int x = 0; x < relay_count; x++) {
      state[x] = 0;
      timers[x] = 0;
    }
  }
}


void heartbeat(uint8_t src, uint8_t dst)
{
  if (src == 0) {
    last_heartbeat = millis();
  }
}


void all_off(uint8_t src, uint8_t dst)
{
  kill_sequence();

  for (int x = 0; x < relay_count; x++) {
    state[x] = 0;
  }
}


void all_on(uint8_t src, uint8_t dst)
{
  kill_sequence();

  for (int x = 0; x < relay_count; x++) {
    state[x] = 1;
  }
}


void all_timed(uint8_t src, uint8_t dst, uint16_t m)
{
  unsigned long now = millis();

  kill_sequence();

  for (int x = 0; x < relay_count; x++) {
    state[x] = 1;
    timers[x] = now + m;
  }
}


void port_off(uint8_t src, uint8_t dst, uint8_t port)
{
  kill_sequence();

  if (port < relay_count) {
    state[port] = 0;
  }
}


void port_on(uint8_t src, uint8_t dst, uint8_t port)
{
  kill_sequence();

  if (port < relay_count) {
    state[port] = 1;
  }
}


void port_timed(uint8_t src, uint8_t dst, uint8_t port, uint16_t m)
{
  unsigned long now = millis();

  kill_sequence();

  if (port < relay_count) {
    state[port] = 1;
    timers[port] = now + m;
  }
}


void sequence_up(uint8_t src, uint8_t dst, uint16_t m)
{
    sequence_active = UP;
    sequence_current = 0;
    sequence_millis = m;
    sequence_timer = 0;
}


void sequence_down(uint8_t src, uint8_t dst, uint16_t m)
{
    sequence_active = DOWN;
    sequence_current = relay_count - 1;
    sequence_millis = m;
    sequence_timer = 0;
}


void setup()
{
  Serial.begin(9600);

  pinMode(heartbeat_led_pin, OUTPUT);      

  for (int x = 0; x < relay_count; x++) {
    pinMode(relays[x], OUTPUT);
  }

  cp.scb_all_off(all_off);
  cp.scb_all_on(all_on);
  cp.scb_all_timed(all_timed);
  cp.scb_port_off(port_off);
  cp.scb_port_on(port_on);
  cp.scb_port_timed(port_timed);
  cp.scb_sequence_up(sequence_up);
  cp.scb_sequence_down(sequence_down);

  cp.send_version(CP_EVERYONE);
  cp.send_model(CP_EVERYONE);
}


void check_heartbeat()
{
  unsigned long now = millis();

  if (now - last_heartbeat > 2000) {
    for (int x = 0; x < relay_count; x++) {
      state[x] = 0;
      timers[x] = 0;
      sequence_active = INACTIVE;
      sequence_timer = 0;
    }
  } else {
    unsigned long now = millis();

    if(now - heartbeat_led_previous > heartbeat_led_interval) {
      heartbeat_led_previous = now;

      if (heartbeat_led_state == LOW)
        heartbeat_led_state = HIGH;
      else
        heartbeat_led_state = LOW;

      digitalWrite(heartbeat_led_pin, heartbeat_led_state);
    }
  }
}


void sequence_step()
{
  unsigned long now = millis();

  switch (sequence_active) {
  case UP:
    if (sequence_current < relay_count && sequence_timer < now) {

      sequence_timer = now + sequence_millis;
      timers[sequence_current] = sequence_timer;
      sequence_current += 1;
    }
    break;

  case DOWN:
    if (sequence_current >= 0 && sequence_timer < now) {

      sequence_timer = now + sequence_millis;
      timers[sequence_current] = sequence_timer;
      sequence_current -= 1;
    }
    break;

  }
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
  sequence_step();
  check_timers();
  check_heartbeat();
  update_relays();
}
