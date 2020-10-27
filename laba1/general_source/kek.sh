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
