#!/bin/bash -f

# A helper script to run an application across all float and posit configurations
# To be run from the individual application directory
# Cmd line: ./run_all.sh <app> <size> <level>
app=$1
sz=$2
level=$3

mkdir -p logs/itrace.${sz}
echo "--------"
echo "****** RUNNING WITH VERBOSITY = 0 (WARM) (FMADD) ******"
../tools/batch-compile --sim --compile --level ${level} --vszs ${sz} --main ${app} --warm-caches --use-fmadd

echo ""
echo "--------"
echo "****** RUNNING WITH VERBOSITY = 1 (WARM) (FMADD) ******"
../tools/batch-compile --sim --compile --level ${level} --vszs ${sz} --main ${app} --warm-caches --use-fmadd --verbosity +v1

# for t in f32 f64
for t in f32 f64
do
   mv logs/$t.${sz}.log logs/itrace.${sz}/$t.fmadd.${sz}.log 
   mv $t.${sz}.${app}.dump $t.${sz}.${app}-fmadd.dump
done

for f in f32-p8 f32-p16 f32-p24 f32-p32 p8 p16 p24 p32
do
   mv logs/${f}.${sz}.log logs/itrace.${sz}/${f}.${sz}.log
done

echo ""
echo "--------"
echo "****** RUNNING WITH VERBOSITY = 0 (WARM) ******"
#../batch-compile --sim --compile --vszs ${sz} --main ${app} --warm-caches --data-types FLOAT --data-types DOUBLE
../tools/batch-compile --sim --compile --level ${level} --vszs ${sz} --main ${app} --warm-caches --data-types FLOAT

echo ""
echo "--------"
echo "****** RUNNING WITH VERBOSITY = 1 (WARM) ******"
#../tools/batch-compile --sim --compile --vszs ${sz} --main ${app} --warm-caches --data-types FLOAT --data-types DOUBLE --verbosity +v1
../tools/batch-compile --sim --compile --level ${level} --vszs ${sz} --main ${app} --warm-caches --data-types FLOAT --verbosity +v1

# for t in f32 f64
for t in f32
do
   mv logs/$t.${sz}.log logs/itrace.${sz}/$t.${sz}.log 
done
