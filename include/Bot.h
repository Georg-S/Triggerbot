#pragma once
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <string>
#include <iomanip>
#include "MemoryReader.h"
#include "FileReader.h"
#include "ToggleButton.h"

class Bot
{
public:
	bool init();
	void run();

private:
	bool load_config();
	bool load_offsets();
	void update_triggerlogic();
	void simulate_left_mouse_button_click();

	ToggleButton button;
	MemoryReader mem_reader = MemoryReader();
	static constexpr int HEX = 16;
	static constexpr int DEC = 10;

	//Will be read from the Config file
	std::string client_dll_name;
	std::string  windowname;
	int delay = 0;

	//Will be read from the Offset file
	DWORD client_dll_address;
	DWORD local_player_offset;
	DWORD crosshair_offset;
	DWORD entity_list_start_offset;
	DWORD player_health_offset;
	DWORD team_offset;
	DWORD entity_listelement_size;
};

