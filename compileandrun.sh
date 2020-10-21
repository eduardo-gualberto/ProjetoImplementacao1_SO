mkdir implementacao{1,2,3}/build > /dev/null 2>&1

make all > /dev/null 2>&1

if [ "$#" == 0 ]
then
    echo "Expected at least 1 parameter to select wich implementation."
    exit
fi

counter=1
for arg in $@
do
    if [ "$counter" == 1 ]
    then
        IMPLEMENTACAO=$arg 
    elif [ "$counter" == 2 ]
    then
        N_THREADS=$arg 
    elif [ "$counter" == 3 ]
    then
        N_SEATS=$arg 
    fi
    counter=$((counter+1))
done

if [ $IMPLEMENTACAO == 1 ]
then
    if [ $# > 1 ]
    then
        cd implementacao1/build/
        ./main $N_THREADS $N_SEATS
    else
        cd implementacao1/build/
        ./main
    fi
elif [ $IMPLEMENTACAO == 2 ]
then
    if [ $# > 1 ]
    then
        cd implementacao2/build/
        ./main $N_THREADS $N_SEATS
    else
        cd implementacao2/build/
        ./main
    fi
elif [ $IMPLEMENTACAO == 3 ]
then
    if [ $# > 1 ]
    then
        cd implementacao3/build/
        ./main $N_THREADS $N_SEATS
    else
        cd implementacao3/build/
        ./main
    fi
elif [ $IMPLEMENTACAO == 4 ]
then
    if [ $# > 1 ]
    then
        cd implementacao4/build/
        ./main $N_THREADS $N_SEATS
    else
        cd implementacao3/build/
        ./main
    fi
else
    echo "Invalid implementation number. Should be either 1, 2, 3 or 4."
    exit
fi

cd ../../; rm 1 2> /dev/null