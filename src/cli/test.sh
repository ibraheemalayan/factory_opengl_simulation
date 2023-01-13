i=0
gcc -o main main.c
while [ $i -lt $1 ]
do
	./main
	i=$((i+1))
	echo '%%%%%%%%%%%%%%%%%%%%%%%%%%%' 
	echo "test $i code $?" 
	echo '%%%%%%%%%%%%%%%%%%%%%%%%%%%'

done
