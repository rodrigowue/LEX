# spice-arc-extractor
CMOS SPICE ARC EXTRACTOR <br>
[UNDER DEVELOPMENT] <br>

This tool is ment to extract arcs from Pull-Down (PDN) and Pull-up Networks (PUN) while analysing a node-graph representation.

To run:

>make

>./a.out <spice file>

or

>g++ main.cpp stlGraph.cpp transistor.cpp

>./a.out <spice file>

By running the nand3.sp example the tool outputs:

(C.(B.A))

(the ISOP expression since the CMOS is negated by nature)



Maybe it could become a part of an open-source characterization tool in the future.
