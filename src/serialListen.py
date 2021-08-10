#!/usr/bin/python3



##############
## Script to listen serial port and write contents into a file
## only for debug purpose 
##############
## requires pySerial to be installed 
## pip3 install pyserial

import serial

from datetime import datetime


now = datetime.now()
dt_string = now.strftime("%H:%M")


serial_port = '/dev/ttyACM0';
baud_rate = 9600; #In arduino, Serial.begin(baud_rate)
write_to_file_path = "/tmp/output" + dt_string +".txt";

output_file = open(write_to_file_path, "w+");
ser = serial.Serial(serial_port, baud_rate)
while True:
    line = ser.readline();
    line = line.decode("utf-8") #ser.readline returns a binary, convert to string
    print(line);
    output_file.write(line);

