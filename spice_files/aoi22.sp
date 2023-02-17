.SUBCKT aoi22 vdd gnd a b c d out 
Mpa out a x vdd PMOS W=4u L=0.24u 
Mpb out b x vdd PMOS W=4u L=0.24u 
Mpc x c vdd vdd PMOS W=4u L=0.24u 
Mpd x d vdd vdd PMOS W=4u L=0.24u 
Mna out a y gnd NMOS W=2u L=0.24u 
Mnb y b gnd gnd NMOS W=2u L=0.24u 
Mnc out c z gnd NMOS W=2u L=0.24u 
Mnd z d gnd gnd NMOS W=2u L=0.24u 
.ENDS 
