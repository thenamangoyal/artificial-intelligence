#!/usr/bin/env bash
make
echo "Running satsolver"
time ./satsolver input/p.txt output/out_sat.txt
echo "Output exported to output/out_sat.txt"