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
[[ -z ${s_arg+x} ]] && s_arg="DATE"
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
flag=`cat main.csv | wc -l`
if(( flag > 1 ))
then
	case $s_arg in
		CATEGORY) tail -n+2 "main.csv" | sort -k2 -t, -o main.csv
				sed -i '1 i DATE,CATEGORY,AMOUNT,NAME' main.csv;;
		AMOUNT) tail -n+2 "main.csv" | sort -k3 -n -t, -o main.csv
				sed -i '1 i DATE,CATEGORY,AMOUNT,NAME' main.csv;;
		NAME) tail -n+2 "main.csv" | sort -k4 -t, -o main.csv
			  sed -i '1 i DATE,CATEGORY,AMOUNT,NAME' main.csv;;
		DATE) tail -n+2 "main.csv" | sort -t, -k1.7,1.10n -k1.4,1.5n -k1.1,1.2n -o main.csv
			  sed -i '1 i DATE,CATEGORY,AMOUNT,NAME' main.csv;;
	esac
fi
if (( h_flag > 0 ))
then
    printf "\nNAME\n    CSV_FILE_HANDLER\n"
    printf "\nSYNOPSIS\n\tbash Assgn1_8_33.sh [OPTIONS]... ARGS\n"
    printf "\nDESCRIPTION\n"
    printf "\nOPTIONS\n"
    printf "\n    Add information to main.csv, file created if does not already exist. Manipulate and observe information\n"
    printf "\n    -c\n\tprint amount of money spent in a given category"
    printf "\n    -n\n\tprint amount of money spent by person having a given name"
    printf "\n    -s\n\tsort the file by the given category"
    printf "\n    -h\n\thelp"
    printf "\nARGS"
    printf "\n    To add an event, enter 4 arguments (in the same order and format specified) : date (dd-mm-yyyy) category amount name"
    printf "\n\nAUTHORS\n\tWritten by Soni Aditya Bharatbhai, Anand Manojkumar Parikh, Pranav Mehrotra, Saransh Sharma\n\n"
fi
