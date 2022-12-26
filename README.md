# S²CTAE - SPICE STD-CELL TIMING ARCS EXTRACTOR
```
  ____  ___  ____ _____  _    _____ 
 / ___||_  )/ ___|_   _|/ \  | ____|
 \___ \ / /| |     | | / _ \ |  _|  
  ___) /___| |___  | |/ ___ \| |___ 
 |____/     \____| |_/_/   \_\_____|
 SPICE STD-CELL TIMING ARCS EXTRACTOR
       [UNDER DEVELOPMENT]

```


This tool is ment to extract arcs from Pull-Down (PDN) and Pull-up Networks (PUN) while analysing a node-graph representation.

To run:

```
make
./a.out <spice file>
```

or

```
g++ main.cpp stlGraph.cpp transistor.cpp
./a.out <spice file>
```

By running the nand3.sp example the tool outputs:

```
--------------------------------------
Fetching Pinage Information
----------------------------------------
PMOS:
0:MP0 VDD OUT
1:MP1 VDD OUT
2:MP2 VDD OUT
NMOS:
3:MN0 OUT rand0
4:MN1 rand0 rand1
5:MN2 rand1 VSS
----------------------------------------
PDN Expression: (C*(A*B))
PUN Expression: (C+(A+B))
----------------------------------------
Expression: !((C+(A+B))*(C*(A*B)))
```

(the ISOP expression since the CMOS is negated by nature)



Maybe it could become a part of an open-source characterization tool in the future.
