#include <iostream>
#include "Bot.h"
#include "FileReader.h"
#include "ToggleButton.h"

int main()
{
	Bot bot = Bot();
	if(bot.init())
		bot.run();

	getchar();
    return 0;
}

