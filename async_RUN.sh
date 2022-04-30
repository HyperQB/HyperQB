#!/bin/sh
#coding:utf-8
HYPERQUBE=./hyperqube.sh
GENMODEL=./async_genmodel.sh
SOLVEQBF=./async_solveqbf.sh
PARSEOUTPUT=./async_parseoutputs.sh
DUMMYPROP=cases_async/dummy_prop.hq
PARSETRAJ=async_traj_parser.py
BUILD_AAE=cases_async/buildtraj_AAE.py # AAE: G((out_{pi, tau'} = out_{pi', tau'}))
BUILD_AAE2=cases_async/buildtraj_AAE2.py # AAE2: ((in_{pi, tau} = in_{pi', tau})) ==> G((out_{pi, tau} = out_{pi', tau}))
BUILD_AAAE=cases_async/buildtraj_AAAE.py # AAAE

TIMEFORMAT='>>>>>>>>>>>>>>> Total: %R sec <<<<<<<<<<<<<<<'

# rm time.txt

### generate model, build trajectories, then solve QBF

#####################
#    ACBD example   #
#####################
### uncomment to run ACDB original examle
# M=6
# TRAJ=12
# MODEL1=cases_async/acdb/original/acdb_composed.smv
# MODEL2=cases_async/acdb/original/acdb_composed.smv
# BUILDTRAJ=${BUILD_AAE}
# time ${GENMODEL} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi
# time python3 ${BUILDTRAJ} ${M} ${M} ${TRAJ}
# time ${SOLVEQBF}

### uncomment to run ACDB examle with non-determinsm
# M=8
# TRAJ=16
# MODEL1=cases_async/acdb/with_ndet/acdb_composed.smv
# MODEL2=cases_async/acdb/with_ndet/acdb_composed.smv
# BUILDTRAJ=${BUILD_AAAE}
# time ${GENMODEL} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi
# time python3 ${BUILDTRAJ} ${M} ${M} ${TRAJ}
# time ${SOLVEQBF}
# python3 ${PARSETRAJ}


########################
#   concurrent leaks   #
########################
### uncomment to run conc_leak original
# M=11
# TRAJ=22
# MODEL1=cases_async/concleaks/original/conc_leaks_2procs.smv
# MODEL2=cases_async/concleaks/original/conc_leaks_2procs.smv
# BUILDTRAJ=${BUILD_AAE}
# time ${GENMODEL} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi
# time python3 ${BUILDTRAJ} ${M} ${M} ${TRAJ}
# time ${SOLVEQBF}
# ${PARSEOUTPUT}
# Check program termination
# ${HYPERQUBE} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi

### uncomment to run conc_leak new
# M=18
# TRAJ=36
# MODEL1=cases_async/concleaks/with_ndet/conc_leaks_3procs.smv
# MODEL2=cases_async/concleaks/with_ndet/conc_leaks_3procs.smv
# BUILDTRAJ=${BUILD_AAAE}
# # time ${GENMODEL} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi
# # time python3 ${BUILDTRAJ} ${M} ${M} ${TRAJ}
# ${PARSEOUTPUT}
# python3 ${PARSETRAJ}

# ## Check program termination
# ${HYPERQUBE} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi


########################
#   optimiazation      #
########################

########################
#   Original: DBE      #
########################
### uncomment to run example original optimization _ DBE
# M=4
# TRAJ=8
# MODEL1=cases_async/optimization/original/dbe/DBE_source.smv
# MODEL2=cases_async/optimization/original/dbe/DBE_target.smv
# BUILDTRAJ=${BUILD_AAE2}
# time ${GENMODEL} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi;
# time python3 ${BUILDTRAJ} ${M} ${M} ${TRAJ};
# time ${SOLVEQBF}
# Check for termination
# ${HYPERQUBE} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi


########################
#   Original: LP      #
########################
### uncomment to run example original optimization _ LP
# M=22
# TRAJ=44
# MODEL1=cases_async/optimization/original/lp/LP_source.smv
# MODEL2=cases_async/optimization/original/lp/LP_target.smv
# BUILDTRAJ=${BUILD_AAE2}
# time ${GENMODEL} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi
# time python3 ${BUILDTRAJ} ${M} ${M} ${TRAJ}
# time ${SOLVEQBF}

# Check for termination
# ${HYPERQUBE} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi

########################
#   Original: EFLP      #
########################
### uncomment to run example original optimization _ LP
# M=32
# TRAJ=64
# MODEL1=cases_async/optimization/original/eflp/EFLP_source.smv
# MODEL2=cases_async/optimization/original/eflp/EFLP_target.smv
# BUILDTRAJ=${BUILD_AAE2}
# time ${GENMODEL} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi
# time python3 ${BUILDTRAJ} ${M} ${M} ${TRAJ}
# time ${SOLVEQBF}
# ${PARSEOUTPUT}
## Check for termination
# ${HYPERQUBE} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi


######################
#   with_ndet: DBE   #
######################
### OPTIMIZATION NEW
# ### uncomment to run DBE new correct
# M=13
# D1=13
# D2=13
# TRAJ=26
# MODEL1=cases_async/optimization/with_ndet/dbe/DBE_source_ndet.smv
# MODEL2=cases_async/optimization/with_ndet/dbe/DBE_target_ndet.smv
# BUILDTRAJ=${BUILD_AAAE}
# time ${GENMODEL} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi
# time python3 ${BUILDTRAJ} ${D1} ${D2} ${TRAJ}
# time ${SOLVEQBF}
# ## Check program termination
# # ${HYPERQUBE} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi

# # ### uncomment to run DBE new wrong
# M=13
# D1=13
# D2=13
# TRAJ=26
# MODEL1=cases_async/optimization/with_ndet/dbe/DBE_source_ndet.smv
# MODEL2=cases_async/optimization/with_ndet/dbe/DBE_target_wrong_ndet.smv
# BUILDTRAJ=${BUILD_AAAE}
# time ${GENMODEL} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi
# time python3 ${BUILDTRAJ} ${D1} ${D2} ${TRAJ}
# time ${SOLVEQBF}
# python3 ${PARSETRAJ}
## Check program termination
# ${HYPERQUBE} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi



######################
#   with_ndet: LP   #
######################
### OPTIMIZATION NEW
# ### uncomment to run LP new correct
# M=17
# D1=17
# D2=17
# TRAJ=34
# MODEL1=cases_async/optimization/with_ndet/lp/LP_source_ndet.smv
# MODEL2=cases_async/optimization/with_ndet/lp/LP_target_ndet.smv
# BUILDTRAJ=${BUILD_AAAE}
# time ${GENMODEL} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi
# time python3 ${BUILDTRAJ} ${D1} ${D2} ${TRAJ}
# time ${SOLVEQBF}
# ${PARSEOUTPUT}
# python3 ${PARSETRAJ}
# Check program termination
# ${HYPERQUBE} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi

# ### uncomment to run LP new incorrect
# M=17
# D1=17
# D2=17
# TRAJ=34
# MODEL1=cases_async/optimization/with_ndet/lp/LP_source_ndet.smv
# MODEL2=cases_async/optimization/with_ndet/lp/LP_target_wrong_ndet.smv
# BUILDTRAJ=${BUILD_AAAE}
# time ${GENMODEL} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi
# time python3 ${BUILDTRAJ} ${D1} ${D2} ${TRAJ}
# time ${SOLVEQBF}
# ${PARSEOUTPUT}
# python3 ${PARSETRAJ}
## Check program termination
# ${HYPERQUBE} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi


#######################
#   with_ndet: EFLP   #
#######################
### OPTIMIZATION NEW
# ### uncomment to run LP new correct
# M=22
# D1=18
# D2=22
# TRAJ=40
# MODEL1=cases_async/optimization/with_ndet/eflp/EFLP_source_ndet.smv
# MODEL2=cases_async/optimization/with_ndet/eflp/EFLP_target_ndet.smv
# BUILDTRAJ=${BUILD_AAAE}
# time ${GENMODEL} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi
# time python3 ${BUILDTRAJ} ${D1} ${D2} ${TRAJ}
# time ${SOLVEQBF}
# ${PARSEOUTPUT}
# python3 ${PARSETRAJ}
# Check program termination
# ${HYPERQUBE} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi




############################
#   with_ndet: LP-2loops   #
############################
### OPTIMIZATION NEW
# ### uncomment to run LP new correct with 2 loops
# M=35
# D1=33
# D2=35
# TRAJ=68
# MODEL1=cases_async/optimization/with_ndet_2loops/lp/LP_source_ndet.smv
# MODEL2=cases_async/optimization/with_ndet_2loops/lp/LP_target_ndet.smv
# BUILDTRAJ=${BUILD_AAAE}
# time ${GENMODEL} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi
# time python3 ${BUILDTRAJ} ${D1} ${D2} ${TRAJ}
# time ${SOLVEQBF}
# ${PARSEOUTPUT}
# python3 ${PARSETRAJ}
# Check program termination
# ${HYPERQUBE} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi



##############################
#   with_ndet: EFLP-2loops   #
##############################
### OPTIMIZATION NEW
# ### uncomment to run EFLP new correct with 2 loops
# M=54
# D1=37
# D2=54
# TRAJ=91
# MODEL1=cases_async/optimization/with_ndet_2loops/eflp/EFLP_source_ndet.smv
# MODEL2=cases_async/optimization/with_ndet_2loops/eflp/EFLP_target_ndet.smv
# BUILDTRAJ=${BUILD_AAAE}
# time ${GENMODEL} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi
# time python3 ${BUILDTRAJ} ${D1} ${D2} ${TRAJ}
# time ${SOLVEQBF}
# ${PARSEOUTPUT}
# python3 ${PARSETRAJ}
# Check program termination
# ${HYPERQUBE} ${MODEL1} ${MODEL2} ${DUMMYPROP} ${M} hpes -find -multi
