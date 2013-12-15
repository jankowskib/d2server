/* ==========================================================
 * d2warden
 * https://github.com/lolet/d2warden
 * ==========================================================
 * Copyright 2011-2013 Bartosz Jankowski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ========================================================== */

#include "stdafx.h"

#include "StatDumpThread.h"

#include "D2Warden.h"

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
	if(!D2Funcs::D2NET_GetClient(ptCurrentClient->ClientID)) continue;
	if(!ptCurrentClient->ptClientData) continue;
	if(ptCurrentClient->ptClientData->InitStatus!=4) continue;
	
	UnitAny* ptUnit = ptCurrentClient->ptPlayer;

			int sFR =   D2Funcs::D2COMMON_GetStatSigned(ptUnit,STAT_FIRERESIST,0);
			int sCR =   D2Funcs::D2COMMON_GetStatSigned(ptUnit,STAT_COLDRESIST,0);
			int sLR =   D2Funcs::D2COMMON_GetStatSigned(ptUnit,STAT_LIGHTNINGRESIST,0);
			int sPR =   D2Funcs::D2COMMON_GetStatSigned(ptUnit,STAT_POISONRESIST,0);
			 
			int sRL = D2Funcs::D2COMMON_GetStatSigned(ptUnit,STAT_HPREGEN,0);

			int sFCR =  D2Funcs::D2COMMON_GetStatSigned(ptUnit,STAT_FASTERCAST,0);
			int sFHR =  D2Funcs::D2COMMON_GetStatSigned(ptUnit,STAT_FASTERHITRECOVERY,0);
			int sIAS =	D2Funcs::D2COMMON_GetStatSigned(ptUnit,STAT_IAS,0);
			int sFRW =  D2Funcs::D2COMMON_GetStatSigned(ptUnit,STAT_FASTERRUNWALK,0);

			int sCABS = D2Funcs::D2COMMON_GetStatSigned(ptUnit,STAT_COLDABSORBPERCENT,0);
			int sLABS = D2Funcs::D2COMMON_GetStatSigned(ptUnit,STAT_LIGHTNINGABSORBPERCENT,0);
			int sFABS = D2Funcs::D2COMMON_GetStatSigned(ptUnit,STAT_FIREABSORBPERCENT,0);

			if(D2Funcs::D2COMMON_GetUnitState(ptUnit,83))  //Nat res
			{
			Skill* pSkill = D2Funcs::D2COMMON_GetSkillById(ptUnit,153,-1);
				if(pSkill)
				{
				int SkillLvl = D2Funcs::D2COMMON_GetSkillLevel(ptUnit,pSkill,1);
				int ResBonus = D2Funcs::D2COMMON_EvaluateSkill(ptUnit,3442,153,SkillLvl);
				sFR-=ResBonus;
				sCR-=ResBonus;
				sLR-=ResBonus;
				sPR-=ResBonus;
				}
			}
			if(D2Funcs::D2COMMON_GetUnitState(ptUnit,8)) //Salv
			{
				StatList * Stats = D2Funcs::D2COMMON_GetStateStatList(ptUnit,8);
				if(Stats) 
				{
				int ResBonus = Stats->Stats.pStat->dwStatValue;
				sFR-=ResBonus;
				sCR-=ResBonus;
				sLR-=ResBonus;
				}
			}
			Room1* aRoom = D2Funcs::D2COMMON_GetUnitRoom(ptUnit);
			int LvlId = 0;
			if(aRoom) LvlId =D2Funcs::D2COMMON_GetLevelNoByRoom(aRoom);
			char * sLoc = ConvertLevel(LvlId);
			char * aClass = ConvertClass(ptUnit->dwClassId);
			LogToFile("RealmStats.txt",0,"%s\t%s\t%s\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",ptCurrentClient->CharName.c_str(),ptCurrentClient->AccountName.c_str(),aClass,ptCurrentClient->ptClientData->pGame->GameName,sLoc,sFR,sCR,sLR,sPR,sFCR,sFHR,sIAS,sFRW,sFABS,sCABS,sLABS,sRL);
}
UNLOCK
LogToFile("RealmStats.txt",1,"Dumping end (%d) ms...",GetTickCount()-Time);
Sleep(DumpInterval * 1000);
}
#ifdef _ENGLISH_LOGS
Log("End of dump thread!");
#else
Log("Koniec watku zrzutu!");
#endif
return 0;
}