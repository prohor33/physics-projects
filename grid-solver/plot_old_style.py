#!/usr/bin/env python

#from __future__ import division
#from matplotlib.patches import Patch
from pylab import *
#import os

gas_index = 0;

data = np.loadtxt("test_T_gas" + str(gas_index) + ".result")

size_x = 35
size_y = 25

#X = data[:,0].reshape(size_x, size_y)
#Y = data[:,1].reshape(size_x, size_y)
Z = data[:,3].reshape(size_x, size_y)

for i in range(0, size_x):
  for j in range(0, size_y):
    if Z[i,j] == 0:
      Z[i,j] = np.nan

#plt.figure(figsize=(34.7/2.54, (size_x+50)/size_y*27.75/2.54)) # in santimeters                    

im = imshow(Z)
#im.set_interpolation('none')
im.set_clim(0.8, 1.0)

colorbar(orientation='horizontal')
ax = plt.axis([0, size_x-1, 0, size_y-1])

plt.title("Temperature")
plt.show()

#plt.savefig("plot.png")

plt.close()

