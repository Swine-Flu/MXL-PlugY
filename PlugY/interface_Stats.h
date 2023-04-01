/*=================================================================
	File created by Yohann NICOLAS.

	Interface Stats functions

=================================================================*/
#pragma once

#include "common.h"

void Install_InterfaceStats();

void STDCALL printStatsPageBtns();
DWORD STDCALL statsPageMouseDown(sWinMessage* msg);
DWORD STDCALL statsPageMouseUp(sWinMessage* msg);

/*================================= END OF FILE =================================*/
