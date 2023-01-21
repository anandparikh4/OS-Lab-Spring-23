#!/bin/bash
mkdir -p "$2"
for file in "$1"/*.jsonl;do
  filename=`basename $file`
  filename="${filename%.jsonl}.csv"
  echo $(IFS=,; echo "${*:3}")>"$2/$filename"
  attributes=$(printf ".%s," "${@:3}")
  attributes="${attributes::-1}"
  jq -r "[$attributes] | @csv" "$file">>"$2/$filename"
done