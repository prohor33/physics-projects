#! /usr/bin/env python

from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt
from matplotlib import cm

from pylab import *

#r_file = file("test_T_gas0.result")
#print r_file.readline().rsplit()[3]

size_x = 6-2
size_y = 8-2

data = np.loadtxt("test_n_gas1.result")
X = data[:,0].reshape(size_x, size_y)
Y = data[:,1].reshape(size_x, size_y)
Z = data[:,3].reshape(size_x, size_y)

fig = plt.figure()
ax = fig.gca(projection='3d')
#X, Y, Z = axes3d.get_test_data(0.05)
ax.plot_surface(X, Y, Z, rstride=1, cstride=1, alpha=0.3)
cset = ax.contourf(X, Y, Z, zdir='z', offset=0.5, cmap=cm.coolwarm)
cset = ax.contourf(X, Y, Z, zdir='x', offset=size_x, cmap=cm.coolwarm)
cset = ax.contourf(X, Y, Z, zdir='y', offset=size_y, cmap=cm.coolwarm)

ax.set_xlabel('X')
ax.set_xlim(1, size_x)
ax.set_ylabel('Y')
ax.set_ylim(1, size_y)
ax.set_zlabel('Z')
ax.set_zlim(0.5, 1.5)

plt.show()
