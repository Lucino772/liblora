cmake_minimum_required(VERSION 3.10)

# set project name (with version)
project(lora VERSION 0.1)

# add the library
add_library(lora STATIC ./src/spi.c ./src/sx127x/driver.c)