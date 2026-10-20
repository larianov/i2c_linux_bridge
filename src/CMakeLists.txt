add_executable(i2c_rp2040
    main.c
    usb_descriptor.c
)

target_include_directories(i2c_rp2040 PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

pico_enable_stdio_uart(i2c_rp2040 1)
pico_enable_stdio_usb(i2c_rp2040 0)

target_link_libraries(i2c_rp2040 PRIVATE
    pico_stdlib
    tinyusb_device
)

firmware_setup(i2c_rp2040)
