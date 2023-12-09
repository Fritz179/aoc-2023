#!/bin/sh

set -e

for i in $(seq -w 1 25)
do 
    path="./day-$i/c"

    if [ -d $path ]
    then
        echo "building day $i"
        cd $path
        ./build.sh
        cd ../..
    else
        echo "skipping day $i"
    fi
done

echo
echo "Commpilation succesfull"
echo "Running tests"

for i in $(seq -w 1 25)
do 
    path="./day-$i/c"

    if [ -d $path ]
    then
        echo
        echo "Running day $i $1"
        cd $path
        ./main $1
        cd ../..
    fi
done