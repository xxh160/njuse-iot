#include <common.h>
#include <led.h>

// red: gpio 0
// blue: gpio 1
// yellow: gpio 3
static struct {
  const char *name;
  int gpios[N_GPIO];
} leds[] = {
  {"red", {HIGH, LOW, LOW}},
  {"blue", {LOW, HIGH, LOW}},
  {"yellow", {LOW, LOW, HIGH}},
  {"all", {HIGH, HIGH, HIGH}},
  {"none", {LOW, LOW, LOW}},
};

static int gp[N_GPIO] = {0, 1, 3};

void led_show(gpio &g, int idx) {
  int num = ARRLEN(leds);
  if (idx == -1 || idx >= num)
    idx = num - 1;
  uinfo("Led: %s", leds[idx].name);
  int *gv = leds[idx].gpios;
  for (int i = 0; i < N_GPIO; ++i) {
    g.gpio_set_value(gp[i], gv[i]);
  }
}
