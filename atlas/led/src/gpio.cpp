#include <fstream>
#include <memory>
#include <sstream>
#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <common.h>
#include <gpio.h>

// i2c_write, for configure PCA6416 register.
int gpio::i2c_write(unsigned char slave, unsigned char reg,
                    unsigned char value) {
  int ret;
  struct i2c_rdwr_ioctl_data ssm_msg = {0};
  unsigned char buf[2] = {0};
  ssm_msg.nmsgs = 1;
  ssm_msg.msgs =
      (struct i2c_msg *)malloc(ssm_msg.nmsgs * sizeof(struct i2c_msg));
  if (ssm_msg.msgs == NULL) {
    uerror("%s", "Memory alloc error !");
    return -1;
  }
  buf[0] = reg;
  buf[1] = value;
  (ssm_msg.msgs[0]).flags = 0;
  (ssm_msg.msgs[0]).addr = (unsigned short)slave;
  (ssm_msg.msgs[0]).buf = buf;
  (ssm_msg.msgs[0]).len = 2;
  ret = ioctl(fd, I2C_RDWR, &ssm_msg);
  if (ret < 0) {
    uerror("write error, ret=%#x, errorno=%#x, %s!", ret, errno,
           strerror(errno));
    free(ssm_msg.msgs);
    ssm_msg.msgs = NULL;
    return -1;
  }
  free(ssm_msg.msgs);
  ssm_msg.msgs = NULL;
  return 0;
}

// i2c_read, for reading PCA6416 register.
int gpio::i2c_read(unsigned char slave, unsigned char reg, unsigned char *buf) {
  int ret;
  struct i2c_rdwr_ioctl_data ssm_msg = {0};
  unsigned char regs[2] = {0};
  regs[0] = reg;
  regs[1] = reg;
  ssm_msg.nmsgs = 2;
  ssm_msg.msgs =
      (struct i2c_msg *)malloc(ssm_msg.nmsgs * sizeof(struct i2c_msg));
  if (ssm_msg.msgs == NULL) {
    uerror("%s", "Memory alloc error!");
    return -1;
  }
  (ssm_msg.msgs[0]).flags = 0;
  (ssm_msg.msgs[0]).addr = slave;
  (ssm_msg.msgs[0]).buf = regs;
  (ssm_msg.msgs[0]).len = 1;
  (ssm_msg.msgs[1]).flags = I2C_M_RD;
  (ssm_msg.msgs[1]).addr = slave;
  (ssm_msg.msgs[1]).buf = buf;
  (ssm_msg.msgs[1]).len = 1;
  ret = ioctl(fd, I2C_RDWR, &ssm_msg);
  if (ret < 0) {
    uerror("read data error,ret=%#x !", ret);
    free(ssm_msg.msgs);
    ssm_msg.msgs = NULL;
    return -1;
  }
  free(ssm_msg.msgs);
  ssm_msg.msgs = NULL;
  return 0;
}

// i2c_init, for access i2c device.
int gpio::i2c_1_init() {
  // open i2c-1 device
  fd = open(I2C1_DEV_NAME, O_RDWR);
  if (fd < 0) {
    uerror("%s", "i2c-1 Can't open !");
    return -1;
  }
  // set i2c-1 retries time
  if (ioctl(fd, I2C_RETRIES, 1) < 0) {
    close(fd);
    fd = 0;
    uerror("%s", "set i2c-1 retry fail!");
    return -1;
  }
  // set i2c-1 timeout time, 10ms as unit
  if (ioctl(fd, I2C_TIMEOUT, 1) < 0) {
    close(fd);
    fd = 0;
    uerror("%s", "set i2c-1 timeout fail!");
    return -1;
  }
  return 0;
}

gpio::gpio(void) { i2c_1_init(); }

gpio::~gpio(void) {}

int gpio::PCA6416_gpio_set_direction(int pin, int dir) {
  unsigned char slave;
  unsigned char reg;
  unsigned char data;
  int ret;

  if ((pin != 3) && (pin != 4) && (pin != 5) && (pin != 6) && (pin != 7)) {
    uerror("%s", "PCA6416 pin not right ,pin param must be 3, 4, 5, 6, 7");
    return -1;
  }

  // set GPIO as output
  slave = PCA6416_SLAVE_ADDR;
  reg = PCA6416_GPIO_CFG_REG;
  data = 0;
  ret = i2c_read(slave, reg, &data);
  if (ret != 0) {
    close(fd);
    fd = 0;
    uerror("GPIO read %#x %#x to %#x fail!\n", slave, data, reg);
    return -1;
  }

  if (dir == 0) {
    if (pin == 3)
      data |= GPIO3_MASK;
    else if (pin == 4)
      data |= GPIO4_MASK;
    else if (pin == 5)
      data |= GPIO5_MASK;
    else if (pin == 6)
      data |= GPIO6_MASK;
    else if (pin == 7)
      data |= GPIO7_MASK;
  } else {
    if (pin == 3)
      data &= ~GPIO3_MASK;
    else if (pin == 4)
      data &= ~GPIO4_MASK;
    else if (pin == 5)
      data &= ~GPIO5_MASK;
    else if (pin == 6)
      data &= ~GPIO6_MASK;
    else if (pin == 7)
      data &= ~GPIO7_MASK;
  }

  ret = i2c_write(slave, reg, data);
  if (ret != 0) {
    close(fd);
    fd = 0;
    uerror("GPIO write %#x %#x to %#x fail!", slave, data, reg);
    return -1;
  }

  return 0;
}

int gpio::PCA6416_gpio_set_value(int pin, int val) {
  unsigned char slave;
  unsigned char reg;
  unsigned char data;
  int ret;

  if ((pin != 3) && (pin != 4) && (pin != 5) && (pin != 6) && (pin != 7)) {
    uerror("%s", "PCA6416 pin not right ,pin param must be 3, 4, 5, 6, 7");
    return -1;
  }

  // Set GPIO output level
  slave = PCA6416_SLAVE_ADDR;
  reg = PCA6416_GPIO_OUT_REG;
  data = 0;
  ret = i2c_read(slave, reg, &data);
  if (ret != 0) {
    close(fd);
    fd = 0;
    uerror("GPIO read %#x %#x to %#x fail!", slave, data, reg);
    return -1;
  }

  if (val == 0) {
    if (pin == 3)
      data &= ~GPIO3_MASK;
    else if (pin == 4)
      data &= ~GPIO4_MASK;
    else if (pin == 5)
      data &= ~GPIO5_MASK;
    else if (pin == 6)
      data &= ~GPIO6_MASK;
    else if (pin == 7)
      data &= ~GPIO7_MASK;
  } else {
    if (pin == 3)
      data |= GPIO3_MASK;
    else if (pin == 4)
      data |= GPIO4_MASK;
    else if (pin == 5)
      data |= GPIO5_MASK;
    else if (pin == 6)
      data |= GPIO6_MASK;
    else if (pin == 7)
      data |= GPIO7_MASK;
  }
  ret = i2c_write(slave, reg, data);
  if (ret != 0) {
    close(fd);
    fd = 0;
    uerror("GPIO write %#x %#x to %#x fail!", slave, data, reg);
    return -1;
  }

  return 0;
}

int gpio::PCA6416_gpio_get_value(int pin, int *val) {
  unsigned char slave;
  unsigned char reg;
  unsigned char data;
  int ret;

  if ((pin != 3) && (pin != 4) && (pin != 5) && (pin != 6) && (pin != 7)) {
    uerror("%s", "PCA6416 pin not right ,pin param must be 3, 4, 5, 6, 7");
    return -1;
  }

  // get GPIO inputput level
  slave = PCA6416_SLAVE_ADDR;
  reg = PCA6416_GPIO_IN_REG;
  data = 0;
  ret = i2c_read(slave, reg, &data);
  if (ret != 0) {
    close(fd);
    fd = 0;
    uerror("GPIO read %#x %#x to %#x fail!", slave, data, reg);
    return -1;
  }

  if (pin == 3)
    data &= GPIO3_MASK;
  else if (pin == 4)
    data &= GPIO4_MASK;
  else if (pin == 5)
    data &= GPIO5_MASK;
  else if (pin == 6)
    data &= GPIO6_MASK;
  else if (pin == 7)
    data &= GPIO7_MASK;

  if (data > 0)
    *val = 1;
  else
    *val = 0;

  return 0;
}

int gpio::ASCEND310_gpio_set_direction(int pin, int dir) {
  int fd_direction;

  if (pin == 0)
    fd_direction = open(ASCEND310_GPIO_0_DIR, O_WRONLY);
  else if (pin == 1)
    fd_direction = open(ASCEND310_GPIO_1_DIR, O_WRONLY);
  else {
    uerror("%s", "ASCEND310 pin not right, pin param must be 0, 1");
    return -1;
  }

  if (-1 == fd_direction) {
    uerror("open gpio DIR file error pin=%d", pin);
    return (-1);
  }

  if (dir == 0) {
    if (-1 == write(fd_direction, "in", sizeof("in"))) {
      uerror("gpio write operation error pin=%d", pin);
      close(fd_direction);
      return (-1);
    }
  } else {
    if (-1 == write(fd_direction, "out", sizeof("out"))) {
      uerror("gpio write operation error pin=%d", pin);
      close(fd_direction);
      return (-1);
    }
  }

  close(fd_direction);
  return 0;
}

int gpio::ASCEND310_gpio_set_value(int pin, int val) {
  int fd_gpio_value;
  unsigned char value;

  if (pin == 0)
    fd_gpio_value = open(ASCEND310_GPIO_0_VAL, O_WRONLY);
  else if (pin == 1)
    fd_gpio_value = open(ASCEND310_GPIO_1_VAL, O_WRONLY);
  else {
    uerror("%s", "ASCEND310 pin not right ,pin param must be 0, 1");
    return -1;
  }

  if (-1 == fd_gpio_value) {
    uerror("open gpio VAL file error pin=%d", pin);
    return (-1);
  }

  if (val == 0) {
    value = '0';
    if (-1 == write(fd_gpio_value, &value, sizeof(value))) {
      uerror("gpio write operation error pin=%d", pin);
      close(fd_gpio_value);
      return (1);
    }
  } else {
    value = '1';
    if (-1 == write(fd_gpio_value, &value, sizeof(value))) {
      uerror("gpio write operation error pin=%d", pin);
      close(fd_gpio_value);
      return (-1);
    }
  }

  close(fd_gpio_value);
  return 0;
}

int gpio::ASCEND310_gpio_get_value(int pin, int *val) {
  int fd_gpio_value;
  char value_str[3];

  if (pin == 0)
    fd_gpio_value = open(ASCEND310_GPIO_0_VAL, O_RDONLY);
  else if (pin == 1)
    fd_gpio_value = open(ASCEND310_GPIO_1_VAL, O_RDONLY);
  else {
    uerror("%s", "ASCEND310 pin not right ,pin param must be 0, 1");
    return -1;
  }

  if (-1 == fd_gpio_value) {
    uerror("open gpio VAL file error pin=%d", pin);
    return (-1);
  }

  if (-1 == read(fd_gpio_value, value_str, 3)) {
    uerror("Failed to read value pin=%d", pin);
    return -1;
  }
  *val = atoi(value_str);

  close(fd_gpio_value);
  return 0;
}

int gpio::gpio_set_direction(int pin, int direction) {
  if ((pin == 0) || (pin == 1))
    return ASCEND310_gpio_set_direction(pin, direction);
  else
    return PCA6416_gpio_set_direction(pin, direction);
}

int gpio::gpio_set_value(int pin, int val) {
  if ((pin == 0) || (pin == 1))
    return ASCEND310_gpio_set_value(pin, val);
  else
    return PCA6416_gpio_set_value(pin, val);
}

int gpio::gpio_get_value(int pin, int *val) {

  if ((pin == 0) || (pin == 1))
    return ASCEND310_gpio_get_value(pin, val);
  else
    return PCA6416_gpio_get_value(pin, val);
}
