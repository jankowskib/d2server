#include "stdafx.h"

#include "StatDumpThread.h"
#include "D2Ptrs.h"
#include "D2Stubs.h"
#include "D2Warden.h"
#include "WardenMisc.h"
#include "D2Structs.h"
#include "Vars.h"

unsigned __stdcall StatThread(void* Params)
{
while (WaitForSingleObject(hEvent, 0) != WAIT_OBJECT_0) 
{
if(!DumpInterval) return 0;
remove("RealmStats.txt");
int Time = GetTickCount();
LogToFile("RealmStats.txt",1,"Creating dump...");
LogToFile("RealmStats.txt",0,"CharName\tAccount	Class\tGame\tLocation\tFire Res\tCold Res\tLight Res\tPoison Res\tFCR\tFHR\tIAS\tFRW\tFire ABS\tCold Abs\tLight Abs\tRL");

LOCK
	
for(list<WardenClient>::iterator ptCurrentClient = hWarden.Clients.begin(); ptCurrentClient !=hWarden.Clients.end(); ++ptCurrentClient)
{	
	if(ptCurrentClient->WardenStatus == WARDEN_START) continue;
	if(!D2NET_GetClient(ptCurrentClient->ClientID)) continue;
	if(!ptCurrentClient->ptClientData) continue;
	if(ptCurrentClient->ptClientData->InitStatus!=4) continue;
	
	UnitAny* ptUnit = ptCurrentClient->ptPlayer;

			int sFR =   D2COMMON_GetStatSigned(ptUnit,STAT_FIRERESIST,0);
			int sCR =   D2COMMON_GetStatSigned(ptUnit,STAT_COLDRESIST,0);
			int sLR =   D2COMMON_GetStatSigned(ptUnit,STAT_LIGHTNINGRESIST,0);
			int sPR =   D2COMMON_GetStatSigned(ptUnit,STAT_POISONRESIST,0);
			 
			int sRL = D2COMMON_GetStatSigned(ptUnit,STAT_HPREGEN,0);

			int sFCR =  D2COMMON_GetStatSigned(ptUnit,STAT_FASTERCAST,0);
			int sFHR =  D2COMMON_GetStatSigned(ptUnit,STAT_FASTERHITRECOVERY,0);
			int sIAS =	D2COMMON_GetStatSigned(ptUnit,STAT_IAS,0);
			int sFRW =  D2COMMON_GetStatSigned(ptUnit,STAT_FASTERRUNWALK,0);

			int sCABS = D2COMMON_GetStatSigned(ptUnit,STAT_COLDABSORBPERCENT,0);
			int sLABS = D2COMMON_GetStatSigned(ptUnit,STAT_LIGHTNINGABSORBPERCENT,0);
			int sFABS = D2COMMON_GetStatSigned(ptUnit,STAT_FIREABSORBPERCENT,0);

			if(D2COMMON_GetUnitState(ptUnit,83))  //Nat res
			{
			Skill* pSkill = D2COMMON_GetSkillById(ptUnit,153,-1);
				if(pSkill)
				{
				int SkillLvl = D2COMMON_GetSkillLevel(ptUnit,pSkill,1);
				int ResBonus = D2COMMON_EvaluateSkill(ptUnit,3442,153,SkillLvl);
				sFR-=ResBonus;
				sCR-=ResBonus;
				sLR-=ResBonus;
				sPR-=ResBonus;
				}
			}
			if(D2COMMON_GetUnitState(ptUnit,8)) //Salv
			{
				StatList * Stats = D2COMMON_GetStateStatList(ptUnit,8);
				if(Stats) 
				{
				int ResBonus = Stats->Stats.pStat->dwStatValue;
				sFR-=ResBonus;
				sCR-=ResBonus;
				sLR-=ResBonus;
				}
			}
			Room1* aRoom = D2COMMON_GetUnitRoom(ptUnit);
			int LvlId = 0;
			if(aRoom) LvlId =D2COMMON_GetLevelNoByRoom(aRoom);
			char * sLoc = ConvertLevel(LvlId);
			char * aClass = ConvertClass(ptUnit->dwClassId);
			LogToFile("RealmStats.txt",0,"%s\t%s\t%s\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",ptCurrentClient->CharName.c_str(),ptCurrentClient->AccountName.c_str(),aClass,ptCurrentClient->ptClientData->pGame->GameName,sLoc,sFR,sCR,sLR,sPR,sFCR,sFHR,sIAS,sFRW,sFABS,sCABS,sLABS,sRL);
}
UNLOCK
LogToFile("RealmStats.txt",1,"Dumping end (%d) ms...",GetTickCount()-Time);
Sleep(DumpInterval * 1000);
}
Log("Koniec watku zrzutu!");
	return 0;
}