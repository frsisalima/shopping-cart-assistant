### Install pistache
Execute follows commands:
```shell script
sudo su
```
```shell script
pip3 install meson --ignore-installed
```
```shell script
git clone https://github.com/pistacheio/pistache.git
```
```shell script
meson setup build
```
```shell script
meson install -C build
```