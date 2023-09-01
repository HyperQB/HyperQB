#!/bin/sh
#####################################################################################################
#  This script runs ALL experiments (0.1-11.1) we provided in Table 3 of our tool paper submission  #
#####################################################################################################
HyperQB=./hyperqb.sh
PES='-pes' # default value
OPT='-opt'
HPES='-hpes'
HOPT='-hopt'
# SINGLE='-single'
# MULTI='-multi'
BUGHUNT="-bughunt" # default value
FIND="-find"
### Requirements:
###   1. Before running, please make sure Docker is installed: (https://docs.docker.com/get-docker/)
###   2. Apple Chop Macs might need to use: --platform linux/amd64

### DEMOS, see README for detailed explination

### [demo 1: run bakery algorithm with symmetry property]
# ./hyperqb.sh demo/bakery.smv demo/bakery.smv demo/symmetry.hq 1 -pes
### [dem0 2: run SNARK algorithm with linearizability propoerty]
# ./hyperqb.sh demo/SNARK_conc.smv demo/SNARK_seq.smv demo/lin.hq 18 -pes
### [demo 3: run simple_krip with simple_krip_formula]
# ./hyperqb.sh demo/mini.smv demo/mini.smv demo/mini.hq 3 -pes
# ./hyperqb.sh demo/mini.smv demo/mini.smv demo/mini.smv demo/mini.smv  demo/mini_try.hq 10 -pes -find
# ./hyperqb.sh demo/mini.smv demo/mini.smv demo/mini.smv demo/mini.hq 3 -pes

# ./hyperqb.sh demo/mini.smv demo/mini.smv demo/mini.smv demo/mini.hq 3 -pes


${HyperQB} cases_bmc/tacas_snark/snark1_M1_concurrent.smv cases_bmc/tacas_snark/snark1_M2_sequential.smv  cases_bmc/tacas_snark/snark1_formula.hq 1 ${PES} ${FIND}
# ${HyperQB} cases_bmc/tacas_snark/snark2_new_M1_concurrent.smv  cases_bmc/tacas_snark/snark2_new_M2_sequential.smv cases_bmc/tacas_snark/snark2_formula.hq 30 ${PES} ${FIND}
