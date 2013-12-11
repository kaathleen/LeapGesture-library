#!/bin/sh

for (( i=1; $i < 6; i=i+1 )) ; do
	txt="./train gesture$i.dat.scale";
    eval $txt

	txt="./predict gesture$i.dat.scale gesture$i.dat.scale.model tmp > out$i";
    eval $txt	   
done
