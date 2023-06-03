/*=================================================================
	File created by Yohann NICOLAS.
	Add support 1.13d by L'Autour.

	More little options.

=================================================================*/

#include "common.h"
#include "plugYFiles.h"
#include "updateServer.h"
#include <stdio.h>

int active_DisplayItemLevel = false;
DWORD nbPlayersCommandByDefault = 0;
DWORD nbPlayersCommand = 0;
int active_alwaysRegenMapInSP = false;
int active_RunLODs = false;
int active_AlwaysDisplayLifeMana = false;
int active_EnabledTXTFilesWithMSExcel = false;
int active_DisplayBaseStatsValue = false;
int active_LadderRunewords = false;
int active_EnabledCowPortalWhenCowKingWasKill = false;

bool option_EnableGambleRefresh = false;
bool option_ForceLegacyBossLifebar = false;
bool option_RemoveRespawnCooldown = false;

/****************************************************************************************************/

void STDCALL displayItemlevel(LPWSTR popup, Unit* ptItem)
{
	if (onRealm && (selectModParam==MOD_NO)) return;
	WCHAR text[0x50];
	_snwprintf(text, sizeof(text), L"%s: %u\n", getLocalString(STR_ITEM_LEVEL), D2GetItemLevel(ptItem));
	D2SetColorPopup(text,WHITE);
	wcscat(popup,text);
}

FCT_ASM ( caller_displayItemlevel_113 )
	PUSH ECX
	PUSH EAX
	PUSH EAX
	LEA EAX,DWORD PTR SS:[ESP+0x1E70]
	PUSH EAX
	CALL displayItemlevel
	POP EAX
	POP ECX
	POP EDX
	PUSH 0x100
	JMP EDX
}}

FCT_ASM ( caller_displayItemlevel_111 )
	PUSH ECX
	PUSH EDX
	PUSH ECX
	LEA EAX,DWORD PTR SS:[ESP+0x1E74]
	PUSH EAX
	CALL displayItemlevel
	POP EDX
	POP ECX
	POP EAX
	PUSH 0x100
	JMP EAX
}}

FCT_ASM ( caller_displayItemlevelSet_111 )
	PUSH ECX
	PUSH EDX
	PUSH ECX
	LEA EAX,DWORD PTR SS:[ESP+0x1958]
	PUSH EAX
	CALL displayItemlevel
	POP EDX
	POP ECX
	POP EAX
	PUSH 0x100
	JMP EAX
}}


FCT_ASM ( caller_displayItemlevel )
	PUSH ECX
	PUSH EDX
	PUSH ECX
	LEA EAX,DWORD PTR SS:[ESP+0x5018]
	PUSH EAX
	CALL displayItemlevel
	POP EDX
	POP ECX
	POP EAX
	PUSH 0x100
	JMP EAX
}}

FCT_ASM ( caller_displayItemlevelSet )
	PUSH ECX
	PUSH EDX
	PUSH ECX
	LEA EAX,DWORD PTR SS:[ESP+0x1220]
	PUSH EAX
	CALL displayItemlevel
	POP EDX
	POP ECX
	POP EAX
	PUSH 0x100
	JMP EAX
}}

FCT_ASM ( caller_displayItemlevel_9 )
	PUSH ECX
	PUSH EDX
	PUSH ECX
	LEA EAX,DWORD PTR SS:[ESP+0x501C]
	PUSH EAX
	CALL displayItemlevel
	POP EDX
	POP ECX
	POP EAX
	PUSH 0x100
	JMP EAX
}}

FCT_ASM ( caller_displayItemlevelSet_9 )
	PUSH ECX
	PUSH EDX
	PUSH ECX
	LEA EAX,DWORD PTR SS:[ESP+0x1224]
	PUSH EAX
	CALL displayItemlevel
	POP EDX
	POP ECX
	POP EAX
	PUSH 0x100
	JMP EAX
}}


void Install_DisplayItemLevel()
{
	static int isInstalled = false;
	if (isInstalled) return;

	log_msg("Patch D2Client for display item popup. (DisplayPopup)\n");

	// print the text in the final buffer
	mem_seek R7(D2Client,	3D47C,	3D47C,	438A1, ADD0A, 789DA, AE0AA, 941C0, 98590);
	memt_byte( 0x68 , 0xE8);
	MEMT_REF4( 0x100, version_D2Client >= V113c ? caller_displayItemlevel_113 : version_D2Client >= V111 ? caller_displayItemlevel_111 : version_D2Client == V110 ? caller_displayItemlevel : caller_displayItemlevel_9);
	//6FAE38A1   . 68 00010000        PUSH 100
	//6FB5DD0A  |. 68 00010000        PUSH 100
	//6FB289DA  |. 68 00010000	      PUSH 100
	//6FB5E0AA  |. 68 00010000    PUSH 100
	//6FB441C0  |. 68 00010000    PUSH 100
	//6FB48590  |. 68 00010000    PUSH 100

	// print the text in the final buffer (for set items)
	mem_seek R7(D2Client,	3C452,	3C452,	427BE, AC773, 77773, ACEB3, 92FE3, 973B3);
	memt_byte( 0x68 , 0xE8);
	MEMT_REF4( 0x100, version_D2Client >= V111  ? caller_displayItemlevelSet_111 : version_D2Client == V110 ? caller_displayItemlevelSet : caller_displayItemlevelSet_9);
	//6FAE27BE   . 68 00010000        PUSH 100
	//6FB5C773  |. 68 00010000        PUSH 100
	//6FB27773  |. 68 00010000        PUSH 100
	//6FB5CEB3  |. 68 00010000    PUSH 100
	//6FB42FE3  |. 68 00010000    PUSH 100
	//6FB473B3  |. 68 00010000    PUSH 100

	log_msg("\n");

	isInstalled = true;
}

/****************************************************************************************************/

char* msgNBPlayersString = "players %u";
DWORD* infoEnabledSendPlayersCommand;
void SendPlayersCommand()
{
	#pragma pack(1)
	struct {
		BYTE displayType;//0x15 main msg;  0x14: char popup
		BYTE un;
		BYTE zero;
		char string[0xFF];
		char null;
	} data;
	#pragma pack()

	if (!needToInit || onRealm) return;
	needToInit=0;

	DWORD info = *infoEnabledSendPlayersCommand;
	if ((info != 0) && (info != 1) && (info != 6) && (info != 8)) return;
	log_msg("SendPlayersCommand %u\n", nbPlayersCommand);

	D2SetNbPlayers(nbPlayersCommand);

	memset(&data,0,sizeof(data));
	data.displayType=0x15;
	data.un=1;
	data.zero=0;//*(BYTE*)(offset_D2Client+0x112CFC); in 1.10
	data.null=NULL;
	sprintf(data.string, msgNBPlayersString, nbPlayersCommand);
	D2SendMsgToAll((BYTE*)&data);
}

FCT_ASM ( caller_SendPlayersCommand )
	CALL SendPlayersCommand
	JMP D2GetResolution
}}

void Install_SendPlayersCommand()
{
	static int isInstalled = false;
	if (isInstalled) return;

	log_msg("Patch D2Client for init default nb /players. (SendPlayersCommand)\n");

	infoEnabledSendPlayersCommand = (DWORD*)R7(D2Client, 111D60, 110BC0, 107960, 11BFBC, 11C2AC, 11BFF8, 11C394, 11D1DC);
	if ( version_D2Client >= V110 )
		msgNBPlayersString = (char*)R7(D2Client, 0000, 0000, D8448, D06A8, D4748, D4680, D4E00, D470C);

	// Set nb Player to default
	mem_seek R7(D2Client,	8723B, 865BB, 81B8B, A3602, 66A02, 90162, C39F2, 1D3F2);//6FB1C7B7-6FAA0000
	MEMJ_REF4( D2GetResolution , caller_SendPlayersCommand);
	//6FB2723A  |. E8 7D8E0300    CALL <JMP.&D2gfx.#10005>
	//6FB265BA  |. E8 7D8E0300    CALL <JMP.&D2gfx.#10005>
	//6FB21B8A   . E8 4F9C0400    CALL <JMP.&D2gfx.#10005>
	//6FB53601  |. E8 E099F6FF    CALL <JMP.&D2gfx.#10000>
	//6FB16A01  |. E8 AA65FAFF    CALL <JMP.&D2gfx.#10063>
	//6FB40161  |. E8 1ECFF7FF    CALL <JMP.&D2gfx.#10043>
	//6FB739F1  |. E8 B896F4FF    CALL <JMP.&D2gfx.#10031>
	//6FACD3F1  |. E8 EAFBFEFF    CALL <JMP.&D2gfx.#10012>

	log_msg("\n");

	isInstalled = true;
}

/****************************************************************************************************/


void Install_AlwaysRegenMapInSP()
{
	static int isInstalled = false;
	if (isInstalled) return;
	
	log_msg("Patch D2Game for always regenerate maps in Single player. (AlwaysRegenMapInSP)\n");

	// Always regenerate maps in Single player
	mem_seek R7(D2Game,	517ED,	51BFA,	5DE88, 6235D, 2966D, 1E1FE, ED3BE, 26D7E);
	memt_byte( 0x74 , 0xEB);		// JMP SHORT D2Game.01FE2368
	//6FC8DE88  |. 74 09          JE SHORT D2Game.6FC8DE93
	//01FE235D  |. 74 09		  JE SHORT D2Game.01FE2368
	//01F9966D  |. 74 09          JE SHORT D2Game.01F99678
	//6FC3E1FE  |. 74 09          JE SHORT D2Game.6FC3E209
	//6FD0D3BE  |. 74 09          JE SHORT D2Game.6FD0D3C9
	//6FC46D7E  |. 74 09          JE SHORT D2Game.6FC46D89

	log_msg("\n");

	isInstalled = true;
}

/****************************************************************************************************/

void Install_RunLODs()
{
	static int isInstalled = false;
	if (isInstalled) return;

	log_msg("Patch D2gfx for launch any number of Diablo II game in the same computer. (RunLODs)\n");

	// execute if it's our packet else continue
	mem_seek R7(D2gfx, 447C, 447C, 446A, 84CF, 84AF, 894F, 85BF, B6B0);
	memt_byte( 0x74, 0xEB );	// JMP SHORT D2gfx.6FA884F6
	//6FA884AF  |. 74 45          JE SHORT D2gfx.6FA884F6
	//6FA8894F  |. 74 45          JE SHORT D2gfx.6FA88996
	//6FA885BF  |. 74 45          JE SHORT D2gfx.6FA88606
	//6FA8B6B0  |. 74 45          JE SHORT D2gfx.6FA8B6F7

	log_msg("\n");

	isInstalled = true;
}

/****************************************************************************************************/


FCT_ASM (caller_AlwaysDisplayLife_113)
	POP EAX
	CMP onRealm,0
	JNZ normalDisplayLife
	CMP active_AlwaysDisplayLifeMana,0
	JE normalDisplayLife
	ADD EAX,0x28
	JMP EAX
normalDisplayLife:
	CMP EDX,0x1E
	JGE cont
	ADD EAX,0xBC
cont:
	JMP EAX
}}

FCT_ASM (caller_AlwaysDisplayLife_111)
	CMP onRealm,0
	JNZ normalDisplayLife
	CMP active_AlwaysDisplayLifeMana,0
	JE normalDisplayLife
	POP EAX
	ADD EAX,0x25
	JMP EAX
normalDisplayLife:
	MOV EAX,ptResolutionY
	MOV EAX,DWORD PTR DS:[EAX]
	RETN
}}

FCT_ASM (caller_AlwaysDisplayLife)
	CMP onRealm,0
	JNZ normalDisplayLife
	CMP active_AlwaysDisplayLifeMana,0
	JE normalDisplayLife
	POP EAX
	ADD EAX,0x28
	JMP EAX
normalDisplayLife:
	MOV EAX,ptResolutionY
	MOV EAX,DWORD PTR DS:[EAX]
	RETN
}}



FCT_ASM (caller_AlwaysDisplayMana_113)
	MOV EAX,DWORD PTR DS:[ptResolutionY]
	MOV EAX,DWORD PTR DS:[EAX]
	CMP onRealm,0
	JNZ normalDisplayMana
	CMP active_AlwaysDisplayLifeMana,0
	JE normalDisplayMana
	POP EAX
	ADD EAX,0x3C
	JMP EAX
normalDisplayMana:
	RETN
}}

FCT_ASM (caller_AlwaysDisplayMana)
	CMP onRealm,0
	JNZ normalDisplayMana
	CMP active_AlwaysDisplayLifeMana,0
	JE normalDisplayMana
	POP EAX
	MOV WORD PTR SS:[ESP+0x14],0
	ADD EAX,0x38
	JMP EAX
normalDisplayMana:
	POP EAX
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	ADD ESP,0x25C
	RETN
}}

FCT_ASM (caller_AlwaysDisplayMana_9)
	CMP onRealm,0
	JNZ normalDisplayMana
	CMP active_AlwaysDisplayLifeMana,0
	JE normalDisplayMana
	ADD DWORD PTR SS:[ESP],0x32
	LEA ECX,DWORD PTR SS:[ESP+0x24]
	PUSH DWORD PTR SS:[ESP+0xC]
	PUSH DWORD PTR SS:[ESP+0xC]
	PUSH DWORD PTR SS:[ESP+0xC]
	CALL D2PrintString
	MOV WORD PTR SS:[ESP+0x24],0
	RETN 0xC
normalDisplayMana:
	POP EAX
	ADD EAX,0xC2
	JMP EAX
}}

void Install_AlwaysDisplayLifeMana()
{
	static int isInstalled = false;
	if (isInstalled) return;

	log_msg("Patch D2Client for always display life and mana. (ALwaysPrintLifeMana)\n");

	if ( version_D2Client >= V113c )
	{
		mem_seek R7(D2Client, 0000, 0000, 0000, 0000, 0000, 0000, 2764A, 6D6FA);
		memt_byte( 0x0F , 0x90);
		memt_byte( 0x8C , 0xE8);
		MEMT_REF4( 0x000000BC, caller_AlwaysDisplayLife_113);
		//6FAD764A  |. 0F8C BC000000  JL D2Client.6FAD770C
		//6FAD7650  |. 83FA 6E        CMP EDX,6E
		//6FAD7653  |. 0F8F B3000000  JG D2Client.6FAD770C
		//6FAD7659  |. A1 4CBCB86F    MOV EAX,DWORD PTR DS:[6FB8BC4C]

		//6FAD7667  |. 0F8C A4000000  JL D2Client.6FAD7711
		//6FB1D717  |. 0F8C A4000000  JL D2Client.6FB1D7C1
	} else {
		// Always display life.
		mem_seek R7(D2Client, 58B32, 58B32, 5F102, 2D713, B5DF3, 81733, 0000, 0000);
		memt_byte( 0xA1 , 0xE8);
		MEMT_REF4( ptResolutionY , version_D2Client >= V111 ? caller_AlwaysDisplayLife_111 : caller_AlwaysDisplayLife);
		//6FADD713  |. A1 605CBA6F    MOV EAX,DWORD PTR DS:[6FBA5C60]
		//6FB65DF3  |. A1 C84FBA6F    MOV EAX,DWORD PTR DS:[6FBA4FC8]
		//6FB31733  |. A1 E4C6B86F    MOV EAX,DWORD PTR DS:[6FB8C6E4]
	}


	// Always display mana.
	if ( version_D2Client >= V113c )
	{
		//mem_seek R7(D2Client, 0000, 0000, 0000, 0000, 0000, 0000, 27711);
		//memt_byte( 0x8B , 0x90);
		//memt_byte( 0x0D , 0xE8);
		//MEMT_REF4( offset_D2Client + 0x11C4B4 , caller_AlwaysDisplayMana_113);
		mem_seek R7(D2Client, 0000, 0000, 0000, 0000, 0000, 0000, 2770C, 6D7BC);
		memt_byte( 0xA1 , 0xE8);
		MEMT_REF4( ptResolutionY , caller_AlwaysDisplayMana_113);
		//6FAD770C  |> A1 4CBCB86F    MOV EAX,DWORD PTR DS:[6FB8BC4C]
		//6FAD7711  |> 8B0D B4C4BC6F  MOV ECX,DWORD PTR DS:[6FBCC4B4]

		//6FAD7708  |. 8B5424 10      MOV EDX,DWORD PTR SS:[ESP+10]

		//6FAD77C7  |> 5F             POP EDI
		//6FAD77C8  |. 5E             POP ESI
		//6FAD77C9  |. 5D             POP EBP
		//6FAD77CA  |. 5B             POP EBX
		//6FAD77CB  |. 81C4 28030000  ADD ESP,328
		//6FAD77D1  \. C3             RETN
		//6FB1D7BC  |> A1 3870BA6F    MOV EAX,DWORD PTR DS:[6FB8BC4C]
	}
	else if ( version_D2Client >= V110 )
	{
		mem_seek R7(D2Client, 0000, 0000, 5F1E6, 2D7FB, B5EDB, 8181B, 0000, 0000);
		memt_byte( 0x5F , 0xE8);
		MEMT_REF4( 0x815B5D5E , caller_AlwaysDisplayMana);
		memt_byte( 0xC4 , 0x90);	// NOP
		memt_byte( 0x5C , 0x90);	// NOP
		memt_byte( 0x02 , 0x90);	// NOP
		memt_byte( 0x00 , 0x90);	// NOP
		memt_byte( 0x00 , 0x90);	// NOP
		//6FADD7FB  |. 5F             POP EDI
		//6FADD7FC  |. 5E             POP ESI
		//6FADD7FD  |. 5D             POP EBP
		//6FADD7FE  |. 5B             POP EBX
		//6FADD7FF  |. 81C4 5C020000  ADD ESP,25C
		//6FB65EDB  |. 5F             POP EDI
		//6FB65EDC  |. 5E             POP ESI
		//6FB65EDD  |. 5D             POP EBP
		//6FB65EDE  |. 5B             POP EBX
		//6FB65EDF  |. 81C4 5C020000  ADD ESP,25C
		//6FB3181B  |. 5F             POP EDI
		//6FB3181C  |. 5E             POP ESI
		//6FB3181D  |. 5D             POP EBP
		//6FB3181E  |. 5B             POP EBX
		//6FB3181F  |. 81C4 5C020000  ADD ESP,25C
		//6FB31825  |. C3             RETN
	} else {
		mem_seek R7(D2Client, 58C09, 58C09, 0000, 0000, 0000, 0000, 0000, 0000);
		memt_byte( 0xE9 , 0xE8);
		MEMT_REF4( 0x000000C2 , caller_AlwaysDisplayMana_9);
		//6FAF8C09   . E9 C2000000    JMP D2Client.6FAF8CD0
	}

	if (active_AlwaysDisplayLifeMana==2)
		active_AlwaysDisplayLifeMana=0;

	log_msg("\n");

	isInstalled = true;
}

/****************************************************************************************************/

void Install_EnabledTXTFilesWithMSExcel()
{
	static int isInstalled = false;
	if (isInstalled) return;

	log_msg("Patch D2Client for enabled the opening of files already opened by MS Excel. (EnabledTXTFilesWithMSExcel)\n");

	mem_seek( (DWORD)D2StormMPQOpenFile + (version_Storm >= V111 ? 0x12A : 0xFF) );
	memt_byte( 0x01 , 0x03);	//; |ShareMode = FILE_SHARE_READ|FILE_SHARE_WRITE									
	//6FC1C969  |. 6A 01          PUSH 1        ; |ShareMode = FILE_SHARE_READ

	log_msg("\n");

	isInstalled = true;
}


/****************************************************************************************************/

#define	getXAssSTRBtn()			RX(200)
#define	getLAssSTRBtn()			44
#define	getYAssSTRBtn()			RY(532)
#define	getHAssSTRBtn()			44
#define isOnAssSTRBtn(x,y)		isOnRect(x, y, getXAssSTRBtn(), getYAssSTRBtn(), getLAssSTRBtn(), getHAssSTRBtn())

#define	getXAssDEXBtn()			RX(200)
#define	getLAssDEXBtn()			44
#define	getYAssDEXBtn()			RY(457)
#define	getHAssDEXBtn()			44
#define isOnAssDEXBtn(x,y)		isOnRect(x, y, getXAssDEXBtn(), getYAssDEXBtn(), getLAssDEXBtn(), getHAssDEXBtn())

#define	getXAssVITBtn()			RX(200)
#define	getLAssVITBtn()			44
#define	getYAssVITBtn()			RY(382)
#define	getHAssVITBtn()			44
#define isOnAssVITBtn(x,y)		isOnRect(x, y, getXAssVITBtn(), getYAssVITBtn(), getLAssVITBtn(), getHAssVITBtn())

#define	getXAssENEBtn()			RX(200)
#define	getLAssENEBtn()			44
#define	getYAssENEBtn()			RY(307)
#define	getHAssENEBtn()			44
#define isOnAssENEBtn(x,y)		isOnRect(x, y, getXAssENEBtn(), getYAssENEBtn(), getLAssENEBtn(), getHAssENEBtn())

void STDCALL printDisplayBaseStatsValue()
{
	if (onRealm) return;

	WCHAR text[100];

	Unit* ptChar = D2GetClientPlayer();
	CharStatsBIN* charStats = D2GetCharStatsBIN(ptChar->nPlayerClass);

	DWORD mx = D2GetMouseX();
	DWORD my = D2GetMouseY();

	D2SetFont(1);
	if (isOnAssSTRBtn(mx, my))
	{
		_snwprintf(text, _countof(text), getLocalString(STR_STATS_BASE_MIN), D2GetPlayerBaseStat(ptChar, STATS_STRENGTH, 0), (int)charStats->baseSTR);
		D2PrintPopup(text, getXAssSTRBtn() + getLAssSTRBtn() / 2, getYAssSTRBtn() - getHAssSTRBtn(), WHITE, 1);
	}
	else if (isOnAssDEXBtn(mx, my))
	{
		_snwprintf(text, _countof(text), getLocalString(STR_STATS_BASE_MIN), D2GetPlayerBaseStat(ptChar, STATS_DEXTERITY, 0), (int)charStats->baseDEX);
		D2PrintPopup(text, getXAssDEXBtn() + getLAssDEXBtn() / 2, getYAssDEXBtn() - getHAssDEXBtn(), WHITE, 1);
	}
	else if (isOnAssVITBtn(mx, my))
	{
		_snwprintf(text, _countof(text), getLocalString(STR_STATS_BASE_MIN), D2GetPlayerBaseStat(ptChar, STATS_VITALITY, 0), (int)charStats->baseVIT);
		D2PrintPopup(text, getXAssVITBtn() + getLAssVITBtn() / 2, getYAssVITBtn() - getHAssVITBtn(), WHITE, 1);
	}
	else if (isOnAssENEBtn(mx, my))
	{
		_snwprintf(text, _countof(text), getLocalString(STR_STATS_BASE_MIN), D2GetPlayerBaseStat(ptChar, STATS_ENERGY, 0), (int)charStats->baseENE);
		D2PrintPopup(text, getXAssENEBtn() + getLAssENEBtn() / 2, getYAssENEBtn() - getHAssENEBtn(), WHITE, 1);
	}
}

FCT_ASM ( caller_printStatsPage_mxls )
	CALL D2DrawStatsPageUiElements
	CALL printDisplayBaseStatsValue
	RETN
}}

FCT_ASM ( caller_displayBaseStatsValue )
	POP EAX
	XOR ECX,ECX
	MOV CX,WORD PTR DS:[ESI+8]
	PUSH ECX
	PUSH EAX
	JMP printDisplayBaseStatsValue
}}

void Install_DisplayBaseStatsValue()
{
	static int isInstalled = false;
	if (isInstalled) return;

	log_msg("Patch D2Sigma for display base stats value. (DisplayBaseStatsValue)\n");

	// Stats page ui items drawing hook
	mem_seek RVA(D2Sigma, 0x6072F0, 0x0764C0, 0x07B110, 0x078E20, 0x078EC0, 0x0795D0, 0x07A820, 0x07AF40, 0x07B310);
	MEMC_REF4(D2DrawStatsPageUiElements, caller_printStatsPage_mxls);

	/*
	// Always print stat button images.
	mem_seek R7(D2Client, 29B12, 29B02, 30073, 82BBA, 8963A, 6B59A, BD1B5, BF955);
	memt_byte( 0x8B, 0xEB );	// JMP SHORT D2Client.6FAD0088
	memt_byte( 0x4C, V7(D2Client, 12, 12, 13, 13, 13, 13, 13, 13) );
	memt_byte( 0x24, 0x90 );	// NOP
	memt_byte( V7(D2Client, 20, 20, 14, 1C, 1C, 1C, 1C, 1C), 0x90 );			// NOP (V109d:0x20 , V110:0x14
	//6FAD0073     8B4C24 14         MOV ECX,DWORD PTR SS:[ESP+14]
	//6FB32BBA   > 8B4C24 1C         MOV ECX,DWORD PTR SS:[ESP+1C]
	//6FB3963A   > 8B4C24 1C         MOV ECX,DWORD PTR SS:[ESP+1C]
	//6FB1B59A   > 8B4C24 1C      MOV ECX,DWORD PTR SS:[ESP+1C]
	//6FB6D1B5  |> 8B4C24 1C      MOV ECX,DWORD PTR SS:[ESP+1C]
	//6FB6F955  |> 8B4C24 1C      MOV ECX,DWORD PTR SS:[ESP+1C]
	*/

	/*
	mem_seek R7(D2Client, 29B9D, 29B8D, 300FD, 82C54, 896D4, 6B637, BD23E, BF9DE);
	MEMJ_REF4( D2PrintImage, caller_displayBaseStatsValue);
	//6FB32C53   . E8 82A3F8FF    CALL <JMP.&D2gfx.#10047>
	//6FB396D3   . E8 D238F8FF    CALL <JMP.&D2gfx.#10044>
	//6FB1B636   . E8 431AFAFF    CALL <JMP.&D2gfx.#10024>
	//6FB6D23D  |. E8 54FEF4FF    |CALL <JMP.&D2gfx.#10041>
	//6FB6F9DD  |. E8 54FEF4FF    |CALL <JMP.&D2gfx.#10042>
	*/

	log_msg("\n");

	isInstalled = true;
}


/****************************************************************************************************/

RunesBIN* STDCALL compileRunesTxt(DWORD unused, const char* filename, BINField* ptFields, DWORD* ptRecordCount, DWORD recordLength)
{
	RunesBIN* ptRunesBin = (RunesBIN*)D2CompileTxtFile(unused, filename, ptFields, ptRecordCount, recordLength);
	for (DWORD i=0; i < *ptRecordCount; i++)
		ptRunesBin[i].Server=0;
	return ptRunesBin;
}

void Install_LadderRunewords()
{
	static int isInstalled = false;
	if (isInstalled) return;

	if ( version_D2Common < V110 )
		return;

	log_msg("Patch D2Common for enabled the ladder only runewords. (LadderRunewords)\n");

	mem_seek R7(D2Common, 0000, 0000, 1E965, 61762, 43A72, 5D492, 724B2, 63782);
	MEMC_REF4( D2CompileTxtFile, compileRunesTxt);
	//01B6E964  |. E8 0714FFFF    CALL D2Common.#10578                     ; \#10578
	//6FDB1761  |. E8 FAA8FCFF    CALL D2Common.#10653                     ; \#10653
	//6FD93A71  |. E8 EAADFCFF    CALL D2Common.#10496                     ; \#10496
	//6FDAD491  |. E8 BA49FEFF    CALL D2Common.#10244                     ; \#10244
	//6FDC24B1  |. E8 8ACAFEFF    CALL D2Common.#10849                     ; \#10849
	//6FDB3781  |. E8 5A93FAFF    CALL D2Common.#10037                     ; \#10037

	log_msg("\n");

	isInstalled = true;
}



/****************************************************************************************************/

void Install_EnabledCowPortalWhenCowKingWasKill()
{
	static int isInstalled = false;
	if (isInstalled) return;

	log_msg("Patch D2Game for enabled the opening of Cow Portal when player have already kill the cow king in that difficulty. (EnabledCowPortalWhenCowKingWasKill)\n");

	mem_seek R7(D2Game, 5DFF7, 5E457, 6C5E7 , B1278, 5DB68, 75C68, 67508, EBE8);
	memt_byte( 0x85 , 0x33); // XOR EAX,EAX
	//6FC8DFF7   . 85C0           TEST EAX,EAX
	//6FC8E457   . 85C0           TEST EAX,EAX
	//6FC9C5E7   . 85C0           TEST EAX,EAX
	//02061278  |. 85C0           TEST EAX,EAX
	//01FEDB68  |. 85C0           TEST EAX,EAX
	//6FC95C68  |. 85C0           TEST EAX,EAX
	//6FC87508  |. 85C0           TEST EAX,EAX
	//6FC2EBE8  |. 85C0           TEST EAX,EAX

	log_msg("\n");

	isInstalled = true;
}

/****************************************************************************************************/

#define	getXRefreshBtn()		RX(244)
#define	getLRefreshBtn()		32
#define	getYRefreshBtn()		RY(101)
#define	getHRefreshBtn()		32
#define isOnRefreshBtn(x,y)		isOnRect(x, y, getXRefreshBtn(), getYRefreshBtn(), getLRefreshBtn(), getHRefreshBtn())

static DWORD isDownRefreshBtn;

void STDCALL PrintStorePageBtns()
{
	if (onRealm) return;

	if (!*ptActiveGambleMenu) return;

	sDrawImageInfo data;
	ZeroMemory(&data, sizeof(data));

	setImage(&data, gpRefreshStockButtonGfx);
	setFrame(&data, isDownRefreshBtn);
	D2PrintImage(&data, getXRefreshBtn(), getYRefreshBtn(), -1, 5, 0);

	D2SetFont(1);
	if (isOnRefreshBtn(D2GetMouseX(), D2GetMouseY()))
	{
		D2PrintPopup(getLocalString(STR_REFRESH_GAMBLE), getXRefreshBtn() + getLRefreshBtn() / 2, getYRefreshBtn() - getHRefreshBtn() - 6, WHITE, 1);
	}
}

DWORD STDCALL StorePageMouseDn(sWinMessage* msg)
{
	if (onRealm) return 1;

	if (!*ptActiveGambleMenu) return 1;

	if (isOnRefreshBtn(msg->x, msg->y))
	{
		log_msg("push down left button refresh\n");
		if ((GetTickCount() - *ptTransactionTimeStamp) >= 500)
		{
			isDownRefreshBtn = 1;
			D2PlaySound(11, 0, 0, 0, 0);
			freeMessage(msg);
			return 0;
		}
	}

	return 1;
}

DWORD STDCALL StorePageMouseUp(sWinMessage* msg)
{
	if (onRealm) return 1;

	if (!*ptActiveGambleMenu) return 1;

	if (isOnRefreshBtn(msg->x, msg->y))
	{
		DWORD nTickCount = GetTickCount();
		log_msg("push up left button refresh\n");
		if (isDownRefreshBtn && (nTickCount - *ptTransactionTimeStamp) >= 500)
		{
			*ptTransactionTimeStamp = nTickCount;
			D2ClearShopMenu(D2GetActiveNpc());
			updateServer(US_REFRESH_GAMBLE);
			freeMessage(msg);
			return 0;
		}
	}

	return 1;
}

FCT_ASM( caller_PrintStorePageBtns_mxls )
	CALL D2DrawStorePage
	CALL PrintStorePageBtns
	RETN
}}

FCT_ASM( caller_StorePageMouseDn_mxls )
	MOV ESI, DWORD PTR SS:[ESP+0x1C]
	PUSH ESI
	CALL StorePageMouseDn
	TEST EAX, EAX
	RETN
}}

FCT_ASM( caller_StorePageMouseUp_mxls )
	MOV ESI, DWORD PTR SS:[ESP+0x1C]
	PUSH ESI
	CALL StorePageMouseUp
	MOV isDownRefreshBtn, 0
	TEST EAX, EAX
	RETN
}}

void Install_RefreshGambleStock()
{
	static int isInstalled = false;
	if (isInstalled) return;

	Install_PlugYImagesFiles();
	Install_UpdateServer();

	log_msg("Patch D2Sigma to provide gamble refresh button for shop interface. (EnableGambleRefresh)\n");

	// Store page drawing hook
	mem_seek RVA(D2Sigma, 0x5E75C3, 0x053249, 0x057769, 0x0544D9, 0x054559, 0x054929, 0x055549, 0x0554F9, 0x055859);
	MEMC_REF4(D2DrawStorePage, caller_PrintStorePageBtns_mxls);

	// Manage mouse down (Play sound)
	mem_seek RVA(D2Sigma, 0x60CCD4, 0x07C194, 0x081394, 0x07FC14, 0x07FCD4, 0x080524, 0x081894, 0x0820B4, 0x0828F4);
	memt_byte(0x8B, 0xE8);
	MEMT_REF4(0x85182474, caller_StorePageMouseDn_mxls);
	memt_byte(0xF6, 0x90);

	// Manage mouse up
	mem_seek RVA(D2Sigma, 0x60CD84, 0x07C244, 0x081444, 0x07FCC4, 0x07FD84, 0x0805D4, 0x081944, 0x082164, 0x0829A4);
	memt_byte(0x8B, 0xE8);
	MEMT_REF4(0x85182474, caller_StorePageMouseUp_mxls);
	memt_byte(0xF6, 0x90);

	log_msg("\n");

	isInstalled = true;
}

/****************************************************************************************************/

void Install_DisableSpecialLifebar()
{
	static int isInstalled = false;
	if (isInstalled) return;

	log_msg("Patch D2Sigma to display standard lifebar for bosses and superunique monsters. (ForceLegacyBossLifebar)\n");

	// Disable special lifebar
	mem_seek RVA(D2Sigma, 0x60BBA6, 0x07B046, 0x07FAB5, 0x07D6FD, 0x07D79D, 0x07DEBD, 0x07F14D, 0x07F89D, 0x07FD1D);
	memt_byte(0xE8, 0x90);
	memt_dword(VAL(D2Sigma, 0xFFFE95D5, 0xFFFE6B65, 0xFFFE66F6, 0xFFFE66EE, 0xFFFE669E, 0xFFFE634E, 0xFFFE5E0E, 0xFFFE5A5E, 0xFFFE592E), 0x90909090);

	// Draw regular lifebar while hovering cursor under the target
	mem_seek RVA(D2Sigma, 0x5F532A, 0x061D5A, 0x06635A, 0x06312B, 0x06317B, 0x06354B, 0x06429B, 0x06463B, 0x06498F);
	memt_dword(0x00000201, 0x00000001); // MonStats3 flags: specLifebar|noLifebar (0x0201)

	// Display resistances and unqiue modifiers
	mem_seek RVA(D2Sigma, 0x5E66A6, 0x052266, 0x056776, 0x0534D6, 0x053556, 0x053926, 0x054546, 0x0544F6, 0x054856);
	memt_dword(0x00000200, 0x00000000); // MonStats3 flags: specLifebar (0x0200)

	log_msg("\n");

	isInstalled = true;
}

/****************************************************************************************************/

void STDCALL NotifyLockoutTimer(WCHAR (&lpText)[0x132], int nLevelId, DWORD dwTimer)
{
	LevelsBIN* ptLevelsBin = D2GetLevelsTxtRecord(nLevelId);
	if (ptLevelsBin == NULL)
		return;

	_snwprintf(lpText, _countof(lpText), getLocalString(STR_LOCKOUT_TIMER), D2GetStringFromIndex(*reinterpret_cast<WORD*>(reinterpret_cast<DWORD>(ptLevelsBin) + 0x145)), dwTimer);

	D2PushNotification(5, GOLD, 5000, lpText, FALSE);
}

FCT_ASM( caller_EventMessage0C_113 )
	LEA EAX, DWORD PTR SS:[ESP + 0x74]
	PUSH DWORD PTR SS:[EBX + 0x18]
	PUSH DWORD PTR SS:[EBX + 0x03]
	PUSH EAX
	CALL NotifyLockoutTimer
	POP EBP
	POP EDI
	POP ESI
	POP EBX
	ADD ESP, 0x2C8
	RETN 0x04
}}

void FASTCALL DispatchLockoutTimer(Game* ptGame, Unit* ptPlayer)
{
	Room* ptRoom = D2GetRoom(ptPlayer);
	if (ptRoom == NULL)
		return;

	int nLevelId = D2GetLevelID(ptRoom);
	if (nLevelId <= 0 || nLevelId >= (int)SgptDataTables->nbLevels)
		return;

	if (!(*reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(reinterpret_cast<DWORD>(ptRoom) + 0x10) + 0x58) + 0x230) & 0x01))
		return;

	DWORD dwTimer = 0;
	for (void* ptQuestControl = *reinterpret_cast<void**>(reinterpret_cast<DWORD>(ptGame) + 0x10F4), *ptTimer = *reinterpret_cast<void**>(reinterpret_cast<DWORD>(ptQuestControl) + 0x10); ptTimer != NULL; ptTimer = *reinterpret_cast<void**>(reinterpret_cast<DWORD>(ptTimer) + 0x10))
	{
		if (*reinterpret_cast<int*>(*reinterpret_cast<DWORD*>(reinterpret_cast<DWORD>(ptTimer) + 0x04) + 0x18) != nLevelId)
			continue;

		dwTimer = *reinterpret_cast<DWORD*>(reinterpret_cast<DWORD>(ptTimer) + 0x08) - *reinterpret_cast<DWORD*>(reinterpret_cast<DWORD>(ptQuestControl) + 0x14);
		if (*reinterpret_cast<DWORD*>(reinterpret_cast<DWORD>(ptTimer) + 0x0C) > 20)
		{
			DWORD dwDecrement = dwTimer < 5 ? dwTimer : 5;
			*reinterpret_cast<DWORD*>(reinterpret_cast<DWORD>(ptTimer) + 0x08) -= dwDecrement;
			dwTimer -= dwDecrement;
		}

		break;
	}

	BYTE packet[0x28];
	ZeroMemory(packet, sizeof(packet));

	packet[0x00] = 0x5A;
	packet[0x01] = 0x0C;
	*reinterpret_cast<int*>(&packet[0x03]) = nLevelId;
	*reinterpret_cast<DWORD*>(&packet[0x18]) = dwTimer;

	for (NetClient* ptClient = *reinterpret_cast<NetClient**>(reinterpret_cast<DWORD>(ptGame) + 0x88); ptClient != NULL; ptClient = *reinterpret_cast<NetClient**>(reinterpret_cast<DWORD>(ptClient) + 0x4A8))
		D2SendPacket(ptClient, &packet, sizeof(packet));
}

FCT_ASM( caller_PlayerDeathEvent_113 )
	MOV EDX, ESI
	MOV ECX, EBP
	CALL DispatchLockoutTimer
	RETN
}}

void Install_RemoveResurrectionDelay()
{
	static int isInstalled = false;
	if (isInstalled) return;

	log_msg("Patch D2Sigma, D2Game & D2Client to remove player respawn cooldown. (RemoveRespawnCooldown)\n");

	// Manage event message
	mem_seek(offset_D2Client + 0x07E6BC);
	MEMT_DWORD(offset_D2Client + 0x07E67C, caller_EventMessage0C_113);

	// Player mode change hook
	mem_seek(offset_D2Game + 0x0797A8);
	memt_byte(0x83, 0xE8);
	MEMT_REF4(0x0E75003E, caller_PlayerDeathEvent_113);

	// Init player resurrection
	mem_seek RVA(D2Sigma, 0x601776, 0x0700E6, 0x074506, 0x072356, 0x072416, 0x072876, 0x0737F6, 0x073BE6, 0x073F56);
	memt_byte(0x89, 0x90);
	memt_byte(version_D2Sigma < MXLS_134 ? 0x48 : 0x41, 0x90);
	memt_byte(0x10, 0x90);

	// Manage player resurrection
	mem_seek RVA(D2Sigma, 0x5FD095, 0x06A7B5, 0x06EBA5, 0x06BD85, 0x06BE45, 0x06C285, 0x06CFF5, 0x06D3A5, 0x06D725);
	memt_dword(0x507DC083, 0x9090006A);

	log_msg("\n");

	isInstalled = true;
}

/*================================= END OF FILE =================================*/