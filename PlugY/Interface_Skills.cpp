/*=================================================================
	File created by Yohann NICOLAS.
	Add support 1.13d by L'Autour.

	Interface functions

=================================================================*/

#include "updateServer.h"	// Install_UpdateServer()
#include "plugYFiles.h"		// Install_PlugYImagesFiles()
#include "skillsPoints.h"
#include "common.h"
#include <stdio.h>

int posXUnassignSkillBtn=-1;
int posYUnassignSkillBtn=-1;

DWORD	btnSkillIsDown;

#define	getXSkillBtn()					RX(936)
#define	getLSkillBtn()					32
#define	getYSkillBtn()					RY(696)
#define	getHSkillBtn()					32
#define isOnButtonUnassignSkill(x,y)	isOnRect(x, y, getXSkillBtn(), getYSkillBtn(), getLSkillBtn(), getHSkillBtn())

void STDCALL printSkillsPageBtns()
{
	if (onRealm) return;
	
	sDrawImageInfo data;
	ZeroMemory(&data, sizeof(data));

	setImage(&data, *ptStatsFrameGfx);
	setFrame(&data, 0);
	D2PrintImage(&data, getXSkillBtn() - 2, getYSkillBtn() + 2, -1, 5, 0);

	setImage(&data, unassignSkillsBtnImages);
	setFrame(&data, btnSkillIsDown);
	D2PrintImage(&data, getXSkillBtn(), getYSkillBtn(), -1, 5, 0);

	D2SetFont(1);
	if (isOnButtonUnassignSkill(D2GetMouseX(), D2GetMouseY()))
	{
		LPWSTR popupText = getLocalString(STR_SKILLS_UNASSIGN);
		D2PrintPopup(popupText, getXSkillBtn() + getLSkillBtn() / 2, getYSkillBtn() - getHSkillBtn(), 0, 1);
	}
}

DWORD STDCALL skillsPageMouseDown(sWinMessage* msg)
{
	if (onRealm) return 1;

	if (isOnButtonUnassignSkill(msg->x, msg->y))
	{
		log_msg("push down left button unassign skill\n");
		btnSkillIsDown = 1;
		D2PlaySound(4, 0, 0, 0, 0);
		freeMessage(msg);
		return 0;
	}

	return 1;
}

DWORD STDCALL skillsPageMouseUp(sWinMessage* msg)
{
	if (onRealm) return 1;

	if (isOnButtonUnassignSkill(msg->x, msg->y))
	{
		log_msg("push up left button unassign skill\n");
		if (btnSkillIsDown)
		{
			updateServer(US_UNASSIGN_SKILLS);
			freeMessage(msg);
			return 0;
		}
	}

	return 1;
}

FCT_ASM( caller_printSkillsPage_mxls )
	CALL D2DrawSkillsPage
	CALL printSkillsPageBtns
	RETN
}}

FCT_ASM ( caller_printSkillsPageBtns_111 )
	CALL printSkillsPageBtns
	POP EDI
	POP ESI
	POP EBX
	RETN
}}


FCT_ASM ( caller_printSkillsPageBtns )
	CALL printSkillsPageBtns
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	ADD ESP,0x48
	RETN
}}

FCT_ASM ( caller_DontPrintSkillPointsRemaining_111 )
	MOV AL,BYTE PTR DS:[onRealm]
	TEST AL,AL
	JNZ dontPrint
	ADD DWORD PTR SS:[ESP],0xF2
	RETN
dontPrint:
	MOV ECX,0x1083
	RETN
}}

FCT_ASM ( caller_DontPrintSkillPointsRemaining )
	MOV AL,BYTE PTR DS:[onRealm]
	TEST AL,AL
	JNZ dontPrint
	ADD DWORD PTR SS:[ESP],0x9D
	RETN
dontPrint:
	MOV ECX,0x1083
	RETN
}}

FCT_ASM( caller_skillsPageMouseDown_mxls )
	MOV EBP, DWORD PTR SS:[ESP+0x24]
	PUSH EBP
	CALL skillsPageMouseDown
	TEST EAX, EAX
	RETN
}}

FCT_ASM ( caller_skillsPageMouseDown_111 )
	PUSH EBP
	CALL skillsPageMouseDown
	RETN
}}


FCT_ASM ( caller_skillsPageMouseDown )
	PUSH EBX
	CALL skillsPageMouseDown
	RETN
}}

FCT_ASM( caller_skillsPageMouseUp_mxls )
	MOV ECX, DWORD PTR SS:[ESP+0x40]
	PUSH ECX
	PUSH ECX
	CALL skillsPageMouseUp
	MOV btnSkillIsDown, 0
	TEST EAX, EAX
	POP ECX
	RETN
}}

FCT_ASM ( caller_skillsPageMouseUp )
	CALL skillsPageMouseUp
	JMP D2FreeWinMessage
}}

void Install_InterfaceSkills()
{
	static int isInstalled = false;
	if (isInstalled) return;

	Install_UpdateServer();
	Install_PlugYImagesFiles();

	log_msg("Patch D2Sigma for skills interface. (InterfaceSkills)\n");

	// Skills page drawing hook
	mem_seek RVA(D2Sigma, 0x60BAA8, 0x07AF48, 0x07F9B7, 0x07D669, 0x07D709, 0x07DE29, 0x07F0B9, 0x07F809, 0x07FC89);
	MEMC_REF4(D2DrawSkillsPage, caller_printSkillsPage_mxls);

	/*
	// Print new buttons images
	mem_seek R7(D2Client, 7AC20, 7AC20, 77073, 16190, 8A9C0, 7F320, 77F20, 2F380);
	memt_byte( 0x5F, 0xE9 );	// JMP caller_printBtns
	if ( version_D2Client >= V111 ) {
		MEMT_REF4( 0xCCC35B5E, caller_printSkillsPageBtns_111);
		//6FAC6190   5F               POP EDI
		//6FAC6191   5E               POP ESI
		//6FAC6192   5B               POP EBX
		//6FAC6193   C3               RETN
		//6FAC6194   CC               INT3
		//6FB3A9C0  |> 5F             POP EDI
		//6FB3A9C1  |. 5E             POP ESI
		//6FB3A9C2  |. 5B             POP EBX
		//6FB3A9C3  \. C3             RETN
		//6FB3A9C4     CC             INT3
		//6FB2F320  |> 5F             POP EDI
		//6FB2F321  |. 5E             POP ESI
		//6FB2F322  |. 5B             POP EBX
		//6FB2F323  \. C3             RETN
		//6FB2F324     CC             INT3
		//6FB27F20  |> 5F             POP EDI
		//6FB27F21  |. 5E             POP ESI
		//6FB27F22  |. 5B             POP EBX
		//6FB27F23  \. C3             RETN
		//6FB27F24     CC             INT3
		//6FADF380  |> 5F             POP EDI
		//6FADF381  |. 5E             POP ESI
		//6FADF382  |. 5B             POP EBX
		//6FADF383  \. C3             RETN
		//6FADF384     CC             INT3
	} else {
		MEMT_REF4( 0x835B5D5E, caller_printSkillsPageBtns);
		memt_byte( 0xC4, 0x90 );	// NOP
		memt_byte( 0x48, 0x90 );	// NOP
		memt_byte( 0xC3, 0x90 );	// NOP
		//6FB17073  |. 5F             POP EDI
		//6FB17074  |. 5E             POP ESI
		//6FB17075  |. 5D             POP EBP
		//6FB17076  |. 5B             POP EBX
		//6FB17077  |. 83C4 48        ADD ESP,48
		//6FB1707A  \. C3             RETN
	}
	*/

	if (posXUnassignSkillBtn==-1 && posYUnassignSkillBtn==-1)
	{
		// Don't print "Skill Points Remaining"
		mem_seek RVA(D2Sigma, 0x604BE1, 0x073C81, 0x078651, 0x0763D1, 0x076471, 0x076B81, 0x077D61, 0x078461, 0x0787B1);
		memt_byte(0xE8, 0x90);
		memt_dword(VAL(D2Sigma, 0xFFFDFD5A, 0xFFFDC2FA, 0xFFFDBE3A, 0xFFFDAA5A, 0xFFFDAA3A, 0xFFFDA68A, 0xFFFD98CA, 0xFFFD91CA, 0xFFFD946A), 0x90909090);
		/*
		mem_seek R7(D2Client, 7AC30, 7AC30, 77080, 16294, 8AC74, 7ECF4, 78334, 2F7E4);
		memt_byte( 0xB9, 0xE8 );
		MEMT_REF4( 0x00001083, version_D2Client >= V111 ? caller_DontPrintSkillPointsRemaining_111 : caller_DontPrintSkillPointsRemaining);
		//6FB17080  /$ B9 83100000    MOV ECX,1083
		//6FAC6294  |. B9 83100000    MOV ECX,1083
		//6FB3AC74  |. B9 83100000    MOV ECX,1083
		//6FB2ECF4  |. B9 83100000    MOV ECX,1083
		//6FB28334  |. B9 83100000    MOV ECX,1083
		//6FADF7E4  |. B9 83100000    MOV ECX,1083
		*/
	}

	// Manage mouse down (Play sound)
	mem_seek RVA(D2Sigma, 0x604174, 0x073214, 0x077BE4, 0x075924, 0x0759D4, 0x0760C4, 0x0772A4, 0x077984, 0x077CF4);
	memt_byte( 0x8B, 0xE8 );
	MEMT_REF4( 0x8520246C, caller_skillsPageMouseDown_mxls);
	memt_byte( 0xED, 0x90 );
	/*
	mem_seek R7(D2Client, 7BBD1, 7BBD1, 780E4, 17BC2, 8C6E2, 808B2, 79C62, 31112);
	memt_byte( 0xC7, 0xE8 );	// CALL caller_skillsPageMouseDown
	MEMT_REF4( version_D2Client >= V111 ? 0x00001845 : 0x00001843, version_D2Client >= V111 ? caller_skillsPageMouseDown_111 : caller_skillsPageMouseDown);
	memt_byte( 0x00, 0x90 );	// NOP
	memt_byte( 0x00, 0x90 );	// NOP
	//6FB180E4   > C743 18 00000000     MOV DWORD PTR DS:[EBX+18],0
	//6FAC7BC2   > C745 18 00000000     MOV DWORD PTR SS:[EBP+18],0
	//6FB3C6E2   > C745 18 00000000     MOV DWORD PTR SS:[EBP+18],0
	//6FB308B2   > C745 18 00000000     MOV DWORD PTR SS:[EBP+18],0
	//6FB29C62   > C745 18 00000000     MOV DWORD PTR SS:[EBP+18],0
	//6FAE1112   > C745 18 00000000     MOV DWORD PTR SS:[EBP+18],0
	*/

	// Manage mouse up
	mem_seek RVA(D2Sigma, 0x6042C0, 0x073360, 0x077D30, 0x075A70, 0x075B20, 0x076210, 0x0773F0, 0x077AD0, 0x077E40);
	memt_dword(0x04244C8B, 0xE838EC83);
	MEMT_REF4(0x8538EC83, caller_skillsPageMouseUp_mxls);
	memt_byte(0xC9, 0x90);
	/*
	mem_seek R7(D2Client, 7BC40, 7BC40, 78466, 17558, 8C078, 80248, 795F8, 30AA8);
	MEMJ_REF4( D2FreeWinMessage, caller_skillsPageMouseUp);//0xFFF93B0A
	//6FB18465   . E8 C07D0400    CALL <JMP.&Storm.#511>
	//6FAC7557   .^E9 4248FFFF    JMP <JMP.&Storm.#511>
	//6FB3C077   .^E9 16FDF7FF    JMP <JMP.&Storm.#511>
	//6FB30247   .^E9 2CBCF8FF    JMP <JMP.&Storm.#511>
	//6FB295F7   .^E9 8828F9FF    JMP <JMP.&Storm.#511>
	//6FAE0AA7   .^E9 E0B2FDFF    JMP <JMP.&Storm.#511>
	*/

	log_msg("\n");

	isInstalled = true;
}

/*================================= END OF FILE =================================*/