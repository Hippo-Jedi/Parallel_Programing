Instructions:

1. Navigate to the rabbit or DGX osu servers.

2. Navigate to the dirctory containing these files.

3. Run "g++ -o printinfo first.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp" on the commandline to run a single test with the given variables.

4. Run "./loop.bash >& file.csv" to output many runs to a .csv file.

5. Changing the i variable in first.cl will allow you to use either multiply or multiply-add. You can also uncomment a for loop in first.cpp on line 207 to use Multiply-reduction.