
# Plot u(x,y,t) in the heat conduction problem
#
#     u  = a(u   + u   ),  0 <= x <= 1, 0 <= y <= 1, 0 <= t < oo.
#      t      xx    yy
#
# For contour plot, the data must have a grid topology, i.e. all the isolines
# are of the same length.  The data points (x,y,u1,u2...) are grouped of
# the same size and separated by blank lines.
#
# Copyright(C) 2016-2021 Western University
# Ge Baolai
# Faculty of Science
# SHARCNET | Compute Ontario | Compute Canada
#
# using gnuplot.
set cntrparam bspline
set parametric	# Must set to parametric
set size square   # Make square, default display 5":3"
set view ,,1,1.5
set zrang[-1:1]
set nokey
unset border
unset xtics
unset ytics
unset ztics
set hidden3d

set term png medium

set term png medium
set output "output.0.png"
splot "output.0" using 2:3:4 with line
pause -1 "Hit return to exit."
set output "output.1.png"
splot "output.1" using 2:3:4 with line
pause -1 "Hit return to exit."
set output "output.10.png"
splot "output.10" using 2:3:4 with line
pause -1 "Hit return to exit."
set output "output.2.png"
splot "output.2" using 2:3:4 with line
pause -1 "Hit return to exit."
set output "output.3.png"
splot "output.3" using 2:3:4 with line
pause -1 "Hit return to exit."
set output "output.4.png"
splot "output.4" using 2:3:4 with line
pause -1 "Hit return to exit."
set output "output.5.png"
splot "output.5" using 2:3:4 with line
pause -1 "Hit return to exit."
set output "output.6.png"
splot "output.6" using 2:3:4 with line
pause -1 "Hit return to exit."
set output "output.7.png"
splot "output.7" using 2:3:4 with line
pause -1 "Hit return to exit."
set output "output.8.png"
splot "output.8" using 2:3:4 with line
pause -1 "Hit return to exit."
set output "output.9.png"
splot "output.9" using 2:3:4 with line
pause -1 "Hit return to exit."
