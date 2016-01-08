/* ==========================================================
* d2warden
* https://bitbucket.org/lolet/d2warden
* ==========================================================
*			Copyright 2008 marsgod
*			 2011-2015 Bartosz Jankowski
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
#include "Warden.h"
#include "WardenClient.h"
#include "WardenChecks.h"
#include "RC4.h"
#include "Build.h"

#include "LWorldEvent.h"

Warden::Warden() : dist(300, 10000), random(rng, dist)
{
	const char Warden_MOD[256] = "3ea42f5ac80f0d2deb35d99b4e9a780b.mod";
	const BYTE RC4_Key[17] = "WardenBy_Marsgod"; //3ea42f5ac80f0d2deb35d99b4e9a780b98ff
	char Maiev[10] = "MAIEV.MOD";
	init_succed = false;
	FILE * fp = 0;
	int i = 0;

	rng.seed(static_cast<unsigned> (std::time(0)));

	Log("Warden ver. 2.0 build %d (%s) by Lolet has started. Compiled on %s, %s", __BUILDNO__,
#ifdef VER_111B
		"1.11b",
#elif defined VER_113D
		"1.13d",
#endif
		__DATE__, __TIME__);

	loadConfig();
#ifdef D2EX_MYSQL
	Log("Connecting to MYSQL server...");
	if (!FG_WaitForMySQLServer(1000 * 6)) { // 6 sec for connection
		Log("Failed to connect to MySQL server");
		return;
	}
#endif
	Log("Setting allowed player limit on %d", wcfgMaxPlayers);
	Log("Patching game...");

	patchD2();

	if (strlen(Warden_MOD) != 36)
	{
		Log("Incorrect name of module '%s' (len!=36)", Warden_MOD);
		return;
	}

	fopen_s(&fp, Warden_MOD, "rb");
	if (!fp)
	{
		Log("Cannot find module filename '%s'!", Warden_MOD);
		return;
	}
	fseek(fp, 0, SEEK_END);
	moduleLength = ftell(fp);

	if (moduleLength <= 260 || moduleLength > 100000)
	{
		Log("Incorrect module size (%d)", moduleLength);
		fclose(fp);
		return;
	}

	moduleEncrypted = new unsigned char[moduleLength];
	if (!moduleEncrypted)
	{
		Log("No memory to allocate module!");
		fclose(fp);
		return;
	}

	fseek(fp, 0, SEEK_SET);
	fread(moduleEncrypted, 1, moduleLength, fp);
	fclose(fp);

	if (moduleEncrypted[moduleLength - 260] != 'N' || moduleEncrypted[moduleLength - 259] != 'G' ||
		moduleEncrypted[moduleLength - 258] != 'I' || moduleEncrypted[moduleLength - 257] != 'S')
	{
		Log("Damaged module file!");
		delete[] moduleEncrypted;
		return;
	}

	unsigned char s[258];
	rc4_setup(s, (BYTE*)RC4_Key, 16);
	rc4_crypt(s, moduleEncrypted, moduleLength);

	AE_Packet00[00] = 0xAE;
	AE_Packet00[01] = 0x25;
	AE_Packet00[02] = 0x00;
	AE_Packet00[03] = 0x00;

	unsigned char tt[2];

	for (i = 0; i<16; i++)
	{
		tt[0] = Warden_MOD[2 * i];
		tt[1] = Warden_MOD[2 * i + 1];
		AE_Packet00[4 + i] = Convert2HEX(tt);
		AE_Packet00[20 + i] = RC4_Key[i];
	}

	AE_Packet00[36] = (unsigned char)(moduleLength % 256);
	AE_Packet00[37] = (unsigned char)((moduleLength / 256) % 256);
	AE_Packet00[38] = (unsigned char)((moduleLength / 65536) % 256);
	AE_Packet00[39] = (unsigned char)((moduleLength / 16777216) % 256);


	Log("Module '%s' has loaded correctly!", Warden_MOD);

	DEBUGMSG("Warden init finished!")
	init_succed = true;
}

Warden::~Warden()
{
	Log("Shutting down...");
	delete[] moduleEncrypted;
	clients.clear();
	Log("Done!");
}


void Warden::onRemoveClient(WardenClient_i client)
{
	clients.erase(client);
}

DWORD Warden::uploadModule(DWORD ClientID, unsigned char *RC4_KEY, DWORD modOffset)
{
	unsigned char MyMod[0x200];
	int ModLen;

	if (moduleLength - 0x1f4 > modOffset) ModLen = 0x1f4;
	else
		ModLen = moduleLength - modOffset;
	MyMod[0] = 0xAE;
	MyMod[1] = (unsigned char)((ModLen + 3) % 256);
	MyMod[2] = (unsigned char)((ModLen + 3) >> 8);
	MyMod[3] = 1;
	MyMod[4] = (unsigned char)(ModLen % 256);
	MyMod[5] = (unsigned char)(ModLen >> 8);
	memcpy(&MyMod[6], moduleEncrypted + modOffset, ModLen);
	rc4_crypt(RC4_KEY, &MyMod[3], ModLen + 3);
	D2Funcs.D2NET_SendPacket(0, ClientID, MyMod, ModLen + 6);
	return ModLen + modOffset;
};



WardenClient_i Warden::findClientById(DWORD clientId)
{
	WardenClient_i ptWardenClient = clients.end();

	for (WardenClient_i i = clients.begin(); i != clients.end(); ++i)
	{
		if (i->ClientID == clientId)
		{
			ptWardenClient = i;
			break;
		}
	}
	
	return ptWardenClient;
}

WardenClient_i Warden::findClientByAcc(string szAcc)
{
	if (szAcc.empty())
		return clients.end();

	WardenClient_i ptWardenClient = clients.end();

	for (WardenClient_i i = clients.begin(); i != clients.end(); ++i)
	{
		if (i->AccountName.empty())
			continue;

		if (_stricmp(i->AccountName.c_str(), szAcc.c_str()) == 0)
		{
			ptWardenClient = i;
			break;
		}
	}

	return ptWardenClient;
}


WardenClient_i Warden::findClientByName(string szName)
{
	if (szName.empty())
		return clients.end();

	WardenClient_i ptWardenClient = clients.end();

	for (WardenClient_i i = clients.begin(); i != clients.end(); ++i)
	{
		if (i->CharName.empty())
			continue;

		if (_stricmp(i->CharName.c_str(), szName.c_str()) == 0)
		{
			ptWardenClient = i;
			break;
		}
	}

	return ptWardenClient;
}

WardenClient_i Warden::findClientByName(Game* pGame, string szName)
{
	if (!pGame) 
		return clients.end();
	if (szName.empty())
		return clients.end();

	WardenClient_i ptWardenClient = clients.end();

	for (WardenClient_i i = clients.begin(); i != clients.end(); ++i)
	{
		if (i->CharName.empty())
			continue;

		if (_stricmp(i->CharName.c_str(), szName.c_str()) == 0)
		{
			ptWardenClient = i;
			break;
		}
	}

	if (ptWardenClient != clients.end() && ptWardenClient->ptGame)
			if (ptWardenClient->ptGame == pGame) 
				return ptWardenClient;

	return clients.end();
}

/*
	Parse a warden packet received from the client.
*/
DWORD __fastcall Warden::onWardenPacketReceive(Game *pGame, UnitAny *pPlayer, BYTE *pPacket, DWORD nSize)
{
	if (!pPlayer)
	{
		DEBUGMSG("WardenPacket: ptPlayer == null!");
		return MSG_HACK;
	}

	DWORD ClientID = pPlayer->pPlayerData->pClientData->ClientID;

	if (nSize < 3)
	{
		DEBUGMSG("WardenPacket: PacketLen < 3 !");
		return MSG_HACK;
	}

	if (!ClientID)
	{
		DEBUGMSG("WardenPacket: No client id!");
		return MSG_HACK;
	}


	WardenClient_i client = findClientById(ClientID);
	if (client != clients.end())
	{
		if (!client->ready) {
			DEBUGMSG("The client isnt ready yet!")
			return MSG_OK;
		}
		client->pWardenPacket.ReceiveTime = GetTickCount();
		client->pWardenPacket.PacketLen = pPacket[2] * 256 + pPacket[1];

		if (client->pWardenPacket.PacketLen == 0 || client->pWardenPacket.PacketLen > 512) // Taka jest maksymalna wielkosc pakietu obslugiowanego przez d2
		{
			DEBUGMSG("WardenPacket: Packet size exceeds 512 bytes!");
			return MSG_HACK;
		}

		BYTE *ThePacket = new BYTE[client->pWardenPacket.PacketLen];
		if (!ThePacket)
		{
			Log("WardenPacket: No memory to allocate packet data!");
			return MSG_HACK;
		}

		memcpy(ThePacket, pPacket + 3, client->pWardenPacket.PacketLen);
		client->pWardenPacket.ThePacket = ThePacket;

		rc4_crypt(client->RC4_KEY_0X66, client->pWardenPacket.ThePacket, client->pWardenPacket.PacketLen);
		client->NextCheckTime = GetTickCount();

		return MSG_OK; // Wszystko OK!
	}
	else
	{
		DEBUGMSG("WardenPacket: Client %d, %s (*%s) is not in WardenQueue!!", ClientID, pPlayer->pPlayerData->pClientData->CharName, pPlayer->pPlayerData->pClientData->AccountName);
		Log("WardenPacket: Unexpected packet from player %s (*%s)! Returning an error..", pPlayer->pPlayerData->szName, pPlayer->pPlayerData->pClientData->AccountName);
		return MSG_HACK;
	}

}

/*
	Join/Create a CBN game
*/
DWORD __fastcall Warden::onJoinGame(PacketData *pPacket)
{
	px68* packet = (px68*)pPacket->aPacket;
	DEBUGMSG("Adding a new client...")
	WardenClient client(pPacket->ClientID, packet->ServerHash);
	client.VerCode = (BYTE)packet->VerByte;

	DEBUGMSG("Client version is %d, server requires %d", packet->VerByte, wcfgD2EXVersion)
	if (packet->VerByte > 13 && packet->VerByte < wcfgD2EXVersion && !wcfgAllowVanilla)
	{
		client.bNeedUpdate = true;
	}
	else if (!wcfgAllowVanilla && packet->VerByte == 13)
	{
		Log("Dropping connection with '%s', reason : No D2Ex2 installed.", packet->szCharName);
		BootPlayer(pPacket->ClientID, BOOT_VERSION_MISMATCH);
		return MSG_ERROR;
	}
	else if (!wcfgAllowVanilla)
	{
		Log("Dropping connection with '%s', reason : Unsupported patch version (1.%d).", packet->szCharName, client.VerCode);
	}

	DEBUGMSG("Added %s to WardenQueue", packet->szCharName);
	clients.push_back(client);
	return MSG_OK;
}

void Warden::loadConfig()
{
	char *tk = 0, *rt = 0;
	char temp[100] = { 0 };
	char filename[MAX_PATH] = { 0 };
	char URL[255] = { 0 };
	char szItem[255] = { 0 };
	GetCurrentDirectory(MAX_PATH, filename);
	wcfgConfigFile.assign(filename);
	wcfgConfigFile += "\\D2Warden.ini";

	srand((DWORD)time(0));

	GetPrivateProfileString("Warden", "AllowTeleport", "1,5", temp, 100, wcfgConfigFile.c_str());
	for (rt = strtok_s(temp, ", ", &tk); rt; rt = strtok_s(NULL, ", ", &tk))
		if (atoi(rt) < 7) wcfgTeleChars[atoi(rt)] = TRUE;

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
	wcfgAllowQuests = GetPrivateProfileInt("Warden", "AllowQuests", 1, wcfgConfigFile.c_str());
	wcfgEnableLevelCmd = GetPrivateProfileInt("Warden", "EnableLevelCmd", 1, wcfgConfigFile.c_str());

	GetPrivateProfileString("Warden", "ClanDataURL", "http://www.lolet.yoyo.pl/Clans.ini", URL, 255, wcfgConfigFile.c_str());
	wcfgClansURL = URL;
	GetPrivateProfileString("Warden", "UpdateURL", "", URL, 255, wcfgConfigFile.c_str());
	wcfgUpdateURL = URL;

	GetPrivateProfileString("Warden", "GSName", "N/A", URL, 255, wcfgConfigFile.c_str());
	wcfgGSName = URL;
	GetPrivateProfileString("Warden", "Admins", "LOLET", URL, 255, wcfgConfigFile.c_str());
	tk = 0;
	for (rt = strtok_s(URL, ", ", &tk); rt; rt = strtok_s(NULL, ", ", &tk))
	{
		boost::to_lower(rt);
		if (rt[0]) wcfgAdmins.push_back(rt);
	}

#ifdef D2EX_MYSQL
	GetPrivateProfileString("Warden", "FGDatabaseName", "projectdvb", URL, 255, wcfgConfigFile.c_str());
	wcfgDatabase = URL;
	GetPrivateProfileString("Warden", "FGDatabaseUser", "root", URL, 255, wcfgConfigFile.c_str());
	wcfgDBUser = URL;
	GetPrivateProfileString("Warden", "FGDatabasePass", "", URL, 255, wcfgConfigFile.c_str());
	wcfgDBPass = URL;
#endif
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
		GetPrivateProfileString("World Event", "Trigger", "rin, unique, 122", szItem, 255, wcfgConfigFile.c_str());

		string sItem(szItem);
		boost::to_lower(sItem);

		typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
		boost::char_separator<char> sep(", ");
		boost::char_separator<char> subsep(": ");
		tokenizer tokens(sItem, sep);
		memset(&WItem.ItemCode, 0, 20 * 4);
		memset(&WItem.FileIdx[0], -1, 20 * 4);
		memset(&WItem.ItemQuality[0], -1, 7);

		int ntk = 1;
		for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); tok_iter++, ntk++)
		{
			switch (ntk)
			{
			case 1:
			{
				tokenizer subtok(*tok_iter, subsep);
				int stk = 0;
				for (tokenizer::iterator iter = subtok.begin(); iter != subtok.end(); iter++, stk++)
				{
					WItem.ItemCode[stk] = TransCode((*iter).c_str());
				}
			}
			break;
			case 2:
			{
				tokenizer subtok(*tok_iter, subsep);
				int stk = 0;
				for (tokenizer::iterator iter = subtok.begin(); iter != subtok.end(); iter++, stk++)
					WItem.ItemQuality[stk] = TransQuality((*iter).c_str());
			}
			break;
			case 3:
			{
				tokenizer subtok(*tok_iter, subsep);
				int stk = 0;
				for (tokenizer::iterator iter = subtok.begin(); iter != subtok.end(); iter++, stk++)
					WItem.FileIdx[stk] = atoi((*iter).c_str());
			}
			break;
			}
		}

	}
	Log("Settings loaded.");
}

char* Warden::getStatusMessage(WardenStatus status)
{
#define ENUMSTR( name ) case name: return # name ;
	switch (status)
	{
	ENUMSTR(WARDEN_START)
	ENUMSTR(WARDEN_CHECK_CLIENT_HAS_MOD_OR_NOT)
	ENUMSTR(WARDEN_DOWNLOAD_MOD)
	ENUMSTR(WARDEN_ERROR_RESPONSE)
	ENUMSTR(WARDEN_WAITING_DOWNLOAD_END)
	ENUMSTR(WARDEN_SEND_REQUEST)
	ENUMSTR(WARDEN_RECEIVE_CHECK)
	ENUMSTR(WARDEN_NOTHING)
	}
	return "WARDEN_UNKNOWN";
#undef ENUMSTR
}

void Warden::loop()
{
	unsigned char WardenCMD0_local[38] = { 0 };
	DWORD dwNextCheck = 0;

	for (WardenClient_i pWardenClient = clients.begin(); pWardenClient != clients.end();)
	{
		DWORD CurrentTick = GetTickCount();
		if ((pWardenClient->NextCheckTime > CurrentTick && pWardenClient->WardenStatus != WARDEN_NOTHING) || !pWardenClient->ready)
		{
			++pWardenClient;
			continue;
		}
		DWORD NetClient = D2Funcs.D2NET_GetClient(pWardenClient->ClientID);

		if (!NetClient)
		{
			DEBUGMSG("MAINLOOP: Removing %s!, reason: %s", pWardenClient->CharName.c_str(), !NetClient ? "CheckValidClient == false" :
				pWardenClient->WardenStatus == WARDEN_NOTHING ? "WardenStatus == NOTHING" : "ErrorCount>5");
			pWardenClient->removePacket();
			pWardenClient = clients.erase(pWardenClient);
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

			memcpy(WardenCMD0_local, &AE_Packet00[3], 37);
			rc4_crypt(pWardenClient->RC4_KEY_0XAE, &AE_Packet00[3], 37);
			D2Funcs.D2NET_SendPacket(0, pWardenClient->ClientID, AE_Packet00, sizeof(AE_Packet00));
			memcpy(&AE_Packet00[3], WardenCMD0_local, 37);
			pWardenClient->WardenStatus = WARDEN_CHECK_CLIENT_HAS_MOD_OR_NOT;
			pWardenClient->NextCheckTime = CurrentTick + 500;
		}
		break;
		case WARDEN_CHECK_CLIENT_HAS_MOD_OR_NOT:
		{
			if (pWardenClient->pWardenPacket.ThePacket)
			{
				if (pWardenClient->pWardenPacket.PacketLen != 1)
				{
					pWardenClient->removePacket();
					pWardenClient->WardenStatus = WARDEN_ERROR_RESPONSE;
					pWardenClient->NextCheckTime = CurrentTick;
					DEBUGMSG("Triggering check event because of pWardenClient->pWardenPacket.PacketLen != 1 in CHECK_CLIENT ");
					break;
				}
				if (pWardenClient->pWardenPacket.ThePacket[0] == 0)
				{
					pWardenClient->removePacket();
					pWardenClient->WardenStatus = WARDEN_DOWNLOAD_MOD;
					pWardenClient->NextCheckTime = CurrentTick;
					DEBUGMSG("Triggering check event becasue of WARDEN_DOWNLOAD_MOD in CHECK_CLIENT ");
					break;
				}
				else
					if (pWardenClient->pWardenPacket.ThePacket[0] == 1)
					{
						pWardenClient->removePacket();
						pWardenClient->WardenStatus = WARDEN_SEND_REQUEST;
						pWardenClient->NextCheckTime = CurrentTick + random();
						break;
					}
					else
					{
						pWardenClient->removePacket();
						pWardenClient->WardenStatus = WARDEN_ERROR_RESPONSE;
						pWardenClient->NextCheckTime = CurrentTick;
						DEBUGMSG("Triggering check event becasue of pWardenClient->pWardenPacket.PacketLen != 1 in CHECK_CLIENT ");
						break;
					}
			}
			else
			{
				if ((GetTickCount() - pWardenClient->ClientLogonTime) > 20000)
				{
					pWardenClient->WardenStatus = WARDEN_ERROR_RESPONSE; //Daje ci 20 sekund (Na cbn jest 45) 
					DEBUGMSG("Triggering check event becasue of GetTickCount() - pWardenClient->ClientLogonTime) > 20000 in CHECK_CLIENT");
				}
				DEBUGMSG("Still not received hello packet answer for %s (*%s), sleeping 200", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str());
				pWardenClient->NextCheckTime = CurrentTick + 200;
			}
		}
		break;
		case WARDEN_DOWNLOAD_MOD:
		{
			if (pWardenClient->MOD_Position >= moduleLength)
			{
				pWardenClient->WardenStatus = WARDEN_WAITING_DOWNLOAD_END;
				pWardenClient->NextCheckTime = CurrentTick;
				DEBUGMSG("Triggering check event becasue of WARDEN_WAITING_DOWNLOAD_END for  %s (*%s) DL status : [%d/%d]",
					pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str(), pWardenClient->MOD_Position, moduleLength);
			}
			else
			{
				pWardenClient->removePacket();
				//DEBUGMSG("WARDENLOOP: DOWNLOAD_MOD DL status : [%d/%d]", pWardenClient->MOD_Position, MOD_Length);
				pWardenClient->MOD_Position = uploadModule(pWardenClient->ClientID, pWardenClient->RC4_KEY_0XAE, pWardenClient->MOD_Position);
				pWardenClient->NextCheckTime = CurrentTick; // Speed up upload process, by manipulation of this parameter
				//DEBUGMSG("WARDENLOOP: Triggering check event becasue of WARDEN_DOWNLOAD_MOD for %s (*%s)", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str());				
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
					pWardenClient->removePacket();
					break;
				}
				if (pWardenClient->pWardenPacket.ThePacket[0] == 0)
				{
					pWardenClient->ErrorCount++;
					pWardenClient->WardenStatus = WARDEN_START;
					pWardenClient->NextCheckTime = CurrentTick + 50;
					pWardenClient->removePacket();
					break;
				}
				else
					if (pWardenClient->pWardenPacket.ThePacket[0] == 1)
					{
						pWardenClient->WardenStatus = WARDEN_SEND_REQUEST;
						pWardenClient->ErrorCount = 0; // Nastepne 5 szans, jako ze udalo ci sie sciagnac modul
						pWardenClient->NextCheckTime = CurrentTick + random();
						pWardenClient->removePacket();
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
				if (GetTickCount() - pWardenClient->ClientLogonTime>30000) pWardenClient->WardenStatus = WARDEN_ERROR_RESPONSE; //Daje ci 30 sekund (Na cbn jest 45)
				pWardenClient->NextCheckTime = CurrentTick + 100;
			}
		}
		break;
		case WARDEN_SEND_REQUEST:
		{
			pWardenClient->removePacket();

			switch (pWardenClient->CheckCounter)
			{
			case 0:  pWardenClient->request("D2Client.dll", D2CLIENT_MOUSEXY, 8, "D2Client.dll", D2CLIENT_UIVAR, 16 * 4); break; // MouseXY && UI VARS
			case 1:  pWardenClient->request("bnclient.dll", BNCLIENT_REDVEX, 16); break; //RedVex
			case 2:  pWardenClient->request("D2Client.dll", D2CLIENT_TMC, 4); break; //TMC
			case 3:  pWardenClient->request(pWardenClient->AnimData, 4); break; //TMC
			case 4:  pWardenClient->request(pWardenClient->AnimData2, 16); break; //TMC
			case 5:  pWardenClient->request("D2Client.dll", D2CLIENT_STING, 6); break; //Sting's MH
			case 6:  pWardenClient->request("D2Client.dll", D2CLIENT_WTFPK, 5); break; //WtfPk
			case 7:  pWardenClient->request("D2Client.dll", D2CLIENT_ONPACKETRCV, 5); break; //OnPacketRecv
			case 8:  pWardenClient->request("D2Common.dll", D2COMMON_SKILLSTXT, 4); break; //Get SkillsTxt
			case 9:  pWardenClient->request(pWardenClient->ptSkillsTxt + (0x23C * 54), 8); break; //Get Teleport Record
			case 10: pWardenClient->request(pWardenClient->ptSkillsTxt + (0x23C * 59), 8); break; //Get Blizzard Record
			case 11: pWardenClient->request(pWardenClient->ptSkillsTxt + (0x23C * 47), 8); break; //Get Fireball Record
			case 12: pWardenClient->request(pWardenClient->ptSkillsTxt + (0x23C * 22), 8); break; //Get Guided Arrow Record
			case 13: pWardenClient->request("D2Client.dll", D2CLIENT_GAMELOOP, 7); break; //GameLoop
			}
			pWardenClient->WardenStatus = WARDEN_RECEIVE_CHECK;
			pWardenClient->NextCheckTime = CurrentTick + 5000; // Give 5 seconds for answer
			pWardenClient->pWardenPacket.SendTime = CurrentTick;
		}
		break;
		case WARDEN_RECEIVE_CHECK:
		{
			if (pWardenClient->pWardenPacket.ThePacket)
			{
				if (pWardenClient->pWardenPacket.PacketLen >= 7)
				{
					switch (pWardenClient->CheckCounter)
					{
					case 0:
					{
						pWardenClient->MouseXPosition = *(WORD*)&(pWardenClient->pWardenPacket.ThePacket[12]);
						pWardenClient->MouseYPosition = *(WORD*)&(pWardenClient->pWardenPacket.ThePacket[8]);
						memcpy(pWardenClient->UIModes, pWardenClient->pWardenPacket.ThePacket + 17, 4 * 16);
#if 0
						if (pWardenClient->MouseXPosition>800 || pWardenClient->MouseYPosition>600)
							Log("Hack: %s (*%s) has mouse in abnormal position (X='%d', Y='%d')!", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str(), pWardenClient->MouseXPosition, pWardenClient->MouseYPosition);
#endif
						pWardenClient->UIModesTime = GetTickCount();
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
						pWardenClient->AnimData = *(DWORD*)&(pWardenClient->pWardenPacket.ThePacket[8]);
					}
					break;
					case 3:
					{
						pWardenClient->AnimData2 = *(DWORD*)&(pWardenClient->pWardenPacket.ThePacket[8]);
					}
					break;
					case 4:
					{
						const BYTE TMCBP[] = { 0x36, 0x00, 0x00, 0x00, 0x30, 0x49, 0x4E, 0x55, 0x48, 0x54, 0x48, 0x00, 0x01, 0x00, 0x00, 0x00 };
						const BYTE TMCOK[] = { 0x36, 0x00, 0x00, 0x00, 0x4C, 0x31, 0x4F, 0x50, 0x48, 0x54, 0x48, 0x00, 0x02, 0x00, 0x00, 0x00 };

						if (memcmp(pWardenClient->pWardenPacket.ThePacket + 8, TMCBP, 16) == 0)
						{
							char out[100];
							ConvertBytesToHexString(pWardenClient->pWardenPacket.ThePacket + 8, 16, out, 50, ',');
							Log("Hack: %s (*%s) is using TMC BP (%s)!", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str(), out);
						}
						//else 
						//if(memcmp(pWardenClient->pWardenPacket.ThePacket+8,TMCOK,16)!=0)
						//{
						//char out[100];
						//ConvertBytesToHexString(pWardenClient->pWardenPacket.ThePacket+8,16,out,50,',');
						//Log("Hack: %s (*%s) MAY use some private TMC (%s)!",pWardenClient->CharName,pWardenClient->AccountName,out);
						//}
						pWardenClient->TMCDetected = true;
					}
					break;
					case 5:
					{
						BYTE MH[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
						if (memcmp(pWardenClient->pWardenPacket.ThePacket + 8, MH, 6) == 0)
						{
							Log("Hack: %s (*%s) is using Sting's MapHack!", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str());
							pWardenClient->StingDetected = true;
						}
					}
					break;
					case 6:
					{
						BYTE WTF[] = { 0x5B, 0x59, 0xC2, 0x04, 0x00 };
						if (memcmp(pWardenClient->pWardenPacket.ThePacket + 8, WTF, 5) != 0)
						{
							char out[50];
							ConvertBytesToHexString(pWardenClient->pWardenPacket.ThePacket + 8, 5, out, 50, ',');
							Log("Hack: %s (*%s) is using WtfPk/MH hack! (%s)", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str(), out);
							pWardenClient->WtfDetected = true;
						}
					}
					break;
					case 7:
					{
						BYTE RCV[] = { 0xE8, 0xAA, 0xF1, 0xF4, 0xFF };
						if (memcmp(pWardenClient->pWardenPacket.ThePacket + 8, RCV, 5) != 0)
						{
							char out[50];
							ConvertBytesToHexString(pWardenClient->pWardenPacket.ThePacket + 8, 5, out, 50, ',');
							Log("Hack: %s (*%s) is using Packet Intercepter hack! (%s)", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str(), out);
							pWardenClient->RCVDetected = true;
						}
					}
					break;
					case 8:
					{
						pWardenClient->ptSkillsTxt = *(DWORD*)(pWardenClient->pWardenPacket.ThePacket + 8);
					}
					break;
					case 9:
					case 10:
					case 11:
					case 12:
					{
						SkillsTxt* pRec = (SkillsTxt*)(pWardenClient->pWardenPacket.ThePacket + 8);
						SkillsTxt* pSrvTxt = (*D2Vars.D2COMMON_sgptDataTables)->pSkillsTxt;

						if (pSrvTxt[pRec->wSkillId].dwFlags.bLeftSkill != pRec->dwFlags.bLeftSkill ||
							pSrvTxt[pRec->wSkillId].dwFlags.bInTown != pRec->dwFlags.bInTown ||
							pSrvTxt[pRec->wSkillId].dwFlags.bSearchEnemyXY != pRec->dwFlags.bSearchEnemyXY ||
							pSrvTxt[pRec->wSkillId].dwFlags.bSearchEnemyNear != pRec->dwFlags.bSearchEnemyNear ||
							pSrvTxt[pRec->wSkillId].dwFlags.bSearchOpenXY != pRec->dwFlags.bSearchOpenXY)
							Log("Hack: %s (*%s) modification of Skills.Txt: Skill %s, Flags : LeftSkill %d InTown %d, bSearchEnemyXY %d, bSearchEnemyNear %d, bSearchOpenXY %d", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str(), ConvertSkill(pRec->wSkillId).c_str(), pRec->dwFlags.bLeftSkill, pRec->dwFlags.bInTown, pRec->dwFlags.bSearchEnemyXY, pRec->dwFlags.bSearchEnemyNear, pRec->dwFlags.bSearchOpenXY);
						if (pWardenClient->CheckCounter == 12) pWardenClient->GMDetected = true;
					}
					break;
					case 13:
					{
						//6FAE2B40  83 EC 20 89 4C 24 0C     ƒì ‰L$.
						BYTE LP[] = { 0x83, 0xEC, 0x20, 0x89, 0x4C, 0x24, 0x0C };
						if (memcmp(pWardenClient->pWardenPacket.ThePacket + 8, LP, 7) != 0)
						{
							char out[100];
							ConvertBytesToHexString(pWardenClient->pWardenPacket.ThePacket + 8, 7, out, 100, ',');
							Log("Hack: %s (*%s) is using some private hack! (%s)", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str(), out);
							pWardenClient->LPDetected = true;
						}
					}
					break;
					}
					pWardenClient->CheckCounter++;
#ifdef VER_111B
					if (pWardenClient->CheckCounter == 1 && pWardenClient->RedVexDetected) pWardenClient->CheckCounter++;
					if (pWardenClient->CheckCounter == 2 && pWardenClient->AnimData) pWardenClient->CheckCounter++;
					if (pWardenClient->CheckCounter == 3 && pWardenClient->AnimData2) pWardenClient->CheckCounter++;
					if (pWardenClient->CheckCounter == 4 && pWardenClient->TMCDetected) pWardenClient->CheckCounter++;
					if (pWardenClient->CheckCounter == 5 && pWardenClient->StingDetected) pWardenClient->CheckCounter++;
					if (pWardenClient->CheckCounter == 6 && pWardenClient->WtfDetected) pWardenClient->CheckCounter++;
					if (pWardenClient->CheckCounter == 7 && pWardenClient->RCVDetected) pWardenClient->CheckCounter++;
					if (pWardenClient->CheckCounter == 8 && pWardenClient->ptSkillsTxt) pWardenClient->CheckCounter++;
					if (pWardenClient->CheckCounter == 9 && pWardenClient->GMDetected) pWardenClient->CheckCounter++;
					if (pWardenClient->CheckCounter == 10 && pWardenClient->GMDetected) pWardenClient->CheckCounter++;
					if (pWardenClient->CheckCounter == 11 && pWardenClient->GMDetected) pWardenClient->CheckCounter++;
					if (pWardenClient->CheckCounter == 12 && pWardenClient->GMDetected) pWardenClient->CheckCounter++;
					if (pWardenClient->CheckCounter == 13 && pWardenClient->LPDetected) pWardenClient->CheckCounter++;
					if (pWardenClient->CheckCounter>13) pWardenClient->CheckCounter = 0;
					if (pWardenClient->CheckCounter == 0 && !wcfgDetectTrick && !pWardenClient->MyIp.empty()
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
					pWardenClient->WardenStatus = WARDEN_SEND_REQUEST;
					pWardenClient->ErrorCount = 0;
				}
				else
				{
					pWardenClient->ErrorCount++;
					if (pWardenClient->ErrorCount>10) pWardenClient->WardenStatus = WARDEN_ERROR_RESPONSE;
					else pWardenClient->WardenStatus = WARDEN_SEND_REQUEST;
					pWardenClient->NextCheckTime = GetTickCount() + random();
				}
			}
			else
			{
				if (CurrentTick - pWardenClient->pWardenPacket.SendTime < 5000)
					pWardenClient->NextCheckTime = GetTickCount() + 10;
				else
				{
					pWardenClient->ErrorCount++;

					if (!pWardenClient->WardenBlocked && pWardenClient->ErrorCount > 5)
					{
						Log("Hack: %s (*%s) is blocking warden request!", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str());
						pWardenClient->WardenBlocked = true;
					}

					if (pWardenClient->ErrorCount > 5)
						pWardenClient->WardenStatus = WARDEN_ERROR_RESPONSE;
					else
						pWardenClient->WardenStatus = WARDEN_SEND_REQUEST;
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
			Log("ERROR: %s (*%s) has been exceeded the error count, player is being removed from the game (%s)", pWardenClient->CharName.c_str(), pWardenClient->AccountName.c_str(), getStatusMessage(pWardenClient->WardenStatus));
			DEBUGMSG("Removing client %s due error on step %s, counter: %d", pWardenClient->CharName.c_str(), getStatusMessage(pWardenClient->WardenStatus), pWardenClient->CheckCounter);
			pWardenClient->NextCheckTime = 0xFFFFFFFF;
			pWardenClient->WardenStatus = WARDEN_NOTHING;
			pWardenClient->removePacket();
			DWORD id = pWardenClient->ClientID;
			pWardenClient = clients.erase(pWardenClient);
			BootPlayer(id, BOOT_CONNECTION_INTERRUPTED);
			continue;
		}
		break;
		} // koniec switcha
		++pWardenClient;
	} // koniec for-a
} // koniec funkcji


