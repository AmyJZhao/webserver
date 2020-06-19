#!/bin/bash

actual_response="snake_test_actual_output"
expected_response="/usr/src/projects/aaa-p2/tests/integration/files/snake.html"

# remove intermediate files and set empty output file
rm -f $actual_response
> $actual_response

# start up server
$1 $2 &
# ./build/bin/server ./tests/integration/snake_config &

SERVER_PID=$!
sleep 0.5

# make request
curl localhost:8020/snake > $actual_response

# check results
kill $SERVER_PID
diff --strip-trailing-cr $actual_response $expected_response
if [ $? -eq 0 ]
then
	echo "Passed - Snake Integration Test"
	rm -f $actual_response
	exit 0
else
	echo "FAILED - Snake Integration Test"
	rm -f $actual_response
	exit 1
fi
