#!/bin/bash

IMGS=src/img
ASSETS_H=lib/assets.h

#rm -f $ASSETS_H
touch $ASSETS_H
for d in $IMGS/*; do

    if [[ $d != $IMGS/"icon" ]]; then
        for f in $d/*; do
            g=${f##*/}
            g=${g%.*}
            echo "#define ${g^^} \"src/${f#*/}\"" #>> $ASSETS_H
        done
    fi

done