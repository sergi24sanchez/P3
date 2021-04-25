#!/bin/bash

for potvalue in -49; do
for r1norm in 0.87; do
for rmaxnorm in 0.29; do
# Put here the program (maybe with path)
GETF0="get_pitch -p $potvalue -r $r1norm -m $rmaxnorm"

echo "Param: potvalue: $potvalue    r1norm: $r1norm     rmaxnorm: $rmaxnorm"
for fwav in pitch_db/train/*.wav; do
    ff0=${fwav/.wav/.f0}
    echo "$GETF0 $fwav $ff0 ----"
    $GETF0 $fwav $ff0 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
done


pitch_evaluate pitch_db/train/*.f0ref
done
done
done
exit 0
