#! /usr/bin/env python

# -*- coding: utf-8 -*-

# Copyright (c) 2012 Peter Kropf. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.


import serial


CPC_HEARTBEAT      = 0x01
CPC_VERSION_QUERY  = 0x10
CPC_VERSION_INFO   = 0x11
CPC_MODEL_QUERY    = 0x12
CPC_MODEL_INFO     = 0x13
CPC_PORT_QUERY     = 0x14
CPC_PORT_INFO      = 0x15
CPC_ALL_OFF        = 0x20
CPC_ALL_ON         = 0x21
CPC_PORT_ON        = 0x22
CPC_PORT_OFF       = 0x23
CPC_SEQUENCE_UP    = 0x24
CPC_SEQUENCE_DOWN  = 0x25
CPC_TIMED_ON       = 0x26
CPC_FOOTER         = 0xfd
CPC_HEADER         = 0xfe


CP_EVERYONE        = 0x20


class ControlPack(object):
    def __init__(self, port, baud):
        self.port = port
        self.baude = baud
        self.ser = serial.Serial(port, baud, timeout=1)
        self.found_header = 0
        self.length = 0
        self.me = 0


    def send_heartbeat(self):
        self.send(CPC_HEARTBEAT, CP_EVERYONE);


    def send(self, cmd, dst):
        buffer = chr(CPC_HEADER) + chr(3) + chr(self.me) + chr(dst) + chr(cmd) + chr(CPC_FOOTER)

        self.ser.write(buffer)
        self.ser.flush()


    def loop(self):
        if self.ser.inWaiting() > 0:
            b = ord(self.ser.read(1))
            if b == CPC_HEADER:
                self.found_header = 1

            elif b == CPC_HEARTBEAT:
                if self.found_header and self.length:
                    self.heartbeat()
                else:
                    raise KeyError, b

            else:
                if self.found_header and self.length == 0:
                    self.length = b
