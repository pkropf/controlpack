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


#ifndef __ControlPack_h
#define __ControlPack_h

#include "Arduino.h"


typedef void (*cbfp)(uint8_t, uint8_t, uint8_t, uint8_t);


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
#define CPC_PORT_ON        0x22
#define CPC_PORT_OFF       0x23
#define CPC_SEQUENCE_UP    0x24
#define CPC_SEQUENCE_DOWN  0x25
#define CPC_TIMED_ON       0x26
#define CPC_FOOTER         0xfd
#define CPC_HEADER         0xfe


// control pack constants
#define CP_EVERYONE        0x20
#define CP_HEARTBEAT_RATE  500
#define CP_MAX_PACKET      32


class ControlPack
{
    public:
        ControlPack(uint8_t me);

        void loop();
        void send(uint8_t cmd, uint8_t dst);

        void scb_test(cbfp fp);
        void scb_heartbeat(cbfp fp);
        void scb_version_query(cbfp fp);
        void scb_version_info(cbfp fp);
        void scb_model_query(cbfp fp);
        void scb_model_info(cbfp fp);
        void scb_port_query(cbfp fp);
        void scb_port_info(cbfp fp);
        void scb_all_off(cbfp fp);
        void scb_all_on(cbfp fp);
        void scb_port_on(cbfp fp);
        void scb_port_off(cbfp fp);
        void scb_sequence_up(cbfp fp);
        void scb_sequence_down(cbfp fp);
        void scb_timed_on(cbfp fp);


    private:
    	void parse();
    	void build_packet();
    	void send_heartbeat();

        cbfp _cb_test;
        cbfp _cb_heartbeat;
        cbfp _cb_version_query;
        cbfp _cb_version_info;
        cbfp _cb_model_query;
        cbfp _cb_model_info;
        cbfp _cb_port_query;
        cbfp _cb_port_info;
        cbfp _cb_all_off;
        cbfp _cb_all_on;
        cbfp _cb_port_on;
        cbfp _cb_port_off;
        cbfp _cb_sequence_up;
        cbfp _cb_sequence_down;
        cbfp _cb_timed_on;

        uint8_t _me;

        uint8_t _buffer[CP_MAX_PACKET];

        int _bindex;
        int _received_header;
        int _expected_length;
        unsigned long _last_heartbeat;
};

#endif
