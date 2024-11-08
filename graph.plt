# Gnuplot script for plotting multithreaded sorting performance

# Set the output format and style
set terminal png enhanced font "arial,10" size 800,600

# Set the output file from command line variable 'filename'
set output filename

# Set the title and labels
set title "Sorting Performance vs Number of Threads\nArray Size: " . system("echo $size") noenhanced
set xlabel "Number of Threads"
set ylabel "Time (milliseconds)"

# Configure the grid
set grid

# Configure the plot style
set style data linespoints
set style line 1 lc rgb '#0060ad' pt 7 ps 1.5 lt 1 lw 2

# Configure the key (legend)
set key top right

# Make sure we use exact data points
set xtics 1
set mxtics 1

# Plot the data
plot "data.dat" using 2:1 title "Sort Time" with linespoints ls 1