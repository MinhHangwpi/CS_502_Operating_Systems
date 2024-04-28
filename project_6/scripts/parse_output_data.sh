#!/bin/bash

output_dir="./outputs"
mmap_csv_file="$output_dir/mmap_results.csv"

# Create or clear the CSV file
echo "num_threads,file_size,num_strings,max_length,cpu_time,wall_clock_time,involuntary_context_switches,voluntary_context_switches,major_page_faults,minor_page_faults,max_rss" > "$mmap_csv_file"

# Function to extract a value given a label
extract_value() {
    grep "$1" | grep -o '[0-9]*' | head -1
}

# Read each mmap output file
for file in "$output_dir"/raw/mmap_*.txt; do
    num_threads=$(echo "$file" | grep -oP '(?<=mmap_)\d+')
    content=$(<"$file")
    
    # Extracting metrics
    file_size=$(echo "$content" | extract_value "File size")
    strings_of_length=$(echo "$content" | extract_value "Strings of at least length")
    max_string_length=$(echo "$content" | extract_value "Maximum string length")
    cpu_time=$(echo "$content" | extract_value "CPU Time Used")
    elapsed_time=$(echo "$content" | extract_value "Elapsed Wall-Clock Time")
    inv_context_switch=$(echo "$content" | extract_value "Involuntary Context Switches")
    vol_context_switch=$(echo "$content" | extract_value "Voluntary Context Switches")
    major_page_faults=$(echo "$content" | extract_value "Major Page Faults")
    minor_page_faults=$(echo "$content" | extract_value "Minor Page Faults")
    max_resident_set=$(echo "$content" | extract_value "Maximum Resident Set Size")
    
    # Writing to CSV
    echo "$num_threads,$file_size,$strings_of_length,$max_string_length,$cpu_time,$elapsed_time,$inv_context_switch,$vol_context_switch,$major_page_faults,$minor_page_faults,$max_resident_set" >> "$mmap_csv_file"
done


read_csv_file="$output_dir/read_results.csv"

# Create or clear the CSV file
echo "read_size,file_size,num_strings,max_length,cpu_time,wall_clock_time,involuntary_context_switches,voluntary_context_switches,major_page_faults,minor_page_faults,max_rss" > "$read_csv_file"

# Function to extract a value given a label
extract_value() {
    echo "$2" | grep "$1" | grep -o '[0-9]*' | head -1
}

# Read each read output file
for file in "$output_dir"/raw/read_size_*.txt; do
    read_size=$(echo "$file" | grep -oP '(?<=read_size_)\d+')
    content=$(<"$file")
    
    # Extracting metrics
    file_size=$(extract_value "File size" "$content")
    strings_of_length=$(extract_value "Strings of at least length" "$content")
    max_string_length=$(extract_value "Maximum string length" "$content")
    cpu_time=$(extract_value "CPU Time Used" "$content")
    elapsed_time=$(extract_value "Elapsed Wall-Clock Time" "$content")
    inv_context_switch=$(extract_value "Involuntary Context Switches" "$content")
    vol_context_switch=$(extract_value "Voluntary Context Switches" "$content")
    major_page_faults=$(extract_value "Major Page Faults" "$content")
    minor_page_faults=$(extract_value "Minor Page Faults" "$content")
    max_resident_set=$(extract_value "Maximum Resident Set Size" "$content")
    
    # Writing to CSV
    echo "$read_size,$file_size,$strings_of_length,$max_string_length,$cpu_time,$elapsed_time,$inv_context_switch,$vol_context_switch,$major_page_faults,$minor_page_faults,$max_resident_set" >> "$read_csv_file"
done

echo "CSV filea created: $mmap_csv_file, $read_csv_file"


