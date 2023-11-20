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