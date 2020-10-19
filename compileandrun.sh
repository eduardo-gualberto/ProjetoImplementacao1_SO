make all 2> /dev/null

if [ "$#" == 0 ]
then
    echo "Expected at least 1 parametes, telling wich implementation."
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
    cd implementacao1/build/
    ./main N_SEATS N_THREADS
elif [ $IMPLEMENTACAO == 2 ]
then
    cd implementacao2/build/
    ./main N_SEATS N_THREADS
elif [ $IMPLEMENTACAO == 3 ]
then
    cd implementacao3/build/
    ./main N_SEATS N_THREADS
else
    echo "Invalid implementation number. Should be either 1 or 2."
    exit
fi