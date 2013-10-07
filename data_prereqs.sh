#!/bin/bash

if [ -n "$(command -v yum)" ]; then
sudo yum -y install nodejs
sudo yum -y install redis-server
sudo yum -y install gnumeric
fi

if [ -n "$(command -v apt-get)" ]; then
sudo add-apt-repository -y ppa:chris-lea/node.js
sudo apt-get update
sudo apt-get -y install nodejs
sudo apt-get -y install redis-server
sudo apt-get -y install gnumeric
fi

npm install ethercalc
echo 'redis-server\nethercalc --expire 86400' > ethercalc
sudo mv ethercalc /etc/init.d/ethercalc
chmod +x /etc/init.d/ethercalc
update-rc.d ethercalc defaults

sudo pip install csvkit

redis-server
ethercalc
