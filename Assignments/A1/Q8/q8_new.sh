#!/bin/bash

# Check if main.csv file exists, if not create it
if [ ! -f main.csv ]; then
    echo "Date,Category,Amount,Name" > main.csv
fi

# Initialize variables for category and name flags
category=''
name=''
sort_column='Date'

# Parse command line flags
while getopts ':c:n:s:' flag; do
  case "${flag}" in
    c) category="${OPTARG}" ;;
    n) name="${OPTARG}" ;;
    s) sort_column="${OPTARG}" ;;
  esac
done

# Insert new record into main.csv
echo "${@: -4:1},${@: -3:1},${@: -2:1},${@: -1}" >> main.csv

# echo "Category: " $category
# echo "Name: " $name
# echo "Sort Column: " $sort_column

# Print total amount spent on category if flag is set
if [ -n "$category" ]; then
  echo "Total amount spent on $category: $(awk -F, '$2 == "'"$category"'" { total += $3 } END { print total }' main.csv)"
fi

# Print total amount spent by name if flag is set
if [ -n "$name" ]; then
  echo "Total amount spent by $name: $(awk -F, '$4 == "'"$name"'" { total += $3 } END { print total }' main.csv)"
fi

# Sort main.csv by column specified by -s flag
if [ "$sort_column" = "Date" ]; then
  tail -n +2 main.csv | sort -n -t, -k1.7 -k1.8 -k1.4 -k1.5 -k1
else 
  tail -n +2 main.csv | sort -t, -k$(awk -F, '{ for (i = 1; i <= NF; i++) if ($i == "'"$sort_column"'") { if(i==3){print i," -n";} else{print i;} break; } }' main.csv)
fi
# sort -t, -k$(awk -F, '{ for (i = 1; i <= NF; i++) if ($i == "'"$sort_column"'") { if(i==3){print i," -n";} else{print i;} break; } }' main.csv) main.csv
# sort -n -t, -k1.7 -k1.8 -k1.4 -k1.5 -k1 main.csv