zeromq-arduino-example
======================

Sending [ZeroMQ (ØMQ)](http://zeromq.org) messages from Arduino (limited zmq wire protocol implementation)

You need an Ethernet Shield and (optionally) some sensors to be read on analog pins 0 and 1

This example sends one ZeroMQ message every second to a ZeroMQ PULL socket on 192.168.11.3 port 7000

This means this code implements the simplest PUSH message.

It does not implement a full ZeroMQ stack. You should use it as a mean of delivering messages to a more stable
ØMQ router running on a server.


Example server
===============

Example of a listening PULL socket (in Python):

    import sys
    import zmq
    import time


    context = zmq.Context()
    sock = context.socket(zmq.PULL)
    sock.bind("tcp://*8080")

    while True:
        message= sock.recv()
        print message
