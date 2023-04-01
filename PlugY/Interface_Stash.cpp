/*=================================================================
	File created by Yohann NICOLAS.
	Add support 1.13d by L'Autour.

	Interface stats page functions

=================================================================*/

#include "interface_Stash.h"
#include "updateServer.h"
#include "infinityStash.h"
#include "plugYFiles.h"		// Install_PlugYImagesFiles()
#include "common.h"
#include <stdio.h>

static struct
{
	union{
		DWORD all;
		struct{
			DWORD previous:1;
			DWORD next:1;
			DWORD toggleToSharedStash:1;
			DWORD previousIndex:1;
			DWORD nextIndex:1;
			DWORD putGold:1;
			DWORD takeGold:1;
		};
	};
} isDownBtn;

bool displaySharedSetItemNameInGreen = true;
int posXPreviousBtn=-1;
int posYPreviousBtn=-1;
int posXNextBtn=-1;
int posYNextBtn=-1;
int posXSharedBtn=-1;
int posYSharedBtn=-1;
int posXPreviousIndexBtn=-1;
int posYPreviousIndexBtn=-1;
int posXNextIndexBtn=-1;
int posYNextIndexBtn=-1;
int posXPutGoldBtn=-1;
int posYPutGoldBtn=-1;
int posXTakeGoldBtn=-1;
int posYTakeGoldBtn=-1;

#define	getXPreviousBtn()					RX(posXPreviousBtn < 0 ? 368 : posXPreviousBtn)
#define	getLPreviousBtn()					32
#define	getYPreviousBtn()					RY(posYPreviousBtn < 0 ? 150 : posYPreviousBtn)
#define	getHPreviousBtn()					32
#define isOnButtonPreviousStash(x,y)		isOnRect(x, y, getXPreviousBtn(), getYPreviousBtn(), getLPreviousBtn(), getHPreviousBtn())

#define	getXNextBtn()						RX(posXNextBtn < 0 ? 400 : posXNextBtn)
#define	getLNextBtn()						32
#define	getYNextBtn()						RY(posYNextBtn < 0 ? 150 : posYNextBtn)
#define	getHNextBtn()						32
#define isOnButtonNextStash(x,y)			isOnRect(x, y, getXNextBtn(), getYNextBtn(), getLNextBtn(), getHNextBtn())

#define	getXSharedBtn()						RX(posXSharedBtn < 0 ? 80 : posXSharedBtn)
#define	getLSharedBtn()						32
#define	getYSharedBtn()						RY(posYSharedBtn < 0 ? 150 : posYSharedBtn)
#define	getHSharedBtn()						32
#define isOnButtonToggleSharedStash(x,y)	isOnRect(x, y, getXSharedBtn(), getYSharedBtn(), getLSharedBtn(), getHSharedBtn())

#define	getXPreviousIndexBtn()				RX(posXPreviousIndexBtn < 0 ? 336 : posXPreviousIndexBtn)
#define	getLPreviousIndexBtn()				32
#define	getYPreviousIndexBtn()				RY(posYPreviousIndexBtn < 0 ? 150 : posYPreviousIndexBtn)
#define	getHPreviousIndexBtn()				32
#define isOnButtonPreviousIndexStash(x,y)	isOnRect(x, y, getXPreviousIndexBtn(), getYPreviousIndexBtn(), getLPreviousIndexBtn(), getHPreviousIndexBtn())

#define	getXNextIndexBtn()					RX(posXNextIndexBtn < 0 ? 432 : posXNextIndexBtn)
#define	getLNextIndexBtn()					32
#define	getYNextIndexBtn()					RY(posYNextIndexBtn < 0 ? 150 : posYNextIndexBtn)
#define	getHNextIndexBtn()					32
#define isOnButtonNextIndexStash(x,y)		isOnRect(x, y, getXNextIndexBtn(), getYNextIndexBtn(), getLNextIndexBtn(), getHNextIndexBtn())

#define	getXPutGoldBtn()					RX(posXPutGoldBtn < 0 ? 68 : posXPutGoldBtn)
#define	getLPutGoldBtn()					32
#define	getYPutGoldBtn()					RY(posYPutGoldBtn < 0 ? 714 : posYPutGoldBtn)
#define	getHPutGoldBtn()					32
#define isOnButtonPutGold(x,y)				isOnRect(x, y, getXPutGoldBtn(), getYPutGoldBtn(), getLPutGoldBtn(), getHPutGoldBtn())

#define	getXTakeGoldBtn()					RX(posXTakeGoldBtn < 0 ? 410 : posXTakeGoldBtn)
#define	getLTakeGoldBtn()					32
#define	getYTakeGoldBtn()					RY(posYTakeGoldBtn < 0 ? 714 : posYTakeGoldBtn)
#define	getHTakeGoldBtn()					32
#define isOnButtonTakeGold(x,y)				isOnRect(x, y, getXTakeGoldBtn(), getYTakeGoldBtn(), getLTakeGoldBtn(), getHTakeGoldBtn())

//closeBtn x: D2C=0x113 LOD=0x110(0->0x28)		y: D2C=0x41 LOD=0x40 (-0x23 -> 5)


void STDCALL printBtns()
{
	if (onRealm) return;

	Unit* ptChar = D2GetClientPlayer();

	sDrawImageInfo data;
	ZeroMemory(&data, sizeof(data));
	setImage(&data, stashBtnsImages);

	setFrame(&data, 0 + isDownBtn.previous);
	D2PrintImage(&data, getXPreviousBtn(), getYPreviousBtn(), -1, 5, 0);

	setFrame(&data, 2 + isDownBtn.next);
	D2PrintImage(&data, getXNextBtn(), getYNextBtn(), -1, 5, 0);

	if (active_sharedStash)
	{
		setFrame(&data, 4 + isDownBtn.toggleToSharedStash + (PCPY->showSharedStash ? 2 : 0));
		D2PrintImage(&data, getXSharedBtn(), getYSharedBtn(), -1, 5, 0);
	}

	setFrame(&data, 8 + isDownBtn.previousIndex);
	D2PrintImage(&data, getXPreviousIndexBtn(), getYPreviousIndexBtn(), -1, 5, 0);

	setFrame(&data, 10 + isDownBtn.nextIndex);
	D2PrintImage(&data, getXNextIndexBtn(), getYNextIndexBtn(), -1, 5, 0);

	if (active_sharedGold)
	{
		setImage(&data, sharedGoldBtnsImages);
		setFrame(&data, 0 + isDownBtn.putGold);
		D2PrintImage(&data, getXPutGoldBtn(), getYPutGoldBtn(), -1, 5, 0);

		setFrame(&data, 2 + isDownBtn.takeGold);
		D2PrintImage(&data, getXTakeGoldBtn(), getYTakeGoldBtn(), -1, 5, 0);
	}


	LPWSTR lpText;
	WCHAR text[100];
	DWORD mx = D2GetMouseX();
	DWORD my = D2GetMouseY();

	D2SetFont(1);

	if (isOnButtonPreviousStash(mx, my)) {
		lpText = getLocalString(STR_STASH_PREVIOUS_PAGE);
		D2PrintPopup(lpText, getXPreviousBtn() + getLPreviousBtn() / 2, getYPreviousBtn() - getHPreviousBtn(), WHITE, 1);

	}
	else if (isOnButtonNextStash(mx, my)) {
		lpText = getLocalString(STR_STASH_NEXT_PAGE);
		D2PrintPopup(lpText, getXNextBtn() + getLNextBtn() / 2, getYNextBtn() - getHNextBtn(), WHITE, 1);

	}
	else if (active_sharedStash && isOnButtonToggleSharedStash(mx, my)) {
		lpText = getLocalString(PCPY->showSharedStash ? STR_TOGGLE_TO_PERSONAL : STR_TOGGLE_TO_SHARED);
		D2PrintPopup(lpText, getXSharedBtn() + getLSharedBtn() / 2, getYSharedBtn() - getHSharedBtn(), WHITE, 1);

	}
	else if (isOnButtonPreviousIndexStash(mx, my)) {
		_snwprintf(text, _countof(text), getLocalString(STR_STASH_PREVIOUS_INDEX), nbPagesPerIndex, nbPagesPerIndex2);
		D2PrintPopup(text, getXPreviousIndexBtn() + getLPreviousIndexBtn() / 2, getYPreviousIndexBtn() - getHPreviousIndexBtn(), WHITE, 1);

	}
	else if (isOnButtonNextIndexStash(mx, my)) {
		_snwprintf(text, _countof(text), getLocalString(STR_STASH_NEXT_INDEX), nbPagesPerIndex, nbPagesPerIndex2);
		D2PrintPopup(text, getXNextIndexBtn() + getLNextIndexBtn() / 2, getYNextIndexBtn() - getHNextIndexBtn(), WHITE, 1);

	}
	else if (active_sharedGold && isOnButtonPutGold(mx, my)) {
		lpText = getLocalString(STR_PUT_GOLD);
		D2PrintPopup(lpText, getXPutGoldBtn() + getLPutGoldBtn() / 2, getYPutGoldBtn() - getHPutGoldBtn(), WHITE, 1);

	}
	else if (active_sharedGold && isOnButtonTakeGold(mx, my)) {
		lpText = getLocalString(STR_TAKE_GOLD);
		D2PrintPopup(lpText, getXTakeGoldBtn() + getLTakeGoldBtn() / 2, getYTakeGoldBtn() - getHTakeGoldBtn(), WHITE, 1);
	}
}


DWORD STDCALL manageBtnDown(sWinMessage* msg)
{
	if (onRealm) return 1;

	if (isOnButtonPreviousStash(msg->x, msg->y))
		isDownBtn.previous = 1;
	else if (isOnButtonNextStash(msg->x, msg->y))
		isDownBtn.next = 1;
	else if (active_sharedStash && isOnButtonToggleSharedStash(msg->x, msg->y))
		isDownBtn.toggleToSharedStash = 1;
	else if (isOnButtonPreviousIndexStash(msg->x, msg->y))
		isDownBtn.previousIndex = 1;
	else if (isOnButtonNextIndexStash(msg->x, msg->y))
		isDownBtn.nextIndex = 1;
	else if (active_sharedGold && isOnButtonPutGold(msg->x, msg->y))
		isDownBtn.putGold = 1;
	else if (active_sharedGold && isOnButtonTakeGold(msg->x, msg->y))
		isDownBtn.takeGold = 1;
	else return 1;

	D2PlaySound(4, 0, 0, 0, 0);
	freeMessage(msg);
	return 0;
}

	
DWORD STDCALL manageBtnUp(sWinMessage* msg)
{
	if (onRealm) return 1;

	Unit* ptChar = D2GetClientPlayer();

	if (isOnButtonPreviousStash(msg->x, msg->y)) {
		log_msg("push up left button previous\n");
		if (isDownBtn.previous)
			if (GetKeyState(VK_SHIFT) < 0)
				updateServer(US_SELECT_PREVIOUS2);
			else
				updateServer(US_SELECT_PREVIOUS);

	}
	else if (isOnButtonNextStash(msg->x, msg->y)) {
		log_msg("push up left button next\n");
		if (isDownBtn.next)
			if (GetKeyState(VK_SHIFT) < 0)
				updateServer(US_SELECT_NEXT2);
			else
				updateServer(US_SELECT_NEXT);

	}
	else if (active_sharedStash && isOnButtonToggleSharedStash(msg->x, msg->y)) {
		log_msg("push up left button shared\n");
		if (isDownBtn.toggleToSharedStash)
			if (PCPY->showSharedStash)
				updateServer(US_SELECT_SELF);
			else
				updateServer(US_SELECT_SHARED);

	}
	else if (isOnButtonPreviousIndexStash(msg->x, msg->y)) {
		log_msg("select left button previous index\n");
		if (isDownBtn.previousIndex)
			if (GetKeyState(VK_SHIFT) < 0)
				updateServer(US_SELECT_PREVIOUS_INDEX2);
			else
				updateServer(US_SELECT_PREVIOUS_INDEX);

	}
	else if (isOnButtonNextIndexStash(msg->x, msg->y)) {
		log_msg("push up left button next index\n");
		if (isDownBtn.nextIndex)
			if (GetKeyState(VK_SHIFT) < 0)
				updateServer(US_SELECT_NEXT_INDEX2);
			else
				updateServer(US_SELECT_NEXT_INDEX);

	}
	else if (active_sharedGold && isOnButtonPutGold(msg->x, msg->y)) {
		log_msg("push up left put gold\n");
		if (isDownBtn.putGold)
			updateServer(US_PUTGOLD);

	}
	else if (active_sharedGold && isOnButtonTakeGold(msg->x, msg->y)) {
		log_msg("push up left take gold\n");
		if (isDownBtn.takeGold)
			updateServer(US_TAKEGOLD);

	}
	else return 1;

	freeMessage(msg);
	return 0;
}

void STDCALL printPageNumber()
{
	if (onRealm) return;

	Unit* ptChar = D2GetClientPlayer();
	WCHAR popupText[100];

	if (PCPY->currentStash)
	{
		bool isShared = PCPY->currentStash->isShared;
		bool isIndex = PCPY->currentStash->isIndex;
		DWORD currentId = PCPY->currentStash->id;

		if (PCPY->currentStash->name && PCPY->currentStash->name[0])
			mbstowcs(popupText, PCPY->currentStash->name, _countof(popupText));
		else
			_snwprintf(popupText, _countof(popupText), getLocalString(isShared ? STR_SHARED_PAGE_NUMBER : STR_PERSONAL_PAGE_NUMBER), currentId + 1);
		D2PrintString(popupText, RX(256 - D2GetPixelLen(popupText) / 2), RY(692), (isShared ? (isIndex ? CRYSTAL_RED : RED) : (isIndex ? BRIGHT_WHITE : WHITE)), 0);
	}
	else
	{
		_snwprintf(popupText, _countof(popupText), getLocalString(STR_NO_SELECTED_PAGE));
		D2PrintString(popupText, RX(256 - D2GetPixelLen(popupText) / 2), RY(692), WHITE, 0);
	}

	//printGoldMaxPopup
	WCHAR maxGoldText[100];
	DWORD mx = D2GetMouseX();
	DWORD my = D2GetMouseY();

	D2SetFont(1);
	if ((RX(142) < mx) && (mx < RX(394)) && (RY(730) < my) && (my < RY(706)))
	{
		_snwprintf(maxGoldText, _countof(maxGoldText), D2GetStringFromIndex(4051), D2GetMaxGoldBank(ptChar));
		if (active_sharedGold)
		{
			_snwprintf(popupText, _countof(popupText), L"%s\n%s: %u", maxGoldText, getLocalString(STR_SHARED_GOLD_QUANTITY), PCPY->sharedGold);
			D2PrintPopup(popupText, RX(256), RY(734), WHITE, 1);
		}
		else
		{
			D2PrintPopup(maxGoldText, RX(256), RY(734), WHITE, 1);
		}
	}
}

Unit* FASTCALL FindItemFromSet(Unit* ptChar, DWORD nSetItemId)
{
	if (!ptChar || !PCInventory || !PCPY)
		return NULL;

	for (Unit* ptItem = D2InventoryGetFirstItem(PCInventory); ptItem != NULL; ptItem = D2UnitGetNextItem(ptItem)) {
		if (ptItem->ptItemData->quality == ITEMQUALITY_SET && ptItem->ptItemData->uniqueID == nSetItemId) {
			return ptItem;
		}
	}

	for (Stash* ptStash = PCPY->selfStash; ptStash != NULL; ptStash = ((displaySharedSetItemNameInGreen && ptStash->isShared) ? NULL : PCPY->sharedStash)) {
		for (Stash* ptPage = ptStash; ptPage != NULL; ptPage = ptPage->nextStash) {
			for (Unit* ptItem = ptPage->ptListItem; ptItem != NULL; ptItem = D2UnitGetNextItem(ptItem)) {
				if (ptItem->ptItemData->quality == ITEMQUALITY_SET && ptItem->ptItemData->uniqueID == nSetItemId) {
					return ptItem;
				}
			}
		}
	}

	return NULL;
}

static Stash* curStash=NULL;
static DWORD currentSawStash=0;
Unit* STDCALL getNextItemForSet(Unit* ptItem)
{
	Unit* item = ptItem?D2UnitGetNextItem(ptItem):NULL;
	if (item) return item;

	if (!curStash)
	{
		Unit* ptChar = D2GetClientPlayer();
		switch (currentSawStash)
		{
		case 0: curStash = PCPY->selfStash;
				currentSawStash = displaySharedSetItemNameInGreen ? 1 : 2;
				break;
		case 1: curStash = PCPY->sharedStash;
				currentSawStash = 2;
				break;
		default:return NULL;
		}
	} else {
		curStash = curStash->nextStash;
	}
	if (curStash)
	{
		item = curStash->ptListItem;
		if (item) return item;
	}
	return getNextItemForSet(item);
}

Unit* STDCALL initGetNextItemForSet(Inventory* ptInventory)
{
	Unit* ptChar = D2GetClientPlayer();
	if (ptChar->nUnitType != UNIT_PLAYER) return NULL;
	if (!PCPY) return NULL;

	curStash = NULL;
	currentSawStash = 0;
	Unit* item = D2InventoryGetFirstItem(ptInventory);
	if (item) return item;
	return getNextItemForSet(item);
}

FCT_ASM( caller_manageBtnDown_mxls )
	MOV ESI, DWORD PTR SS:[ESP+0x1C]
	PUSH ESI
	CALL manageBtnDown
	TEST EAX, EAX
	RETN
}}

FCT_ASM( caller_manageBtnDown_111 )
	PUSH EBP
	CALL manageBtnDown
	TEST EAX,EAX
	JE IS_NOT_ON_BUTTON
	POP EDX
	MOV EDX, DWORD PTR DS:[EDX+0x10]
	MOV DWORD PTR DS:[EDX],1
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RETN 4
IS_NOT_ON_BUTTON:
	JMP D2ClickOnStashButton
}}

FCT_ASM( caller_manageBtnDown )
	PUSH EDI
	CALL manageBtnDown
	TEST EAX,EAX
	JE IS_NOT_ON_BUTTON
	POP EDX
	MOV EDX, DWORD PTR DS:[EDX+0xF3]
	MOV DWORD PTR DS:[EDX],1
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RETN 4
IS_NOT_ON_BUTTON:
	JMP D2isLODGame
}}

FCT_ASM( caller_manageBtnUp_mxls )
	MOV ESI, DWORD PTR SS:[ESP+0x1C]
	PUSH ESI
	CALL manageBtnUp
	MOV isDownBtn.all, 0
	TEST EAX, EAX
	RETN
}}

FCT_ASM( caller_manageBtnUp_111 )
	PUSH EBX
	CALL manageBtnUp
	MOV isDownBtn.all,0
	TEST EAX,EAX
	JE IS_NOT_ON_BUTTON
	POP EDX
	MOV EDX, DWORD PTR DS:[EDX+0x1A]
	MOV DWORD PTR DS:[EDX],0
	SUB EDX,8
	MOV DWORD PTR DS:[EDX],0
	ADD EDX,4
	MOV DWORD PTR DS:[EDX],0
	ADD EDX,0x68
	MOV DWORD PTR DS:[EDX],0
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	POP ECX
	RETN 4
IS_NOT_ON_BUTTON:
	JMP D2ClickOnStashButton
}}

FCT_ASM( caller_manageBtnUp )
	PUSH EBP
	CALL manageBtnUp
	MOV isDownBtn.all,0
	TEST EAX,EAX
	JE IS_NOT_ON_BUTTON
	POP EDX
	MOV EDX, DWORD PTR DS:[EDX+0xEA]
	MOV DWORD PTR DS:[EDX],0
	SUB EDX,8
	MOV DWORD PTR DS:[EDX],0
	ADD EDX,4
	MOV DWORD PTR DS:[EDX],0
	ADD EDX,0x68
	MOV DWORD PTR DS:[EDX],0
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RETN 4
IS_NOT_ON_BUTTON:
	JMP D2isLODGame
}}

FCT_ASM ( initBtnsStates )
	MOV isDownBtn.all,0
	ADD ESP,0x104
	RETN
}}

FCT_ASM( caller_printStashPage_mxls )
	CALL D2DrawStashPage
	CALL printBtns
	CALL printPageNumber
	RETN
}}

void Install_InterfaceStash()
{
	static int isInstalled = false;
	if (isInstalled) return;

	Install_UpdateServer();
	Install_PlugYImagesFiles();

	log_msg("Patch D2Sigma for stash interface. (InterfaceStash)\n");

	// Stash page drawing hook
	mem_seek RVA(D2Sigma, 0x5E75D9, 0x05325F, 0x05777F, 0x0544EF, 0x05456F, 0x05493F, 0x05555F);
	MEMC_REF4(D2DrawStashPage, caller_printStashPage_mxls);

	/*
	// Print button images
	mem_seek R7(D2Client, 39060, 39060, 3F399, B1006, 7DF86, B3656, 99A56, 9DE26);
	MEMC_REF4( D2LoadBuySelBtn, printBtns);
	//6FADF398  |. E8 734D0400    CALL D2Client.6FB24110
	//6FB61005  |. E8 A6D3FEFF    CALL D2Client.6FB4E3B0
	//6FB2DF85  |. E8 3642FEFF    CALL D2Client.6FB121C0
	//6FB63655  |. E8 9682FDFF    CALL D2Client.6FB3B8F0
	//6FB49A55  |. E8 96500200    CALL D2Client.6FB6EAF0
	//6FB4DE25  |. E8 76ACF7FF    CALL D2Client.6FAC8AA0
	*/

	/*
	// print page number
	mem_seek R7(D2Client, 3903C, 3903C, 3F375, B0FE3, 7DF63, B3633, 99A33, 9DE03);
	MEMJ_REF4( D2PrintString, printPageNumber);
	//6FADF374  |. E8 3FCC0800    CALL <JMP.&D2Win.#10117>
	//6FB60FE2  |. E8 99C2F5FF    CALL <JMP.&D2Win.#10020>
	//6FB2DF62  |. E8 19F3F8FF    CALL <JMP.&D2Win.#10064>
	//6FB63632  |. E8 299DF5FF    CALL <JMP.&D2Win.#10001>
	//6FB49A32  |. E8 4739F7FF    CALL <JMP.&D2Win.#10150>
	//6FB4DE02  |. E8 4739F7FF    CALL <JMP.&D2Win.#10076>
	*/

	// Manage mouse down (Play sound)
	mem_seek RVA(D2Sigma, 0x605394, 0x074434, 0x078E04, 0x076A84, 0x076B24, 0x077234, 0x078444);
	memt_byte(0x8B, 0xE8);
	MEMT_REF4(0x85182474, caller_manageBtnDown_mxls);
	memt_byte(0xF6, 0x90);
	/*
	mem_seek R7(D2Client, 45091, 45091, 4BBA1, B4666, 7FDD6, B54A6, 9B8A6, 9FC76);
	MEMC_REF4( version_D2Client >= V111 ? (DWORD)D2ClickOnStashButton : (DWORD)D2isLODGame, version_D2Client >= V111 ? caller_manageBtnDown_111 : caller_manageBtnDown);
	//6FAEBBA0   > E8 DB04FCFF    CALL D2Client.6FAAC080
	//6FB64665   . E8 B61EFFFF    CALL D2Client.6FB56520
	//6FB2FDD5   . E8 E612FFFF    CALL D2Client.6FB210C0
	//6FB654A5   . E8 9611FFFF    CALL D2Client.6FB56640
	//6FB4B8A5   . E8 5614FFFF    CALL D2Client.6FB3CD00
	//6FB4FC75   . E8 960FFFFF    CALL D2Client.6FB40C10
	*/

	// Manage mouse up
	mem_seek RVA(D2Sigma, 0x6053F4, 0x074494, 0x078E64, 0x076AE4, 0x076B84, 0x077294, 0x0784A4);
	memt_byte(0x8B, 0xE8);
	MEMT_REF4(0x85182474, caller_manageBtnUp_mxls);
	memt_byte(0xF6, 0x90);
	/*
	mem_seek R7(D2Client, 455F9, 455F9, 4C0F9, B57B9, 7FC09, B52D9, 9B6D9, 9FAA9);
	MEMC_REF4( version_D2Client >= V111 ? (DWORD)D2ClickOnStashButton : (DWORD)D2isLODGame, version_D2Client >= V111 ? caller_manageBtnUp_111 : caller_manageBtnUp);
	//6FAEC0F8   > E8 83FFFBFF    CALL D2Client.6FAAC080
	//6FB657B8   > E8 630DFFFF    CALL D2Client.6FB56520
	//6FB2FC08   > E8 B314FFFF    CALL D2Client.6FB210C0
	//6FB652D8   > E8 6313FFFF    CALL D2Client.6FB56640
	//6FB4B6D8   > E8 2316FFFF    CALL D2Client.6FB3CD00
	//6FB4FAA8   > E8 6311FFFF    CALL D2Client.6FB40C10
	*/

	/*
	// init state of button on open stash page
	mem_seek R7(D2Client, 45B3A, 45B3A, 4C63A, A645A, 749DA, A9D9A, 8CC3A, 9441A);
	memt_byte( 0x81, 0xE9 );	// CALL initBtnsStates
	MEMT_REF4( 0x000104C4, initBtnsStates);
	memt_byte( 0x00, 0x90 );
	//6FAE5B3A  |> 81C4 04010000  ADD ESP,104
	//6FAE5B3A  |> 81C4 04010000  ADD ESP,104
	//6FAEC63A  |> 81C4 04010000  ADD ESP,104
	//6FB5645A  |> 81C4 04010000  ADD ESP,104
	//6FB249DA  |> 81C4 04010000  ADD ESP,104
	//6FB59D9A  |> 81C4 04010000  ADD ESP,104
	//6FB3CC3A  |> 81C4 04010000  ADD ESP,104
	//6FB4441A  |> 81C4 04010000  ADD ESP,104
	*/

	// Search for set items in a personal/shared stash
	mem_seek RVA(D2Sigma, 0x5F086E, 0x05CE5E, 0x06148E, 0x05E30E, 0x05E36E, 0x05E73E, 0x05F42E);
	MEMC_REF4(D2FindSetItemById, FindItemFromSet);

	/*
	// init the search of print in green the item set name we have in stash
	mem_seek R7(D2Client, 3F098, 3F098, 45997, A71F4, 71F64, A7314, 8D434, 91A24);
	MEMJ_REF4( D2InventoryGetFirstItem, initGetNextItemForSet);
	//6FAE5996  |. E8 D5500800    CALL <JMP.&D2Common.#10277>
	//6FB571F3  |. E8 D64EF6FF    CALL <JMP.&D2Common.#10402>
	//6FB21F63  |. E8 96A1F9FF    CALL <JMP.&D2Common.#10535>
	//6FB57313  |. E8 8250F6FF    CALL <JMP.&D2Common.#11151>
	//6FB3D433  |. E8 B8F0F7FF    CALL <JMP.&D2Common.#10460>
	//6FB41A23  |. E8 96A7F7FF    CALL <JMP.&D2Common.#11040>
	*/

	/*
	// Get next item for print in green the item set name we have in stash
	mem_seek R7(D2Client, 3F0FA, 3F0FA, 45A1C, A728B, 71FFB, A73AB, 8D4CB, 91ABB);
	MEMJ_REF4( D2UnitGetNextItem, getNextItemForSet);
	//6FAE5A1B   E8 4A500800      CALL <JMP.&D2Common.#10304>
	//6FB5728A  |. E8 0B4FF6FF    |CALL <JMP.&D2Common.#10934>
	//6FB21FFA  |. E8 A1A1F9FF    |CALL <JMP.&D2Common.#11140>
	//6FB573AA  |. E8 7550F6FF    |CALL <JMP.&D2Common.#10770>
	//6FB3D4CA  |. E8 99F0F7FF    |CALL <JMP.&D2Common.#10464>
	//6FB41ABA  |. E8 A3A8F7FF    |CALL <JMP.&D2Common.#10879>
	*/

	log_msg("\n");

	isInstalled = true;
}

/*================================= END OF FILE =================================*/