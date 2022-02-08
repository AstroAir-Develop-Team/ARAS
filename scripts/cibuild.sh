project_path=$(cd `dirname $0`; pwd)

echo "Update & Upgrade\n"
sudo apt-get update
sudo apt-get upgrade

echo "Set includes & library"
sudo cp -R $project_path/../includes/configor /usr/include
sudo cp -R $project_path/../includes/imgui /usr/include
sudo cp -R $project_path/../includes/spdlog /usr/include

echo "Build sockpp"
git clone https://github.com/fpagliughi/sockpp
cd sockpp
mkdir build
cmake -DCMAKE_INSTALL_PREFIX=/usr -S . -B build
cmake --build build/
cd build && make -j4
sudo make install
cd ../../

git clone https://github.com/ArthurSonzogni/FTXUI
cd FTXUI
mkdir build
cmake -DCMAKE_INSTALL_PREFIX=/usr -S . -B build
cd build && make -j4
sudo make install
sudo cp ftxui-component.a ../../libraries/ftxui
sudo cp ftxui-dom.a ../../libraries/ftxui
sudo cp ftxui-screen.a ../../libraries/ftxui
cd ../../

wget http://www.lua.org/ftp/lua-5.4.4.tar.gz
tar -xvf lua-5.4.4.tar.gz
cd lua-5.4.4
make -j4
sudo make install
sudo cp src/liblua.a ../libraries/lua
cd ..

sudo apt install libwebsocketpp-dev libglfw3-dev libgl-dev libboost-dev

