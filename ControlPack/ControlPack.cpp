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


#include "ControlPack.h"


ControlPack::ControlPack(uint8_t me)
{
    _me = me;

	_bindex          = 0;
    _received_header = 0;
    _expected_length = 0;
    _last_heartbeat  = 0;

    _cb_test          = 0;
    _cb_heartbeat     = 0;
    _cb_version_query = 0;
    _cb_version_info  = 0;
    _cb_model_query   = 0;
    _cb_model_info    = 0;
    _cb_port_query    = 0;
    _cb_port_info     = 0;
    _cb_all_off       = 0;
    _cb_all_on        = 0;
    _cb_port_on       = 0;
    _cb_port_off      = 0;
    _cb_sequence_up   = 0;
    _cb_sequence_down = 0;
    _cb_timed_on      = 0;
}


void ControlPack::loop()
{
    build_packet();
    send_heartbeat();
}


void ControlPack::send(uint8_t cmd, uint8_t dst)
{
    uint8_t buffer[CP_MAX_PACKET];
    
    buffer[0] = CPC_HEADER;
    buffer[1] = 0x03;
    buffer[2] = _me;
    buffer[3] = dst;
    buffer[4] = cmd;
    buffer[5] = CPC_FOOTER;

    Serial.write(buffer, 0x06);
}


void ControlPack::send_heartbeat()
{
    unsigned long m = millis();

    // if we haven't wrapped millis, and 20ms have not passed, then don't do anything
    if ( m >= _last_heartbeat && m < _last_heartbeat + CP_HEARTBEAT_RATE)
        return;

    _last_heartbeat = m;
    send(CPC_HEARTBEAT, CP_EVERYONE);
}


void ControlPack::build_packet()
{
    uint8_t b = 0;

    if (Serial.available()) {
        b = Serial.read();

        switch (b) {
        case CPC_FOOTER:
            if (_expected_length == _bindex) {
                parse();
            }
            break;

        case CPC_HEADER:
            _received_header = 1;
            break;

        default:
            if (_received_header) {
                if (_expected_length == 0) {
                    _expected_length = b;
                } else {
                    _buffer[_bindex++] = b;
                }
            }

            // ignore any bytes received w/o proper header being seen
            break;
        }

    }
}


void ControlPack::parse()
{
    uint8_t src = _buffer[0];
    uint8_t dst = _buffer[1];
    uint8_t cmd = _buffer[2];

    if (dst == _me || dst == CP_EVERYONE) {
        switch (cmd) {
        case CPC_HEARTBEAT:
            if (_cb_heartbeat != 0) {
                _cb_heartbeat(src, dst, 0, 0);
            }
            break;

        case CPC_VERSION_QUERY:
            if (_cb_version_query != 0) {
                _cb_version_query(src, dst, 0, 0);
            }
            break;

        case CPC_VERSION_INFO:
            if (_cb_version_info != 0) {
                _cb_version_info(src, dst, 0, 0);
            }
            break;

        case CPC_MODEL_QUERY:
            if (_cb_model_query != 0) {
                _cb_model_query(src, dst, 0, 0);
            }
            break;

        case CPC_MODEL_INFO:
            if (_cb_model_info != 0) {
                _cb_model_info(src, dst, 0, 0);
            }
            break;

        case CPC_PORT_QUERY:
            if (_cb_port_query != 0) {
                _cb_port_query(src, dst, 0, 0);
            }
            break;

        case CPC_PORT_INFO:
            if (_cb_port_info != 0) {
                _cb_port_info(src, dst, 0, 0);
            }
            break;

        case CPC_ALL_OFF:
            if (_cb_all_off != 0) {
                _cb_all_off(src, dst, 0, 0);
            }
            break;

        case CPC_ALL_ON:
            if (_cb_all_on != 0) {
                _cb_all_on(src, dst, 0, 0);
            }
            break;

        case CPC_PORT_ON:
            if (_cb_port_on != 0) {
                _cb_port_on(src, dst, 0, 0);
            }
            break;

        case CPC_PORT_OFF:
            if (_cb_port_off != 0) {
                _cb_port_off(src, dst, 0, 0);
            }
            break;

        case CPC_SEQUENCE_UP:
            if (_cb_sequence_up != 0) {
                _cb_sequence_up(src, dst, 0, 0);
            }
            break;

        case CPC_SEQUENCE_DOWN:
            if (_cb_sequence_down != 0) {
                _cb_sequence_down(src, dst, 0, 0);
            }
            break;

        case CPC_TIMED_ON:
            if (_cb_timed_on != 0) {
                _cb_timed_on(src, dst, 0, 0);
            }
            break;

        default:
            break;
        }
    }
}


void ControlPack::scb_test(cbfp fp)
{
    _cb_test = fp;
}


void ControlPack::scb_heartbeat(cbfp fp)
{
    _cb_heartbeat = fp;
}


void ControlPack::scb_version_query(cbfp fp)
{
    _cb_version_query = fp;
}


void ControlPack::scb_version_info(cbfp fp)
{
    _cb_version_info = fp;
}


void ControlPack::scb_model_query(cbfp fp)
{
    _cb_model_query = fp;
}


void ControlPack::scb_model_info(cbfp fp)
{
    _cb_model_info = fp;
}


void ControlPack::scb_port_query(cbfp fp)
{
    _cb_port_query = fp;
}


void ControlPack::scb_port_info(cbfp fp)
{
    _cb_port_info = fp;
}


void ControlPack::scb_all_off(cbfp fp)
{
    _cb_all_off = fp;
}


void ControlPack::scb_all_on(cbfp fp)
{
    _cb_all_on = fp;
}


void ControlPack::scb_port_on(cbfp fp)
{
    _cb_port_on = fp;
}


void ControlPack::scb_port_off(cbfp fp)
{
    _cb_port_off = fp;
}


void ControlPack::scb_sequence_up(cbfp fp)
{
    _cb_sequence_up = fp;
}


void ControlPack::scb_sequence_down(cbfp fp)
{
    _cb_sequence_down = fp;
}


void ControlPack::scb_timed_on(cbfp fp)
{
    _cb_timed_on = fp;
}
