#!/bin/bash
make
for i in  {1..6}; do
echo "Running planner input/${i}.txt output/${i}.txt"
time ./planner input/${i}.txt output/${i}.txt
echo ""
done
