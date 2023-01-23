rev $1 > "b.txt" 
echo $(gawk -F'\n' '{if(NF > 0){if(NR==1)lcm=$1;else{if($1!=0)lcm=(lcm*$1)/gcd(lcm,$1);}}}function gcd(a,b){return(b==0)?a:gcd(b,a%b);}END{print(lcm);}' "c.txt") | bc
