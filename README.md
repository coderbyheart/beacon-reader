# Temperature Beacon Reader using the nRF9160 DK

This implements a temperature beacon reader using the nRF9160 DK:

## Building

### Prepare the Docker image with all build dependencies

    docker build -t sdk-nrf-tbr .

### Build the firmware

### For the nRF9160 of the DK

    docker run --rm -v ${PWD}:/workdir/ncs/tbr sdk-nrf-tbr \
        /bin/bash -c 'cd /workdir/ncs/tbr && west build -p always -b nrf9160dk_nrf9160ns'
    nrfjprog -f nrf91 --program build/zephyr/merged.hex --sectoranduicrerase -r --log

### For the nRF52840 of the DK

    docker run --rm -v ${PWD}:/workdir/ncs/tbr sdk-nrf-tbr \
        /bin/bash -c 'cd ncs/zephyr && west build samples/bluetooth/hci_uart -p always -b nrf9160dk_nrf52840 && ls -la build/zephyr && cp build/zephyr/zephyr.hex /workdir/ncs/tbr/hci_uart.hex'
    nrfjprog -f nrf52 --program hci_uart.hex --sectoranduicrerase -r --log
