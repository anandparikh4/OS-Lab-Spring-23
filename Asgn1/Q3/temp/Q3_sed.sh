#!/bin/bash
mkdir -p "$2"
for file in "$1"/*.jsonl;do
  file_name=`basename $file`
  file_name="${file_name%.jsonl}.csv"
  echo $(IFS=,; echo "${*:3}")>"$2/$file_name"
  attributes_string=$(printf "[\"%s\"]," "${@:3}")
  attributes_string="${attributes_string::-1}"
  sed -n -e "s/\(.*\)\($attributes_string\)\(.*\)/\2/p" "$file"| tr -d '[],' >> "$2/$file_name"
done