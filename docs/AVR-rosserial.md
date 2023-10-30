# Использование rosserial с AVR и UART

## 1. Исходный код лежит в пакете ros-avr

Исходники лежат здесь (добавить где)

## 2. Подготовка
### 2.1 AVR Development Environment

Для сборки исходников необходимо установить следущие утилиты и библиотеки
```
sudo apt-get install binutils-avr avr-libc gcc-avr avrdude
```

## 3. Загрузка исходников 

### 3.1 rosserial_arduino
Для работы rosserial используются библиотеки rosserial_arduino. Можно их достать самостоятельно, либо воспользоваться нашим репозиторием

### 3.2 Дополнительные модули
Также дополнительно необходимы следующие модули:

avr_time - использует прерывание для подсчета количества переполнений таймера с момента запуска программы и конвертирует его в миллисекунды.

avr_uart - настраивает UART AVR и может отправлять и получать данные

Atmega2560Hardware.h - заголовочный файл для работы нашего контроллера с rosserial

ros.h - typedef для удобства

## 4. Написание узла
Пример написания узла публикации приведен ниже. 

```
#include "ros.h"
#include "std_msgs/String.h"
// Include C headers (ie, non C++ headers) in this block
extern "C"
{
  #include <util/delay.h>
}

// Needed for AVR to use virtual functions
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

ros::NodeHandle nh;

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

char hello[13] = "hello world!";

int main()
{
  uint32_t lasttime = 0UL;

  // Initialize ROS
  nh.initNode();
  nh.advertise(chatter);

  while(1)
  {
    // Send the message every second
    if(avr_time_now() - lasttime > 1000)
    {
      str_msg.data = hello;
      chatter.publish(&str_msg);
      lasttime = avr_time_now();
    }
    nh.spinOnce();
  }

  return 0;
}
```


В целом синтакс практически не отличается от написание узлов для ардуино, за исключением использования задержки посредствам самостоятельно написанной библиотеки avr_time.c

## 6. Makefile

Для сборки прошивки и программирования используется Makefile, который также можно найти в исходниках. Он содержит много кода, но по существу нас интересуют только следущие строки:

Имя целевого файла (без расширения):
```
TARGET = avr_chatter 
```
Исходные файлы C и C++ соответсвенно:
```
SRC =avr_time.c avr_uart.c
CPPSRC = $(TARGET).cpp ros_lib/time.cpp ros_lib/duration.cpp
```

Стандарт с++
```
CPPFLAGS += c++11
```

## 7. Сборка прошивки

Откройте терминал в директории rosserial_avr_tutorial/src и выполните
```
make
```
Если сборка прошла удачно, должен появиться файл avr_chatter.hex