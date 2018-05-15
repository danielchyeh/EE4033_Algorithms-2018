#!/bin/bash

#############################################################################
# File       [ checkSubmitpa2.sh ]
# Author     [ funnymean ]
# Modified   [ 2014/03/18 ]
# Synopsis   [ simple test for algorithm pa2 homework format program ]
# Date       [ Ver. 1.0 started 2013/03/18 ]
#############################################################################


if [ "$1" == "" ]; then
    echo "Usage:   ./checkSubmitpa2.sh <fileName>"
    echo "Example: ./checkSubmitpa2.sh b90901000_pa2.tgz"
    echo "This program will build a dir name checkSubmitpa2_$$ env"
    exit
fi

binaryFileName='WorkerAnt'
fileName=$1

echo "Target file $fileName"
if [ ! -f $fileName ]; then
    echo "Cannot find tgz file $filename!"
    exit
fi
if [ -d check_$$ ]; then
    echo "Directory check_$$ exists"
    echo "Please delete the directory and try again"
    exit
fi

mkdir check_$$
cd check_$$


tar zxvf ../$fileName

stuid=`basename $fileName | sed -e 's/_.*//'`
echo "$fileName"
echo "$stuid"

if [ ! -d ${stuid}_pa2 ]; then
    echo "**ERROR Cannot find directory <student id>_pa2"
    cd ..
    rm -rf check_$$
    exit
fi

cd ${stuid}_pa2
echo "checking README ..."
if [ ! -f README ]; then
    echo "**ERROR Cannot find README"
    cd ../../
    rm -rf check_$$
    exit
fi

echo "checking src/ ..."
if [ ! -d src ]; then
    echo "Cannot find src dir"
    cd ../../
    rm -rf check_$$
    exit
fi

echo "checking doc/ ..."
if [ ! -d doc ]; then
    echo "Cannot find doc dir"
    cd ../../
    rm -rf check_$$
    exit
fi

echo "checking report ..."
report_exist=`ls doc | grep ${stuid}_pa2_report`
reportPDF_exist=`ls doc | grep ${stuid}_pa2_report.pdf`
reportDOC_exist=`ls doc | grep ${stuid}_pa2_report.doc`
if [ "${report_exist}" = "" ] && [ "${reportPDF_exist}" = "" ] && [ "${reportDOC_exist}" = "" ]; then
    echo "cannot find doc/${stuid}_pa2_report"
    cd ../../
    rm -rf check_$$
    exit
fi

rm -f bin/$binaryFileName
make clean
make

if [ ! -f bin/$binaryFileName ]; then
    echo "cannot find bin/$binaryFileName"
    cd ../../
    rm -rf check_$$
    exit
fi

echo "Congratulations!  Passed submission checking!"
cd ../../
rm -rf check_$$

