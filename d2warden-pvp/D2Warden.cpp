/* ==========================================================
 * d2warden
 * https://github.com/lolet/d2warden
 * ==========================================================
 * Copyright 2008 marsgod
 *			 2011-2014 Bartosz Jankowski
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

#include "D2Warden.h"
#include "WardenChecks.h"
#include "StatDumpThread.h"

#include "LQuests.h"
#include "LItems.h"
#include "LWorldEvent.h"

#include "RC4.h"
#include "global.h"
#include "MD5.h"
#include "process.h"

#include "Build.h"

using namespace std;
//#pragma comment(lib, "urlmon.lib")

static boost::mt19937 rng;
static boost::uniform_int<int> dist(300, 10000);

DWORD SendPartOfMOD2Client(DWORD ClientID, unsigned char *RC4_KEY,DWORD MOD_Position)
{
	unsigned char MyMod[0x200];
	int ModLen;
	
	if (MOD_Length - 0x1f4 > MOD_Position) ModLen = 0x1f4;
	else
		ModLen = MOD_Length - MOD_Position;
	MyMod[0] = 0xAE;
	MyMod[1] = (unsigned char)((ModLen+3)%256);
	MyMod[2] = (unsigned char)((ModLen+3)>>8);
	MyMod[3] = 1;
	MyMod[4] = (unsigned char)(ModLen%256);
	MyMod[5] = (unsigned char)(ModLen>>8);
	memcpy(&MyMod[6],MOD_Enrypted+MOD_Position,ModLen);
	rc4_crypt(RC4_KEY,&MyMod[3],ModLen+3);
	D2Funcs.D2NET_SendPacket(0,ClientID,MyMod,ModLen+6);
	//D2ASMFuncs::D2GAME_SendPacket(ptClient,MyMod,ModLen+6);
	return ModLen+MOD_Position;
};

//void Warden_DownloadIni()
//{
//	if(URLDownloadToFile(0,"http://realm.angrenost.org//txt//Colors.ini","Colors.ini",0,0) == S_OK)
//	{
//#ifdef _ENGLISH_LOGS
//	Log("Downloading configuration file...");
//#else
//	Log("Pobieram plik konfiguracyjny!");
//#endif
//	}
//	else
//	{
//#ifdef _ENGLISH_LOGS
//	Log("An error occured during configuration file download");
//#else
//	Log("Wystapil blad podczas pobierania pliku konfiguracyjnego");
//#endif
//	}
//}

void Warden_Config()
{
	char *tk =0, *rt =0;
	char temp[100] = {0};
	char filename[MAX_PATH] = {0};
	char URL[255] = {0};
	char ITEM[255] = {0};
	GetCurrentDirectory(MAX_PATH,filename);
	wcfgConfigFile.assign(filename);
	wcfgConfigFile+="\\D2Warden.ini";

	GetPrivateProfileString("Warden", "AllowTeleport", "1,5", temp, 100, wcfgConfigFile.c_str());
	for (rt = strtok_s(temp, ", ", &tk); rt; rt = strtok_s(NULL, ", ", &tk))
		if (atoi(rt) < 7) TeleChars[atoi(rt)] = TRUE;

	wcfgD2EXVersion = GetPrivateProfileInt("Warden", "D2ExVersion", 16, wcfgConfigFile.c_str());
	wcfgSpectator = GetPrivateProfileInt("Warden", "EnableSpectator", 1, wcfgConfigFile.c_str());
	wcfgAddKillerClass = GetPrivateProfileInt("Warden", "AddKillerClass", 0, wcfgConfigFile.c_str());
	wcfgAutoIdentify = GetPrivateProfileInt("Warden", "AutoIdentify", 0, wcfgConfigFile.c_str());
	wcfgHostileLevel = GetPrivateProfileInt("Warden", "HostileLevel", 0, wcfgConfigFile.c_str());
	wcfgRespawnTimer = GetPrivateProfileInt("Warden", "RespawnTimer", 0, wcfgConfigFile.c_str());
	wcfgDisableHostileDelay = GetPrivateProfileInt("Warden", "DisableHostileDelay", 1, wcfgConfigFile.c_str());
	wcfgAllowTourMode = GetPrivateProfileInt("Warden", "AllowTourMode", 1, wcfgConfigFile.c_str());
	wcfgAllowVanilla = GetPrivateProfileInt("Warden", "AllowVanilla", 0, wcfgConfigFile.c_str());
	wcfgAllowGU = GetPrivateProfileInt("Warden", "AllowGU", 0, wcfgConfigFile.c_str());
	wcfgAllowHB = GetPrivateProfileInt("Warden", "AllowHB", 1, wcfgConfigFile.c_str());
	wcfgAllowNLWW = GetPrivateProfileInt("Warden", "AllowNLWW", 0, wcfgConfigFile.c_str());
	wcfgAllowLoggin = GetPrivateProfileInt("Warden", "LogChat", 1, wcfgConfigFile.c_str());
	wcfgDumpInterval = GetPrivateProfileInt("Warden", "ResDumpInterval", 0, wcfgConfigFile.c_str());
	wcfgMaxPlayers = GetPrivateProfileInt("Warden", "MaxPlayers", 8, wcfgConfigFile.c_str());
	wcfgMoveToTown = GetPrivateProfileInt("Warden", "MoveToTown", 1, wcfgConfigFile.c_str());
	wcfgDropEarGold = GetPrivateProfileInt("Warden", "DontDropEarGold", 1, wcfgConfigFile.c_str());
	wcfgStrBugFix = GetPrivateProfileInt("Warden", "StrBugFix", 1, wcfgConfigFile.c_str());
	wcfgFFAMode = GetPrivateProfileInt("Warden", "FFAMode", 1, wcfgConfigFile.c_str());
	wcfgEnableSeed = GetPrivateProfileInt("Warden", "EnableSeedChange", 1, wcfgConfigFile.c_str());
	wcfgAllowD2Ex = GetPrivateProfileInt("Warden", "AllowD2Ex", 1, wcfgConfigFile.c_str());
	wcfgDetectTrick = GetPrivateProfileInt("Warden", "DetectTrick", 1, wcfgConfigFile.c_str());
	wcfgExpRange = GetPrivateProfileInt("Warden", "ExpRange", 6400, wcfgConfigFile.c_str());
	wcfgDisableRepairCost = GetPrivateProfileInt("Warden", "DisableRepairCosts", 1, wcfgConfigFile.c_str());
	//DmgRekord = GetPrivateProfileInt("Warden","DmgRekord",1,wcfgConfigFile.c_str());
	GetPrivateProfileString("Warden", "ClanDataURL", "http://www.lolet.yoyo.pl/Clans.ini", URL, 255, wcfgConfigFile.c_str());
	wcfgClansURL = URL;
	GetPrivateProfileString("Warden", "UpdateURL", "", URL, 255, wcfgConfigFile.c_str());
	wcfgUpdateURL = URL;
	//GetPrivateProfileString("Warden","DmgOwner","N/A",URL,255,wcfgConfigFile.c_str());
	//DmgOwner = URL;
	GetPrivateProfileString("Warden", "GSName", "N/A", URL, 255, wcfgConfigFile.c_str());
	wcfgGSName = URL;
	GetPrivateProfileString("Warden", "Admins", "LOLET", URL, 255, wcfgConfigFile.c_str());
	tk = 0;
	for (rt = strtok_s(URL, ", ", &tk); rt; rt = strtok_s(NULL, ", ", &tk))
	{
		boost::to_lower(rt);
		if (rt[0]) wcfgAdmins.push_back(rt);
	}

	GetPrivateProfileString("Warden", "FGDatabaseName", "projectdvb", URL, 255, wcfgConfigFile.c_str());
	wcfgDatabase = URL;
	GetPrivateProfileString("Warden", "FGDatabaseUser", "root", URL, 255, wcfgConfigFile.c_str());
	wcfgDBUser = URL;
	GetPrivateProfileString("Warden", "FGDatabasePass", "", URL, 255, wcfgConfigFile.c_str());
	wcfgDBPass = URL;

	//World Event Stuff
	wcfgEnableWE = GetPrivateProfileInt("World Event", "Enabled", 0, wcfgConfigFile.c_str());


	if (wcfgEnableWE)
	{
		SellCount = GetPrivateProfileInt("World Event", "SellCount", 0, wcfgConfigFile.c_str());
		NextDC = GetPrivateProfileInt("World Event", "NextDC", 0, wcfgConfigFile.c_str());
		MinSell = GetPrivateProfileInt("World Event", "MinSell", 10, wcfgConfigFile.c_str());
		MaxSell = GetPrivateProfileInt("World Event", "MaxSell", 50, wcfgConfigFile.c_str());
		InfoDelay = GetPrivateProfileInt("World Event", "InfoDelay", 10, wcfgConfigFile.c_str());
		if (!InfoDelay)
		{
			Log("Invalid InfoDelay value. Defaulting to 1.");
			InfoDelay = 1;
		}
		GetPrivateProfileString("World Event", "Trigger", "rin, unique, 122", ITEM, 255, wcfgConfigFile.c_str());

		string sItem = ITEM;
		boost::to_lower(sItem);

		typedef boost::tokenizer<boost::char_separator<char>>   tokenizer;
		boost::char_separator<char> sep(", ");
		boost::char_separator<char> subsep(": ");
		tokenizer tokens(sItem, sep);
		memset(&WItem.ItemCode,0,20*4);
		memset(&WItem.FileIdx[0], -1,20*4);
		memset(&WItem.ItemQuality[0], -1,7);

		int ntk =  1;
		for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); tok_iter++,ntk++)
		{
		switch(ntk)
		{
		case 1:
			{
			tokenizer subtok(*tok_iter, subsep);
			int stk = 0;
			for (tokenizer::iterator iter = subtok.begin(); iter != subtok.end(); iter++,stk++)
			{
			WItem.ItemCode[stk] = TransCode((*iter).c_str());
			}
			}
		break;
		case 2:
			{
			tokenizer subtok(*tok_iter, subsep);
			int stk = 0;
			for (tokenizer::iterator iter = subtok.begin(); iter != subtok.end(); iter++,stk++)
			WItem.ItemQuality[stk] = TransQuality((*iter).c_str());
			}
		break;
		case 3:
			{
			tokenizer subtok(*tok_iter, subsep);
			int stk = 0;
			for (tokenizer::iterator iter = subtok.begin(); iter != subtok.end(); iter++,stk++)
			WItem.FileIdx[stk] = atoi((*iter).c_str());
			}
		break;
		}
		}

		if(!NextDC) WE_GenerateNextDC();

	}
//	Warden_DownloadIni();
#ifdef _ENGLISH_LOGS
	Log("Settings loaded.");
#else
	Log("Ustawienia zaladowane.");
#endif
}

void Warden_Init()
{
	char Warden_MOD[256] = "3ea42f5ac80f0d2deb35d99b4e9a780b.mod";
	unsigned char RC4_Key[17] = "WardenBy_Marsgod"; //3ea42f5ac80f0d2deb35d99b4e9a780b98ff
	char Maiev[10] = "MAIEV.MOD";
	FILE * fp = 0;
	int i = 0;

#ifdef _ENGLISH_LOGS
	Log("Warden ver. 1.6b build %d (%s) by Lolet has started. Compiled on %s, %s", __BUILDNO__,
#ifdef VER_111B
		"1.11b",
#elif defined VER_113D
		"1.13d",
#endif
		__DATE__, __TIME__);
#else
	Log("Warden ver. 1.6a build %d (%s) by Lolet rozpoczal dzialanie. Skompilowano %s o %s",__BUILDNO__,
#ifdef VER_111B
		"1.11b",
#elif defined VER_113D
		"1.13d",
#endif
		__DATE__,__TIME__);
#endif

	Warden_Config();

	//Zmieniam dozwolona liczbê graczy
#ifdef _ENGLISH_LOGS
	Log("Setting allowed player limit on %d", wcfgMaxPlayers);
#else
	Log("Ustalam dozwolona liczbe graczy w grze na %d", wcfgMaxPlayers);
#endif

#ifdef _ENGLISH_LOGS
	Log("Patching game...");
#else
	Log("Przeprogramowuje gre...");
#endif

	PatchD2();

	if (strlen(Warden_MOD) != 36)
	{
#ifdef _ENGLISH_LOGS
		Log("Incorrect name of module '%s' (len!=36)",Warden_MOD);
#else
		Log("Nieprawidlowa nawa modulu '%s' (len!=36)",Warden_MOD);
#endif
		Warden_Enable = false;
		return;
	}

	fopen_s(&fp,Warden_MOD,"rb");
	if (!fp)
	{
#ifdef _ENGLISH_LOGS
		Log("Cannot find module filename '%s'!",Warden_MOD);
#else
		Log("Nie moge znalesc pliku modulu '%s'!",Warden_MOD);
#endif
		Warden_Enable = false;
		return;
	}
	fseek(fp,0,SEEK_END);
	MOD_Length = ftell(fp);
	
	if (MOD_Length <= 260 || MOD_Length > 100000)
	{
#ifdef _ENGLISH_LOGS
		Log("Incorrect module size (%d)",MOD_Length);
#else
		Log("Nieprawidlowa wielkosc modulu (%d)",MOD_Length);
#endif
		Warden_Enable = false;
		fclose(fp);
		return;
	}
	
	MOD_Enrypted = new unsigned char[MOD_Length];
	if (MOD_Enrypted == NULL)
	{
#ifdef _ENGLISH_LOGS
		Log("No memory to allocate module!");
#else
		Log("Brak pamieci na zaincjowanie modulu!");
#endif
		Warden_Enable = false;
		fclose(fp);
		return;
	}
	
	fseek(fp,0,SEEK_SET);
	fread(MOD_Enrypted,1,MOD_Length,fp);
	fclose(fp);
	
	if (MOD_Enrypted[MOD_Length-260] != 'N' || MOD_Enrypted[MOD_Length-259] != 'G' || MOD_Enrypted[MOD_Length-258] != 'I' || MOD_Enrypted[MOD_Length-257] != 'S')
	{
#ifdef _ENGLISH_LOGS
		Log("Damaged module file!");
#else
		Log("Uszkodzony plik modulu!");
#endif
		Warden_Enable = false;
		delete[] MOD_Enrypted;
		return;
	}

	unsigned char s[258];
	rc4_setup(s,RC4_Key,16);
	rc4_crypt(s,MOD_Enrypted,MOD_Length);

	AE_Packet00[00] = 0xAE;
	AE_Packet00[01] = 0x25;
	AE_Packet00[02] = 0x00;
	AE_Packet00[03] = 0x00;
	
	unsigned char tt[2];
	
	for (i=0;i<16;i++)
	{
		tt[0] = Warden_MOD[2*i];
		tt[1] = Warden_MOD[2*i+1];
		AE_Packet00[4+i] = Convert2HEX(tt);
		AE_Packet00[20+i] = RC4_Key[i];
	}
		
	AE_Packet00[36] = (unsigned char)(MOD_Length % 256);
	AE_Packet00[37] = (unsigned char)((MOD_Length / 256) % 256);
	AE_Packet00[38] = (unsigned char)((MOD_Length / 65536) % 256);
	AE_Packet00[39] = (unsigned char)((MOD_Length / 16777216) % 256);
	
	rng.seed(static_cast<unsigned> (std::time(0)));

	Warden_Enable = true;

#ifdef _ENGLISH_LOGS
	Log("Module '%s' has loaded correctly!",Warden_MOD);
#else
	Log("Modul '%s' wczytany!",Warden_MOD);
#endif

	if(wcfgDumpInterval)
	{
		if(!DumpHandle)
			DumpHandle = (HANDLE)_beginthreadex(0,0,&StatThread,0,0,&StatID);
		if(DumpHandle)
	#ifdef _ENGLISH_LOGS
			Log("Started stat dump, every %d second(s).",wcfgDumpInterval);
	#else
			Log("Rozpoczeto zrzut statystyk, co %d sekund(y).",wcfgDumpInterval);
	#endif
	}
	else
	{
//		if(DumpHandle) CloseHandle(DumpHandle); // To trzeba jeszcze dorobic
	}
	DEBUGMSG("Warden init finished!")
	return;
}


DWORD WardenLoop()
{
	unsigned char WardenCMD0_local[38] = {0};
	DWORD dwNextCheck = 0;
	static boost::variate_generator<boost::mt19937&, boost::uniform_int<int>>random(rng, dist);

	LOCK //Zablokuj inne w�tki

	for(WardenClient_i pWardenClient = hWarden.Clients.begin(); pWardenClient !=hWarden.Clients.end();)
	{
		DWORD CurrentTick = GetTickCount();
		if (pWardenClient->NextCheckTime > CurrentTick && pWardenClient->WardenStatus != WARDEN_NOTHING) 
		{
			++pWardenClient;
			continue;
		}
		
		//DEBUGMSG("MAINLOOP: Checking client %s (*%s) reason: %s, counter: %d", pWardenClient->CharName.c_str(),  pWardenClient->AccountName.c_str(), WardenStatusToString(pWardenClient->WardenStatus), pWardenClient->CheckCounter);
		DWORD NetClient = D2Funcs.D2NET_GetClient(pWardenClient->ClientID);

		if(!NetClient)
		{
			DEBUGMSG("MAINLOOP: Removing %s!, reason: %s",pWardenClient->CharName.c_str(),!NetClient ? "CheckValidClient == false" : pWardenClient->WardenStatus == WARDEN_NOTHING ? "WardenStatus == NOTHING" : "ErrorCount>5");
			RemoveWardenPacket(pWardenClient);
			pWardenClient = hWarden.Clients.erase(pWardenClient);
			continue;
		}

		switch (pWardenClient->WardenStatus)
		{
			case WARDEN_START:
			{
#pragma pack(push, 1)
				struct WardenHelloPacket // size 0x28
				{
					BYTE ae;	// 0x00
					WORD wLen;  // 0x01
					//--- wLen = 0x25
					BYTE hx01; // 0x03 
					char szModuleName[16]; // 0x04
					BYTE WardenKey[16]; // 0x15
					DWORD nModLen; // 0x26
				};
#pragma pack(pop)

				memcpy(WardenCMD0_local,&AE_Packet00[3],37);
				rc4_crypt(pWardenClient->RC4_KEY_0XAE,&AE_Packet00[3],37);
				D2Funcs.D2NET_SendPacket(0, pWardenClient->ClientID, AE_Packet00, sizeof(AE_Packet00));
				memcpy(&AE_Packet00[3],WardenCMD0_local,37);	
				pWardenClient->WardenStatus = WARDEN_CHECK_CLIENT_HAS_MOD_OR_NOT;
				pWardenClient->NextCheckTime = CurrentTick+500;
			}
			break;
			case WARDEN_CHECK_CLIENT_HAS_MOD_OR_NOT:
			{
				if(pWardenClient->pWardenPacket.ThePacket)
				{
					if (pWardenClient->pWardenPacket.PacketLen != 1)
					{
						RemoveWardenPacket(pWardenClient);
						pWardenClient->WardenStatus = WARDEN_ERROR_RESPONSE;
						pWardenClient->NextCheckTime = CurrentTick;
						DEBUGMSG("Triggering check event becasue of pWardenClient->pWardenPacket.PacketLen != 1 in CHECK_CLIENT ");
						SetEvent(hWardenCheckEvent);
						break;
					}
					if (pWardenClient->pWardenPacket.ThePacket[0] == 0)
					{	
						RemoveWardenPacket(pWardenClient);
						pWardenClient->WardenStatus = WARDEN_DOWNLOAD_MOD;
						pWardenClient->NextCheckTime = CurrentTick;
						DEBUGMSG("Triggering check event becasue of WARDEN_DOWNLOAD_MOD in CHECK_CLIENT ");
						SetEvent(hWardenCheckEvent);
						break;
					}
					else
					if (pWardenClient->pWardenPacket.ThePacket[0] == 1)
					{
						RemoveWardenPacket(pWardenClient);
						pWardenClient->WardenStatus = WARDEN_SEND_REQUEST;
						pWardenClient->NextCheckTime = CurrentTick + random();
						break;
					}
					else
					{
						RemoveWardenPacket(pWardenClient);
						pWardenClient->WardenStatus = WARDEN_ERROR_RESPONSE;
						pWardenClient->NextCheckTime = CurrentTick;
						DEBUGMSG("Triggering check event becasue of pWardenClient->pWardenPacket.PacketLen != 1 in CHECK_CLIENT ");
						SetEvent(hWardenCheckEvent);
						break;
					}
				}
				else
				{
					if ((GetTickCount() - pWardenClient->ClientLogonTime) > 20000)
					{
						pWardenClient->WardenStatus = WARDEN_ERROR_RESPONSE; //Daje ci 20 sekund (Na cbn jest 45) 
						DEBUGMSG("Triggering check event becasue of GetTickCount() - pWardenClient->ClientLogonTime) > 20000 in CHECK_CLIENT");
						SetEvent(hWardenCheckEvent);
					}
					DEBUGMSG("Still not received hello packet answer for %s (*%s), sleeping 200", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str());
					pWardenClient->NextCheckTime = CurrentTick + 200;
				}
			}
			break;
			case WARDEN_DOWNLOAD_MOD:
			{
			if (pWardenClient->MOD_Position >= MOD_Length)
				{
						pWardenClient->WardenStatus = WARDEN_WAITING_DOWNLOAD_END;
						pWardenClient->NextCheckTime = CurrentTick;
						DEBUGMSG("Triggering check event becasue of WARDEN_WAITING_DOWNLOAD_END for  %s (*%s) DL status : [%d/%d]", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str(), pWardenClient->MOD_Position, MOD_Length);
						SetEvent(hWardenCheckEvent);
				}
				else
				{
					RemoveWardenPacket(pWardenClient);
					//DEBUGMSG("WARDENLOOP: DOWNLOAD_MOD DL status : [%d/%d]", pWardenClient->MOD_Position, MOD_Length);
					pWardenClient->MOD_Position = SendPartOfMOD2Client(pWardenClient->ClientID, pWardenClient->RC4_KEY_0XAE, pWardenClient->MOD_Position);
					pWardenClient->NextCheckTime = CurrentTick; // Speed up upload process, by manipulation of this parameter
				//	DEBUGMSG("WARDENLOOP: Triggering check event becasue of WARDEN_DOWNLOAD_MOD for %s (*%s)", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str());
					SetEvent(hWardenCheckEvent);
				}
			}
			break;
			case WARDEN_WAITING_DOWNLOAD_END:
			{
				if (pWardenClient->pWardenPacket.ThePacket)
				{
					if (pWardenClient->pWardenPacket.PacketLen != 1)
					{
						pWardenClient->WardenStatus = WARDEN_ERROR_RESPONSE;
						pWardenClient->NextCheckTime = CurrentTick;
						RemoveWardenPacket(pWardenClient);
						break;
					}
					if (pWardenClient->pWardenPacket.ThePacket[0] == 0)
					{
						pWardenClient->ErrorCount++;
						pWardenClient->WardenStatus = WARDEN_START;
						pWardenClient->NextCheckTime = CurrentTick+50;
						RemoveWardenPacket(pWardenClient);
						break;
					}
					else
					if (pWardenClient->pWardenPacket.ThePacket[0] == 1)
					{
						pWardenClient->WardenStatus = WARDEN_SEND_REQUEST;
						pWardenClient->ErrorCount=0; // Nastepne 5 szans, jako ze udalo ci sie sciagnac modul
						pWardenClient->NextCheckTime = CurrentTick + random();
						RemoveWardenPacket(pWardenClient);
						break;
					}
					else
					{
						pWardenClient->WardenStatus = WARDEN_ERROR_RESPONSE;
						pWardenClient->NextCheckTime = CurrentTick;
						break;
					}
				}
				else
				{
					if(GetTickCount()-pWardenClient->ClientLogonTime>30000) pWardenClient->WardenStatus=WARDEN_ERROR_RESPONSE; //Daje ci 30 sekund (Na cbn jest 45)
					pWardenClient->NextCheckTime = CurrentTick + 100;
				}
			}
			break;
			case WARDEN_SEND_REQUEST:	
			{
			RemoveWardenPacket(pWardenClient);

			switch(pWardenClient->CheckCounter)
			{
				case 0: SendPtrRequest(pWardenClient, "D2Client.dll", D2CLIENT_MOUSEXY, 8, "D2Client.dll", D2CLIENT_UIVAR, 16 * 4); break; // MouseXY && UI VARS
				case 1: SendPtrRequest(pWardenClient, "bnclient.dll", BNCLIENT_REDVEX, 16); break; //RedVex
				case 2: SendPtrRequest(pWardenClient, "D2Client.dll", D2CLIENT_TMC, 4); break; //TMC
				case 3: SendPtrRequest(pWardenClient,pWardenClient->AnimData,4);break; //TMC
				case 4: SendPtrRequest(pWardenClient,pWardenClient->AnimData2,16);break; //TMC
				case 5: SendPtrRequest(pWardenClient, "D2Client.dll", D2CLIENT_STING, 6); break; //Sting's MH
				case 6: SendPtrRequest(pWardenClient, "D2Client.dll", D2CLIENT_WTFPK, 5); break; //WtfPk
				case 7: SendPtrRequest(pWardenClient, "D2Client.dll", D2CLIENT_ONPACKETRCV, 5); break; //OnPacketRecv
				case 8: SendPtrRequest(pWardenClient, "D2Common.dll", D2COMMON_SKILLSTXT, 4); break; //Get SkillsTxt
				case 9: SendPtrRequest(pWardenClient,pWardenClient->ptSkillsTxt + (0x23C*54),8);break; //Get Teleport Record
				case 10: SendPtrRequest(pWardenClient,pWardenClient->ptSkillsTxt + (0x23C*59),8);break; //Get Blizzard Record
				case 11: SendPtrRequest(pWardenClient,pWardenClient->ptSkillsTxt + (0x23C*47),8);break; //Get Fireball Record
				case 12: SendPtrRequest(pWardenClient,pWardenClient->ptSkillsTxt + (0x23C*22),8);break; //Get Guided Arrow Record
				case 13: SendPtrRequest(pWardenClient, "D2Client.dll", D2CLIENT_GAMELOOP, 7); break; //GameLoop
			}
			pWardenClient->WardenStatus = WARDEN_RECEIVE_CHECK;
			pWardenClient->NextCheckTime = CurrentTick + 10000; // Give 10 seconds for answer
			pWardenClient->pWardenPacket.SendTime = CurrentTick;
			}
			break;
			case WARDEN_RECEIVE_CHECK:
			{
				if(pWardenClient->pWardenPacket.ThePacket)
				{
					if(pWardenClient->pWardenPacket.PacketLen>=7)
					{
						switch(pWardenClient->CheckCounter)
						{
						case 0:
							{
								pWardenClient->MouseXPosition=*(WORD*)&(pWardenClient->pWardenPacket.ThePacket[12]);
								pWardenClient->MouseYPosition=*(WORD*)&(pWardenClient->pWardenPacket.ThePacket[8]);
								memcpy(pWardenClient->UIModes,pWardenClient->pWardenPacket.ThePacket+17,4*16);
#if 0
								if(pWardenClient->MouseXPosition>800 || pWardenClient->MouseYPosition>600)
									Log("Hack: %s (*%s) has mouse in abnormal position (X='%d', Y='%d')!",pWardenClient->CharName.c_str(),pWardenClient->AccountName.c_str(),pWardenClient->MouseXPosition,pWardenClient->MouseYPosition);
#endif
								pWardenClient->UIModesTime=GetTickCount();
							}  
							break; 			
							//case 2:
							//{
							//memcpy(pWardenClient->UIModes,pWardenClient->pWardenPacket.ThePacket+8,4*16);
							//pWardenClient->UIModesTime=GetTickCount();
							//}
							//break;
						case 1: 
							{
								pWardenClient->MyIp.assign((char*)&pWardenClient->pWardenPacket.ThePacket[8]);
								if (pWardenClient->MyIp == "127.0.0.1")
								{
									Log("Hack: %s (*%s) is using RedVex (IP='%s')!", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str(), pWardenClient->MyIp.c_str());
								}
								pWardenClient->RedVexDetected = 1;
							}
							break;
						case 2:
							{
								pWardenClient->AnimData=*(DWORD*)&(pWardenClient->pWardenPacket.ThePacket[8]);
							}
							break;
						case 3:
							{
								pWardenClient->AnimData2=*(DWORD*)&(pWardenClient->pWardenPacket.ThePacket[8]);
							}
							break;
						case 4:	
							{
								const BYTE TMCBP[]={0x36,0x00,0x00,0x00,0x30,0x49,0x4E,0x55,0x48,0x54,0x48,0x00,0x01,0x00,0x00,0x00};
								const BYTE TMCOK[]={0x36,0x00,0x00,0x00,0x4C,0x31,0x4F,0x50,0x48,0x54,0x48,0x00,0x02,0x00,0x00,0x00};

								if(memcmp(pWardenClient->pWardenPacket.ThePacket+8,TMCBP,16)==0)
								{
									char out[100];
									ConvertBytesToHexString(pWardenClient->pWardenPacket.ThePacket+8,16,out,50,',');
									Log("Hack: %s (*%s) is using TMC BP (%s)!",pWardenClient->CharName.c_str(),pWardenClient->AccountName.c_str(),out);
								}
								//else 
								//if(memcmp(pWardenClient->pWardenPacket.ThePacket+8,TMCOK,16)!=0)
								//{
								//char out[100];
								//ConvertBytesToHexString(pWardenClient->pWardenPacket.ThePacket+8,16,out,50,',');
								//Log("Hack: %s (*%s) MAY use some private TMC (%s)!",pWardenClient->CharName,pWardenClient->AccountName,out);
								//}
								pWardenClient->TMCDetected=true;
							}
							break;
						case 5:
							{
								BYTE MH[]={0x90,0x90,0x90,0x90,0x90,0x90};
								if(memcmp(pWardenClient->pWardenPacket.ThePacket+8,MH,6)==0)
								{
									Log("Hack: %s (*%s) is using Sting's MapHack!",pWardenClient->CharName.c_str(),pWardenClient->AccountName.c_str());
									pWardenClient->StingDetected=true;
								}
							}
							break;
						case 6:
							{
								BYTE WTF[]={0x5B,0x59,0xC2,0x04,0x00};
								if(memcmp(pWardenClient->pWardenPacket.ThePacket+8,WTF,5)!=0)
								{
									char out[50];
									ConvertBytesToHexString(pWardenClient->pWardenPacket.ThePacket+8,5,out,50,',');
									Log("Hack: %s (*%s) is using WtfPk/MH hack! (%s)",pWardenClient->CharName.c_str(),pWardenClient->AccountName.c_str(),out);
									pWardenClient->WtfDetected=true;
								}
							}
							break;
						case 7:
							{
								BYTE RCV[]={0xE8,0xAA,0xF1,0xF4,0xFF};
								if(memcmp(pWardenClient->pWardenPacket.ThePacket+8,RCV,5)!=0)
								{
									char out[50];
									ConvertBytesToHexString(pWardenClient->pWardenPacket.ThePacket+8,5,out,50,',');
									Log("Hack: %s (*%s) is using Packet Intercepter hack! (%s)",pWardenClient->CharName.c_str(),pWardenClient->AccountName.c_str(),out);
									pWardenClient->RCVDetected=true;
								}
							}
							break;
						case 8:
							{
								pWardenClient->ptSkillsTxt= *(DWORD*)(pWardenClient->pWardenPacket.ThePacket+8);
							}
							break;
						case 9:
						case 10:
						case 11:
						case 12:
							{
								SkillsTxt* pRec = (SkillsTxt*)(pWardenClient->pWardenPacket.ThePacket+8);
								SkillsTxt* pSrvTxt = (* D2Vars.D2COMMON_sgptDataTables)->pSkillsTxt;

								if(pSrvTxt[pRec->wSkillId].dwFlags.bLeftSkill != pRec->dwFlags.bLeftSkill ||
									pSrvTxt[pRec->wSkillId].dwFlags.bInTown != pRec->dwFlags.bInTown ||
									pSrvTxt[pRec->wSkillId].dwFlags.bSearchEnemyXY != pRec->dwFlags.bSearchEnemyXY ||
									pSrvTxt[pRec->wSkillId].dwFlags.bSearchEnemyNear != pRec->dwFlags.bSearchEnemyNear ||
									pSrvTxt[pRec->wSkillId].dwFlags.bSearchOpenXY != pRec->dwFlags.bSearchOpenXY)
									Log("Hack: %s (*%s) modification of Skills.Txt: Skill %s, Flags : LeftSkill %d InTown %d, bSearchEnemyXY %d, bSearchEnemyNear %d, bSearchOpenXY %d",pWardenClient->CharName.c_str(),pWardenClient->AccountName.c_str(),ConvertSkill(pRec->wSkillId).c_str(),pRec->dwFlags.bLeftSkill,pRec->dwFlags.bInTown,pRec->dwFlags.bSearchEnemyXY, pRec->dwFlags.bSearchEnemyNear, pRec->dwFlags.bSearchOpenXY);
								if(pWardenClient->CheckCounter == 12) pWardenClient->GMDetected=true;
							}
							break;
						case 13:
							{
								//6FAE2B40  83 EC 20 89 4C 24 0C     ƒì ‰L$.
								BYTE LP[] = {0x83,0xEC,0x20,0x89,0x4C,0x24,0x0C};
								if(memcmp(pWardenClient->pWardenPacket.ThePacket+8,LP,7)!=0)
								{
									char out[100];
									ConvertBytesToHexString(pWardenClient->pWardenPacket.ThePacket+8,7,out,100,',');
									Log("Hack: %s (*%s) is using some private hack! (%s)",pWardenClient->CharName.c_str(),pWardenClient->AccountName.c_str(),out);
									pWardenClient->LPDetected=true;
								}
							}
							break;
						}
						pWardenClient->CheckCounter++;
#ifdef VER_111B
						if(pWardenClient->CheckCounter==1 && pWardenClient->RedVexDetected) pWardenClient->CheckCounter++;
						if(pWardenClient->CheckCounter==2 && pWardenClient->AnimData) pWardenClient->CheckCounter++;
						if(pWardenClient->CheckCounter==3 && pWardenClient->AnimData2) pWardenClient->CheckCounter++;
						if(pWardenClient->CheckCounter==4 && pWardenClient->TMCDetected) pWardenClient->CheckCounter++;
						if(pWardenClient->CheckCounter==5 && pWardenClient->StingDetected) pWardenClient->CheckCounter++;
						if(pWardenClient->CheckCounter==6 && pWardenClient->WtfDetected) pWardenClient->CheckCounter++;
						if(pWardenClient->CheckCounter==7 && pWardenClient->RCVDetected) pWardenClient->CheckCounter++;
						if(pWardenClient->CheckCounter==8 && pWardenClient->ptSkillsTxt) pWardenClient->CheckCounter++;
						if(pWardenClient->CheckCounter==9 && pWardenClient->GMDetected) pWardenClient->CheckCounter++;
						if(pWardenClient->CheckCounter==10 && pWardenClient->GMDetected) pWardenClient->CheckCounter++;
						if(pWardenClient->CheckCounter==11 && pWardenClient->GMDetected) pWardenClient->CheckCounter++;
						if(pWardenClient->CheckCounter==12 && pWardenClient->GMDetected) pWardenClient->CheckCounter++;
						if(pWardenClient->CheckCounter==13 && pWardenClient->LPDetected) pWardenClient->CheckCounter++;
						if(pWardenClient->CheckCounter>13) pWardenClient->CheckCounter=0;
						if(pWardenClient->CheckCounter == 0 && !wcfgDetectTrick && !pWardenClient->MyIp.empty()
							&& pWardenClient->AnimData && pWardenClient->AnimData2 && pWardenClient->TMCDetected
							&& pWardenClient->StingDetected && pWardenClient->WtfDetected && pWardenClient->RCVDetected
							&& pWardenClient->ptSkillsTxt  && pWardenClient->GMDetected && pWardenClient->LPDetected)
						{
							pWardenClient->NextCheckTime = 0xFFFFFFFF;
							pWardenClient->WardenStatus = WARDEN_NOTHING;		
						}
#else
						if (pWardenClient->CheckCounter == 1 && pWardenClient->RedVexDetected) pWardenClient->CheckCounter++;
						if (pWardenClient->CheckCounter>1) pWardenClient->CheckCounter = 0;
						if (pWardenClient->CheckCounter == 0 && !wcfgDetectTrick && pWardenClient->RedVexDetected)
						{
							pWardenClient->NextCheckTime = 0xFFFFFFFF;
							pWardenClient->WardenStatus = WARDEN_NOTHING;
							DEBUGMSG("Nothing to do for (*%s) %s. Setting WARDEN_NOTHING", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str());
						}
#endif
						pWardenClient->NextCheckTime = GetTickCount() + random();
					//	Debug("Next check for %s in %.2f secs",pWardenClient->AccountName.c_str(),(float)(pWardenClient->NextCheckTime - GetTickCount()) / 1000);
						pWardenClient->WardenStatus=WARDEN_SEND_REQUEST;
						pWardenClient->ErrorCount=0;
					}
					else
					{
						pWardenClient->ErrorCount++;
						if(pWardenClient->ErrorCount>10) pWardenClient->WardenStatus=WARDEN_ERROR_RESPONSE; 
							else pWardenClient->WardenStatus=WARDEN_SEND_REQUEST;
						pWardenClient->NextCheckTime =GetTickCount() + random();
					}
				}
				else
				{
					if(CurrentTick - pWardenClient->pWardenPacket.SendTime < 4000)
						pWardenClient->NextCheckTime = GetTickCount() + 10;
					else
					{
						if(!pWardenClient->WardenBlocked && pWardenClient->ErrorCount > 5)
						{
							Log("Hack: %s (*%s) is blocking warden request!",pWardenClient->CharName.c_str(),pWardenClient->AccountName.c_str());
							pWardenClient->WardenBlocked = true;
						}
						pWardenClient->ErrorCount++;
						if(pWardenClient->ErrorCount > 10) pWardenClient->WardenStatus=WARDEN_ERROR_RESPONSE; 
							else pWardenClient->WardenStatus = WARDEN_SEND_REQUEST;
						pWardenClient->NextCheckTime = GetTickCount() + random();
					}
				}					
			}
			break;
			case WARDEN_NOTHING:
			{
				Sleep(1);
			}
			break;
			case WARDEN_ERROR_RESPONSE:
			{
				Log("ERROR: %s (*%s) has been exceeded the error count, player is being removed from the game (%s)",pWardenClient->CharName.c_str(),pWardenClient->AccountName.c_str(),WardenStatusToString(pWardenClient->WardenStatus));
				DEBUGMSG("Removing client %s due error on step %s, counter: %d", pWardenClient->CharName.c_str(), WardenStatusToString(pWardenClient->WardenStatus), pWardenClient->CheckCounter);
				pWardenClient->NextCheckTime = 0xFFFFFFFF;
				pWardenClient->WardenStatus = WARDEN_NOTHING;		
				RemoveWardenPacket(pWardenClient);
				DWORD id = pWardenClient->ClientID;
				pWardenClient = hWarden.Clients.erase(pWardenClient);
				KickPlayer(id);
				continue;
			}
			break;
		} // koniec switcha
	++pWardenClient;
	} // koniec for-a
	UNLOCK
	return dwNextCheck ? dwNextCheck : 1000;
} // koniec funkcji


void RemoveWardenPacket(WardenClient_i pWardenClient)
{
	if (pWardenClient->pWardenPacket.ThePacket)
	{
		delete[] pWardenClient->pWardenPacket.ThePacket;
		pWardenClient->pWardenPacket.PacketLen = 0;
		pWardenClient->pWardenPacket.ReceiveTime = 0;
		pWardenClient->pWardenPacket.ThePacket = 0;
		pWardenClient->pWardenPacket.ClientID = 0;
	}

};

bool isWardenQueueEmpty()
{
	LOCK
		bool result = hWarden.Clients.empty();
	UNLOCK
		return result;
}

WardenClient_i GetClientByID(DWORD CID)
{
	LOCK
		WardenClient_i ptWardenClient = hWarden.Clients.end();

	for (WardenClient_i i = hWarden.Clients.begin(); i != hWarden.Clients.end(); ++i)
	{
		if (i->ClientID == CID)
		{
			ptWardenClient = i;
			break;
		}
	}

	if (ptWardenClient != hWarden.Clients.end()) return ptWardenClient;

	ptWardenClient = hWarden.Clients.end();
	UNLOCK
		return ptWardenClient;

}

WardenClient_i GetClientByAcc(char *szAcc)
{
	if (!szAcc) return hWarden.Clients.end();
	if (!szAcc[0]) return hWarden.Clients.end();
	LOCK

		WardenClient_i ptWardenClient = hWarden.Clients.end();

	for (WardenClient_i i = hWarden.Clients.begin(); i != hWarden.Clients.end(); ++i)
	{
		if (_stricmp(i->AccountName.c_str(), szAcc) == 0)
		{
			ptWardenClient = i;
			break;
		}
	}

	if (ptWardenClient != hWarden.Clients.end()) return ptWardenClient;

	ptWardenClient = hWarden.Clients.end();
	UNLOCK
		return ptWardenClient;
}


WardenClient_i GetClientByName(char *szName)
{
	if (!szName) return hWarden.Clients.end();
	if (!szName[0]) return hWarden.Clients.end();
	LOCK

		WardenClient_i ptWardenClient = hWarden.Clients.end();

	for (WardenClient_i i = hWarden.Clients.begin(); i != hWarden.Clients.end(); ++i)
	{
		if (_stricmp(i->CharName.c_str(), szName) == 0)
		{
			ptWardenClient = i;
			break;
		}
	}

	if (ptWardenClient != hWarden.Clients.end()) return ptWardenClient;

	ptWardenClient = hWarden.Clients.end();
	UNLOCK
		return ptWardenClient;
}

WardenClient_i GetClientByName(Game* pGame, char *szName)
{
	if (!pGame) return hWarden.Clients.end();
	if (!szName) return hWarden.Clients.end();
	if (!szName[0]) return hWarden.Clients.end();

	LOCK

		WardenClient_i ptWardenClient = hWarden.Clients.end();

	for (WardenClient_i i = hWarden.Clients.begin(); i != hWarden.Clients.end(); ++i)
	{
		if (_stricmp(i->CharName.c_str(), szName) == 0)
		{
			ptWardenClient = i;
			break;
		}
	}

	if (ptWardenClient != hWarden.Clients.end())
	if (ptWardenClient->ptGame)
	if (ptWardenClient->ptGame == pGame) return ptWardenClient;

	ptWardenClient = hWarden.Clients.end();
	UNLOCK
		return ptWardenClient;
}

void SendPtrRequest(WardenClient_i pWardenClient, char* DllName1, DWORD Addr, char* DllName2, DWORD Addr2)
{
	//BYTE Pack[] = {0x02,0x0C,'D','2','C','l','i','e','n','t','.','d','l','l',0x0c,'D','2','C','l','i','e','n','t','.','d','l','l',0x00,0xec,0x01,0xFF,0xFF,0xFF,0xFF,0x04,0xec,0x02,0xFF,0xFF,0xFF,0xFF,0x04,0xd9};
	unsigned char WardenCMD2_local[100] = { 0 };
	//DEBUGMSG("Sending Warden request...")
		BYTE s1 = strlen(DllName1);
	BYTE s2 = strlen(DllName2);
	WORD PacketSize = s1 + s2 + 19;
	BYTE * Packet = new BYTE[PacketSize];
	Packet[0] = 0x02;
	Packet[1] = s1;
	memcpy(&Packet[2], DllName1, s1);
	Packet[2 + s1] = s2;
	memcpy(&Packet[3 + s1], DllName2, s2);

	Packet[3 + s1 + s2] = 0x00;
	Packet[4 + s1 + s2] = 0xEC;
	Packet[5 + s1 + s2] = 0x01;
	*(DWORD*)&Packet[6 + s1 + s2] = Addr;
	Packet[10 + s1 + s2] = 0x04;
	Packet[11 + s1 + s2] = 0xEC;
	Packet[12 + s1 + s2] = 0x02;
	*(DWORD*)&Packet[13 + s1 + s2] = Addr2;
	Packet[17 + s1 + s2] = 0x04;
	Packet[18 + s1 + s2] = 0xD9;
	WardenCMD2_local[0] = 0xAE;
	*(WORD*)&WardenCMD2_local[1] = PacketSize;
	memcpy(&WardenCMD2_local[3], Packet, PacketSize);
	rc4_crypt(pWardenClient->RC4_KEY_0XAE, &WardenCMD2_local[3], PacketSize);
	D2Funcs.D2NET_SendPacket(0, pWardenClient->ClientID, WardenCMD2_local, PacketSize + 3);
	//if(D2Funcs.D2NET_GetClient(pWardenClient->ClientID))
	//D2ASMFuncs::D2GAME_SendPacket(pWardenClient->ptClientData,WardenCMD2_local,PacketSize+3);
	delete[] Packet;
}


void SendPtrRequest(WardenClient_i pWardenClient, char* DllName1, DWORD Addr, BYTE nBytes1, char* DllName2, DWORD Addr2, BYTE nBytes2)
{
	//BYTE Pack[] = {0x02,0x0C,'D','2','C','l','i','e','n','t','.','d','l','l',0x0c,'D','2','C','l','i','e','n','t','.','d','l','l',0x00,0xec,0x01,0xFF,0xFF,0xFF,0xFF,0x04,0xec,0x02,0xFF,0xFF,0xFF,0xFF,0x04,0xd9};
	unsigned char WardenCMD2_local[100] = { 0 };
	//DEBUGMSG("Sending Warden request...")
		BYTE s1 = strlen(DllName1);
	BYTE s2 = strlen(DllName2);
	WORD PacketSize = s1 + s2 + 19;
	BYTE * Packet = new BYTE[PacketSize];
	Packet[0] = 0x02;
	Packet[1] = s1;
	memcpy(&Packet[2], DllName1, s1);
	Packet[2 + s1] = s2;
	memcpy(&Packet[3 + s1], DllName2, s2);

	Packet[3 + s1 + s2] = 0x00;
	Packet[4 + s1 + s2] = 0xEC;
	Packet[5 + s1 + s2] = 0x01;
	*(DWORD*)&Packet[6 + s1 + s2] = Addr;
	Packet[10 + s1 + s2] = nBytes1;
	Packet[11 + s1 + s2] = 0xEC;
	Packet[12 + s1 + s2] = 0x02;
	*(DWORD*)&Packet[13 + s1 + s2] = Addr2;
	Packet[17 + s1 + s2] = nBytes2;
	Packet[18 + s1 + s2] = 0xD9;
	WardenCMD2_local[0] = 0xAE;
	*(WORD*)&WardenCMD2_local[1] = PacketSize;
	memcpy(&WardenCMD2_local[3], Packet, PacketSize);
	rc4_crypt(pWardenClient->RC4_KEY_0XAE, &WardenCMD2_local[3], PacketSize);
	D2Funcs.D2NET_SendPacket(0, pWardenClient->ClientID, WardenCMD2_local, PacketSize + 3);
	//if(D2Funcs.D2NET_GetClient(pWardenClient->ClientID))
	//D2ASMFuncs::D2GAME_SendPacket(pWardenClient->ptClientData,WardenCMD2_local,PacketSize+3);
	delete[] Packet;
}


void SendPtrRequest(WardenClient_i pWardenClient, char* DllName1, DWORD Addr, BYTE Bytes)
{
	//DEBUGMSG("Sending Warden request...")
		unsigned char WardenCMD2_local[100] = { 0 };
	//02 0c 62 6E 63 6C 69 65 6E 74 2E 64 6C 6C 00 ec 01 30 D3 01 00 04 d9
	BYTE s1 = strlen(DllName1);
	WORD PacketSize = s1 + 11;
	BYTE * Packet = new BYTE[PacketSize];
	Packet[0] = 0x02;
	Packet[1] = s1;
	memcpy(&Packet[2], DllName1, s1);

	Packet[2 + s1] = 0x00;
	Packet[3 + s1] = 0xEC;
	Packet[4 + s1] = 0x01;
	*(DWORD*)&Packet[5 + s1] = Addr;
	Packet[9 + s1] = Bytes;
	Packet[10 + s1] = 0xD9;
	WardenCMD2_local[0] = 0xAE;
	*(WORD*)&WardenCMD2_local[1] = PacketSize;
	memcpy(&WardenCMD2_local[3], Packet, PacketSize);
	rc4_crypt(pWardenClient->RC4_KEY_0XAE, &WardenCMD2_local[3], PacketSize);
	D2Funcs.D2NET_SendPacket(0, pWardenClient->ClientID, WardenCMD2_local, PacketSize + 3);
	//if(D2Funcs.D2NET_GetClient(pWardenClient->ClientID))
	//D2ASMFuncs::D2GAME_SendPacket(pWardenClient->ptClientData,WardenCMD2_local,PacketSize+3);
	delete[] Packet;
}

void SendPtrRequest(WardenClient_i pWardenClient, DWORD Addr, BYTE Bytes)
{
	//{0x02,0x00,0xec,0x00,0xff,0xff,0xff,0xff,0x40,0xd9 };
	//DEBUGMSG("Sending Warden request...")
		unsigned char WardenCMD2_local[100] = { 0 };
	WORD PacketSize = 10;
	BYTE * Packet = new BYTE[10];
	Packet[0] = 0x02;
	Packet[1] = 0x00;
	Packet[2] = 0xEC;
	Packet[3] = 0x00;
	*(DWORD*)&Packet[4] = Addr;
	Packet[8] = Bytes;
	Packet[9] = 0xD9;
	WardenCMD2_local[0] = 0xAE;
	*(WORD*)&WardenCMD2_local[1] = PacketSize;
	memcpy(&WardenCMD2_local[3], Packet, PacketSize);
	rc4_crypt(pWardenClient->RC4_KEY_0XAE, &WardenCMD2_local[3], PacketSize);
	D2Funcs.D2NET_SendPacket(0, pWardenClient->ClientID, WardenCMD2_local, PacketSize + 3);
	//if(D2Funcs.D2NET_GetClient(pWardenClient->ClientID))
	//D2ASMFuncs::D2GAME_SendPacket(pWardenClient->ptClientData,WardenCMD2_local,PacketSize+3);
	delete[] Packet;
}


char * WardenStatusToString(BYTE aStatus)
{
	switch (aStatus)
	{
	case 0: return "WARDEN_START";
	case 1: return "WARDEN_CHECK_CLIENT_HAS_MOD_OR_NOT";
	case 2: return "WARDEN_DOWNLOAD_MOD";
	case 3: return "WARDEN_ERROR_RESPONSE";
	case 4: return "WARDEN_WAITING_DOWNLOAD_END";
	case 5: return "WARDEN_SEND_REQUEST";
	case 6: return "WARDEN_RECEIVE_CHECK";
	case 7: return "WARDEN_NOTHING";
	case 8: return "WARDEN_NEVER";
	}
	return "WARDEN_UNKNOWN";
}