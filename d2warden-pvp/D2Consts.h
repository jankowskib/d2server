/* ==========================================================
 * d2warden
 * https://github.com/lolet/d2warden
 * ==========================================================
 * Copyright 2011-2014 Bartosz Jankowski
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

enum PartyButtons
{
	PB_LOOT = 1,
	PB_MUTE = 2,
	PB_SQUELCH = 3,
	PB_HOSTILE = 4,
	PB_INVITE_PLAYER = 6,
	PB_INVITE_CANCEL = 7,
	PB_INVITE_ACCEPT = 8,
	PB_LEAVE_PARTY = 9,
	// -- my addtion below
	PB_SPECATE = 10,
};

enum ExEventMsgs
{
	EXEVENT_TEXTMSG = 1,
	EXEVENT_OVERHEAD = 2,
	EXEVENT_DOWNLOAD = 3,
	EXEVENT_MAPREVEAL = 4,
	EXEVENT_OPTIONS = 5
};

enum ExEventOption
{
	EXOP_RESPAWNTIME = 1,
};

enum EventMsgType
{
	EVENT_TIMEOUT = 0,
	EVENT_DROPPED = 1,
	EVENT_JOINED = 2,
	EVENT_LEFT = 3,
	EVENT_NOT_IN_GAME = 4,
	EVENT_NOT_LOGGED = 5,
	EVENT_SLAIN = 6,
	EVENT_PARTY = 7,
	EVENT_BUSY = 8,
	EVENT_TRADE_WAIT = 9,
	EVENT_ITEMS_BOX = 10,
	EVENT_GUILD_BOOTED = 11,
	EVENT_GUILD_UNK = 12,
	EVENT_NOT_LISTEN = 13,
	EVENT_NO_MANA = 14,
	EVENT_REALM_SHUTDOWN = 15,
	EVENT_HOSTILE_WAIT = 16,
	EVENT_SOJ_SELL = 17,
	EVENT_DIABLO_WALKS = 18
};

enum D2Langs
{
	LOC_EN = 0,
	LOC_PL = 10
};

enum ExSounds
{
	STAND_SELECT = 2,
	STAND_CLICK = 4,
	STAND_WINOPEN = 5,
	STAND_LEVLEUP = 7,
	STAND_ARCANEPORTAL = 2556,
	D2EX_IMPRESSIVE = 4699,
	D2EX_DOUBLEKILL = 4700,
	D2EX_FIRSTBLOOD = 4701,
	D2EX_HEADSHOT = 4702,
	D2EX_HOLYSHIT = 4703,
	D2EX_HUMILATION = 4704,
	D2EX_KILLINGSPREE = 4705,
	D2EX_MONSTERKILL = 4706,
	D2EX_PERFECT = 4707,
	D2EX_PLAY = 4708,
	D2EX_PREPARE = 4709,
	D2EX_TRIPLEKILL = 4710,
	D2EX_UNSTOPPABLE = 4711,
	D2EX_DOMINATION = 4712,
	D2EX_ACHIVEMENT = 4713
};

enum UnitTypes
{
	UNIT_PLAYER = 0,
	UNIT_MONSTER = 1,
	UNIT_OBJECT = 2,
	UNIT_MISSILE = 3,
	UNIT_ITEM = 4,
	UNIT_TILE = 5
};

enum UNITFLAG
{
	UNITFLAG_OPERATED = 0x1,
	UNITFLAG_SELECTABLE = 0x2,
	UNITFLAG_ATTACKABLE = 0x4,
	UNITFLAG_VALIDTARGET = 0x8,
	UNITFLAG_SEEDSET = 0x10,
	UNITFLAG_DEATHVANISH = 0x20,
	UNITFLAG_SKILLEND = 0x40,
	UNITFLAG_NEWMODE = 0x80,
	UNITFLAG_HOVERMSG = 0x100,
	UNITFLAG_HASHOVERBAR = 0x200,
	UNITFLAG_CLIENTSOUND = 0x400,
	UNITFLAG_SUMMONER = 0x800,
	UNITFLAG_REFRESH = 0x1000,
	UNITFLAG_NONEUTRAL = 0x2000,
	UNITFLAG_CLIENTUPDATE = 0x8000,
	UNITFLAG_DEAD = 0x10000,
	UNITFLAG_NOITEMS = 0x20000,
	UNITFLAG_AISTARTED = 0x80000,
	UNITFLAG_NEUTRAL = 0x200000,
	UNITFLAG_UNITINIT = 0x1000000,
	UNITFLAG_OTHERINIT = 0x2000000,
	UNITFLAG_NOEXP = 0x4000000,
	UNITFLAG_PETIGNORE = 0x40000000,
	UNITFLAG_ISREVIVE = 0x80000000,
};


enum D2Colors
{
	COL_WHITE = 0x0,
	COL_RED = 0x1,
	COL_LIGHTGREEN = 0x2,
	COL_BLUE = 0x3,
	COL_DARK_GOLD = 0x4,
	COL_GREY = 0x5,
	COL_BLACK = 0x6,
	COL_GOLD = 0x7,
	COL_ORANGE = 0x8,
	COL_YELLOW = 0x9,
	COL_DARKGREEN = 0xA,
	COL_PURPLE = 0xB,
	COL_GREEN = 0xC,
	COL_WHITE2 = 0xD,
	COL_BLACK2 = 0xE,
	COL_DARKWHITE = 0xF,
};

enum ItemLocation
{
	ITEMLOC_NONE = 0,			// Not equipped
	ITEMLOC_HEAD,				// Head
	ITEMLOC_AMULET,			// Amulet
	ITEMLOC_BODY,				// Body armor
	ITEMLOC_RIGHT_PRIMARY,	// Right primary slot
	ITEMLOC_LEFT_PRIMARY,		// Left primary slot
	ITEMLOC_RIGHT_RING,		// Right ring
	ITEMLOC_LEFT_RING,		// Left ring
	ITEMLOC_BELT,				// Belt
	ITEMLOC_FEET,				// Feet
	ITEMLOC_GLOVES,			// Gloves
	ITEMLOC_RIGHT_SECONDARY,	// Right secondary slot
	ITEMLOC_LEFT_SECONDARY,	// Left secondary slot
};


enum UnitModes
{
	PLAYER_MODE_DEATH = 0,				// death
	PLAYER_MODE_STAND_OUTTOWN,			// standing outside town
	PLAYER_MODE_WALK_OUTTOWN,			// walking outside town
	PLAYER_MODE_RUN,					// running
	PLAYER_MODE_BEING_HIT,				// being hit
	PLAYER_MODE_STAND_INTOWN,			// standing inside town
	PLAYER_MODE_WALK_INTOWN,			// walking outside town
	PLAYER_MODE_ATTACK1,				// attacking 1
	PLAYER_MODE_ATTACK2,				// attacking 2
	PLAYER_MODE_BLOCK,					// blocking
	PLAYER_MODE_CAST,					// casting spell
	PLAYER_MODE_THROW,					// throwing
	PLAYER_MODE_KICK,					// kicking (assassin)
	PLAYER_MODE_USESKILL1,				// using skill 1
	PLAYER_MODE_USESKILL2,				// using skill 2
	PLAYER_MODE_USESKILL3,				// using skill 3
	PLAYER_MODE_USESKILL4,				// using skill 4
	PLAYER_MODE_DEAD,					// dead
	PLAYER_MODE_SEQUENCE,				// sequence
	PLAYER_MODE_BEING_KNOCKBACK
};		// being knocked back

enum MonsterMode
{
	NPC_MODE_DEATH = 0,		// NPC death
	NPC_MODE_STAND,			// NPC standing still
	NPC_MODE_WALK,			// NPC walking
	NPC_MODE_BEING_HIT,		// NPC getting hit
	NPC_MODE_ATTACK1,		// NPC attacking 1
	NPC_MODE_ATTACK2,		// NPC attacking 2
	NPC_MODE_BLOCK,			// NPC blocking
	NPC_MODE_CAST,			// NPC casting spell skill
	NPC_MODE_USESKILL1,		// NPC using skill 1
	NPC_MODE_USESKILL2,		// NPC using skill 2
	NPC_MODE_USESKILL3,		// NPC using skill 3
	NPC_MODE_USESKILL4,		// NPC using skill 4
	NPC_MODE_DEAD,			// NPC dead
	NPC_MODE_BEING_KNOCKBACK,// NPC being knocked back
	NPC_MODE_SEQUENCE,		// NPC sequence
	NPC_MODE_RUN			// NPC running
};


enum ItemMode
{
	ITEM_MODE_INV_STASH_CUBE_STORE = 0,	// Item inven stash cube store
	ITEM_MODE_EQUIPPED = 1,					// Item equipped self or merc
	ITEM_MODE_IN_BELT = 2,					// Item in belt
	ITEM_MODE_ON_GROUND = 3,				// Item on ground
	ITEM_MODE_ON_CURSOR = 4,				// Item on cursor
	ITEM_MODE_BEING_DROPPED = 5,			// Item being dropped
	ITEM_MODE_SOCKETED_IN_ITEM = 6		// Item socketed in item
};

enum ItemQuality
{
	ITEM_QUALITY_INVALID = 0,
	ITEM_QUALITY_LOW,
	ITEM_QUALITY_NORMAL,
	ITEM_QUALITY_SUPERIOR,
	ITEM_QUALITY_MAGIC,
	ITEM_QUALITY_SET,
	ITEM_QUALITY_RARE,
	ITEM_QUALITY_UNIQUE,
	ITEM_QUALITY_CRAFTED,
	ITEM_QUALITY_TEMPERED
};

enum ItemFlags
{
	ITEMFLAG_NEWITEM = 0x00000001,
	ITEMFLAG_TARGET = 0x00000002,
	ITEMFLAG_TARGETING = 0x00000004,
	ITEMFLAG_DELETED = 0x00000008,
	ITEMFLAG_IDENTIFIED = 0x00000010,
	ITEMFLAG_QUANTITY = 0x00000020,
	ITEMFLAG_WEAPONSET_IN = 0x00000040,
	ITEMFLAG_WEAPONSET_OUT = 0x00000080,
	ITEMFLAG_BROKEN = 0x00000100,
	ITEMFLAG_REPAIRED = 0x00000200,
	ITEMFLAG_UNKNOWN2 = 0x00000400,
	ITEMFLAG_SOCKETED = 0x00000800,
	ITEMFLAG_NON_SELLABLE = 0x00001000,
	ITEMFLAG_INSTORE = 0x00002000,
	ITEMFLAG_NOEQUIP = 0x00004000,
	ITEMFLAG_NAMED = 0x00008000,
	ITEMFLAG_ORGAN = 0x00010000,
	ITEMFLAG_SELLCHEAP = 0x00020000,
	ITEMFLAG_UNK5 = 0x00040000,
	ITEMFLAG_INIT = 0x00080000,
	ITEMFLAG_UNK6 = 0x00100000,
	ITEMFLAG_COMPACTSAVE = 0x00200000,
	ITEMFLAG_ETHEREAL = 0x00400000,
	ITEMFLAG_JUSTSAVED = 0x00800000,
	ITEMFLAG_PERSONALIZED = 0x01000000,
	ITEMFLAG_LOWQUALITY = 0x02000000,
	ITEMFLAG_RUNEWORD = 0x04000000,
	ITEMFLAG_ITEM = 0x08000000
};

enum QuestsId
{
	ACT1_INTRO = 0x00, //a1q0 		act introduction
	ACT1_DEN = 0x01, //a1q1   Den of Evil
	ACT1_RAVEN = 0x02, //a1q2   Sisters' Burial Grounds
	ACT1_IMBUE = 0x03, //a1q3   Tools of the Trade
	ACT1_CAIN = 0x04, //a1q4   Rescue Deckard Cain
	ACT1_COUNTESS = 0x05, //a1q5   The Forgotten Tower
	ACT1_ANDARIEL = 0x06, //a1q6   Sisters to the Slaughter
	ACT1_UNUSED = 0x07,   //       <<not used???>>
	ACT2_INTRO = 0x08, //a2q0 	act introduction
	ACT2_RADAMENT = 0x09, //a2q1   Radament's Lair
	ACT2_STAFF = 0x0A, //a2q2   The Horadric Staff
	ACT2_AMU = 0x0B, //a2q3   The Tainted Sun
	ACT2_ARCANE = 0x0C, //a2q4   The Arcane Sanctuary
	ACT2_SUMMONER = 0x0D, //a2q5   The Summoner
	ACT2_DURIEL = 0x0E, //a2q6   The Tomb of Tal Rasha
	ACT2_UNUSED = 0x0F,        //  <<not used???>>
	ACT3_INTRO = 0x10, //a3q0 	act introduction
	ACT3_BOOK = 0x11, //a3q1   Lam Esen's Tome
	ACT3_KHALIM = 0x12, //a3q2   Khalim's Will
	ACT3_GIDBIN = 0x13, //a3q3   Blade of The Old Religion
	ACT3_GOLDENBIRD = 0x14, //a3q4   The Golden Bird   
	ACT3_TRAVINCAL = 0x15, //a3q5   The Blackened Temple
	ACT3_MEPH = 0x16, //a3q6   The Lord of Hatred
	ACT3_UNUSED = 0x17,        //  <<not used???>>
	ACT4_IZUAL = 0x18, //a4q1   The Fallen Angel
	ACT4_DIABO = 0x19, //a4q2   Terror's End
	ACT4_FORGE = 0x1A, //a4q3   The Hellforge
	ACT4_FORGE_EX = 0x1B,         // (referred to) (also items "hfh" and "dss" -- "hfh" is Hellforge Hammer,   but "dss" not in item tables)
	ACT4_FORGE_EXX = 0x1C,       //   (referred to in Hellforge code)
	ACT4_UNUSED = 0x1D,        // <<not used???>>
	ACT2_Q7 = 0x1E, //a2q7   vestigial reference???
	ACT2_Q8 = 0x1F, //a2q8   vestigial reference???
	ACT3_Q7 = 0x20, //a3q7   vestigial reference???
	ACT_UNUSED = 0x21,         // <<not used???>>
	ACT_UNUSED2 = 0x22,       //   <<not used???>>
	ACT5_SHENK = 0x23, //a5q1   The Siege of Harrogath
	ACT5_RUNEQUEST = 0x24, //a5q2   Rescue on Mount Arreat
	ACT5_ANYA = 0x25, //a5q3   Prison of Ice
	ACT5_NITHALAK = 0x26, //a5q4   The Betrayal of Harrogath
	ACT5_ANCIENTS = 0x27, //a5q5   The Rite of Passage
	ACT5_BAAL = 0x28 //a5q6   Eve of Destruction
};

enum DmgResultFlag
{
	RESULTFLAG_HIT = 0x00000001,
	RESULTFLAG_DEATH = 0x00000002,
	RESULTFLAG_GETHIT = 0x00000004,
	RESULTFLAG_KNOCKBACK = 0x00000008,
	RESULTFLAG_BLOCK = 0x00000010,
	RESULTFLAG_DISALLOW_EVENTS = 0x00000020,
	RESULTFLAG_DODGE = 0x00000100,
	RESULTFLAG_AVOID = 0x00000200,
	RESULTFLAG_SOFTHIT = 0x00002000,
	RESULTFLAG_WEAPONBLOCK = 0x00008000
};

enum DmgHitFlag
{
	HITFLAG_SRCDAMAGE_MISSILE = 0x00000001,
	HITFLAG_SUCCESSFUL_CRITICAL_HIT = 0x00000002,
	HITFLAG_FRIENDLY_FIRE = 0x00000010,
	HITFLAG_SRCDAMAGE_MELEE = 0x00000020,
	HITFLAG_BYPASS_UNDEAD = 0x00000100,
	HITFLAG_BYPASS_BEASTS = 0x00000400,
	HITFLAG_BYPASS_DEMONS = 0x00000200,
};

enum Staty
{
	STAT_STRENGTH = 0		// str
	, STAT_ENERGY = 1		// energy
	, STAT_DEXTERITY = 2		// dexterity
	, STAT_VITALITY = 3		// vitality
	, STAT_HP = 6		// life
	, STAT_MAXHP = 7		// max life
	, STAT_MANA = 8		// mana
	, STAT_MAXMANA = 9		// max mana
	, STAT_STAMINA = 10		// stamina
	, STAT_MAXSTAMINA = 11		// max stamina
	, STAT_LEVEL = 12		// level
	, STAT_EXP = 13		// experience
	, STAT_GOLD = 14		// gold
	, STAT_GOLDBANK = 15		// stash gold
	, STAT_TOBLOCK = 20		// to block
	, STAT_DEFENSE = 30
	, STAT_DAMAGEREDUCTION = 36		// damage reduction
	, STAT_MAGICDAMAGEREDUCTION = 35		// magic damage reduction
	, STAT_MAGICRESIST = 37		// magic resist
	, STAT_MAXMAGICRESIST = 38		// max magic resist
	, STAT_FIRERESIST = 39		// fire resist
	, STAT_MAXFIRERESIST = 40		// max fire resist
	, STAT_LIGHTNINGRESIST = 41		// lightning resist
	, STAT_MAXLIGHTNINGRESIST = 42		// max lightning resist
	, STAT_COLDRESIST = 43		// cold resist
	, STAT_MAXCOLDRESIST = 44		// max cold resist
	, STAT_POISONRESIST = 45		// poison resist
	, STAT_MAXPOISONRESIST = 46		// max poison resist
	, STAT_LIFELEECH = 60		// Life Leech
	, STAT_MANALEECH = 62		// Mana Leech
	, STAT_VELOCITYPERCENT = 67		// effective run/walk
	, STAT_AMMOQUANTITY = 70		// ammo quantity(arrow/bolt/throwing)
	, STAT_DURABILITY = 72		// item durability
	, STAT_MAXDURABILITY = 73		// max item durability
	, STAT_HPREGEN = 74		// Life Replenish
	, STAT_GOLDFIND = 79		// Gold find (GF)
	, STAT_MAGICFIND = 80		// magic find (MF)
	, STAT_IAS = 93		// IAS
	, STAT_FASTERRUNWALK = 96		// faster run/walk
	, STAT_FASTERHITRECOVERY = 99		// faster hit recovery
	, STAT_FASTERBLOCK = 102		// faster block rate
	, STAT_FASTERCAST = 105		// faster cast rate
	, STAT_POISONLENGTHREDUCTION = 110		// Poison length reduction
	, STAT_OPENWOUNDS = 135		// Open Wounds
	, STAT_CRUSHINGBLOW = 136		// crushing blow
	, STAT_DEADLYSTRIKE = 141		// deadly strike
	, STAT_FIREABSORBPERCENT = 142		// fire absorb %
	, STAT_FIREABSORB = 143		// fire absorb
	, STAT_LIGHTNINGABSORBPERCENT = 144		// lightning absorb %
	, STAT_LIGHTNINGABSORB = 145		// lightning absorb
	, STAT_COLDABSORBPERCENT = 148		// cold absorb %
	, STAT_COLDABSORB = 149		// cold absorb
	, STAT_SLOW = 150		// slow %
	, STAT_SOCKETS = 194		// number of sockets
};



enum UIModes
{
	UI_INVENTORY = 0x01
	, UI_CHARACTER = 0x02
	, UI_MINISKILL = 0x03
	, UI_SKILL = 0x04
	, UI_CHAT = 0x05
	, UI_NPCMENU = 0x08
	, UI_MAINMENU = 0x09
	, UI_AUTOMAP = 0x0a
	, UI_CONFIG = 0x0b
	, UI_NPCSHOP = 0x0c
	, UI_ALTDOWN = 0x0d
	, UI_ANVIL = 0x0e
	, UI_QUEST = 0x0f
	, UI_QUESTLOG = 0x11
	, UI_STATUSAREA = 0x12
	, UI_WPMENU = 0x14
	, UI_MINIPANEL = 0x15
	, UI_PARTY = 0x16
	, UI_TRADE = 0x17
	, UI_MSGS = 0x18
	, UI_STASH = 0x19
	, UI_CUBE = 0x1a
	, UI_BELT = 0x1f
	, UI_HELP = 0x21
	, UI_MERC = 0x24
	, UI_SCROLL = 0x25
};

///////////////////////////////////////////////////
// Common Spells
///////////////////////////////////////////////////

#define D2S_INVALID				0xffff // should never happen
#define D2S_ATTACK				0x0000
#define D2S_KICK				0x0001
#define D2S_THROW				0x0002
#define D2S_UNSUMMON			0x0003
#define D2S_LEFTTHROW			0x0004
#define D2S_LEFTSWING			0x0005
#define D2S_TOMEOFIDENTIFY		0x00da
#define D2S_SCROLLOFIDENTIFY	0x00d9
#define D2S_TOMEOFTOWNPORTAL	0x00dc
#define D2S_SCROLLOFTOWNPORTAL	0x00db

///////////////////////////////////////////////////////////////
// Amazon Spells
///////////////////////////////////////////////////////////////

// Javelin and Spear spells
#define D2S_JAB					0x000a
#define D2S_IMPALE				0x0013
#define D2S_FEND				0x001e
#define D2S_POWERSTRIKE			0x000e
#define D2S_CHARGEDSTRIKE		0x0018
#define D2S_LIGHTNINGSTRIKE		0x0022
#define D2S_POISONJAVELIN		0x000f
#define D2S_LIGHTNINGBOLT		0x0014
#define D2S_PLAGUEJAVELIN		0x0019
#define D2S_LIGHTNINGFURY		0x0023

// Passive and Magic spells
#define D2S_INNERSIGHT			0x0008
#define D2S_SLOWMISSILES		0x0011
#define D2S_DECOY				0x001c
#define D2S_VALKYRIE			0x0020
#define D2S_DODGE				0x000d
#define D2S_AVOID				0x0012
#define D2S_EVADE				0x001d
#define D2S_CRITICALSTRIKE		0x0009
#define D2S_PENETRATE			0x0017
#define D2S_PIERCE				0x0021

// Bow and Crossbow spells
#define D2S_COLDARROW			0x000b
#define D2S_ICEARROW			0x0015
#define D2S_FREEZINGARROW		0x001f
#define D2S_MAGICARROW			0x0006
#define D2S_MULTIPLESHOT		0x000c
#define D2S_GUIDEDARROW			0x0016
#define D2S_STRAFE				0x001a
#define D2S_FIREARROW			0x0007
#define D2S_EXPLODINGARROW		0x0010
#define D2S_IMMOLATIONARROW		0x001b


///////////////////////////////////////////////////////////////
// Assassin Spells
///////////////////////////////////////////////////////////////

// Martial Arts
#define D2S_FISTSOFFIRE			0x0103
#define D2S_CLAWSOFTHUNDER		0x010d
#define D2S_BLADESOFICE			0x0112
#define D2S_TIGERSTRIKE			0x00fe
#define D2S_COBRASTRIKE			0x0109
#define D2S_PHOENIXSTRIKE		0x0118
#define D2S_DRAGONTALON			0x00ff
#define D2S_DRAGONCLAW			0x0104
#define D2S_DRAGONTAIL			0x010e
#define D2S_DRAGONFLIGHT		0x0113

// Shadow Disciplines
#define D2S_BURSTOFSPEED		0x0102
#define D2S_FADE				0x010b
#define D2S_VENOM				0x0116
#define D2S_CLAWMASTERY			0x00fc
#define D2S_WEAPONBLOCK			0x0107
#define D2S_SHADOWWARRIOR		0x010c
#define D2S_SHADOWMASTER		0x0117
#define D2S_PSYCHICHAMMER		0x00fd
#define D2S_CLOAKOFSHADOWS		0x0108
#define D2S_MINDBLAST			0x0111

// Traps
#define D2S_SHOCKWEB			0x0100
#define D2S_CHARGEDBOLTSENTRY	0x0105
#define D2S_LIGHTNINGSENTRY		0x010f
#define D2S_DEATHSENTRY			0x0114
#define D2S_FIREBLAST			0x00fb
#define D2S_WAKEOFFIRE			0x0106
#define D2S_WAKEOFINFERNO		0x0110
#define D2S_BLADESENTINEL		0x0101
#define D2S_BLADEFURY			0x010a
#define D2S_BLADESHIELD			0x0115


///////////////////////////////////////////////////////////////
// Barbarian Spells
///////////////////////////////////////////////////////////////

// Warcries
#define D2S_HOWL				0x0082
#define D2S_TAUNT				0x0089
#define D2S_BATTLECRY			0x0092
#define D2S_WARCRY				0x009a
#define D2S_SHOUT				0x008a
#define D2S_BATTLEORDERS		0x0095
#define D2S_BATTLECOMMAND		0x009b
#define D2S_FINDPOTION			0x0083
#define D2S_FINDITEM			0x008e
#define D2S_GRIMWARD			0x0096

// Combat Masteries
#define D2S_SWORDMASTERY		0x007f
#define D2S_POLEARMMASTERY		0x0086
#define D2S_INCREASEDSTAMINA	0x008d
#define D2S_INCREASEDSPEED		0x0094
#define D2S_AXEMASTERY			0x0080
#define D2S_THROWINGMASTERY		0x0087
#define D2S_MACEMASTERY			0x0081
#define D2S_SPEARMASTERY		0x0088
#define D2S_IRONSKIN			0x0091
#define D2S_NATURALRESISTANCE	0x0099

// Combat spells
#define D2S_LEAP				0x0084
#define D2S_LEAPATTACK			0x008f
#define D2S_WHIRLWIND			0x0097
#define D2S_BASH				0x007e
#define D2S_STUN				0x008b
#define D2S_CONCENTRATE			0x0090
#define D2S_BERSERK				0x0098
#define D2S_DOUBLESWING			0x0085
#define D2S_DOUBLETHROW			0x008c
#define D2S_FRENZY				0x0093


///////////////////////////////////////////////////////////////
// Druid Spells
///////////////////////////////////////////////////////////////

// Elemental
#define D2S_FIRESTORM			0x00e1
#define D2S_MOLTENBOULDER		0x00e5
#define D2S_FISSURE				0x00ea
#define D2S_VOLCANO				0x00f4
#define D2S_ARMAGEDDON			0x00f9
#define D2S_TWISTER				0x00f0
#define D2S_TORNADO				0x00f5
#define D2S_HURRICANE			0x00fa
#define D2S_ARCTICBLAST			0x00e6
#define D2S_CYCLONEARMOR		0x00eb

// Shape Shifting
#define D2S_WEREWOLF			0x00df
#define D2S_FERALRAGE			0x00e8
#define D2S_RABIES				0x00ee
#define D2S_FURY				0x00f8
#define D2S_LYCANTHROPY			0x00e0
#define D2S_FIRECLAWS			0x00ef
#define D2S_HUNGER				0x00f2
#define D2S_WEREBEAR			0x00e4
#define D2S_MAUL				0x00e9
#define D2S_SHOCKWAVE			0x00f3

// Summoning
#define D2S_OAKSAGE				0x00e2
#define D2S_HEARTOFWOLVERINE	0x00ec
#define D2S_SPIRITOFBARBS		0x00f6
#define D2S_RAVEN				0x00dd
#define D2S_SUMMONSPIRITWOLF	0x00e3
#define D2S_SUMMONDIREWOLF		0x00ed
#define D2S_SUMMONGRIZZLY		0x00f7
#define D2S_POISONCREEPER		0x00de
#define D2S_CARRIONVINE			0x00e7
#define D2S_SOLARCREEPER		0x00f1


///////////////////////////////////////////////////////////////
// Necromancer Spells
///////////////////////////////////////////////////////////////

// Summoning Spells
#define D2S_SKELETONMASTERY		0x0045
#define D2S_GOLEMMASTERY		0x004f
#define D2S_SUMMONRESIST		0x0059
#define D2S_CLAYGOLEM			0x004b
#define D2S_BLOODGOLEM			0x0055
#define D2S_IRONGOLEM			0x005a
#define D2S_FIREGOLEM			0x005e
#define D2S_RAISESKELETON		0x0046
#define D2S_RAISESKELETALMAGE	0x0050
#define D2S_REVIVE				0x005f

// Poison and Bone Spells
#define D2S_POISONDAGGER		0x0049
#define D2S_POISONEXPLOSION		0x0053
#define D2S_POISONNOVA			0x005c
#define D2S_TEETH				0x0043
#define D2S_CORPOSEEXPLOSION	0x004a
#define D2S_BONESPEAR			0x0054
#define D2S_BONESPIRIT			0x005d
#define D2S_BONEARMOR			0x0044
#define D2S_BONEWALL			0x004e
#define D2S_BONEPRISON			0x0058

// Curses
#define D2S_DIMVISION			0x0047
#define D2S_CONFUSE				0x0051
#define D2S_ATTRACT				0x0056
#define D2S_AMPLIFYDAMAGE		0x0042
#define D2S_IRONMAIDEN			0x004c
#define D2S_LIFETAP				0x0052
#define D2S_LOWERRESIST			0x005b
#define D2S_WEAKEN				0x0048
#define D2S_TERROR				0x004d
#define D2S_DECREPIFY			0x0057


///////////////////////////////////////////////////////////////
// Paladin Spells
///////////////////////////////////////////////////////////////

// Defensive Auras
#define D2S_PRAYER				0x0063
#define D2S_CLEANSING			0x006d
#define D2S_MEDITATION			0x0078
#define D2S_DEFIANCE			0x0068
#define D2S_VIGOR				0x0073
#define D2S_REDEMPTION			0x007c
#define D2S_RESISTFIRE			0x0064
#define D2S_RESISTCOLD			0x0069
#define D2S_RESISTLIGHTNING		0x006e
#define D2S_SALVATION			0x007d

// Offensive Auras
#define D2S_MIGHT				0x0062
#define D2S_BLESSEDAIM			0x006c
#define D2S_CONCENTRATION		0x0071
#define D2S_FANATICISM			0x007a
#define D2S_HOLYFIRE			0x0066
#define D2S_HOLYFREEZE			0x0072
#define D2S_HOLYSHOCK			0x0076
#define D2S_THORNS				0x0067
#define D2S_SANCTUARY			0x0077
#define D2S_CONVICTION			0x007b

// Combat spells
#define D2S_SACRIFICE			0x0060
#define D2S_ZEAL				0x006a
#define D2S_VENGEANCE			0x006f
#define D2S_CONVERSION			0x0074
#define D2S_HOLYBOLT			0x0065
#define D2S_BLESSEDHAMMER		0x0070
#define D2S_FISTOFTHEHEAVENS	0x0079
#define D2S_SMITE				0x0061
#define D2S_CHARGE				0x006b
#define D2S_HOLYSHIELD			0x0075


///////////////////////////////////////////////////////////////
// Sorceress Spells
///////////////////////////////////////////////////////////////

// Cold Spells
#define D2S_FROSTNOVA			0x002c
#define D2S_BLIZZARD			0x003b
#define D2S_FROZENORB			0x0040
#define D2S_ICEBOLT				0x0027
#define D2S_ICEBLAST			0x002d
#define D2S_GLACIALSPIKE		0x0037
#define D2S_COLDMASTERY			0x0041
#define D2S_FROZENARMOR			0x0028
#define D2S_SHIVERARMOR			0x0032
#define D2S_CHILLINGARMOR		0x003c

// Lightning Spells
#define D2S_STATICFIELD			0x002a
#define D2S_NOVA				0x0030
#define D2S_THUNDERSTORM		0x0039
#define D2S_CHARGEDBOLT			0x0026
#define D2S_LIGHTNING			0x0031
#define D2S_CHAINLIGHTNING		0x0035
#define D2S_LIGHTNINGMASTERY	0x003f
#define D2S_TELEKINESIS			0x002b
#define D2S_TELEPORT			0x0036
#define D2S_ENERGYSHIELD		0x003a

// Fire Spells
#define D2S_INFERNO				0x0029
#define D2S_BLAZE				0x002e
#define D2S_FIREWALL			0x0033
#define D2S_FIREBOLT			0x0024
#define D2S_FIREBALL			0x002f
#define D2S_METEOR				0x0038
#define D2S_FIREMASTERY			0x003d
#define D2S_WARMTH				0x0025
#define D2S_ENCHANT				0x0034
#define D2S_HYDRA				0x003e


enum D2States
{
	freeze = 0x1,
	poison = 0x2,
	resistfire = 0x3,
	resistcold = 0x4,
	resistlight = 0x5,
	resistmagic = 0x6,
	playerbody = 0x7,
	resistall = 0x8,
	amplifydamage = 0x9,
	frozenarmor = 0xA,
	cold = 0xB,
	inferno = 0xC,
	blaze = 0xD,
	bonearmor = 0xE,
	concentrate = 0xF,
	enchant = 0x10,
	innersight = 0x11,
	skill_move = 0x12,
	weaken = 0x13,
	chillingarmor = 0x14,
	stunned = 0x15,
	spiderlay = 0x16,
	dimvision = 0x17,
	slowed = 0x18,
	fetishaura = 0x19,
	shout = 0x1A,
	taunt = 0x1B,
	conviction = 0x1C,
	convicted = 0x1D,
	energyshield = 0x1E,
	venomclaws = 0x1F,
	battleorders = 0x20,
	might = 0x21,
	prayer = 0x22,
	holyfire = 0x23,
	thorns = 0x24,
	defiance = 0x25,
	thunderstorm = 0x26,
	lightningbolt = 0x27,
	blessedaim = 0x28,
	stamina = 0x29,
	concentration = 0x2A,
	holywind = 0x2B,
	holywindcold = 0x2C,
	cleansing = 0x2D,
	holyshock = 0x2E,
	sanctuary = 0x2F,
	meditation = 0x30,
	fanaticism = 0x31,
	redemption = 0x32,
	battlecommand = 0x33,
	preventheal = 0x34,
	conversion = 0x35,
	uninterruptable = 0x36,
	ironmaiden = 0x37,
	terror = 0x38,
	attract = 0x39,
	lifetap = 0x3A,
	confuse = 0x3B,
	decrepify = 0x3C,
	lowerresist = 0x3D,
	openwounds = 0x3E,
	dopplezon = 0x3F,
	criticalstrike = 0x40,
	dodge = 0x41,
	avoid = 0x42,
	penetrate = 0x43,
	evade = 0x44,
	pierce = 0x45,
	warmth = 0x46,
	firemastery = 0x47,
	lightningmastery = 0x48,
	coldmastery = 0x49,
	swordmastery = 0x4A,
	axemastery = 0x4B,
	macemastery = 0x4C,
	polearmmastery = 0x4D,
	throwingmastery = 0x4E,
	spearmastery = 0x4F,
	increasedstamina = 0x50,
	ironskin = 0x51,
	increasedspeed = 0x52,
	naturalresistance = 0x53,
	fingermagecurse = 0x54,
	nomanaregen = 0x55,
	justhit = 0x56,
	slowmissiles = 0x57,
	shiverarmor = 0x58,
	battlecry = 0x59,
	blue = 0x5A,
	red = 0x5B,
	death_delay = 0x5C,
	valkyrie = 0x5D,
	frenzy = 0x5E,
	berserk = 0x5F,
	revive = 0x60,
	skel_mastery = 0x61,
	sourceunit = 0x62,
	redeemed = 0x63,
	healthpot = 0x64,
	holyshield = 0x65,
	just_portaled = 0x66,
	monfrenzy = 0x67,
	corpse_nodraw = 0x68,
	alignment = 0x69,
	manapot = 0x6A,
	shatter = 0x6B,
	sync_warped = 0x6C,
	conversion_save = 0x6D,
	pregnant = 0x6E,
	golem_mastery = 0x6F,
	rabies = 0x70,
	defense_curse = 0x71,
	blood_mana = 0x72,
	burning = 0x73,
	dragonflight = 0x74,
	maul = 0x75,
	corpse_noselect = 0x76,
	shadowwarrior = 0x77,
	feralrage = 0x78,
	skilldelay = 0x79,
	progressive_damage = 0x7A,
	progressive_steal = 0x7B,
	progressive_other = 0x7C,
	progressive_fire = 0x7D,
	progressive_cold = 0x7E,
	progressive_lightning = 0x7F,
	shrine_armor = 0x80,
	shrine_combat = 0x81,
	shrine_resist_lightning = 0x82,
	shrine_resist_fire = 0x83,
	shrine_resist_cold = 0x84,
	shrine_resist_poison = 0x85,
	shrine_skill = 0x86,
	shrine_mana_regen = 0x87,
	shrine_stamina = 0x88,
	shrine_experience = 0x89,
	fenris_rage = 0x8A,
	wolf = 0x8B,
	bear = 0x8C,
	bloodlust = 0x8D,
	changeclass = 0x8E,
	attached = 0x8F,
	hurricane = 0x90,
	armageddon = 0x91,
	invis = 0x92,
	barbs = 0x93,
	wolverine = 0x94,
	oaksage = 0x95,
	vine_beast = 0x96,
	cyclonearmor = 0x97,
	clawmastery = 0x98,
	cloak_of_shadows = 0x99,
	recycled = 0x9A,
	weaponblock = 0x9B,
	cloaked = 0x9C,
	quickness = 0x9D,
	bladeshield = 0x9E,
	fade = 0x9F,
	summonresist = 0xA0,
	oaksagecontrol = 0xA1,
	wolverinecontrol = 0xA2,
	barbscontrol = 0xA3,
	debugcontrol = 0xA4,
	itemset1 = 0xA5,
	itemset2 = 0xA6,
	itemset3 = 0xA7,
	itemset4 = 0xA8,
	itemset5 = 0xA9,
	itemset6 = 0xAA,
	runeword = 0xAB,
	restinpeace = 0xAC,
	corpseexp = 0xAD,
	whirlwind = 0xAE,
	fullsetgeneric = 0xAF,
	monsterset = 0xB0,
	delerium = 0xB1,
	antidote = 0xB2,
	thawing = 0xB3,
	staminapot = 0xB4,
	passive_resistfire = 0xB5,
	passive_resistcold = 0xB6,
	passive_resistltng = 0xB7,
	uberminion = 0xB8,
};
