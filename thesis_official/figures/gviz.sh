#!/bin/bash

# maly skrypcik do konwersji dot i gviz'ow do PDFow. Moze sie przydac.

EXT=gviz
for i in `find . -name "*.${EXT}"`; do
    NAME=`basename $i .${EXT}`
    DIR=`dirname $i`
    BASE=${DIR}/${NAME}

    if [ $i -nt ${BASE}.eps ]; then
        echo "Processing ${EXT}: " $i
        dot -Tps -o ${BASE}.eps $i
        cat ${BASE}.eps | gs -q -dEPSCrop -dNOCACHE -sDEVICE=epswrite -sOutputFile=tmp - -c quit
        mv tmp ${BASE}.eps
        epstopdf ${BASE}.eps
    else
        echo "Skipping ${EXT}: " $i
    fi
done

EXT=dot
for i in `find . -name "*.${EXT}"`; do
    NAME=`basename $i .${EXT}`
    DIR=`dirname $i`
    BASE=${DIR}/${NAME}

    if [ $i -nt ${BASE}.eps ]; then
        echo "Processing ${EXT}: " $i
        neato -s -n2 -Tps -o ${BASE}.eps $i
        cat ${BASE}.eps | gs -q -dEPSCrop -dNOCACHE -sDEVICE=epswrite -sOutputFile=tmp - -c quit
        mv tmp ${BASE}.eps
        epstopdf ${BASE}.eps
    else
        echo "Skipping ${EXT}: " $i
    fi
done
