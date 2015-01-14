# OmrHistogram
Reads stimulus file and recorded signal file.

According to those generates a histogram of speed differences and the ratio of samples when signal follows stimulus.

# Building

## eclipse
1. project contains *cmake_debug* target that should run cmake
1. make command: "make -C ${ProjDirPath}/debug VERBOSE=1"

## command line
1. create a debug folder to the root level
1. from root level
    * cmake -E chdir debug cmake -G "Eclipse CDT4 - Unix Makefiles" ../src -DCMAKE_BUILD_TYPE=Debug
1. run make in debug folder
    * make -C debug VERBOSE=1

# Running
1. binary is build in debug/OmrHistogram
1. Usage: ./OmrHistogram [options] stimulus signal
1. for help type: *OmrHistogram --help*
