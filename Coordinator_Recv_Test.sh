#!/bin/bash
COORDINATOR_IP=127.0.0.1
COORDINATOR_PORT=8001
NC_TIMEOUT=3
ERROR_RETRY_TIMES=10

set_result=""
function send_set_command
{
	key_len=$1
	key=$2
	value_len="$3"
	value=$4

	printf -v set_command "*3\r\n\$3\r\nSET\r\n\$${key_len}\r\n${key}\r\n\$${value_len}\r\n${value}\r\n"

	for (( i=0; i<$ERROR_RETRY_TIMES; i++ ))
	do
		retval_set=`printf "$set_command" | nc -w ${NC_TIMEOUT} ${COORDINATOR_IP} ${COORDINATOR_PORT}`

	    if [[ $retval_set =~ $standard_error ]]
	    then
	    	sleep 0.5
	    	continue
	    else
	    	break
	    fi
	done

	printf -v set_result "%s" "${retval_set}"
}

send_set_command 9 item2_key 11 item2_value
