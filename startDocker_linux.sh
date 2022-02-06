#!/bin/bash
docker build -t bruteforcecracker .
docker run -it --rm -v $PWD:/home/host/dev bruteforcecracker