/* ==========================================================
 * d2warden
 * https://github.com/lolet/d2warden
 * ==========================================================
 * Copyright	2008 marsgod
 *				2011-2013 Bartosz Jankowski
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

#ifndef D2WARDEN_H__
#define D2WARDEN_H__

#define MAX_CHAT_MSG_BUF 10

#define WARDEN_START	0
#define WARDEN_CHECK_CLIENT_HAS_MOD_OR_NOT	1
#define WARDEN_DOWNLOAD_MOD	2
#define WARDEN_ERROR_RESPONSE	3
#define WARDEN_WAITING_DOWNLOAD_END 4
#define WARDEN_SEND_REQUEST 5
#define WARDEN_RECEIVE_CHECK 6
#define WARDEN_NOTHING 7

#define NEVER	0xFF

#define MAXERRORCOUNT 10

#define LOG_HACK 0
#define LOG_INFO 1
#define LOG_DEBUG 2

struct WardenPacket
{
//#ifdef _DEBUG
	DWORD SendTime;
	DWORD ReceiveTime;
//#endif
	DWORD ClientID;
	DWORD PacketLen;
	BYTE *ThePacket;
};

struct WardenClient
{
	DWORD ClientID;
	DWORD ErrorCount;

	UnitAny* ptPlayer;
	Game* ptGame;
	ClientData* ptClientData;

	unsigned char SessionKey[16];

	std::string CharName;
	std::string AccountName;
	std::string MyIp;

	DWORD ClientLogonTime;
	DWORD NextCheckTime;

	BYTE WardenStatus;
	BYTE CheckCounter;
	BYTE LocaleId;
	BYTE NewPatch;
	BYTE DebugTrick;

	WardenPacket pWardenPacket;
	DWORD MOD_Position;

	unsigned char RC4_KEY_0X66[258];
	unsigned char RC4_KEY_0XAE[258];
	
	signed short MouseXPosition;
	signed short MouseYPosition;

	DWORD UIModes[16];
	DWORD UIModesTime;
	
	DWORD AnimData;
	DWORD AnimData2;

	DWORD ptSkillsTxt;

	BYTE TMCDetected;
	BYTE StingDetected;
	BYTE WtfDetected;
	BYTE RCVDetected;
	BYTE GMDetected;
	BYTE LPDetected;
	BYTE WardenBlocked;
	BYTE RedVexDetected;
	BYTE DupeDetected;

	BYTE VerCode;
	bool bNeedUpdate;
};


struct Warden
{
	CRITICAL_SECTION WardenLock;
	std::list<WardenClient> Clients;
};


struct Vote
{
	int Votes;
	int Score;
	WardenClient* pVoter;
	DWORD Time;
	DWORD ThreadID;
};

struct WEItem
{
	DWORD ItemCode[20];
	DWORD FileIdx[20];
	BYTE ItemQuality[7];
};

extern "C" void  __fastcall HashGameSeed(unsigned char *pt_0XAE_RC4_KEY, unsigned char *pt_0X66_RC4_KEY, unsigned char * TheGameSeed, unsigned int TheLength);
extern "C" void  __fastcall Double_MD5(DWORD *Mod_Length, DWORD unk, unsigned char *ptResult);

DWORD WardenLoop();
void Warden_Init();
void PatchD2();
void Warden_Config();

// Warden struct functions

typedef std::list<WardenClient>::iterator WardenClient_i;

void RemoveWardenPacket(WardenClient_i ptCurrentClient);
bool isWardenQueueEmpty();
WardenClient_i GetClientByName(char *szAcc);
WardenClient_i GetClientByAcc(char *szName);
WardenClient_i GetClientByID(DWORD CID);
WardenClient_i GetClientByName(Game* pGame, char *szName);
void SendPtrRequest(WardenClient_i ptCurrentClient, char* DllName1, DWORD Addr, BYTE Bytes);
void SendPtrRequest(WardenClient_i ptCurrentClient, DWORD Addr, BYTE Bytes);
void SendPtrRequest(WardenClient_i ptCurrentClient, char* DllName1, DWORD Addr, char* DllName2, DWORD Addr2);
void SendPtrRequest(WardenClient_i ptCurrentClient, char* DllName1, DWORD Addr, BYTE nBytes1, char* DllName2, DWORD Addr2, BYTE nBytes2);

char * WardenStatusToString(BYTE aStatus);

#endif