/*=================================================================
	File created by Yohann NICOLAS.

	Unassign Skill Point for futher re-assignment.

=================================================================*/

#include "updateServer.h"
#include "interface_Skills.h"	// Install_InterfaceSkills()
#include "common.h"

bool active_SkillsPoints=0;
bool unassignSkillsPointsOneByOne=0;

int (*getskPoint)(SkillData* ptSkill);
int (*getCharClass)(SkillData* ptSkill);
TD2GetPlayerStat getSingleSkillValue;

int getskPoint_109(SkillData* ptSkill) {return -1;}
int getskPoint_11X(SkillData* ptSkill) {return ptSkill->ptSkillBin->skpoints;}

int getCharClass_109(SkillData* ptSkill) {return *(&ptSkill->ptSkillBin->charclass+8);}
int getCharClass_11X(SkillData* ptSkill) {return ptSkill->ptSkillBin->charclass;}

int STDCALL getSingleSkillValue_109(Unit* ptChar, DWORD , DWORD skillID)
{
	DWORD stat;
	DWORD ret=0;
	stat = D2GetPlayerStat(ptChar, STATS_ITEM_SINGLESKILL+0, skillID);
	if ((stat & 255) == skillID) ret += (stat >> 8) & 255;
	stat = D2GetPlayerStat(ptChar, STATS_ITEM_SINGLESKILL+1, skillID);
	if ((stat & 255) == skillID) ret += (stat >> 8) & 255;
	stat = D2GetPlayerStat(ptChar, STATS_ITEM_SINGLESKILL+2, skillID);
	if ((stat & 255) == skillID) ret += (stat >> 8) & 255;
	return ret;
}

void UnassignAllSkillsPoints(Unit* ptChar)
{
	int nbPoints = 0;
	NetClient* ptClient = D2GetClient(ptChar, __FILE__, __LINE__);
	for (int skillNo = 0, skillCount = D2GetPlayerSkillCount(ptChar->nPlayerClass); skillNo < skillCount; skillNo++)
	{
		int skillId = D2GetClassSkillId(ptChar->nPlayerClass, skillNo);
		if (SkillData* ptSkill = D2GetSkillById(ptChar, skillId, -1))
		{
			nbPoints += D2GetSkillLevel(ptChar, ptSkill, FALSE);
			D2SetSkillLevel(ptChar, skillId, 0, TRUE);
			D2UpdateSkillLevel(ptClient, ptChar, skillId, 0, TRUE);
		}
	}
	D2AddPlayerStat(ptChar, STATS_NEWSKILLS, nbPoints, 0);
}

void Install_SkillsPoints()
{
	static int isInstalled = false;
	if (isInstalled) return;

	Install_InterfaceSkills();
	Install_UpdateServer();

	if ( version_D2Common >= V110 )
	{
		getskPoint = getskPoint_11X;
		getCharClass = getCharClass_11X;
		getSingleSkillValue = D2GetPlayerStat;
	} else {
		getskPoint = getskPoint_109;
		getCharClass = getCharClass_109;
		getSingleSkillValue = getSingleSkillValue_109;
	}

	isInstalled = true;
}

/*================================= END OF FILE =================================*/