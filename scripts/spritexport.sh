#!/bin/bash

cd ../sprites

if [ "$#" -eq 0 ]
then
  units0="wall archer spearman swordsman"
else
  units0="$@"
fi

for u in $units0; do
  states=$(ls units/0/$u/ | sed 's/.ase$//';)
  for s in $states; do 
    c=0
    while [ $c -ne 3 ]
    do
      aseprite -b units/0/$u/$s.ase \
	      --filename-format \
	      '{path}/../data/hero/0/animations/'$u'/'$s'/'$c'_{frame}.{extension}' \
	      --save-as output.png

      aseprite -b units/poof.ase \
	      --filename-format \
	      '{path}/../data/hero/0/animations/'$u'/poof/'$c'_{frame}.{extension}' \
	      --save-as output.png
      c=$(($c+1))
    done
  done 
done


