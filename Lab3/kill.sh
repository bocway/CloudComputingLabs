#!/bin/sh
sudo killall -9 kvstore2pcsystem
sudo killall -9 client
sudo lsof -i:8000
sudo lsof -i:8001
find ./log -name "*.log" | xargs sudo rm
