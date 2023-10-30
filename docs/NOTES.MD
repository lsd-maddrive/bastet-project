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
