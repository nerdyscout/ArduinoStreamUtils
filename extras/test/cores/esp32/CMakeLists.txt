# StreamUtils - github.com/bblanchon/ArduinoStreamUtils
# Copyright Benoit Blanchon 2019-2022
# MIT License

add_library(Esp32Core
	EEPROM.cpp
	EEPROM.h
	Stream.h
)

target_include_directories(Esp32Core
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/../avr
)

target_compile_definitions(Esp32Core
	PUBLIC
		ARDUINO_ARCH_ESP32
		ESP_ARDUINO_VERSION_MAJOR=2
		ESP_ARDUINO_VERSION_MINOR=0
		ESP_ARDUINO_VERSION_PATCH=5
)
