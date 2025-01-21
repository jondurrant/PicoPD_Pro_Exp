
if (NOT DEFINED SSD1306_DIR)
    set(SSD1306_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/pico-ssd1306")
endif()


add_library(ssd1306 STATIC 
	${SSD1306_DIR}/ssd1306.c
)


# Add include directory
target_include_directories(ssd1306 PUBLIC 
   ${SSD1306_DIR}
)

# Add the standard library to the build
target_link_libraries(ssd1306 PUBLIC 
	pico_stdlib
	hardware_i2c
	)

