#!/usr/bin/env bash

function usage() {
    echo "Usage: $0 [-s size] [-f filename]"
    echo "Options:"
    echo "  -s  The size of the array to sort"
    echo "  -f  The output plot file name (without extension)"
    echo "  -h  Display this help message"
    exit 0
}

# Parse command line arguments
[ $# -eq 0 ] && usage
while getopts "hs:f:" arg; do
    case $arg in
        s) # The size of the array to sort
            size=${OPTARG}
            ;;
        f) # The plot file name
            name=${OPTARG}
            ;;
        h | *) # Display help
            usage
            exit 0
            ;;
    esac
done

# Validate required arguments
if [ -z "$name" ] || [ -z "$size" ]; then
    echo "Error: Both size (-s) and filename (-f) are required"
    usage
    exit 1
fi

# Check if myprogram exists
if [ ! -e "./myprogram" ]; then
    echo "Error: myprogram not found. Did you compile your code?"
    exit 1
fi

# Remove old data file if it exists
if [ -e "data.dat" ]; then
    rm -f data.dat
fi

# Create header for data file
echo "# Time Threads" > data.dat

echo "Running myprogram to generate data..."
# Run program with different thread counts
for n in $(seq 1 32); do
    echo -ne "Running with $n thread(s)...\r"
    ./myprogram "$size" "$n" >> data.dat
done
echo -e "\nData collection complete."

# Check if gnuplot is installed
if ! command -v gnuplot &> /dev/null; then
    echo "Error: gnuplot is not installed. Please install it to generate plots."
    echo "Data has been saved to data.dat"
    exit 1
fi

# Generate plot
gnuplot -e "filename='$name.png'" graph.plt

echo "Created plot $name.png from data.dat file"