#ifndef D2WARDEN_H__
#define D2WARDEN_H__

#define MAX_CHAT_MSG_BUF 10

#define WARDEN_START	0
#define WARDEN_CHECK_CLIENT_HAS_MOD_OR_NOT	1
#define WARDEN_DOWNLOAD_MOD	2
#define WARDEN_ERROR_RESPONSE	3
#define WARDEN_WAITING_DOWNLOAD_END 4
#define WARDEN_SEND_CHECK 5
#define WARDEN_RECEIVE_CHECK 6
#define WARDEN_NOTHING 7

#define NEVER	0xFF

#define MAXERRORCOUNT 5

#define LOG_HACK 0
#define LOG_INFO 1
#define LOG_DEBUG 2

struct WardenClient;

#include "D2Structs.h"
#include <list>

using namespace std;


struct Vote
{
	int Votes;
	int Score;
	WardenClient* pVoter;
	DWORD Time;
	DWORD ThreadID;
};

struct Spec
{
	UnitAny* MyUnit;
	UnitAny* SpecUnit;
	DWORD RequesterID;
	DWORD SpecID;
};

struct WEITem
{
	DWORD ItemCode[20];
	DWORD FileIdx[20];
	BYTE ItemQuality[7];
};

struct WardenPacket
{
	DWORD ReceiveTime;
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

	string CharName;
	string AccountName;
	string MyIp;

	DWORD ClientLogonTime;
	DWORD NextCheckTime;

	BYTE WardenStatus;
	BYTE CheckCounter;
	BYTE LocaleId;
	BYTE Specing;
	BYTE NewPatch;
	BYTE DebugTrick;


	DWORD pWardenPackets_ReceiveTime;
    WardenPacket* pWardenPackets;
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
};


struct hWarden_Struct
{
	CRITICAL_SECTION WardenLock;
	list<WardenClient> Clients;
};

extern "C" void  __fastcall HashGameSeed(unsigned char *pt_0XAE_RC4_KEY, unsigned char *pt_0X66_RC4_KEY, unsigned char * TheGameSeed, unsigned int TheLength);
extern "C" void  __fastcall Double_MD5(DWORD *Mod_Length, DWORD unk, unsigned char *ptResult);

void WardenLoop();
void Warden_Init();
void Warden_Config();



#endif