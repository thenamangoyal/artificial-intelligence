#!/bin/bash

make
for i in {1..20}; do
	echo "Running ${i}.txt"
	./auction < ${i}.txt
done
