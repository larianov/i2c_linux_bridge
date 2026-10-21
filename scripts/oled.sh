#!/usr/bin/env bash
# Stream the Wayland screen to an SSD1306 128x64 OLED over I2C.

BUS=8
ADDR=0x3c
CAPTURE="grim -t ppm -"

cmd() { i2ctransfer -y "$BUS" "w$(( $# + 1 ))@$ADDR" 0x00 "$@"; }

# Init: vertical addressing mode, charge pump on, display on.
cmd 0xAE 0xD5 0x80 0xA8 0x3F 0xD3 0x00 0x40 0x8D 0x14 0x20 0x01 \
    0xA1 0xC8 0xDA 0x12 0x81 0xCF 0xD9 0xF1 0xDB 0x40 0xA4 0xA6 0xAF

trap 'cmd 0xAE; exit' INT TERM

while true; do
    # Transpose so MONO packing matches SSD1306 column/page layout.
    frame=$($CAPTURE \
        | magick - -resize '128x64!' -colorspace Gray -dither FloydSteinberg \
                   -monochrome -negate -transpose mono:- \
        | od -An -v -tx1 | sed 's/[0-9a-f]\{2\}/0x&/g')

    cmd 0x21 0x00 0x7F 0x22 0x00 0x07
    i2ctransfer -y "$BUS" "w1025@$ADDR" 0x40 $frame
done