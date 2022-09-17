cmake_minimum_required(VERSION 3.10)

set(VERSION 0.1)

project(lora VERSION ${VERSION})
set(TARGET lora)

# Source Files
set(lora_src
    ./src/rf95.c ./src/utils.c
    ./src/drivers/drivers.c
    ./src/drivers/wiringpi.c
)

# Headers file
set(lora_h
    ./include/rf95.h
    ./include/drivers.h
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
install(FILES ${lora_h} DESTINATION include)
