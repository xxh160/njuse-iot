#include <common.h>
#include <led.h>
#include <gpio.h>
#include <stdlib.h>

// altas200DK gpio:
// https://support.huaweicloud.com/productdesc-A200dk_3000/atlas200_DK_pdes_19_0020.html

// pin 0 1 3 4 5 6 7
// dir: 0, input; 1, output
// value: 0, low; 1, high
static void set_all_out(gpio &g) {
	int all_pins[] = { 0, 1, 3, 4, 5, 6, 7 };
	for (int i = 0; i < ARRLEN(all_pins); ++i) {
		g.gpio_set_direction(all_pins[i], OUT);
	}
}

static void all_value(gpio &g) {
	int all_pins[] = { 0, 1, 3, 4, 5, 6, 7 };
	int value = 0;
	for (int i = 0; i < ARRLEN(all_pins); ++i) {
		g.gpio_get_value(all_pins[i], &value);
		uinfo("Pin: %d, value: %d", all_pins[i], value);
	}
}

// input: led index
// red: 0, green: 1, yellow: 2, all: 3
int main(int argc, char* argv[]) {
	uinfo("Argc: %d", argc);
	gpio g;
	set_all_out(g);

	int idx = -1;
	if (argc > 1)
		idx = (int) strtol(argv[1], NULL, 10);
	uinfo("LED idx: %d", idx);
	led_show(g, idx);

	all_value(g);
	return 0;
}

