#!/usr/bin/env python

from pylab import *
import mpl_toolkits.mplot3d.axes3d as p3
import struct

fin = open("result_T_gas_0.dat", "rb")

size_x = 35;
size_y = 25;

dt = np.dtype([('x','i'),('y','i'),('z','i'),('value','d')])

data = np.fromfile(fin, dtype=dt)
#print data

X = np.zeros(shape = (size_x, size_y))
Y = np.zeros(shape = (size_x, size_y))
Z = np.zeros(shape = (size_x, size_y))

it = 0
for i in range(0, size_x):
  for j in range(0, size_y):
    X[i][j] = data[it][0]
    Y[i][j] = data[it][1]
    Z[i][j] = data[it][3]
    it+=1
#print X[i][j], Y[i][j], Z[i][j]


for i in range(0, size_x):
  for j in range(0, size_y):
    if Z[i][j] == 0:
      Z[i][j] = nan

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

plt.savefig("mpi_plot.png")

plt.show()

plt.close()

