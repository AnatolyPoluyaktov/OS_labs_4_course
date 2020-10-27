#!/bin/sh
echo "$0"
counti=0
sighandler(){
	count=$((count+1))
	if [ $((count % 2 )) != 0 ]
	then
		file *|grep text|cut -d: -f1

	fi
}
trap "sighandler" 2
path=/home/black_andromeda/LABA2OS/input.txt

wc -w < $path|tee output.txt

while true
do
	echo 'Enter "exit" to stop or Ctrl+C to show the list of non-binary files in current dir'
	read command
	if [ "$command" = "exit" ]
       	then
		exit 0
	fi
done
