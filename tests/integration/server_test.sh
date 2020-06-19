#!/bin/bash

BAD_RESPONSE='HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nBad Request'
GOOD_RESPONSE='GET / HTTP/1.1\r\nHost: localhost:8000\r\nUser-Agent: curl/7.58.0\r\nAccept: */*\r\n\r\n'

# Start webserver in the background
$1 $2 &
SERVER_PID=$!

# Good response test
curl --max-time 3 http://localhost:8000 > test_response1
if [ '`grep -s -F $GOOD_RESPONSE test_response1`x' == 'x' ];
then
    echo "==> Good response test: FAILED"
    exit 1
else
    echo "==> Good response test: SUCCESS"
fi

# Stop the webserver
kill $SERVER_PID


# Test for bad call to server
$1
EXIT_VAL=$?

if [ $EXIT_VAL == 1 ];
then
    echo "==> Bad call to server test: SUCCESS"
    exit 0
else
    echo "==> Bad call to server test: FAILED"
    exit 1
fi
