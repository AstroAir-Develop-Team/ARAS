project_path=$(cd `dirname $0`; pwd)

echo "Update & Upgrade\n"
sudo apt-get update
sudo apt-get upgrade

echo "Set includes && library"
sudo cp -R $project_path/../includes/configor /usr/include
sudo cp -R $project_path/../includes/imgui /usr/include
sudo cp -R $project_path/../includes/spdlog /usr/include
sudo cp -R $project_path/../includes/sockpp /usr/include
sudo cp -R $project_path/../libraries/sockpp /usr/lib

sudo apt install libwebsocketpp-dev

