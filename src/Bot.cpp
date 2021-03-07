#include "Bot.h"

bool Bot::init()
{
	if (!load_config())
	{
		std::cout << "Loading Config Failed, check Config File" << std::endl;
		return false;
	}

	if (!load_offsets())
	{
		std::cout << "Loading Offsets Failed, check Offsets File" << std::endl;
		return false;
	}

	if (!mem_reader.attach_to_process(windowname.c_str()))
	{
		std::cout << "Couldn't find process: Quiting " << std::endl;
		std::cout << "Make Sure CS-GO is open " << std::endl;
		return false;
	}

	client_dll_address = mem_reader.get_module_address(client_dll_name.c_str());
	return client_dll_address != NULL;
}

bool Bot::load_config()
{
	windowname = FileReader::read_value_of_string_in_file("config.txt", "windowName");
	client_dll_name = FileReader::read_value_of_string_in_file("config.txt", "client Dll Name");
	delay = stol(FileReader::read_value_of_string_in_file("config.txt", "delay"), NULL, DEC);
	unsigned int trigger_button = stol(FileReader::read_value_of_string_in_file("config.txt", "triggerButton"), NULL, HEX);

	if (windowname == "" || client_dll_name == "" || trigger_button == 0)
		return false;

	button.set_toggle_button(trigger_button);

	return true;
}

bool Bot::load_offsets()
{
	local_player_offset = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "localPlayer"), NULL, HEX);
	crosshair_offset = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "crosshairOffset"), NULL, HEX);
	entity_list_start_offset = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "entityBase"), NULL, HEX);
	player_health_offset = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "PlayerHealth"), NULL, HEX);
	team_offset = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "teamOffset"), NULL, HEX);
	entity_listelement_size = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "EntLoopDist"), NULL, HEX);

	if (local_player_offset == 0 || crosshair_offset == 0 ||
		player_health_offset == 0 || team_offset == 0 || entity_listelement_size == 0)
		return false;

	return true;
}

void Bot::run() 
{
	while (true) 
	{
		button.update();

		if (button.is_on())
			update_triggerlogic();
	}
}

void Bot::update_triggerlogic()
{
	DWORD local_player = mem_reader.read_memory<DWORD>(client_dll_address + local_player_offset);
	int local_team = mem_reader.read_memory<int>(local_player + team_offset);
	int cross_hair_ID = mem_reader.read_memory<int>(local_player + crosshair_offset);

	if (cross_hair_ID <= 0 && cross_hair_ID > 300)
		return;

	DWORD enemy_in_crosshair = mem_reader.read_memory<DWORD>(
		client_dll_address + entity_list_start_offset + ((cross_hair_ID - 1) * entity_listelement_size));
	int enemy_health = mem_reader.read_memory<int>(enemy_in_crosshair + player_health_offset);
	int enemy_team = mem_reader.read_memory<int>(enemy_in_crosshair + team_offset);

	if ((local_team != enemy_team) && (enemy_health < 200) && (enemy_health > 0))
	{
		simulate_left_mouse_button_click();
		Sleep(delay);
	}
}

void Bot::simulate_left_mouse_button_click()
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
	mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
}
