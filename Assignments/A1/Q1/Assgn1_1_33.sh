gawk -F'\n' '{if(NF > 0){lcm=(NR<2)?$1:(lcm*$1)/gcd(lcm,$1)}}function gcd(a,b){return(b)?gcd(b,a%b):a}END{print lcm}' <(rev "$1")|bc
