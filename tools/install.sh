#!/bin/bash
cd
rm -rf ./ProjectManager
git clone https://github.com/Dionysen/ProjectManager.git
cd ProjectManager
rm -rf build/*
cd build
cmake ..
make
sudo cp ./pm /usr/bin/pm
