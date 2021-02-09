#define maxPlayers 27

#include <	amxmodx		>
#include <	amxmisc		>
#include <	cstrike		>
#include <	fun		>
#include <	fakemeta	>
#include <	fakemeta_util	>
#include <	engine		>
#include <	csx		>
#include <	fvault		>
#include <	StripWeapons	>	
#include <	http2		>
#include <	xs		>
#include <	sockets		>
#include <	tutor		>
#include <	sqlx		>
#include <	regex		>
#include <	colorchat	>
	
#if AMXX_VERSION_NUM < 183
	#include <dhudmessage>
#endif

//#define CHRISTMAS_ADDON
#define SCHROOM_ADDON

#include "TURBObasebuilder/vars.inl"
#include "TURBObasebuilder/stocks.inl"
#include "TURBObasebuilder/moveBlock.inl"
#include "TURBObasebuilder/team.inl"
#include "TURBObasebuilder/saveLoad.inl"
#include "TURBObasebuilder/adminOptions.inl"
#include "TURBObasebuilder/dropSystem.inl"
#include "TURBObasebuilder/weaponSystem.inl"
#include "TURBObasebuilder/zombieSystem.inl"
#include "TURBObasebuilder/shopSystem.inl"
#include "TURBObasebuilder/soundSystem.inl"
#include "TURBObasebuilder/missionSystem.inl"
#include "TURBObasebuilder/accountSystem.inl"
#include "TURBObasebuilder/cloneAllBlock.inl"
#include "TURBObasebuilder/happyHourTime.inl"
#include "TURBObasebuilder/sms.inl"
#include "TURBObasebuilder/day.inl"
#include "TURBObasebuilder/humanSystem.inl"
#include "TURBObasebuilder/sqlFile.inl"
#include "TURBObasebuilder/miningCave.inl"
#include "TURBObasebuilder/clans.inl"
#include "TURBObasebuilder/case.inl"
#include "TURBObasebuilder/award.inl"
#include "TURBObasebuilder/hatSystem.inl"

#if defined CHRISTMAS_ADDON
	#include "TURBObasebuilder/christmas.inl"
#endif
#if defined SCHROOM_ADDON
	#include "TURBObasebuilder/grzybki.inl"
#endif

#include "TURBObasebuilder/deathPrice.inl"
#include "TURBObasebuilder/warning.inl"

public plugin_precache(){

	
	tutorPrecache();
	
	precache_model(modelNuggetDrop);

	new entBomb = create_entity("info_bomb_target");
	entity_set_origin(entBomb, Float:{9999.0,9999.0,9999.0});
	
	team_spr 		= 	precache_model(TEAMSPRITE);
	sprite_bluez 		= 	precache_model(BLUEZSPRITE);
	spriteBeam 		= 	precache_model(LASERSPRITE);
	thunder 			= 	precache_model(THUNDER);
	sprite_explode		= 	precache_model(EXPLODE); 
	sprite_pouFire		=	precache_model(LEVELUP);
	sprite_expFireball	= 	precache_model(EXPLODEFIREBALL); 
	sprite_expIceBolt	=	precache_model(EXPLODICEBOLT);
	sprite_pouIce		=	precache_model(POUICEBOLT);
	sprite_sleepshroom	=	precache_model(SHROOMSLEEP);
	blood 			= 	precache_model("sprites/blood.spr");
	blood2	 		= 	precache_model("sprites/bloodspray.spr");
	
	precache_model(modelPoison);
	precache_model(modelField);
	precache_model(modelTrap);
	precache_model(modelAmmo);
	precache_model(modelBomb);
	precache_model(modelIce);
	precache_model(modelFire);
	precache_model(modelTrapBomb);
	precache_model(modelRocket);

	for( new i = 0;  i < sizeof(classHumanKnifeModel);  i++){
		if( strlen(classHumanKnifeModel[i][0])>0 || strlen(classHumanKnifeModel[i][1])>0){	
			precache_model(classHumanKnifeModel[i][0]);
			precache_model(classHumanKnifeModel[i][1]);
		}
	}
	
	precache_model(MODELHANDS);
	precache_model("models/rpgrocket.mdl");

	soundPrecache();
	zombiePrecache();
	casePrecache();
	costumePrecache();

	#if defined SCHROOM_ADDON
		precache_model("models/basebuildervt/grzybeku.mdl");
		precache_model("sprites/basebuildervt/digitalCounter.spr");
	#endif

	#if defined CHRISTMAS_ADDON
		christmasPrecache();
	#endif
}

public plugin_init(){	
	

	register_plugin
	(
		PLUGIN,
		VERSION,
		AUTHOR
	)
	
	register_cvar("basebuilder_version", VERSION, FCVAR_SERVER);
	
	serverOffChat 	=	false;

	freeChest 	= 	find_ent_by_tname(-1, "freechest");
	gBarrier 	= 	find_ent_by_tname(-1, "barrier" );
	
	register_clcmd("Haslo", 			"readPassword");
	register_clcmd("say", 	   		"cmdSay");
	register_clcmd("say_team",		"cmdSayClan");
	register_clcmd( "+jetpack", 		"userJetPackOn" );
	register_clcmd( "-jetpack", 		"userJetPackOff" );
	register_clcmd("chooseteam", 		"cmdChangeTeam");
	register_clcmd("jointeam", 		"cmdChangeTeam");
	register_clcmd("lastinv", 		"rotateBlock");
	register_clcmd("KodZwrotny", 		"codeCheck");
	register_clcmd("Ilosc", 			"addValue");
	register_clcmd("Ilosc_Brylek", 		"addValueTransfer");
	register_clcmd("WartoscKoloruHud",	"readColor");
	register_clcmd("chat", 			"ChatOff");
	register_clcmd("bb_offset", 		"cloneOffset");
	register_clcmd("moc", 			"bindPower");
	register_clcmd("moc2", 			"resetTime");	
	register_clcmd("PodajNazweKlanu", 	"createClanHandle");
	register_clcmd("IloscBrylekKlan", 	"depositNuggetHandle");
	register_clcmd("Ogloszenie_Klanu", 	"updateInfo");
	register_clcmd("Potwierdz_Nazwe_Klanu", 	"checkNameClan");
	register_clcmd("Kod_Motd_Klanu", 	"checkCodeMotd");
	register_clcmd("IlePlaceZaZgona", 	"priceDeath");
	register_clcmd("DoKiedyMozeZabic", 	"timeDeath");
	register_clcmd("warning", 		"writeWarning");
	register_clcmd("change_desc", 		"updateWarning");
	
	register_impulse(201, 			"impulsePush");
	register_impulse(100, 			"impulseClone");
	
	new const blockCommandAll[][] = { "drop","buy", "buyammo1", "buyammo2", "radio", "radio1", "radio2", "radio3", "votekick", "votemap", "vote" , "kick"}
	for(new i = 0; i < sizeof(blockCommandAll); i++) register_clcmd(blockCommandAll[i],  "blockCommand");
	
	register_event("DeathMsg", 		"DeathMsg", 	"ade");
	register_event("CurWeapon", 		"CurWeapon", 	"be", 	"1=1");
	register_event("ResetHUD", 		"OnResetHud", 	"b");
	register_event("WeapPickup", 		"WeapPickup", 	"b");
	
	register_logevent("round_start", 	2, 		"1=Round_Start");
	register_logevent("round_End", 		2, 		"1=Round_End");
	
	register_menu("showClanMenu", 		B1 | B2 | B3 | B4 | B5 | B6 | B7 | B8 | B9 | B0 , "showClanMenu_2");
	register_menu("mainMenuAdmin", 		B1 | B2 | B3 | B4 | B5 | B6 | B7 | B8 | B9 | B0 , "mainMenuAdmin_2");
	register_menu("miningMenu", 		B1 | B2 | B3 | B4 | B5 | B6 | B7 | B8 | B9 | B0 , "miningMenu_2");
	register_menu("globalMenu", 		B1 | B2 | B3 | B4 | B5 | B6 | B7 | B8 | B9 | B0 , "globalMenu_2");
	//register_menu("menuPerrmissions", 	B1 | B2 | B3 | B4 | B5 | B6 | B8 | B9 | B0 , 	"menuPerrmissions_2");
	register_menu("smsMainMenu",		B1 | B2 | B3 | B6| B0 ,			"smsMainMenu_2");
	register_menu("menuMissionAll",		B1 | B2 | B3 ,				"menuMissionAll_2");
	register_menu("luzaczkiFinal",		B1 | B2 | B3,				"luzaczkiFinal_2");
	register_menu("upgradeClass",		B1 | B2 ,					"upgradeClass_2");
	register_menu("resetMenu",		B1 | B2,					"resetMenu_2");
	register_menu("buyClass",		B1 | B2,					"buyClass_2");
	register_menu("selectClass", 		B1 | B2 | B0 , 				"selectClass_2");
	register_menu("upgradeZombieClass", 	B1 | B2 | B3 | B0 , 			"upgradeZombieClass_2");
	register_menu("selectWeapon", 		B1 | B2 , 				"selectWeapon_2");
	register_menu("upgradeWeapon", 		B1 | B2 | B0 , 				"upgradeWeapon_2");
	register_menu("displayBuy",		B1 | B2,					"displayBuy_2");
	register_menu("missionMenuDesc",	B1 | B2  , 				"missionMenuDesc_2")
	register_menu("viewUpgradeMine", 	B1 | B2 , 				"viewUpgradeMine_2");
	register_menu("showInfoVip", 		B1 | B0 , 				"showInfoVip_2");	
	register_menu("buyCave", 		B1 | B2, 					"buyCave_2");
	register_menu("yourBuyService",		B0,					"yourBuyService_2");
	register_menu("showVipsOnline",		B0,					"showVipsOnline_2");	
	
	register_menu("createClanMenu",		B1 | B2 | B3 | B0,				"createClanMenu_2");		
	register_menu("avansClanMenu",		B1 | B2 | B3 | B0,				"avansClanMenu_2");		
	register_menu("upgradeClanMenu", 	B1 | B2 | B3 | B4 | B5 | B6 | B7 | B8 | B9 | B0 , "upgradeClanMenu_2");	
	register_menu("clanInfoDesc", 		B7 | B8 | B9 | B0 , 			"clanInfoDesc_2");
	register_menu("clanInfoMember", 		B9 | B0 , 				"clanInfoMember_2");
	
	
	register_menu("warningInfo", 		B1 | B2 | B3 | B4 | B5 | B6 | B7 | B8 | B9 | B0 , "warningInfo_2");
	register_menu("warningAddMenu", 		B1 | B2 | B0 , 				"warningAddMenu_2");
	register_menu("warningDesc", 		B1 | B2 | B0 , 				"warningDesc_2");
	
	register_message(get_user_msgid("SendAudio"), 		"messageAudioMsg");
	register_message(get_user_msgid("TextMsg"), 		"messageTextMsg");	
	register_message(get_user_msgid("SayText"), 		"messageSayText");
	register_message(get_user_msgid("ShowMenu"), 		"messageShowMenu");
	register_message(get_user_msgid("VGUIMenu"), 		"messageVGUIMenu");
	register_message(get_user_msgid("StatusIcon"), 		"messageStatusIcon");
	register_message(get_user_msgid("HideWeapon"), 		"MSG_HideWeapon");
	register_message(get_user_msgid("ScoreAttrib"), 		"vipStatus");

	new const wpCPost[][]=	{ "weapon_shield", "weaponbox", "armoury_entity" }
	new const entFire[][] = 	{ "worldspawn", "player" }
	
	for(new i; i < sizeof(wpCPost); i++) RegisterHam(Ham_Touch, wpCPost[i],"ham_WeaponCleaner_Post", 0);
	for(new i; i < sizeof(entFire); i++) RegisterHam(Ham_TraceAttack, entFire[i], "fw_TraceAttack");
	RegisterHam(Ham_Spawn, 		"player", 		"ham_Spawn",			1);
	RegisterHam(Ham_TakeDamage, 	"player", 		"ham_TakeDamage");
	RegisterHam(Ham_Item_Deploy, 	"weapon_c4", 		"ham_ItemC4Deploy", 		1);
	RegisterHam(Ham_TakeDamage, 	"info_target",		"ham_TakeDamageEnt");
	RegisterHam(Ham_Use, 		"func_button", 		"ham_UseButtonEnt");

	
	register_forward(FM_GetGameDescription, 			"fw_GetGameDescription");
	register_forward(FM_SetClientKeyValue, 			"fw_SetClientKeyValue");
	register_forward(FM_ClientUserInfoChanged, 		"fw_ClientUserInfoChanged");
	register_forward(FM_CmdStart, 				"fw_CmdStart");
	register_forward(FM_PlayerPreThink, 			"fw_PlayerPreThink");
	register_forward(FM_EmitSound,				"fw_EmitSound");
	register_forward(FM_UpdateClientData, 			"fw_UpdateClientData",		1);
	register_forward(FM_ClientKill, 				"fw_ClientKill" );
	register_forward(FM_TraceLine, 				"fw_TraceLine", 			1);
	register_forward(FM_TraceHull, 				"fw_TraceHull", 			1);
	register_forward(FM_Touch, 				"fw_touch");
	register_forward(FM_StartFrame, 				"fw_ServerFrame");
	register_forward(FM_Voice_SetClientListening, 		"fw_SetClientListening");
	register_forward(FM_SetModel,				"fw_SetModel");
	register_forward(FM_AddToFullPack, 			"fw_addtofullpack", 		1);
	
	makeBarrierSolid();
	clonePrepare();
	happyStart();
	
	set_task(random_float(60.0, 120.0), "playerRandomInfo");
	
	bbClans 		= 	ArrayCreate(clanInfo);

	register_think("nugget", "nuggetThink");
	register_think(caseClass, "chestThink");
	register_touch("player", 	"player", 		"blockJumpOnHead");
	register_touch(rocketClass, 	"*", 			"rocketTouch");
	register_touch("player", 	"func_wall", 		"jumpBlockTouch");
	
	new globalTime[9], map[33]; 
	get_time("%H:%M:%S",globalTime,sizeof(globalTime));
	get_mapname(map, sizeof(map));

	log_amx("+------------------------------------------+");
	log_amx("Autor: %s", AUTHOR);
	log_amx("Nazwa: %s", PLUGIN);
	log_amx("Werscja: %s", VERSION);
	log_amx("Godzina: %s", globalTime);
	log_amx("Mnoznik: %s", activeMultiply ? "TAK" : "NIE");
	log_amx("HappyTime: %s", hourTime ? "TAK" : "NIE");
	if(hourTime) log_amx("HappyTime Bonus: %s", happyHourDesc[randomHappyHour][0]);
	log_amx("Mapa: %s", map);
	log_amx("Entity: %d",entity_count());
	log_amx("+------------------------------------------+");
	
	
	
	#if defined SCHROOM_ADDON
		register_clcmd("schroom", 	"menuGrzybki");
		register_think("grzybek", 	"grzybekThink");
		prepareShroom()
		prepareDigitalCounter();
	#endif
	
	#if defined CHRISTMAS_ADDON
		register_menu("infoCostumes", 		B1 | B2 , 				"infoCostumes_2");
		register_clcmd("christmas", "menuCreateChristmas");
		register_think(szChristmasTreeSprite, "treeThink");
		register_touch("player", 	szChristmasTree, 	"christmasTouchTree");
		loadChristmas();
		
	#endif
	
	new hour; time(hour);
	if(hour >= 22 || hour < 6) activeMultiply = true;
	else activeMultiply = false;
}
public ham_TakeDamageEnt(ent, idinflictor, id, Float:damage, damagebits){
	
	#if defined SCHROOM_ADDON
		if(!pev_valid(ent) || !is_user_alive(shroomPlayer)) return HAM_IGNORED;
		if(isSchroom(ent)){
			grzybekDMG(shroomPlayer, ent, 0.0)	;
		}	
		return HAM_IGNORED;
	#endif
}
public ham_UseButtonEnt(this, id, idactivator, use_type, Float:value){
	
	#if defined SCHROOM_ADDON 
	
	if(id != idactivator) return HAM_IGNORED;
	
	new szTarget[12];
	entity_get_string(this, EV_SZ_targetname, szTarget, sizeof(szTarget));
	
	if( equal(szTarget, "playshroom") ){
		new hour;
		time(hour)
		if( !has_flag(id, "a") ){
			if( hour >= 0 && hour < 7 ) {    
			ColorChat(id, GREEN, "---^x01 Grzybki poszly spac i wstana dopioro o 7^x04 ---");
			return PLUGIN_CONTINUE;
			    
			}
			if( !( prepTime ||  buildTime ) ){        
				ColorChat(id, GREEN, "---^x01 Teraz nie wolno strzelac do grzybkow^x04 ---");
				return PLUGIN_CONTINUE;
			}
			new tt = numPlayers(1, false);
			new ct = numPlayers(2, false);
			if( ct + tt < 4 ){        
				ColorChat(id, GREEN, "---^x01 Za malo graczy na grzybki^x04 ---");
				return PLUGIN_CONTINUE;
			    
			}
		}
		startShroomGame(id);            
	}
	return HAM_IGNORED;
	#endif 
}

public fw_SetModel(ent,const model[]){
	
	if(!pev_valid(ent)) return FMRES_IGNORED;
	
	#if defined CHRISTMAS_ADDON
		new szClass[12];
		entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass));
	
		if(equal(szClass, "grenade")){
			engfunc(EngFunc_SetModel, ent, modelGranade[CHRISTMAS_W]);
			return FMRES_SUPERCEDE;
		}
	#endif 
	return FMRES_IGNORED;
}
public jumpBlockTouch(id, ent){
	if( !pev_valid(ent)) return;

	new szClass[14], szTarget[7];
	entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass));
	entity_get_string(ent, EV_SZ_targetname, szTarget, sizeof(szTarget));
	
	if(equal(szClass, "func_wall") && equal(szTarget, "JUMP")){
		jumpFuncBlock(id, 2);
	} 
}
public teamCampTouching(id){
	
	if( get_user_team(id) != 2) return;
	if(!prepTime && !gameTime) return;
	if(isAdmin(id)) return;
		
	new ground = pev(id, pev_groundentity);
	
	if( ground != 0 && ( pev(id, pev_flags) & FL_ONGROUND ) ){
		if( getOwner(ground) != id && getOwner(ground) != 0 && getOwner(ground) != userTeam[id] ){				
			static Float:fTime[ 33 ];
			if( ( get_gametime() - fTime[ id ] ) >= 1.0 ){

				new Float:fPunchAngle[3];
				fPunchAngle[0] = float(random(15));
				fPunchAngle[1] = float(random(15));
				fPunchAngle[2] = float(random(15));
				
				set_pev(id, pev_punchangle, fPunchAngle);
				set_pev(id, pev_velocity, Float:{0.0,0.0,0.0});
				
				screenShake(id, 5,1 ,5);
			
				set_dhudmessage(85, 255, 170, -1.0, 0.7, 0, 0.0, 1.0, 0.01, 0.02);
				show_dhudmessage(id, "To nie twoj Blok!^nWlasciciel bloku: %s", userName[getOwner(ground)]);
				fTime[ id ] = get_gametime() ;
			}
			set_pev(id, pev_gravity, 5.0);
			if( task_exists(id+TASK_GRAVITY) )
				remove_task(id+TASK_GRAVITY);
			set_task(0.1, "removeGravity", id+TASK_GRAVITY);
			
			userDarkScreen[id]=min( 275, userDarkScreen[id]+5);
			if(userDarkScreen[id] >= 275 ){						
				ExecuteHamB(Ham_CS_RoundRespawn, id);
				set_pev(id, pev_groundentity, 0);
				ColorChat(id, GREEN, "---^x01 To nie twoja kampa^x04 ---");					
				userDarkScreen[id]-=70;
			}					
			Display_Fade(id,(1<<12),(1<<12),(1<<12),0, 0, 0, min( 255, userDarkScreen[id]));
		}
	}else userDarkScreen[id] = max( userDarkScreen[id]-2, 0);
}
public plugin_cfg(){
	cfgClan();
	plugin_init_sql();
}
public plugin_end(){ 
	if (sql != Empty_Handle) SQL_FreeHandle(sql);
	if (connection != Empty_Handle) SQL_FreeHandle(connection);
	endClan();
}
public WeapPickup(id) return PLUGIN_HANDLED;
public menuColor(id, item){
	
	if(!isVip(id)){
		ColorChat(id, GREEN, "%s Kolorowanie dostepne tylko dla^x03 VIP'a", PREFIXSAY);
		return;
	}
	new gText[512], iLen = 0;
	new size = sizeof(gText) - iLen - 1;
	iLen = format(gText[iLen], size, "\r[BaseBuilder]\y Kolorowanie Blokow^n");
	iLen = format(gText[iLen], size, "\dZakaz Kolorowania podstway");
	
	new menu = menu_create(gText, "menuColor_2");
	
	for(new i = 0; i < sizeof(colorName); i++){
		format(gText, sizeof(gText), "%s",colorName[i]);
		menu_additem(menu, gText);
	}
	menu_display(id, menu, userPageColor[id]/7);
}
public menuColor_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return PLUGIN_CONTINUE;
	}	
	userPageColor[id] = item;
	
	coloredBlock(id, item);
	menuColor(id, item);
	return PLUGIN_CONTINUE;
}
public coloredBlock(id, color){
	
	if(!isVip(id)){
		ColorChat(id, GREEN, "%s Kolorowanie dostepne tylko dla^x03 VIP'a", PREFIXSAY);
		return;
	}
	
	new ent,body;
	
	get_user_aiming(id, ent, body);
	
	if(get_user_team(id) == 1 && !isAdmin(id)){
		ColorChat(id, GREEN, "%s Nie mozesz kolorowac bedac Zombi", PREFIXSAY);
		return;
	}
	if(buildTime && !isAdmin(id)){
		ColorChat(id, GREEN, "%s Nie mozesz kolorowac podczas Budowania", PREFIXSAY);
		return;
	}
	if(getOwner(ent) == 0){
		ColorChat(id, GREEN, "%s Nie mozesz tego pokolorowac!", PREFIXSAY);
		return;	
	}
	
	if(ent && !userGrab[id]){

		if (!isAdmin(id)){
			if (id != getOwner(ent) && (userTeam[id] != getOwner(ent))){
				ColorChat(id, GREEN, "%s Nie mozesz tego pokolorowac!", PREFIXSAY);
				return;
			}
		}
		if(!canBeMoved(id, ent)){
			ColorChat(id, GREEN, "%s Nie mozesz tego pokolorowac!", PREFIXSAY);
			return;
		}
		
		new Float:gRender[3] = 0.0;
		pev(ent, pev_rendercolor, gRender);
		

		if(gRender[0] == colorAmount[color][0] && gRender[1] == colorAmount[color][1] && gRender[2] == colorAmount[color][2]){
			set_pev(ent,pev_rendermode,kRenderNormal);
			set_pev(ent,pev_rendercolor,0.0,0.0,0.0);
			set_pev(ent,pev_renderamt,255.0);
			
		} else {		
			set_pev(ent, pev_renderfx, kRenderFxPulseSlowWide);
			set_pev(ent,pev_rendermode,kRenderTransColor);
			set_pev(ent,pev_rendercolor,colorAmount[color][0], colorAmount[color][1],colorAmount[color][2]);
			set_pev(ent,pev_renderamt,colorAmount[color][3]);
		}		
	}
}
public OnResetHud(id){

	if (!is_user_alive(id)) return;

	static hideWeapon;

	if (!hideWeapon) hideWeapon = get_user_msgid("HideWeapon");

	message_begin(MSG_ONE, hideWeapon, _, id);
	write_byte(HUD_HIDE_FLASH  | HUD_HIDE_TIMER /* | HUD_HIDE_RHA*/ | HUD_HIDE_MONEY);
	message_end();
}

public fw_TraceLine(Float:start[3], Float:end[3], conditions, id, trace) {
	traceShot(id,trace);
}
public fw_TraceHull(Float:start[3], Float:end[3], conditions, hull, id, trace){
	return traceShot(id, trace);
}
public ham_ItemC4Deploy(wpn){
	static id;
	id = pev(wpn, 18);
	if (!is_user_alive(id) || !is_user_connected(id) || get_user_team(id) == 1){
		return PLUGIN_CONTINUE;
	}
	new wpnID = cs_get_weapon_id(wpn);
	if (wpnID == 6){
		strip_user_weapons(id);
		give_item(id, "weapon_knife");
	}
	return PLUGIN_CONTINUE;
}
public fw_TraceAttack(victim, attacker, Float:damage, Float:direction[3], ptr, damagebits){
	if(!is_user_connected(attacker) || is_user_bot(attacker) || is_user_hltv(attacker) || !is_user_alive(attacker)
	 || !is_user_connected(victim) || is_user_bot(victim) || is_user_hltv(victim) || !is_user_alive(victim) )
		return PLUGIN_CONTINUE;
		
	new weapon = get_user_weapon(attacker);

	if(!is_user_connected(attacker)) return HAM_IGNORED;
	if((weapon == CSW_KNIFE)) return HAM_IGNORED;

	if(userDamagexTwo[attacker] > get_gametime()){
		static Float:flEnd[3], Float:vecPlane[3];
		get_tr2(ptr, TR_vecEndPos, flEnd);
		get_tr2(ptr, TR_vecPlaneNormal, vecPlane);
		laserMaker(attacker, flEnd, thunder, 1, 1 ,1, 25, 15, 16, 16, 255, 255);
	}
	return HAM_IGNORED;
}
traceShot(id, trace){
	
	if(!is_user_alive(id ) || userClassHuman[id] != human_AIM) return FMRES_IGNORED;
	
	if(random_float(0.00, 100.00) <= str_to_float(paramClassesHuman[human_AIM][0])* userHumanLevel[id][human_AIM] || userHitOnlyHs[id] > get_gametime()){
		if( checkWeapon(get_user_weapon(id))){	
			new target = get_tr2(trace, TR_pHit);
			
			if (!is_user_alive(target)) return FMRES_IGNORED;
			
			new Float:origin[3], Float:angles[3];
			engfunc(EngFunc_GetBonePosition, target, 8, origin, angles);
			set_tr2(trace, TR_vecEndPos, origin);
			set_tr2(trace, TR_iHitgroup, HIT_HEAD);
		}
	}
	return FMRES_IGNORED;
}
public plugin_natives(){
	register_native("bb_get_class", "native_return_class", 1);
	register_native("bb_add_exp", "native_return_add_exp", 1);
}
public Float:native_return_add_exp(id, Float:value){
	addExpToFinal(id, Float:value);
}
public CurWeapon(id){
	if( !is_user_connected(id) || !is_user_alive(id) ) return;

	new wpnID = read_data(2);
	if( get_user_team(id) == 2 ){
	
		if( checkWeapon(wpnID) ) cs_set_user_bpammo(id, wpnID, 200 );
		if(wpnID == CSW_KNIFE) setModelsKnifeHuman(id);
		
	}else{		
		new class = userClass[id];
		if(wpnID == CSW_KNIFE) entity_set_string( id , EV_SZ_viewmodel , classesZombies[class][6])  ;
		entity_set_string( id , EV_SZ_weaponmodel , "" ) ;
	}
	#if defined CHRISTMAS_ADDON
		granadeSetModel(id);
	#endif
	
	setUserSpeed(id);
	
	if(userFov[id] > 90 ){
		new weapon = get_user_weapon(id);
		
		if(cs_get_user_zoom(id) == CS_SET_AUGSG552_ZOOM && ( weapon == CSW_AUG || weapon == CSW_SG552 ) ) setFov(id, 55);
		else if((cs_get_user_zoom(id) == CS_SET_FIRST_ZOOM)   && weapon == CSW_AWP ) setFov(id, 40);
		else if((cs_get_user_zoom(id) == CS_SET_SECOND_ZOOM) && weapon == CSW_AWP ) setFov(id, 10);
		else if((cs_get_user_zoom(id) == CS_SET_FIRST_ZOOM)   && ( weapon == CSW_SCOUT || weapon == CSW_G3SG1 || weapon == CSW_SG550 ) ) setFov(id, 40);
		else if((cs_get_user_zoom(id) == CS_SET_SECOND_ZOOM) && ( weapon == CSW_SCOUT || weapon == CSW_G3SG1 || weapon == CSW_SG550 ) ) setFov(id, 15);	
		else setFov(id, userFov[id]);
	}	
	
	if( checkWeapon(wpnID) ){
		if( get_gametime() < userUnlimited[id] ) fm_cs_get_weapon_ammo( get_pdata_cbase(id, 373), g_MaxClipAmmo[ wpnID ]);		
	}
}


public checkWeapon(wpnID){
	if( 	wpnID == 0  || 
		wpnID == CSW_KNIFE ||
		wpnID == CSW_C4 ||
		wpnID == CSW_HEGRENADE ||
		wpnID == CSW_SMOKEGRENADE || 
		wpnID == CSW_FLASHBANG
	) return false;
	return true;
}
public fw_GetGameDescription(){
	new gText[32];
	format(gText, sizeof(gText), "%s %s", PLUGIN, VERSION);
	forward_return(FMV_STRING, gText);
	return FMRES_SUPERCEDE;
}
public fw_UpdateClientData(id, sw, cd_handle){
	if( !is_user_alive(id) || !is_user_connected(id) ) return FMRES_IGNORED;
	
	if( userNoRecoil[id]) set_cd(cd_handle, CD_PunchAngle, {0.0,0.0,0.0});
		
	return FMRES_IGNORED;
}
public fw_ServerFrame(){
	for(new i = 1;i< maxPlayers;i++){
		if(!is_user_alive(i) || !userNoRecoil[i]) continue;
		
		if(pev(i, pev_button) & IN_ATTACK){
			set_pev(i, pev_punchangle, {0.0,0.0,0.0});
		}
	}
	return 0;
}

public isInReconnect(id){
	new auth[33];
	get_user_authid(id, auth, sizeof(auth) );
	for( new i=0;i<sizeof(reconnectTable); i ++ ){
		if( equal(reconnectTable[i], auth) ){
			if( get_gametime()-reconnectTableTime[i] < 10.0 )
				return i;
		}
	}
	return -1;
}
public addToReconnect(id, type){
	
	if( type == 0){
		new i =isInReconnect(id);
		if( i!=-1 && type == 0){
			userReconnected[id] =true;
			return 0;
		}
	}else{
		new auth[33];
		get_user_authid(id, auth, sizeof(auth) );
		for( new i = 0; i< sizeof( reconnectTable ); i ++ ){
			if( get_gametime()-reconnectTableTime[i] > 10.0 ){			
				copy( reconnectTable[ i ], sizeof( reconnectTable[ ] ), auth );
				reconnectTableTime[i]=get_gametime();
				break;
			}
		}
		
	}
	return 1;
}

public fw_PlayerPreThink(id){
	if( !is_user_alive(id) || !is_user_connected(id) || userFPS[id] >= 105) return FMRES_IGNORED;

	new Float:speedMulti=1.0;

	if(get_user_team(id) == 1){
		if( userClass[id] == class_TERMINATOR){
			speedMulti -= 0.05;
		}
	}
	if(userClassHuman[id] == human_MINER) {
		if(userSpeedFire[id] > get_gametime()){
			speedMulti -= 0.15;
			if(didPro(id, pro_MINER)) speedMulti -= 0.05;
		}
		
	}
	if( speedMulti < 1.0 ){
		new weapon = get_user_weapon(id);
		if( (checkWeapon(weapon) && get_user_team(id) == 2) || (get_user_team(id) == 1 && weapon == CSW_KNIFE) ){
			new nameWeapon[33];
			get_weaponname(get_user_weapon(id), nameWeapon, sizeof(nameWeapon));
			new weapon = fm_find_ent_by_owner(-1, nameWeapon, id);					
			if(weapon){
				set_pdata_float(weapon, 46, ( get_pdata_float(weapon, 46, 4) * speedMulti), 4);
				set_pdata_float(weapon, 47, ( get_pdata_float(weapon, 47, 4) * speedMulti), 4);
			}
		}
	}
	if(userNoRecoil[id]){
		set_pev(id, pev_punchangle, {0.0,0.0,0.0});
	}
	return FMRES_IGNORED;
}
public fw_ClientUserInfoChanged(id) { 
	static const name[] = "name" ;
	static szOldName[32], szNewName[32] ;
	pev(id, pev_netname, szOldName, sizeof(szOldName));
	
	if( szOldName[0] ) { 
		get_user_info(id, name, szNewName, sizeof(szNewName));
	
		if( !equal(szOldName, szNewName) ) { 
			set_user_info(id, name, szOldName) ;
			client_print(id, print_center, "Zakaz zmiany nicku!!");
			return FMRES_HANDLED ;
		} 
	} 
	return FMRES_IGNORED;
}
public client_putinserver(id){
	set_task(0.1, "startLogin", id+ TASK_LOGIN);
	
	if(isAdmin(id))
		userAdmin[id] = true;
	else userAdmin[id] = false;
	
	userConnected[id] = true;
	userFirstSpawn[id] = false;

	set_task(2.0,"radarOff");
	//set_task(2.0,"snowOn");
	
	return PLUGIN_CONTINUE;
}
public radarOff(id){
	cmd_execute(id, "hideradar");
	client_cmd(id, "hideradar");	
	console_cmd(id, "hideradar");
}
public snowOn(id){
	cmd_execute(id, "cl_weather 1");
	client_cmd(id, "cl_weather 1");	
	console_cmd(id, "cl_weather 1");
}
public client_disconnect(id){
	
	fVaultSave(id);	
	mysqlSave(id);

	if (get_user_team(id) == 2) removeCamp(id);
	
	leaveParty(id);
		
	remove_task(id+TASK_RESPAWN);
	remove_task(id+TASK_IDLESOUND);
	
	addToReconnect(id, 1);
	
	userHelpAdmin[id] 		= 	false;
	needHelp[id]			=	0;
	
}

public client_connect(id){
	get_user_name(id, userName[id], sizeof(userName[]));
	
		
	new bool:kick = false;
	
	for(new i = 0; i < strlen(userName[id]); i ++){
		if( userName[id][i] > 126 || userName[id][i] < 32){
			kick = true;
			break;
		}
	}
	if(kick){
		ColorChat(0, GREEN, "---^x01 Gracz^x03 %s^x01 zostal wyrzucony za niedozwolone znaki w Nicku!^x04 ---", userName[id]);
		server_cmd("kick #%d ^"Niedozwolone znaki w nicku!!^"", get_user_userid(id));
		return;
	}
	
	replace_all_string("_", userName[id], sizeof(userName[]));
		
	new gText[128], steam[35], ip[32], pw[33];
	
	get_user_ip(id, ip, sizeof(ip), 1);
	get_user_authid(id, steam, sizeof(steam));
	get_user_info(id, "_pw", pw, sizeof(pw));
	
	new day;
	date(_,_, day);
	
	addToReconnect(id, 0);
	
	copy(userPassword[id], sizeof(userPassword[]), "_");
	
	userLoadVault[id]		= 	false;
	
	userNugget[id] 			= 	0;
	userLevel[id]			=	1
	userExp[id]			=	0.0;
	userNuggetAll[id]		=	0;
	timeVip[id] 			= 	0;
	userLuzCoin[id]			= 	0;
	userAfkValue[id]		=	0.00
	userBlocksUsed[id] 		= 	0;
	userCloned[id] 			= 	0;
	userMoveAs[id] 			= 	0;
	userAdmin[id] 			= 	0;
	userNoClip[id] 			= 	0;
	userGodMod[id] 			= 	0;
	userReconnected[id] 		= 	false;
	userTimeConnectServer[id]	=	get_gametime();
	userPlayerConnected[id]		=	floatround(get_gametime());
	userAllowBuild[id] 		= 	0;
	userPush[id]			=	false;
	userBlocksUsed[id] 		= 	0;
	userUnlimited[id] 		= 	0.0;
	userExtraDmg[id] 		= 	false;
	userNoRecoil[id] 		= 	false;
	userSpeedAttack[id]		= 	false;
	userHPAddRound[id] 		= 	0;
	userHPRegen[id] 			= 	false;
	userHelp[id]			= 	false;
	userNuggetCollectedRound[id]	=	0;
	userSelectWeapon[id] 		=	0;
	userDeathNum[id]		=	0;
	userFirstSpawn[id] 		= 	false;
	userDraculaUsed[id]		= 	false;
	userWeaponBool[id] 		= 	false;
	userClassHuman[id]		=	0;
	userNewClassHuman[id]		=	0;
	userNewClass[id]		=	0;
	userClass[id]			=	0;
	userAllAward[id]		=	0;
	userAllDmg[id]			=	0;
	userFov[id]			=	90;
	userScrollExp[id]		=	playedTime(id);
	userScrollNugget[id]		=	playedTime(id);
	userBoostMine[id]		=	playedTime(id);
	userKills[id] 			=  	0;
	userDeaths[id] 			=  	0;
	userPoints[id] 			=  	0;		
	userTime[id] 			=  	0;			
	userMaxDmg[id] 			=  	0;
	userProSelected[id]		=	-1;	
	userLastDay[id] 			=	day;	
	userHud[id][PLAYER_HUD_RED]	= 	120;
	userHud[id][PLAYER_HUD_GREEN] 	= 	255;
	userHud[id][PLAYER_HUD_BLUE] 	= 	60;
	userJetpackSpeed[id]		= 	500;
	userMoverBlockColor[id]		=	0;
	userStaminaDayRefresh[id]	= 	day;			
	userTypeMine[id]		=	0;
	userMineNugget[id]		=	0;	
	userLastUpgradeTime[id]		=	0;
	unlockCave[id]			=	0;
	startUpgrade[id]		=	0;	
	userLastAwardTime[id]		=	playedTime(id);
	
	userLastStaminaTime[id]		=	playedTime(id);
	userStaminaDayRefresh[id]	= 	day;
	userMinePayGoblin[id]		=	true;
			
	userVip[id]			= 	false;
	for(new x = 0; x < sizeof(shopDescBuilder); x++) 		userShopBuilder[id][x] 	= 	0;
	for(new x = 0; x < sizeof(shopDescZombie); x++) 		userShopZombie[id][x] 	= 	0;
	for(new x = 0; x < sizeof(classesHuman); x++) 		userHumanLevel[id][x] 	= 	0;
	for(new x = 0; x < sizeof(classesHuman); x++) 		userExpClass[id][x] 	= 	0.0;
	for(new x = 0 ; x < sizeof(userHuman[]); x ++ ){
		if(str_to_num(classesHuman[human_FREE][2]) == 0 )
			userHuman[id] |= (1<<x);
	}
	for(new x = 0 ; x < sizeof(userHumanLevel[]); x ++ ){
		if(str_to_num(classesHuman[human_FREE][2]) == 0 )
			userHumanLevel[id][human_FREE] = 1;	
		userHumanLevel[id][x] = 0;		
	}
	for(new i = 0 ; i < sizeof(userMission[]); i ++ ) 		userMission[id][i] 	= 	0;
	for(new i = 0 ; i < sizeof(userMissionSecret[]); i ++ ) 	userMissionSecret[id][i]= 	0;
	for(new i = 0 ; i < sizeof(userPro[]); i ++ ) 		userPro[id][i] 		= 	0;
	for(new i = 0 ; i < sizeof(userUpgradeMine[]); i ++ ) 	userUpgradeMine[id][i] 	= 	0;
	for(new i = 0 ; i < sizeof(userDigging[]); i ++ ) 		userDigging[id][i] 	= 	0;
	for(new i = 0 ; i < sizeof(userMission[]); i ++ ) 		userMission[id][i] 	= 	0;
	for(new i = 0 ; i < sizeof(userMissionSecret[]); i ++ ) 	userMissionSecret[id][i]= 	0;
	
	userMute[id] 			= 	0;
	userMaxHelp[id]			=	maxHelpCount;
	userDayHelp[id]			=	day;
	userHelpAdmin[id] 		= 	false;
	needHelp[id]			=	0;
	clan[id] 			= 	0;
	userHudAdd[id][0] 		= 	4;
	userHudAdd[id][1]		= 	1;
	userHudAdd[id][2] 		= 	1;
	userHudAdd[id][3] 		= 	0;
	userSecretPoint[id] 		= 	0;
	userLastAwardGot[id]		=	playedTime(id);
	userLastAwardFree[id]		=	get_systime();
	userLastAwardRow[id] 		=	0;		
	needHelp[findHelp(id)]		=	-1	
	userChristmasStart[id] 		= 	0;
	userChristmasMission[id] 	= 	0;
	userChristmasType[id] 		= 	0;

	for(new i = 0 ; i < 4; i ++) userHat[id][i] 	= 	0;

	
	userSelectHat[id] 	= -1;
	userSelectNewHat[id] = -1;
	
	userCheckCamp[id] = false;
	
	resetPriceDefault(id);
	
	#if defined CHRISTMAS_ADDON
		userMenuChristmas[id] = false;
	#endif
	
	loadInt(id);
	
	logType[id] = LOG_CONNECT;
	if(logType[id] == LOG_CONNECT){
		if(is_user_connected(id) || !is_user_hltv(id)){
			format(gText, sizeof(gText), "wchodzi [steam: %s][ip: %s][pw: %s]", steam, ip, pw);
			logBB(id,gText);
		}
	}
	
	addMission(id, mission_CONNECT, 1);
}

public addFlags(id){
	if(!superAdminLocalhost) return;
	set_user_flags(id, read_flags("abcdefghijklmnouprs"));	
	set_task(60.0, "infoAddFlag", id);
}
public infoAddFlag(id){
	ColorChat(id, GREEN, "---^x01 Posiadasz Admina nadanego Automatycznie przez serwer!^x04 ---");
	set_task(60.0, "infoAddFlag", id);
}
public playedTime(id){
	return userTime[id] + ( floatround(get_gametime())-userPlayerConnected[id] );
}
#if defined CHRISTMAS_ADDON
	new countDeath;
#endif
public DeathMsg(){
	new attacker	=	read_data(1);
	new victim	=	read_data(2);
	new hs		=	read_data(3);
	
	
	if( victim == attacker  ) return PLUGIN_CONTINUE;
		
	if( attacker != victim ){
		
		userExpShow[attacker] += hs ? random_float(1.0, 2.0) : random_float(0.5, 1.0);
		
		userKills[attacker] += 1;
		userPoints[attacker] += 2;
			
		userDeaths[victim] += 1;
		userPoints[victim] += 3;
		
		if(clan[attacker]) set_clan_info(clan[attacker], CLAN_KILLS, get_clan_info(clan[attacker], CLAN_KILLS) + 1);
		if(userHelp[victim]) userHelp[victim] = false;
		
		if(get_user_team(attacker) == 1 || get_user_team(victim) == 1){
			
			deathPlayerPrice(attacker, victim);
			
			addKillBone(attacker, random_num(1,3));
			addKillBone(victim, random_num(1,3));
			
			createNugget(attacker, victim);
			if(random_float(0.0, 100.0) <= dropChest(attacker)) createCase(attacker, victim)
			
			addMission(victim, mission_HUMANDEATH, 1);
			addMission(attacker, mission_TOCIX, 1);
			
			#if defined CHRISTMAS_ADDON
		
				addChristmasMission(attacker, CH_BUILDER, 1);
				
			#endif
	
			
			if(!userFirstDeathHuman){
				
				#if defined CHRISTMAS_ADDON
					countDeath = 0;
					addChristmasMission(victim, CH_FIRST, 1);
				#endif
				
				userFirstDeathHuman = true;
			}
			
			#if defined CHRISTMAS_ADDON
				countDeath ++
				if(countDeath == 2) addChristmasMission(victim, CH_SECOND, 1);
				if(countDeath == 3) addChristmasMission(victim, CH_THREE, 1);
			#endif
		}	
		gameUserDeath(victim);
	}
	return PLUGIN_CONTINUE;
}
public globalHud(id){
	id -= TASK_GLOBAL;
	
	if( !is_user_connected(id) || is_user_bot(id) || is_user_hltv(id)){
		remove_task( id + TASK_GLOBAL );
		return PLUGIN_CONTINUE;
	}
	
	new iLen;
	new gText[512];
	
	new bonus 		= 	str_to_num(classesHuman[userClassHuman[id]][4]);
	new newAwardLeft 	= 	userLastAwardTime[id] + userAwardTime - playedTime(id);
	new newLeftExp  		=  	userScrollExp[id] - playedTime(id);
	new newLeftNugget  	=  	userScrollNugget[id] - playedTime(id);
	
	// SQL NIE POLACZYLO SIE
	if(sql == Empty_Handle){
		set_hudmessage(215, 15, 15, -1.0, 0.35, 0, 0.5, 1.0, 0.0, 1.0);
		show_hudmessage(id, "Wystapil blad przy probie nawiazania polaczenia z baza danych!");
		set_hudmessage(15, 215, 15, -1.0, 0.35, 0, 0.5, 1.0, 0.0, 1.0);
		show_hudmessage(id, "^nNapisz do %s'a jesli to widzisz!", AUTHOR);
		set_hudmessage(215, 215, 15, -1.0, 0.35, 0, 0.5, 1.0, 0.0, 1.0);
		show_hudmessage(id, "^n^nTeraz nic sie nie zapisuje!");
		set_task(1.0, "globalHud" ,id+ TASK_GLOBAL);
		return PLUGIN_CONTINUE;
	}
	
	// GLOWNY HUD
	if(userLogged[id] && !(userExtraFps[id] || userFPS[id] >= 150)){
		
		if( get_user_team(id) == 1) iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n|^t%s Zombie^t|^n", classesZombies[userClass[id]][0]) ;
		else if(get_user_team(id) == 2 && hasClassHuman(id, userClassHuman[id])) iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n|^t%s - Lv: %d - XP: %0.2f%%^t|^n", classesHuman[userClassHuman[id]][0],  userHumanLevel[id][userClassHuman[id]], (userExpClass[id][userClassHuman[id]]*100.0/needXpClass(userHumanLevel[id][userClassHuman[id]])));
		
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "|^tPostac - Lv: %d - XP: %0.2f%%^t|^n", userLevel[id], (userExp[id]*100.0/needXp(id, userLevel[id])));
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "|^tTwoje Zycie: %s^t|^n", formatNumber(get_user_health(id)));
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "|^tBrylki: %s^t|^tBilans: %s^t|^n",formatNumber(userNugget[id]), formatNumber( userNuggetCollectedRound[id]));
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "|^tKosci: %s^t|^n",formatNumber(userBone[id]));
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n");
		
		new symbolLevel = SYMBOL_LINE_CURVE;
		
		new weapon = userWeaponSelect[id] ;
		if(equal(allGuns[userWeaponSelect[id]][0], weapons[get_user_weapon(id)])){
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^t%s^tBron: %s (Lv: %d/%d)^n", symbolsCustom[symbolLevel], allGuns[userWeaponSelect[id]][1], userWeaponLevel[id][weapon], str_to_num(allGuns[userWeaponSelect[id]][4]));
			
			if( userWeaponLevel[id][weapon] <  str_to_num(allGuns[userWeaponSelect[id]][4]))
				iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^t%s^tZabici: %d %s %d^n",symbolsCustom[symbolLevel], userWeaponKill[id][weapon],symbolsCustom[SYMBOL_LINE],  needKills(id , weapon, userWeaponLevel[id][weapon])-userWeaponKill[id][weapon]);
			 else iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^t%s^tZabici: %d^n",symbolsCustom[symbolLevel], userWeaponKill[id][weapon]);
	
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^t%s^tDmg: +%0.2f%%^n^n", symbolsCustom[symbolLevel], userWeaponDamage[id][weapon]);
		}
		if( newAwardLeft > 0)	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "-^tNagroda: %d:%s%d:%s%d^n", ( newAwardLeft / HOUR ), ( newAwardLeft / MINUTE % MINUTE )<10?"0":"", ( newAwardLeft / MINUTE % MINUTE ), (newAwardLeft%MINUTE)<10?"0":"", ( newAwardLeft %MINUTE ));
		else			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "-^tNagroda: Do odebrania^n");
		if(newLeftExp > 0 ) iLen += format(gText[iLen], sizeof(gText)-iLen-1,"-^tZwoj Doswiadczenia: %d:%s%d:%s%d^n",  ( newLeftExp / HOUR ),(newLeftExp / MINUTE % MINUTE)<10?"0":"", ( newLeftExp / MINUTE % MINUTE ), (newLeftExp%MINUTE)<10?"0":"", ( newLeftExp %MINUTE ));
		if(newLeftNugget > 0 ) iLen += format(gText[iLen], sizeof(gText)-iLen-1,"-^tZwoj Szczescia: %d:%s%d:%s%d^n",  ( newLeftNugget / HOUR ),(newLeftNugget / MINUTE % MINUTE)<10?"0":"", ( newLeftNugget / MINUTE % MINUTE ), (newLeftNugget%MINUTE)<10?"0":"", ( newLeftNugget %MINUTE ));

		if(!(bonus == -1)){
			new Float:coolDown;
			coolDown = floatsub(userClassUsed[id][bonus], get_gametime());
			if(userSkillLast[id][bonus] > get_gametime() )  iLen += format(gText[iLen], sizeof(gText)-iLen-1,  "-^t%s: W trakcie^n",  bonusClass[bonus][0]);
			else if( coolDown <= 0.0 ) iLen += format(gText[iLen], sizeof(gText)-iLen-1,  "-^t%s: Do uzycia^n",  bonusClass[bonus][0]);
			else iLen += format(gText[iLen], sizeof(gText)-iLen-1,"-^t%s: %0.1f^n",  bonusClass[bonus][0], coolDown);
		}
		new Float:cdUn = floatsub(userUnlimited[id], get_gametime());
		new Float:cdSn = floatsub(userAutoSniper[id], get_gametime());
		new Float:cdMg = floatsub(userMiniGun[id], get_gametime());
		
		
		if( cdUn >= 0.0 ) iLen += format(gText[iLen], sizeof(gText)-iLen-1,"-^tBezlik: %0.1f^n",  cdUn);
		if( cdSn >= 0.0 ) iLen += format(gText[iLen], sizeof(gText)-iLen-1,"-^tAutoKampa: %0.1f^n",  cdSn);
		if( cdMg >= 0.0 ) iLen += format(gText[iLen], sizeof(gText)-iLen-1,"-^tKrowa: %0.1f^n",  cdMg);
		
		
		if( iLen > 0  ){
			set_hudmessage(userHud[id][PLAYER_HUD_RED], userHud[id][PLAYER_HUD_GREEN], userHud[id][PLAYER_HUD_BLUE], 0.0, 0.0, 0, 0.0, 0.2, 0.01, 0.02);
			show_hudmessage(id, "%s", gText);
		}
	}
	
	// LICZNIK EXPA I BRYLEK
	if(userExpShow[id] > 0.0 || userNuggetShow[id] > 0 || userBoneShow[id] > 0){
		new gText[128], iLen=0;    
		
		if(userExpShow[id]) 	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n[ +%0.2f EXP'a %s ]",userExpShow[id], (typeExpClass[userGiveClassExp[id]][1]));
		if(userNuggetShow[id]) 	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n[ +%d Brylek ]",userNuggetShow[id]);
		if(userBoneShow[id]) 	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n[ +%d Kosci ]",userBoneShow[id]);
		
		if( iLen > 0  ){
			set_hudmessage(userHud[id][PLAYER_HUD_RED], userHud[id][PLAYER_HUD_GREEN], userHud[id][PLAYER_HUD_BLUE],-1.0, -1.0, 0, 0.0, 0.2, 0.0, 0.05);
			show_hudmessage(id, "^n^n^n^n^n^n^n^n^n%s", gText);
		}
	}
	
	// WLACZONE ATRYBTUY
	iLen = 0;
	if(userNoClip[id] || userGodMod[id] || userAllowBuild[id] || userPush[id]){
		new Float:got = floatsub(userTimeAdmin[id], get_gametime());
		
		if( userNoClip[id] ) 	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "[NoClip]^n");
		if( userGodMod[id] ) 	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "[GodMod]^n");
		if( userAllowBuild[id] ) 	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "[Budowanie]^n");
		if( userPush[id]) 	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "[Odpychanie]^n");
	
		if( got >= 0.0 ) 		iLen += format(gText[iLen], sizeof(gText)-1-iLen, "[God + Odpychanie - %0.1f]^n",  got);
		
		if( iLen > 0  ){
			set_dhudmessage(userHud[id][PLAYER_HUD_RED], userHud[id][PLAYER_HUD_GREEN], userHud[id][PLAYER_HUD_BLUE], -1.0, 0.70, 0, 0.1, 0.1, 0.1, 0.1);
			show_dhudmessage(id, "%s", gText);
		}
	}
	
	// PORUSZA TOBA
	iLen = 0;
	if( userHudMoving[id] != 0 ){
		iLen += format(gText[iLen], sizeof(gText)-1-iLen, "Porusza Toba admin %s", userName[userHudMoving[id]]);
		
		if(iLen > 0 ){
			set_dhudmessage(85, 255, 170, -1.0, -1.0, 0, 0.0, 0.20, 0.01, 0.02);
			show_dhudmessage(id, "%s", gText);	
		}
	} 
	
	// KTO PROSI O POMOC
	iLen = 0;
	if(isAdmin(id) && ( prepTime || ( gameTime && gTime > gGameTime - 10 ))){
		for(new i = 0, count = 0; i < sizeof(needHelp); i ++){
		
				
			if( needHelp[i] == 0 ) break;
			if( needHelp[i] == -1 ) continue;
	
			count ++;
			if(count == 1) iLen += format(gText[iLen], sizeof(gText)-1-iLen, "Potrzebuje pomocy:^n");
			iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^t%d. %s^n", (count), userName[needHelp[i]]);
		
		}
		if(iLen > 0 ){
			set_hudmessage(255, 32, 32, 0.70, 0.15, 0, 0.0, 0.2, 0.0, 0.05);
			show_hudmessage(id, "%s", gText);
		}
	}	
	set_task(0.2, "globalHud" ,id+ TASK_GLOBAL);
	return PLUGIN_CONTINUE;
}

public hudDealDmg(id, Float:dmg, szText[]){
	userMaxDmg[id] = max(userMaxDmg[id], floatround(dmg));
	
	userHudDeal[id] = (userHudDeal[id]+1)% sizeof(hudPosHit);
	
	if( strlen(szText) > 0) set_dhudmessage(128,213,255, 0.75, hudPosHit[userHudDeal[id]][0], 0, 6.0, 0.3, 0.1, 0.1);
	else set_dhudmessage(0, 170, 255, 0.75, hudPosHit[userHudDeal[id]][0], 0, 6.0, 0.3, 0.1, 0.1);

	new ammo[15];
	new bool:isProAdd = didPro(id, pro_LARGEAMMO);
	format(ammo, sizeof(ammo), "| %d Ammo", 1 + (isProAdd ? 1 : 0 ));
	show_dhudmessage(id, "%d %s%s %s", floatround(dmg), userCritical[id] ? "| Kryt" : "", userAddAmmo[id] ? ammo : "", szText);	
	
	
}
public hudGetDmg(id, Float:dmg, szText[]){
	userHudGet[id] = (userHudGet[id]+1)% sizeof(hudPosHit);
	
	if( strlen(szText) > 0)
		set_dhudmessage(240, 119, 143, 0.9, hudPosHit[userHudGet[id]][0], 0, 6.0, 0.3, 0.1, 0.1);
	else set_dhudmessage(255, 42, 85, 0.9, hudPosHit[userHudGet[id]][0], 0, 6.0, 0.3, 0.1, 0.1);
	show_dhudmessage(id, "%d %s", floatround(dmg), szText);
	
}
public messageStatusIcon(const iMsgId, const iMsgDest, const iPlayer){
	if(is_user_alive(iPlayer) && userConnected[iPlayer]) {
		static szMsg[8];
		get_msg_arg_string(2, szMsg, 7);
		
		new const blockIcon[][] = {
			"c4",
			"escape",
			"rescue",
			"defuser",
			"buyzone",
			"vipsafety"
		}
		
		for(new i = 0; i < sizeof(blockIcon); i ++){
			if(equal(szMsg, blockIcon[i])){
				set_pdata_int(iPlayer, 235, get_pdata_int(iPlayer, 235) & ~(1<<0));
				return PLUGIN_HANDLED;
			}
		}
	}
	return PLUGIN_CONTINUE;
} 
public fw_addtofullpack( es, e, ent, host, host_flags, player, p_set ){
	if(!pev_valid(ent)) return FMRES_IGNORED;
	
	if( player ){
		if(	is_user_alive(host) &&
			//!isWithParty(ent, host)  &&
			hasOption(userSaveOption[host], save_INVIS) &&
			host != ent &&
			is_user_alive(ent) &&
			pev_valid(ent) &&
			pev_valid(host) &&
			cs_get_user_team( host ) == cs_get_user_team( ent )
		) 
			set_es(  es, ES_Origin, { 999999999.0, 999999999.0, 999999999.0 } );
			
	}
	new szClass[18]; 
	entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass));
	
	if(pev(ent, pev_iuser3) != host ){
		if(equal(szClass, "nugget")){
			set_es(  es, ES_Origin, { 999999999.0, 999999999.0, 999999999.0 } );
		}
	}
	return FMRES_IGNORED;
}

new userButtonAfk[33];

public checkCamping(id){	
	id -= TASK_AFK;
	
	if( !is_user_connected(id) || is_user_bot(id) || is_user_hltv(id) || isSuperAdmin(id)){
		if( task_exists(id+TASK_AFK) )
			remove_task(id + TASK_AFK);
		return PLUGIN_CONTINUE;
	}
		
	if(userAfkValue[id] >= 100.00 ){
		ColorChat(0, GREEN, "---^x01 Gracz^x03 %s^x01 zostal wyrzucony z powodu^x03 AFK^x04 ---", userName[id]);
		server_cmd("kick #%d ^"Zostales wyrzucony z powodu AFK!!^"", get_user_userid(id));
		new gText[128];
		logType[id] = LOG_AFK;
		if(logType[id] == LOG_AFK){
			format(gText, sizeof(gText), "zostal wyrzucony za AFK'a");
			logBB(id, gText);
		}
		return PLUGIN_CONTINUE;
	}
	new Float:fVelocity[3];
	pev(id,pev_velocity,fVelocity);
	
	static button;
	button = get_user_button(id);	
	
	if(button != userButtonAfk[id]){
		userButtonAfk[id] = button;
		if(fVelocity[0] != 0|| fVelocity[1] != 0 || fVelocity[2] != 0 )
			userAfkValue[id] -= 0.20;
	} else  userAfkValue[id] += 0.05;
	
	userAfkValue[id] = floatclamp(userAfkValue[id], 0.00, 100.00);

	set_task(0.2, "checkCamping", id + TASK_AFK);
	return PLUGIN_CONTINUE;
}

// **********************************************************************************

public ham_WeaponCleaner_Post(weapon, id){
	if (pev_valid(weapon)) remove_entity(weapon);
	return HAM_SUPERCEDE;
}
public ham_TakeDamage(victim, inflictor, attacker, Float:damage, damagebits){
	
	if( (damagebits & DMG_FALL) ) return HAM_SUPERCEDE;
	
	#if defined SCHROOM_ADDON 
		if( isSchroom(attacker) || isSchroom(victim) ) return HAM_IGNORED;
	#endif 
	
	if(!isPlayer(attacker) || !isPlayer(victim) ) return HAM_IGNORED;
	if(!is_user_alive(attacker) || !is_user_alive(victim) ) return HAM_IGNORED;
	if(!is_valid_ent(victim) || !is_user_alive(victim) || !is_user_connected(attacker)) return HAM_IGNORED;	
	if(get_user_team(attacker) == get_user_team(victim)) return HAM_IGNORED;
	//if(bb_is_in_barrier(victim)) return HAM_IGNORED
	if(buildTime || roundEnd || prepTime)return HAM_SUPERCEDE;
	if(userGodMod[attacker]) return HAM_SUPERCEDE;	
	if(victim == attacker) return HAM_SUPERCEDE;

	/*----------------------*\
	| SKILL	| CLASS	| KLAN	 |
	\*----------------------*/
	
	if(get_user_team(victim) != get_user_team(attacker)){
		class_humanTakeDamage(victim, inflictor, attacker, damage, damagebits);
		classTakeDamage(victim, inflictor, attacker, damage, damagebits);
	}
	if(userClassHuman[attacker] == human_TRUPOSZ) {
		if(userDamagexTwo[attacker] > get_gametime()){
			damage *= 2.0;
		}
		if(random_float(0.00, 100.00) <= str_to_float(paramClassesHuman[human_TRUPOSZ][2])* userHumanLevel[attacker][human_TRUPOSZ] ){
			userCritical[attacker] = true;
		} else userCritical[attacker] = false;
	}
	
	if(clan[attacker]){
		
		if(get_clan_info(clan[attacker], CLAN_DAMAGEPLAYER)){

			damage *= floatadd((float(get_clan_info(clan[attacker], CLAN_DAMAGEPLAYER) ) / 100), 1.01 );
		}
		if(get_clan_info(clan[attacker], CLAN_CRITIC)){
			if(random_float(0.00, 100.00) <= float(get_clan_info(clan[attacker], CLAN_CRITIC))){
					userCritical[attacker] = true;	
			} else userCritical[attacker] = false;
		}
	}
	
	if(userCritical[attacker]) damage *= 2.0;
	if( userDracula[victim] > get_gametime()) damage *= -1.0;
	
	/*----------------------*\
	| SHOP			 |
	\*----------------------*/
	
	if( userExtraDmg[attacker] ) damage +=25.0;
	
	/*----------------------*\
	| HAPPY HOUR TIME	 |
	\*----------------------*/
	
	
	if(hourTime){
		
		new weapon = get_user_weapon(attacker);
		
		// ALL WEAPON
		
		if(bb_happy_hour() == happy_2XDMG) damage *= 1.1;
		if(bb_happy_hour() == happy_ALL_EXP_NUGGET) damage *= 1.15;		

		// ONLY PISTOL
		
		if(bb_happy_hour() == happy_DMG_PISTOL){
			if(weapon == CSW_USP || weapon == CSW_GLOCK18 || weapon == CSW_P228 || weapon == CSW_DEAGLE || weapon == CSW_ELITE || weapon == CSW_FIVESEVEN ) damage *= 1.3;
		}
		
		// ONLY RIFLE
		
		if(bb_happy_hour() == happy_DMG_WEAPON){
			if(!(weapon == CSW_USP || weapon == CSW_GLOCK18 || weapon == CSW_P228 || weapon == CSW_DEAGLE || weapon == CSW_ELITE || weapon == CSW_FIVESEVEN || weapon == CSW_KNIFE)) damage *= 1.2;
		}
	}
	
	/*----------------------*\
	| VIP			 |
	\*----------------------*/
	
	if(isVip(attacker) && get_user_team(attacker) == 2) damage *= 1.05;
	
	/*----------------------*\
	| LV. WEAPON		|
	\*----------------------*/

	
	if( get_user_team(attacker) == 2){
		if( (damagebits & DMG_BULLET) ){	
			if(equal(allGuns[userWeaponSelect[attacker]][0], weapons[get_user_weapon(attacker)])){
				damage += damage *(userWeaponDamage[attacker][userWeaponSelect[attacker]]* 0.01);
			}
		}
	}
	
	if( get_user_team(attacker) == 2){
		if(foundedHat(attacker, HAT_NIEDZWIEDZ)) damage *= 1.05;
		if(foundedHat(attacker, HAT_SWIATECZNASKARPETA)){
			if(userClass[victim] != class_SNOWMAN){
				if(random_float(0.00, 100.00) <= 1.0){
					bb_set_in_ice(attacker, victim, 3);
				}
			}
		}
		if(foundedHat(attacker, HAT_PINGWINNAPLECACH)){
			if(random_float(0.00, 100.00) <= 2.0){
				bb_set_in_ice(attacker, victim, 3);
			}
		}		
	}

	/*----------------------*\
	| AFK			 |
	\*----------------------*/	
	
	if(userAfkValue[attacker] > 0.00 ) userAfkValue[attacker] -= 0.50;
		
	/* ---------------------*/
	
	if(playerLogged(attacker)){
		if(get_user_team(victim) != get_user_team(attacker)){
			new gText[128];
			hudDealDmg(attacker, damage, gText );
			hudGetDmg(victim, damage, gText )	;
		}
	}
	if(get_user_team(victim) != get_user_team(attacker)){
		addShotExp(attacker, damage);	
	}
		
	if( damage + 1.0 >= float(get_user_health(victim)) && get_user_team(victim) == 1 ){
		if(random(100) <= 15){
			if(get_user_team(victim) == 1){
				if( userClass[victim] == class_HEALTH ){
					new Float:fOrigin[3], Float:fOriginTarget[3];
					for( new i = 1 ; i < maxPlayers;i ++ ){
						
						if( !is_user_connected(i) || !is_user_alive(i) || get_user_team(i) != 1  || get_user_godmode(i) ||  i == attacker || i == victim) continue;
						
						pev(victim, pev_origin, fOrigin);
						pev(i, pev_origin, fOriginTarget);
						
						if( get_distance_f(fOrigin, fOriginTarget) < 150.0){
							
							set_user_health(i, get_user_health(i) + 250);
							Display_Fade(i, 2048, 2048, 2048, 255, 64, 64, 130);
							set_dhudmessage(33, 255,32, -1.0, -1.0, 0, 0.3, 0.8, 0.3);
							show_dhudmessage(i, "++ Zostales Ulczony ++");
							
							BeamLight(fOrigin, fOriginTarget, spriteBeam, 0, 0, 3, 8, 5, 255, 32, 32, 255, 255)	;
						}
					}
				}
			}
		}
		killPlayerRespawn(attacker, victim, (get_pdata_int(victim, OFFSET_LASTHITGROUP, OFFSET_LINUX_PLAYER) == HIT_HEAD) ? 1 : 0 );
		return HAM_SUPERCEDE;	
	}
	#if defined CHRISTMAS_ADDON	
		addChristmasMission(attacker, CH_DMG, floatround(damage));
	#endif
	userAllDmg[attacker] += floatround(damage);
	SetHamParamFloat(4, damage);
	
	return HAM_IGNORED;
}

public killPlayerRespawn(attacker, victim, headShot){	
	
	new szWeapon[33];
	new weapon = get_user_weapon(attacker);
	get_weaponname(weapon, szWeapon, sizeof(szWeapon));
	replace_all(szWeapon, sizeof(szWeapon), "weapon_", "");
	
	make_deathmsg(attacker, victim, headShot, szWeapon);	
	
	cs_set_user_deaths(victim, cs_get_user_deaths(victim)+1);
	set_pev(attacker, pev_frags, pev(attacker, pev_frags)+1.0);
	userDeathNum[victim] ++;
	userKills[attacker] += 1;
	userPoints[attacker] += 2;
	userDeaths[victim] += 1;
	userPoints[victim] += 3;
	
	if(foundedHat(attacker, HAT_BALWAN)){
		if(random_float(0.00, 100.00) <= 3.0){
			give_item(attacker, "weapon_smokegrenade");
		}
	}
		
	#if defined CHRISTMAS_ADDON
	
		addChristmasMission(attacker, CH_RANK, 2);
		addChristmasMission(victim,     CH_RANK, 3);
		addChristmasMission(attacker, CH_KILL1, 1);
		addChristmasMission(attacker, CH_KILL2, 1);
		addChristmasMission(attacker, CH_KILL3, 1);
		addChristmasMission(attacker, CH_KILL4, 1);
		addChristmasMission(victim,     CH_DEATH, 1);
		if(weapon == CSW_GLOCK) addChristmasMission(attacker, CH_GLOCK, 1);
		if(userClass[victim] == class_SPEED) addChristmasMission(attacker, CH_SPEEDZOMBIE, 1);
		if(userClass[victim] == class_CLASSIC) addChristmasMission(victim, CH_CLASSICZOMBIE, 1);

	#endif
		
	
	if(clan[attacker]) set_clan_info(clan[attacker], CLAN_KILLS, get_clan_info(clan[attacker], CLAN_KILLS) + 1);
	
	if(userHelp[victim]) userHelp[victim] = false;


	addLevelKills(attacker, userWeaponSelect[attacker], headShot);
	
	
	if(userDamagexTwo[attacker] > get_gametime()){
		addPro(attacker, pro_TRUPOSZ, 1);
	}

	new all = 0, ct = 0;
	for(new i = 1; i < maxPlayers; i ++){
		if(!is_user_connected(i) || is_user_hltv(i) || is_user_bot(i)) continue;
		
		all++;

		if(get_user_team(i) == 2) ct  ++;		
	}
			
	if(get_user_team(victim) == 1) addNuggetToFinal(victim, ( (all - ct) * 2 ) + userDeathNum[victim] );
	
	createNugget(attacker, victim);
	if(random_float(0.0, 100.0) <= dropChest(attacker)) createCase(attacker, victim);
	
	refreshStats(victim);
	refreshStats(attacker);
	respawnPlayerAdmin(victim);
	
	Display_Fade(victim,2048,2048,2048,0,0,0, 200);
	
}
public classTakeDamage(victim, inflictor, attacker, &Float:damage, damagebits){
	
	
	if( get_user_team(victim) == 1 && !userDraculaUsed[victim] ){
		if( userClass[victim] == class_DRACULA ){
			if( damage>=float(get_user_health(victim))){
				if(random(100) < 25){
					addPro(victim, pro_DRACULA, 1);
					set_rendering(victim, kRenderFxGlowShell, 255, 0 , 0, kRenderNormal, 15);
					
					new bool:isProTime = didPro(victim, pro_DRACULA);
					
					new Float: timeDracula = 3.0 + (isProTime ? 2.0 : 0.0);
					
					userDracula[victim] = get_gametime()+ timeDracula;
					userDraculaUsed[victim] = true;	
					emitGameSound(victim, sound_LAUGHT);
					set_task(timeDracula, "dracula_Off", victim);
				}
			}
		}
	}
	if( userClass[victim] == class_DEMON ){
		new Float:reduceDamage;
		new bool:isProDamage= didPro(victim,  pro_PAKER);
		if(isProDamage)	
			damage *= 0.75;
		else damage *= 0.85;
		
		reduceDamage = damage;
		addPro(victim, pro_PAKER, floatround(reduceDamage));
	}
	if(get_user_team(attacker) == 1){
		new bool:isProPercent = didPro(attacker,  pro_DEATH);
		if(random(100) <= 5 + ( isProPercent ? 3 : 0 )){
			if( userClass[attacker] == class_DEATH ){
				
				damage = float(get_user_health(victim))
				addPro(attacker, pro_DEATH, 1);
			}
		}
		
		if( userClass[attacker] == class_TERMINATOR) damage = 10.0;
		
	}
	
	
	if(get_user_team(attacker) == 2 && attacker != victim ){
		if(random_float(0.0, 100.0) <= 0.5 ){
			if( userClass[victim] == class_POISON){
				if(get_user_health(victim) <= 4)
					return HAM_IGNORED;
					
				#if defined CHRISTMAS_ADDON
					addChristmasMission(victim, CH_POISON, 1);
						
				#endif
	
				userPoisonKiller[attacker]	=	victim;
				userPoison[attacker]		=	5;
					
				addPro(victim, pro_POISON, 1);
					
				if( !task_exists( attacker+TASK_CLASS_POISON) ){
					set_task(1.0, "class_poison_active", attacker+TASK_CLASS_POISON);
				}		
			}
		}
	}
	return HAM_IGNORED;
}
public class_poison_active(id){		
	id -= TASK_CLASS_POISON;	
	
	new bool:isProDamage= didPro(userPoisonKiller[id],  pro_POISON);	
	new damage = 2 + (isProDamage ? 2 : 0 );
		
	if(get_user_health(id) <= damage)
		return PLUGIN_CONTINUE;

	if( userPoison[id] > 0 ){	
		
		addExpToFinal(userPoisonKiller[id],random_float(0.1, 0.5));
		ExecuteHamB( Ham_TakeDamage, id, userPoisonKiller[id], userPoisonKiller[id], float(damage), DMG_SONIC );		
		
		Display_Fade(id,1024,1024,1024,0,128,0, 200);
		set_dhudmessage(33, 255,32, -1.0, -1.0, 0, 0.3, 0.8, 0.3);
		show_dhudmessage(id, "!! Zostales Zatruty !!");
	
		userPoison[id] --;
	}
	if(userPoison[id] > 0) set_task(1.0, "class_poison_active", id+TASK_CLASS_POISON);
	
	return PLUGIN_CONTINUE;
}
public dracula_Off(id){
	set_rendering(id, kRenderFxNone, 0,0,0, kRenderNormal, 255);
}
public class_humanTakeDamage(victim, inflictor, attacker, Float:damage, damagebits){

	if(get_user_team(attacker) == 2){
		
		if(userClassHuman[attacker] == human_HEALER) {
			damage += str_to_num(paramClassesHuman[human_HEALER][3])*userHumanLevel[attacker][human_HEALER];
		}
		
		if(userClassHuman[attacker] == human_FREE) {
				
			if(random_float(0.00, 100.00) <= str_to_float(paramClassesHuman[human_FREE][0])* userHumanLevel[attacker][human_FREE] ){
				addNuggetToFinal(attacker, random(10));
			}
			
		}
		
		if(userClassHuman[attacker] == human_ELEKTRYK) {
				
			if(random_float(0.00, 100.00) <= str_to_float(paramClassesHuman[human_ELEKTRYK][0])* userHumanLevel[attacker][human_ELEKTRYK] ){
				if( is_valid_ent( victim ) && is_user_alive( victim ) ){
					
					if( get_user_team( attacker ) == get_user_team( victim ) ) return PLUGIN_HANDLED;
						
					new fOrigin[ 3 ];
					new fOriginEnd[ 3 ];
					
					get_user_origin( attacker, fOrigin);
					get_user_origin( victim, fOriginEnd);
							
					message_begin(MSG_PVS, SVC_TEMPENTITY, fOriginEnd, victim);		
					write_byte(TE_DLIGHT);	
					write_coord(fOriginEnd[0]);	
					write_coord(fOriginEnd[1]);
					write_coord(fOriginEnd[2]);
					write_byte(10);		
					write_byte(255); 
					write_byte(255); 
					write_byte(255); 					
					write_byte(150);			
					write_byte(10);			
					message_end();
						
					showBeam(fOrigin, fOriginEnd, thunder, 5, 40, 3, 255, 255, 255);
					
					emitBonusSound(victim, bonus_sound_ELECTRO);
					new damage = (str_to_num(paramClassesHuman[human_ELEKTRYK][1]) * userHumanLevel[attacker][human_ELEKTRYK]) + floatround(damageClassClan(attacker));
					ExecuteHam( Ham_TakeDamage, victim, attacker, attacker, float( damage ), DMG_ENERGYBEAM );	
				}
			} 
		}
		if(userClassHuman[attacker] == human_LAB) {
			if(random_float(0.00, 100.00) <= str_to_float(paramClassesHuman[human_LAB][0])* userHumanLevel[attacker][human_LAB] ){
				new bonus = str_to_num(classesHuman[human_LAB][4])
				new Float:coolDown;
				coolDown = floatsub(userClassUsed[attacker][bonus], get_gametime());
				if(coolDown > 0.0){
					userClassLast[attacker][bonus_BOTTLE] = 0.0;
					userClassUsed[attacker][bonus_BOTTLE] = 0.0;
				
					Display_Fade(attacker,1024,1024,1024,0,128,0, 200);
					set_dhudmessage(33, 255,32, -1.0, -1.0, 0, 0.3, 0.8, 0.3);
					show_dhudmessage(attacker, "!! Fiolka zostala uzupelniona !!");
		
					ColorChat(attacker, GREEN,"%s Fiolka zostala uzupelniona!", PREFIXSAY);
				}
			}
		}
		if(userClassHuman[attacker] == human_HUNTER) {
			new bool:isProTime = didPro(attacker, pro_JELLY);
			
			if(random_float(0.00, 100.00) <= str_to_float(paramClassesHuman[human_HUNTER][0])* userHumanLevel[attacker][human_HUNTER] + ( isProTime ? 1.0 : 0.0 )){
				userSlow[victim] = 40.0;
				addPro(attacker, pro_JELLY, 1);
				set_task(0.5, "removeSlow", victim);
			}
		}
		if(userClassHuman[attacker] == human_SHOOTER) {
			if(random_float(0.00, 100.00) <= str_to_float(paramClassesHuman[human_SHOOTER][0])* userHumanLevel[attacker][human_SHOOTER]){
				new bool:isProAdd = didPro(attacker, pro_LARGEAMMO);
				new clip, ammo;
				get_user_weapon(attacker, clip, ammo);
				fm_cs_get_weapon_ammo( get_pdata_cbase(attacker, 373) , clip+1 + (isProAdd ? 1 : 0 )  );
				addPro(attacker, pro_LARGEAMMO,1);
				userAddAmmo[attacker] = true;
					
			} else userAddAmmo[attacker] = false;	
		} 
		if(userClassHuman[attacker] == human_BULLDOZER) {
			if(random_float(0.00, 100.00) <= str_to_float(paramClassesHuman[human_BULLDOZER][0])* userHumanLevel[attacker][human_BULLDOZER]){
				new iOrigin[3], Float:fOrigin[3], Float:fOriginTarget[3];
			
				get_user_origin(attacker, iOrigin, 2);
				IVecFVec(iOrigin, fOrigin);
				
				new Float:fVelocity[3]
				entity_get_vector(attacker, 	EV_VEC_origin, 	fOrigin);
			
				entity_get_vector(victim, 	EV_VEC_origin, 	fOriginTarget);
				xs_vec_sub(fOrigin, fOriginTarget, fVelocity);
				xs_vec_normalize( fVelocity , fVelocity );	
					
				new bool:isProPush = didPro(attacker, pro_CAT);	
				new powerPush =  str_to_num(paramClassesHuman[human_BULLDOZER][2]) * userHumanLevel[attacker][human_BULLDOZER] + ( isProPush ? 50 : 0 );
					
				xs_vec_mul_scalar( fVelocity , -float(powerPush) , fVelocity );
				fVelocity[2] *= 1.5;
				entity_set_vector(victim, 	EV_VEC_velocity, 	fVelocity);
			}
		}
		if(userClassHuman[attacker] == human_MAG) {
			if(userClass[victim] != class_DEVIL){
				if(random_float(0.00, 100.00) <= str_to_float(paramClassesHuman[human_MAG][0])* userHumanLevel[attacker][human_MAG]){
					bb_set_in_fire(attacker, victim, 3);
				}
			}
		}
		if(userClassHuman[attacker] == human_ICEMAG) {
			if(userClass[victim] != class_SNOWMAN){
				if(random_float(0.00, 100.00) <= str_to_float(paramClassesHuman[human_ICEMAG][0])* userHumanLevel[attacker][human_ICEMAG]){
					bb_set_in_ice(attacker, victim, 3);
				}
			}
		}
		if(userClassHuman[attacker] == human_TRUPOSZ) {			
			if(random_float(0.00, 100.00) <= str_to_float(paramClassesHuman[human_TRUPOSZ][0])* userHumanLevel[attacker][human_TRUPOSZ]){
				new closest = returnClosestOne(victim, 500, 1);
				if (closest != -1)
					ExecuteHamB( Ham_TakeDamage, closest, attacker, attacker, damage * 0.5, DMG_SONIC );
			}	
		}		
		if(userClassHuman[attacker] == human_BOOMBERMA) {		
			if( damage > pev(victim, pev_health)){
				
				if(random_float(0.00, 100.00) <= str_to_float(paramClassesHuman[human_BOOMBERMA][0])* userHumanLevel[attacker][human_BOOMBERMA]){
					new Float:fOrigin[3];
					entity_get_vector(victim, EV_VEC_origin,	fOrigin);
					bombTrapCreate(attacker, fOrigin);
				}
			}		
		}
		if(userClassHuman[attacker] == human_MINER) {
			if(random_float(0.00, 100.00) <= str_to_float(paramClassesHuman[human_MINER][0])* userHumanLevel[attacker][human_MINER]){
				createNugget(attacker, victim);
				
				if(userSpeedFire[attacker] > get_gametime()) addPro(attacker, pro_MINER, 1);
			}
		}
		
	}
	return HAM_IGNORED;
}
public round_start(){
	gameStart();	
	resetBlocks();
	removeSkill();	
}
public removeSkill(){
	new ent = -1;

	while ((ent = find_ent_by_class(ent, classPoison))){
		if (pev_valid(ent)) remove_entity(ent);
	}
	while ((ent = find_ent_by_class(ent, classField))){
		if (pev_valid(ent)) remove_entity(ent);	
	}
	while ((ent = find_ent_by_class(ent, classTrap))){
		if (pev_valid(ent)) remove_entity(ent);	
	}
	while ((ent = find_ent_by_class(ent, classAmmo))){
		if (pev_valid(ent)) remove_entity(ent);
	}
	while ((ent = find_ent_by_class(ent, classFireBall))){
		if (pev_valid(ent)) remove_entity(ent);
	}
	while ((ent = find_ent_by_class(ent, classIceBolt))){
		if (pev_valid(ent)) remove_entity(ent);
	}
	while ((ent = find_ent_by_class(ent, classbombTrap))){
		if (pev_valid(ent)) remove_entity(ent);
	}
	while ((ent = find_ent_by_class(ent, classBomb))){
		if (pev_valid(ent)) remove_entity(ent);
	}
	while ((ent = find_ent_by_class(ent, rocketClass))){
		if (pev_valid(ent)) remove_entity(ent);
	}
}

public gameStart(){
	
	roundEnd 	= false;
	gameTime 	= false;
	buildTime 	= false;
	prepTime	= false;
	
	clearQue();
	
	bonusExpTeam = random_float(1.00,6.00);
	
	for( new i = 1 ; i < maxPlayers ; i ++ ){		
		if( !is_user_alive(i) || !is_user_connected(i) || is_user_hltv(i)) continue;
		
		userRoundTeam[i]=get_user_team(i);
		userCloned[i] = 0;
		userMoveAs[i] = i;
		userNoClip[i] = 0;
		userGodMod[i] = 0;
		userAllowBuild[i] = 0;
		userBlocksUsed[i] = 0;
		userUnlimited[i] = 0.0;
		
		for(new x = 0; x < sizeof(shopDescBuilder); x++) userShopBuilder[i][x] = 0;
		for(new x = 0; x < sizeof(shopDescZombie); x++) userShopZombie[i][x] = 0;
		
		userExtraDmg[i] 	= false;
		userNoRecoil[i] 	= false;
		userSpeedAttack[i]= false;
		userHPAddRound[i] = 0;
		userHPRegen[i] = false;
		userHelp[i]	= false;
		userNuggetCollectedRound[i] = 	0;
		userSelectWeapon[i]	= 	0;
		userDeathNum[i]	= 0;
		userDraculaUsed[i]	= 	false;
		userWeaponBool[i] 	= 	false;
		userFirstDeathHuman	=	false;
	
		granadeMission[i][0] = false;
		granadeMission[i][1] = false;
		granadeMission[i][2] = false;
		granadeMission[i][3] = false;

		userHelp[i]		=	false;
		userHelpAdmin[i] 	= 	false;
		needHelp[findHelp(i)]	=	-1;
		
	}
	
	new ct = numPlayers(2, false);
	new tt = numPlayers(1, false);
	
	if( ct > 0 && tt > 0 ){
		roundGood=true;		
		startNewRound();
	} else roundGood=false;
}
public startNewRound(){	
	
	if( task_exists(TASK_CLOCK) )
		remove_task(TASK_CLOCK);
		
	new ct=numPlayers(2, false);
	new tt=numPlayers(1, false);
	
	resetBlocks();
	
	if( ct <= 0 || tt <= 0 || (ct + tt ) <= 1 ) return 0;
	
	startBuild();
	timerStart();
		
	freeChestCreate()
	
	for(new i = 1; i < maxPlayers; i ++){
		
		if( !is_user_connected(i) || is_user_hltv(i) || is_user_bot(i)) continue;
			
		#if defined CHRISTMAS_ADDON
			addChristmasMission(i, CH_ROUND, 1);
		#endif
		userCheckCamp[i] = false;	
	}
	#if defined CHRISTMAS_ADDON
		randomSoundChristmas = random(sizeof(timePlayChristmas));
	#endif
	return 1;
}
public startBuild(){
	buildTime=true;
	prepTime=false;	
	gameTime=false;
	
	spkGameSound(0, sound_BUILD);
	makeBarrierSolid();
	
	gTime = gBuildTime;
}
public happyColorBarrier(){
	
	new Float:color[3];	
	color[0] = hourTime ? float(colorsHappy[nextColorsHappy][0]) : float(255);
	color[1] = hourTime ? float(colorsHappy[nextColorsHappy][1]) : float(102);
	color[2] = hourTime ? float(colorsHappy[nextColorsHappy][2]) : float(0);
	
	set_pev(gBarrier,pev_rendermode,kRenderTransColor);	
	set_pev(gBarrier,pev_rendercolor,color);
	set_pev(gBarrier,pev_renderamt, Float:{100.0} );

	return PLUGIN_HANDLED;
}
public timerStart(){
	
	if (gTime > 0 && !clockStop) gTime --;
	if(roundEnd) return PLUGIN_CONTINUE;
	
	new number[5];
	
	#if defined CHRISTMAS_ADDON
		soundPrep();
		if ( ( gTime <= 10 && gTime > 0 || gTime == 30 ) && !gameTime && !clockStop){
			num_to_word(gTime, number, sizeof(number));
			cmd_execute(0, "spk %s", number);
		}
	#else 
		if ( ( gTime <= 10 && gTime > 0 || gTime == 30 ) && !gameTime && !clockStop){
			num_to_word(gTime, number, sizeof(number));
			cmd_execute(0, "spk %s", number);
		}
	#endif
	
	new gText[132], gRound[132], iLen;
	new happy[132];
	
	if(hourTime)  format(happy, sizeof(happy), "[ HAPPY HOUR TIME - /happy ]^n", happyHourDesc[randomHappyHour][0]);
	
	if(clockStop) format(gRound, sizeof(gRound), "Zatrzymane");
	else  {	
		if (buildTime){
			if (!gTime) startPrep();
			format(gRound, sizeof(gRound), "Budowanie");
		}	
		if(prepTime){
			if (!gTime){
				Release_Zombies();
				startRelease();	
			} 
			format(gRound, sizeof(gRound), "Przygotowanie");
		}
		if(gameTime) format(gRound, sizeof(gRound), "Runda");
	}
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "%s[ %s - %d:%s%d ]", happy, gRound, gTime/60, (gTime%60 < 10 ? "0" : ""), gTime%60);
	
	nextColorsHappy = (nextColorsHappy+1)%sizeof(colorsHappy)	
	set_dhudmessage(hourTime ? colorsHappy[nextColorsHappy][0] : 255, hourTime ? colorsHappy[nextColorsHappy][1] : 102, hourTime ? colorsHappy[nextColorsHappy][2] : 0, -1.0, 0.0, 0, 0.5, 0.9, 0.5, 0.5);
	show_dhudmessage(0, "%s",gText);
	
	if(hourTime){
		if(buildTime || prepTime) happyColorBarrier();
	}
	set_task(1.0, "timerStart", TASK_CLOCK);
	return PLUGIN_CONTINUE;
}
public startRelease(){
	gTime = gGameTime;
	gameTime = true;
	buildTime = false;
	prepTime = false;
	
	removeNotUsedBlock();

}
public startPrep(){
	
	#if defined CHRISTMAS_ADDON
		randomSoundChristmas = random(sizeof(timePlayChristmas));
	#endif

	spkGameSound(0, sound_PREP);
	
	for(new i = 1; i < maxPlayers; i ++){
		if( get_user_team(i) == 2) menuWeapon(i);
		userBlocksUsed[i] = 0;	
	}
	
	respawnAll();	
	removeNotUsedBlock();
	gTime = gPrepTime;
	buildTime = false;
	prepTime = true;
	gameTime = false;
	return PLUGIN_CONTINUE;
}
public respawnAll(){
	for( new i = 1; i<maxPlayers; i++ ){
		if( !is_user_connected(i) || get_user_team(i) != 2) continue;
		stopEnt(i);
		ExecuteHamB(Ham_CS_RoundRespawn, i);
		set_pev(i, pev_velocity, Float:{0.0,0.0,0.0});
	}
}
public round_End(){
	gameEnd();
}
public gameEnd(){
	
	roundEnd = true;
	gTime = 0;
	
	new ct = numPlayers(2, false);
	new ctAlive = numPlayers(2, true);
	new tt = numPlayers(1, false);

	for(new i = 1; i < maxPlayers; i ++){
		if( !is_user_connected(i) || is_user_hltv(i) || is_user_bot(i)) continue;
			
		deathPlayerWin(i);
		resetPriceDefault(i);
	}
	
	if( roundGood ){
		if( ctAlive > 0 ){
			set_dhudmessage(16, 16, 255, -1.0, 0.20, 0, 0.5, 3.0, 0.5, 0.5)
			show_dhudmessage(0,  "^n!! BUDOWNICZY WYGRALI !!");
			spkGameSound(0, sound_WINCT)
			
			for(new i = i; i < maxPlayers; i ++){
				if(get_user_team(i) == 2){
					
					addMission(i, mission_SURVIVOR, 1);
						
					new Float:randomExp = 0.0;
					new randomNugget	= 0;
						
					if(isVip(i)){
						randomExp = random_float(gExpForWillSurviveVip/2, gExpForWillSurviveVip);
						randomNugget = random_num(gNuggetWillSurviveVip/2, gNuggetWillSurviveVip);
					} else {
						randomExp =random_float(gExpForWillSurvive/2, gExpForWillSurvive)
						randomNugget = random_num(gNuggetWillSurvive/2, gNuggetWillSurvive)
					}
						
					addExpToFinal(i, randomExp);
					addNuggetToFinal(i, randomNugget);
						
					ColorChat(i, GREEN, "%s Otrzymales za przetrwanie rundy^x04 |^x01 Brylek:^x03 %d^x04 /^x01 Expa:^x03 %0.1f", PREFIXSAY, randomNugget, randomExp);
				}
				
			}
		}else {
			set_dhudmessage(255, 16, 16, -1.0, 0.20, 0, 0.5, 3.0, 0.5, 0.5);
			show_dhudmessage(0, "^n!! ZOMBI WYGRALO !!");
			spkGameSound(0, sound_WINTT);
		}	
		if( ct > 0 || tt > 0 ){
			
			new idMax		= 0;
			new iCoinCollected	= 0;
			for( new i = 1; i < maxPlayers; i++ ){
				if( !is_user_connected(i) ) continue;
				if( iCoinCollected < userNuggetCollectedRound[i] ){
					idMax = i;
					iCoinCollected=userNuggetCollectedRound[i];
				}
			}
			if( iCoinCollected > 0 ){
				addMission(idMax, mission_BEST, 1);
				ColorChat(0, GREEN, "---^x01 Gracz^x03 %s^x01 zdobyl najwiecej^x03 brylek^x01 w tej rundzie!^x03 [^x04 %d^x03 ]^x04 ---", userName[idMax], iCoinCollected);
			}
			for( new i = 1; i < maxPlayers; i++ ){
				if( !is_user_connected(i) || is_user_hltv(i)) continue;
				cs_set_user_team(i, userRoundTeam[i] == 1 ? 2 : 1 );
			}
			tt = numPlayers(1, false);
			ct = numPlayers(2, false);
			new dif = abs(ct-tt);
			if( dif > 1 ){			
				new lookFor = ct>tt ? 2 : 1
				new iTabPlayers[33], iPlayers=0;	
				
				for( new i = 1; i < maxPlayers; i++ ){
					if( !is_user_connected(i) || is_user_hltv(i))  continue;
					if( get_user_team(i) != lookFor ) continue;
					iTabPlayers[iPlayers++] = i;					
				}
				new idSave=0,contentSave=0;
				for( new i = 0; i < iPlayers ; i ++ ){
					idSave = random(iPlayers);
					contentSave = iTabPlayers[idSave] ;
					iTabPlayers[idSave] =  iTabPlayers[i];
					iTabPlayers[i] = contentSave;
				}
				new target = 0 ;
				for( new i = 0; i < dif - 1; i ++ ){
					target = iTabPlayers[i];				
					cs_set_user_team(target, lookFor == 2 ? 1 : 2 );
				}
			}
		}
	}
}
public makeBarrierSolid(){
	if (gBarrier){
		set_pev(gBarrier,pev_solid,SOLID_BSP);
		set_pev(gBarrier,pev_rendermode,kRenderTransColor);
		set_pev(gBarrier,pev_rendercolor, Float:{255.0, 255.0, 64.0} );	
		set_pev(gBarrier,pev_renderamt, Float:{100.0} );
	}
}
public makeBarrierNoSolid(){
	if (gBarrier){
		set_pev(gBarrier,pev_solid,SOLID_NOT);
		set_pev(gBarrier,pev_rendercolor, Float:{64.0, 255.0, 64.0} );
		set_pev(gBarrier,pev_renderamt, Float:{20.0} );
	}
}
public gameUserDeath(id){
	if( !roundEnd ){
		
		if( task_exists(id+TASK_RESPAWN) ) remove_task(id+TASK_RESPAWN);
			
		if( get_user_team( id ) == 2 ){		
			if( buildTime || prepTime )
				set_task(1.5, "respawnPlayer", id+TASK_RESPAWN);
			else{
				removeCamp(id);
				new ct = numPlayers(2, true);
				if( ct>0 ){
					if( task_exists(id+TASK_RESPAWN) )
						remove_task(id+TASK_RESPAWN);
					set_task(1.5, "respawnPlayerAsTT", id+TASK_RESPAWN);
				}
			}	
		}else{			
			if( task_exists(id+TASK_RESPAWN) )
				remove_task(id+TASK_RESPAWN);
			set_task(1.5, "respawnPlayer", id+TASK_RESPAWN);
		}
	}
}
public gameUserSpawn(id){
	if( prepTime || buildTime ) userRoundTeam[id] = get_user_team(id);
	if( task_exists(id+TASK_RESPAWN) ) remove_task(id+TASK_RESPAWN);
		
	StripWeapons(id, Primary);
	StripWeapons(id, Secondary);
	StripWeapons(id, Grenades);
}
public fw_ClientKill(id){
	return FMRES_SUPERCEDE;	
}
public respawnPlayerAsTT(id){
	id -= TASK_RESPAWN
	if( is_user_hltv(id) ||  get_user_team(id) > 2  || !is_user_connected(id)) return;

	if( !is_user_alive(id) && is_user_connected(id) ){		
		cs_set_user_team(id, 1);
		ExecuteHamB(Ham_CS_RoundRespawn, id);
	}
}
public respawnPlayer(id){	
	id -= TASK_RESPAWN;
	if( get_user_team(id) > 2 || !is_user_connected(id)) return;
	if( !is_user_alive(id) && is_user_connected(id) ){		
		ExecuteHamB(Ham_CS_RoundRespawn, id);	
	}
}
public numPlayers(team, bool:alive ){
	new iNum=0
	for(new i=1;i<maxPlayers;i++){
		if( !is_user_connected(i) || is_user_hltv(i) ) continue;
		if( alive ){
			if( !is_user_alive(i)) continue;
		}
		if( get_user_team(i) != team ) continue;
		iNum++;
	}
	return iNum;
}
public removeCamp(id){
	new target = userTeam[id];
	if( target == 0 ){
		adminResetBlock(2, id, id);	
	}else{
		if( get_user_team(target) == 2 && is_user_alive(target) ){
			reWriteBlocksParty(id, target);
		}else {
			adminResetBlock(2, id, id);			
		}
	}
}
public ham_Spawn(id){
	
	if( !is_user_alive(id) || !is_user_connected(id) || is_user_bot(id) || is_user_hltv(id)) return HAM_IGNORED;


	if (!userFirstSpawn[id]){

		set_task(2.0, "globalHud" , id + TASK_GLOBAL);
		set_task(speedMine - float(userUpgradeMine[id][up_GOBLIN]), "miningNugget", id + TASK_MINER);
		
		if( !task_exists(id+TASK_CHECKFPS) )
			set_task(1.0, "checkFPS", id+TASK_CHECKFPS);
			
		if(!isSuperAdmin(id)){	
			if( !task_exists(id+TASK_AFK) )
				set_task(0.1, "checkCamping", id + TASK_AFK);
		}

		userFirstSpawn[id] = true;
		
		remove_task(id + TASK_PUSH);
		remove_task(id + TASK_RESPAWN);
		remove_task(id + TASK_IDLESOUND);
	
	}

	remove_task( id+TASK_SKILL_BLOOD);
	gameUserSpawn(id);
	setZombieClass(id);
	setHumanClass(id);
	#if defined CHRISTMAS_ADDON
		setHat(id);
	#endif
	
	setFov(id, userFov[id]);
	removeGlow(id);
	stopEnt(id);
	infoTabRemove(id);
	
	userGotGrenades[id] = 0;
	
	userNoClip[id]  = false;
	userGodMod[id]  = false;
	userAllowBuild[id] =false; 
	userPush[id]	= false;
	
	return HAM_IGNORED;
}
public changeTeamIfSuggested(id){
	if( userSuggestTeam[id] == 0 || gameTime) return;
	
	new ct=numPlayers(2, false)-(get_user_team(id)==2?1:0);
	new tt=numPlayers(1, false)-(get_user_team(id)==1?1:0);
	new dif = abs(ct-tt);
	
	if( tt == 0 || ct == 0 ) return;
	if( dif > 1 ) return;
	
	cs_set_user_team(id,  userSuggestTeam[id] );
	respawnPlayerAdmin(id);
	userRoundTeam[id] = userSuggestTeam[id];
	userSuggestTeam[id]=0;
	client_print(0, print_chat, "changeTeamIfSuggested");
}
public cmdChangeTeam(id){
	if( get_user_team(id) ){
		globalMenu(id);
		return PLUGIN_HANDLED;
	}
	return PLUGIN_CONTINUE;
}
public messageShowMenu(msgid, dest, id) {
	static team_select[] = "#Team_Select";
	static menu_text_code[sizeof team_select];
	get_msg_arg_string(4, menu_text_code, sizeof menu_text_code - 1);
	if (!equal(menu_text_code, team_select))
		return PLUGIN_CONTINUE;
	forceTeam(id, msgid);

	return PLUGIN_HANDLED;
}

public messageVGUIMenu(msgid, dest, id) {
	if (get_msg_arg_int(1) != 2)
		return PLUGIN_CONTINUE;

	forceTeam(id, msgid);

	return PLUGIN_HANDLED;
}
public forceTeam(id, msgid){
	static gData[1];
	gData[0] = msgid;
	set_task(0.1, "joinTeam", id, gData, sizeof(gData));
	
}
public joinTeam(menu_msgid[], id) {
	if (get_user_team(id) || is_user_hltv(id)) return;
		
	static msg_block, joinclass[] = "joinclass";
	msg_block = get_msg_block(menu_msgid[0]);
	set_msg_block(menu_msgid[0], BLOCK_SET);
	new ct=numPlayers(2,false);
	new tt=numPlayers(1,false);
	if( ct == 0 && tt == 0 )
		engclient_cmd(id, "jointeam", "2");
	else engclient_cmd(id, "jointeam", "5");
	engclient_cmd(id, joinclass, "5");
	set_msg_block(menu_msgid[0], msg_block);
	if( userReconnected[id] ){				
		cs_set_user_team(id, 1);
		//ColorChat(0, GREEN, "---^x01 Gracz^x03 %s^x01 zrobil^x03 RC^x01 zostal przeniesiony do^x03 ZM^x04 ---", userName[id]);
	} else addMission(id, mission_CONNECT, 1);
	set_task( 1.0, "checkTeam", id);
}
public messageSayText(){
	new arg[32];
	get_msg_arg_string(2, arg, sizeof(arg));
	if(containi(arg,"name")!=-1){
		return PLUGIN_HANDLED;
	}
	return PLUGIN_CONTINUE;
}
public messageAudioMsg(iMsgId, iMsgDest, id){
	
	new szMsg[23];	
	get_msg_arg_string(2, szMsg, sizeof(szMsg));
	if( equal(szMsg, "%!MRAD_ROUNDDRAW") ){
		return PLUGIN_HANDLED;
	}
	return PLUGIN_CONTINUE;
}
public messageTextMsg(iMsgId, iMsgDest, id){
		
	new szMsg[23];	
	get_msg_arg_string(2, szMsg, sizeof(szMsg));
	if( equal(szMsg, "#Round_Draw" ) || equal(szMsg, "#Terrorists_Win" )  || equal(szMsg, "#CTs_Win" ) ){
		return PLUGIN_HANDLED;
	}
	return PLUGIN_CONTINUE;
}
public checkTeam(id){	
	
	if( !is_user_connected(id) || is_user_hltv(id) ) return PLUGIN_CONTINUE;

	if( gameTime ){
		if( get_user_team(id) == 2){				
			if( !is_user_alive(id ) ){				
				cs_set_user_team(id, 1);
				respawnPlayer(id+TASK_RESPAWN);
			}
		}else if( get_user_team(id) == 1){			
			if( !is_user_alive(id ) )
				respawnPlayer(id+TASK_RESPAWN);
		}
	}else if( buildTime || prepTime ){
		if( get_user_team(id) == 2){				
			if( !is_user_alive(id ) ){	
				respawnPlayer(id+TASK_RESPAWN);
			}
		}else if( get_user_team(id) == 1){			
			if( !is_user_alive(id ) )
				respawnPlayer(id+TASK_RESPAWN);
		}
	}
	return PLUGIN_CONTINUE;	
}
public blockCommand(id){
	return PLUGIN_HANDLED;
}
public blockJumpOnHead(id, target){
	if(get_user_team(id) != get_user_team(target)/* || get_user_team(id) == 2 || get_user_team(id) == 1*/){
		
		new Float:fOrigin[3], Float:fOriginTarget[3];
		pev(id, pev_origin, fOrigin);
		pev(target, pev_origin, fOriginTarget);
		if( fOrigin[2] - fOriginTarget[2] > 68 ){
			set_pev(id, pev_gravity, 3.0);
			if( task_exists(id+TASK_GRAVITY) )
				remove_task(id+TASK_GRAVITY);
			set_task(0.1, "removeGravity", id+TASK_GRAVITY);
		}
	}
}
public removeGravity(id){
	id -= TASK_GRAVITY;
	set_pev(id, pev_gravity, 1.0);
}
new oldMessage[33][124];
public cmdSay(id){
	if( !isPlayer(id) ) return PLUGIN_HANDLED;
	
	new szMessage[124];
	read_args(szMessage, sizeof( szMessage )); 
	remove_quotes(szMessage);
		
	if(szMessage[0] == '/' || szMessage[0] == '!'){
		if(!playerLogged(id)){
			if(equal(szMessage, "/konto", 6)  || equal(szMessage, "/haslo", 6) || equal(szMessage, "/zaloguj", 8) || equal(szMessage, "/login", 6) || equal(szMessage, "/zarejestruj", 12)){
				mainMenuAccount(id);
				return PLUGIN_HANDLED;
			}
			ColorChat(id, GREEN, "---^x01 Zaloguj sie aby wpisywac komendy!^x04 ---");
			mainMenuAccount(id);
			return PLUGIN_HANDLED;
		} else {
			if(isAdmin(id)){
				if( adminCommands(id, szMessage) == PLUGIN_HANDLED )
					return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/sklep") || equal(szMessage, "/shop")){
				shopGlobalMenu(id);
				return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/konto", 6)  || equal(szMessage, "/haslo", 6) || equal(szMessage, "/zaloguj", 8) || equal(szMessage, "/login", 6)){
				mainMenuAccount(id);
				return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/odblokuj") || equal(szMessage, "/o") ){
				cmdUnstuck(id);
				return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/czas", 5) || equal(szMessage, "/time", 5)){
				loadStatsSql(id, 3);
				return PLUGIN_HANDLED;
			}
			if( equal(szMessage, "/team", 8) || 0 <= contain(szMessage, "/dru") || 0 <= contain(szMessage, "/zap") || equal(szMessage, "/t")){
				teamOption(id);
				return PLUGIN_HANDLED;
			}
			if( equal(szMessage, "/guns", 4) || equal(szMessage, "/weap", 5) || equal(szMessage, "/bron", 5)){
				menuWeapon(id);
				return PLUGIN_HANDLED;
			}
			if( equal(szMessage, "/happy") ){
				if(!(gameTime || buildTime || prepTime)){
					ColorChat(id, GREEN, "[HAPPY HOUR]^x01 Runda jeszcze nie wystartowala!");
					return PLUGIN_HANDLED;
				}
					
				if(!hourTime){
					ColorChat(id, GREEN, "[HAPPY HOUR]^x01 Przykro mi :( Aktualnie nie ma^x04 happy hour!");
					ColorChat(id, GREEN, "[HAPPY HOUR]^x01 Moze niedlugo sie trafi :)");
					return PLUGIN_HANDLED;
					
				}
					
				ColorChat(id, GREEN, "[HAPPY HOUR]^x01 Aktualnie trwa^x04 happy hour!");
				ColorChat(id, GREEN, "[HAPPY HOUR]^x01 Bonus:^x03 %s", happyHourDesc[randomHappyHour][0]);
				return PLUGIN_HANDLED;
			}
			
			if( 0 <= contain(szMessage, "/me")){
				globalMenu(id);
				return PLUGIN_HANDLED;
			}	
			if(equal(szMessage, "/drop")){
				ColorChat(id,GREEN, "---^x01 Szansa na^x03 drop skrzyni^x01 wynosi^x03 %0.2fproc.^x04 ---",dropChest(id));
				return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/klasa")){
				if(get_user_team(id) != 2) classZombie(id);
				else classHuman(id);
				return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/zombi", 6)){
				classZombie(id);
				return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/budowniczy")){
				classHuman(id);
				return PLUGIN_HANDLED;
			}
			if( equal(szMessage, "/help", 5) || equal(szMessage, "/pomoc", 6) ||  equal(szMessage, "/p")){
				adminHelp(id);
				return PLUGIN_HANDLED;
			}
			if( equal(szMessage, "/misj", 5) || equal(szMessage, "/sekretna") ||  equal(szMessage, "/podstawowa")){
				menuMissionAll(id);
				return PLUGIN_HANDLED;
			}
			if ( equal(szMessage, "/respawn") || equal(szMessage, "/odrodz") || equal(szMessage, "/r")){
				if((get_user_team(id) == 2) && ( buildTime || prepTime)){
					respawnPlayerAdmin(id);
					return PLUGIN_HANDLED;
				
				} else if (( get_user_team(id) == 1) && get_user_health(id) == userMaxHealth[id]){
					respawnPlayerAdmin(id);
					return PLUGIN_HANDLED;
				} else {
					ColorChat(id, GREEN, "---^x01 Komenda jest nie dostepna w tym momencie! ---");
					return PLUGIN_HANDLED;
				}
			}
			if(equal(szMessage, "/sms", 4)){
				smsMainMenu(id);
				return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/vips")){
				showVipsOnline(id);
				return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/vip")){
				showInfoVip(id);
				return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/kopalnia")){
				globalMenuCave(id);
				return PLUGIN_HANDLED;	
			}
			if(0 <= contain(szMessage, "/top")){
				loadStatsSql(id, 16);
				return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/rank")){
				loadStatsSql(id, 2);
				return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/info", 5)){	
				new szName[33], szCommand[10]
				parse( szMessage,
					szCommand, 	sizeof(szCommand),
					szName, 	sizeof(szName)
				);
				new target = cmd_target(id, szName, 0);
				
				infoPlayer(id, target);
				
				return PLUGIN_HANDLED;
			}
			if (equal(szMessage, "/maxdmg")){
				ColorChat(id, GREEN, "---^x01 Twoj najwiekszy zadany^x03 DMG^x01 to:^x03 %d^x04 ---", userMaxDmg[id]);
				return PLUGIN_HANDLED;
			}
			if (equal(szMessage, "/resetdmg")){
				if(userMaxDmg[id] == 0){
					ColorChat(id, GREEN, "---^x01 Nie mozesz zresetowac^x03 0^x01 DMG^x04 ---", userMaxDmg[id]);
					return PLUGIN_HANDLED;		
				}
				ColorChat(0, GREEN, "---^x01 Gracz^x03 %s^x01 zresetowal^x04 %d^x01 DMG^x04 ---", userName[id], userMaxDmg[id]);
				userMaxDmg[id] = 0;
				return PLUGIN_HANDLED;
			}
			
			if (equal(szMessage, "/id")){
				ColorChat(id, GREEN, "---^x01 Twoje^x03 ID^x01 konta:^x03 %d^x04 ---", userSqlId[id]);
				return PLUGIN_HANDLED;
			}
			if (equal(szMessage, "/nagroda")){
				checkForAward(id);
				return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/k") || equal(szMessage, "/kolor") || equal(szMessage, "/colors", 3)){
				menuColor(id, userPageColor[id]);
				return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/komendy")){
				show_motd(id, "komendy.txt", "Komendy");
				return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/bind",5)){
				show_motd(id, "bindy.txt", "Bindy");
				return PLUGIN_HANDLED;
			}
			if(equal(szMessage, "/regulamin")){	
				regulamin(id);	
				return PLUGIN_HANDLED;
			}
			if (equal(szMessage, "/admins")){
				adminsPointHelp(id);
				return PLUGIN_HANDLED;
			}
			if (equal(szMessage, "/klan")){
				globalClanMenu(id);
				return PLUGIN_HANDLED;
			}
			if (equal(szMessage, "/free")){
				menuFreeAward(id);
				return PLUGIN_CONTINUE;
			}
			if (equal(szMessage, "/hide")){
				if( hasOption(userSaveOption[id], save_INVIS) ){
					removeOption(userSaveOption[id], save_INVIS);
					ColorChat(id, GREEN, "---^x01 Widzenie osob wlaczone^x04 ---");
				}else{
					addOption(userSaveOption[id], save_INVIS);
					ColorChat(id, GREEN, "---^x01 Widzenie osob wylaczone^x04 ---");
				}
				return PLUGIN_HANDLED;
			}
			if (equal(szMessage, "/exp")){
				ColorChat(id, GREEN, "---^x01 Postac:^x03 Lv: %d - XP: %0.1f / %0.1f^x04 ---",userLevel[id], userExp[id],needXp(id, userLevel[id]));
				ColorChat(id, GREEN, "---^x01 Klasa %s:^x03 Lv: %d - XP: %0.1f / %0.1f^x04 ---",classesHuman[userClassHuman[id]][0],  userHumanLevel[id][userClassHuman[id]], userExpClass[id][userClassHuman[id]],needXpClass(userHumanLevel[id][userClassHuman[id]]));
				return PLUGIN_HANDLED;
			}
			if( equal(szMessage, "/daj", 4) ){
				new szCommand[4], szPlayer[33], szValue[8];				
				parse(szMessage, 
					szCommand, sizeof(szCommand), 
					szPlayer, sizeof(szPlayer), 
					szValue, sizeof(szValue) 
				);
				new target = cmd_target(id, szPlayer, 0);
				if( target == id ) {
					ColorChat(id, GREEN, "%s Sobie nie mozesz wyslac Brylek!", PREFIXSAY);
					return PLUGIN_HANDLED;
				}
				if( !target ) {
					userMenuPlayer[id] = MENU_PLAYER_NUGGET;
					choosePlayer(id, 0);
				
					return PLUGIN_HANDLED;
				}
				new gValue = abs(str_to_num(szValue));
				if( gValue == 0){
					ColorChat(id, GREEN, "%s Nie mozesz dac 0 Brylek!", PREFIXSAY);
					return PLUGIN_CONTINUE;
				}
				if( gValue > userNugget[id] ){
					ColorChat(id, GREEN, "%s Nie masz tyle Brylek!", PREFIXSAY);
					return PLUGIN_CONTINUE;
				}
				new gText[128];
				logType[id] = LOG_TRANSFER;
				if(logType[id] == LOG_TRANSFER){
					format(gText, sizeof(gText), "wyslal [%d] brylek graczowi [%s]", gValue, userName[target]);
					logBB(id, gText);
				}
				userNugget[id]		-=	gValue;
				userNugget[target]	+=	gValue;
				ColorChat(id, 		GREEN, 	"%s Wyslales^x03 %s^x01 Brylek do gracza^x04 %s!", PREFIXSAY, formatNumber(gValue), userName[target] );
				ColorChat(target, 	GREEN, 	"%s Otrzymales^x03 %s^x01 Brylek do gracza^x04 %s!", PREFIXSAY, formatNumber(gValue), userName[id] );
				return PLUGIN_HANDLED;
			}
			if (equal(szMessage, "/kostium")){	
				globalMenuCostumes(id);
				return PLUGIN_HANDLED;
			}
			if (equal(szMessage, "/zabij")){	
				deathPriceMenu(id);
				return PLUGIN_HANDLED;
			}
			if (equal(szMessage, "/wycisz")){	
				userMenuPlayer[id] = MENU_PLAYER_MUTE;
				choosePlayer(id, 0);
				return PLUGIN_HANDLED;
			}
			if (equal(szMessage, "/ban")) return PLUGIN_HANDLED;
		}
		
		ColorChat(id, GREEN, "---^x01 Nie ma takiej komendy ---");
		return PLUGIN_HANDLED;
	}
	
	szMessage[sizeof(szMessage)-1] = 0;
	trim(szMessage);
	if( strlen(szMessage) > 0 ){
		if(!playerLogged(id)){
			ColorChat(id, GREEN, "---^x01 Zaloguj sie aby moc pisac!^x04 ---");
			mainMenuAccount(id);
			return PLUGIN_HANDLED;
		}
		
		if(serverOffChat && !isSuperAdmin(id)){
			ColorChat(id, RED, "^x04---^x03 Chat Zostal wylaczony przez Admina^x04 ---");
			ColorChat(id, GREEN, "---^x01 Twoja wiadomosc:^x03 '%s'^x04 ---", szMessage);
			return PLUGIN_HANDLED;
		}
		
		new muteLeft = (userMute[id] - get_systime())	
		if ((userMute[id] - get_systime() > 0)){
			ColorChat(id, RED, "Zostales zmutowany!^x01 Koniec za:^x04 %d godz %s%d min %s%d sek!",  ( muteLeft / HOUR ),(muteLeft / MINUTE % MINUTE)<10?"0":"", ( muteLeft / MINUTE % MINUTE ), (muteLeft%MINUTE)<10?"0":"", ( muteLeft %MINUTE ));
			return PLUGIN_HANDLED;
		}
		if(equali( oldMessage[id], szMessage)){
			ColorChat(id, GREEN, "%s Nie powtarzaj tej samej^x03 wiadomosci^x01 kilka razy!", PREFIXSAY);
			return PLUGIN_HANDLED;
		} 
		
		oldMessage[id] = szMessage;
		
		new chatPrefix[33];
		get_clan_info(clan[id], CLAN_NAME, chatPrefix, sizeof(chatPrefix));

		format(chatPrefix, sizeof(chatPrefix), "^x03[^x01%s^x03] ", chatPrefix);	
		
		if(isVip(id)) ColorChat(0, get_user_team(id)==1?RED:BLUE,"%s^x04| Vip |^x03 %s%s^x01:^x01 %s",clan[id] ? chatPrefix : "",  userName[id], isSuperAdmin(id) ? "^x04*" : "", szMessage);
		else ColorChat(0, get_user_team(id)==1?RED:BLUE,"%s^x03%s%s^x01: %s", clan[id] ? chatPrefix : "", userName[id], isSuperAdmin(id) ? "^x04*" : "",  szMessage);
		
		if(userFPS[id] < 30) addSecretMission(id, mission_secret_CHEATER, 1);
		
		new gText[192];
		logType[id] = LOG_CHAT;
		if(logType[id] == LOG_CHAT){
			format(gText, sizeof(gText), "global chat: [%s]", szMessage);
			logBB(id,gText);
		}
	}
	return PLUGIN_HANDLED;
}


public adminsPointHelp(id){	
		
	new gText[2048], iLen=0;
	
	new sizeText = sizeof(gText)-iLen-1;
	
	iLen += format(gText[iLen], sizeText, "\
					<style>\
							*{ font-size: 16px; font-family: Montserrat; color: white; text-align: center; padding: 0; margin: 0;}\
							body{border: 1px solid %s; background: #111}\
			 				b{color:%s; text-shadow: 0 0 5px %s;}\
					</style>",accentMotdColor,accentMotdColor,accentMotdColor);		
									
	iLen += format(gText[iLen], sizeText, "<p>Pomoce Adminow ( Online )</p><hr size=1 color=%s>",accentMotdColor);
	iLen += format(gText[iLen], sizeText, "<table style=^"margin: 20px;width:710%^">");	
	iLen += format(gText[iLen], sizeText, "<tr><td><b>Admin</b></td><td><b>Pomocy</b></td></tr></br>");

	for(new i = 1; i < maxPlayers; i++){
		if(!is_user_connected(i) || is_user_hltv(i) || is_user_bot(i) || !isAdmin(i)) continue;
		
		iLen += format(gText[iLen], sizeText, "<tr><td>%s</td><td>%d</td></tr>",userName[i], userHelpPoint[i]);
	}
		
	iLen += format(gText[iLen], sizeText, "</table>");
	show_motd(id, gText, "Lista Adminow");
}

public regulamin(id){
	new menu = menu_create("\r[BaseBuilder]\y Regulamin!", "regulamin_2");	
	
	menu_additem(menu, "Regulamin\d Ogolny");
	menu_additem(menu, "Regulamin\y Budowniczych");
	menu_additem(menu, "Regulamin\y Zombi");
	menu_additem(menu, "Regulamin\r Admina");
	
	menu_display(id, menu, 0);
}

public regulamin_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return PLUGIN_CONTINUE;
	}
	switch(item){
		case 0:show_motd(id, "regulaminOgolny.txt", "Regulamin Ogolny");
		case 1:show_motd(id, "regulaminBudowniczych.txt", "Regulamin Budowniczych");
		case 2:show_motd(id, "regulaminZombie.txt", "Regulamin Zombi");
		case 3:show_motd(id, "regulaminAdmin.txt", "Regulamin Admina");
	}
	regulamin(id);
	return PLUGIN_CONTINUE;
}

public cmdSayClan(id){
	
	if( !isPlayer(id) ) return PLUGIN_HANDLED;
	
	if(!playerLogged(id)){
		ColorChat(id, GREEN, "---^x01 Zaloguj sie aby moc pisac!^x04 ---");
		mainMenuAccount(id);
		return PLUGIN_HANDLED;
	}
	if(!clan[id]){
		ColorChat(id, GREEN, "---^x01 Dolacz do klanu aby pisac tylko do klanu!^x04 ---");
		return PLUGIN_HANDLED;
	}
		
	new szMessage[124];
	read_args(szMessage, sizeof( szMessage )); 
	remove_quotes(szMessage)

	szMessage[sizeof(szMessage)-1] = 0;
	trim(szMessage);
	if( strlen(szMessage) > 0 ){
		
		new clanName[14];
		get_clan_info(clan[id], CLAN_NAME, clanName, sizeof(clanName));	
		
		for(new i = 1; i < maxPlayers; i ++)
			if(is_user_connected(i) && clan[id] == clan[i])
				ColorChat(i, get_user_team(i)==1?RED:BLUE,"^x04[[-^x03 %s^x04 -]]^x01 %s:^x04 %s", clanName, userName[id], szMessage);
		
		new gText[192];
		logType[id] = LOG_CHAT;
		if(logType[id] == LOG_CHAT){
			format(gText, sizeof(gText), "clan chat: [%s]", szMessage);
			logBB(id,gText);
		}		
		return PLUGIN_HANDLED_MAIN;
	}
	return PLUGIN_HANDLED;
}
public respawnPlayerAdmin(id){
	ExecuteHamB(Ham_CS_RoundRespawn, id);	
}


public Release_Zombies(){
	
	gTime = gameTime;
	buildTime = false;

	startRelease();
	
	for( new i = 1 ; i < maxPlayers; i++ ){
		if( !is_user_connected(i) ) continue;
		
		if(userGrab[i]) stopEnt(i);
			
		if(get_user_team(i) == 2){
			
			if(userWeaponBool[i]){
				giveWeapons(i, userWeaponSelect[i])
				userWeaponBool[i] = false;
			}
			else if(gameTime) menuWeapon(i);
		}	
	}
	switch(random_num(0,1)){
		case 0:spkGameSound(0,sound_START);
		case 1:spkGameSound(0,sound_START2);
	}
	makeBarrierNoSolid();
}
public cmdUnstuck(id){
	
	if(get_user_team(id) == 1) return PLUGIN_CONTINUE;
	if(gameTime) return PLUGIN_CONTINUE;
	
	static Float:origin[3];
	static Float:mins[3], hull;
	static Float:vec[3];
	static o;
	if (is_user_connected(id) && is_user_alive(id)){
		pev(id, pev_origin, origin);
		hull = pev(id, pev_flags) & FL_DUCKING ? HULL_HEAD : HULL_HUMAN;
		if (!is_hull_vacant(origin, hull,id) && !get_user_noclip(id) && !(pev(id,pev_solid) & SOLID_NOT)) {
			++stuck[id];
			pev(id, pev_mins, mins);
			vec[2] = origin[2];
			for (o=0; o < sizeof size; ++o) {
				vec[0] = origin[0] - mins[0] * size[o][0];
				vec[1] = origin[1] - mins[1] * size[o][1];
				vec[2] = origin[2] - mins[2] * size[o][2];
				if (is_hull_vacant(vec, hull,id)) {
					engfunc(EngFunc_SetOrigin, id, vec);
					Display_Fade(id, 512,512,512, 255,32,32, 90);
					set_dhudmessage(255, 32, 32, -1.0, 0.3, 0, 0.5, 0.9, 0.5, 0.5);
					show_dhudmessage(id, "!! Odblokowano !!");
					set_pev(id,pev_velocity,{0.0,0.0,0.0});
					o = sizeof size;
				}	
			}
		}
		else stuck[id] = 0;	
	}
	return PLUGIN_CONTINUE;
}
public userJetPackOn(id){
	if(  buildTime || isAdmin(id)){
		userJetPack[id] = true;
		entity_set_int(id,EV_INT_sequence, 8);
	}
	return PLUGIN_HANDLED;
}

public userJetPackOff(id){
	userJetPack[id] = false;
	return PLUGIN_HANDLED;
}

public fw_CmdStart( id, uc_handle, randseed ){
	
	if( !is_user_alive(id) || !is_user_connected(id) || is_user_hltv(id) ) return FMRES_IGNORED;
	
	userFPS[id] = floatround(1 / (get_uc(uc_handle, UC_Msec) * 0.001));
	
	setUserSpeed(id);
	teamLineOrSprite(id);
	buildingMain(id);
	
	teamCampTouching(id);
	findCoinArround(id);
	jumpFuncBlock(id, 1);
	
	moveRocket(id);
	
	if(userFPS[id] >= 125 || !playerLogged(id)){
		if(entity_get_int(id, EV_INT_button) & IN_JUMP || entity_get_int(id, EV_INT_button) & IN_DUCK ){
			entity_set_int(id, EV_INT_oldbuttons, entity_get_int(id, EV_INT_button));
		}
	}
	if( userJetPack[ id ] ){
		if( buildTime || isAdmin(id) ){		
			new Float:fVelo[ 3 ];		
			VelocityByAim(id, userJetpackSpeed[id], fVelo );
			entity_set_vector(id , EV_VEC_velocity, fVelo );
			entity_set_int(id,EV_INT_sequence, 8 ) ;									
		}			
	}
	if (!userGrab[id] || !is_valid_ent(userGrab[id])) return FMRES_HANDLED;
	if(userNoRecoil[id]) set_pev(id, pev_punchangle, Float:{0.0,0.0,0.0});
	
	return FMRES_IGNORED;
}
public jumpFuncBlock(id, type){
	switch(type){
		case 1:{
			if(jumpBlock[id]){
				new Float:powerJump = 550.0;
				new Float:fVelocity[3] = 0.0;
		
				pev(id, pev_velocity, fVelocity);
		
				fVelocity[2] = powerJump;
				set_pev(id, pev_velocity, fVelocity);
				jumpBlock[id] = false;
			} 
		}
		case 2:{
			jumpBlock[id] = true;
		}
	}
}
public fw_EmitSound(id,channel,const sample[],Float:volume,Float:attn,flags,pitch){
	if (!is_user_connected(id) || get_user_team(id) != 1 || buildTime || prepTime || roundEnd) return FMRES_IGNORED;
		
	if(equal(sample[7], "die", 3) || equal(sample[7], "dea", 3)){
		emit_sound(id,channel,zombieSound[random_num(3,5)],volume,attn,flags,pitch);
		return FMRES_SUPERCEDE;
	}
	if(equal(sample[7], "bhit", 4)){
		emit_sound(id,channel,zombieSound[random_num(0,2)],volume,attn,flags,pitch);
		return FMRES_SUPERCEDE;
	}
	/*if (equal(sample[8], "kni", 3)){
		
		if (equal(sample[14], "sla", 3)){
			//emit_sound(id,channel,zombieSound[random_num(12,14)],volume,attn,flags,pitch)
			return FMRES_SUPERCEDE;
		}
		if (equal(sample[14], "hit", 3)){
			if (sample[17] == 'w'){
				//emit_sound(id,channel,zombieSound[random_num(9,11)],volume,attn,flags,pitch)
				return FMRES_SUPERCEDE;
			} else{
				//emit_sound(id,channel,zombieSound[random_num(9,11)],volume,attn,flags,pitch)
				return FMRES_SUPERCEDE;
			}
		
			if (equal(sample[14], "sta", 3)){
			//emit_sound(id,channel,zombieSound[random_num(12,14)],volume,attn,flags,pitch)
				return FMRES_SUPERCEDE;
			}
		}
	}*/
	return FMRES_IGNORED;
}


public resetBlocks(){

	new szClass[10], szTarget[7];
	new Float:fOrigin[3];		
	for(new ent=maxPlayers; ent<maxEnts;ent ++){
		
		if( !pev_valid(ent) ) continue;
		if( ent == gBarrier )  continue;
			
		entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass));
		entity_get_string(ent, EV_SZ_targetname, szTarget, sizeof(szTarget));
		
		if( !equal(szClass, "func_wall") || containi(szTarget, "ignore") !=-1 || equal(szTarget, "barrier")) continue;
		
		unSetBlock( ent )
		
		if( getLock(ent) == 3 ) remove_entity(ent);
		else if(getLock(ent) ==  2){			
			entity_get_vector(ent, EV_VEC_vuser3, fOrigin );
			entity_set_vector(ent, EV_VEC_vuser4, fOrigin );
			engfunc( EngFunc_SetOrigin, ent, fOrigin );
		}else{	
			engfunc( EngFunc_SetOrigin, ent, Float:{0.0,0.0,0.0} );			
			set_pev(ent,pev_rendermode,kRenderNormal);
			set_pev(ent,pev_rendercolor, Float:{ 0.0, 0.0, 0.0 });
			set_pev(ent,pev_renderamt, 255.0 );
		}
	}
	return PLUGIN_CONTINUE;
}
public removeNotUsedBlock(){

	new szClass[10], szTarget[10];
	for(new ent=maxPlayers; ent<maxEnts;ent ++){
		
		if( !pev_valid(ent) ) continue;
		if( ent == gBarrier )  continue;
		
		entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass));
		entity_get_string(ent, EV_SZ_targetname, szTarget, sizeof(szTarget));
		
		if(!equal(szClass, "func_wall") || containi(szTarget, "ignore")!=-1 || containi(szTarget, "JUMP")!=-1) continue;			
		if(getOwner(ent) != 0 ) continue;
		
		if( getLock(ent) == 3 ) remove_entity(ent);
		else engfunc( EngFunc_SetOrigin, ent, Float:{ -8192.0, -8192.0, -8192.0 } );
	}
	return PLUGIN_CONTINUE;
}

public globalMenu(id){
	
	if(!playerLogged(id)){
		mainMenuAccount(id);
		return;
	}
	
	new gText[1536], iLen;

	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dTURBOMod by\r KoRrNiK^n"); // No i po co usuwacie?? xd
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Glowne Menu^n");

	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r1.\w Sklep");
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r2.\w Menu Broni\d [\r%d/3\d]", userSelectWeapon[id]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r3.\w Klasy Zombi:\r %s", classesZombies[userClass[id]][0]);
	
	if(hasClassHuman(id, userClassHuman[id]))
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r4.\w Klasa Budowniczych:\r %s^n",  classesHuman[userClassHuman[id]][0]);
	else iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r4.\w Klasa Budowniczych:\r Nie wybrano^n");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r5.\w Odblokuj & Odrodz");
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r6.\r Pomoc!\d [%d/%d]", userMaxHelp[id],maxHelpCount );
	
	if (userTeam[id])
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r7.\y Druzyna z:\r %s", userName[userTeam[id]]);
	else iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r7.\y Zapros do druzyny");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r8.\w Oddajesz EXP'a Klasie:\r %s", typeExpClass[userGiveClassExp[id]][0]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\r9.\r Moce!");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\r0.\w Wyjdz");
	
	show_menu(id, B1 | B2 | B3 | B4 | B5 | B6 | B7 | B8 | B9 | B0, gText, -1, "globalMenu" );	
}
public globalMenu_2(id, item){
	
	switch(item){
		
		case 0:shopGlobalMenu(id);
		case 1:menuWeapon(id);
		case 2:classZombie(id);
		case 3:classHuman(id);
		case 4:{
			if(get_user_team(id) == 1 && get_user_health(id) == userMaxHealth[id]){
				respawnPlayerAdmin(id);
				return PLUGIN_CONTINUE;
			}
			if (!gameTime){
				if (is_user_alive(id)) cmdUnstuck(id);
				else respawnPlayerAdmin(id);
			} else ColorChat(id, GREEN, "%s Nie mozesz teraz sie odrodzic", PREFIXSAY);
		}
		case 5:adminHelp(id);
		case 6:teamOption(id);
		case 7:{
			userGiveClassExp[id] 	= (userGiveClassExp[id]+1) 	% sizeof(typeExpClass);
			globalMenu(id);
		}
		case 8:atributeMenu(id);
	}
	return PLUGIN_CONTINUE;
}
public logBB(id, szText[]){
	new szMessage[256];
	new szName[32];
	new szIP[32];
	new szCurrentTime[9];
	new szData[9];
	new szDir[128];
	
	get_time("%H:%M:%S",szCurrentTime,sizeof(szCurrentTime));
	get_time("%Y%m%d",szData,sizeof(szData));
	
	get_user_name(id,szName,sizeof(szName));
	get_user_ip(id,szIP,sizeof(szIP));

	get_basedir(szDir,sizeof(szDir));
	
	new const folderLogs[]	=	"bbLogs";
	
	switch(logType[id]){
		case LOG_BUY: 		format(szDir,sizeof(szDir),"%s/%s/shopSmsLog/sklep_%s.log",szDir, folderLogs, szData);	
		case LOG_ADD: 		format(szDir,sizeof(szDir),"%s/%s/shopSmsLog/dodawanie_%s.log",szDir, folderLogs, szData);	
		case LOG_LOGIN: 		format(szDir,sizeof(szDir),"%s/%s/accountLog/login_%s.log",szDir, folderLogs, szData);	
		case LOG_REGISTER: 	format(szDir,sizeof(szDir),"%s/%s/accountLog/register_%s.log",szDir, folderLogs, szData);	
		case LOG_LOGOUT:		format(szDir,sizeof(szDir),"%s/%s/accountLog/logout_%s.log",szDir, folderLogs, szData);	
		case LOG_ERROR: 		format(szDir,sizeof(szDir),"%s/%s/accountLog/error_%s.log",szDir, folderLogs, szData);	
		case LOG_DELETEACCOUNT: 	format(szDir,sizeof(szDir),"%s/%s/accountLog/delete_%s.log",szDir, folderLogs, szData);	
		case LOG_TRANSFER: 	format(szDir,sizeof(szDir),"%s/%s/transferLog/transfer_%s.log",szDir, folderLogs, szData);	
		
		case LOG_AWARD: 		format(szDir,sizeof(szDir),"%s/%s/awardLog/award_%s.log",szDir, folderLogs, szData);
		
		case LOG_MISSION: 	format(szDir,sizeof(szDir),"%s/%s/missionLog/mission_%s.log",szDir, folderLogs, szData);
		case LOG_CLASS: 		format(szDir,sizeof(szDir),"%s/%s/classLog/class_%s.log",szDir, folderLogs, szData);
		case LOG_MUTE: 		format(szDir,sizeof(szDir),"%s/%s/muteLog/mute_%s.log",szDir, folderLogs, szData);
		case LOG_AFK: 		format(szDir,sizeof(szDir),"%s/%s/afkLog/afk_%s.log",szDir, folderLogs, szData);
		case LOG_CAVE: 		format(szDir,sizeof(szDir),"%s/%s/caveLog/cave_%s.log",szDir, folderLogs, szData);
		case LOG_CONNECT: 	format(szDir,sizeof(szDir),"%s/%s/conLog/con_%s.log",szDir, folderLogs, szData);
		case LOG_CHAT: 		format(szDir,sizeof(szDir),"%s/%s/chatLog/chat_%s.log",szDir, folderLogs, szData);
		
		case LOG_CLAN_ADD: 	format(szDir,sizeof(szDir),"%s/%s/clanLog/add_%s.log",szDir, folderLogs, szData);
		case LOG_CLAN_CREATE:	format(szDir,sizeof(szDir),"%s/%s/clanLog/create_%s.log",szDir, folderLogs, szData);
		case LOG_CLAN_PROMOTION:	format(szDir,sizeof(szDir),"%s/%s/clanLog/promotion_%s.log",szDir, folderLogs, szData);
		case LOG_CLAN_DELETE: 	format(szDir,sizeof(szDir),"%s/%s/clanLog/delete_%s.log",szDir, folderLogs, szData);
		case LOG_CLAN_UPGRADE: 	format(szDir,sizeof(szDir),"%s/%s/clanLog/upgrade_%s.log",szDir, folderLogs, szData);
		case LOG_CLAN_DEPOSIT: 	format(szDir,sizeof(szDir),"%s/%s/clanLog/deposit_%s.log",szDir, folderLogs, szData);
		case LOG_CLAN_RESET: 	format(szDir,sizeof(szDir),"%s/%s/clanLog/reset_%s.log",szDir, folderLogs, szData);
		case LOG_CLAN_LEAVE: 	format(szDir,sizeof(szDir),"%s/%s/clanLog/leave_%s.log",szDir, folderLogs, szData);
		case LOG_CLAN_MANAGE: 	format(szDir,sizeof(szDir),"%s/%s/clanLog/manage_%s.log",szDir, folderLogs, szData);
		
		case LOG_HAT_REMOVE:	format(szDir,sizeof(szDir),"%s/%s/costume/remove_%s.log",szDir, folderLogs, szData);
		case LOG_HAT_ADD: 	format(szDir,sizeof(szDir),"%s/%s/costume/add_%s.log",szDir, folderLogs, szData);
		
		case LOG_WARNING_ADD: 	format(szDir,sizeof(szDir),"%s/%s/warnings/add_%s.log",szDir, folderLogs, szData);
		case LOG_WARNING_REMOVE: format(szDir,sizeof(szDir),"%s/%s/warnings/remove_%s.log",szDir, folderLogs, szData);
		case LOG_WARNING_CHANGE: format(szDir,sizeof(szDir),"%s/%s/warnings/change_%s.log",szDir, folderLogs, szData);
	}
	format(szMessage,sizeof(szMessage),"[%s] %s {%d} - (%s) : %s",szCurrentTime,szName, userSqlId[id], szIP,szText);
	write_file(szDir, szMessage);
}

public checkFPS(id){
	id -= TASK_CHECKFPS;
	if( !is_user_connected(id) ) return;
		
	new bool:save = userExtraFps[id];
	userExtraFps[id] = false;
	
	query_client_cvar(id, "fps_max", "maxFPS");
	query_client_cvar(id, "fps_modem", "maxFPS");
	
	if( ( userExtraFps[id] || userFPS[id] >= 150 || save != userExtraFps[id] ) && get_gametime() - userInfoFps[id] > 1.0){
		userInfoFps[id] = get_gametime();
		
		new Float:fPunchAngle[3]
		fPunchAngle[0] = float(random(500));
		fPunchAngle[1] = float(random(500));
		fPunchAngle[2] = float(random(500));
		
		set_pev(id, pev_punchangle, fPunchAngle);
		set_pev(id, pev_velocity, Float:{0.0,0.0,0.0});
		screenShake(id, 250 , 20, 255);
		
		new color[3];
		color[0]  = random(45);
		color[1]  = random(45);
		color[2]  = random(45);
		
		Display_Fade(id, 4096,4096, 4096,color[0],color[1], color[2],255);
		ColorChat(id, GREEN, "---^x01 Ustaw^x03 fps_max^x04 101^x01 |^x03 fps_modem^x04 101^x01 aby nie byc blokowanym  ^x04 ---");
		set_dhudmessage(color[0]*3, color[1]*3, color[2]*3, -1.0, -1.0, _, _, 0.9, _, _);
		show_dhudmessage(id, "Wpisz w konsoli fps_max 101 | fps_modem 101 aby moc grac^n^n");
		
		client_cmd(id, "fp%s%s 1%s", "s_m", "ax", "01");
	}
	
	set_task(0.2, "checkFPS", id+TASK_CHECKFPS);

}
public maxFPS(id, const cvar[], const val[]){
	
	new fps = str_to_num(val);
	new dot = 0;
	
	new i = 0;
	while(i < strlen(val)){
		if(val[i] == '.' ) dot ++;
		i++;
	}
	
	if( fps > 101 || dot >= 2) userExtraFps[id] = true;
}
public showVipsOnline(id) {

	new gText[1536];
	
	new players[32], num, x;
	get_players(players, num);

	format(gText, sizeof(gText), "\r[BaseBuilder]\y Lista Graczy co posiadaja VIP'a");
	new menu = menu_create(gText, "showVipsOnline_2");
	for(new i = 0; i <= num; i ++){
		
		x = players[i];
		
		if(!is_user_connected(x) || is_user_bot(x) || is_user_hltv(x)) continue;
		if(!isVip(x)) continue;
			
		format(gText, sizeof(gText), "\y%s\d -\w VIP",userName[x]);
		menu_additem(menu, gText);
	}
	menu_display(id, menu, 0);
	return PLUGIN_CONTINUE;
}

public showVipsOnline_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return PLUGIN_CONTINUE;
	}
	showVipsOnline(id);
	return PLUGIN_CONTINUE;
}

public vipStatus(){
	new id = get_msg_arg_int(1);
	if (is_user_alive(id) && isVip(id)) set_msg_arg_int(2, ARG_BYTE, get_msg_arg_int(2) | 4);
}

public showInfoVip(id){
		
	new daysLeft = 0;	
	new iLen = 0;
	new gText[612];
	new size = sizeof(gText) - iLen - 1;
	
	iLen = format(gText[iLen], size, "\r[BaseBuilder]\y Opis Vipa^n");
	
	daysLeft = (timeVip[id] - get_systime())
	
	if(has_flag(id, flagVip))	iLen += format(gText[iLen],size, "^n\y%s^t^t\dPosiadasz Vipa:\r Bez Limitu Czasu^n", symbolsCustom[SYMBOL_DR_ARROW]);
	else if((daysLeft) / HOUR > 0 )	iLen += format(gText[iLen],size, "^n\y%s^t^t\dPosiadasz Vipa:\r %d\d godz\r %d\d min^n", symbolsCustom[SYMBOL_DR_ARROW], (daysLeft) / HOUR, ((daysLeft) / MINUTE) %MINUTE);
	else if( daysLeft / MINUTE > 0 ) 	iLen += format(gText[iLen],size, "^n\y%s^t^t\dPosiadasz Vipa:\r %d\d min^n", symbolsCustom[SYMBOL_DR_ARROW], (daysLeft / MINUTE) %MINUTE);
	else 				iLen += format(gText[iLen],size, "^n\y%s^t^t\dNie posiadasz Vipa^n", symbolsCustom[SYMBOL_DR_ARROW]);
	
	iLen += format(gText[iLen], size, "^n^t\yWiecej\r Expa\y oraz\r Brylek");
	iLen += format(gText[iLen], size, "^n^t\yCzas Mocy skrocony o\r 5%%");
	iLen += format(gText[iLen], size, "^n^t\yHP\d (\r Budowniczy +25\d ) (\r Zombi +500\d )");
	iLen += format(gText[iLen], size, "^n^t\yZadajesz o\r 5%%\y wiecej DMG jako Budowniczy");
	iLen += format(gText[iLen], size, "^n^t\yKolorowanie blokow:\r /kolor");
	iLen += format(gText[iLen], size, "^n^t\yLepsze bonusy pod:\r /nagroda");
	iLen += format(gText[iLen], size, "^n^t\yZwiekszony limit klockow o:\r 5");
	
	iLen += format(gText[iLen],size, "^n^n\r1.\w Kup Vipa");
	iLen += format(gText[iLen],size, "^n\r0.\w Wyjdz");

	show_menu(id, B1 | B0 , gText, -1, "showInfoVip" );
}
public showInfoVip_2(id, item){
	switch(item){
		case 0: smsMainMenu(id);
		default:{}
	}
}
public infoPlayer(id, target){
	if(  !is_user_connected(target) || !is_user_connected(id)) return PLUGIN_CONTINUE;

	new gText[1536];
	new iLen = 0;

	new sizeText = sizeof(gText)-iLen-1;
	
	new timeOnline		=	userPlayerConnected[target]=floatround(get_gametime());
			
	iLen += format(gText[iLen], sizeText, "<html>")
	iLen += format(gText[iLen], sizeText, "\
					<style>\
						*{ font-size: 15px;color: white; text-align: center; padding: 0; margin: 0;}\
						body{border: 1px solid %s; box-shadow:0 0 10px %s; background: #111;font-family: Montserrat;}\
						b{color:%s; text-shadow: 0 0 5px %s;}\
					</style>",accentMotdColor,accentMotdColor,accentMotdColor,accentMotdColor);		
							
	iLen += format(gText[iLen], sizeText, "%s:<b> %s</b><br>",  isVip(target) ? "Vip" : "Gracz", userName[target]);
	iLen += format(gText[iLen], sizeText, "<hr size=1 color=%s>",accentMotdColor);	
	iLen += format(gText[iLen], sizeText, "<table style=^"margin: 20px;width:600%;margin-left:auto;margin-right:auto;^">");	
	
	
	iLen += format(gText[iLen], sizeText, "<tr><td>Pierwszy raz byl:</td><td><b>%s</b></td></tr>", userFirstLogin[target]);
	iLen += format(gText[iLen], sizeText, "<tr><td>Poziom Postaci:</td><td><b>%d</b> %0.2f%%</td></tr>", userLevel[target], (userExp[target]*100.0/needXp(id, userLevel[target])));
	iLen += format(gText[iLen], sizeText, "<tr><td>Aktualna Klasa %s:</td><td><b>%d</b> %0.2f%%</td></tr>",classesHuman[userClassHuman[target]][0],  userHumanLevel[target][userClassHuman[target]], (userExpClass[target][userClassHuman[target]]*100.0/needXpClass(userHumanLevel[target][userClassHuman[target]])));
	
	
	//iLen += format(gText[iLen], sizeText, "<tr><td>Poziom Postaci:</td><td><b>%d</b> [ %0.2f/%0.2f ]</td></tr>", userLevel[target], userExp[target],needXp(target, userLevel[target]))
	//iLen += format(gText[iLen], sizeText, "<tr><td>Aktualna Klasa %s:</td><td><b>%d</b> [ %0.2f/%0.2f ]</td></tr>",classesHuman[userClassHuman[target]][0],  userHumanLevel[target][userClassHuman[target]], userExpClass[target][userClassHuman[target]],needXpClass(userHumanLevel[target][userClassHuman[target]]));
	
	iLen += format(gText[iLen], sizeText, "<tr><td>Czas Ogolny:</td><td><b>%d</b> godz <b>%s%d</b> min<b> %s%d</b> sek</td></tr>", playedTime(target)/HOUR, (playedTime(target)/MINUTE)<10?"0":"",playedTime(target)/MINUTE%MINUTE, playedTime(target)%MINUTE<10?"0":"", playedTime(target)%MINUTE)
	//iLen += format(gText[iLen], sizeText, "<tr><td>Czas Ogolny:</td><td><b>%02d</b> godz <b>%02d</b> min<b> %02d</b> sek</td></tr>", playedTime(target)/HOUR,(playedTime(target) % HOUR) / MINUTE, playedTime(target)%MINUTE);
	iLen += format(gText[iLen], sizeText, "<tr><td>Czas Online:</td><td><b>%d</b> godz <b>%s%d</b> min<b> %s%d</b> sek</td></tr>", timeOnline/HOUR, (timeOnline/MINUTE)<10?"0":"",timeOnline/MINUTE%MINUTE, timeOnline%MINUTE<10?"0":"", timeOnline%MINUTE);
	iLen += format(gText[iLen], sizeText, "<tr><td>Zdobyte Brylki:</td><td><b>%s</b></td></tr>",  formatNumber(userNuggetAll[target]));
	iLen += format(gText[iLen], sizeText, "<tr><td>Brylki:</td><td><b>%s</b></td></tr>",  formatNumber(userNugget[target]));
	iLen += format(gText[iLen], sizeText, "<tr><td>Zabici:</td><td><b>%s</b></td></tr>",  formatNumber(userKills[target]));
	iLen += format(gText[iLen], sizeText, "<tr><td>Zgony:</td><td><b>%s</b></td></tr>",  formatNumber(userDeaths[target]));
	iLen += format(gText[iLen], sizeText, "<tr><td>Punkty:</td><td><b>%d</b></td></tr>", userPoints[target]);
	iLen += format(gText[iLen], sizeText, "<tr><td>Resety:</td><td><b>%d</b></td></tr>", userReset[target]);
	iLen += format(gText[iLen], sizeText, "<tr><td>Calkowite Obrazenia:</td><td><b>%s</b></td></tr>", formatNumber(userAllDmg[target]));
	
	iLen += format(gText[iLen], sizeText, "<tr><td>Kolor Hud:</td><td><b style=^"color: rgb(%d,%d,%d); text-shadow: 0 0 5px rgb(%d,%d,%d)^">%d,%d,%d</b></td></tr>", userHud[target][PLAYER_HUD_RED], userHud[target][PLAYER_HUD_GREEN], userHud[target][PLAYER_HUD_BLUE],userHud[target][PLAYER_HUD_RED], userHud[target][PLAYER_HUD_GREEN], userHud[target][PLAYER_HUD_BLUE],userHud[target][PLAYER_HUD_RED], userHud[target][PLAYER_HUD_GREEN], userHud[target][PLAYER_HUD_BLUE]);
	
	iLen += format(gText[iLen], sizeText, "</table>")	;
	iLen += format(gText[iLen], sizeText, "</html>");
	
	if(id != target) ColorChat(target, GREEN, "%s Gracz^x03 %s^x01 przeglada twoje statystyki!", PREFIXSAY, userName[id]);
	
	show_motd(id, gText, formatm("Informacje o grcazu: %s",userName[target]));
	return PLUGIN_CONTINUE;
}
public checkForAward(id){
	
	if( !playerLogged(id)){
		ColorChat(id, GREEN, "%s Zaloguj sie aby odebrac nagrode!", PREFIXSAY);
		return PLUGIN_CONTINUE;
	}
	
	new newAwardLeft = userLastAwardTime[id] + userAwardTime - playedTime(id);
	
	if( userLastAwardTime[id] + userAwardTime > userTime[id] && newAwardLeft > 0){
		ColorChat(id, GREEN, "%s Jeszcze nie mozna odebrac nagrody! Nagroda za^x04 %d:%s%d:%s%d", PREFIXSAY, ( newAwardLeft / HOUR ), ( newAwardLeft / MINUTE % MINUTE )<10?"0":"", ( newAwardLeft / MINUTE % MINUTE ), (newAwardLeft%MINUTE)<10?"0":"", ( newAwardLeft %MINUTE ));
		return PLUGIN_CONTINUE;
	}
	
	new bool:empty	=	true;
	
	new gText[128], iLen;
	logType[id] = LOG_AWARD;
	if(logType[id] == LOG_AWARD){
		if( isVip(id)){
			
			switch(random_num(1,6)){
				case 1:{
					
					new vipRandom = random_num(750,2000);
					addNuggetToFinal(id, vipRandom);
					ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 otrzymal^x04 %d Brylek^x01 z nagrody.", PREFIXSAY, userName[id],vipRandom);
					iLen += format(gText[iLen], sizeof(gText)-1-iLen,"dostal %d brylek z Nagrody!",vipRandom);
					empty	=	false;
				}
				case 2:{
					new Float:expVipRandom = random_float(200.0, 500.0);
					addExpToFinal(id, expVipRandom);
					ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 otrzymal^x04 %0.1f EXP'a^x01 z nagrody.", PREFIXSAY, userName[id],expVipRandom);
					iLen += format(gText[iLen], sizeof(gText)-1-iLen, "dostal %0.1f EXP'a z Nagrody!",expVipRandom);
					empty	=	false;
				}
				case 3:{
					new vipRandomVip = random_num(1,10);
					ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 otrzymal^x04 VIP'a^x01 na^x04 %d %s^x01 z nagrody!", PREFIXSAY, userName[id], vipRandomVip, vipRandomVip == 1 ? "dzien" : "dni" );
					timeVip[id]	= 	max( timeVip[id] + (DAY*vipRandomVip), get_systime() + (DAY*vipRandomVip) );
					empty	=	false;
				}
				case 4:{
					new Float:fOrigin[3];
					pev(id, pev_origin, fOrigin);
					
					ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 otrzymal^x04 Skrzynke^x01 z nagrody.", PREFIXSAY, userName[id]);
					createCases(fOrigin, .disappear = 1, .owner = id);
					empty	=	false;
				}
				case 5:{
					new vipTimeScrool = (HOUR*random_num(1,5));
					new newLeftExp  =  userScrollExp[id] - playedTime(id);
					if(newLeftExp <= 0){
						userScrollExp[id] = playedTime(id);
						userScrollExp[id] += vipTimeScrool;
					} else userScrollExp[id] += vipTimeScrool;	
					
					ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 otrzymal^x04 Zwoj Doswiadczenia^x01 na^x04 %d %s^x01 z nagrody!", PREFIXSAY, userName[id], (vipTimeScrool/HOUR), (vipTimeScrool/HOUR) == 1  ? "godzine" : (vipTimeScrool/HOUR) ==  5 ? "godzin" : "godziny" );
					empty	=	false;
				}
				case 6:{
					new vipTimeScrool = (HOUR*random_num(1,5));
					new newLeftNugget  =  userScrollNugget[id] - playedTime(id);
					if(newLeftNugget <= 0){
						userScrollNugget[id] = playedTime(id);
						userScrollNugget[id] += vipTimeScrool;	
					} else userScrollNugget[id] += vipTimeScrool;
					
					ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 otrzymal^x04 Zwoj Szczescia^x01 na^x04 %d %s^x01 z nagrody!", PREFIXSAY, userName[id], (vipTimeScrool/HOUR), (vipTimeScrool/HOUR) == 1  ? "godzine" : (vipTimeScrool/HOUR) ==  5 ? "godzin" : "godziny" );
					empty	=	false;
				}
				
			}
		}else{	
			switch(random_num(1,7)){
				case 1:{
					new userRandom = random_num(500,1000);
					addNuggetToFinal(id, userRandom);
					ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 otrzymal^x04 %d Brylek^x01 z nagrody.", PREFIXSAY, userName[id],userRandom);
					iLen += format(gText[iLen], sizeof(gText)-1-iLen, "dostal %d brylek z Nagrody!",userRandom);
					empty	=	false;
				}
				case 2:{
					new Float:expRandom = random_float(100.0, 300.0);
					addExpToFinal(id, expRandom)
					ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 otrzymal^x04 %0.1f EXP'a^x01 z nagrody.", PREFIXSAY, userName[id],expRandom);
					iLen += format(gText[iLen], sizeof(gText)-1-iLen, "dostal %0.1f EXP'a z Nagrody!",expRandom);
					empty	=	false;
				} 
				case 3:{
					new userRandomVip = random_num(1,5);
					ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 otrzymal^x04 VIP'a^x01 na^x04 %d %s^x01 z nagrody!", PREFIXSAY, userName[id], userRandomVip, userRandomVip == 1 ? "dzien" : "dni" );
					timeVip[id]	= 	max( timeVip[id] + (DAY*userRandomVip), get_systime() + (DAY*userRandomVip) );
					empty	=	false;
				}
				case 4:{
					new Float:fOrigin[3];
					pev(id, pev_origin, fOrigin);
					
					ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 otrzymal^x04 Skrzynke^x01 z nagrody.", PREFIXSAY, userName[id]);
					createCases(fOrigin, .disappear = 1, .owner = id);
					empty	=	false;
				}
				case 5:{
					new userTimeScrool = (HOUR*random_num(1,3))
					new newLeftExp  =  userScrollExp[id] - playedTime(id);
					if(newLeftExp <= 0){
						userScrollExp[id] = playedTime(id);
						userScrollExp[id] += userTimeScrool;	
					} else userScrollExp[id] += userTimeScrool;
					
					ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 otrzymal^x04 Zwoj Doswiadczenia^x01 na^x04 %d %s^x01 z nagrody!", PREFIXSAY, userName[id], (userTimeScrool/HOUR), (userTimeScrool/HOUR) == 1  ? "godzine" : "godziny" );
					empty	=	false;
				}
				case 6:{
		
					new userTimeScrool = (HOUR*random_num(1,3));
					new newLeftNugget  =  userScrollNugget[id] - playedTime(id);
					if(newLeftNugget <= 0){
						userScrollNugget[id] = playedTime(id);
						userScrollNugget[id] += userTimeScrool;
					} else userScrollNugget[id] += userTimeScrool;
					
					ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 otrzymal^x04 Zwoj Szczescia^x01 na^x04 %d %s^x01 z nagrody!", PREFIXSAY, userName[id], (userTimeScrool/HOUR), (userTimeScrool/HOUR) == 1  ? "godzine" : "godziny" );
					empty	=	false;
				}
			}
		}
		if( empty ){
			ColorChat(id, GREEN, "%s Ojejku :( Nic nie dostales.", PREFIXSAY);
			iLen += format(gText[iLen], sizeof(gText)-1-iLen, "nic nie dostal z Nagrody!");
		}
		logBB(id, gText);
	}
	userAllAward[id] ++;
	userLastAwardTime[id]=playedTime(id);
	return PLUGIN_CONTINUE;
}
public playerRandomInfo(){
	new listPlayer[33];
	new iNum = 0;
	for( new i = 1; i < maxPlayers; i ++ ){
		if( !is_user_connected(i) || is_user_hltv(i) ) continue;
		listPlayer[iNum++] = i;
	}
	if( iNum > 0 ){
		new target = listPlayer[random(iNum)];
		switch(random(6)){
			case 0: ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 zabil juz lacznie^x03 %d^x01 osob!", PREFIXSAY, userName[target], userKills[target]);
			case 1: ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 zginal juz lacznie^x03 %d^x01 razy!", PREFIXSAY, userName[target], userDeaths[target]);
			case 2: ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 przegral juz lacznie^x03 %dgodz %s%dmin %s%dsek^x01!", PREFIXSAY, userName[target],playedTime(target)/HOUR, (playedTime(target)/MINUTE)<10?"0":"",playedTime(target)/MINUTE%MINUTE, playedTime(target)%MINUTE<10?"0":"", playedTime(target)%MINUTE);
			case 3: ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 zadal lacznie^x03 %d^x01 obrazen!", PREFIXSAY, userName[target], userAllDmg[target]);
			case 4: ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 odebral juz lacznie^x03 %d^x01 nagrod!", PREFIXSAY, userName[target], userAllAward[target]);
			case 5: ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 posiada^x03 %d^x01 punktow!", PREFIXSAY, userName[target], userPoints[target]);
		}
	}	
	set_task(random_float(60.0, 120.0), "playerRandomInfo");	
}

public fw_SetClientListening(iReceiver, iSender, bool:bListen){
	if (get_systime() < userMute[iSender]/* || get_systime() < userMute[iReceiver]*/ || userMutePlayer[iReceiver][iSender]){
		engfunc(EngFunc_SetClientListening, iReceiver, iSender, false);
		forward_return(FMV_CELL, false);
		return FMRES_SUPERCEDE;
	}
	engfunc(EngFunc_SetClientListening, iReceiver, iSender, true);
	forward_return(FMV_CELL, true);
	return FMRES_SUPERCEDE;
}
public adminHelpPush(id){
	
	id -= TASK_PUSH;
	
	if(!userPush[id]) return;
		
	new Float:fOrigin[3], Float:fOriginTarget[3], iOrigin[3], Float:fVelocity[3];	
	entity_get_vector(id, EV_VEC_origin, fOrigin);
		
	for( new i = 1; i < maxPlayers; i ++ ){
		
		if( i == id ) continue;
		if(isAdmin(i)) continue;
		if(buildTime || prepTime) continue;
		if(!is_user_alive(i) || get_user_team(i) != 1)  continue;
			
		entity_get_vector(i, EV_VEC_origin, fOriginTarget);

		if( get_distance_f(fOriginTarget, fOrigin) >= 200.0) continue;
			
		get_user_origin(id, iOrigin, 2);
		IVecFVec(iOrigin, fOrigin);
		entity_get_vector(id, 	EV_VEC_origin, 	fOrigin);
		entity_get_vector(i, 	EV_VEC_origin, 	fOriginTarget);
		fOriginTarget[2] = fOrigin[2];
		xs_vec_sub(fOrigin, fOriginTarget, fVelocity);
		xs_vec_normalize( fVelocity , fVelocity );		
		xs_vec_mul_scalar( fVelocity , -700.0 , fVelocity );
		fVelocity[2] *= 1.5;
		entity_set_vector(i, 	EV_VEC_velocity, 	fVelocity);
	}
	set_task(0.3, "adminHelpPush", id + TASK_PUSH);
}

public ChatOff(id){
	if(!isSuperAdmin(id)) return;
	serverOffChat =! serverOffChat;
}



public bloodSpriteGo(attacker, victim){
	static const Float:fVec[][3]={
		{0.0,0.0,0.0},
		{0.0,0.0,0.0},
		{0.0,0.0,25.0},
		{0.0,0.0,10.0},
		{5.0,5.0,10.0},
		{-5.0,-5.0,10.0},
		{5.0,5.0,-10.0},
		{-5.0,-5.0,-10.0}
	};
	
	new Float:fOrigin[3];
	pev(victim,pev_origin,fOrigin);
	
	new hitpoint, weapon;
	get_user_attacker(victim,weapon,hitpoint);
	
	xs_vec_add(fOrigin,fVec[hitpoint],fOrigin);

}


public createNugget(id, id2){
	new Float:fOrigin[3];
	pev(id2, pev_origin, fOrigin);
	
	for(new i = 0; i < random(5); i ++){
		switch(random(100)){
			case 0: 		createNuggetOrigin(fOrigin, 1,1, BLACK_NUGGET, 	.owner=id)		
			case 1..5: 	createNuggetOrigin(fOrigin, 1,1, PINK_NUGGET, 	.owner=id)		
			case 6..16: 	createNuggetOrigin(fOrigin, 1,1, BLUE_NUGGET, 	.owner=id)		
			case 17..32:	createNuggetOrigin(fOrigin, 1,1, YELLOW_NUGGET, 	.owner=id)		
			case 33..53:	createNuggetOrigin(fOrigin, 1,1, GREEN_NUGGET, 	.owner=id)		
			default:	createNuggetOrigin(fOrigin, 1,1, RED_NUGGET, 	.owner=id)	
		}
	}
}
public nuggetThink(ent){
	if( !pev_valid(ent) ) return;
		
	engfunc( EngFunc_SetSize, ent , Float:{-16.0, -16.0, -16.0}, Float:{16.0,16.0,16.0});
	new Float:fAngles[3];
	pev(ent, pev_angles, fAngles);
	fAngles[1] += pev(ent, pev_fuser2);
	
	set_pev(ent, pev_angles, fAngles);
	if( get_gametime() - pev(ent, pev_fuser3) > delayEffectNugget[pev(ent, pev_body)] ){
		new Float:fOrigin[3];
		pev(ent, pev_origin, fOrigin);
		fOrigin[2] += 20.0;
		
		if( get_gametime() - pev(ent, pev_fuser1) > 3.0 ){
			if( pev(ent, pev_flags) & FL_ONGROUND ){
				new Float:fVelocity[3];
				pev(ent, pev_velocity, fVelocity);
				fVelocity[2] = random_float(45.0, 65.0);			
				//set_pev(ent, pev_velocity, fVelocity)
			}
		}
		set_pev(ent, pev_fuser3, get_gametime());
	}
	if( get_gametime() - pev(ent, pev_fuser1) > 10.0 ){
		
		listNuggetOnFloor[pev(ent, pev_iuser1)] = 0;
		if( ent!= 0 ) remove_entity(ent);
		
	}else set_pev(ent, pev_nextthink, get_gametime()+0.1)	
}
createNuggetOrigin(Float:fOrigin[3], minN, maxN, type, owner = 0){

	new Float:fVelocity[3];
			
	new ent = -1;
	for( new i = 0 ; i < random_num(minN,maxN); i ++ ){		
		ent = addNuggetEnt();
		if( ent == 0) return;
	
		set_pev(ent, pev_classname, "nugget");
		
		set_pev(ent, pev_solid, SOLID_TRIGGER);
		set_pev(ent, pev_movetype, MOVETYPE_TOSS);
		set_pev(ent, pev_fuser1, get_gametime() );
		set_pev(ent, pev_fuser2, random_float(1.0, 50.0));
		set_pev(ent, pev_fuser3, get_gametime());
		
		set_pev(ent, pev_iuser3, owner);
		set_pev(ent, pev_skin, type);

		set_pev(ent, pev_origin, fOrigin);
		engfunc( EngFunc_SetSize, ent , Float:{-16.0, -16.0, -16.0}, Float:{16.0,16.0,16.0});
		
		fVelocity[0] = random_float(-150.0, 150.0);
		fVelocity[1] = random_float(-150.0, 150.0);
		fVelocity[2] = random_float(100.0, 300.0);
		set_pev(ent, pev_velocity, fVelocity);


		set_rendering(ent, kRenderFxGlowShell, 	colorNugget[type][0], colorNugget[type][1] ,     colorNugget[type][2], 	kRenderNormal, 5);

		entity_set_model(ent, modelNuggetDrop);
		set_pev(ent, pev_nextthink, get_gametime()+0.1);
		
		if(is_user_connected(owner)){
			message_begin(MSG_ONE, SVC_TEMPENTITY, _ , owner);
			write_byte(22);
			write_short(ent);
			write_short(thunder);
			write_byte(5); 
			write_byte(8); 
			write_byte(colorNugget[type][0]);
			write_byte(colorNugget[type][1]); 
			write_byte(colorNugget[type][2]); 
			write_byte(75);
			message_end();
		}
	}
}
public findCoinArround(id){
	
	new Float:fOrigin[3];
	new Float:fOriginCoin[3];
	new pickUp = false;
	
	pev(id, pev_origin, fOrigin);
	new ent = 0;
	new value = 0;
	
	for( new i = 0 ; i < MAXNUGGETSFLOOR; i ++ ){
		ent = listNuggetOnFloor[i];
		if(!pev_valid(ent)) continue;		
		if(get_gametime() - pev(ent, pev_fuser1) < random_float(1.50,2.50)) continue;
		
	
		if(teamWorks(id)){
			id = userTeam[id];
			
			if(pev(ent, pev_iuser3) != id) continue;
		}
		
		if(pev(ent, pev_iuser3) != id) continue;
			
		pev(ent, pev_origin, fOriginCoin);
		
		if(!pickUp){
			if( get_distance_f(fOrigin, fOriginCoin) > 155.0){
					
				if( get_gametime() - pev(ent, pev_fuser1) < random_float(1.50,2.50) ) continue;
				if( get_distance_f(fOrigin, fOriginCoin)  < 300.0 ){
					new Float:fVeloc[3];
					xs_vec_sub(fOrigin, fOriginCoin, fVeloc)				
					xs_vec_normalize( fVeloc , fVeloc );				
					xs_vec_mul_scalar( fVeloc , 300.0 , fVeloc );
					set_pev(ent, pev_velocity, fVeloc);
					
					
				} continue;
			}
		}
		
		switch(pev(ent, pev_skin)){
			case RED_NUGGET:  {
				value = random_num(1,3);
				tutorBB(id, TUTOR_YELLOW, "%dx Czerwona Brylka", value);
			}
			
			case GREEN_NUGGET: {
				value = 4;
				tutorBB(id, TUTOR_YELLOW, "%dx Zielona Brylka", value);
			}
			case YELLOW_NUGGET: {
				value = 5;
				addSecretMission(id, mission_secret_GOLDNUGGET, value)
				
				#if defined CHRISTMAS_ADDON
		
					addChristmasMission(id, CH_PICKUPGOLD, 1);
				
				#endif
				tutorBB(id, TUTOR_YELLOW, "%dx Zolta Brylka", value);
				
			}
			case BLUE_NUGGET:{
				if(userNugget[id] == 0 ) addSecretMission(id, mission_secret_BLUE, 1)
				value = 6;
				tutorBB(id, TUTOR_YELLOW, "%dx Niebieska Brylka", value);
			}
			case PINK_NUGGET:  {
				value = 9;
				tutorBB(id, TUTOR_YELLOW, "%dx Rozowa Brylka", value);
			}
			case BLACK_NUGGET:  {
				value = random_num(12,28);
				addSecretMission(id, mission_secret_LUCK, 1);
				#if defined CHRISTMAS_ADDON
		
					addChristmasMission(id, CH_BLACKNUGGET, 1);
				
				#endif
				tutorBB(id, TUTOR_YELLOW, "%dx Czarna Brylka", value);
			}
		}
		
		
		if(foundedHat(id, HAT_AUREOLA)) {
			new Float:valRound = 0.0;
			valRound = float(value);
			valRound *= 1.5;
			value = floatround(valRound);
			
		}
	
		
		addKillNugget(id, value);
		
		#if defined CHRISTMAS_ADDON
		
			addChristmasMission(id, CH_PICKUP, 1);
		
		#endif
		
		spkGameSound(id, sound_PICKUP);
		listNuggetOnFloor[pev(ent, pev_iuser1)] = 0
		remove_entity(ent);
	}
}

public addNuggetEnt(){
	new slotCoin = freeNgugetSlot();
	if( slotCoin!=-1){
		new ent = create_entity("info_target")
		if( !pev_valid(ent) ) return 0;
		listNuggetOnFloor[slotCoin] = ent;
		set_pev(ent, pev_iuser1, slotCoin);
		return ent;
	}
	return 0
}
public freeNgugetSlot(){
	for( new i = 0 ; i < MAXNUGGETSFLOOR; i ++ ){
		if( listNuggetOnFloor[i] == 0 ) return i;
	}
	return -1;
}

new userTutorMsg[33][181];
new Float:userLastTutor[33];
public tutorBB(id, TutorColor:colorTUTOR, const textTUTOR[], any:...){	

	new szMessage[181], gText[181], iLen = 0;
	vformat(szMessage, sizeof(szMessage) - 1, textTUTOR, 4);
	
	szMessage[180] = '^0';
	
	if(userTutorMsg[id][0]) iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "%s^n", userTutorMsg[id]);
	iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "%s", szMessage);
	
	userTutorMsg[id] = gText;
	
	if(get_gametime() - userLastTutor[id] < 2.0 ){
		if(task_exists( id + TASK_TUTORFINAL ))
			remove_task( id + TASK_TUTORFINAL);
	}
	userLastTutor[id] = get_gametime();
	
	if(strlen(gText) < 180) tutorMake(id, colorTUTOR, 2.0, "%s", gText);
	else {
		tutorBB_Final(id + TASK_TUTORFINAL);
		tutorMake(id, colorTUTOR, 2.0, "%s", szMessage);
	}
	
	set_task(2.0, "tutorBB_Final", id+ TASK_TUTORFINAL);
}
public tutorBB_Final(id){
	id -= TASK_TUTORFINAL;
	userTutorMsg[id] = "";
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
