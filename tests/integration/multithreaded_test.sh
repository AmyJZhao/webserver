#!/bin/bash
echo $(pwd)

actual_response="multithreaded_test_actual_output"
expected_response="/usr/src/projects/aaa-p2/tests/integration/multithreaded_test_expected_output"

# remove intermediate files and set empty output file
rm -f $actual_response
> $actual_response

# start up the server
$1 $2 &
SERVER_PID=$!
sleep 0.5

# start a slow request with nc
nc -lv -p 8010 &
NC_PID=$!
printf "GET /slow HTTP/1.1\r\n\r\n" | nc localhost 8010 &
SLOW_PID=$!

# start a fast request with curl
curl -s -S localhost:8000/echo > $actual_response
diff $actual_response $expected_response
if [ $? -ne 0 ]
then
  kill $NC_PID
  kill $SLOW_PID
  kill $SERVER_PID
  echo "FAILED - Multithreaded Integration Test - completed echo request was different from expected"
  exit 1
fi

sleep 0.5 # unhang the nc listener

kill $NC_PID
kill $SLOW_PID
kill $SERVER_PID
rm -f $actual_response
echo "Passed - Multithreaded Integration Test"
exit 0
