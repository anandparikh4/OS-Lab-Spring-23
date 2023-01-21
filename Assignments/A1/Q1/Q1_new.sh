gawk -F'\n' '{if(NF > 0){{lcm=(NR==1)?$1:(lcm*$1)/gcd(lcm,$1);}}}function gcd(a,b){return(b==0)?a:gcd(b,a%b);}END{print(lcm);}' <(rev "$1")|bc
