#!/usr/bin/env python

"""
  Illustrate simple contour plotting, contours on an image with
  a colorbar for the contours, and labelled contours.
  
  See also contour_image.py.
  """
from __future__ import division
from matplotlib.patches import Patch
from pylab import *
import os
import pdb

start_quantity_of_files = 37;
end_quantity_of_files = 38
#end_quantity_of_files = input("Enter quantity of files: ")

gas_index = 1;

#directory = "result " + str(gas_index) + "/concentration"
#if not os.path.exists(directory):
#  os.makedirs(directory)
#directory = "result " + str(gas_index) + "/enlarged velocity right"
#if not os.path.exists(directory):
#  os.makedirs(directory)
#directory = "result " + str(gas_index) + "/enlarged velocity left"
#if not os.path.exists(directory):
#  os.makedirs(directory)
#directory = "result " + str(gas_index) + "/temperature"
#if not os.path.exists(directory):
#  os.makedirs(directory)
#directory = "result " + str(gas_index) + "/velocity"
#if not os.path.exists(directory):
#  os.makedirs(directory)

GridSize_x = 35
GridSize_y = 25
print "Grid:", GridSize_x, GridSize_y

FLAG_use_contour = 1
FLAG_use_interpolation = 1
quantity_of_files = start_quantity_of_files
start_file_name = "concentration"
start_info_file_name = "info"
#global_data = np.loadtxt("../output/gas %i/global_info.txt"%(gas_index))
#min_concentration = global_data[0]
#max_concentration = global_data[1]
#min_temperature = global_data[2]
#max_temperature = global_data[3]
#min_mod_gas_velocity = global_data[4]
#max_mod_gas_velocity = global_data[5]
min_concentration = 0
max_concentration = 1
min_temperature = 0
max_temperature = 1
min_mod_gas_velocity = 0
max_mod_gas_velocity = 1
max_quantity_of_contours_conc = 3
max_quantity_of_contours_temp = 3

FLAG_only_temperature = 0
FLAG_use_titles = 0


#sp hack??
sp = GridSize_x
GridSize_x = GridSize_y
GridSize_y = sp

#============================ plots decoration (stupid) ==========================
def plots_decoration():
  x_max = 300 #200
  x_start = 91 #50
  x_end = 168 #129
  y_start = 70 #25
  y_end = 79 #29
  x_h = 4; #2
  x_d = 4; #2
  y_sl = 0.2 #0.2
  # draw left line part
  plt.plot([0,x_start+0.5,x_start+0.5,0],[y_start-0.5+y_sl,y_start-0.5+y_sl,y_end+0.5+y_sl,y_end+0.5+y_sl],'k')
  gaps_quant = 9;
  # draw line boxes
  for i in range(0, gaps_quant):
    plt.plot([x_start+x_d+0.5+i*(x_d+x_h),x_start+x_d+0.5+i*(x_d+x_h),x_start+x_d+x_h+0.5+i*(x_d+x_h),\
              x_start+x_d+x_h+0.5+i*(x_d+x_h),x_start+x_d+0.5+i*(x_d+x_h)],\
             [y_end+0.5+y_sl,y_start-0.5+y_sl,y_start-0.5+y_sl,y_end+0.5+y_sl,y_end+0.5+y_sl],'k')
  # draw right line part
  plt.plot([x_max,x_end-0.5,x_end-0.5,x_max],[y_start-0.5+y_sl,y_start-0.5+y_sl,y_end+0.5+y_sl,y_end+0.5+y_sl],'k')
  
  def draw_dash( x, y, delta, d_size, size, is_horisontal ):
    for i in range(0, int(size/delta)):
      if is_horisontal:
        plt.plot([x+i*delta,x+i*delta+d_size],[y,y+d_size],'k')
      else:
        plt.plot([x,x+d_size],[y+i*delta,y+i*delta+d_size],'k')
  def draw_dash_box( x, y, size_x, size_y, delta ):
    sp_sl_y = 0.0
    for i in range(0, 200):
      if i*delta > (size_x+size_y):
        break;
      if i*delta < size_x:
        plt.plot([x,x+i*delta],[y+size_y-delta*i+sp_sl_y,y+size_y+sp_sl_y],'k')
      else:
        if i*delta > size_y:
          plt.plot([x+i*delta-size_y,x+size_x],[y+sp_sl_y,y+size_y-delta*i+size_x+sp_sl_y],'k')
        else:
          plt.plot([x,x+size_x],[y+size_y-delta*i+sp_sl_y,y+size_y-delta*i+size_x+sp_sl_y],'k')
  d_size = 2.0; #1.0
  x=0
  y=y_start-0.5
  box_size_y = y_end-y_start+1;
  delta=2.1 #1.1
  # draw left dash part
  draw_dash( -0.1, y+y_sl, delta, d_size, x_start+0.5, 1 )
  draw_dash( x_start+0.5-d_size, y+y_sl, delta, d_size, box_size_y, 0 )
  draw_dash( 0, y_end+0.5-d_size+y_sl, delta, d_size*0.9, x_start+1.5, 1 )
  # draw dash boxes
  for i in range(0, gaps_quant):
    draw_dash_box( i*(x_d+x_h)+ x_start+0.5+x_d, y_start-0.5+y_sl, x_h, box_size_y-0.1, delta )
  # draw right dash part
  draw_dash( x_end-0.5, y+y_sl, delta, d_size, x_max-x_end+0.5, 1 )
  draw_dash( x_end-0.5, y+y_sl, delta, d_size, box_size_y, 0 )
  draw_dash( x_end-0.5, y_end+0.5-d_size+y_sl, delta, d_size*0.9, x_max-x_end+0.5, 1 )
#==================================================================================

# Concentration
input_file_name = "n.txt"
data = np.loadtxt(input_file_name)

#data begining with concentration
quantity_of_contours = max_quantity_of_contours_conc

#print(quantity_of_contours)
NX=GridSize_x;
NY=GridSize_y;
X = data[:,0].reshape(NX,NY)
Y = data[:,1].reshape(NX,NY)
Z = data[:,2].reshape(NX,NY)

for i in range(0, NX):
  for j in range(0, NY):
    if Z[i,j] == 0:
      Z[i,j] = np.nan

Z = ma.masked_where(np.isnan(Z), Z)

min_concentration = Z.min()
max_concentration = Z.max()


#plt.figure()
#plt.figure(figsize=(34.7/2.54, 27.75/2.54)) # in santimeters
#plt.figure(figsize=(34.7/2.54, (NX+50)/NY*27.75/2.54)) # in santimeters
plt.figure(figsize=(34.7/2.54, (NX)/NY*27.75/2.54)) # in santimeters


im = imshow(Z)
#im.set_interpolation('bilinear')
if( not FLAG_use_interpolation ):
  im.set_interpolation('none')
im.set_clim(min_concentration, max_concentration)
#pcolor(X, Y, Z, norm=plt.Normalize(0.200, 0.25))
#pcolor(X, Y, Z)
#colorbar()
colorbar(orientation='vertical')
if( ( quantity_of_contours > 0 ) & FLAG_use_contour ):
  CS = plt.contour(X, Y, Z, quantity_of_contours,
                   colors='k', # negative contours will be dashed by default
                   )
  plt.clabel(CS, inline=1, fontsize=10)
ax = plt.axis([0, NY-1, 0, NX-1]) #hack again??

#plots_decoration()

"""
  plt.figure()
  CS = plt.contour(X, Y, Z)
  plt.clabel(CS, inline=1, fontsize=10)
  """
#f_info = file("../output/gas %i/info%i.txt"%(gas_index,quantity_of_files),"r")
#f_info.readline()
#f_info.readline()
#f_info.readline()
#quantity_of_iterations = f_info.readline().rsplit()[4]
#f_info.readline()
#f_info.readline()
#f_info.readline()
#time_step = f_info.readline().rsplit()[2]
quantity_of_iterations = 0
time_step = 0.02

str_title = "Gas %i  Concentration   t = %.2lf" % (gas_index,int(quantity_of_iterations)*float(time_step));
if (FLAG_use_titles):
  plt.title(str_title)
#plt.show()
output_file_name = "%s%s%i.png" % ("result " + str(gas_index) + "/Concentration/", \
                                   start_file_name, quantity_of_files)
#plt.savefig(output_file_name)
plt.savefig('../img/T.png')
print 100/4*float(quantity_of_files)/end_quantity_of_files, "%"
quantity_of_files = quantity_of_files + 1
                                   
plt.close()
exit()

# Temperature
quantity_of_files = start_quantity_of_files
start_file_name = "temperature"
while(quantity_of_files < end_quantity_of_files):
  #file_name = "%s %d\t%3.2f %-8s" % ("Values:",42,56.89732,"tot")
  input_file_name = "%s%i%s%s%i.txt" % ("../output/gas ", gas_index, "/", start_file_name, quantity_of_files)
  data = np.loadtxt(input_file_name)
  
  #data begining with concentration
  quantity_of_contours = max_quantity_of_contours_temp
  
  NX=GridSize_x;
  NY=GridSize_y;
  X = data[:,0].reshape(NX,NY)
  Y = data[:,1].reshape(NX,NY)
  Z = data[:,2].reshape(NX,NY)
  
  for i in range(0, NX):
    for j in range(0, NY):
      if Z[i,j] == 0:
        Z[i,j] = np.nan
  
  #plt.figure()
  #plt.figure(figsize=(1366/1000*10.0, 1366/1000*4.5))
  #plt.figure(figsize=(34.7/2.54, 27.75/2.54)) # in santimeters
  #plt.figure(figsize=(34.7/2.54, (NX+50)/NY*27.75/2.54)) # in santimeters
  plt.figure(figsize=(34.7/2.54, (NX+100)/NY*27.75/2.54)) # in santimeters
  
  im = imshow(Z)
  if( not FLAG_use_interpolation ):
    im.set_interpolation('none')
  im.set_clim(min_temperature, max_temperature)
  #pcolor(X, Y, Z, norm=plt.Normalize(0.690, 0.87))
  #pcolor(X, Y, Z)
  colorbar(orientation='horizontal')
  if( ( quantity_of_contours > 0 ) & FLAG_use_contour ):
    CS = plt.contour(X, Y, Z, quantity_of_contours,
                     colors='k', # negative contours will be dashed by default
                     )
    plt.clabel(CS, inline=1, fontsize=10)
  ax = plt.axis([0, NY-1, 0, NX-1]) #hack again??
  
  plots_decoration()
  
  """
    plt.figure()
    CS = plt.contour(X, Y, Z)
    plt.clabel(CS, inline=1, fontsize=10)
    """
  f_info = file("../output/gas %i/info%i.txt"%(gas_index,quantity_of_files),"r")
  f_info.readline()
  f_info.readline()
  f_info.readline()
  quantity_of_iterations = f_info.readline().rsplit()[4]
  f_info.readline()
  f_info.readline()
  f_info.readline()
  time_step = f_info.readline().rsplit()[2]
  
  
  str_title = "Gas %i  Temperature   t = %.2lf" % (gas_index, int(quantity_of_iterations)*float(time_step));
  if (FLAG_use_titles):
    plt.title(str_title)
  #plt.title('Temperature')
  
  output_file_name = "%s%s%i.png" % ("result " + str(gas_index) + "/temperature/",start_file_name, quantity_of_files)
  plt.savefig(output_file_name)
  #plt.show()
  print 25+100/4*float(quantity_of_files)/end_quantity_of_files, "%"
  quantity_of_files = quantity_of_files + 1
                                     
  plt.close()

# Gas velocity
quantity_of_files = start_quantity_of_files
start_file_name = "gas_velocity"
while(quantity_of_files < end_quantity_of_files):
  input_file_name = "%s%i%s%s%i.txt" % ("../output/gas ", gas_index, "/", start_file_name, quantity_of_files)
  data = np.loadtxt(input_file_name)
  
  #data begining with concentration
  quantity_of_contours = max_quantity_of_contours_temp
  
  NX=GridSize_x;
  NY=GridSize_y;
  X = data[:,0].reshape(NX,NY)
  Y = data[:,1].reshape(NX,NY)
  U = data[:,2].reshape(NX,NY)
  V = data[:,3].reshape(NX,NY)
  
  for i in range(0, NX):
    for j in range(0, NY):
      if U[i,j] == 100:
        if V[i,j] == 100:
          U[i,j] = np.nan
          V[i,j] = np.nan
  
  #=============================================================
  AvU = list(range(NX))
  AvV = list(range(NX))
  for i in range(NX):
    AvU[i] = list(range(NY))
    AvV[i] = list(range(NY))
  
  for i in range(0, NX):
    for j in range(0, NY):
      if not(70 <= i <= 79): #hack
        AvU[i][j] = np.nan
        AvV[i][j] = np.nan
      else:
        AvU[i][j] = U[i,j]
        AvV[i][j] = V[i,j]
  
  dx = 4;
  dy = 20;
  for i in range(0, int(NX/dx)):
    for j in range(0, int(NY/dy)):
      quant_av = 0
      if not(70 <= (i*dx+int(dx/2)) <= 79): #hack
        AvU[i*dx+int(dx/2)][j*dy+int(dy/2)] = 0
        AvV[i*dx+int(dx/2)][j*dy+int(dy/2)] = 0
        for k in range(0, dx):
          for l in range(0, dy):
            if not(isnan(U[i*dx+k,j*dy+l])):
              AvU[i*dx+int(dx/2)][j*dy+int(dy/2)] += U[i*dx+k,j*dy+l]
              AvV[i*dx++int(dx/2)][j*dy+int(dy/2)] += V[i*dx+k,j*dy+l]
              quant_av += 1
        AvU[i*dx+int(dx/2)][j*dy+int(dy/2)] /= quant_av
        AvV[i*dx+int(dx/2)][j*dy+int(dy/2)] /= quant_av
  #pdb.set_trace()
  #=============================================================================
  
  #plt.figure(figsize=(34.7/2.54, 27.75/2.54)) # in santimeters
  plt.figure(figsize=(34.7/2.54, NX/NY*27.75/2.54)) # in santimeters
  
  M = sqrt(pow(U, 2) + pow(V, 2))
  
  
  Q = quiver( X, Y, AvU, AvV, clim=(min_mod_gas_velocity, max_mod_gas_velocity), units='x',
             pivot='tip', width=1.0/2.0, scale=1.0/50.0 )
             
  ##    Q = quiver( X[::3, ::3], Y[::3, ::3], U[::3, ::3], V[::3, ::3],
  ##             color='r', units='x',
  ##                linewidths=(2,), edgecolors=('k'), headaxislength=5, scale=1/35.0  )

  qk = quiverkey(Q, 0.95, 1.05, 0.1, '0.1', labelpos='W')

  ax = plt.axis([0, NY-1, 0, NX-1]) #hack again??

  plots_decoration()

  f_info = file("../output/gas %i/info%i.txt"%(gas_index,quantity_of_files),"r")
  f_info.readline()
  f_info.readline()
  f_info.readline()
  quantity_of_iterations = f_info.readline().rsplit()[4]
  f_info.readline()
  f_info.readline()
  f_info.readline()
  time_step = f_info.readline().rsplit()[2]

  str_title = "Gas %i  Gas Velocity   t = %.2lf" % (gas_index,int(quantity_of_iterations)*float(time_step));
  if (FLAG_use_titles):
    plt.title(str_title)

  output_file_name = "%s%s%i.png" % ("result " + str(gas_index) + "/velocity/",\
                                     start_file_name, quantity_of_files)
  plt.savefig(output_file_name)
  #plt.show()
  print 50+100/4*float(quantity_of_files)/end_quantity_of_files, "%"
  quantity_of_files = quantity_of_files + 1

  plt.close()

# Enlarged gas velocity RIGHT
quantity_of_files = start_quantity_of_files
start_file_name = "gas_velocity"
while(quantity_of_files < end_quantity_of_files):
  input_file_name = "%s%i%s%s%i.txt" % ("../output/gas ", gas_index, "/", start_file_name, quantity_of_files)
  data = np.loadtxt(input_file_name)
  
  #data begining with concentration
  quantity_of_contours = max_quantity_of_contours_temp
  
  NX=GridSize_x;
  NY=GridSize_y;
  X = data[:,0].reshape(NX,NY)
  Y = data[:,1].reshape(NX,NY)
  U = data[:,2].reshape(NX,NY)
  V = data[:,3].reshape(NX,NY)
  
  for i in range(0, NX):
    for j in range(0, NY):
      if U[i,j] == 100:
        if V[i,j] == 100:
          U[i,j] = np.nan
          V[i,j] = np.nan
  
  #plt.figure(figsize=(34.7/2.54, 27.75/2.54)) # in santimeters
  #plt.figure(figsize=(34.7/2.54, NX/NY*27.75/2.54)) # in santimeters
  plt.figure(figsize=(34.7/2.54, (85-65)/(175-85)*34.7/2.54)) # in santimeters
  
  M = sqrt(pow(U, 2) + pow(V, 2))
  
  ##    Q = quiver( X[::3, ::3], Y[::3, ::3], U[::3, ::3], V[::3, ::3],
  ##            pivot='mid', color='r', units='inches' )
  
  ##    Q = quiver( X, Y, U, V,
  ##                M, clim=(min_mod_gas_velocity, max_mod_gas_velocity), units='x',
  ##            pivot='mid', width=0.5/2.0, scale=1/5.0 )
  
  Q = quiver( X, Y, U, V, clim=(min_mod_gas_velocity, max_mod_gas_velocity), units='x', pivot='mid', width=0.25/2.0, scale=1.0/35.0 )
             
  ##    Q = quiver( X[::3, ::3], Y[::3, ::3], U[::3, ::3], V[::3, ::3],
  ##             color='r', units='x',
  ##                linewidths=(2,), edgecolors=('k'), headaxislength=5, scale=1/35.0  )

  qk = quiverkey(Q, 0.95, 1.05, 0.1, '0.1', labelpos='W')

  #ax = plt.axis([0, NY-1, 0, NX-1]) #hack again??
  #ax = plt.axis([45, 135, 15, 40]) #hack again??
  #ax = plt.axis([100, 135, 20, 35]) #hack again??
  ax = plt.axis([85, 175, 65, 85]) #hack again??

  # place this bullshit here in irder to change to correct some small mistaces in coord
  #============================ plots decoration (stupid) ==========================
  x_max = 300 #200
  x_start = 91 #50
  x_end = 168 #129
  y_start = 70 #25
  y_end = 79 #29
  x_h = 4; #2
  x_d = 4; #2
  y_sl = 0.2 #0.2
  # draw left line part
  plt.plot([0,x_start+0.5,x_start+0.5,0],[y_start-0.5+y_sl,y_start-0.5+y_sl,y_end+0.5+y_sl,y_end+0.5+y_sl],'k')
  gaps_quant = 9;
  # draw line boxes
  for i in range(0, gaps_quant):
    plt.plot([x_start+x_d+0.5+i*(x_d+x_h),x_start+x_d+0.5+i*(x_d+x_h),x_start+x_d+x_h+0.5+i*(x_d+x_h),\
              x_start+x_d+x_h+0.5+i*(x_d+x_h),x_start+x_d+0.5+i*(x_d+x_h)],\
             [y_end+0.5+y_sl,y_start-0.5+y_sl,y_start-0.5+y_sl,y_end+0.5+y_sl,y_end+0.5+y_sl],'k')
  # draw right line part
  plt.plot([x_max,x_end-0.5,x_end-0.5,x_max],[y_start-0.5+y_sl,y_start-0.5+y_sl,y_end+0.5+y_sl,y_end+0.5+y_sl],'k')

  def draw_dash( x, y, delta, d_size, size, is_horisontal ):
    for i in range(0, int(size/delta)):
      if is_horisontal:
        plt.plot([x+i*delta,x+i*delta+d_size],[y,y+d_size],'k')
      else:
        plt.plot([x,x+d_size],[y+i*delta,y+i*delta+d_size],'k')
  def draw_dash_box( x, y, size_x, size_y, delta ):
    sp_sl_y = 0.0
    for i in range(0, 200):
      if i*delta > (size_x+size_y):
        break;
      if i*delta < size_x:
        plt.plot([x,x+i*delta],[y+size_y-delta*i+sp_sl_y,y+size_y+sp_sl_y],'k')
      else:
        if i*delta > size_y:
          plt.plot([x+i*delta-size_y,x+size_x],[y+sp_sl_y,y+size_y-delta*i+size_x+sp_sl_y],'k')
        else:
          plt.plot([x,x+size_x],[y+size_y-delta*i+sp_sl_y,y+size_y-delta*i+size_x+sp_sl_y],'k')
  d_size = 0.6; #1.0
  x=0
  y=y_start-0.5
  box_size_y = y_end-y_start+1;
  delta=0.6 #1.1
  ##    # draw left dash part
  ##    draw_dash( -0.1, y+y_sl, delta, d_size, x_start+0.5, 1 )
  ##    draw_dash( x_start+0.5-d_size, y+y_sl, delta, d_size, box_size_y, 0 )
  ##    draw_dash( 0, y_end+0.5-d_size+y_sl, delta, d_size*0.9, x_start+1.5, 1 )
  ##    # draw dash boxes
  ##    for i in range(0, gaps_quant):
  ##        draw_dash_box( i*(x_d+x_h)+ x_start+0.5+x_d, y_start-0.5+y_sl, x_h, box_size_y-0.1, delta )
  ##    # draw right dash part
  ##    draw_dash( x_end-0.5, y+y_sl, delta, d_size, x_max-x_end+0.5, 1 )
  ##    draw_dash( x_end-0.5, y+y_sl, delta, d_size, box_size_y, 0 )
  ##    draw_dash( x_end-0.5, y_end+0.5-d_size+y_sl, delta, d_size*0.9, x_max-x_end+0.5, 1 )
  draw_dash( -0.1, y+y_sl, delta, d_size, x_start+0.5, 1 )
  draw_dash( x_start+0.5-d_size, y+y_sl, delta, d_size, box_size_y, 0 )
  draw_dash( 0, y_end+0.5-d_size+y_sl, delta, d_size*0.9, x_start+1.5, 1 )
  for i in range(0, gaps_quant):
    #draw_dash_box( i*4+ 50+0.5+2, 25-0.5+y_sl, 2, 4.9, delta )
    draw_dash( i*(x_d+x_h)+ x_start+0.5+x_d, y_start-0.5+y_sl, delta, d_size, x_h, 1 )
    draw_dash( i*(x_d+x_h)+ x_start+0.5+x_d, y_start-0.5+y_sl, delta, d_size, box_size_y-0.1, 0 )
    draw_dash( i*(x_d+x_h)+ x_start+0.5+x_d, y_start-0.5+y_sl+box_size_y-0.1-d_size, delta, d_size, x_h, 1 )
    draw_dash( i*(x_d+x_h)+ x_start+0.5+x_d+x_h-d_size, y_start-0.5+y_sl, delta, d_size, box_size_y-0.1, 0 )
  draw_dash( x_end-0.5, y+y_sl, delta, d_size, x_max-x_end+0.5, 1 )
  draw_dash( x_end-0.5, y+y_sl, delta, d_size, box_size_y, 0 )
  draw_dash( x_end-0.5, y_end+0.5-d_size+y_sl, delta, d_size*0.9, x_max-x_end+0.5, 1 )
  #==================================================================================

  f_info = file("../output/gas %i/info%i.txt"%(gas_index,quantity_of_files),"r")
  f_info.readline()
  f_info.readline()
  f_info.readline()
  quantity_of_iterations = f_info.readline().rsplit()[4]
  f_info.readline()
  f_info.readline()
  f_info.readline()
  time_step = f_info.readline().rsplit()[2]

  str_title = "Gas %i  Gas Velocity  t = %.2lf" % (gas_index, int(quantity_of_iterations)*float(time_step));
  if (FLAG_use_titles):
    plt.title(str_title)

  output_file_name = "%s%s%i.png" % ("result " + str(gas_index) + "/enlarged velocity right/",\
                                     "enlarged_velocity", quantity_of_files)
  plt.savefig(output_file_name)
  #plt.show()
  print 75+100/4*float(quantity_of_files)/end_quantity_of_files, "%"
  quantity_of_files = quantity_of_files + 1

  plt.close()

# Enlarged gas velocity LEFT
quantity_of_files = start_quantity_of_files
start_file_name = "gas_velocity"
while(quantity_of_files < end_quantity_of_files):
  input_file_name = "%s%i%s%s%i.txt" % ("../output/gas ", gas_index, "/", start_file_name, quantity_of_files)
  data = np.loadtxt(input_file_name)
  
  #data begining with concentration
  quantity_of_contours = max_quantity_of_contours_temp
  
  NX=GridSize_x;
  NY=GridSize_y;
  X = data[:,0].reshape(NX,NY)
  Y = data[:,1].reshape(NX,NY)
  U = data[:,2].reshape(NX,NY)
  V = data[:,3].reshape(NX,NY)
  
  for i in range(0, NX):
    for j in range(0, NY):
      if U[i,j] == 100:
        if V[i,j] == 100:
          U[i,j] = np.nan
          V[i,j] = np.nan

  #plt.figure(figsize=(34.7/2.54, 27.75/2.54)) # in santimeters
  plt.figure(figsize=(34.7/2.54, NX/NY*27.75/2.54)) # in santimeters

  M = sqrt(pow(U, 2) + pow(V, 2))

  ##    Q = quiver( X[::3, ::3], Y[::3, ::3], U[::3, ::3], V[::3, ::3],
  ##            pivot='mid', color='r', units='inches' )

  ##    Q = quiver( X, Y, U, V,
  ##                M, clim=(min_mod_gas_velocity, max_mod_gas_velocity), units='x',
  ##            pivot='mid', width=0.5/2.0, scale=1/5.0 )
  
  Q = quiver( X, Y, U, V, clim=(min_mod_gas_velocity, max_mod_gas_velocity), units='x', pivot='mid', width=0.12/2.0, scale=1.0/25.0 )
             
  ##    Q = quiver( X[::3, ::3], Y[::3, ::3], U[::3, ::3], V[::3, ::3],
  ##             color='r', units='x',
  ##                linewidths=(2,), edgecolors=('k'), headaxislength=5, scale=1/35.0  )

  qk = quiverkey(Q, 0.95, 1.05, 0.1, '0.1', labelpos='W')

  #ax = plt.axis([0, NY-1, 0, NX-1]) #hack again??
  #ax = plt.axis([45, 135, 15, 40]) #hack again??
  ax = plt.axis([44, 79, 20, 35]) #hack again??

  # place this bullshit here in irder to change to correct some small mistaces in coord
  #============================ plots decoration (stupid) ==========================
  x_max = 300 #200
  x_start = 91 #50
  x_end = 168 #129
  y_start = 70 #25
  y_end = 79 #29
  x_h = 4; #2
  x_d = 4; #2
  y_sl = 0.2 #0.2
  # draw left line part
  plt.plot([0,x_start+0.5,x_start+0.5,0],[y_start-0.5+y_sl,y_start-0.5+y_sl,y_end+0.5+y_sl,y_end+0.5+y_sl],'k')
  gaps_quant = 9;
  # draw line boxes
  for i in range(0, gaps_quant):
    plt.plot([x_start+x_d+0.5+i*(x_d+x_h),x_start+x_d+0.5+i*(x_d+x_h),x_start+x_d+x_h+0.5+i*(x_d+x_h),\
              x_start+x_d+x_h+0.5+i*(x_d+x_h),x_start+x_d+0.5+i*(x_d+x_h)],\
             [y_end+0.5+y_sl,y_start-0.5+y_sl,y_start-0.5+y_sl,y_end+0.5+y_sl,y_end+0.5+y_sl],'k')
  # draw right line part
  plt.plot([x_max,x_end-0.5,x_end-0.5,x_max],[y_start-0.5+y_sl,y_start-0.5+y_sl,y_end+0.5+y_sl,y_end+0.5+y_sl],'k')

  def draw_dash( x, y, delta, d_size, size, is_horisontal ):
    for i in range(0, int(size/delta)):
      if is_horisontal:
        plt.plot([x+i*delta,x+i*delta+d_size],[y,y+d_size],'k')
      else:
        plt.plot([x,x+d_size],[y+i*delta,y+i*delta+d_size],'k')
  def draw_dash_box( x, y, size_x, size_y, delta ):
    sp_sl_y = 0.0
    for i in range(0, 200):
      if i*delta > (size_x+size_y):
        break;
      if i*delta < size_x:
        plt.plot([x,x+i*delta],[y+size_y-delta*i+sp_sl_y,y+size_y+sp_sl_y],'k')
      else:
        if i*delta > size_y:
          plt.plot([x+i*delta-size_y,x+size_x],[y+sp_sl_y,y+size_y-delta*i+size_x+sp_sl_y],'k')
        else:
          plt.plot([x,x+size_x],[y+size_y-delta*i+sp_sl_y,y+size_y-delta*i+size_x+sp_sl_y],'k')
  d_size = 2.0; #1.0
  x=0
  y=y_start-0.5
  box_size_y = y_end-y_start+1;
  delta=2.1 #1.1
  # draw left dash part
  draw_dash( -0.1, y+y_sl, delta, d_size, x_start+0.5, 1 )
  draw_dash( x_start+0.5-d_size, y+y_sl, delta, d_size, box_size_y, 0 )
  draw_dash( 0, y_end+0.5-d_size+y_sl, delta, d_size*0.9, x_start+1.5, 1 )
  # draw dash boxes
  for i in range(0, gaps_quant):
    draw_dash_box( i*(x_d+x_h)+ x_start+0.5+x_d, y_start-0.5+y_sl, x_h, box_size_y-0.1, delta )
  # draw right dash part
  draw_dash( x_end-0.5, y+y_sl, delta, d_size, x_max-x_end+0.5, 1 )
  draw_dash( x_end-0.5, y+y_sl, delta, d_size, box_size_y, 0 )
  draw_dash( x_end-0.5, y_end+0.5-d_size+y_sl, delta, d_size*0.9, x_max-x_end+0.5, 1 )
  #==================================================================================

  f_info = file("../output/gas %i/info%i.txt"%(gas_index,quantity_of_files),"r")
  f_info.readline()
  f_info.readline()
  f_info.readline()
  quantity_of_iterations = f_info.readline().rsplit()[4]
  f_info.readline()
  f_info.readline()
  f_info.readline()
  time_step = f_info.readline().rsplit()[2]

  str_title = "Gas %i  Gas Velocity   t = %.2lf" % (gas_index, int(quantity_of_iterations)*float(time_step));
  if (FLAG_use_titles):
    plt.title(str_title)

  output_file_name = "%s%s%i.png" % ("result " + str(gas_index) + "/enlarged velocity left/", "enlarged_velocity", quantity_of_files)
  plt.savefig(output_file_name)
  #plt.show()
  print 75+100/4*float(quantity_of_files)/end_quantity_of_files, "%"
  quantity_of_files = quantity_of_files + 1

  plt.close()

print "Done."
#input("Done.")
