echo "Project 4"
foreach ArrSize (1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608)
  g++ -DARRAYSIZE=$ArrSize all04.cpp -o all04 -lm -fopenmp
  ./all04
end