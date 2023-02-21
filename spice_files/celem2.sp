.SUBCKT celem2 a b out vdd gnd
MP0 midp0 a   vdd   vdd PMOS W=4u L=0.24u 
MP1 mid   b   midp0 vdd PMOS W=4u L=0.24u 
MP2 midp1 b   vdd   vdd PMOS W=4u L=0.24u 
MP3 midp1 a   vdd   vdd PMOS W=4u L=0.24u 
MP4 mid   out midp1 vdd PMOS W=4u L=0.24u 
MP5 out   mid vdd   vdd PMOS W=4u L=0.24u 
MN0 mid   b   midn0 gnd NMOS W=4u L=0.24u 
MN1 midn0 a   gnd   gnd NMOS W=4u L=0.24u 
MN2 mid   out midn1 gnd NMOS W=4u L=0.24u 
MN3 midn1 b   gnd   gnd NMOS W=4u L=0.24u 
MN4 midn1 a   gnd   gnd NMOS W=4u L=0.24u 
MN5 out   mid gnd   gnd NMOS W=4u L=0.24u 
.ENDS 
