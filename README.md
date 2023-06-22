# LEX - SPICE STD-CELL ARCS EXTRACTOR


This tool is ment to extract arcs from Pull-Down (PDN) and Pull-up Networks (PUN) while analysing a node-graph representation.

To run:

```
make
./lex <spice file>
```

or

```
g++ main.cpp map.cpp transistor.cpp
./a.out <spice file>
```

By running the spice_files/nand3.sp example the tool outputs:

```
======================================
 SPICE STD-CELL TIMING ARCS EXTRACTOR
       [UNDER DEVELOPMENT]
======================================
Subcircuit:NAND3D0BWP
----------------------------------------
Inputs & Outputs

input:A
input:B
input:C
output:OUT
----------------------------------------
PUN Expressions:
OUT=(C*(A*B))

PDN Expressions:
OUT=(C*(A*B))
----------------------------------------
Expression After Flattening: 
!((C*(A*B)))*!((C*(A*B)))
----------------------------------------
TRUTH TABLE:
000|1
100|1
010|1
110|1
001|1
101|1
011|1
111|0
----------------------------------------
ARCS:
Number of literals: 3
1 1 F  | Rise
1 F 1  | Rise
F 1 1  | Rise
R 1 1  | Fall
1 R 1  | Fall
1 1 R  | Fall
----------------------------------------
```
A <*>.arcs file is generated as an output.


This tool is currently feeding a characterization tool that is being developed under the following repo:

https://github.com/rodrigowue/DPAC
