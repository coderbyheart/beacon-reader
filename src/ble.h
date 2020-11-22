#ifndef _BLE_H_
#define _BLE_H_

void ble_init(void);

#define NAME_LEN 30

struct sensor_data {
	float temperature;
	char name[NAME_LEN];
	bool fresh;
};

#endif /* _BLE_H_ */