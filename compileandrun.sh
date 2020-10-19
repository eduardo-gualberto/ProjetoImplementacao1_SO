mkdir implementacao{1,2,3}/build 2> /dev/null

make all 2> /dev/null

if [ "$#" == 0 ]
then
    echo "Expected at least 1 parameter, telling wich implementation."
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
        N_SEATS=$arg 

    elif [ "$counter" == 3 ]
    then
        N_THREADS=$arg 
    fi
    counter=$((counter+1))
done

if [ $IMPLEMENTACAO == 1 ]
then
    if [ $# > 1 ]
    then
        cd implementacao1/build/
        ./main $N_SEATS $N_THREADS
    else
        cd implementacao1/build/
        ./main
    fi
elif [ $IMPLEMENTACAO == 2 ]
then
    if [ $# > 1 ]
    then
        cd implementacao2/build/
        ./main $N_SEATS $N_THREADS
    else
        cd implementacao2/build/
        ./main
    fi
elif [ $IMPLEMENTACAO == 3 ]
then
    if [ $# > 1 ]
    then
        cd implementacao3/build/
        ./main $N_SEATS $N_THREADS
    else
        cd implementacao3/build/
        ./main
    fi
elif [ $IMPLEMENTACAO == 4 ]
then
    if [ $# > 1 ]
    then
        cd implementacao4/build/
        ./main $N_SEATS $N_THREADS
    else
        cd implementacao3/build/
        ./main
    fi
else
    echo "Invalid implementation number. Should be either 1, 2, 3 or 4."
    exit
fi

cd ../../; rm 1 2> /dev/null