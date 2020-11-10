#!/bin/bash

#$1 Gol.exe
#$2 UID
#$3 Inputfile-Folder
#$4 Threads

#echo "Removing Folder 'EPR_Output'"
#rm -r EPR_Output
echo "Creating Folder 'EPR_Output"
mkdir EPR_Output
mkdir EPR_Output/OutputFiles

for i in 1000 2000 3000 4000 5000 6000 7000 8000 9000 10000
do
    echo "Running File '$i', Mode=SEQ"
    ./$1 --load $3/random"$i"_in.gol --save EPR_Output/OutputFiles/"$2"_"$i"_cpu_out.gol --generations 250 --measure --mode seq >> EPR_Output/"$2"_cpu_time.csv
    echo "Running File '$i', Mode=OMP, Threads=$4"
    ./$1 --load $3/random"$i"_in.gol --save EPR_Output/OutputFiles/"$2"_"$i"_openmp_out.gol --generations 250 --measure --mode omp --threads $4 >> EPR_Output/"$2"_openmp_time.csv
    echo "Running File '$i', Mode=OCL, Device=CPU"
    ./$1 --load $3/random"$i"_in.gol --save EPR_Output/OutputFiles/"$2"_"$i"_opencl_cpu_out.gol --generations 250 --measure --mode ocl --device cpu >> EPR_Output/"$2"_opencl_cpu_time.csv
    echo "Running File '$i', Mode=OCL, Device=GPU"
    ./$1 --load $3/random"$i"_in.gol --save EPR_Output/OutputFiles/"$2"_"$i"_opencl_gpu_out.gol --generations 250 --measure --mode ocl --device gpu >> EPR_Output/"$2"_opencl_gpu_time.csv
done

