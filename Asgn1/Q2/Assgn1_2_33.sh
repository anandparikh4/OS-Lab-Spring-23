while read line
do
    if [[ ${#line} -ge 5 && ${#line} -le 20 && $line =~ ^[a-zA-Z][a-zA-Z0-9]*[0-9][a-zA-Z0-9]*$ ]]
    then echo $line|grep -iqf fruits.txt &&echo "NO"|| echo "YES"
    else echo "NO"
    fi
done <$1 >"validation_results.txt"