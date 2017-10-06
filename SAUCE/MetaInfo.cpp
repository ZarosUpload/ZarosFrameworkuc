

#include "MetaInfo.h"
#include "Utilities.h"

void PrintMetaHeader()
{
	printf("                                  Zaros");
	Utilities::SetConsoleColor(FOREGROUND_INTENSE_GREEN);
	printf("Ware\n");
	Utilities::SetConsoleColor(FOREGROUND_WHITE);
	Utilities::Log("Build %s", __DATE__);
	Utilities::Log("Setting Up Zaros Framework for %s", AYYWARE_META_GAME);
}