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

First result from server:

![image](https://dl.dropboxusercontent.com/u/59969938/Images/first_server_result.png)

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
