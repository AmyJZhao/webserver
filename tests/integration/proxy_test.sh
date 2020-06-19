#! /bin/bash

SERVER_PATH=$1
BASE_CONFIG=$2
PROXY_CONFIG=$3

$SERVER_PATH $BASE_CONFIG &
SERVER_PID=$!

$SERVER_PATH $PROXY_CONFIG &
PROXY_PID=$!

ERROR=0

curl --max-time 3 localhost:8080/proxy/static1/sample.txt -o proxy.out
curl --max-time 3 localhost:8000/static1/sample.txt -o expected.out
diff proxy.out expected.out
if [ "$?" -eq "0" ]; then
    echo "==> Proxy response test: PASSED"
else
    echo "==> Proxy response test: FAILED"
    ERROR=$((ERROR + 1))
fi

kill $SERVER_PID
kill $PROXY_PID

if [ $ERROR -eq 0 ]
then 
	echo "All tests finished and passed."
	exit 0
else 
	echo "All tests finished with some failed."
	exit $ERROR
fi
