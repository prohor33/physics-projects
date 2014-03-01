#!/usr/bin/env python

from pylab import *
import mpl_toolkits.mplot3d.axes3d as p3
import struct

from os import listdir
from os.path import isfile, join

mypath = "../output/"
only_Flow_file = [ f for f in listdir(mypath) \
  if isfile(join(mypath,f)) and f.find("_Flow_")!=-1 ]

#for now it's build only one plot
result_filename = only_Flow_file[0]

fin = open(mypath + result_filename, "rb")

dt = np.dtype([('x','i'),('y','i'),('z','i'),('u_x','d'),('u_y','d'),('u_z','d')])

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
u_x = np.zeros(shape = (size_x, size_y))
u_y = np.zeros(shape = (size_x, size_y))
u_z = np.zeros(shape = (size_x, size_y))

it = 0
for i in range(0, size_x):
  for j in range(0, size_y):
    X[i][j] = data[it][0]
    Y[i][j] = data[it][1]
    u_x[i][j] = data[it][3]
    u_y[i][j] = data[it][4]
    u_z[i][j] = data[it][5]
    it+=1

for i in range(0, size_x):
  for j in range(0, size_y):
    if u_x[i][j] == 100 and u_y[i][j] == 100 and u_z[i][j] == 100:
      u_x[i][j] = np.nan
      u_y[i][j] = np.nan
      u_z[i][j] = np.nan

#plt.figure(figsize=(34.7/2.54, (size_x+50)/size_y*27.75/2.54)) # in santimeters                    

#im = imshow(Z)


#Q = quiver( X, Y, u_x, u_y, units='x',
#           pivot='tip', width=1.0/8.0, scale=1.0/5.0 )
Q = quiver( X, Y, u_x, u_y, units='x',
           pivot='tip', width=1.0/12.0, scale=1.0/3.0 )

qk = quiverkey(Q, 0.95, 1.05, 0.1, '0.1', labelpos='W')

#colorbar(orientation='horizontal')
#ax = plt.axis([0, size_x-1, 0, size_y-1])

plt.title("Flow")

img_name = result_filename.replace(".dat", ".png")

plt.savefig("../img/"+img_name)

#plt.show()

plt.close()

