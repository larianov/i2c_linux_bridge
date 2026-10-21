#!/usr/bin/env bash
# Show BME280 readings (kernel IIO driver) in real time.

BUS=8
ADDR=0x76

find_dev() { grep -l bme280 /sys/bus/iio/devices/iio:device*/name 2>/dev/null | head -n1; }

dev=$(find_dev)
if [[ -z $dev ]]; then
    sudo modprobe bmp280_i2c
    echo bme280 "$ADDR" | sudo tee "/sys/bus/i2c/devices/i2c-$BUS/new_device" > /dev/null
    sleep 1
    dev=$(find_dev)
fi
[[ -z $dev ]] && { echo "bme280 not found" >&2; exit 1; }
dev=${dev%/name}

while true; do
    awk -v t="$(< "$dev/in_temp_input")" \
        -v h="$(< "$dev/in_humidityrelative_input")" \
        -v p="$(< "$dev/in_pressure_input")" \
        'BEGIN { printf "\rT %6.2f °C   RH %5.1f %%   P %7.2f hPa ", t/1000, h/1000, p*10 }'
    sleep 1
done