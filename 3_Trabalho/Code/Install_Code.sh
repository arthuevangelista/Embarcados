i2c-bcm2708
i2c-dev

echo Reinicie o computador caso tenha ocorrido a instalação de maneira adequada

sudo i2cdetect -y 1
sudo apt-get install cmake
sudo apt-get install libqt4-dev
cd ./Linux
cd build
cmake ..
make -j4
sudo make install
sudo ldconfig
startx
