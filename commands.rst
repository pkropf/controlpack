control pack commands
=====================

control pack commands packets are sent between the coordinator and control packs to execute specific commands. it is assumed that the communications will take place as broadcast messages, all control packs receive all command packets. because of this, the packets must include the originator and destination. a destination of 0xff indicated that all control packs are to execute the command.

it can be assumed that the coordinator's node number is 0.

all control packs will issue heartbeat commands every 500ms. the coordinator will use these to ensure that command packs are online and active. control packs will use the coordinator heartbeat to verify that the coordinator is still online and active. in the event that a control pack doesn't receive a coordinator heartbeat for 2000ms, it will assume that the coordinator is offline or suffering some malfunction. in that event, the control pack will set all digital pins off.


packet format
+++++++++++++

hhllssttccppff
 | | | | | | |
 | | | | | | +-- footer, always 0xfd
 | | | | | |
 | | | | | +-- optional command parameter(s), there can be 0 or more parameter bytes
 | | | | |
 | | | | +-- command
 | | | |
 | | | +-- target node number, 0xff = all
 | | |
 | | +-- source node number
 | |
 | +-- content length, 0x01 to 0xfc
 |
 +-- header, always 0xfe


commands
++++++++

0x01 heartbeat
0x10 version query
0x11 version info
0x12 model query
0x13 model info
0x14 port count query
0x15 port count info
0x20 all off
0x21 all on
0x22 turn on specified i/o port, 0 - n
0x23 turn off specified i/o port, 0 - n
0x24 sequence on/off ports 0 thru n
0x25 sequence on/off ports n thru 0
0x26 turn on specified i/o port, 0 - n, for specified milliseconds, m
0xfd packet footer
0xfe packet header


command packet examples
+++++++++++++++++++++++

version
-------

version query

    0xfe 0x03 0x00 0xff 0x10 0xfd


version info packet

    0xfe 0x05 0xID 0x00 0x11 0xMJ 0xMI 0xfd

    0xID is the control pack's id number

    0xMJ is the control pack's major version number

    0xMI is the control pack's minor version number


model
-----

model query

    0xfe 0x03 0x00 0xff 0x12 0xfd


model info

    0xfe 0x04 0xID 0x00 0x13 0xMD 0xfd

    0xID is the control pack's id number

    0xMD is the control pack's model number, 1 = quad pack, 2 = octa pack


port
----

port query

    0xfe 0x03 0x00 0xff 0x14 0xfd


port count info

    0xfe 0x04 0xID 0x00 0x15 0xPT 0xfd

    0xID is the control pack's id number

    0xPT is the count of the control pack's active digital i/o ports


all off/on
----------

all off

    0xfe 0x03 0x00 0xff 0x20 0xfd


all on

    0xfe 0x03 0x00 0xff 0x21 0xfd


port on/off
-----------

port 1 on

    0xfe 0x04 0x00 0xff 0x22 0x01 0xfd


port 3 off

    0xfe 0x04 0x00 0xff 0x23 0x03 0xfd


sequent up/down
---------------

sequence up

    0xfe 0x03 0x00 0xff 0x24 0xfd


sequence down

    0xfe 0x03 0x00 0xff 0x25 0xfd


port on for time
----------------

on control pack 1, turn on port 2 for 1,000 milliseconds

    0xfe 0x03 0x00 0x01 0x26 0x02 0x03 0xe8 0xfd

    milliseconds = (p1 << 8) + p2
    1000 = (0x03 << 8) + 0xe8
