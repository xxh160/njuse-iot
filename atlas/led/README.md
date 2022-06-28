# Atlas200DK LED

使用 Atlas200DK 上的 GPIO 扩展接口来控制接入的 LED 灯.

参考仓库: [sample-gpio](https://gitee.com/Atlas200DK/hardware_expansio/tree/master/sample-gpio).

## 前提

使用户 HwHiAiUser 获得 GPIO 引脚的操作权限.

在 `/etc/rc.local` 的 `exit 0` 前加入:

```text
echo 504 >/sys/class/gpio/export
echo 444 >/sys/class/gpio/export
chown -R HwHiAiUser /sys/class/gpio/gpio444
chown -R HwHiAiUser /sys/class/gpio/gpio504
chown -R HwHiAiUser /sys/class/gpio/gpio444/direction
chown -R HwHiAiUser /sys/class/gpio/gpio504/direction
chown -R HwHiAiUser /sys/class/gpio/gpio444/value
chown -R HwHiAiUser /sys/class/gpio/gpio504/value
chown -R HwHiAiUser /dev/i2c-1
chown -R HwHiAiUser /dev/i2c-2
chown -R HwHiAiUser /dev/ttyAMA0
chown -R HwHiAiUser /dev/ttyAMA1
usermod -aG HwHiAiUser HwHiAiUser
```

当然手动执行也可以, 就是每次重启都要执行.

## Usage

本例中使用的是一个三色 LED, 共有四个引脚, 分别对应红色, 蓝色, 黄色和接地引脚.

将接地引脚接入任意一个 GND GPIO 引脚, 红色, 蓝色, 黄色对应引脚接入 `GPIO{0, 1, 3}` 方便程序控制.

> 如果是普通 LED 阴极就是接地引脚

编译项目:

```shell
make
```

控制 LED:

```shell
make run LED=$NUM
````

`$NUM` 为 0, 1, 2, 3, 4 中任意一个, 分别对应亮红色, 蓝色, 黄色, 全部都亮, 全部不亮.

## 复用

本项目本质上是用于控制 GPIO 的输入输出.

如果想要修改控制的引脚和电平, 请着重关注 `src/led.cpp` 和 `include/led.h`.

`src/gpio.cpp` 和 `include/gpio.h` 不用修改, 它们是通用的控制 GPIO 的程序接口.

本项目代码量不多, 自行 RTFSC 即可.
