#!/bin/bash

#SBATCH -J proj05
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --constraint=v100
#SBATCH --gres=gpu:1
#SBATCH -o proj06.out
#SBATCH -e proj06.csv
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=smitmic5@oregonstate.edu
for localS in 32 64 128 256 512
        do  
            # for 1KiB, 2KiB, 4KiB, 8KiB, 16KiB, 32KiB, 64KiB, 128KiB, 256KiB, 512KiB, 1MiB, 2 MiB, 4MiB, 8MiB
            for numElements in 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608
            do
                g++ -DLOCAL_SIZE=$localS -DNUM_ELEMENTS=$numElements -o proj6 first.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
                ./proj6
            done
        done