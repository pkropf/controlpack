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


#ifndef __ControlPack_h
#define __ControlPack_h

#include "Arduino.h"


typedef void (*cbfp)(uint8_t, uint8_t);
typedef void (*cbfpb1)(uint8_t, uint8_t, uint8_t);
typedef void (*cbfpb2)(uint8_t, uint8_t, uint16_t);
typedef void (*cbfpb1b2)(uint8_t, uint8_t, uint8_t, uint16_t);


// control pack commands
#define CPC_HEARTBEAT      0x01
#define CPC_VERSION_QUERY  0x10
#define CPC_VERSION_INFO   0x11
#define CPC_MODEL_QUERY    0x12
#define CPC_MODEL_INFO     0x13
#define CPC_PORT_QUERY     0x14
#define CPC_PORT_INFO      0x15
#define CPC_ALL_OFF        0x20
#define CPC_ALL_ON         0x21
#define CPC_ALL_TIMED      0x22
#define CPC_PORT_OFF       0x23
#define CPC_PORT_ON        0x24
#define CPC_PORT_TIMED     0x25
#define CPC_SEQUENCE_UP    0x26
#define CPC_SEQUENCE_DOWN  0x27
#define CPC_FOOTER         0xfd
#define CPC_HEADER         0xfe


// version details
// upper byte is the major release number
// lower byte is the minor release number
#define CP_VERSION   0x0001


// model details
#define CP_MODEL_COORDINATOR 0
#define CP_MODEL_4PACK       1
#define CP_MODEL_8PACK       2


// control pack constants
#define CP_EVERYONE        0x20
#define CP_HEARTBEAT_RATE  500
#define CP_MAX_PACKET      32


class ControlPack
{
    public:
        ControlPack(uint8_t me, uint8_t model);

        void loop();

        void send_version(uint8_t dst);
        void send_model(uint8_t dst);
        void send_ports(uint8_t dst);
        void send_all_off(uint8_t dst);
        void send_all_on(uint8_t dst);
        void send_all_timed(uint8_t dst, uint16_t millis);
        void send_port_off(uint8_t dst, uint8_t port);
        void send_port_on(uint8_t dst, uint8_t port);
        void send_port_timed(uint8_t dst, uint8_t port, uint16_t millis);
        void send_sequence_up(uint8_t dst, uint16_t millis);
        void send_sequence_down(uint8_t dst, uint16_t millis);

        void scb_heartbeat(cbfp fp);
        void scb_version_info(cbfpb2 fp);
        void scb_model_info(cbfpb1 fp);
        void scb_port_info(cbfpb1 fp);
        void scb_all_off(cbfp fp);
        void scb_all_on(cbfp fp);
        void scb_all_timed(cbfpb2 fp);
        void scb_port_off(cbfpb1 fp);
        void scb_port_on(cbfpb1 fp);
        void scb_port_timed(cbfpb1b2 fp);
        void scb_sequence_up(cbfpb2 fp);
        void scb_sequence_down(cbfpb2 fp);


    private:
    	void parse();
    	void read_packet();
    	void send_heartbeat();
        void send(uint8_t cmd, uint8_t dst);
        void sendb1(uint8_t cmd, uint8_t dst, uint8_t p1);
        void sendb2(uint8_t cmd, uint8_t dst, uint16_t p1);
        void sendb1b2(uint8_t cmd, uint8_t dst, uint8_t p1, uint16_t p2);

        cbfp     _cb_heartbeat;
        cbfpb2   _cb_version_info;
        cbfpb1   _cb_model_info;
        cbfpb1   _cb_port_info;
        cbfp     _cb_all_off;
        cbfp     _cb_all_on;
        cbfpb2   _cb_all_timed;
        cbfpb1   _cb_port_off;
        cbfpb1   _cb_port_on;
        cbfpb1b2 _cb_port_timed;
        cbfpb2   _cb_sequence_up;
        cbfpb2   _cb_sequence_down;

        uint8_t _me;
        uint8_t _model;

        uint8_t _buffer[CP_MAX_PACKET];

        int _bindex;
        int _received_header;
        int _expected_length;
        unsigned long _last_heartbeat;
};

#endif
