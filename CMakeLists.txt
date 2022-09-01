cmake_minimum_required(VERSION 3.10)

# set project name (with version)
project(lora VERSION 0.1)

set(lorasrcs ./src/spi.c ./src/rf95.c)
set(loraheaders ./include/rf95.h)

add_library(loraobjlib OBJECT ${lorasrcs})
set_property(TARGET loraobjlib PROPERTY POSITION_INDEPENDENT_CODE 1)

# add the library
add_library(lora_shared SHARED $<TARGET_OBJECTS:loraobjlib>)
add_library(lora_static STATIC $<TARGET_OBJECTS:loraobjlib>)

# install library & header files
install(TARGETS lora_shared DESTINATION lib)
install(TARGETS lora_static DESTINATION lib)
install(FILES ${loraheaders} DESTINATION include)