i=0
gcc -g -pthread main.c -o main
while [ $i -lt $1 ]
do
	./main
	i=$((i+1))
	echo '%%%%%%%%%%%%%%%%%%%%%%%%%%%' 
	echo "test $i code $?" 
	echo '%%%%%%%%%%%%%%%%%%%%%%%%%%%'

done
