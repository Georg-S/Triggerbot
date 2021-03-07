cmake_minimum_required (VERSION 3.8)
project(TriggerBot)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set (HEADERS
	"include/Bot.h"
	"include/FileReader.h"
	"include/ToggleButton.h"
	"include/MemoryReader.h"
	)

set(SOURCES 
	"src/Bot.cpp"
	"src/FileReader.cpp"
	"src/ToggleButton.cpp"
	"src/main.cpp"
	"src/MemoryReader.cpp"
	)


add_executable(TriggerBot ${SOURCES} ${HEADERS})
target_include_directories(TriggerBot PUBLIC "include")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TriggerBot)