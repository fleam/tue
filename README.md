git clone https://github.com/DaveGamble/cJSON.git
cd cJSON
mkdir build
cd build
cmake ..
make
make install

git clone https://github.com/cesanta/mongoose.git

sudo apt-get update
sudo apt-get install libmysqlclient-dev

ip route show | grep -i default | awk '{ print $3 }'

todo: support debug