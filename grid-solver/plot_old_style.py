#!/usr/bin/env python

#from __future__ import division
#from matplotlib.patches import Patch
from pylab import *
#import osi
import mpl_toolkits.mplot3d.axes3d as p3

gas_index = 0;

data = np.loadtxt("test_T_gas" + str(gas_index) + ".result")

#size_x = 20;
size_x = 13;
size_y = 25;
#size_x = 35
#size_y = 25
#size_x = 6
#size_y = 8

X = data[:,0].reshape(size_x, size_y)
Y = data[:,1].reshape(size_x, size_y)
Z = data[:,3].reshape(size_x, size_y)

for i in range(0, size_x):
  for j in range(0, size_y):
    if Z[i,j] == 0:
      Z[i,j] = nan

Z = np.ma.masked_invalid(Z)

#plt.figure(figsize=(34.7/2.54, (size_x+50)/size_y*27.75/2.54)) # in santimeters                    

#im = imshow(Z)

fig, ax = plt.subplots()

p = ax.pcolormesh(X, Y, Z, cmap=cm.jet, vmin=Z.min(),
                vmax=Z.max(), edgecolors = 'None')
cb = fig.colorbar(p, ax=ax)

#im.set_interpolation('none')
#im.set_clim(0.8, 1.0)

#colorbar(orientation='horizontal')
#ax = plt.axis([0, size_x-1, 0, size_y-1])

plt.title("Temperature")

#plt.savefig("plot.png")

plt.show()

plt.close()

