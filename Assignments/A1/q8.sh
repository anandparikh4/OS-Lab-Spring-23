#!/bin/bash

if [ ! -f "main.csv" ]; then
    echo "DATE,CATEGORY,AMOUNT,NAME" > main.csv
fi

declare -a arr
i=1
echo $@
for arg in $@;do
	arr["$i"]=$arg
	i=$(($i+1))
done

i=1
c_flag=0
c_arg=0
n_flag=0
n_arg=0
s_flag=0
s_arg=0
h_flag=0

default_flag=0
date=0
category=0
amount=0
name=0

while [ $i -le $# ];do
	if [ ${arr["$i"]} = "-c" ]
	then
		c_flag=1
		c_arg=${arr[$(($i+1))]}
		i=$(($i+2))
	elif [ ${arr["$i"]} = "-n" ]
	then
		n_flag=1
		n_arg=${arr[$(($i+1))]}
		i=$(($i+2))
	elif [ ${arr["$i"]} = "-s" ]
	then
		s_flag=1
		s_arg=${arr[$(($i+1))]}
		i=$(($i+2))
	elif [ ${arr["$i"]} = "-h" ]
	then
		h_flag=1
		i=$(($i+1))
	else
		default_flag=1
		date=${arr[$(($i))]}
		category=${arr[$(($i+1))]}
		amount=${arr[$(($i+2))]}
		name=${arr[$(($i+3))]}
		i=$(($i+4))
	fi
done

if (( default_flag==1 ));then
	echo "Ok"
	echo "$date","$category","$amount","$name" >> main.csv
fi

if (( h_flag==1 )); then
	echo "Name : CSV Handler"
fi

gawk -v c_flag="$c_flag" -v n_flag="$n_flag" -v c_arg="$c_arg" -v n_arg="$n_arg" '
BEGIN{
	FS = ","
	c_amount = 0
	n_amount = 0
	l = 0
}
NR <=1 {next}
{
	if(c_flag == 1 && $2 == c_arg) c_amount = int(c_amount) + int($3)
	if(n_flag == 1 && $4 == n_arg) n_amount = int(n_amount) + int($3) 
}
END{
	if(c_flag == 1) printf("Amount spent on %s = %s\n",c_arg,c_amount)
	if(n_flag == 1) printf("Amount spent by %s = %s\n",n_arg,n_amount)
}
' "main.csv"

if (( s_flag==1 )); then
	if (( "$s_arg"=="CATEGORY" )); then
		sort -k2 -t, "main.csv" -o "main.csv"
	elif (( "$s_arg"=="AMOUNT" )); then
		sort -k3 -n -t, "main.csv" -o "main.csv"
	elif (( "$s_arg"=="NAME" )); then
		sort -k4 -t, "main.csv" -o "main.csv"
	fi
fi

# sort by date
# implement help option 