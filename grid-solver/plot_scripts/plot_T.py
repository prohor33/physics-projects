#!/usr/bin/env python

from pylab import *
import mpl_toolkits.mplot3d.axes3d as p3
import struct

from os import listdir
from os.path import isfile, join

mypath = "../output/"
only_n_T_files = [ f for f in listdir(mypath) \
  if isfile(join(mypath,f)) and f.find("_T_")!=-1 ]

#for now it's build only one plot
result_filename = only_n_T_files[0]

fin = open(mypath + result_filename, "rb")

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

#print 'grid size: ', size_x, size_y

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

f = open('n.txt', 'w')
for j in range(0, size_y):
  for i in range(0, size_x):
    f.write(str(X[i][j])+' '+str(Y[i][j])+' '+str(Z[i][j])+'\n')

execfile('../plot_scripts/plot_T_new.py')

exit()

#for i in range(0, size_x):
#  for j in range(0, size_y):
#    if Z[i][j] == 0:
#      Z[i][j] = nan

#Z = np.ma.masked_invalid(Z)
Z = ma.masked_where(np.isnan(Z), Z)

#plt.figure(figsize=(34.7/2.54, (size_x+50)/size_y*27.75/2.54)) # in santimeters                    

#im = imshow(Z)

fig, ax = plt.subplots()

p = ax.pcolormesh(X, Y, Z, cmap=cm.jet, vmin=Z.min(),
                vmax=Z.max(), edgecolors = 'None')
cb = fig.colorbar(p, ax=ax)

x = X
y = Y
a = Z

import matplotlib.pyplot as plt
import scipy.interpolate

def main():

  xi, yi = np.mgrid[0:35, 0:25]
  
  a_orig = normal_interp(x, y, a, xi, yi)
  a_rescale = rescaled_interp(x, y, a, xi, yi)
  
  plot(x, y, a, a_orig, 'Not Rescaled')
  plot(x, y, a, a_rescale, 'Rescaled')
  plt.show()

def normal_interp(x, y, a, xi, yi):
  rbf = scipy.interpolate.Rbf(x, y, a)
  ai = rbf(xi, yi)
  return ai

def rescaled_interp(x, y, a, xi, yi):
  a_rescaled = (a - a.min()) / a.ptp()
  ai = normal_interp(x, y, a_rescaled, xi, yi)
  ai = a.ptp() * ai + a.min()
  return ai

def plot(x, y, a, ai, title):
  fig, ax = plt.subplots()
  
  im = ax.imshow(ai.T, origin='lower',
                 extent=[x.min(), x.max(), y.min(), y.max()])
  ax.scatter(x, y, c=a)
  ax.set(xlabel='X', ylabel='Y', title=title)
  fig.colorbar(im, ax=ax)

main()

#im = plt.imshow(Z, cmap=cm.jet, vmin=Z.min(), vmax=Z.max(),
#           extent=[X.min(), X.max(), Y.min(), Y.max()],
#           interpolation='bilinear', origin='lower')

#im.set_interpolation('bilinear')
plt.title('image (interp. nearest)')

# should do interpolation?
#imshow(fig, interpolation='bilinear') #doesnt work
#im.set_interpolation('none') # either
#im.set_clim(0.8, 1.0)

#colorbar(orientation='horizontal')
#ax = plt.axis([0, size_x-1, 0, size_y-1])

plt.title("Temperature")

img_name = result_filename.replace(".dat", ".png")

plt.savefig("../img/"+img_name)

#plt.show()

plt.close()

