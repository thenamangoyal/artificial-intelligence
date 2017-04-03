#!/usr/bin/env bash
make
echo "Running bs"
time bin/bs input/p.txt output/out_bs.txt > stat/stat_bs.txt
echo "Output exported to output/out_bs.txt"

echo "Running bsI"
time bin/bsI input/p.txt output/out_bsI.txt > stat/stat_bsI.txt
echo "Output exported to output/out_bsI.txt"

echo "Running bsII"
time bin/bsII input/p.txt output/out_bsII.txt > stat/stat_bsII.txt
echo "Output exported to output/out_bsII.txt"

echo "Running bsMAC"
time bin/bsMAC input/p.txt output/out_bsMAC.txt > stat/stat_bsMAC.txt
echo "Output exported to output/out_bsMAC.txt"