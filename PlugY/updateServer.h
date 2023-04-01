/*=================================================================
	File created by Yohann NICOLAS.

	Updating Server.

=================================================================*/
#pragma once

#include "common.h"

inline void updateServer(WORD p)
{
	if (!onRealm) D2SendToServer3(0x4A, p);//D2SendToServer3(0x3A, p);
}

void Install_UpdateServer();

/*================================= END OF FILE =================================*/