echo "Project 4"
foreach ArrSize (1000 2000 4000 8000 16000 32000 64000 128000 256000 512000 1024000 2048000 4096000 8192000)
  g++ -DARRAYSIZE=$ArrSize all04.cpp -o all04 -lm -fopenmp
  ./all04
end