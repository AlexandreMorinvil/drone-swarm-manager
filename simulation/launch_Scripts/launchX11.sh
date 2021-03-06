#!/bin/bash
cd ~/.ssh
docker build -f $1 . --tag simulation-argos --network host
x11docker --hostdisplay --hostnet --user=RETAIN -- --privileged simulation-argos