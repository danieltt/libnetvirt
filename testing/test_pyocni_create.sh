#!/bin/bash

max=$1
for num in $(seq 1 1 $max)
do
curl -X POST -d@config.json/test$num.json -H 'content-type: application/json:occi' -v http://localhost:8090/clonenode/ > /dev/null &> /dev/null
#curl -X POST -d@config.json/test$num.json -H 'content-type: application/json:occi' -v http://localhost:8090/clonenode/ 
done
