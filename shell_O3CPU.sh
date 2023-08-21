#!/bin/bash

gem5_executable=~/gem5/build/X86/gem5.opt
se=~/gem5/configs/deprecated/example/se.py

output_folder=~/arq2prova1/output/

mkdir -p output_folder

edmonds_executable=~/arq2prova1/edmondskarp/exec
INPUT_FILE_PRIM=("~/arq2prova1/prim/geradorDeTestes/100teste.txt")
INPUT_FILE_EDMONDS=("~/arq2prova1/edmondskarp/geradorDeTestes/100vertices.txt" \
        "~/arq2prova1/edmondskarp/geradorDeTestes/1000vertices.txt")

for entrada in ${INPUT_FILE_EDMONDS[@]}; do \
    $gem5_executable $se --caches --l1i_size=16kB --l1d_size=64kB \
    --cpu-type=O3CPU --bp-type=BiModeBP --cmd=$edmonds_executable -o $entrada \
	
    $gem5_executable $se --caches --l1i_size=16kB --l1d_size=64kB \
    --cpu-type=O3CPU --bp-type=LocalBP --cmd=$edmonds_executable -o $entrada

done
