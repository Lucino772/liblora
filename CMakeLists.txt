cmake_minimum_required(VERSION 3.10)

set(VERSION 0.1)

project(lora VERSION ${VERSION})
set(TARGET lora)
set(INCLUDE_DIR ./include/liblora)

# Source Files
set(lora_src
    # Frameworks
    ./src/frameworks/arduino.c
    ./src/frameworks/wiringpi.c

    # Radio
    ./src/reg/sx127x.c ./src/rf95.c

    # Misc
    ./src/machine/com.c
)

# Headers file
set(lora_h
    ${INCLUDE_DIR}/rf95.h
    ${INCLUDE_DIR}/liblora.h
)

# Add library target
add_library (${TARGET} SHARED ${lora_src})

# Version
set_target_properties(${TARGET} PROPERTIES
                        VERSION ${PROJECT_VERSION}
                        SOVERSION ${PROJECT_VERSION_MAJOR})

# Add include directory for liblora
target_include_directories (${TARGET} PUBLIC
                            ${CMAKE_CURRENT_SOURCE_DIR}/include
                            ${CMAKE_INSTALL_PREFIX}/include)


                            
# Required for cross compiling to find wiringPi
target_link_directories(${TARGET} PUBLIC 
                            ${CMAKE_INSTALL_PREFIX}/lib)
                            
# Add the following required libraries: WiringPi
target_link_libraries(${TARGET} wiringPi)

# Add install
install(TARGETS ${TARGET} DESTINATION lib)
install(DIRECTORY ${INCLUDE_DIR} DESTINATION include)
