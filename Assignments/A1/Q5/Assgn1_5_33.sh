#!/bin/bash
files=`find "$1" -type f -name "*.py"`
arr=($files)
for f in ${arr[*]};do
	echo
	echo $f
	gawk '
			BEGIN{flag=0;prev=-1;}
			{	
				if(flag==0){
					if($0 ~ /^([ \t]*([\x27]{3}))/){
						flag=1
						prev=1
						print "Line " NR ":" $0
						if($0 ~ /(^([ \t]*([\x27]{3}))).*([\x27]{3})[ \t]*$/){
							flag=0
						}
					}
					else if($0 ~ /^([ \t]*([\x22]{3}))/){
						flag=1
						prev=2
						print "Line " NR ":" $0
						if($0 ~ /(^([ \t]*([\x22]{3}))).*([\x22]{3})[ \t]*$/){
							flag=0
						}
					}
					else if($0 ~ /^([ \t]*[#])/){
						print "Line " NR ":" $0
					}
					else if($0 ~ /([ \t]*#.*)$/){
						if( ! ( ($0 ~ /([^#\x22]*[\x22][^#\x22]*[#]+[^\x22]*[\x22][^#]*)$/)  || ($0 ~ /([^#\x27]*[\x27][^#\x27]*[#]+[^\x27]*[\x27][^#]*)$/) ) ){
							split($0,arr,"")
							q=0
							idx=-1
							for(i=1;i<=length(arr);i++){
								if(arr[i] ~ /[\x27]/){
									if(q==0)q=1
									else if(q==1)q=0
								}
								else if(arr[i] ~ /[\x22]/){
									if(q==0)q=2
									else if(q==2)q=0
								}
								else if(arr[i] ~ /[#]/){
									if(q==0){
										idx=i
										break
									}
								}
							}
							printf("Line %d:",NR)
							for(i=idx;i<=length(arr);i++){
								printf("%s",arr[i]);
							} 
							printf("\n")
						}
					}
				}
				else if(flag==1){
					print $0
					if(prev==1 && $0 ~ /.*([\x27]{3})[ \t]*$/)flag=0
					else if(prev==2 && $0 ~ /.*([\x22]{3})[ \t]*$/)flag=0
				}
			}
	' $f
done
