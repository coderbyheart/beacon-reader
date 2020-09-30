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

#include <dk_buttons_and_leds.h>
#include <sys/byteorder.h>

#define BEACON_UUID_START 4
#define BEACON_UUID_SIZE 16

#define BEACON_MAJOR_START 20
#define BEACON_MAJOR_SIZE 2

#define BEACON_MINOR_START 22
#define BEACON_MINOR_SIZE 2

uint8_t beacon_uuid[16];
uint8_t beacon_major[2];
uint8_t beacon_minor[2];

#define NAME_LEN 30

static bool adv_data_found(struct bt_data *data, void *user_data)
{
        char *name = user_data;
		uint16_t tempVal;

        switch(data->type) {
        case BT_DATA_NAME_SHORTENED:
		case BT_DATA_NAME_COMPLETE:
			memcpy(name, data->data, MIN(data->data_len, NAME_LEN - 1));
			return true;
        case BT_DATA_SVC_DATA16:
				
				tempVal = (data->data[3] <<  8) | data->data[2];
				printf("temp: %d\n", tempVal);
                return true;
        default:
                return true;
        }
}

static void scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type,
		    struct net_buf_simple *ad)
{
        char le_addr[BT_ADDR_LE_STR_LEN];
        char name[NAME_LEN];
		(void)memset(name, 0, sizeof(name));
        
        bt_addr_le_to_str(addr, le_addr, sizeof(le_addr));
		if(strcmp(le_addr, "f0:49:04:8f:16:e5 (random)") == 0 || strcmp(le_addr, "d6:6f:5e:2f:a3:81 (random)") == 0) {
			bt_data_parse(ad, adv_data_found, name); // FIXME use struct for name+temp
			printk("%s %s %ddBm\n", le_addr, name, rssi);
		}
}

static void scan_start(void)
{
        struct bt_le_scan_param scan_param = {
		.type       = BT_HCI_LE_SCAN_PASSIVE,
		.options    = BT_LE_SCAN_OPT_NONE,
		.interval   = 0x0060,
		.window     = 0x0060,
	};

	int err;

        err = bt_le_scan_start(&scan_param, scan_cb);
		
	if (err) {
		printk("Starting scanning failed (err %d)\n", err);
		return;
        }
}

static void ble_ready(int err)
{
	printk("Bluetooth ready\n");
	scan_start();
}

void main(void) {
	int err;

	printk("Hello World!\n");
	
	printk("Initializing Bluetooth..\n");
	err = bt_enable(ble_ready);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}
}
