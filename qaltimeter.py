#!/usr/bin/env python3

import serial
import sys


ser = serial.Serial(port='/dev/ttyS0',
                   baudrate=115200)

message = sys.argv[1] + "\r\n"
ser.write(str.encode(message))

response = ser.readline()
print("> " + response.decode("utf-8"))

ser.close();
