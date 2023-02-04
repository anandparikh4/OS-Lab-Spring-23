#!/bin/bash
[[ ! -f "main.csv" ]] && echo "DATE,CATEGORY,AMOUNT,NAME" > main.csv
arr=($*)
i=0
while [[ $i -lt $# ]];do
	if [[ ${arr["$i"]} == "-c" ]];then
		c_arg=${arr[$(($i+1))]}
		i=$(($i+2))
	elif [[ ${arr["$i"]} == "-n" ]];then
		n_arg=${arr[$(($i+1))]}
		i=$(($i+2))
	elif [[ ${arr["$i"]} == "-s" ]];then
		s_arg=${arr[$(($i+1))]}
		i=$(($i+2))
	elif [[ ${arr["$i"]} == "-h" ]];then
		h_flag=1
		i=$(($i+1))
	else
		date=${arr[$(($i))]}
		category=${arr[$(($i+1))]}
		amount=${arr[$(($i+2))]}
		name=${arr[$(($i+3))]}
		i=$(($i+4))
	fi
done
echo $n_arg
c_amt=0
n_amt=0
[[ ! -z ${date+x} ]] && echo "$date,$category,$amount,$name" >> main.csv
sed 1d main.csv | ( while IFS=, read col1 col2 col3 col4;do 
	[[ ! -z ${n_arg+x} ]] && [[ "$col4" == "$n_arg" ]] && n_amt=$(($n_amt+$col3))
	[[ ! -z ${c_arg+x} ]] && [[ "$col2" == "$c_arg" ]] && c_amt=$(($c_amt+$col3))	
done
[[ ! -z ${c_arg+x} ]] && echo "Amount spent in category: $c_arg is $c_amt"
[[ ! -z ${n_arg+x} ]] && echo "Amount spent by name: $n_arg is $n_amt"
)
case $s_arg in
	CATEGORY) tail -n+2 "main.csv" | sort -k2 -t, -o main.csv
			  sed -i '1 i DATE,CATEGORY,AMOUNT,NAME' main.csv;;
	AMOUNT) tail -n+2 "main.csv" | sort -k3 -n -t, -o main.csv
			sed -i '1 i DATE,CATEGORY,AMOUNT,NAME' main.csv;;
	NAME) tail -n+2 "main.csv" | sort -k4 -t, -o main.csv
		  sed -i '1 i DATE,CATEGORY,AMOUNT,NAME' main.csv;;
esac

#[[ ! -z ${s_arg+x} ]] && sed -i '1 i DATE,CATEGORY,AMOUNT,NAME' main.csv