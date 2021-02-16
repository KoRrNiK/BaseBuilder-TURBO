new const PLUGIN[] 	= 	"BaseBuilder TURBO";
new const VERSION[] 	= 	"v1.3.3";
new const AUTHOR[] 	= 	"KoRrNiK";
new const PREFIXSAY[] 	=  	"^xc2^xa6 BaseBuilder ^xc2^xa6^x01";
new const fVAULTFILE[]	=	 "TURBOBB";

new bool:superAdminLocalhost = false;

new const accentMotdColor[]	=	"#ff003c";
new const accentColorHud[]	=	{ 120, 250, 50 };


#define DAY 				86400
#define HOUR 				3600
#define MINUTE 				60
#define OFFSET_CLIPAMMO        		51
#define OFFSET_LINUX_WEAPONS   		4

#define OFFSET_LASTHITGROUP 		75
#define OFFSET_LINUX_PLAYER		5

#define HUD_HIDE_MONEY 			(1<<5)
#define	HUD_HIDE_FLASH 			(1<<1)
#define	HUD_HIDE_TIMER 			(1<<4)
#define HUD_HIDE_RHA 			(1<<3)

#define fm_cs_get_weapon_ammo(%1,%2)    set_pdata_int(%1, OFFSET_CLIPAMMO, %2, OFFSET_LINUX_WEAPONS)

#define isPlayer(%1) 			((1 <= %1 && %1 < maxPlayers))
#define forPlayer(%1) 			for(new %1 = 1; %1 < maxPlayers; %1++)
#define forArray(%1,%2) 		for(new %1 = 0; %1 < sizeof(%2); %1++)

#pragma semicolon 	1
#pragma dynamic 	32768

native bb_set_in_fire(id, victim, duration);
native bb_set_in_ice(id, victim, duration);
native bool:bb_is_in_barrier(id);

	/*----------------------*\
--------| SMS			 |
	\*----------------------*/


new timeVip[33];
new timeSVip[33];
new Float:fCodeCheckTime;
new szSmsCode[14];
new lastBuyer;
new userService[33];
new userLuzCoin[33];

#define TOTAL_BUY_SMSSHOP_PRICES 10
new const pricesMenu[TOTAL_BUY_SMSSHOP_PRICES][5][]={
	{"SKLEP", "72480",	"2,46",		"1.00",		"6"	},	//2
	{"SKLEP", "73480",	"3,69",		"1.50",		"9"	},	//3
	{"SKLEP", "74480",	"4,92",		"2.00",		"12"	},	//4
	{"SKLEP", "75480",	"6,15",		"2.50",		"15"	},	//5
	{"SKLEP", "76480",	"7,38",		"3.00",		"18"	},	//6
	{"SKLEP", "79480",	"11,07",	"4.50",		"27"	},	//9
	{"SKLEP", "91400",	"17,22",	"7.00",		"56"	},	//10
	{"SKLEP", "91900",	"23,37",	"9.50",		"76"	},	//11
	{"SKLEP", "92022",	"24,60",	"10.00",	"80"	},	//11
	{"SKLEP", "92521",	"30,75",	"12.50",	"100"	}	//12
};

#define TOTAL_BUY_SMSSHOP 5
new const itemsShop[TOTAL_BUY_SMSSHOP][3][]={
	// NAZWA							| CENA		| OPIS
	  {"Luzacki Vip [30 Dni]", 					"18",		"Otrzymujesz Luzackiego Vipa ( Wiecej pod /vip )"}
	 ,{"Super Luzacki Vip [30 Dni]", 					"27",		"Otrzymujesz Super Luzackiego Vipa ( Wiecej pod /vip )"}
	,{"Zwoj Doswiadczenia (2x Exp) [6 Godziny]", 			"12",		"Otrzymujesz dwa razy wiecej EXP'a za strzelanie"}
	,{"Zwoj Szczescia (2x Brylki) [6 Godziny]", 			"12",		"Otrzymujesz dwa razy wiecej Brylek za zabicie"}
	,{"Boost - Kopalnia (2x Szybciej) [55 Minut]",			"7",		"Gobliny kopia 2 razy szybciej!"}
};

	/*----------------------*\
--------| TASK			 |
	\*----------------------*/

enum ( += 100){
	
	TASK_CLOCK = 100,
	TASK_MODEL,
	TASK_RESPAWN,
	TASK_IDLESOUND,
	TASK_GLOBAL,
	TASK_ADDEXP,
	TASK_ADDNUGGET,
	TASK_ADDBONE,
	TASK_IDLEZOMBIE,
	TASK_LOGIN,
	TASK_GRAVITY,
	TASK_AFK,
	TASK_MINER,
	TASK_SKILL_BLOOD,
	TASK_CHECKFPS,
	TASK_CLASS_POISON,
	TASK_PUSH,
	TASK_CODE,
	TASK_DAMAGEDEAL,
	TASK_EXPLODETORPED,
	TASK_SHOW_AD,
};


new userPoisonKiller[33];
new userPoison[33];

	/*----------------------*\
--------| USER			 |
	\*----------------------*/
	
	#define MAXBITAUTHID 35
	#define MAXBITIP 32
	
	new userClone[33];
	new userCloned[33];
	new userNoClip[33];
	new userGodMod[33];
	new userAllowBuild[33];
	new userAdmin[33];
	new userVip[33];
	new userSVip[33];
	new userConnected[33];
	new bool:userJetPack[33];
	new userName[33][33];
	new userIp[33][MAXBITIP];
	new userSid[33][MAXBITAUTHID];
	new userTeam[33];
	new userMenuId;
	new userTeamMenu[33];
	new userTeamSend[33];
	new Float:userTimeConnectServer[33];
	new userPlayerConnected[33];
	new userTime[33];
	new userMoveAs[33];
	new Float:userPushBlock[33] ;
	new userPlayerList[33][33];
	new userMenuPlayer[33];
	new bool:userReconnected[33];
	new userRoundTeam[33];
	new userGotGrenades[33];
	new userLevel[33];
	new Float:userExp[33];
	new Float:userExpShow[33];
	new Float:userLastExpDealt[33];
	new userNugget[33];
	new userNuggetShow[33];
	new Float:userLastNuggetDealt[33];
	new userBone[33];
	new userBoneShow[33];
	new Float:userLastBoneDealt[33];
	new userGrab[33];
	new userHudMoving[33];
	new Float:userAimingHud[33];
	new userBlocksUsed[33];
	new Float:userLength[33];
	new userVarList[33][33];
	new userVarMenu[33];
	new bool:userCanGrab[33];
	new Float:userTeamLine[33];
	new userTeamBlock[33];
	new userTeamExp[33];
	new userTeamNugget[33];
	new userHudDeal[33];
	new userHudGet[33];
	new userNuggetCollectedRound[33];
	new userTeamOpen[33];
	new userSelectWeapon[33];
	new userFirstSpawn[33];
	new bool:userExtraFps[33];
	new Float:userInfoFps[33];
	new userReset[33];
	new userNuggetAll[33];
	new userJetpackSpeed[33] = 500;
	new userLastAwardTime[33];
	enum { BLOCK_COLOR, BLOCK_RENDER, BLOCK_NORENDER };
	new userMoverBlockColor[33];
	new userSqlId[33];
	new userAllDmg[33];
	new userAllAward[33];
	new userMaxDmg[33];
	new userFov[33];
	new Float:userAfkValue[33];
	new userScrollExp[33];
	new userScrollNugget[33];
	new userBoostMine[33];
	new userMute[33];
	new bool:serverOffChat;
	new userPush[33];
	new userSuggestTeam[33];
	new userMaxHelp[33];
	new userDayHelp[33];
	new userHelpPoint[33];
	new Float:userSpeedAdmin[33];
	new userFirstLogin[33][33];
	new userSecretPoint[33];
	new userLastAwardFree[33];
	new userLastAwardRow[33];
	new userLastAwardGot[33];
	new userCheckCamp[33];
	new userDarkScreen[33];
	new userPageColor[33];
	new bool:jumpBlock[33];

new userStaminaDayRefresh[33];
new userLastStaminaTime[33];
new bool:userMinePayGoblin[33];

new userLastUpgradeTime[33];
new unlockCave[33];
new startUpgrade[33];

	
new userMineNugget[33];
//new Float:userInfoMine[33];
enum { mine_STONE = 0, mine_COAL, mine_IRON, mine_GOLD, mine_DIAMOND, mine_TOTAL };
new const mineOre[mine_TOTAL][2][] = {
	// NAZWA	| TWARDOSC
	  { "Kamien" ,	"100"}
	,{ "Wegiel",	"150"}
	,{ "Zelazo",	"200"}
	,{ "Zloto",	"80"}
	,{ "Diament",	"250"}
	
};
new userDigging[33][mine_TOTAL];
new userTypeMine[33];

enum { up_PICKAXE, up_STAMINA, up_LUCK, up_GOBLIN,up_POWER, up_SEARCH,up_TRUCK, up_TOTAL };
new const upgradeMine[up_TOTAL][4][] = {
	// NAZWA		| OPIS								| POZIOM	| CENA
	  { "Kilof" ,		"Umozliwia wykopanie wiekszej ilosci Mineralow ( Bonus )",	"10",		"2500"}
	,{ "Wyczerpanie",	"Gobliny moga dluzej kopac w kopalni",				"5",		"2750"}
	,{ "Szczescie",		"Szansa na poczworne wykopki ( Wykopane brylki x4 z Mineralow)","5",		"2000"}
	,{ "Gobliny",		"Im wiecej posiadasz goblinow tym szybciej kopiesz Mineraly!",	"4",		"3000"}
	,{ "Koncowka Kilofa" ,	"Zwieksza Moc kopania",						"5",		"2250"}
	,{ "Szukanie" ,		"Pozwala Ci znajdywac wiecej rodzajow Mineralow",		"4",		"1750"}
	,{ "Wozek" ,		"Posiadasz wiekszy wozek ( Wiekszy magazyn Mineralow )",	"2",		"1500"}
};

new costMiner		=	15000;		// CENA KUPNA KOPALNI
new levelMiner 		=	10;		// POTRZEBNY LVL DO KUPNA KOPALNI
new staminaTime 		= 	3600;		// PODSTAWOWY CZAS STAMINY GOBLINOW
new powerMine 		=	2;		// PO ILE MA DODAWAC MOC KOPANIA ZA LVL
new Float:speedMine 	= 	15.0;		// ILE SEKUND TRWA JEDENO UDEZENIE W MINERAL
new mineralMax 		= 	3000;		// STANDARDOWA WARTOSC WUZKA W KOPALNI
new upgradeCave 		= 	5400;		// CZAS ILE SIE KUPUJE KOPALNIA
new truckMine		=	1000;		// + ILE MA DODAWAC WUZKA ZA LVL
new Float:boostAmount	=	2.0;		// ILE RAZY MA SZYBCIEJ KOPAC

new userUpgradeMine[33][up_TOTAL];
new userSaveOption[33];
enum {  PLAYER_HUD_RED, PLAYER_HUD_GREEN, PLAYER_HUD_BLUE , PLAYER_TOTAL };
new userHud[33][PLAYER_TOTAL];	
enum{save_TEAM,save_SPRAY,save_MODELS, save_CAVE, save_INVIS, save_SOUND, save_TOTAL};
enum { 
	MENU_VIEW_BLOCK = 0, MENU_CHANGE_TEAM, MENU_PASSWORD, MENU_GIVING_VIP, MENU_GIVING_NUGGET, MENU_GIVING_LUZCOIN, MENU_GIVING_LVL, MENU_GIVING_SKILLPOINT , MENU_GIVING_TIME, MENU_GIVING_RESET,
	MENU_GIVING_EXP, MENU_PLAYER_NUGGET, MENU_PLAYER_AWARD, MENU_PLAYER_STAMINA, MENU_PLAYER_MUTE, MENU_PLAYER_WARNING, MENU_PLAYER_CAMP, MENU_SELECT_PLAYER, MENU_GIVING_BONES, MENU_CASE_CREATE,
	MENU_GIVING_EXP_ALL, MENU_GIVING_BONES_ALL, MENU_GIVING_NUGGET_ALL,MENU_GIVING_VIP_ALL, MENU_GIVING_SVIP_ALL, MENU_GIVING_SVIP
};
enum ( <<= 1 ){B1 = 1, B2, B3, B4, B5, B6, B7, B8, B9, B0 };

new const flagVip[]	=	"t";
new const flagSVip[]	=	"s";
public bool:isSuperAdmin(id)	return (!!(has_flag(id, "a")));
public bool:isAdmin(id)		return (!!(has_flag(id, "b")));
public bool:isVip(id)		return (!!(has_flag(id, flagVip)) || userVip[id]  || (timeVip[id] - get_systime() > 0));
public bool:isSVip(id)		return (!!(has_flag(id, flagSVip)) || userSVip[id]  || (timeSVip[id] - get_systime() > 0));
	
	
#define MAXHUDDEAL 5
new const Float:hudPosHit[9][1] = {
	0.4, 0.44, 0.48, 0.52, 0.56, 0.60, 0.64, 0.68, 0.72
};
	
new reconnectTable[33][33];
new Float:reconnectTableTime[33];
new Float:fOriginSave[33][3];
new Float:fOffset[33][3];
new bool:extraClone;

new userFPS[33];

new gTime;
new gBarrier;
new bool:buildTime;
new bool:prepTime;
new bool:roundEnd;
new bool:gameTime;
new bool:roundGood;
new bool:clockStop;

new gGameTime		=	230;
new gBuildTime 		= 	145;
new gPrepTime 		= 	40;


new const hpSVipHuman	=	40;
new const hpSVipZombi	=	800;

new const hpVipHuman	=	25;
new const hpVipZombi	=	500;


new const userAwardTime	=	18000;
	
new maxEnts 		= 	1024;	

	/*----------------------*\
--------| WEAPON		 |
	\*----------------------*/
	
#define TOTALWEAPONS 24	
	
new userWeaponSelect[33];

	
new userWeaponHs[33][TOTALWEAPONS];
new userWeaponKill[33][TOTALWEAPONS];
new userWeaponLevel[33][TOTALWEAPONS];
new Float:userWeaponDamage[33][TOTALWEAPONS];

new const weapons[][] = { 
	"", "weapon_p228", "", "weapon_scout", "weapon_hegrenade", "weapon_xm1014", "weapon_c4", "weapon_mac10",
	"weapon_aug", "weapon_smokegrenade", "weapon_elite", "weapon_fiveseven", "weapon_ump45", "weapon_sg550", "weapon_galil",
	"weapon_famas", "weapon_usp", "weapon_glock18", "weapon_awp", "weapon_mp5navy", "weapon_m249", "weapon_m3", "weapon_m4a1",
	"weapon_tmp", "weapon_g3sg1", "weapon_flashbang", "weapon_deagle", "weapon_sg552", "weapon_ak47", "weapon_knife", "weapon_p90" 
};

new const MAXLVL		=	60;
new const allGuns[TOTALWEAPONS][6][]={
	// GIVE			| NAME			| LEVEL		| TIME		| MAX LV: 	| KILLS
	  {"weapon_glock18", 	"Glock",		"0",		"0",		"10",		"100"   }
	,{"weapon_usp",     	"Usp",			"2",		"600",		"10",		"100" 	}
	,{"weapon_p228",    	"P228",    		"3",		"1800",		"10",		"100" 	}  
	,{"weapon_fiveseven",	"FiveSeven",    		"4",		"2400",		"10",		"100" 	}
	,{"weapon_deagle",  	"Deagle",  		"5",		"3600",		"10",		"100" 	}
	,{"weapon_elite",   	"Dual Elite", 		"6",		"4200",		"10",		"100" 	}	  
	,{"weapon_tmp",     	"Tmp",      		"7",		"7200",		"6",		"150" 	}      
	,{"weapon_mac10",   	"Mac10",    		"10",		"7800",		"6",		"150" 	}
	,{"weapon_ump45",   	"Ump45",    		"12",		"8400",		"6",		"150" 	}
	,{"weapon_mp5navy", 	"Mp5",      		"14",		"10800",	"6",		"150" 	}  
	,{"weapon_p90",     	"P90",      		"16",		"18000",	"6",		"150" 	}
	,{"weapon_scout",   	"Scout",   		"17",		"21600",	"5",		"50" 	}
	,{"weapon_awp",     	"Awp",      		"19",		"21600",	"5",		"50" 	}  
	,{"weapon_m3",      	"M3 Shotgun",   		"21",		"28800",	"3",		"225" 	}
	,{"weapon_xm1014",  	"Xm1014 Shotgun",	"23",		"32400",	"3",		"225" 	}
	,{"weapon_galil",    	"Galil",     		"26",		"36000",	"4",		"350" 	}
	,{"weapon_famas",   	"Famas",   		"30",		"43200",	"4",		"350" 	}
	,{"weapon_sg552",   	"SG552",    		"33", 		"46800",	"4",		"400" 	}      
	,{"weapon_aug",     	"Aug",      		"36",  		"50400",	"4",		"400" 	}
	,{"weapon_m4a1",    	"M4",       		"40",  		"54000",	"7",		"650" 	}
	,{"weapon_ak47",    	"Ak47",     		"45", 		"64800",	"7",		"650" 	}
	,{"weapon_sg550",   	"SG550",    		"50",		"72000",	"8",		"700" 	}
	,{"weapon_g3sg1",   	"G3SG1",    		"55",		"90000",	"8",		"700" 	}
	,{"weapon_m249",    	"M249",    	 	"60",		"126000",	"10",		"1000" 	}
};

new Float:paramWeaponFloat[TOTALWEAPONS][] = {
	// MAX DMG	| COST		| MIN * ( lvl * 0.5 ) - MAX * ( lvl * 2):
	  { 65.0,		100.0,		1.0}		// GLOCK
	,{ 65.0,		100.0,		1.0 }		// USP
	,{ 65.0,		100.0,		1.0 }		// P228
	,{ 65.0,		100.0,		1.0 }		// FIVESEVEN
	,{ 65.0,		100.0,		1.0 }		// DEAGLE
	,{ 65.0,		100.0,		1.0 }		// ELITE
	,{ 20.0,		100.0,		1.0 }		// TMP
	,{ 20.0,		100.0,		1.0 }		// MAC10
	,{ 20.0,		100.0,		1.0 }		// UMP45
	,{ 20.0,		100.0,		0.5 }		// MP5
	,{ 20.0,		100.0,		1.0 }		// P90
	,{ 40.0,		100.0,		1.5 }		// SCOUT
	,{ 40.0,		100.0,		1.5 }		// AWP
	,{ 35.0,		100.0,		0.5 }		// M3
	,{ 35.0,		100.0,		0.5 }		// XM1014
	,{ 30.0,		100.0,		0.5 }		// GALIL
	,{ 30.0,		100.0,		0.5 }		// FAMAS
	,{ 20.0,		100.0,		0.5 }		// SG552
	,{ 20.0,		100.0,		0.5 }		// AUG
	,{ 15.0,		100.0,		0.5 }		// M4A1
	,{ 15.0,		100.0,		0.5 }		// AK47
	,{ 10.0,		100.0,		0.5 }		// SG550
	,{ 10.0,		100.0,		0.5 }		// G3SG1
	,{ 25.5,		100.0,		0.5 }		// M249
	
	
};

new const g_MaxClipAmmo[] = {0,13,0,10,0,7,0,30,30,0,15,20,25,30,35,25,12,20,10,30,100,8,30,30,20,0,7,30,30,0,50};
new bool:userWeaponBool[33];


	/*----------------------*\
--------| SOUND / SPRITE	 |
	\*----------------------*/

new const TEAMSPRITE[]		=	"sprites/basebuildervt/teams3.spr";	
new const BLUEZSPRITE[]		=	"sprites/basebuildervt/bluez.spr";
new const LASERSPRITE[]		=	"sprites/basebuildervt/laserbeam.spr";
new const THUNDER[]		=	"sprites/basebuildervt/lgtning.spr";
new const EXPLODE[]		=	"sprites/basebuildervt/poison.spr";
new const LEVELUP[]		=	"sprites/basebuildervt/pousprite.spr";
new const EXPLODEFIREBALL[]	=	"sprites/basebuildervt/explo.spr";
new const EXPLODICEBOLT[]	=	"sprites/basebuildervt/iceboltexplode.spr";
new const POUICEBOLT[]		=	"sprites/basebuildervt/iceBoltPou.spr";
new const SHROOMSLEEP[]		=	"sprites/basebuildervt/sleepshroom.spr";

new const modelPoison[]		=	"models/basebuildervt/bottle.mdl";
new const modelField[]		=	"models/basebuildervt/field.mdl";
new const modelTrap[]		=	"models/basebuildervt/trap.mdl";
new const modelAmmo[]		=	"models/basebuildervt/ammobox.mdl";
new const modelBomb[]		=	"models/basebuildervt/bomb.mdl";
new const modelIce[]		=	"models/basebuildervt/icebonus.mdl";
new const modelFire[]		=	"models/basebuildervt/firebonus.mdl";
new const modelTrapBomb[]	=	"models/basebuildervt/bombertrap.mdl";
new const modelRocket[] 		= 	"models/basebuildervt/rocket.mdl";
new const modelRocketDefault[] 	= 	"models/rpgrocket.mdl";

new const classPoison[]		=	"poisonClass";
new const classField[]		=	"electricClass";
new const classTrap[]		=	"trapClass";
new const classAmmo[]		=	"ammoClass";
new const classFireBall[]	=	"fireBallClass";
new const classIceBolt[]		=	"iceBoldClass";
new const classBomb[]		=	"bombClass";
new const classbombTrap[]	=	"bombTrapClass";
new const rocketClass[] 		= 	"rocketClass";


new spriteBeam;
new sprite_explode;
new sprite_pouFire;
new sprite_pouIce;
new sprite_expIceBolt;
new sprite_expFireball;
new team_spr;
new sprite_bluez;
new thunder;
new sprite_sleepshroom;

enum{ sound_BUILD, sound_PREP , sound_START, sound_START2, sound_WINCT, sound_WINTT, sound_MOVESTART, sound_MOVESTOP, sound_HELLO,sound_LAUGHT, sound_PICKUP,sound_TOTAL };
new const soundsGame[sound_TOTAL][]={
	
	"basebuildervt/phase_build3.wav",
	"basebuildervt/phase_prep3.wav",
	"basebuildervt/round_start.wav",
	"basebuildervt/round_start2.wav",
	"basebuildervt/win_builders2.wav",
	"basebuildervt/win_zombies2.wav",
	"basebuildervt/block_drop.wav",
	"basebuildervt/block_grab.wav",
	"basebuildervt/hello.wav",
	"basebuildervt/laugh1.wav",
	"basebuildervt/pickupgem.wav",
	
};
enum { bonus_sound_ELECTRO,   bonus_sound_TRAP,  bonus_sound_FIRE, bonus_sound_ICEBOLT, bonus_sound_QUADDAMAGE,bonus_sound_POISON, bonus_sound_BRICK, bonus_sound_TOTAL };
new const bonusSound[bonus_sound_TOTAL][] = {
	  "basebuildervt/electroshock.wav"
	,"basebuildervt/trap.wav"
	,"basebuildervt/fireballexplode.wav"
	,"basebuildervt/iceboltexplode.wav"
	,"basebuildervt/quaddamage.wav"
	,"basebuildervt/glasspoison.wav"
	,"basebuildervt/brick.wav"
};
new const zombieSound[15][] ={
	
	"basebuildervt/zombie/pain/pain1.wav",		// 0
	"basebuildervt/zombie/pain/pain2.wav",		// 1
	"basebuildervt/zombie/pain/pain3.wav" ,		// 2
	"basebuildervt/zombie/death/death1.wav",		// 3	
	"basebuildervt/zombie/death/death2.wav",		// 4
	"basebuildervt/zombie/death/death2.wav" ,		// 5
	"basebuildervt/zombie/idle/idle1.wav",		// 6
	"basebuildervt/zombie/idle/idle2.wav",		// 7
	"basebuildervt/zombie/idle/idle3.wav" ,		// 8
	"basebuildervt/zombie/hit/hit1.wav",		// 9
	"basebuildervt/zombie/hit/hit2.wav",		// 10
	"basebuildervt/zombie/hit/hit3.wav" ,		// 11
	"basebuildervt/zombie/miss/miss1.wav",		// 12
	"basebuildervt/zombie/miss/miss2.wav",		// 13
	"basebuildervt/zombie/miss/miss3.wav" 		// 14
	
};

	/*----------------------*\
--------| STUCK			 |
	\*----------------------*/

new const Float:size[][3] = {
	{0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}, {0.0, 1.0, 0.0}, {0.0, -1.0, 0.0}, {1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}, {-1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0, 1.0, -1.0}, {-1.0, -1.0, 1.0}, {1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0}, {-1.0, -1.0, -1.0},
	{0.0, 0.0, 2.0}, {0.0, 0.0, -2.0}, {0.0, 2.0, 0.0}, {0.0, -2.0, 0.0}, {2.0, 0.0, 0.0}, {-2.0, 0.0, 0.0}, {-2.0, 2.0, 2.0}, {2.0, 2.0, 2.0}, {2.0, -2.0, 2.0}, {2.0, 2.0, -2.0}, {-2.0, -2.0, 2.0}, {2.0, -2.0, -2.0}, {-2.0, 2.0, -2.0}, {-2.0, -2.0, -2.0},
	{0.0, 0.0, 3.0}, {0.0, 0.0, -3.0}, {0.0, 3.0, 0.0}, {0.0, -3.0, 0.0}, {3.0, 0.0, 0.0}, {-3.0, 0.0, 0.0}, {-3.0, 3.0, 3.0}, {3.0, 3.0, 3.0}, {3.0, -3.0, 3.0}, {3.0, 3.0, -3.0}, {-3.0, -3.0, 3.0}, {3.0, -3.0, -3.0}, {-3.0, 3.0, -3.0}, {-3.0, -3.0, -3.0},
	{0.0, 0.0, 4.0}, {0.0, 0.0, -4.0}, {0.0, 4.0, 0.0}, {0.0, -4.0, 0.0}, {4.0, 0.0, 0.0}, {-4.0, 0.0, 0.0}, {-4.0, 4.0, 4.0}, {4.0, 4.0, 4.0}, {4.0, -4.0, 4.0}, {4.0, 4.0, -4.0}, {-4.0, -4.0, 4.0}, {4.0, -4.0, -4.0}, {-4.0, 4.0, -4.0}, {-4.0, -4.0, -4.0},
	{0.0, 0.0, 5.0}, {0.0, 0.0, -5.0}, {0.0, 5.0, 0.0}, {0.0, -5.0, 0.0}, {5.0, 0.0, 0.0}, {-5.0, 0.0, 0.0}, {-5.0, 5.0, 5.0}, {5.0, 5.0, 5.0}, {5.0, -5.0, 5.0}, {5.0, 5.0, -5.0}, {-5.0, -5.0, 5.0}, {5.0, -5.0, -5.0}, {-5.0, 5.0, -5.0}, {-5.0, -5.0, -5.0}
};
new stuck[33];

	/*----------------------*\
--------| ACCOUNT		 |
	\*----------------------*/

new bool:userLogged[33];
new bool:userLoaded[33];
new userPassword[33][12];

	/*----------------------*\
--------| ZOMBIE | CT		 |
	\*----------------------*/

enum{class_CLASSIC, class_SPEED, class_FAT, class_TANK, class_DRACULA, class_SNOWMAN,  class_DEVIL, class_HEALTH, class_POISON, class_DEATH, class_TERMINATOR, class_DEMON, class_TOTAL};
new const classesZombies[class_TOTAL][8][] = {
	// NAZWA		| HP		| SPEED		| LVL		| GODZ / sek	| MODEL			| RECE						| OPIS					
	{"Klasyczne",     		"2500",         	"250",         	"0",        	"0",        	"bb_classic",    		"models/basebuildervt/v_hands.mdl",		"Klasyczne Zombi | Balans"}
	,{"Szybkie",     		"1600",         	"350",         	"3",        	"1800",    	"bb_nbh",    		"models/basebuildervt/v_knive_zombie.mdl",	"Bardzo szybkie"}
	,{"Grube",     		"3500",         	"230",         	"7",        	"7200",    	"bb_ntank",    		"models/basebuildervt/v_cso_tank2.mdl",		"Posiada duzo HP, otrzymuje mniej dmg"}
	,{"Tank",     		"5500",         	"225",         	"10",       	"7800",     	"bb_ntanko",    		"models/basebuildervt/v_cso_tank2.mdl",		"Posiada Bardzo duzo HP ale jest powolne"}
	,{"Dracula",     		"3000",         	"275",        	"20",        	"21500",     	"bb_nwitch",    		"models/basebuildervt/v_zombie_guerilla.mdl",	"Gdy ma zginac zaczyna sie leczyc szansa 25%!"}
	,{"Balwan",     		"4000",         	"250",         	"25",        	"28000",     	"bb_snowman",    		"models/basebuildervt/v_snowman.mdl",		"Odporny na grananty zamrazajace!"}
	,{"Plomyk",    		"4000",		"250",         	"27",        	"30500",     	"bb_devil",    		"models/basebuildervt/v_devil.mdl",		"Odporny na grananty podpalajace!"}
	,{"Pielegniarka", 	"3500",		"250",		"35",		"40500",	"bb_nnurse",		"models/basebuildervt/v_nurse.mdl",		"Po smierci ulecza dookola siebie zombie 250HP z szansa 15%" }
	,{"Truciciel",		"2750",		"250",		"39",		"50000",	"bb_npoisoner",		"models/basebuildervt/v_poison.mdl",		"Zatruwa CT na 5sek 2dmg/s - Nie zabija zostaje min-hp | Leczy go Fiolka" }
	,{"Smierc",		"3000",		"275",		"44",		"60000",	"bb_ndeath",		"models/basebuildervt/v_death.mdl",		"Posiada 5% na zabicie Budowniczego 1/1" }
	,{"Terminator",		"4000",		"300",		"50",		"70000",	"bb_nterminator",	"models/basebuildervt/v_terminator.mdl",	"Posiada szybsze rece zadaje po 10dmg" }
	,{"Demon",		"5000",		"220",		"59",		"120000",	"bb_ndemon",		"models/basebuildervt/v_demon.mdl",		"Dostaje 15% mniej obrazenia"}

};
new userNewClass[33];
new userClass[33];
new userMaxHealth[33];
new Float:userMaxSpeed[33];
new userMaxArmor[33];
new bool:userDraculaUsed[33];
new Float:userDracula[33];
//new userClassSelect[33]

new const MODELHANDS[]	=	 "models/basebuildervt/kosa/v_hand.mdl";

new const MAXLVLCLASS 	=	30;
enum{ human_FREE, human_HEALER, human_ELEKTRYK, human_LAB, human_HUNTER,  human_SHOOTER, human_BULLDOZER,human_MAG, human_ICEMAG, human_AIM, human_BOOMBERMA, human_TRUPOSZ, human_SEARCH, human_MINER,  human_TOTAL };

new const defaultCostClass = 7500;
new const classesHuman[human_TOTAL][5][] = {	
	// NAZWA		| OPIS												| CENA 		| ULEPSZENIE CENA	| ID MOCY
	  {"Budowniczy",		"Posiada szanse na dodatokwe Brylki podczas strzelania",					"0",		"1",			"-1"}
	,{"Lekarz", 		"Posiada wiecej HP + wiekszy DMG! Mozliwosc uleczenia sojusznikow", 				"0",		"1",			"3"}
	,{"Elektryk", 		"Posiada szanse, ze strzelisz piorunem oraz Posiada Stojak Elektryczny", 			"0",		"1",			"2"}
	,{"Naukowiec", 		"Posiada butelke z trucinza! Oraz szanse na odnowienie", 					"0",		"1",			"0"}
	,{"Mysliwy", 		"Posiada pulapke z kolcami na zombi! Oraz szanse na spowolnienie", 				"0",		"1",			"1"}
	,{"Strzelec",		"Posiada szanse na dodatkowa amunicje! Posiada stacyjke z amunicja!",				"0",		"1",			"4"}
	,{"Buldozer",		"Posiada szanse na odepchniecie Zombi! Posiada pierscien odpychajacy!",				"0",		"1",			"5"}
	,{"Mag Ognia",		"Posiada kule ognia i moze nia strzelic! Posiada szasne na podpalenie Zombi!",			"0",		"1",			"6"}
	,{"Lodowy Mag",		"Posiada kule lodowa i moze nia strzelic! Posiada szasne na zamrozenie Zombi!",			"0",		"1",			"7"}
	,{"Aimer",		"Posiada 100% na HSA oraz kilka % na trafienie w glowe strzelajac gdzies indziej!",		"0",		"1",			"8"}
	,{"BomberMan",		"Posiada Bombe ktora wybucha! Oraz na pojawienie sie pulapki po zabiciu Zombi",			"0",		"1",			"9"}
	,{"Truposz",		"Moze wlaczyc DMG x2! Wbudowany rykoszet + krytyk",						"0",		"1",			"10"}
	,{"Poszukiwacz",	"Posiada wieksza szanse na dropniecie skrzyni! Oraz rakiete ktora moze sterowac",		"0",		"1",			"11"}
	,{"Gornik",		"Ma szanse na wypadniecie brylek podczas strzelania! Oraz szybkostrzelnosc",			"0",		"1",			"12"}
	
};
new const classHumanKnifeModel[human_TOTAL][2][] = {
	
	  { "models/basebuildervt/kosa/v_budowniczy.mdl", 	"models/basebuildervt/kosa/p_budowniczy.mdl" } 
	,{ "models/basebuildervt/kosa/v_lekarz.mdl" , 	"models/basebuildervt/kosa/p_lekarz.mdl" } 
	,{ "models/basebuildervt/kosa/v_elektryk.mdl" , 	"models/basebuildervt/kosa/p_elektryk.mdl" } 
	,{ "models/basebuildervt/kosa/v_naukowiec.mdl", 	"models/basebuildervt/kosa/p_naukowiec.mdl" } 
	,{ "models/basebuildervt/kosa/v_mysliwy.mdl", 	"models/basebuildervt/kosa/p_mysliwy.mdl" } 
	,{ "models/basebuildervt/kosa/v_strzelec.mdl" ,	"models/basebuildervt/kosa/p_strzelec.mdl" }
	,{ "models/basebuildervt/kosa/v_buldozer.mdl", 	"models/basebuildervt/kosa/p_buldozer.mdl" } 
	,{ "models/basebuildervt/kosa/v_firemag.mdl", 	"models/basebuildervt/kosa/p_firemag.mdl" } 
	,{ "models/basebuildervt/kosa/v_icemag2.mdl", 	"models/basebuildervt/kosa/p_icemag2.mdl" } 
	,{ "models/basebuildervt/kosa/v_aim.mdl", 	"models/basebuildervt/kosa/p_aim.mdl" } 
	,{ "models/basebuildervt/kosa/v_bomberman.mdl" ,  	"models/basebuildervt/kosa/p_bomberman.mdl" } 
	,{ "models/basebuildervt/kosa/v_truposz.mdl", 	"models/basebuildervt/kosa/p_truposz.mdl" } 
	,{ "models/basebuildervt/kosa/v_poszukiwacz.mdl" ,"models/basebuildervt/kosa/p_poszukiwacz.mdl" } 
	,{ "models/basebuildervt/kosa/v_pickaxe.mdl", 	"models/basebuildervt/kosa/p_pickaxe.mdl" } 
	
};
new const paramClassesHuman[human_TOTAL][4][] = {
	// TYP (1.1)	|  TYP (1.2)		| TYP (2.1)	|  TYP (2.2)
	  { "0.05",		"_",			"_",		"_" }
	,{ "_",			"1",			"_",		"1" }
	,{ "0.05",		"20",			"0.1",		"_" }
	,{ "0.05",		"_",			"_",		"_" }
	,{ "0.09",		"_",			"_",		"_" }
	,{ "0.11",		"_",			"_",		"_" }
	,{ "0.04",		"_",			"20",		"_" }
	,{ "0.10",		"_",			"_",		"_" }
	,{ "0.10",		"_",			"_",		"_" }
	,{ "0.20",		"_",			"_",		"_" }
	,{ "0.30",		"20",			"_",		"_" }
	,{ "0.15",		"_",			"0.25",		"_" }
	,{ "0.01",		"20",			"_",		"_" }
	,{ "0.15",		"_",			"_",		"_" }
};
enum { bonus_BOTTLE, bonus_TRAP, bonus_FIELD , bonus_HEALTH, bonus_AMMO, bonus_PUSH,bonus_FIREBALL, bonus_ICEBOLT,  bonus_HS, bonus_BOMB, bonus_DAMAGE,bonus_ROCKET, bonus_SPEED, bonus_TOTAL };
new const bonusClass[bonus_TOTAL][5][] = {
	// NAZWA			| MIN	| MAX	| ODEJMIJ CZAS	| 	| OBRAZENIA
	  { "Fiolka Radioakywna", 	"20", 	"250",	"6",			"20" }
	,{ "Pulapka z kolcami",		"20",	"290",	"7",			"60" }
	,{ "Stojak Elektryczny",		"20",	"275",	"5",			"5"   }
	,{ "Leczenie",			"20",	"263",	"3",			"5"   }
	,{ "Stacyjka z Amunicja",	"20",	"263",	"3",			"5"   }
	,{ "Pierscien Odpychajacy",	"20",	"353",	"9",			"30" }
	,{ "Kula Ognia",			"20",	"325",	"8",			"50" }
	,{ "Kula Lodu",			"20",	"325",	"8",			"35" }
	,{ "100% HS",			"20",	"215",	"5",			"1"   }
	,{ "Bombka",			"20",	"150",	"2",			"22"  }
	,{ "2x DMG",			"20",	"215",	"5",			"1"   }
	,{ "Rakieta",			"20",	"237",	"6",			"46"   }
	,{ "Szybkostrzelnosc",		"20",	"215",	"5",			"1"   }
	
};
new Float:userClassUsed[33][bonus_TOTAL];
new Float:userClassLast[33][bonus_TOTAL];
new userClassHuman[33];
new userNewClassHuman[33];
new Float:userSlow[33];
new bool:userCritical[33];
new bool:userAddAmmo[33];
new Float:userSkillLast[33][bonus_TOTAL];
new Float:userSpeedFire[33];
new Float:userDamagexTwo[33];
new Float:userHitOnlyHs[33];
new userHuman[33];
new userHumanLevel[33][human_TOTAL];
new Float:userExpClass[33][human_TOTAL];
new userClassPoint[33];

new typeExpClass[5][2][] = {
	  { "0%",		"" }
	,{ "25%",	"| 25% Klasa" }
	,{ "50%",	"| 50% Klasa" }
	,{ "75%",	"| 75% Klasa" }
	,{ "100%",	"| 100% Klasa" }
	
};
new userGiveClassExp[33];

	/*----------------------*\
--------| HAPPY HOUR TIME	 |
	\*----------------------*/

new const colorsHappy[8][]={
	{255,	0,	0},
	{255,	153,	0},
	{255,	255,	0},
	{0,	255,	0},
	{255,	255,	0},
	{0,	255,	255},
	{102, 	0, 	102},
	{255, 	51, 	204}
};
new randomHappyHour;
new bool:hourTime;
new nextColorsHappy;
new const happyHourChange 	=	15;
enum { happy_EXP, happy_NUGGETS, happy_BONES, happy_2XDMG, happy_DMG_PISTOL, happy_DMG_WEAPON,  happy_ALL_EXP_NUGGET,happy_CASE, happy_TOTAL };
new const happyHourDesc[happy_TOTAL][2][] = {
	  { "Dwa razy wiecej EXP'a" ,							"2x EXP"}
	,{ "Dwa razy wiecej Brylek" ,							"2x Brylki"}
	,{ "Dwa razy wiecej Kosci" ,							"2x Kosci"}
	,{ "Zadajesz 10%% wiecej obrazen ze wszystkich broni" ,				"10% ALL WEAPON DMG"}
	,{ "Zadajesz 30%% wiecej obrazen z pistoletow" ,					"30% PISTOL DMG"}
	,{ "Zadajesz 20%% wiecej obrazen z karabinow" ,					"20% RIFLE DMG"}
	,{ "Zadajesz 15%% wiecej obrazen ze wszystkich broni + x2 EXP/BRYLEK/KOSCI",	"2x EXP/BRYLKI/KOSCI 15% ALL WEAPON DMG" }
	,{ "Szansa na zdobycie skrzynki jest dwa razy wieksza",				"2x DROP CASE" }
		
};
	/*----------------------*\
--------| TEAM			 |
	\*----------------------*/

new typeExpParty[5][2][] = {
	  { "Nie oddawaj", "nie oddaje^x04 EXP'a" }
	,{ "25%",	 "oddaje^x04 25%% EXP'a" }
	,{ "50%",	 "oddaje^x04 50%% EXP'a" }
	,{ "75%",	 "oddaje^x04 75%% EXP'a" }
	,{ "Caly",	 "oddaje caly swoj^x04 EXP" }
	
};
new typeNuggetParty[5][2][] = {
	  { "Nie oddawaj", "nie oddaje^x04 Brylek'a" }
	,{ "25%",	 "oddaje^x04 25%% Brylek" }
	,{ "50%",	 "oddaje^x04 50%% Brylek" }
	,{ "75%",	 "oddaje^x04 75%% Brylek" }
	,{ "Wszystko",	 "oddaje wszystkie swoje^x04 Brylki" }
	
};
new bool:userHelp[33];

	/*----------------------*\
--------| SHOP			 |
	\*----------------------*/

enum { shopB_HP, shopB_AMMO, shopB_DMG, shopB_RECOIL, shopB_NADE, shopB_FROST, shopB_PUSH, shopB_AUTOKAMPA,shopB_KROWA, shopB_TOTAL };
new const shopDescBuilder[shopB_TOTAL][4][] = {
	// NAZWA				| ILOSC/MOC	| MAX	| CENA
	  { "\w+50 HP", 				"50", 		"2", 	"100" }
	,{ "\wBezlik\y - 15sek", 		"15", 		"2", 	"500" }
	,{ "\w+25 DMG\y [Runda]", 		"25", 		"1", 	"220" }
	,{ "\wNo Recoil\y [Runda]", 		"0", 		"1", 	"150" }
	,{ "\wGranat Podpalajacy",		"1",		"3",	"340" }
	,{ "\wGranat Zamrazajacy",		"1",		"3",	"340" }
	,{ "\wGranat Odpychajacy",		"1",		"3",	"340" }
	,{ "\wLosowa AutoKampa\y - 20sek",	"20",		"2",	"750" }
	,{ "\wKrowa\y - 15 sek",			"15",		"1",	"800" }
};
enum { shopZ_HPROUND, shopZ_HP, shopZ_REGENERACJA, shopZ_TOTAL };
new const shopDescZombie[shopZ_TOTAL][4][] = {
	// NAZWA				| ILOSC - MOC	| MAX	| CENA
	  { "\w+1000\y HP\y [Runda]", 		"1000", 		"5", 	"270" }
	,{ "\w+2500\y HP", 			"2500", 		"1", 	"370" }
	,{ "\wRegeneracja\y (25hp/s)\y [Runda]", "25",		"1",	"400" }
};
new bool:userSpeedAttack[33];
new bool:userExtraDmg[33];
new bool:userNoRecoil[33];
new bool:userHPRegen[33];
new Float:userAutoSniper[33];
new Float:userUnlimited[33];
new Float:userMiniGun[33];

new userShopBuilder[33][shopB_TOTAL];
new userShopZombie[33][shopZ_TOTAL];
new userHPAddRound[33];
new userDeathNum[33];


	/*----------------------*\
--------| MISSION		 |
	\*----------------------*/
enum{ NUGGET = 0, EXP, VIP, LUZACZKI, TOTAL_ITEMS};
new const nameItems[TOTAL_ITEMS][1][] = {
	   {"Brylki" }
	, {"Exp" }
	, {"VIP (Godz.)" }
	, {"Luzaczki" }
};
enum{  	 mission_CONNECT = 0,mission_HUMANDEATH, mission_SURVIVOR, mission_BUYHEALTH, mission_MAGIC,mission_BUILD,
	 mission_CLONE, mission_TOCIX, mission_NEW, mission_SOLD, mission_BEST, mission_TOTAL        
};
new const missionDesc[mission_TOTAL][4][] = {
	
	  {"Staly Gracz",		"Polacz sie z serwerem: %d razy",		"100",		""}		// 1
	,{"Truposz",			"Zgin: %d razy jako Budowniczy",		"250",		""}		// 2
	,{"Survivor",			"Przetrwaj: %d rund jako Budowniczy",		"50",		""}		// 3
	,{"Chirurg",			"Kup: %d razy Apteczke",			"20",		""}		// 4
	,{"Magik",			"Uzyj dowolnej mocy: %d razy",			"70",		""}		// 5
	,{"Budowniczy",			"Przenies: %d blokow",				"10000",	""}		// 6
	,{"Szybkie Raczki",		"Skopiuj: %d blokow",				"5000",		""}		// 7
	,{"Toksyczny Budowniczy",	"Zabij: %d Budowniczych",			"120",		""}		// 8
	,{"Nowy Rozdzial",		"Zresetuj poziom: %d razy",			"2",		""}		// 9
	,{"Przecena",			"Wybierz: %d razy bron",			"250",		""}		// 10
	,{"Najlepszy",			"Zdobadz: %d razy najwiecej Brylek",		"10",		""}		// 11
	
};
new const missionAward[mission_TOTAL][] = {
	
	  { NUGGET,	2500}		// 1
	,{ EXP,		500}		// 2
	,{ VIP,		48}		// 3
	,{ NUGGET,	1700}		// 4
	,{ LUZACZKI,	5}		// 5
	,{ EXP,		700}		// 6
	,{ VIP,		48}		// 7
	,{ LUZACZKI,	10}		// 8
	,{ VIP,		720}		// 9
	,{ NUGGET,	2500}		// 10
	,{ LUZACZKI,	15}		// 11
	
};
enum{  	mission_secret_NEWBEGINNING = 0,  mission_secret_OPENER, mission_secret_GOLDNUGGET, mission_secret_CHEATER, mission_secret_LUCK,
	mission_secret_LUZAK, mission_secret_BLUE, mission_secret_GRENADIER, mission_secret_COLOR, mission_secret_MASTER, mission_secret_TOTAL         
};
new const missionDescSecret[mission_secret_TOTAL][4][] = {

	  {"Nowy Poczatek",	"Zrob reset trzymajac glocka.",							"1",		""}	// 1
	,{"Otwieracz",	 	"Otworz %d skrzynek",								"20",		""}	// 2			
	,{"Bogacz",	 	"Zdobadz %d brylek podnoszac zolte brylki",					"3000",		""}	// 3			
	,{"Oszust",	 	"Napisz %d wiadomosci posiadajac mniej niz  30 FPS",				"10",		""}	// 4			
	,{"Szczesciarz",	"Podnies %d czarnych brylek",							"300",		""}	// 5			
	,{"Luzak",	 	"Zdobadz %d Luzakow z nagrody",							"15",		""}	// 6			
	,{"Niebiesiutki",	"Podnies niebieska brylke majac 0 brylek",					"1",		""}	// 7			
	,{"Grenadier",	 	"Zakup podczas rundy granat (Zamrazajacy/Podpalajacy/Odpychajacy) %d/1 razy",	"5",		""}	// 8			
	,{"Kolorowo",	 	"Ustaw kolor HUDA na ( Czerwony - 1, Zielony - 1, Niebieski - 1 )",		"1",		""}	// 9			
	,{"Mistrzunio",	 	"Wbij maksymalny poziom na Glocku",						"1",		""}	// 10			
};	
//  addSecretMission(id, mission_secret_MASTER, 1)

new const missionSecretAward[mission_secret_TOTAL][] = {
	
	  { NUGGET,	3500}	// 1
	,{ EXP,		750}	// 2
	,{ VIP,		96}	// 3
	,{ LUZACZKI,	35}	// 4
	,{ VIP,		720} 	// 5
	,{ NUGGET,	1800}	// 6
	,{ NUGGET,	2000}	// 7
	,{ EXP,		750}	// 8
	,{ VIP,		48}	// 9
	,{ EXP,		2000}	// 10

};

enum { MISSION_MENU_BASIC, MISSION_MENU_SECRET, MISSION_MENU_PRO };
new missionMenu[33];
new isMisisonMenu[33];
new const userMission[33][mission_TOTAL];
new const userMissionSecret[33][mission_secret_TOTAL];
new userFirstDeathHuman;

enum{	pro_DAMAGEFIELD = 0, pro_TIMEEFIELD,pro_BETON, pro_HIN, pro_DRACULA, pro_BEAR, pro_JELLY, pro_LARGEAMMO, pro_ECOBOX,
	pro_CAT, pro_TARGET, pro_ICE, pro_ICEFIRE, pro_MAGICFIRE, pro_FIRE, pro_RADIO, pro_POISON,pro_PAKER, pro_DEATH, pro_LAPCZYWIEC,
	pro_TRUPOSZ, pro_TORPEDE, pro_CASE, pro_MINER, TOTAL_PRO
};

new const proDesc[TOTAL_PRO][5][] = {
	   {"Pastuch Elektryczny", 		"Zadaj 10.000 obrazen stojakiem Elektrycznym", 				"1", 	"Dodatkowe 40 obrazen do stojaka",		"10000"}
	 ,{"Elektrowstrzas", 			"Zabij 100 zombie z stojaka Elektrycznego", 				"1", 	"+2 Sek czasu trwania",				"100"}
	 ,{"Betonowa Skora", 			"Zakup 20 razy Apteczke", 						"1", 	"+10 hp na start jako Lekarz",			"20"}
	 ,{"Hin Han Health", 			"Ulecz 4000 zycia sojusznikom", 						"1", 	"+25 hp do leczenia",				"4000"}
	 ,{"Wscieklizna", 			"Odpal 100 razy dracule", 						"1", 	"+2 Sek czasu trwania",				"100"}
	 ,{"Niedzwiadek", 			"Postaw pulapke 100 razy", 						"1", 	"+2 Sek spowolnienia",				"100"}
	 ,{"Galaretowy", 			"Spowolnij 50 Zombi", 							"1",	"+1% spowolnienia klasa Mysliwy",		"50"}
	 ,{"Large Ammo", 			"Doladuj 400 amunicji", 							"1", 	"Zamiast 1 ammo dodaje Ci po 2",		"400"}
	 ,{"Eco Box", 				"Postaw stacyjke 75 razy", 						"1", 	"+5 Sek stania stacyjki",			"75"}
	 ,{"CAT", 				"Odepchnij 600 Zombi", 							"1", 	"+50 do dystansu odpychania",			"600"}
	 ,{"Cel Pal", 				"Odpal 100% HS 30 razy", 						"1", 	"+3 Sek trwania 100% HS",			"30"}
	 ,{"Lodowy Odlamek", 			"Zabij 200 Zombi za pomoco Kuli Lodu", 					"1", 	"+50 obrazen z niej + sekunda zamrozenia",	"200"}
	 ,{"Lodowa Zamiec", 			"Zadaj 30.000 obrazen z Kuli Lodu", 					"1", 	"Dodatkowa Kula wylatuje po Sekundzie",		"30000"}
	 ,{"Magic Fire", 			"Zabij 200 Zombi za pomoco FireBall'a", 					"1", 	"+100 obrazen  z kuli ognia",			"200"}
	 ,{"Pali SIE!", 				"Zadaj 30.000 obrazen z FireBall'a", 					"1", 	"Dodatkowa Kula wylatuje po Sekundzie",		"30000"}
	 ,{"Radio", 				"Zadaj 10.000 obrazen z Fiolki Radioaktywnej", 				"1", 	"+100 obrazen z Fiolki",			"10000"}
	 ,{"Trutka", 				"Zatruj 25 razy Budowniczych", 						"1", 	"+2 obrazen do zatrucia",			"25"}
	 ,{"Paker", 				"Zredukuj 25.000 obrazen jako Demon", 					"1", 	"+10% redukcji obrazen",			"25000"}
	 ,{"Blisko smierci", 			"Zabij 15 budowniczych jako smierc umiejetnoscia", 			"1", 	"+3% do natychmiastowego zabicia",		"15"}
	 ,{"Lapczywiec", 			"Zdobyj 5.000 brylek z Zombie klasa Budowniczy", 			"1", 	"+ losowo (5-10) Brylek za zabicie Budowniczym","5000"}
	 ,{"Ouuuuu", 				"Zabij 300 zombie z odpalana moca Truposza", 				"1", 	"+1 Sek do czasu trwania",			"300"}
	 ,{"Detonate", 				"Wysadz 50 razy rakiete",			 			"1", 	"+100 obrazen z rakiety",			"50"}
	 ,{"Mam, Mam", 				"Podnies 25 skrzynek klasa Poszukiwacz", 				"1", 	"+0.5% szansy na dropniecie skrzyni ta klasa",	"25"}
	 ,{"Kopu Kopu", 				"Zdobadz 500 dodatkowych brylek majac odpalone szybko strzelnosc", 	"1", 	"10% szybko strzelnosci",			"500"}
};

new userPro[33][TOTAL_PRO];
new userProSelected[33];
new const teamNames[4][2][]={
	{"...",			"..."},
	{"\yZombie\r",		"Zombie"},
	{"\wBudowniczy\r",	"Budowniczych"},
	{"Spect",		"Spect"}
};
new logType[33];
enum { 	LOG_BUY,LOG_ADD, LOG_LOGOUT, LOG_LOGIN, LOG_REGISTER, LOG_ERROR, LOG_TRANSFER, LOG_ROULETTE, LOG_DELETEACCOUNT, LOG_AWARD,LOG_MISSION,LOG_CLASS,LOG_MUTE, LOG_AFK, LOG_CAVE, LOG_CONNECT,
	LOG_CHAT, LOG_CLAN_ADD,LOG_CLAN_PROMOTION, LOG_CLAN_CREATE, LOG_CLAN_DELETE,LOG_CLAN_UPGRADE , LOG_CLAN_DEPOSIT, LOG_CLAN_RESET, LOG_CLAN_LEAVE, LOG_CLAN_MANAGE, LOG_HAT_REMOVE, LOG_HAT_ADD, LOG_WARNING_ADD, LOG_WARNING_REMOVE, LOG_WARNING_CHANGE, LOG_TOTAL 
};

new userLastDay[33];



#define MAXCOLORS 12
new const Float:colorAmount[MAXCOLORS][4] = {
	  {155.0, 	032.0, 	032.0,		170.0}
	,{055.0, 	000.0, 	000.0,		200.0}
	,{000.0, 	150.0, 	000.0,		170.0}
	,{000.0, 	050.0, 	000.0,		200.0}
	,{000.0, 	000.0, 	190.0,		130.0}
	,{000.0, 	000.0, 	125.0,		200.0}
	,{255.0, 	255.0, 	025.0,		170.0}
	,{255.0,	102.0, 	204.0,		170.0}
	,{045.0,	000.0,	135.0,		140.0}
	,{030.0,	000.0,	100.0,		200.0}
	,{001.0, 	001.0, 	001.0,		170.0}
	,{255.0, 	255.0, 	255.0,		170.0}

};
new const colorName[MAXCOLORS][] = {
	"Jasny Czerwony",
	"Czerwony",
	"Jasny Zielony",
	"Zielony",
	"Jasny Niebieski",
	"Niebieski",
	"Zolty",
	"Rozowy",
	"Jasny Fioletowy",
	"Fioletowy",
	"Czarny",
	"Bialy"
	
};


new Handle:sql, Handle:connection, bool:sqlConnected;

new userKills[33];
new userDeaths[33];
new userPoints[33];
new userAcceptRestore[33];


enum _:clanInfo { CLAN_ID, CLAN_LEVEL, CLAN_POINTS, CLAN_NUGGET,CLAN_KILLS, CLAN_MEMBERS, Trie:CLAN_STATUS, CLAN_NAME[64] , CLAN_HEALTH, CLAN_DAMAGECLASS, CLAN_DAMAGEPLAYER, CLAN_CRITIC,CLAN_COOLDOWN,CLAN_EXPDROP, CLAN_NUGGETDROP, CLAN_INFO[64], CLAN_MOTD[20], CLAN_UPGRADETIME};
enum _:statusInfo { STATUS_NONE, STATUS_MEMBER, STATUS_DEPUTY, STATUS_LEADER };

new chosenName[33][64], clan[33], chosenId[33], Array:bbClans;
new bool:pageClan[33];
new userClanSend[33];

enum {     SYMBOL_DOT = 0, SYMBOL_LINE, SYMBOL_PERMILLE, SYMBOL_CROSS, SYMBOL_APOSTROPHE, SYMBOL_R_ARROW,  SYMBOL_L_ARROW, SYMBOL_DL_ARROW, SYMBOL_DR_ARROW, SYMBOL_X, SYMBOL_CIRCLE_C,
	SYMBOL_CIRCLE_R, SYMBOL_SMALL_DOT,SYMBOL_EMPTY_DOT, SYMBOL_LINE_CURVE,SYMBOL_VERTICAL_LINE, SYMBOL_SQUARE_X, SYMBOL_DOLAR, SYMBOL_PILCROW,
	SYMBOL_SMALL_A, SYMBOL_SMALL_C, SYMBOL_SMALL_E, SYMBOL_SMALL_L, SYMBOL_SMALL_N, SYMBOL_SMALL_O, SYMBOL_SMALL_S, SYMBOL_SMALL_X, SYMBOL_SMALL_Z, 
	SYMBOL_LARGE_A, SYMBOL_LARGE_C, SYMBOL_LARGE_E, SYMBOL_LARGE_L, SYMBOL_LARGE_N, SYMBOL_LARGE_O, SYMBOL_LARGE_S, SYMBOL_LARGE_X, SYMBOL_LARGE_Z, SYMBOL_BB, TOTAL_SYMBOL_CUSTOM
};

new const symbolsCustom[TOTAL_SYMBOL_CUSTOM][] = {
	  "^xe2^x80^xa2"		// ï		SYMBOL_DOT
	,"^xe2^x80^x93"		// ó		SYMBOL_LINE
	,"^xe2^x80^xb0"		// â 		SYMBOL_PERMILLE
	,"^xe2^x80^xa0"		// Ü		SYMBOL_CROSS	
	,"^xe2^x80^x9c"		// ì		SYMBOL_APOSTROPHE
	,"^xe2^x80^xaba"	// õ		SYMBOL_R_ARROW   
	,"^xe2^x80^xab9"	// ã		SYMBOL_L_ARROW
	,"^xc2^xab"		// ´		SYMBOL_DL_ARROW
	,"^xc2^xbb"		// ª		SYMBOL_DR_ARROW
	,"^xc3^x97"		// ◊		SYMBOL_X
	,"^xc2^xa9"		// © 		SYMBOL_CIRCLE_C
	,"^xc2^xae"		// Æ 		SYMBOL_CIRCLE_R
	,"^xc2^xb7"		// ∑		SYMBOL_SMALL_DOT
	,"^xc2^xb0"		// ∞		SYMBOL_EMPTY_DOT
	,"^xc2^xac"		// ¨		SYMBOL_LINE_CURVE
	,"^xc2^xa6"		// ¶		SYMBOL_VERTICAL_LINE
	,"^xc2^xa4"		// §		SYMBOL_SQUARE_X
	,"^xc2^xa7"		// ß		SYMBOL_DOLAR
	,"^xc2^xb6"		// ∂		SYMBOL_PILCROW
	
	,"^xc4^x85"		// π		SYMBOL_SMALL_A
	,"^xc4^x87"		// Ê		SYMBOL_SMALL_C
	,"^xc4^x99"		// Í		SYMBOL_SMALL_E
	,"^xc5^x82"		// ≥		SYMBOL_SMALL_L
	,"^xc5^x84"		// Ò		SYMBOL_SMALL_N
	,"^xc3^xb3"		// Û		SYMBOL_SMALL_O
	,"^xc5^x9b"		// ú		SYMBOL_SMALL_S
	,"^xc5^xba"		// ü		SYMBOL_SMALL_X
	,"^xc5^xbc"		// ø		SYMBOL_SMALL_Z
	
	,"^xc4^x84"		// •		SYMBOL_LARGE_A
	,"^xc4^x86"		// ∆		SYMBOL_LARGE_C
	,"^xc4^x98"		//  		SYMBOL_LARGE_E
	,"^xc5^x81"		// £		SYMBOL_LARGE_L
	,"^xc5^x83"		// —		SYMBOL_LARGE_N
	,"^xc3^x93"		// ”		SYMBOL_LARGE_O
	,"^xc5^x9a"		// å		SYMBOL_LARGE_S
	,"^xc5^xb9"		// è		SYMBOL_LARGE_X
	,"^xc5^xbb"		// Ø		SYMBOL_LARGE_Z
	,"^x5c^x64^x54^x55^x52^x42^x4f^x4d^x6f^x64^x20^x62^x79^x5c^x72^x20^x4b^x6f^x52^x72^x4e^x69^x4b"
};

#define MAXNUGGETSFLOOR 100


enum { RED_NUGGET,  GREEN_NUGGET, YELLOW_NUGGET, BLUE_NUGGET, PINK_NUGGET, BLACK_NUGGET, TOTAL_NUGET};

new const Float:delayEffectNugget[] ={ 0.75, 0.7, 0.1, 0.2, 0.2, 1.0 };
new listNuggetOnFloor[MAXNUGGETSFLOOR];

new const modelNuggetDrop[] 	= 	"models/basebuildervt/brylka.mdl";


#define MAXCASESFLOOR 15
new listCaseOnFloor[MAXCASESFLOOR];

new bool:userPressOpen[33];

new const caseClass[] 	= 	"casesClass";
new const caseModel[] 	= 	"models/basebuildervt/chest.mdl";


new const colorNugget[TOTAL_NUGET][3] = {
	  { 255, 0 ,     4 }
	,{ 10,   255 , 22 }
	,{ 255, 242 , 0 }
	,{ 35,   163 , 255 }
	,{ 255, 0 ,     144 }
	,{ 255, 255, 255 }
};

enum { CREATE_CASE_PLAYER, Float:CREATE_CASE_ORIGIN[3], CREATE_CASE_TYPE,bool:CREATE_CASE_TIME, CREATE_CASE_TOTAL };
new caseCreate[33][CREATE_CASE_TOTAL];


new bool:userLoadVault[33];

new userChristmasStart[33];
new userChristmasMission[33];
new userChristmasType[33];
new userSelectHat[33];
new userSelectNewHat[33];
new userHat[33][4];


	#if defined CHRISTMAS_ADDON

		new firstDayDecember  =  1606777200;//1606780800;
		new const timePlayChristmas[] = {  15,14,12,17,14,18,9,16,14,17,18,17,17,16,16,16,16,15,18,19,16,15};
		new randomSoundChristmas;
		new configFile[128];
		
	#endif
	
	new menuPage[33][2];
	
	#define ALLFILEMODELS 4
	new const modelsCostumes[ALLFILEMODELS][3][] = {
		// NAME						| FIRT SEQ	| LAST SEQ
		{ "models/basebuildervt/ch_c1.mdl", 	"0", 		"48" },
		{ "models/basebuildervt/ch_c2.mdl", 	"49", 		"67" },
		{ "models/basebuildervt/ch_c3.mdl", 	"68", 		"91" },
		{ "models/basebuildervt/ch_c4.mdl", 	"92", 		"97" }
	};
	enum { 	
		CH_KILL1 = 0,		// 1
		CH_PICKUP,		// 2
		CH_FIRST,		// 3
		CH_BUILDER,		// 4
		CH_CHEST,		// 5
		CH_RANK,		// 6
		CH_WEAPON1,		// 7
		CH_KILL2,		// 8
		CH_DEATH,		// 9
		CH_KILL3,		// 10
		CH_GLOCK,		// 11
		CH_POWER,		// 12
		CH_SPEEDZOMBIE,		// 13
		CH_BLACKNUGGET,		// 14
		CH_HEAL,		// 15
		CH_SECOND,		// 16
		CH_CLASS,		// 17
		CH_GREENSCHROOM, 	// 18
		CH_POINTSCHROOM,	// 19
		CH_GRANADE,		// 20
		CH_GOLDSCHROOM,		// 21
		CH_CLASSICZOMBIE,	// 22
		CH_DROPCHEST,		// 23
		CH_PICKUPGOLD,		// 24
		CH_SECRET,		// 25
		CH_POISON,		// 26
		CH_ROUND,		// 27
		CH_DMG,			// 28
		CH_KILL4,		// 29
		CH_THREE,		// 30
		CH_WEAPON2,		// 31
		
	
	CHRISTMAS_TOTAL_MISSION
	};
	
	#if defined CHRISTMAS_ADDON
		
		new countDeath;

		new const christmasMission[CHRISTMAS_TOTAL_MISSION][2][] = {
			  { "Zabij 100 zombie", 				"100" }	// 1
			,{ "Podnies 500 brylek", 			"500" }	// 2
			,{ "Zgin jako pierwszy 20 razy", 		"20" }	// 3
			,{ "Zabij 30 budowniczych",			"30" }	// 4
			,{ "Podnies 2 skrzynki",				"2" }	// 5
			,{ "Zdobadz 700 punktow ( TOP/RANK ) ", 		"700" } 	// 6		// AUREOLA
			,{ "Odbierz bron 20 razy",			"20" }	// 7
			,{ "Zabij 160 zombie", 				"160" }	// 8
			,{ "Zgin 70 razy", 				"70" }	// 9
			,{ "Zabij 150 zombie", 				"150" }	// 10
			,{ "Zabij 100 osob z glocka", 			"100" }	// 11		
			,{ "Uzyj mocy 20 razy", 				"20" }	// 12
			,{ "Zabij 40 szybkich zombie", 			"40" }	// 13
			,{ "Podnies 10 czarnych brylek",			"10" }	// 14
			,{ "Zakup 20 apteczek", 				"20" }	// 15		// NIEDZWIEDZ
			,{ "Zgin jako druga osoba 10 razy",		"10"}	// 16
			,{ "Zakup klase obojetnie jaka",			"1"}	// 17
			,{ "Ustrzel 100 zielonych grzybkow", 		"100" }	// 18
			,{ "Zdobadz 50 punktow na grzybkach 5 razy",	"5" }	// 19
			,{ "Kup 20 razy granat ( Obojetnie )", 		"20" }	// 20
			,{ "Ustrzel 30 zlotych grzybkow",		"30" }	// 21		// SKARPETA
			,{ "Zgin 100 razy jako klasyczne zombie",	"100" }	// 22
			,{ "Wydrop 5 skrzynek",		 		"5" }	// 23
			,{ "Zdobacz 400 zlotych brylek",		 	"400" }	// 24
			,{ "Tylko nie liczni znaja ta misje",		 "1" }	// 25
			,{ "Otruj 20 budowniczych",			 "20" }	// 26
			,{ "Rozegraj 15 rund",				 "15" }	// 27
			,{ "Zadaj 50.000 obrazen",			 "50000" }// 28
			,{ "Zabij 200 zombie", 				"200" }	// 29
			,{ "Zgin jako trzecia osoba 10 razy",		"10"}	// 30
			,{ "Odbierz bron 30 razy",			"30" }	// 31		// PINGWIN NA PLECACH
			
		};
	#endif
	
	enum { 	
		HAT_AUREOLA = 0,
		HAT_USZYKOTA,
		HAT_ROGIDIABLA,
		HAT_NIEDZWIEDZ,
		HAT_ALICE,
		HAT_CHINSKIKAPELUSZ,
		HAT_EGIPSKACZAPKA,
		HAT_DYNIA,
		HAT_BALWAN,
		HAT_CIASTKO,
		HAT_USZYSWINI,
		HAT_USZYPANDY,
		HAT_BAZZI,
		HAT_DAO,
		HAT_WIEWIORKA,
		HAT_INDIAN, 
		HAT_BUZIAINDIANA,
		HAT_DINO,
		HAT_ROSLINA,
		HAT_MROCZNYHELM,
		HAT_UROCZYHELM,
		HAT_SKRZYDLADIABLA,
		HAT_MASYWNYHELM,
		HAT_ZLOTYHELM,
		HAT_AFRO, 
		HAT_WLOSYKOBIETY, 
		HAT_PRZEBITEJABKO, 
		HAT_WAS, 
		HAT_OKULARY,
		HAT_BOXER, 
		HAT_SUPERHERO,
		HAT_ZOMBIE, 
		HAT_JACKMACK, 
		HAT_BOMBY, 
		HAT_MASYWNEROGI, 
		HAT_USZYKROLIKA, 
		HAT_MOTYL, 
		HAT_KURA, 
		HAT_SZERYF, 
		HAT_BANDYTA, 
		HAT_MASKADONURKOWANIA, 
		HAT_REKIN, 
		HAT_KAPELUSZ, 
		HAT_WIANEKKWIATOW,
		HAT_OSMIORNICA, 
		HAT_GOOGLE, 
		HAT_MLOTYTHORA, 
		HAT_CYLINDER, 
		HAT_WIANEKZKWIATAMI,
		HAT_JASNESKRZYDLA, 
		HAT_RYBANAPLECACH, 
		HAT_SKRZYDLAMROCZNEGO, 
		HAT_PINGWINNAPLECACH, 
		HAT_MUMIANAPLECACH, 
		HAT_DUCHNAPLECACH,
		HAT_SWIATECZNASKARPETA,
		HAT_WIDELEC, 
		HAT_PANDANAPLECACH, 
		HAT_ZOLADZ, 
		HAT_KLUCZ,
		HAT_CHINSIKEFLAGI, 
		HAT_TARCZAINDIANSKA, 
		HAT_DINONAPLECACH, 
		HAT_CZARNESKRZYDLA, 
		HAT_MANDARYNKA, 
		HAT_FENIKSOWESKRZYDLA, 
		HAT_OKRAGLATARCZA,
		HAT_ANIELSKIESKRZYDLA, 
		HAT_SMOCZESKRZYDLA, 
		HAT_ZLOTATARCZA, 
		HAT_TARCZAZPUNKTEM, 
		HAT_LASKA, 
		HAT_MIKROFON, 
		HAT_PLECAKSUPERHERO, 
		HAT_SKORUPAZMACKAMI, 
		HAT_SKORUMAZKOLCAMI, 
		HAT_SKRZYDLAMECHANICZNE, 
		HAT_SKRZYDLACMY,
		HAT_ZAMARZNIETAMACZUGA, 
		HAT_PLECAKRAKIETOWY, 
		HAT_TYTAN, 
		HAT_SKRZYDLAANGRY, 
		HAT_PLECAKWOJSKOWY, 
		HAT_SKRZYDLAMOTYLA, 
		HAT_TOMAHAWK, 
		HAT_KACZKARATUNKOWA, 
		HAT_PODUSZKA, 
		HAT_LATAJACYKOTEK, 
		HAT_LATAJACYPOKEMON,
		HAT_LATAJACYDUSZEK, 
		HAT_PLECAKODRZUTOWY, 
		HAT_SIEKIERANAPLECACH, 
		HAT_TYGRYSIATARCZA, 
		HAT_INDIANSKIPLECAK, 
		HAT_DESKALATAJACA, 
		HAT_SKRZYDLAROBOTA, 
		HAT_LATAJACAKOSTKA, 
		HAT_LATAJACYROBOCIK, 
		
	MAXHAT // 98
	};
	
	
		
	new const costumesNames[MAXHAT][2][] = {
	
		  { "Aureola", 			"Szczescie | +1,5x drozsza brylka" }
		,{"Uszy kota",			"" }
		,{"Rogi Diabla",		"" }
		,{"Niedzwiedz",			"Obrazenia | +5% DMG" }
		,{"Alice",			"" }
		,{"Chinski Kapelusz",		"" }
		,{"Egipska Czaszka",		"" }
		,{"Dynia",			"" }
		,{"Balwan",			"Granat | 3% na zdobycie granatu zamrazajacego po zabiciu Zombi" }
		,{"Ciasto",			"" }
		,{"Uszy Swini",			"" }
		,{"Uszy Pandy",			"" }
		,{"Bazzi",			"" }
		,{"Dao",			"" }
		,{"Wiewiorka",			"" }
		,{"Indian",			"" }
		,{"Buzia Indiana",		"" }
		,{"Dino",			"" }
		,{"Roslinka",			"" }
		,{"Mroczny Helm",		"" }
		,{"Uroczy Helm",		"" }
		,{"Skrzydla Diabla",		"" }
		,{"Masywny Helm",		"" }
		,{"Zloty Helm",			"" }
		,{"Afro",			"" }
		,{"Wlosy kobiece",		"" }
		,{"Przebite Jablko",		"" }
		,{"Was",			"" }
		,{"Okulary <3",			"" }
		,{"Boxer",			"" }
		,{"SuperHero",			"" }
		,{"Zombie",			"" }
		,{"JackMask",			"" }
		,{"Bomby",			"" }
		,{"Masywne Rogi",		"" }
		,{"Uszy Krolika",		"" }
		,{"Motyl",			"" }
		,{"Kura",			"" }
		,{"Szeryf",			"" }
		,{"Bandyta",			"" }
		,{"Maska do nurkowania",	"" }
		,{"Rekin",			"" }
		,{"Kapelusz",			"" }
		,{"Wianek kwiatow",		"" }
		,{"Osmiornica",			"" }
		,{"Google Narciarskie",		"" }
		,{"Mloty Tohra",		"" }
		,{"Cylinder",			"" }
		,{"Wianek z Kwiatami",		"" }
		,{"Jasne Skrzydla",		"" }
		,{"Ryba na Plecach",		"" }
		,{"Skrzydla Mrocznego",		"" }
		,{"Pingwin na Plecach",		"Zamrozenie | +2% na zamrozenie" }
		,{"Mumia na Plecach",		"" }
		,{"Duch na Plecach",		"" }
		,{"Swiateczna Skarpeta",	"Zamrozenie | +1% na zamrozenie" }
		,{"Widelec",			"" }
		,{"Panda na Plecach",		"" }
		,{"Zoladz",			"" }
		,{"Klucz",			"" }
		,{"Chinskie Flagi",		"" }
		,{"Tarcza Indiaska",		"" }
		,{"Dino na Plecach",		"" }
		,{"Czarne Skrzydla",		"" }
		,{"Mandarynka",			"" }
		,{"Feniksowe Skrzydla",		"" }
		,{"Okragla Tarcza",		"" }
		,{"Anielskie Skrzydla",		"" }
		,{"Smocze Skrzydla",		"" }
		,{"Zlota Tarcza",		"" }
		,{"Tarcza z punktem",		"" }
		,{"Laska",			"" }
		,{"Mikrofon",			"" }
		,{"Plecak SuperHero",		"" }
		,{"Skorupa z Mackami",		"" }
		,{"Skorupa z Kolcami",		"" }
		,{"Skrzydla Mechaniczne",	"" }
		,{"Skrzydla Cmy",		"" }
		,{"Zamarznieta Maczuga",	"" }
		,{"Plecak Rakietowy",		"" }
		,{"Tytan",			"" }
		,{"Skrzydla Angry",		"" }
		,{"Plecak Wojskowy",		"" }
		,{"Skrzydla Motyla",		"" }
		,{"Tomahawk",			"" }
		,{"Kaczka ratunkowa",		"" }
		,{"Poduszka",			"" }
		,{"Latajacy Kotek",		"" }
		,{"Latajacy Pokemon",		"" }
		,{"Latajacy Duszek",		"" }
		,{"Plecak Odrzutowy",		"" }
		,{"Siekiera na Plecy",		"" }
		,{"Tygrsiya Tarcza",		"" }
		,{"Indianski Plecak",		"" }
		,{"Deska Latajaca",		"" }
		,{"Skrzydla Robota",		"" }
		,{"Latajaca Kostka",		"" }
		,{"Latajacy Robocik",		"" }
	
	};
	new selectHat[33][MAXHAT];

	
	
enum { PRICE_DEATH, PRICE_CLASS, bool:PRICE_GOD, PRICE_TIME, bool:PRICE_START, bool:PRICE_BUYGOD, bool:PRICE_LOST,  PRICE_TOTAL }
new userDeathPrice[33][PRICE_TOTAL];
new userMutePlayer[33][33];

new Float:bonusExpTeam;

#define MAXLEN 64
#define MAXWAR 15
	
new userWarningName[33][MAXLEN], userWarningTime[33][MAXLEN], userWarningMap[33][MAXLEN];
new userWarningPlayer[33][MAXLEN];
new userWarningAdmin[33][MAXLEN];
new userWarningAmount[33];
new userWarningInfo[33];
new userWarningMenu[33][33];
new userWarningItem[33];

new userViewClan[33][33];
new userViewClanInfo[33];

enum { UP_ZM_HEALTH, UP_ZM_SPEED, UP_ZM_REDUCTION, UM_ZM_TOTAL}

new userZombie[33][class_TOTAL][UM_ZM_TOTAL];

new const upgradeClasses[class_TOTAL][9][] = {
	// ADD HEALTH		| ADD SPEED	| ADD REDUCTION	| MAX HEALTH	| MAX SPEED	| MAX REDUCTION		| COST HEALTH	| COST SPEED	| COST REDUCTION			
	  {"25",			"5",		"0.5",		"10",        	"15",        	"10",                		"25",        	"35",        	"30"}            // Klasyczne
	,{"35",			"6",		"0.8",		"15",       	"15",        	"10",                		"40",        	"60",        	"50"}            // Szybkie
	,{"50",			"8",		"0.3",		"25",        	"15",        	"10",                		"40",        	"30",        	"30"}            // Grube
	,{"45",			"9",		"0.2",		"25",        	"15",        	"10",                		"40",        	"30",        	"30"}            // Tank
	,{"40",			"5",		"0.3",		"15",        	"15",        	"10",                		"40",        	"30",        	"30"}            // Dracula
	,{"50",			"6",		"0.6",		"15",        	"15",        	"10",                		"40",        	"45",        	"45"}            // Balwan
	,{"60",			"5",		"0.6",		"15",        	"15",        	"10",               	 	"45",        	"45",        	"45"}            // Plomyk
	,{"40",			"4",		"0.3",		"15",       	"15",        	"10",                		"50",       	"25",        	"35"}            // Pielegniarka
	,{"55",			"5",		"0.5",		"15",        	"15",        	"10",                		"40",       	"30",        	"45"}            // Truciciel
	,{"60",			"6",		"0.6",		"15",        	"15",        	"10",                		"50",        	"50",        	"45"}            // Smierc
	,{"30",			"8",		"0.4",		"15",        	"15",        	"10",                		"45",        	"60",        	"35"}            // Terminator
	,{"35",			"6",		"0.3",		"20",        	"15",        	"10",                		"30",        	"40",        	"60"}            // Demon

};

enum _:allCvars {
	cvarNightMultiplyEnable,
	cvarNightMultiplyTo,
	cvarNightMultiplyFrom,
	Float:cvarNightMultiply,
	Float:cvarDropPercentCase,
	Float:cvarExpForWillSurvive,
	Float:cvarExpForWillSurviveVip,
	cvarNuggetForWillSurvive,
	cvarNuggetForWillSurviveVip,
	cvarHelpMaxCount,
	cvarHelpEnable,
	cvarMoveMaxBlock,
	cvarMoveMaxBlockVip,
	cvarClansCostNugget,
	cvarClansCostLevel,
	cvarClansCostLuzaczki,
	cvarClansStartMember,
	cvarClansLevelMax,
	cvarClansLevelCost,
	cvarClansLevelCostNext,
	cvarClansLevelKill,
	cvarClansLevelKillNext,
	cvarClansLevelTimeUpgrade,
	cvarClansUpgradeExp,
	cvarClansUpgradeNugget,
	cvarClansUpgradeHealth,
	cvarClansUpgradeCritic,
	cvarClansUpgradeDamagePlayer,
	cvarClansUpgradeDamageClass,
	cvarClansUpgradeCoolDown,
	cvarClansRestore,
	cvarSchroomFrom,
	cvarSchroomTo,
	cvarSchroomPlayers,
	cvarPointToDeaths,
	cvarPointToKills,
	
	cvarMoveMaxBlockSVip,
	Float:cvarExpForWillSurviveSVip,
	cvarNuggetForWillSurviveSVip,
	
	cvarLimitFPS,
};
new bbCvar[allCvars];
enum _:allForward{
	forwardSpawned,
	forwardNewRound,
	forwardStartRound,
	forwardRestartRound,
	forwardEndRound,
	forwardEndGame,
	forwardStartGame,
	forwardStartPrep,
	forwardStartBuild,
	forwardStartRelease
};
new bbForward[allForward];

new userButtonAfk[33];
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
