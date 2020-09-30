#include <zephyr.h>
#include <device.h>
#include <stdio.h>
#include <stdlib.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/gatt.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt_dm.h>
#include <bluetooth/scan.h>
#include <bluetooth/hci.h>

#include "ble.h"

struct sensor_data inside = { .name = "", .temperature = -127.0 };
struct sensor_data outside = { .name = "", .temperature = -127.0 };

static bool adv_data_found(struct bt_data *data, void *user_data)
{
	struct sensor_data *sensorData = user_data;
	
	switch (data->type) {
	case BT_DATA_NAME_SHORTENED:
	case BT_DATA_NAME_COMPLETE:
		(void)memset(sensorData->name, 0, sizeof(sensorData->name));
		memcpy(sensorData->name, data->data,
		       MIN(data->data_len, sizeof(sensorData->name) - 1));
		return true;
	case BT_DATA_SVC_DATA16:
		sensorData->temperature = ((data->data[3] << 8) | data->data[2]) / 100.0;
		return true;
	default:
		return true;
	}
}

static void scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type,
		    struct net_buf_simple *ad)
{
	char le_addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(addr, le_addr, sizeof(le_addr));
	if (strcmp(le_addr, "f0:49:04:8f:16:e5 (random)") == 0) {
		bt_data_parse(ad, adv_data_found, &outside);
		printf("%s (%ddBm) %f\n", outside.name,
		       rssi, outside.temperature);
	} else if (strcmp(le_addr, "d6:6f:5e:2f:a3:81 (random)") == 0) {
		bt_data_parse(ad, adv_data_found, &inside);
		printf("%s (%ddBm) %f\n", inside.name,
		       rssi, inside.temperature );
	}
}

static void scan_start(void)
{
	struct bt_le_scan_param scan_param = {
		.type = BT_HCI_LE_SCAN_PASSIVE,
		.options = BT_LE_SCAN_OPT_NONE,
		.interval = 0x0060,
		.window = 0x0060,
	};

	int err;

	err += bt_le_scan_start(&scan_param, scan_cb);

	if (err) {
		printk("Starting scanning failed (err %d)\n", err);
		return;
	}
}

static void ble_ready(int err)
{
	printk("Bluetooth initialized.\n");
	scan_start();
}

void ble_init(void)
{
	printk("Initializing Bluetooth..\n");
	int err = bt_enable(ble_ready);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}
}