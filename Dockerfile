FROM coderbyheart/fw-nrfconnect-nrf-docker:latest
RUN rm -rf /workdir/ncs
COPY . /workdir/ncs/tbr
RUN \
    # Zephyr requirements of tbr
    cd /workdir/ncs/tbr; west init -l && \
    cd /workdir/ncs; west update && \
    pip3 install -r zephyr/scripts/requirements.txt && \
    pip3 install -r nrf/scripts/requirements.txt && \
    pip3 install -r bootloader/mcuboot/scripts/requirements.txt && \
    rm -rf /workdir/ncs/tbr