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
ENTRADAS=(100teste.txt 150teste.txt)
SAIDAS=(saida_100.txt saida_150.txt)
QTDE_ENTRADAS=(100 150)
CONFIGURACOES=(
"--cpu-clock=2GHz --mem-size=2GB \
            --param system.cpu[0].fetchWidth=6 \
            --param system.cpu[0].decodeWidth=6 \
            --param system.cpu[0].commitWidth=6 --bp-type=BiModeBP"

"--cpu-clock=2GHz --mem-size=2GB \
            --param system.cpu[0].decodeWidth=2 \
            --param system.cpu[0].commitWidth=2 \
            --param system.cpu[0].fetchWidth=2 --bp-type=BiModeBP")
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
        for config in "${CONFIG_LABEL[@]}"; do
            echo "Executando $programa com $config e $entrada"

            ${GEM5_BUILD_DIR} -d ${output} ${GEM5_SE_DIR}\
            --cmd=$TOP_DIR/edmondskarp/${programa}\
            --options="$TOP_DIR/edmondskarp/geradorDeTestes/${entrada} ${saida_algoritmos}/edmondskarp/${SAIDAS[idx]}"\
            --cpu-type=O3CPU --caches --l1d_size=128kB --l1i_size=32kB \
            ${CONFIGURACOES[idxConfig]}

            mv ${output}/stats.txt ${output}/${programa}_${config}_${QTDE_ENTRADAS[idx]}.txt

            echo "Finalizado $programa com $config e $entrada"

            idxConfig=$((idxConfig+1))
        done
        idx=$((idx+1))
    done
done

for programa in ${NOME_PROG[0]}; do
    idx=0
    for entrada in ${ENTRADAS[@]}; do
        idxConfig=0
        for config in "${CONFIG_LABEL[@]}"; do
            echo "Executando $programa com $config e $entrada"

            ${GEM5_BUILD_DIR} -d ${output} ${GEM5_SE_DIR}\
            --cmd=$TOP_DIR/prim/${programa}\
            --options="$TOP_DIR/prim/geradorDeTestes/${entrada} ${saida_algoritmos}/prim/${SAIDAS[idx]}"\
            --cpu-type=O3CPU --caches --l1d_size=128kB --l1i_size=32kB \
            ${CONFIGURACOES[idxConfig]}

            mv ${output}/stats.txt ${output}/${programa}_${config}_${QTDE_ENTRADAS[idx]}.txt

            echo "Finalizado $programa com $config e $entrada"

            idxConfig=$((idxConfig+1))
        done
        idx=$((idx+1))
    done
done
