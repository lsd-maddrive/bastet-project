# Инструкции по разработке

## Требования

- Нативно установленная система [Ubuntu 20.04 Desktop](https://releases.ubuntu.com/focal/)
    - Docker, установленный с помощью [скрипта](https://get.docker.com/) с последующей [настройкой](https://docs.docker.com/engine/install/linux-postinstall/). Образ изображения находится в файлах метапакета [maddrive_ros_shared](https://github.com/lsd-maddrive/maddrive_ros_shared)
- Установленный [ROS Noetic](http://wiki.ros.org/noetic/Installation)

## Некоторые правила разработки

- Перед началом работы над задачей (созданием ветки под задачу) сделайте `git pull` в `develop` ветке.
- Каждый комит должен содержать номер таски, в рамках которой делалась работа в этом комите. Пример: `#88 designed config structure and added comments`.
- Имя ветки, в которой ведется работа, должна содержать номер таски. Пример: `feature/88_radar_config`.
- При создании задачи указывайте проект в меню Projects.
- После завершения работы над задачей создавайте Pull Request на вивание ветки в `develop`. При создании указывайте ревьюверов (как минимум ведущего, можно и остальных), проект в Projects и связанные задачи (Linked Issues).
- После апрува сливает в `develop` ведущий разработчик.

## Подготовка к работе

* Для работы в среде ROS требуется установить в workspace (папка `src`) [репозиторий с общими тулами для ROS](https://github.com/lsd-maddrive/maddrive_ros_shared).

* Для запуска и сохранения карт настройте переменную окружения 

    ```bash
    export CATKIN_WORKSPACE="$HOME/catkin_ws"
    export GAZEBO_MODEL_PATH=$(rospack find maddrive_worlds)/models
    ```

### Установка catkin_tools

- Установите с помощью `sudo apt-get install -y python3-catkin-tools`

### Преднастройка

- В пакете `maddrive_ros_shared` установите бинарные пакеты при помощи команды `./scripts/install_packages.sh`.
    - P.s. если у вас при выполнении команды `./scripts/install_packages.sh` выдается ошибка `no such file or directory`, убедитесь, что находитесь в папке `maddrive_ros_shared`.
- В пакете `maddrive_ros_shared` установите пакеты для сборки при помощи команды `./scripts/install_third_party.sh`.
- При работе с железом установите требуемые библиотеки и тулсы при помощи команды `./scripts/install_hardware.sh`.

### Сборка

- Соберите все пакеты с зависимостями с помощью скрипта [build.sh](/kitty-project/scripts/build.sh)


## Запуск симулятора

```bash
roslaunch kitty_software sim_start.launch
```
