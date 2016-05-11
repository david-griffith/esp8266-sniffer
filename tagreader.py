#!/usr/bin/python

import serial
import time

port = "/dev/ttyUSB0"
baud = 115200

mydict = {}
ser = serial.Serial(port, baud, timeout =1)

print("Dave's Tag Locatorer")
print("Enter the last three bytes of the MAC address in hex")
print("(eg 76:3a:59) or leave blank to find all tags")
mytag = raw_input("Enter tag info :   ")
mytag = mytag.lower()

while True:
    mydata = ser.readline().split(",")
    if mydata != [' ']:
        if mydata[0].find('00:0c:cc'):
            mydict[mydata[0]] = (int(mydata[1]),time.time())
    print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n")
    print(" -----------------------------")
    print("       MAC           RSSI  TTL")
    #print tags sorted by mac, delete tags older than 10 seconds
    for key in sorted(mydict):
	if (mytag in key): print key," ", mydict[key][0]," ", 10-int(time.time()-mydict[key][1])
        if time.time() - mydict[key][1] > 10:
            del mydict[key]

    print(" -----------------------------")

