/*=================================================================
	File created by Yohann NICOLAS.

	Interface functions

=================================================================*/

#include "updateServer.h"	// Install_UpdateServer()
#include "plugYFiles.h"		// Install_PlugYImagesFiles()
#include "statsPoints.h"
#include "newInterfaces.h"
#include "common.h"
#include <stdio.h>


#define	getXCloseBtn()				RX(482)
#define	getLCloseBtn()				24
#define	getYCloseBtn()				RY(738)
#define	getHCloseBtn()				24
#define isOnCloseBtn(x,y)			isOnRect(x, y, getXCloseBtn(), getYCloseBtn(), getLCloseBtn(), getHCloseBtn())

#define	getXPreviousPageBtn()		RX(384)
#define	getLPreviousPageBtn()		32
#define	getYPreviousPageBtn()		RY(104)
#define	getHPreviousPageBtn()		32
#define isOnPreviousPageBtn(x,y)	isOnRect(x, y, getXPreviousPageBtn(), getYPreviousPageBtn(), getLPreviousPageBtn(), getHPreviousPageBtn())

#define	getXNextPageBtn()			RX(426)
#define	getLNextPageBtn()			32
#define	getYNextPageBtn()			RY(104)
#define	getHNextPageBtn()			32
#define isOnNextPageBtn(x,y)		isOnRect(x, y, getXNextPageBtn(), getYNextPageBtn(), getLNextPageBtn(), getHNextPageBtn())


#define	getLAssignBtn()				32
#define	getHAssignBtn()				32

#define	getXAssSTRBtn()				RX(208)
#define	getYAssSTRBtn()				RY(542)
#define isOnAssSTRBtn(x,y)			isOnRect(x, y, getXAssSTRBtn(), getYAssSTRBtn(), getLAssignBtn(), getHAssignBtn())

#define	getXAssDEXBtn()				RX(208)
#define	getYAssDEXBtn()				RY(466)
#define isOnAssDEXBtn(x,y)			isOnRect(x, y, getXAssDEXBtn(), getYAssDEXBtn(), getLAssignBtn(), getHAssignBtn())

#define	getXAssVITBtn()				RX(208)
#define	getYAssVITBtn()				RY(390)
#define isOnAssVITBtn(x,y)			isOnRect(x, y, getXAssVITBtn(), getYAssVITBtn(), getLAssignBtn(), getHAssignBtn())

#define	getXAssENEBtn()				RX(208)
#define	getYAssENEBtn()				RY(314)
#define isOnAssENEBtn(x,y)			isOnRect(x, y, getXAssENEBtn(), getYAssENEBtn(), getLAssignBtn(), getHAssignBtn())


#define	getLUnassignBtn()			getLAssignBtn()
#define	getHUnassignBtn()			getHAssignBtn()

#define	getXUnaSTRBtn()				getXAssSTRBtn()
#define	getYUnaSTRBtn()				getYAssSTRBtn()
#define isOnUnaSTRBtn(x,y)			isOnRect(x, y, getXUnaSTRBtn(), getYUnaSTRBtn(), getLUnassignBtn(), getHUnassignBtn())

#define	getXUnaDEXBtn()				getXAssDEXBtn()
#define	getYUnaDEXBtn()				getYAssDEXBtn()
#define isOnUnaDEXBtn(x,y)			isOnRect(x, y, getXUnaDEXBtn(), getYUnaDEXBtn(), getLUnassignBtn(), getHUnassignBtn())

#define	getXUnaVITBtn()				getXAssVITBtn()
#define	getYUnaVITBtn()				getYAssVITBtn()
#define isOnUnaVITBtn(x,y)			isOnRect(x, y, getXUnaVITBtn(), getYUnaVITBtn(), getLUnassignBtn(), getHUnassignBtn())

#define	getXUnaENEBtn()				getXAssENEBtn()
#define	getYUnaENEBtn()				getYAssENEBtn()
#define isOnUnaENEBtn(x,y)			isOnRect(x, y, getXUnaENEBtn(), getYUnaENEBtn(), getLUnassignBtn(), getHUnassignBtn())



void** ptD2RemainingStatsPointsBoxImages = (void**)0x6FBB5E50;
#define D2RemainingStatsPointsBoxImages (*ptD2RemainingStatsPointsBoxImages)

void** ptD2AssignStatsPointsBoxImages = (void**)0x6FBB5BB8;
#define D2AssignStatsPointsBoxImages (*ptD2AssignStatsPointsBoxImages)

void** ptD2AssignStatsPointsBtnImages = (void**)0x6FBB5BB4;
#define D2AssignStatsPointsBtnImages (*ptD2AssignStatsPointsBtnImages)



static struct
{
	union{
		DWORD all;
		struct{
			DWORD close:1;
			DWORD AssSTR:1;
			DWORD AssDEX:1;
			DWORD AssVIT:1;
			DWORD AssENE:1;
			DWORD UnaSTR:1;
			DWORD UnaDEX:1;
			DWORD UnaVIT:1;
			DWORD UnaENE:1;
			DWORD previousPage:1;
			DWORD nextPage:1;
		};
	};
} isDownBtn;


void print2Lines(WORD id, LPWSTR lpText, DWORD x, DWORD l, DWORD y)
{
	wcsncpy(lpText, D2GetStringFromIndex(id), 0x63);
	lpText[0x64]= L'\0';
	
	LPWSTR lptmp = lpText;
	while (*lptmp)
	{
		if (*lptmp==L'\n')
		{
			*lptmp = L'\0';
			if (*(++lptmp))
			{
				DWORD nbPixel = D2GetPixelLen(lpText);
				D2PrintString(lpText, MILIEU(x,l,nbPixel), y-4, WHITE, 0);
				nbPixel = D2GetPixelLen(lptmp);
				D2PrintString(lptmp, MILIEU(x,l,nbPixel), y+4, WHITE, 0);
				return;
			}
			break;
		}
		lptmp++;
	}
	DWORD nbPixel = D2GetPixelLen(lpText);
	D2PrintString(lpText, MILIEU(x,l,nbPixel), y, WHITE, 0);
}


#define BUFSIZE 0x80
void STDCALL printNewStatsPage()
{
	WCHAR text[100];
	Unit* ptChar = D2GetClientPlayer();

	d2_assert(!ptChar, "Printing stats page : no character selected", __FILE__, __LINE__);
	d2_assert(ptChar->nUnitType != UNIT_PLAYER, "Printing stats page : bad unit type", __FILE__, __LINE__);

	//Init data for print image
	sDrawImageInfo data;
	ZeroMemory(&data, sizeof(data));

	//print background
	D2DrawStatsPageBackground();
	D2DrawStatsPageUiElements();

	//print button close
	setImage(&data, gpClosePageButtonGfx);
	setFrame(&data, isDownBtn.close);
	D2PrintImage(&data, getXCloseBtn(), getYCloseBtn(), -1, 5, 0);

	//new intefraces
	if (active_newInterfaces)
	{
		setImage(&data, *ptStatsFrameGfx);
		setFrame(&data, 0);
		D2PrintImage(&data, getXPreviousPageBtn() - 2, getYPreviousPageBtn() + 2, -1, 5, 0);
		D2PrintImage(&data, getXNextPageBtn() - 2, getYNextPageBtn() + 2, -1, 5, 0);

		//print previous page button
		setImage(&data, D2LoadBuySelBtn());
		setFrame(&data, 12 + isDownBtn.previousPage);
		D2PrintImage(&data, getXPreviousPageBtn(), getYPreviousPageBtn(), -1, 5, 0);

		//print next page button
		setFrame(&data, 14 + isDownBtn.nextPage);
		D2PrintImage(&data, getXNextPageBtn(), getYNextPageBtn(), -1, 5, 0);
	}

//////////////////// STAT POINTS REMAINING MANAGEMENT ////////////////////
	//print Assign box of btns : Str,Dex,Ene,Vit
	setImage(&data, *ptStatsFrameGfx);
	setFrame(&data, 0);
	D2PrintImage(&data, getXAssSTRBtn() - 3, getYAssSTRBtn() + 4, -1, 5, 0);
	D2PrintImage(&data, getXAssDEXBtn() - 3, getYAssDEXBtn() + 4, -1, 5, 0);
	D2PrintImage(&data, getXAssENEBtn() - 3, getYAssENEBtn() + 4, -1, 5, 0);
	D2PrintImage(&data, getXAssVITBtn() - 3, getYAssVITBtn() + 4, -1, 5, 0);

	DWORD nbStatPointsRemaining = D2GetPlayerBaseStat(ptChar, STATS_STATPTS, 0);
	CharStatsBIN* charStats = D2GetCharStatsBIN(ptChar->nPlayerClass);

	if (!(GetKeyState(keyUsedForUnassignStatPoint) < 0))
	{
		setImage(&data, *ptStatsButtonGfx);
		setFrame(&data, (nbStatPointsRemaining > 0 ? isDownBtn.AssSTR : 2));
		D2PrintImage(&data, getXAssSTRBtn(), getYAssSTRBtn(), -1, 5, 0);
		setFrame(&data, (nbStatPointsRemaining > 0 ? isDownBtn.AssDEX : 2));
		D2PrintImage(&data, getXAssDEXBtn(), getYAssDEXBtn(), -1, 5, 0);
		setFrame(&data, (nbStatPointsRemaining > 0 ? isDownBtn.AssVIT : 2));
		D2PrintImage(&data, getXAssVITBtn(), getYAssVITBtn(), -1, 5, 0);
		setFrame(&data, (nbStatPointsRemaining > 0 ? isDownBtn.AssENE : 2));
		D2PrintImage(&data, getXAssENEBtn(), getYAssENEBtn(), -1, 5, 0);
	}
	else
	{
		setImage(&data, unassignStatsBtnImages);
		setFrame(&data, ((int)charStats->baseSTR < D2GetPlayerBaseStat(ptChar, STATS_STRENGTH, 0) ? isDownBtn.UnaSTR : 2));
		D2PrintImage(&data, getXUnaSTRBtn(), getYUnaSTRBtn(), -1, 5, 0);
		setFrame(&data, ((int)charStats->baseDEX < D2GetPlayerBaseStat(ptChar, STATS_DEXTERITY, 0) ? isDownBtn.UnaDEX : 2));
		D2PrintImage(&data, getXUnaDEXBtn(), getYUnaDEXBtn(), -1, 5, 0);
		setFrame(&data, ((int)charStats->baseVIT < D2GetPlayerBaseStat(ptChar, STATS_VITALITY, 0) ? isDownBtn.UnaVIT : 2));
		D2PrintImage(&data, getXUnaVITBtn(), getYUnaVITBtn(), -1, 5, 0);
		setFrame(&data, ((int)charStats->baseENE < D2GetPlayerBaseStat(ptChar, STATS_ENERGY, 0) ? isDownBtn.UnaENE : 2));
		D2PrintImage(&data, getXUnaENEBtn(), getYUnaENEBtn(), -1, 5, 0);
	}

//////////////////// POPUP PRINTING ////////////////////
	//set MouseX & MouseY
	DWORD x = D2GetMouseX();
	DWORD y = D2GetMouseY();

	if (!active_StatsShiftClickLimit)
		_snwprintf(text, _countof(text), getLocalString(STR_STATS_UNASSIGN_WITHOUT_LIMIT));
	else
		_snwprintf(text, _countof(text), getLocalString(STR_STATS_UNASSIGN_WITH_LIMIT), limitValueToShiftClick);
	wcscat(text, L"\n");

	D2SetFont(1);
	if (active_newInterfaces && isOnPreviousPageBtn(x, y))	//print popup "previous page"
	{
		LPWSTR lpText = getLocalString(STR_PREVIOUS_PAGE);
		D2PrintPopup(lpText, getXPreviousPageBtn() + getLPreviousPageBtn() / 2, getYPreviousPageBtn() - getHPreviousPageBtn(), WHITE, 1);
	}
	else if (active_newInterfaces && isOnNextPageBtn(x, y))	//print popup "next page"
	{
		LPWSTR lpText = getLocalString(STR_NEXT_PAGE);
		D2PrintPopup(lpText, getXNextPageBtn() + getLNextPageBtn() / 2, getYNextPageBtn() - getHNextPageBtn(), WHITE, 1);
	}
	else if (isOnAssSTRBtn(x, y))
	{
		int len = wcslen(text);
		_snwprintf(text + len, _countof(text) - len, getLocalString(STR_STATS_BASE_MIN), D2GetPlayerBaseStat(ptChar, STATS_STRENGTH, 0), (int)charStats->baseSTR);
		D2PrintPopup(text, getXAssSTRBtn() + getLAssignBtn() / 2, getYAssSTRBtn() - getHAssignBtn(), WHITE, 1);
	}
	else if (isOnAssDEXBtn(x, y))
	{
		int len = wcslen(text);
		_snwprintf(text + len, _countof(text) - len, getLocalString(STR_STATS_BASE_MIN), D2GetPlayerBaseStat(ptChar, STATS_DEXTERITY, 0), (int)charStats->baseDEX);
		D2PrintPopup(text, getXAssDEXBtn() + getLAssignBtn() / 2, getYAssDEXBtn() - getHAssignBtn(), WHITE, 1);
	}
	else if (isOnAssVITBtn(x, y))
	{
		int len = wcslen(text);
		_snwprintf(text + len, _countof(text) - len, getLocalString(STR_STATS_BASE_MIN), D2GetPlayerBaseStat(ptChar, STATS_VITALITY, 0), (int)charStats->baseVIT);
		D2PrintPopup(text, getXAssVITBtn() + getLAssignBtn() / 2, getYAssVITBtn() - getHAssignBtn(), WHITE, 1);
	}
	else if (isOnAssENEBtn(x, y))
	{
		int len = wcslen(text);
		_snwprintf(text + len, _countof(text) - len, getLocalString(STR_STATS_BASE_MIN), D2GetPlayerBaseStat(ptChar, STATS_ENERGY, 0), (int)charStats->baseENE);
		D2PrintPopup(text, getXAssENEBtn() + getLAssignBtn() / 2, getYAssENEBtn() - getHAssignBtn(), WHITE, 1);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////

DWORD STDCALL mouseNewStatsPageLeftDown(sWinMessage* msg)
{
	Unit* ptChar = D2GetClientPlayer();
	DWORD nbStatPointsRemaining = D2GetPlayerBaseStat(ptChar, STATS_STATPTS, 0);
	CharStatsBIN* charStats = D2GetCharStatsBIN(ptChar->nPlayerClass);

	if (isOnCloseBtn(msg->x, msg->y))
	{
		log_msg("push down left button close\n");
		isDownBtn.close = 1;
		D2PlaySound(4, 0, 0, 0, 0);
	}
	else if (active_newInterfaces && isOnPreviousPageBtn(msg->x, msg->y))
	{
		log_msg("push down left button previous page\n");
		isDownBtn.previousPage = 1;
		D2PlaySound(4, 0, 0, 0, 0);
	}
	else if (active_newInterfaces && isOnNextPageBtn(msg->x, msg->y))
	{
		log_msg("push down left button next page\n");
		isDownBtn.nextPage = 1;
		D2PlaySound(4, 0, 0, 0, 0);
	}
	else if (!(GetKeyState(keyUsedForUnassignStatPoint) < 0))
	{
		if (isOnAssSTRBtn(msg->x, msg->y) && (nbStatPointsRemaining > 0))
		{
			log_msg("push down left button assign strengh\n");
			isDownBtn.AssSTR = 1;
			D2PlaySound(5, 0, 0, 0, 0);
		}
		else if (isOnAssDEXBtn(msg->x, msg->y) && (nbStatPointsRemaining > 0))
		{
			log_msg("push down left button assign dexterity\n");
			isDownBtn.AssDEX = 1;
			D2PlaySound(5, 0, 0, 0, 0);
		}
		else if (isOnAssVITBtn(msg->x, msg->y) && (nbStatPointsRemaining > 0))
		{
			log_msg("push down left button assign vitality\n");
			isDownBtn.AssVIT = 1;
			D2PlaySound(5, 0, 0, 0, 0);
		}
		else if (isOnAssENEBtn(msg->x, msg->y) && (nbStatPointsRemaining > 0))
		{
			log_msg("push down left button assign energy\n");
			isDownBtn.AssENE = 1;
			D2PlaySound(5, 0, 0, 0, 0);
		}
	}
	else
	{
		if (isOnUnaSTRBtn(msg->x, msg->y) && ((int)charStats->baseSTR < D2GetPlayerBaseStat(ptChar, STATS_STRENGTH, 0)))
		{
			log_msg("push down left button unassign strengh\n");
			isDownBtn.UnaSTR = 1;
			D2PlaySound(5, 0, 0, 0, 0);
		}
		else if (isOnUnaDEXBtn(msg->x, msg->y) && ((int)charStats->baseDEX < D2GetPlayerBaseStat(ptChar, STATS_DEXTERITY, 0)))
		{
			log_msg("push down left button unassign dexterity\n");
			isDownBtn.UnaDEX = 1;
			D2PlaySound(5, 0, 0, 0, 0);
		}
		else if (isOnUnaVITBtn(msg->x, msg->y) && ((int)charStats->baseVIT < D2GetPlayerBaseStat(ptChar, STATS_VITALITY, 0)))
		{
			log_msg("push down left button unassign vitality\n");
			isDownBtn.UnaVIT = 1;
			D2PlaySound(5, 0, 0, 0, 0);
		}
		else if (isOnUnaENEBtn(msg->x, msg->y) && ((int)charStats->baseENE < D2GetPlayerBaseStat(ptChar, STATS_ENERGY, 0)))
		{
			log_msg("push down left button unassign energy\n");
			isDownBtn.UnaENE = 1;
			D2PlaySound(5, 0, 0, 0, 0);
		}
	}

	freeMessage(msg);
	return 0;
}


void sendAssignStats(DWORD code, DWORD nbStatPointsRemaining)
{
	DWORD nbPoints = GetKeyState(VK_SHIFT)<0? nbStatPointsRemaining : 1;
	if (active_StatsShiftClickLimit && (nbPoints>limitValueToShiftClick)) 
		 nbPoints = limitValueToShiftClick;
	while ( nbPoints>0 )
	{
		DWORD nbTemp = nbPoints > 0x20 ? 0x20 : nbPoints;
		D2SendToServer3(0x3A, (WORD)(code + (nbTemp-1)*256));
		nbPoints -= nbTemp;
	}
}

DWORD STDCALL mouseNewStatsPageLeftUp(sWinMessage* msg)
{
	Unit* ptChar = D2GetClientPlayer();
	DWORD nbStatPointsRemaining = D2GetPlayerBaseStat(ptChar, STATS_STATPTS, 0);
	CharStatsBIN* charStats = D2GetCharStatsBIN(ptChar->nPlayerClass);

	if (isOnCloseBtn(msg->x, msg->y))
	{
		log_msg("push up left button close\n");
		if (isDownBtn.close)
			D2TogglePage(2, 1, 0);
	}
	else if (active_newInterfaces && isOnPreviousPageBtn(msg->x, msg->y))
	{
		log_msg("push up left button previous page\n");
		if (isDownBtn.previousPage)
		{
			GoPreviousStatPage();
			log_msg("previous page press\n");
		}
	}
	else if (active_newInterfaces && isOnNextPageBtn(msg->x, msg->y))
	{
		log_msg("push up left button next page\n");
		if (isDownBtn.nextPage)
		{
			GoNextStatPage();
			log_msg("next page press\n");
		}
	}
	else if (!(GetKeyState(keyUsedForUnassignStatPoint) < 0))
	{
		if (isOnAssSTRBtn(msg->x, msg->y) && (nbStatPointsRemaining > 0))
		{
			log_msg("push up left button assign strengh\n");
			if (isDownBtn.AssSTR)
				D2SendToServer3(0x3A, (GetKeyState(VK_SHIFT) & 0x8000) | STATS_STRENGTH);
		}
		else if (isOnAssDEXBtn(msg->x, msg->y) && (nbStatPointsRemaining > 0))
		{
			log_msg("push up left button assign dexterity\n");
			if (isDownBtn.AssDEX)
				D2SendToServer3(0x3A, (GetKeyState(VK_SHIFT) & 0x8000) | STATS_DEXTERITY);
		}
		else if (isOnAssVITBtn(msg->x, msg->y) && (nbStatPointsRemaining > 0))
		{
			log_msg("push up left button assign vitality\n");
			if (isDownBtn.AssVIT)
				D2SendToServer3(0x3A, (GetKeyState(VK_SHIFT) & 0x8000) | STATS_VITALITY);
		}
		else if (isOnAssENEBtn(msg->x, msg->y) && (nbStatPointsRemaining > 0))
		{
			log_msg("push up left button assign energy\n");
			if (isDownBtn.AssENE)
				D2SendToServer3(0x3A, (GetKeyState(VK_SHIFT) & 0x8000) | STATS_ENERGY);
		}
	}
	else
	{
		if (isOnUnaSTRBtn(msg->x, msg->y) && ((int)charStats->baseSTR < D2GetPlayerBaseStat(ptChar, STATS_STRENGTH, 0)))
		{
			log_msg("push up left button unassign strengh\n");
			if (isDownBtn.UnaSTR)
				if (GetKeyState(VK_SHIFT) < 0)
					updateServer(US_UNASSIGN_STR_POINTS);
				else
					updateServer(US_UNASSIGN_STR_POINT);
		}
		else if (isOnUnaDEXBtn(msg->x, msg->y) && ((int)charStats->baseDEX < D2GetPlayerBaseStat(ptChar, STATS_DEXTERITY, 0)))
		{
			log_msg("push up left button unassign dexterity\n");
			if (isDownBtn.UnaDEX)
				if (GetKeyState(VK_SHIFT) < 0)
					updateServer(US_UNASSIGN_DEX_POINTS);
				else
					updateServer(US_UNASSIGN_DEX_POINT);
		}
		else if (isOnUnaVITBtn(msg->x, msg->y) && ((int)charStats->baseVIT < D2GetPlayerBaseStat(ptChar, STATS_VITALITY, 0)))
		{
			log_msg("push up left button unassign vitality\n");
			if (isDownBtn.UnaVIT)
				if (GetKeyState(VK_SHIFT) < 0)
					updateServer(US_UNASSIGN_VIT_POINTS);
				else
					updateServer(US_UNASSIGN_VIT_POINT);
		}
		else if (isOnUnaENEBtn(msg->x, msg->y) && ((int)charStats->baseENE < D2GetPlayerBaseStat(ptChar, STATS_ENERGY, 0)))
		{
			log_msg("push up left button unassign energy\n");
			if (isDownBtn.UnaENE)
				if (GetKeyState(VK_SHIFT) < 0)
					updateServer(US_UNASSIGN_ENE_POINTS);
				else
					updateServer(US_UNASSIGN_ENE_POINT);
		}
	}

	freeMessage(msg);
	isDownBtn.all = 0;
	return 0;
}


/*================================= END OF FILE =================================*/