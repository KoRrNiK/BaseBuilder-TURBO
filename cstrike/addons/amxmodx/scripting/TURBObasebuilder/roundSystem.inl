#include <amxmodx>
#include <amxmisc>

public timerStart(){
	if(OX[OX_START]) return PLUGIN_CONTINUE;
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
	
	nextColorsHappy = (nextColorsHappy+1)%sizeof(colorsHappy);	
	
	set_dhudmessage(hourTime ? colorsHappy[nextColorsHappy][0] : 255, hourTime ? colorsHappy[nextColorsHappy][1] : 102, hourTime ? colorsHappy[nextColorsHappy][2] : 0, -1.0, 0.0, 0, 0.5, 0.9, 0.5, 0.5);
	show_dhudmessage(0, "%s",gText);
	
	
	if(hourTime && ( buildTime || prepTime ) ) happyColorBarrier();
	
	set_task(1.0, "timerStart", TASK_CLOCK);
	return PLUGIN_CONTINUE;
}

public newRound(){
	new ret;
	ExecuteForward(bbForward[forwardNewRound], ret);
}
public round_start(){
	gameStart();	
	resetBlocks();
	removeSkill();	
	
	new ret;
	ExecuteForward(bbForward[forwardStartRound], ret);
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
	
	new ret;
	ExecuteForward(bbForward[forwardStartGame], ret);	
	
}
public startNewRound(){	
	
	if( task_exists(TASK_CLOCK) )
		remove_task(TASK_CLOCK);

	resetBlocks();

	new ct = numPlayers(2, false);
	new tt = numPlayers(1, false);
	
	if( ct <= 0 || tt <= 0 || (ct + tt ) <= 1 ) return 0;
	
	startBuild();
	timerStart();
		
	freeChestCreate();
	
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
	
	new ret;
	ExecuteForward(bbForward[forwardStartBuild], ret);
}

public startRelease(){
	gTime = gGameTime;
	gameTime = true;
	buildTime = false;
	prepTime = false;
	
	for(new i = 1; i < maxPlayers; i ++){
		if(!is_user_connected(i) || is_user_hltv(i) || is_user_bot(i)) continue;
			
		Display_Fade(i,1024,1024,1024,0, 0, 0, 180);
	}
	

	set_dhudmessage(255, 16, 255, -1.0, 0.20, 0, 0.5, 1.0, 0.5, 0.5);
	show_dhudmessage(0,  "^n!! RUNDA WYSTARTOWALA !!");
	
	
	removeNotUsedBlock();
	
	new ret;
	ExecuteForward(bbForward[forwardStartRelease], ret);

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
	
	new ret;
	ExecuteForward(bbForward[forwardStartPrep], ret);

}

public round_End(){
	gameEnd();
	
	new ret;
	ExecuteForward(bbForward[forwardEndRound], ret);
	
}
public gameEnd(){
	
	roundEnd = true;
	gTime = 0;
	
	new ct 		= 	numPlayers(2, false);
	new ctAlive 	= 	numPlayers(2, true);
	new tt 		= 	numPlayers(1, false);

	for(new i = 1; i < maxPlayers; i ++){
		if( !is_user_connected(i) || is_user_hltv(i) || is_user_bot(i)) continue;
			
		deathPlayerWin(i);
		resetPriceDefault(i);
	}
	
	if( roundGood ){
		if( ctAlive > 0 ){
	
			set_dhudmessage(16, 16, 255, -1.0, 0.20, 0, 0.5, 3.0, 0.5, 0.5);
			show_dhudmessage(0,  "^n!! BUDOWNICZY WYGRALI !!");
			
			
			spkGameSound(0, sound_WINCT);
			
			for(new i = 1; i < maxPlayers; i ++){
				if(!is_user_connected(i) || is_user_hltv(i) || is_user_bot(i)) continue;
					
				Display_Fade(i,(1<<12),(1<<12),(1<<12),0, 0, 0, 180);
			}
		
			
			for(new i = i; i < maxPlayers; i ++){
				if(get_user_team(i) == 2){
					
					addMission(i, mission_SURVIVOR, 1);
						
					new Float:randomExp = 0.0;
					new randomNugget	= 0;
						
					if(isSVip(i)){
						randomExp = random_float(bbCvar[cvarExpForWillSurviveSVip]/2, bbCvar[cvarExpForWillSurviveSVip]);
						randomNugget = random_num(bbCvar[cvarNuggetForWillSurviveSVip]/2, bbCvar[cvarNuggetForWillSurviveSVip]);
					} else if(isVip(i)){
						randomExp = random_float(bbCvar[cvarExpForWillSurviveVip]/2, bbCvar[cvarExpForWillSurviveVip]);
						randomNugget = random_num(bbCvar[cvarNuggetForWillSurviveVip]/2, bbCvar[cvarNuggetForWillSurviveVip]);
					} else {
						randomExp =random_float(bbCvar[cvarExpForWillSurvive]/2, bbCvar[cvarExpForWillSurvive]);
						randomNugget = random_num(bbCvar[cvarNuggetForWillSurvive]/2, bbCvar[cvarNuggetForWillSurvive]);
					}
						
					addExpToFinal(i, randomExp);
					addNuggetToFinal(i, randomNugget);
						
					chatPrint(i, PREFIX_NORMAL, "Otrzymales za przetrwanie rundy^4 |^1 Brylek:^3 %d^4 /^1 Expa:^3 %0.1f", randomNugget, randomExp);
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
				chatPrint(0, PREFIX_LINE, "Gracz^3 %s^1 zdobyl najwiecej^3 brylek^1 w tej rundzie!^3 [^4 %d^3 ]", userName[idMax], iCoinCollected);
			}
			for( new i = 1; i < maxPlayers; i++ ){
				if( !is_user_connected(i) || is_user_hltv(i)) continue;
				cs_set_user_team(i, userRoundTeam[i] == 1 ? 2 : 1 );
			}
			tt = numPlayers(1, false);
			ct = numPlayers(2, false);
			new dif = abs(ct-tt);
			if( dif > 1 ){			
				new lookFor = ct>tt ? 2 : 1;
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
	new ret;
	ExecuteForward(bbForward[forwardEndGame], ret);
}

public restartRound(){
	new ret;
	ExecuteForward(bbForward[forwardRestartRound], ret);
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
	if (!equal(menu_text_code, team_select)) return PLUGIN_CONTINUE;
	forceTeam(id, msgid);
	return PLUGIN_HANDLED;
}

public messageVGUIMenu(msgid, dest, id) {
	if (get_msg_arg_int(1) != 2) return PLUGIN_CONTINUE;
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
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
