Physics Projects
================
This projects connected with our studying at MIPT.

Grid Solver
----------------

####What we have done:
1. Gas transport solver (without collision integral)
2. Two gases
3. 3D grid
4. Checking mass conservation (delta is about 1*e-13 per iteration)
5. Computing global parameters (temperature, concentration) and plotting simple graphics
6. Read/write grid config functions
7. Build our final grid in 3D, but with small sizes (like 30x10x6) to show the results to Cheremisin
8. Add colision integral
9. Parallel our solver with MPI

![image](https://dl.dropboxusercontent.com/u/59969938/Images/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%20%D0%BE%D1%82%202013-12-07%2019%3A25%3A06.png)

Some demo result:

![image](https://dl.dropboxusercontent.com/u/59969938/Images/snap.png)

One iteration with our small grid:
![image](https://dl.dropboxusercontent.com/u/59969938/Images/plot.png)

Five iterations with our small grid:
![image](https://dl.dropboxusercontent.com/u/59969938/Images/plot_5_it.png)

####What we need to do:

1. Set streams
2. Add ability of using symmerty
3. Optimize the solver and compute bigger grids
4. Develop more beautifull plots
5. Maybe draw some 3D plots with OSG

####What shall we do next?

Set streams and compute real task for small grid.

Grid Maker
----------------

1. Making sample cube grid.
2. Making our small grid.
