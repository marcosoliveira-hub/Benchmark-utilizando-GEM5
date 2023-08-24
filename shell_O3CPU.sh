#!/bin/bash

# Altere o TOP_DIR para a localização do diretório contendo as pastas /edmondskarp e /prim
TOP_DIR=~/arq2prova1    

# Altere GEM_DIR para o diretório onde o gem5 está localizado
GEM5_DIR=~/gem5

# Altere o executável do gem5 para gem5.fast, gem5.opt ou gem5.debug
GEM5_BUILD_DIR=$GEM5_DIR/build/X86/gem5.opt

# Caso a versao do gem5 seja anterior a 20/05/2020, altere o caminho para o script se.py
# para $GEM5_DIR/configs/example/se.py
GEM5_SE_DIR=$GEM5_DIR/configs/deprecated/example/se.py

NOME_PROG=("edmondskarp_exe" "prim_exe")
ENTRADAS=(150teste.txt 200teste.txt)
SAIDAS=(saida_150.txt saida_200.txt)
QTDE_ENTRADAS=(150 200)
CONFIGURACOES=("--num-cpus=4 --mem-type=DDR3_1600_8x8 --mem-size=2GB --cpu-clock=2GHz" 
            "--num-cpus=1 --mem-type=DDR3_1600_8x8 --mem-size=1GB --cpu-clock=1GHz")
CONFIG_LABEL=(Config1 Config2)

output="$TOP_DIR/saida_o3"
saida_algoritmos="${output}/saida_algoritmos"

mkdir -p $output

mkdir -p $saida_algoritmos/edmondskarp
mkdir -p $saida_algoritmos/prim

for programa in ${NOME_PROG[0]}; do
    idx=0
    for entrada in ${ENTRADAS[@]}; do
        idxConfig=0
        for config in ${!CONFIGURACOES[@]}; do
            echo "Executando $programa com ${CONFIG_LABEL[idxConfig]} e $entrada"

            ${GEM5_BUILD_DIR} -d ${output} ${GEM5_SE_DIR}\
            --cmd=$TOP_DIR/edmondskarp/${programa}\
            --options="$TOP_DIR/edmondskarp/geradorDeTestes/${entrada} ${saida_algoritmos}/edmondskarp/${SAIDAS[idx]}"\
            --cpu-type=O3CPU --caches --l1d_size=64kB --l1i_size=16kB\
            ${CONFIGURACOES[idxConfig]}

            mv ${output}/stats.txt ${output}/${programa}_${CONFIG_LABEL[idxConfig]}_${QTDE_ENTRADAS[idx]}.txt

            echo "Finalizado $programa com ${CONFIG_LABEL[idxConfig]} e $entrada"

            idxConfig=$((idxConfig+1))
        done
        idx=$((idx+1))
    done
done

for programa in ${NOME_PROG[1]}; do
    idx=0
    for entrada in ${ENTRADAS[@]}; do
        idxConfig=0
        for config in ${!CONFIGURACOES[@]}; do
            echo "Executando $programa com ${CONFIG_LABEL[idxConfig]} e $entrada"

            ${GEM5_BUILD_DIR} -d ${output} ${GEM5_SE_DIR}\
            --cmd=$TOP_DIR/prim/${programa}\
            --options="$TOP_DIR/prim/geradorDeTestes/${entrada} ${saida_algoritmos}/prim/${SAIDAS[idx]}"\
            --cpu-type=O3CPU --caches --l1d_size=64kB --l1i_size=16kB\
            ${CONFIGURACOES[idxConfig]}

            mv ${output}/stats.txt ${output}/${programa}_${CONFIG_LABEL[idxConfig]}_${QTDE_ENTRADAS[idx]}.txt

            echo "Finalizado $programa com ${CONFIG_LABEL[idxConfig]} e $entrada"
            
            idxConfig=$((idxConfig+1))

        done
        idx=$((idx+1))
    done
done
