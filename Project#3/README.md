Instructions:

1. Navigate to the directory with the main.cpp in it.

2. Run "clang++ -Xpreprocessor -fopenmp -I/usr/local/include -L/usr/local/lib -lomp  main.cpp -o main" or simply "Make" on the commandline to create an executable file called "main".

3. If you wish to create a .csv file and write the output to it then run "csh loop >& proj3.csv" on the commandline and view the results in the created .csv file.