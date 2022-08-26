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