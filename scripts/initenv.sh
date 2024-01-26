#!/bin/bash
echo  'KERNEL=="ttyACM*", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="0042", MODE:="0666", GROUP:="dialout", SYMLINK+="m2560"' >/etc/udev/rules.d/m2560.rules

service udev reload
sleep 2
service udev restart
