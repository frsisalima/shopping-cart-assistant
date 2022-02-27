### Create access point
Execute follows commands:
```shell script
IFNAME="wlan0"
CON_NAME="wifiname"
CON_PASS="wifipass"
```

```shell script
sudo nmcli con add type wifi ifname $IFNAME con-name $CON_NAME autoconnect yes ssid $CON_NAME
```

```shell script
sudo nmcli con modify $CON_NAME 802-11-wireless.mode ap 802-11-wireless.band bg ipv4.method shared
```

```shell script
sudo nmcli con modify $CON_NAME wifi-sec.psk $CON_PASS
```

```shell script
sudo nmcli con up $CON_NAME
```
