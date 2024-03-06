# Заметки и полезные ссылки

## Установка драйвера TP-Link AC600 Archer T2U Nano для Ubuntu

Обновите пакеты:
```bash
sudo apt update && sudo apt upgrade
```
Для установки райвера выполните следующее:
```bash
sudo apt install git dkms
git clone https://github.com/aircrack-ng/rtl8812au.git
cd rtl8812au
sudo make dkms_install
```

Отключите адаптер и снова подключите его. Светодиод начнет мигать. Убедитесь, что драйвер установлен и загружен с помощью команды:

```bash
sudo dkms status
```

Если адаптер установлен, вы увидите следующий вывод:

```bash
8812au, 5.6.4.2_35491.20191025, 5.11.15-1-default, x86_64: installed
```
## Проблема с запуском rosserial python

Если при запуске rosserial python на Jetson Nano пишет, что нет доступа к порту, то нужно сделать:

```bash
sudo su
//type your password
cd /
cd dev
chown username ttyUSB0
```
## SSH

Следует установить пакет, соддержащую утилиту netstat, для этого

```bash
sudo apt install net-tools
```
Устанавливаем сервер ssh:

```bash
sudo apt install openssh-server
```

Далее проверяем, какие порты прослушиваются:

```bash
netstat -t -l -n
```
Должен быть порт 22

Чтобы узнать адрес устройства, к которому будем подключаться, необходимо **на нем** выполнить команду:

```bash
ip addr
```

При помощи следующей команды происходит подключение к устройству по SSH. При первом подключении нужно будет подтвердить, ввести пароль.

```bash
ssh {USERNAME}@{IP ADRESS}
```

При работе в VS Code нужно установить **Remote SSH**. Далее **Connect to Host - Add New SSH HOST** и ввести **ssh {USERNAME}@{IP ADRESS}** 
в всплывающем вверху окне. 

## Joy

Джойстик используется в режиме XInput (переключатель находится на передней части геймпада)

## Input/output Error
https://www.chippiko.com/arduino-input-output-error

How to Fix Input/output Error
To be able to fix it is to modify the grub file that is on the linux system itself. The steps are:

Open the group file using a text editor via terminal. I use gedit text editor

sudo gedit /etc/default/grub
- Look for the line containing the word GRUB_CMDLINE_LINUX_DEFAULT=””
- Then enter the word usbcore.autosuspend=-1 to ” “. and becoming GRUB_CMDLINE_LINUX_DEFAULT=”usbcore.autosuspend=-1”

If by default your linux is GRUB_CMDLINE_LINUX_DEFAULT=”qui ```et splash”, and then becoming to GRUB_CMDLINE_LINUX_DEFAULT=”quiet splash usbcore.autosuspend=-1″

after that save the file.
- Update grub with command:

sudo update-grub
Reboot
After that, reconnect the Arduino board to the computer, reopen the Arduino IDE, re-upload it and upload the code will successfully.

Hopefully this Arduino Input/output Error article can help you in learning the Arduino Environment. If this Arduino error article is useful, please share it using the share button below.

 
## Прошивка через usb
burn bootlader
прошиваем один раз blink через arduino IDE
не должно быть udev (либо прошить с винды 1 раз)???????????

/snap/arduino/85/hardware/tools/avr/bin/avrdude -C/snap/arduino/85/hardware/tools/avr/etc/avrdude.conf -v -p atmega2560 -c wiring -P /dev/ttyACM0 -b 115200 -D -Uflash:w:avr_subscriber.hex:i


/snap/arduino/85/hardware/tools/avr/bin/avrdude -C/snap/arduino/85/hardware/tools/avr/etc/avrdude.conf -v -patmega2560 -cstk500v2 -P/dev/ttyUSB0 -e -Ulock:w:0x3F:m -Uefuse:w:0xFD:m -Uhfuse:w:0xD8:m -Ulfuse:w:0xFF:m

# [SOLVED] can't open device "/dev/ttyUSB0": Perm. denied (Linux permissions)

sudo usermod -a -G dialout <username>

## Beelink auto power on
https://www.reddit.com/r/BeelinkOfficial/comments/13lqrz4/beelink_eq12_eq12pro_how_to_set_auto_power_on/

1. Enter BIOS setup.
2. Use the arrow keys to enter the Chipset page. Select “PCH-IO Configuration” and press the Enter key.
3. Select “State After G3”.
4. Select “S0 State”. “S0 State” is to enable auto power on and “S5 State” is to disable auto power on.
5. Press the F4 and select “Yes” to save the configuration.

