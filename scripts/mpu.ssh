#!/usr/bin/env bash
# Show MPU-6050 accelerometer, gyroscope and temperature in real time.

BUS=8
ADDR=0x68

# Wake up (PWR_MGMT_1 = 0). Defaults: accel ±2 g, gyro ±250 °/s.
i2ctransfer -y "$BUS" "w2@$ADDR" 0x6B 0x00

while true; do
    # 14 bytes from ACCEL_XOUT_H: accel xyz, temp, gyro xyz (big-endian int16).
    read -ra b <<< "$(i2ctransfer -y "$BUS" "w1@$ADDR" 0x3B r14)"

    v=()
    for i in 0 2 4 6 8 10 12; do
        x=$(( (b[i] << 8) | b[i + 1] ))
        (( x >= 32768 )) && (( x -= 65536 ))
        v+=("$x")
    done

    awk -v ax="${v[0]}" -v ay="${v[1]}" -v az="${v[2]}" -v t="${v[3]}" \
        -v gx="${v[4]}" -v gy="${v[5]}" -v gz="${v[6]}" \
        'BEGIN { printf "\rA % .2f % .2f % .2f g   G % 7.1f % 7.1f % 7.1f °/s   T %5.1f °C ",
                 ax/16384, ay/16384, az/16384, gx/131, gy/131, gz/131, t/340 + 36.53 }'
    sleep 0.1
done