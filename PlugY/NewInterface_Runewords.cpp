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

#define NB_RUNES_PER_PAGE		12

#define	getXCloseBtn()			RX(482)
#define	getLCloseBtn()			24
#define	getYCloseBtn()			RY(738)
#define	getHCloseBtn()			24
#define isOnCloseBtn(x,y)		isOnRect(x, y, getXCloseBtn(), getYCloseBtn(), getLCloseBtn(), getHCloseBtn())

#define	getXNextPageBtn()		RX(240)
#define	getLNextPageBtn()		32
#define	getYNextPageBtn()		RY(104)
#define	getHNextPageBtn()		32
#define isOnNextPageBtn(x,y)	isOnRect(x, y, getXNextPageBtn(), getYNextPageBtn(), getLNextPageBtn(), getHNextPageBtn())

#define	getXPrevRunesBtn()		RX(384)
#define	getLPrevRunesBtn()		32
#define	getYPrevRunesBtn()		RY(104)
#define	getHPrevRunesBtn()		32
#define isOnPrevRunesBtn(x,y)	isOnRect(x, y, getXPrevRunesBtn(), getYPrevRunesBtn(), getLPrevRunesBtn(), getHPrevRunesBtn())

#define	getXNextRunesBtn()		RX(426)
#define	getLNextRunesBtn()		32
#define	getYNextRunesBtn()		RY(104)
#define	getHNextRunesBtn()		32
#define isOnNextRunesBtn(x,y)	isOnRect(x, y, getXNextRunesBtn(), getYNextRunesBtn(), getLNextRunesBtn(), getHNextRunesBtn())

int curRunesPage=0;
int maxRunesPage=0xFFFF;

static struct
{
	union{
		DWORD all;
		struct{
			DWORD close:1;
			DWORD nextPage:1;
			DWORD prevRunes:1;
			DWORD nextRunes:1;
		};
	};
} isDownBtn;


void printRuneword(RunesBIN* runesData, DWORD pos)
{
	WCHAR temp[50];
	WCHAR runesList[50];
	WCHAR typesList[50];
	WCHAR typesEList[50];

	LPWSTR lpText = D2GetStringFromIndex(runesData->RuneNameID);
	if (LPWSTR lpName = wcsrchr(lpText, L'\n')) lpText = lpName++;
	D2SetFont(1);
	DWORD nbPixel = D2GetPixelLen(lpText);
	D2PrintString(lpText, 256 - nbPixel / 2, 65 + pos * 48, GOLD, 0);

	typesList[0] = L'\0';
	DWORD numItype = 0;
	while (numItype < 6)
	{
		WORD type = runesData->Itypes[numItype];
		if (!type) break;
		ItemTypesBIN* itemTypeData = D2GetItemTypesBIN(type);
		LPCWSTR typeName = getLocalTypeString(itemTypeData->code);
		if (numItype) wcscat(typesList, L"/");
		wcscat(typesList, typeName);
		numItype++;
	}
	D2SetFont(4);
	nbPixel = D2GetPixelLen(typesList);
	DWORD x1 = nbPixel > 216 ? 40 : 148 - nbPixel / 2;
	D2PrintString(typesList, x1, 80 + pos * 48, BLUE, 0);

	typesEList[0] = L'\0';
	DWORD numEtype = 0;
	while (numEtype < 3)
	{
		WORD type = runesData->Etypes[numEtype];
		if (!type) break;
		ItemTypesBIN* itemTypeData = D2GetItemTypesBIN(type);
		LPCWSTR typeName = getLocalTypeString(itemTypeData->code);
		if (numEtype) wcscat(typesEList, L"/");
		wcscat(typesEList, typeName);
		numEtype++;
	}
	D2SetFont(4);
	nbPixel = D2GetPixelLen(typesEList);
	x1 = nbPixel > 216 ? 40 : 148 - nbPixel / 2;
	D2PrintString(typesEList, x1, 92 + pos * 48, RED, 0);

	runesList[0] = L'\0';
	DWORD numRune = 0;
	int curRuneID = runesData->Runes[0];
	while ((curRuneID > 0) && (numRune < 6))
	{
		ItemsBIN* itemsData = D2GetItemsBIN(curRuneID);
		d2_assert(!itemsData, "itemsData", __FILE__, __LINE__);
		GemsBIN* gemData = D2GetGemsBIN(itemsData->GemOffset);
		if (gemData) {
			d2_assert(!gemData, "gemData", __FILE__, __LINE__);
			mbstowcs(temp, gemData->letter, 50);
			if (numRune) wcscat(runesList, L" ");
//		else wcscat(runesList,L"");
			wcscat(runesList, temp);
		}
		numRune++;
		curRuneID = runesData->Runes[numRune];
	}
//	wcscat(runesList,L"");

	D2SetFont(8);
	DWORD y1;
	nbPixel = D2GetPixelLen(runesList);
	if (nbPixel > 216)
	{
		D2SetFont(0);
		nbPixel = D2GetPixelLen(runesList);
		x1 = nbPixel > 216 ? 512 - 40 - nbPixel : 256 + 108 - nbPixel / 2;
		y1 = 82;
	}
	else
	{
		x1 = 256 + 108 - nbPixel / 2;
		y1 = 84;
	}
	D2PrintString(runesList, x1, y1 + pos * 48, WHITE, 0);

	D2FillArea(60, 94 + pos * 48, 452, 95 + pos * 48, 22, 5);
}


#define BUFSIZE 0x80
//6FB21FAA
void STDCALL printRunewordsPage()
{
	LPWSTR lpText;
	bDontPrintBorder = true;

	//Init data for print image
	sDrawImageInfo data;
	ZeroMemory(&data, sizeof(data));

	//print background
	fillRect(0, 0, RX(512), RY(54), 0, 5);
	setImage(&data, gpStatsPageBackgroundGfx);
	setFrame(&data, 0);
	D2PrintImage(&data, RX(0), RY(512), -1, 5, 0);
	setFrame(&data, 1);
	D2PrintImage(&data, RX(256), RY(512), -1, 5, 0);
	setFrame(&data, 2);
	D2PrintImage(&data, RX(0), RY(256), -1, 5, 0);
	setFrame(&data, 3);
	D2PrintImage(&data, RX(256), RY(256), -1, 5, 0);
	setFrame(&data, 4);
	D2PrintImage(&data, RX(0), RY(64), -1, 5, 0);
	setFrame(&data, 5);
	D2PrintImage(&data, RX(256), RY(64), -1, 5, 0);

	setImage(&data, *ptStatsFrameGfx);
	setFrame(&data, 0);
	D2PrintImage(&data, getXPrevRunesBtn() - 2, getYPrevRunesBtn() + 2, -1, 5, 0);
	D2PrintImage(&data, getXNextRunesBtn() - 2, getYNextRunesBtn() + 2, -1, 5, 0);

	//print button close
	setImage(&data, gpClosePageButtonGfx);
	setFrame(&data, isDownBtn.close);
	D2PrintImage(&data, getXCloseBtn(), getYCloseBtn(), -1, 5, 0);

	//print next page button
//	setImage(&data, D2LoadBuySelBtn());
//	setFrame(&data, isDownBtn.nextPage);
//	D2PrintImage(&data, getXNextPageBtn(), getYNextPageBtn(), -1, 5, 0);

	//print previous runes button
	setImage(&data, stashBtnsImages);
	setFrame(&data, isDownBtn.prevRunes);
	D2PrintImage(&data, getXPrevRunesBtn(), getYPrevRunesBtn(), -1, 5, 0);

	//print previous runes button
	setFrame(&data, 2 + isDownBtn.nextRunes);
	D2PrintImage(&data, getXNextRunesBtn(), getYNextRunesBtn(), -1, 5, 0);

	D2SetFont(6);

	//146;//B400
	int nbRunes = *D2GetNbRunesBIN();
	RunesBIN* runesFirst = nbRunes > 1 ? D2GetRunesBIN(1) - 1 : NULL;
	RunesBIN* runesLast = runesFirst ? runesFirst + nbRunes : NULL;
//	log_msg("nbRunes(%d,%d) runesFirst(%08X, %08X) runesLast(%08X, %08X)",nbRunes,SgptDataTables->nbRunes,runesFirst, SgptDataTables->runes,runesLast,SgptDataTables->runes + SgptDataTables->nbRunes);

	int nbRunesCompleted = 0;
	DWORD curNbRunes = 0;
	for (RunesBIN* runesData = runesFirst; runesData < runesLast; runesData++)
	{
		if (!runesData->Complete || runesData->Server) continue;
		nbRunesCompleted++;
		if ((curRunesPage * NB_RUNES_PER_PAGE < nbRunesCompleted) &&
			(nbRunesCompleted <= (curRunesPage + 1) * NB_RUNES_PER_PAGE))
			printRuneword(runesData, curNbRunes++);
	}
	maxRunesPage = nbRunesCompleted ? (nbRunesCompleted - 1) / NB_RUNES_PER_PAGE : 0;


//////////////////// POPUP PRINTING ////////////////////
	//set MouseX & MouseY
	DWORD x = D2GetMouseX();
	DWORD y = D2GetMouseY();

	D2SetFont(1);
//	if (isOnNextPageBtn(x,y))	// print popup "next page"
//	{
//		lpText = getLocalString(STR_NEXT_PAGE);
//		D2PrintPopup(lpText, getXNextPageBtn()+getLNextPageBtn()/2, getYNextPageBtn()-getHNextPageBtn(), WHITE, 1);
//	}
	if (isOnPrevRunesBtn(x, y))
	{
		lpText = getLocalString(STR_PREVIOUS_PAGE);
		D2PrintPopup(lpText, getXPrevRunesBtn() + getLPrevRunesBtn() / 2, getYPrevRunesBtn() - getHPrevRunesBtn(), WHITE, 1);
	}
	else if (isOnNextRunesBtn(x, y))
	{
		lpText = getLocalString(STR_NEXT_PAGE);
		D2PrintPopup(lpText, getXNextRunesBtn() + getLNextRunesBtn() / 2, getYNextRunesBtn() - getHNextRunesBtn(), WHITE, 1);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////

DWORD STDCALL mouseRunewordsPageLeftDown(sWinMessage* msg)
{
	if (isOnCloseBtn(msg->x, msg->y))
	{
		log_msg("push down left button close\n");
		isDownBtn.close = 1;
		D2PlaySound(4, 0, 0, 0, 0);
	}
	else if (isOnNextPageBtn(msg->x, msg->y))
	{
		log_msg("push down left button next page\n");
		isDownBtn.nextPage = 1;
		D2PlaySound(4, 0, 0, 0, 0);
	}
	else if (isOnPrevRunesBtn(msg->x, msg->y))
	{
		log_msg("push down left button prev page\n");
		isDownBtn.prevRunes = 1;
		D2PlaySound(4, 0, 0, 0, 0);
	}
	else if (isOnNextRunesBtn(msg->x, msg->y))
	{
		log_msg("push down left button next runes\n");
		isDownBtn.nextRunes = 1;
		D2PlaySound(4, 0, 0, 0, 0);
	}

	freeMessage(msg);
	return 0;
}


DWORD STDCALL mouseRunewordsPageLeftUp(sWinMessage* msg)
{
	if (isOnCloseBtn(msg->x, msg->y))
	{
		log_msg("push up left button close\n");
		if (isDownBtn.close)
			D2TogglePage(2, 1, 0);
	}
	else if (isOnNextPageBtn(msg->x, msg->y))
	{
		log_msg("push up left button next page\n");
		if (isDownBtn.nextPage)
		{
			GoStatPage(GetCurrentPage() + 1);
			log_msg("next page press\n");
		}
	}
	else if (isOnPrevRunesBtn(msg->x, msg->y))
	{
		log_msg("push up left button prev runes\n");
		if (isDownBtn.prevRunes)
		{
			if (curRunesPage) curRunesPage--;
		}
	}
	else if (isOnNextRunesBtn(msg->x, msg->y))
	{
		log_msg("push up left button next runes\n");
		if (isDownBtn.nextRunes)
		{
			if (curRunesPage < maxRunesPage) curRunesPage++;
		}
	}

	freeMessage(msg);
	isDownBtn.all = 0;
	return 0;
}


/*================================= END OF FILE =================================*/