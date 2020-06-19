#!/bin/bash
bad_response='HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nBad Request'
GOOD_RESPONSE='GET / HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.58.0\r\nAccept: */*\r\n\r\n'

SERVER_PATH="./bin/server"
RESPONSE_PATH=../tests

echo "
http {
    server {
        listen 8080;
    }
}" > TEMP_config

# Start webserver in the background
# ../../build/bin/server ./test_config &
$SERVER_PATH TEMP_config &
pid_server=$!
echo $pid_server

# Good response test
curl --max-time 3 http://localhost:8080 > test_response1
if [ `grep -s -F $GOOD_RESPONSE test_response1`x == 'x' ];
then
    echo “FAILED”
else
    echo “SUCCESS good response”
fi

# Test 1 should success:
echo "finished 1"

# Stop the webserver
kill -9 $pid_server
exit 0