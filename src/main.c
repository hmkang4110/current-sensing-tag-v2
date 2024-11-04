/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   5000
#define LATCHING_DELAY 	100
#define INIT_DELAY		100
/* The devicetree node identifier for the "led0" alias. */
// #define LED0_NODE DT_ALIAS(led0)
#define RELAY1_NODE DT_ALIAS(relay1)
#define RELAY2_NODE DT_ALIAS(relay2)

#define LEDGREEN_NODE DT_ALIAS(ledgreen)
#define LEDRED_NODE DT_ALIAS(ledred)


/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec relay1 = GPIO_DT_SPEC_GET(RELAY1_NODE, gpios);
static const struct gpio_dt_spec relay2 = GPIO_DT_SPEC_GET(RELAY2_NODE, gpios);

static const struct gpio_dt_spec led_green = GPIO_DT_SPEC_GET(LEDGREEN_NODE, gpios);
static const struct gpio_dt_spec led_red = GPIO_DT_SPEC_GET(LEDRED_NODE, gpios);

static int led_rgb_init()
{
	int ret;
	ret = gpio_pin_set_dt(&led_green, 0);
	ret = gpio_pin_set_dt(&led_red, 0);
	k_sleep(K_MSEC(INIT_DELAY));
}

static int led_rgb_on()
{
	int ret;
	ret = gpio_pin_set_dt(&led_green, 0);
	ret = gpio_pin_set_dt(&led_red, 0);
}

static int led_rgb_off()
{
	int ret;
	ret = gpio_pin_set_dt(&led_green, 1);
	ret = gpio_pin_set_dt(&led_red, 1);
}

static int led_red_on()
{
	int ret;
	ret = gpio_pin_set_dt(&led_red, 0);
}

static int led_green_on()
{
	int ret;
	ret = gpio_pin_set_dt(&led_green, 0);
}

static int led_red_off()
{
	int ret;
	ret = gpio_pin_set_dt(&led_red, 1);
}

static int led_green_off()
{
	int ret;
	ret = gpio_pin_set_dt(&led_green, 1);
}

static int latch_relay_init()
{
	int ret;
	ret = gpio_pin_set_dt(&relay1, 0);
	ret = gpio_pin_set_dt(&relay2, 0);
	k_sleep(K_MSEC(INIT_DELAY));
}

static int latch_relay_on()
{
	int ret;
	k_sleep(K_MSEC(LATCHING_DELAY));
	ret = gpio_pin_set_dt(&relay2, 1);
	k_sleep(K_MSEC(LATCHING_DELAY));
	ret = gpio_pin_set_dt(&relay2, 0);
	k_sleep(K_MSEC(LATCHING_DELAY));

	return ret;
}

static int latch_relay_off()
{
	int ret;
	k_sleep(K_MSEC(LATCHING_DELAY));
	ret = gpio_pin_set_dt(&relay1, 1);
	k_sleep(K_MSEC(LATCHING_DELAY));
	ret = gpio_pin_set_dt(&relay1, 0);
	k_sleep(K_MSEC(LATCHING_DELAY));

	return ret;
}

static int led_rgb_gpio_configuration_init()
{
	int ret;

	if (!device_is_ready(led_green.port)) {
		return;
	}
	if (!device_is_ready(led_red.port)) {
		return;
	}

	ret = gpio_pin_configure_dt(&led_green, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}
	ret = gpio_pin_configure_dt(&led_red, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}
}

static int latch_relay_gpio_configuration_init()
{
	int ret;

	if (!device_is_ready(relay1.port)) {
		return;
	}
	if (!device_is_ready(relay2.port)) {
		return;
	}

	ret = gpio_pin_configure_dt(&relay1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}
	ret = gpio_pin_configure_dt(&relay2, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}
}

void main(void)
{
	int ret;

	latch_relay_gpio_configuration_init();
	latch_relay_init();
	led_rgb_gpio_configuration_init();
	led_rgb_init();

	// fot testing
	for(;;)
	{
		for (int i=0; i<3; i++)
		{
			latch_relay_on();
			k_sleep(K_MSEC(3000));
			latch_relay_off();
			k_sleep(K_MSEC(3000));
		}
		for (int i=0; i<3; i++)
		{
			led_green_on();
			k_sleep(K_MSEC(500));
			led_green_off();
			k_sleep(K_MSEC(500));

			led_red_on();
			k_sleep(K_MSEC(500));
			led_red_off();
			k_sleep(K_MSEC(500));

			led_rgb_on();
			k_sleep(K_MSEC(500));
			led_rgb_off();
			k_sleep(K_MSEC(500));
		}

		k_sleep(K_MSEC(2000));
	}


}
