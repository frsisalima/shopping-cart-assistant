upgrade cmake 3.19.5
wget https://github.com/Kitware/CMake/releases/download/v3.19.5/cmake-3.19.5-Linux-aarch64.tar.gz -q --show-progress
tar -zxvf cmake-3.19.5-Linux-aarch64.tar.gz
cd cmake-3.19.5-Linux-aarch64/
sudo cp -rf bin/ doc/ share/ /usr/local/
sudo cp -rf man/* /usr/local/man
sync
cmake --version

sudo su
pip3 install meson --ignore-installed
//reload shell
git clone https://github.com/pistacheio/pistache.git
meson setup build
meson install -C build


sudo apt install libnm-dev


IFNAME="wlan0"
CON_NAME="wifiname"
CON_PASS="wifipass"
sudo nmcli con add type wifi ifname $IFNAME con-name $CON_NAME autoconnect yes ssid $CON_NAME
sudo nmcli con modify $CON_NAME 802-11-wireless.mode ap 802-11-wireless.band bg ipv4.method shared
sudo nmcli con modify $CON_NAME wifi-sec.psk $CON_PASS
sudo nmcli con up $CON_NAME
