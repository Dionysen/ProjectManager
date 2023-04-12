#!/bin/bash
cd
rm -rf ./projectmanager
git clone git@gitee.com:sential/projectmanager.git
cd projectmanager
rm -rf build/*
cd build
cmake ..
make
sudo cp ./pm /usr/bin/pm