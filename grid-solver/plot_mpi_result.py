#!/usr/bin/env python

from pylab import *
import mpl_toolkits.mplot3d.axes3d as p3
import struct

fin = open("result_T_gas_0.dat", "rb")

dt = np.dtype([('x','i'),('y','i'),('z','i'),('value','d')])

data = np.fromfile(fin, dtype=dt)
#print data

size_x = 0;
size_y = 0;

for i in range(0, data.size):
  if data[i][0] > size_x:
    size_x = data[i][0]
  if data[i][1] > size_y:
    size_y = data[i][1]

size_x += 1
size_y += 1

print 'grid size: ', size_x, size_y

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

# should do interpolation?
#imshow(fig, interpolation='bilinear') #doesnt work
#im.set_interpolation('none') # either
#im.set_clim(0.8, 1.0)

#colorbar(orientation='horizontal')
#ax = plt.axis([0, size_x-1, 0, size_y-1])

plt.title("Temperature")

plt.savefig("mpi_plot.png")

plt.show()

plt.close()

