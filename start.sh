rm -rf tinyServer/
cp -r /mnt/c/Users/QiangFangChen/Desktop/tinyServer/ /home/tinyServer
cd tinyServer/build
cmake ..
make
./tinyServer