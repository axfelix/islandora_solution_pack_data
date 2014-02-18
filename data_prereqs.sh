#!/bin/bash

if [ -n "$(command -v yum)" ]; then
sudo yum -y install nodejs
sudo yum -y install redis
sudo yum -y install gnumeric
sudo yum -y install python-pip
sudo yum -y install npm
fi

if [ -n "$(command -v apt-get)" ]; then
sudo add-apt-repository -y ppa:chris-lea/node.js
sudo apt-get update
sudo apt-get -y install nodejs
sudo apt-get -y install redis-server
sudo apt-get -y install gnumeric
fi

sudo npm install -g ethercalc
echo -e 'redis-server\nethercalc --expire 86400' > etherstart
sudo mv etherstart /etc/init.d/etherstart
chmod +x /etc/init.d/etherstart

if [ -n "$(command -v apt-get)" ]; then
sudo update-rc.d ethercalc defaults
fi

sudo pip install csvkit

nohup redis-server&
nohup ethercalc --expire 86400&
