Instructions:

1. Use these commands to compile into main:
  "clang++ -Xpreprocessor -fopenmp -I/usr/local/include -L/usr/local/lib -lomp  1_thread.cpp -o main" 
  or 
  "clang++ -Xpreprocessor -fopenmp -I/usr/local/include -L/usr/local/lib -lomp  4_thread.cpp -o main"
  
2. Run "./main" from the commandline to view output