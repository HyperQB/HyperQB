#!/bin/sh

### Parameters
SINGLE_PARSER=exec/single_model_parser.py
MULTI_PARSER=exec/multi_model_parser.py
# HyperQube subTools
GENQBF=exec/genqbf
QUABS=exec/quabs
MAP=exec/util_mapvars
PARSE_BOOL=exec/util_parsebools
# output files
QCIR_OUT=HQ.qcir
QUABS_OUT=HQ.quabs
MAP_OUT1=OUTPUT_byName.cex
MAP_OUT2=OUTPUT_byTime.cex
PARSE_OUT=OUTPUT_formatted.cex


## updated Jan.28:merge parse and bmc
echo "\n------( HyperQube START! )------\n"
## get current location
PWD=$(pwd)
## get all arguments
ALLARG=$@
# echo ${ALLARG}

## execute python scripts on docker
docker run -v ${PWD}:/mnt tzuhanmsu/hyperqube:latest /bin/bash -c "cd mnt/; ./parse.sh ${ALLARG}; "

## clean up previous generated
echo "(clean up previous generated files..)\n"
find . -name "*.bool"   -delete
find . -name "*.cex"    -delete
find . -name "*.quabs"  -delete
find . -name "*.qcir"   -delete
find . -name "*.cex"    -delete
# make clean

# if wrong number of arguments
if [ "$#" -ne 4 ] && [ "$#" -ne 5 ] && [ "$#" -ne 6 ] && [ "$#" -ne 7 ]; then
  echo "HyperQube error: wrong number of arguments of HyperQube: \n"
  echo "- Simgle-model BMC: $0 {model}.smv {formula}.hq"
  echo "- Multi-model BMC:  $0 {model_1}.smv {model_2}.smv {formula}.hq \n"
  echo "\n------(END HyperQube)------\n"
  exit 1
fi



if echo $* | grep -e "-find" -q
then
  echo "[ Running with find witness mode ]"
  FLAG="-find"
elif echo $* | grep -e "-bughunt" -q
then
  echo "[ Running with bug hunting mode ]"
  FLAG="-bughunt"
else
  echo FLAG=""
fi


if echo $* | grep -e "-single" -q
then
  echo "[ Running with single model mode ]"
  MODE=single
  echo "perform single model BMC"
  NUSMVFILE=$1
  FORMULA=$2
  k=$3
  SEMANTICS=$4
  if [ ! -f "$NUSMVFILE" ]; then
      echo "error: $NUSMVFILE does not exist"
      exit 1
  fi
  if [ ! -f "$FORMULA" ]; then
      echo "error: $FORMULA does not exist"
      exit 1
  fi
  I=I.bool
  R=R.bool
  J=I.bool
  S=R.bool
  P=P.bool

  # python3 ${SINGLE_PARSER} ${NUSMVFILE} ${FORMULA} ${I} ${R} ${P} ${FLAG}
  ### using docker
  docker run -v ${PWD}:/mnt tzuhanmsu/hyperqube:latest /bin/bash -c "cd mnt/; python3  ${SINGLE_PARSER} ${NUSMVFILE} ${FORMULA} ${I} ${R} ${P} ${FLAG}; "
elif echo $* | grep -e "-multi" -q
then
  echo "[ Running with multi model mode ]"
  MODE=multi
  M1_NUSMVFILE=$1
  M2_NUSMVFILE=$2
  FORMULA=$3
  k=$4
  SEMANTICS=$5
  if [ ! -f "$M1_NUSMVFILE" ]; then
      echo "error: $M1_NUSMVFILE does not exist"
      exit 1
  fi
  if [ ! -f "$M2_NUSMVFILE" ]; then
      echo "error: $M2_NUSMVFILE does not exist"
      exit 1
  fi
  if [ ! -f "$FORMULA" ]; then
      echo "error: $FORMULA does not exist"
      exit 1
  fi
  I=I.bool
  R=R.bool
  J=J.bool
  S=S.bool
  P=P.bool
  # python3 ${MULTI_PARSER} ${M1_NUSMVFILE} ${I} ${R} ${M2_NUSMVFILE} ${J} ${S} ${FORMULA}  ${P} ${FLAG}
  ### using docker
  docker run -v ${PWD}:/mnt tzuhanmsu/hyperqube:latest /bin/bash -c "cd mnt/; python3 ${MULTI_PARSER} ${M1_NUSMVFILE} ${I} ${R} ${M2_NUSMVFILE} ${J} ${S} ${FORMULA}  ${P} ${FLAG}; "
else
  echo "HyperQube error: please specify mode: -single | -multi \n"
  exit 1
fi


## run BMC
# ./bmc.sh ${ALLARG}
## updated Jan.28
# ./bmc_new.sh ${ALLARG}
if echo $* | grep -e "-pes" -q
then
  SEM="PES"
elif echo $* | grep -e "-opt" -q
then
  SEM="OPT"
elif echo $* | grep -e "-hpes" -q
then
  SEM="TER_PES"
elif echo $* | grep -e "-hopt" -q
then
  SEM="TER_OPT"
else
  echo "HyperQube error: incorrect semantic input. "
  echo " use { -pes | -opt | -hpes | -hopt } semantics of the unrolling from one of the follows:"
  echo "             (pessimistic / optimistic / halting-pessimistic / halting-optimistic)"
  exit 1
fi


if [ ! -f "QS.bool" ]; then
    exit 1
fi
source QS.bool


echo "\n--------------- Summary of Model Checking Info ---------------"
echo "|  Quantifiers:" ${QS}
echo "|  Bound k:    " ${k}
echo "|  Semantics:  " ${SEMANTICS}
if [ "$MODE" = "single" ]; then
  echo "|  Model:      " ${NUSMVFILE}
elif [ "$MODE" = "multi" ]; then
  echo "|  Models:     " ${M1_NUSMVFILE} ", " ${M2_NUSMVFILE}
fi
echo "|  HyperLTL formula: " ${FORMULA}
echo "-------------------------------------------------------------- \n\n"


echo "\n============ Unrolling with genQBF + Solving with QuAbS ============"
echo "generating QBF BMC..."
${GENQBF} -I ${I} -R ${R} -J ${J} -S ${S} -P ${P} -k ${k} -F ${QS}  -f qcir -o ${QCIR_OUT} -sem ${SEM} -n --fast


echo "solving QBF..."
${QUABS}  --partial-assignment ${QCIR_OUT} 2>&1 | tee ${QUABS_OUT}
#  ${QUABS} --statistics --preprocessing 0 --partial-assignment ${QCIR_OUT} 2>&1 | tee ${QUABS_OUT}


# echo "---Parse All Binary Numbers---"
echo "\n============ Get Nice-formatted Output if Output is avaialbe ============"

if [ ! -f "$QCIR_OUT" ]; then
    echo "$QCIR_OUT not exists"
    exit 1
fi

echo "parsing into readable format..."
# # echo "---Counterexample Mapping---"
# javac ${MAP}.java
# java ${MAP}.java ${QCIR_OUT} ${QUABS_OUT} ${MAP_OUT1} ${MAP_OUT2}
${MAP} ${QCIR_OUT} ${QUABS_OUT} ${MAP_OUT1} ${MAP_OUT2}

# javac ${PARSE_BOOL}.java
# java ${PARSE_BOOL}.java ${MAP_OUT2} ${PARSE_OUT}
${PARSE_BOOL} ${MAP_OUT2} ${PARSE_OUT}
# echo  "(under condtruction...)"
# python3 ${PARSE_OUTPUT} ${MAP_OUT2} ${PARSE_OUT} ${k}
# #by time

echo "\n------(END HyperQube)------\n"
