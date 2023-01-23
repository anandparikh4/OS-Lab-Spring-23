rev $1>"b.txt"
echo $(gawk '{if(NR==1)lcm=$1;else{lcm=(lcm*$1)/gcd(lcm,$1);}}function gcd(a,b){return(b==0)?a:gcd(b,a%b);}END{print(lcm);}' "b.txt")|bc
