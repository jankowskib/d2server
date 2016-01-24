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
*/

#ifndef __WARDEN_H__
#define __WARDEN_H__

#include <list>
#include <string>
#include <boost/random.hpp>
#include "WardenClient.h"
#include "inih\cpp\INIReader.h"

using namespace std;

enum WardenStatus
{
	WARDEN_START = 0,
	WARDEN_CHECK_CLIENT_HAS_MOD_OR_NOT,
	WARDEN_DOWNLOAD_MOD,
	WARDEN_ERROR_RESPONSE,
	WARDEN_WAITING_DOWNLOAD_END,
	WARDEN_SEND_REQUEST,
	WARDEN_RECEIVE_CHECK,
	WARDEN_NOTHING,
};

class Warden
{
public:
	Warden();
	~Warden();


	static Warden& getInstance()
	{
		static Warden warden;
		return warden;
	}

	DWORD uploadModule(DWORD ClientID, unsigned char *RC4_KEY, DWORD MOD_Position);
	void loadConfig();
	void loop(Game* pGame);
	void reloadClans();


	WardenClient_i findClientByName(string szName);
	WardenClient_i findClientByName(Game* pGame, string szName);
	WardenClient_i findClientByAcc(string szAcc);
	WardenClient_i findClientById(DWORD clientId);

	bool empty() { return clients.empty(); }
	bool isInited() { return init_succed; }

	WardenClient_i getInvalidClient() { return clients.end(); }
	DWORD getUpTime() { return uptime; }
	DWORD getClientCount() { return clients.size(); }

	DWORD __fastcall onWardenPacketReceive(Game *pGame, UnitAny *pPlayer, BYTE *pPacket, DWORD nSize);
	DWORD __fastcall onJoinGame(PacketData *pPacket);
	void onRemoveClient(WardenClient_i client);

	//configuration
	DWORD wcfgDumpInterval;
	DWORD wcfgExpRange;
	DWORD wcfgRespawnTimer;
	BYTE wcfgMaxPlayers;
	BYTE wcfgD2EXVersion;
	BYTE wcfgSpectator;
	BYTE wcfgAddKillerClass;
	BYTE wcfgAutoIdentify;
	BYTE wcfgHostileLevel;
	BYTE wcfgAllowLoggin;
	BYTE wcfgAllowTourMode;
	BYTE wcfgAllowVanilla;
	BYTE wcfgAllowGU;
	BYTE wcfgAllowHB;
	BYTE wcfgDisableHostileDelay;
	BYTE wcfgMoveToTown;
	BYTE wcfgDropEarGold;
	BYTE wcfgStrBugFix;
	BYTE wcfgFFAMode;
	BYTE wcfgEnableSeed;
	BYTE wcfgAllowD2Ex;
	BYTE wcfgAllowNLWW;
	BYTE wcfgDetectTrick;
	BYTE wcfgDisableRepairCost;
	BYTE wcfgAllowQuests;
	BYTE wcfgEnableWE;
	BYTE wcfgEnableLevelCmd;
	string wcfgDatabase;
	string wcfgDBUser;
	string wcfgDBPass;
	string wcfgConfigFile;
	string wcfgUpdateURL;
	string wcfgGSName;
	INIReader wcfgClans;
	bool clansAvailable;
	list<string> wcfgAdmins;
	BYTE wcfgTeleChars[7];

	// keeps the highest dmg amount
	int DmgRekord;

	map<DWORD, vector<string>> threadMap;

	unsigned char AE_Packet00[40];

	// World Event related
	int SellCount;
	int NextDC;
	int MinSell;
	int MaxSell;
	int InfoDelay;
	WEItem WItem;

private:

	void patchD2();

	char* getStatusMessage(WardenStatus status);

	boost::mt19937 rng;
	boost::uniform_int<int> dist;
	boost::variate_generator<boost::mt19937&, boost::uniform_int<int>> random;
	bool init_succed;
	DWORD uptime;
	bool enabled;

	DWORD moduleLength;
	BYTE *moduleEncrypted;

	list<WardenClient> clients;

	Warden(const Warden&) = delete;
	Warden& operator=(const Warden&) = delete;
};

#endif