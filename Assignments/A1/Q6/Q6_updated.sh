n=1000000
primes=($(seq 0 $n))
for((i=2;i<=n;i+=2));do
    primes[i]=2
done
for((i=3;((i*i))<=n;i+=2));do
    if((${primes[i]}==$i));then
        for((j=((i*i));j<=n;j+=i));do
            if((${primes[j]}>$i));then
                primes[j]=$i
            fi
        done
    fi
done
while read line;do
    while ((line>1));do
        echo -n ${primes[line]} "";
        line=$(($line/${primes[line]}));
    done
    echo ""
done <"input.txt" >"output5.txt"