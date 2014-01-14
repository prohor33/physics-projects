#!/usr/bin/env python

import struct

fin = open("final.dat", "rb")

i = fin.read(4)
count = 0

while i:
  print struct.unpack('i', i)[0],
  print struct.unpack('i', fin.read(4))[0],
  print struct.unpack('i', fin.read(4))[0],
  print struct.unpack('d', fin.read(8))[0]
  i = fin.read(4)
  count+=1
print "parameters quantity: " + str(count)
