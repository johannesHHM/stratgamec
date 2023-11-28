#!/bin/bash

cd ../sprites

if [ "$#" -eq 0 ]
then
  units0="wall archer spearman swordsman"
else
  units0="$@"
fi

red0="#b45252"
red1="#9e4646"

green0="#8ab060"
green1="#73954b"

blue0="#4b80ca"
blue1="#3872bc"

for u in $units0; do
  states=$(ls units/0/$u/ | sed 's/.ase$//';)
  for s in $states; do
    path='../data/hero/0/animations/'$u'/'$s''

    aseprite -b units/0/$u/$s.ase \
	      --filename-format \
        ''$path'/0_{frame}.{extension}' \
        --save-as output.png

    # Create green versions
    for file in $path/0_*.png; do
    if [ -e "$file" ]; then
      x="${file%.*}"
      x="${x#*_}"

      green_file=""$path"/1_${x}.png"
      blue_file=""$path"/2_${x}.png"

      convert "$file" -fill "$green0" -opaque "$red0" tmp.png
      convert "tmp.png" -fill "$green1" -opaque "$red1" "$green_file"

      convert "$file" -fill "$blue0" -opaque "$red0" tmp.png
      convert "tmp.png" -fill "$blue1" -opaque "$red1" "$blue_file"
    fi
    done

    aseprite -b units/poof.ase \
	      --filename-format \
        '{path}/../data/hero/0/animations/'$u'/poof/0_{frame}.{extension}' \
        --save-as output.png
  done 
done


