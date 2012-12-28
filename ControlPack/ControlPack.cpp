/* -*- coding: utf-8 -*- */

/* Copyright (c) 2012 Peter Kropf. All rights reserved.
 *
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


ControlPack::ControlPack(uint8_t me, uint8_t model)
{
    _me = me;
    _model = model;

	_bindex          = 0;
    _received_header = 0;
    _expected_length = 0;
    _last_heartbeat  = 0;

    _cb_heartbeat     = 0;
    _cb_version_info  = 0;
    _cb_model_info    = 0;
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
    read_packet();
    send_heartbeat();
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


void ControlPack::send_version(uint8_t dst)
{
    sendb2(CPC_VERSION_INFO, dst, CP_VERSION);
}


void ControlPack::send_model(uint8_t dst)
{
    sendb1(CPC_MODEL_INFO, dst, _model);
}


void ControlPack::send_ports(uint8_t dst)
{
    uint8_t ports = 0;

    switch (_model) {
    case CP_MODEL_4PACK:
        ports = 4;
        break;

    case CP_MODEL_8PACK:
        ports = 8;
        break;
    }

    sendb1(CPC_PORT_INFO, dst, ports);
}


void ControlPack::send_all_off(uint8_t dst)
{
    send(CPC_ALL_OFF, dst);
}


void ControlPack::send_all_on(uint8_t dst)
{
    send(CPC_ALL_ON, dst);
}


void ControlPack::send_port_off(uint8_t dst, uint8_t port)
{
    sendb1(CPC_PORT_OFF, dst, port);
}


void ControlPack::send_port_on(uint8_t dst, uint8_t port)
{
    sendb1(CPC_PORT_ON, dst, port);
}


void ControlPack::send_sequence_up(uint8_t dst, uint16_t millis)
{
    sendb2(CPC_SEQUENCE_UP, dst, millis);
}


void ControlPack::send_sequence_down(uint8_t dst, uint16_t millis)
{
    sendb2(CPC_SEQUENCE_DOWN, dst, millis);
}


void ControlPack::send_timed_on(uint8_t dst, uint8_t port, uint16_t millis)
{
    sendb1b2(CPC_TIMED_ON, dst, port, millis);
}


void ControlPack::read_packet()
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
                _cb_heartbeat(src, dst);
            }
            break;

        case CPC_VERSION_QUERY:
            send_version(dst);
            break;

        case CPC_VERSION_INFO:
            if (_cb_version_info != 0) {
                uint16_t version = (_buffer[3] << 8) + _buffer[4];

                _cb_version_info(src, dst, version);
            }
            break;

        case CPC_MODEL_QUERY:
            send_model(dst);
            break;

        case CPC_MODEL_INFO:
            if (_cb_model_info != 0) {
                _cb_model_info(src, dst, _buffer[3]);
            }
            break;

        case CPC_PORT_QUERY:
            send_ports(dst);
            break;

        case CPC_PORT_INFO:
            if (_cb_port_info != 0) {
                _cb_port_info(src, dst, _buffer[3]);
            }
            break;

        case CPC_ALL_OFF:
            if (_cb_all_off != 0) {
                _cb_all_off(src, dst);
            }
            break;

        case CPC_ALL_ON:
            if (_cb_all_on != 0) {
                _cb_all_on(src, dst);
            }
            break;

        case CPC_PORT_ON:
            if (_cb_port_on != 0) {
                _cb_port_on(src, dst, _buffer[3]);
            }
            break;

        case CPC_PORT_OFF:
            if (_cb_port_off != 0) {
                _cb_port_off(src, dst, _buffer[3]);
            }
            break;

        case CPC_SEQUENCE_UP:
            if (_cb_sequence_up != 0) {
                uint16_t millis = (_buffer[3] << 8) + _buffer[4];

                _cb_sequence_up(src, dst, millis);
            }
            break;

        case CPC_SEQUENCE_DOWN:
            if (_cb_sequence_down != 0) {
                uint16_t millis = (_buffer[3] << 8) + _buffer[4];

                _cb_sequence_down(src, dst, millis);
            }
            break;

        case CPC_TIMED_ON:
            if (_cb_timed_on != 0) {
                uint16_t millis = (_buffer[4] << 8) + _buffer[5];

                _cb_timed_on(src, dst, _buffer[3], millis);
            }
            break;

        default:
            break;
        }
    }
}


void ControlPack::send(uint8_t cmd, uint8_t dst)
{
    uint8_t buffer[CP_MAX_PACKET];
    
    buffer[0] = CPC_HEADER;
    buffer[1] = 3;
    buffer[2] = _me;
    buffer[3] = dst;
    buffer[4] = cmd;
    buffer[5] = CPC_FOOTER;

    Serial.write(buffer, 6);
}


void ControlPack::sendb1(uint8_t cmd, uint8_t dst, uint8_t p1)
{
    uint8_t buffer[CP_MAX_PACKET];
    
    buffer[0] = CPC_HEADER;
    buffer[1] = 4;
    buffer[2] = _me;
    buffer[3] = dst;
    buffer[4] = cmd;
    buffer[5] = p1;
    buffer[6] = CPC_FOOTER;

    Serial.write(buffer, 7);
}


void ControlPack::sendb2(uint8_t cmd, uint8_t dst, uint16_t p1)
{
    uint8_t buffer[CP_MAX_PACKET];
    
    buffer[0] = CPC_HEADER;
    buffer[1] = 5;
    buffer[2] = _me;
    buffer[3] = dst;
    buffer[4] = cmd;
    buffer[5] = uint8_t(p1 >> 8);
    buffer[6] = uint8_t((p1 << 8) >> 8);
    buffer[7] = CPC_FOOTER;

    Serial.write(buffer, 8);
}


void ControlPack::sendb1b2(uint8_t cmd, uint8_t dst, uint8_t p1, uint16_t p2)
{
    uint8_t buffer[CP_MAX_PACKET];
    
    buffer[0] = CPC_HEADER;
    buffer[1] = 6;
    buffer[2] = _me;
    buffer[3] = dst;
    buffer[4] = cmd;
    buffer[5] = p1;
    buffer[6] = uint8_t(p2 >> 8);
    buffer[7] = uint8_t((p2 << 8) >> 8);
    buffer[8] = CPC_FOOTER;

    Serial.write(buffer, 9);
}


void ControlPack::scb_heartbeat(cbfp fp)
{
    _cb_heartbeat = fp;
}


void ControlPack::scb_version_info(cbfpb2 fp)
{
    _cb_version_info = fp;
}


void ControlPack::scb_model_info(cbfpb1 fp)
{
    _cb_model_info = fp;
}


void ControlPack::scb_port_info(cbfpb1 fp)
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


void ControlPack::scb_port_on(cbfpb1 fp)
{
    _cb_port_on = fp;
}


void ControlPack::scb_port_off(cbfpb1 fp)
{
    _cb_port_off = fp;
}


void ControlPack::scb_sequence_up(cbfpb2 fp)
{
    _cb_sequence_up = fp;
}


void ControlPack::scb_sequence_down(cbfpb2 fp)
{
    _cb_sequence_down = fp;
}


void ControlPack::scb_timed_on(cbfpb1b2 fp)
{
    _cb_timed_on = fp;
}
