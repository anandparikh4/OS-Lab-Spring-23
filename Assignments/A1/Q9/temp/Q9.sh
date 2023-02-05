awk '{print $2}' $1 | sort | uniq -c | awk '{print $2" "$1}' | sort -k3 -k2nr
awk '{print $1}' $1 | sort | uniq -d
awk '{print $1}' $1 | sort | uniq -u|wc -l