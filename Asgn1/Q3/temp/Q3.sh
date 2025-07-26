#!/bin/bash
src_dir=$1
dst_dir=$2
attributes=($@)
attributes=("${attributes[@]:2}")
mkdir -p $dst_dir
for file in $src_dir/*.jsonl;do
    file_name=`basename $file`
    file_name=${file_name%.jsonl}
    dst_file=$dst_dir/$file_name.csv
    echo $(IFS=,; echo "${attributes[*]}") >$dst_file
    attributes_string=$(printf ".%s," "${attributes[@]}")
    attributes_string=("${attributes_string::-1}")
    # attributes_string=$(IFS='.,'; echo "${attributes[*]}")
    # echo $attributes_string
    # jq '.id_str,.retweet_count' $file
    jq -r "[$attributes_string] | @csv" $file >>$dst_file

done
