#!/bin/bash

read_sizes=(1 1024 2048 4096 8192)
num_threads=(1 2 4 8 16)
input_path="./test_input/affordable-health.txt"

# Directory to store output files
output_dir="./outputs2/raw"
mkdir -p "$output_dir"


# read mode
for size in "${read_sizes[@]}"; do
    ./doit ./proj6 $input_path $size p0 > "$output_dir/read_size_${size}.txt"
done

# mmap mode
for num in "${num_threads[@]}"; do
    ./doit ./proj6 $input_path mmap p$num > "$output_dir/mmap_${num}_threads.txt"
done


echo "All tests completed. Check $output_dir for outputs."
