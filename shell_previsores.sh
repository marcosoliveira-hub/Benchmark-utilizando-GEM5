#!/bin/bash

GEM5_DIR=~/gem5
GEM5_BUILD_DIR=$GEM5_DIR/build/X86/gem5.opt
GEM5_SE_DIR=$GEM5_DIR/configs/deprecated/example/se.py
NOME_PROG=("edmondskarp_exe" "prim_exe")
ENTRADAS=(200teste.txt 400teste.txt)
SAIDAS=(saida_200.txt saida_400.txt)
QTDE_ENTRADAS=(200 400)
PREVISORES=(LocalBP BiModeBP)

output="./saida_previsores"
saida_algoritmos="${output}/saida_algoritmos"

mkdir -p $output

mkdir -p $saida_algoritmos/edmondskarp
mkdir -p $saida_algoritmos/prim

for programa in ${NOME_PROG[0]}; do
    idx=0
    for entrada in ${ENTRADAS[@]}; do
        for previsor in ${PREVISORES[@]}; do
            echo "Executando $programa com $previsor e $entrada"

            ${GEM5_BUILD_DIR} -d ${output} ${GEM5_SE_DIR}\
            --cmd=./edmondskarp/${programa}\
            --options="./edmondskarp/geradorDeTestes/${entrada} ${saida_algoritmos}/edmondskarp/${SAIDAS[idx]}"\
            --cpu-type=O3CPU --caches --l1d_size=64kB --l1i_size=16kB\
            --bp-type=${previsor}

            mv ${output}/stats.txt ${output}/${programa}_${previsor}_${QTDE_ENTRADAS[idx]}.txt

            echo "Finalizado $programa com $previsor e $entrada"

        done
        idx=$((idx+1))
    done
done

for programa in ${NOME_PROG[1]}; do
    idx=0
    for entrada in ${ENTRADAS[@]}; do
        for previsor in ${PREVISORES[@]}; do
            echo "Executando $programa com $previsor e $entrada"

            ${GEM5_BUILD_DIR} -d ${output} ${GEM5_SE_DIR} --cmd=./prim/${programa}\
            --options="./prim/geradorDeTestes/${entrada} ${saida_algoritmos}/prim/${SAIDAS[idx]}"\
            --cpu-type=O3CPU --caches --l1d_size=64kB --l1i_size=16kB\
            --bp-type=${previsor}

            mv ${output}/stats.txt ${output}/${programa}_${previsor}_${QTDE_ENTRADAS[idx]}.txt

            echo "Finalizado $programa com $previsor e $entrada"

        done
        idx=$((idx+1))
    done
done
