### Upgrade cmake to 3.19.5
Execute follows commands:
```shell script
wget https://github.com/Kitware/CMake/releases/download/v3.19.5/cmake-3.19.5-Linux-aarch64.tar.gz -q --show-progress
```

```shell script
tar -zxvf cmake-3.19.5-Linux-aarch64.tar.gz
```

```shell script
cd cmake-3.19.5-Linux-aarch64/
```

```shell script
sudo cp -rf bin/ doc/ share/ /usr/local/
```

```shell script
sudo cp -rf man/* /usr/local/man
```

```shell script
sync
```

```shell script
cmake --version
```