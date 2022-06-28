#ifndef __GPIO_H_
#define __GPIO_H_

class gpio {
public:
  gpio(void);
  ~gpio(void);

	// pin 0 1 3 4 5 6 7
	// dir: 0 input, 1 output
  int gpio_set_direction( int pin, int direction); 
	// val: 0 low, 1 high 
  int gpio_set_value(int pin, int val); 
  int gpio_get_value(int pin, int *val); 

private:
  int i2c_write(unsigned char slave, unsigned char reg, unsigned char value);
  int i2c_read(unsigned char slave, unsigned char reg, unsigned char *buf);
  int i2c_1_init();
  int PCA6416_gpio_set_direction(int pin, int dir);
  int PCA6416_gpio_set_value(int pin, int val);
  int PCA6416_gpio_get_value(int pin, int *val);
  int ASCEND310_gpio_set_direction(int pin, int dir);
  int ASCEND310_gpio_set_value(int pin, int val);
  int ASCEND310_gpio_get_value(int pin, int *val);

private:
  int fd;
  struct i2c_msg {
		// slave addr
    unsigned short addr; 
    unsigned short flags;
    unsigned short len;
		// msg data ptr
    unsigned char *buf; 
  };
  struct i2c_rdwr_ioctl_data {
		// i2c_msg[] ptr
    struct i2c_msg *msgs; 
		// i2c_ms nums
    int nmsgs;
  };
};

// direction
#define OUT  1
#define IN   0
// value
#define LOW  0
#define HIGH 1

// ascend310 pin: GPIO0 GPIO1
#define ASCEND310_GPIO_0_DIR "/sys/class/gpio/gpio504/direction"
#define ASCEND310_GPIO_1_DIR "/sys/class/gpio/gpio444/direction"
#define ASCEND310_GPIO_0_VAL "/sys/class/gpio/gpio504/value"
#define ASCEND310_GPIO_1_VAL "/sys/class/gpio/gpio444/value"

// I2C Device
#define I2C1_DEV_NAME "/dev/i2c-1"
#define I2C_RETRIES 0x0701
#define I2C_TIMEOUT 0x0702
#define I2C_SLAVE 0x0703
#define I2C_RDWR 0x0707
#define I2C_M_RD 0x01
#define PCA6416_SLAVE_ADDR 0x20
#define PCA6416_GPIO_CFG_REG 0x07
#define PCA6416_GPIO_PORARITY_REG 0x05
#define PCA6416_GPIO_OUT_REG 0x03
#define PCA6416_GPIO_IN_REG 0x01
// GPIO MASK
#define GPIO3_MASK 0x10
#define GPIO4_MASK 0x20
#define GPIO5_MASK 0x40
#define GPIO6_MASK 0x80
#define GPIO7_MASK 0x08

#endif 

