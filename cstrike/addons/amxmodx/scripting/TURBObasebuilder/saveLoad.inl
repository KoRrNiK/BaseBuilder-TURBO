#include <amxmisc>
#include <fvault>

public loadInt(id){
	
	userLogged[id] = false;
	userLoaded[id] = false;
	userLoadVault[id] = false;
	
	loadStatsSql(id, 0)
	
	// loadData(id) /* Jest juz w loadStatsSql */
}
public fVaultLoad(id){
	
	new day;
	date(_,_, day);
	
	new szData[756];
	if( fvault_get_data(fVAULTFILE, userName[id], szData, sizeof(szData) - 1) ){
		
		new szOption[5]
		new szAllNugget[11]
		new szTimeVip[11]
		new szLuzCoin[11]
		new szClassHuman[10]
		new szMission[mission_TOTAL*7]
		new szMissionOne[mission_TOTAL][7];
		new szMissionSecret[mission_secret_TOTAL*7]
		new szMissionSecretOne[mission_secret_TOTAL][7];
		new szPro[TOTAL_PRO*5]
		new szProOne[TOTAL_PRO][5]
		new szProSelected[3]
		new szHumanSelectNew1[3]
		new szHumanSelectNew2[3]
		new szZombieSelectNew1[3]
		new szZombieSelectNew2[3]
		new szExpGiveClass[5];
		new szDayFreeNugger[3];
		new szJetpackSpeed[7];
		new szColorRendering[3];
		new szColorRed[4], szColorGreen[4], szColorBlue[4];
		new szTimeStamina[10]
		new szStaminaDayRefresh[10];
		new szUpgradeMine[up_TOTAL*7]
		new szUpgradeMineOne[up_TOTAL][7]
		new szDigging[mine_TOTAL*7]
		new szDiggingOne[mine_TOTAL][7]
		new szTypeMine[10];
		new szMineNugget[10]
		new szPayDayGoblin[10]
		new szLastUpgradeTime[7]
		new szUnlockCave[3]
		new szStartUpgrade[3];
		new szAwardTime[7];
		new szAllAward[7];
		new szAllDmg[11];
		new szFov[4]
		new szScrollExp[10];
		new szScrollNugget[10];
		new szBoostMine[10];
		new szMute[11]
		new szMaxHelp[3]
		new szHelpDay[3];
		new szHelpPointAdmin[5];
		new szPointSecret[7]
		new szLastAwardGot[11]
		new szLastAwardFree[11]
		new szLastAwardRow[7]
		new szChristmasStart[5];
		new szChristmasMission[5]
		new szChristmasType[5];
		new szHat[4][11];
		new szSelectHat[2][3];
		
		parse(szData,
			szLuzCoin,		sizeof(szLuzCoin),
			szTimeVip,		sizeof(szTimeVip),
			szOption,		sizeof(szOption),
			szAllNugget,		sizeof(szAllNugget),
			szClassHuman, 		sizeof(szClassHuman),
			szMission, 		sizeof(szMission),
			szMissionSecret,	sizeof(szMissionSecret),
			szPro,			sizeof(szPro),
			szProSelected,		sizeof(szProSelected),
			szHumanSelectNew1,	sizeof(szHumanSelectNew1),
			szHumanSelectNew2,	sizeof(szHumanSelectNew2),
			szZombieSelectNew1,	sizeof(szZombieSelectNew1),
			szZombieSelectNew2,	sizeof(szZombieSelectNew2),
			szExpGiveClass,		sizeof(szExpGiveClass),
			szDayFreeNugger, 	sizeof(szDayFreeNugger),
			szColorRed,		sizeof(szColorRed),
			szColorGreen,		sizeof(szColorGreen),
			szColorBlue,		sizeof(szColorBlue),
			szJetpackSpeed,		sizeof(szJetpackSpeed),
			szColorRendering,	sizeof(szColorRendering),
			szTimeStamina,		sizeof(szTimeStamina),
			szStaminaDayRefresh,	sizeof(szStaminaDayRefresh),
			szUpgradeMine,		sizeof(szUpgradeMine),
			szDigging,		sizeof(szDigging),
			szTypeMine,		sizeof(szTypeMine),
			szMineNugget,		sizeof(szMineNugget),
			szPayDayGoblin,		sizeof(szPayDayGoblin),
			szLastUpgradeTime,	sizeof(szLastUpgradeTime),
			szUnlockCave,		sizeof(szUnlockCave),
			szStartUpgrade,		sizeof(szStartUpgrade),
			szAwardTime,		sizeof(szAwardTime),
			szAllAward,		sizeof(szAllAward),
			szAllDmg,		sizeof(szAllDmg),
			szFov,			sizeof(szFov),
			szScrollExp,		sizeof(szScrollExp),
			szScrollNugget,		sizeof(szScrollNugget),
			szBoostMine,		sizeof(szBoostMine),
			szMute,			sizeof(szMute),
			szMaxHelp,		sizeof(szMaxHelp),
			szHelpDay,		sizeof(szHelpDay),
			szHelpPointAdmin,	sizeof(szHelpPointAdmin),
			szPointSecret,		sizeof(szPointSecret),
			szLastAwardGot,		sizeof(szLastAwardGot),
			szLastAwardFree,	sizeof(szLastAwardFree),
			szLastAwardRow,		sizeof(szLastAwardRow),
			szChristmasStart,	sizeof(szChristmasStart),
			szChristmasMission,	sizeof(szChristmasMission),
			szChristmasType,	sizeof(szChristmasType),
			szHat[0],		sizeof(szHat[]),
			szHat[1],		sizeof(szHat[]),
			szHat[2],		sizeof(szHat[]),
			szHat[3],		sizeof(szHat[]),
			szSelectHat[0],		sizeof(szSelectHat[]),
			szSelectHat[1],		sizeof(szSelectHat[])
		
			
		);
		timeVip[id] 			= 	str_to_num(szTimeVip);
		userSaveOption[id]		=	str_to_num(szOption);
		userNuggetAll[id]		=	str_to_num(szAllNugget);
		if(timeVip[id] 			> 	get_systime()) userVip[id] = true
		userLuzCoin[id]			= 	str_to_num(szLuzCoin);
		
		userHuman[id]		 = 	str_to_num(szClassHuman)

		explode(szMission	,'_'	,szMissionOne	,sizeof(szMissionOne)	,sizeof(szMissionOne[]))
		for(new i = 0 ; i < sizeof(szMissionOne); i ++ )
			userMission[id][i] = str_to_num(szMissionOne[i])
			
		explode(szMissionSecret	,'_'	,szMissionSecretOne	,sizeof(szMissionSecretOne)	,sizeof(szMissionSecretOne[]))
		for(new i = 0 ; i < sizeof(szMissionSecretOne); i ++ )
			userMissionSecret[id][i] = str_to_num(szMissionSecretOne[i])	
			
		
		userProSelected[id]			=	str_to_num(szProSelected)
		userClassHuman[id]			=	str_to_num(szHumanSelectNew1)
		userNewClassHuman[id]			=	str_to_num(szHumanSelectNew2)
		userNewClass[id]			=	str_to_num(szZombieSelectNew1)
		userClass[id]				=	str_to_num(szZombieSelectNew2)
		userGiveClassExp[id]			=	str_to_num(szExpGiveClass)
		userLastDay[id]				=	str_to_num(szDayFreeNugger);
		userHud[id][PLAYER_HUD_RED] 		= 	str_to_num(szColorRed);
		userHud[id][PLAYER_HUD_GREEN] 		= 	str_to_num(szColorGreen);
		userHud[id][PLAYER_HUD_BLUE] 		= 	str_to_num(szColorBlue);
		userJetpackSpeed[id]			=	str_to_num(szJetpackSpeed);
		userMoverBlockColor[id]			=	str_to_num(szColorRendering);
		
		if(str_to_num(szHelpDay) != day){
			userMaxHelp[id]			=	bbCvar[cvarHelpMaxCount]
			userDayHelp[id]			=	day
		} else {
			userMaxHelp[id]			=	str_to_num(szMaxHelp)
			userDayHelp[id]			=	str_to_num(szHelpDay)
		}
		
		if( str_to_num(szStaminaDayRefresh) != day ){
			userLastStaminaTime[id] 		= 	playedTime(id)
			userStaminaDayRefresh[id]	= 	day
			userMinePayGoblin[id]		=	true;
		}else{			
			userLastStaminaTime[id] 		= 	str_to_num(szTimeStamina)
			userStaminaDayRefresh[id] 	= 	str_to_num(szStaminaDayRefresh)
			userMinePayGoblin[id]		=	false;
		}
		
		explode(szPro		,'_'	,szProOne	,sizeof(szProOne)	,sizeof(szProOne[]))
		for(new i = 0 ; i < sizeof(szProOne); i ++ )
			userPro[id][i] = str_to_num(szProOne[i])
			
		explode(szUpgradeMine		,'_'	,szUpgradeMineOne	,sizeof(szUpgradeMineOne)	,sizeof(szUpgradeMineOne[]))
		for(new i = 0 ; i < sizeof(szUpgradeMineOne); i ++ )
			userUpgradeMine[id][i] = str_to_num(szUpgradeMineOne[i])
			
		explode(szDigging		,'_'	,szDiggingOne	,sizeof(szDiggingOne)	,sizeof(szDiggingOne[]))
		
		for(new i = 0 ; i < sizeof(szDiggingOne); i ++ )
			userDigging[id][i] = str_to_num(szDiggingOne[i])		
		
		userTypeMine[id]			=	str_to_num(szTypeMine);
		userMineNugget[id]			=	str_to_num(szMineNugget);
		userLastUpgradeTime[id]			=	str_to_num(szLastUpgradeTime);
		unlockCave[id]				=	str_to_num(szUnlockCave);
		startUpgrade[id]			=	str_to_num(szStartUpgrade);
		
		userLastAwardTime[id] 			=	str_to_num(szAwardTime);
		
		userAllAward[id]			=	str_to_num(szAllAward);
		userAllDmg[id]				=	str_to_num(szAllDmg);
		
		userFov[id]				=	str_to_num(szFov);
				
		userScrollExp[id]			=	str_to_num(szScrollExp);
		userScrollNugget[id]			=	str_to_num(szScrollNugget);
		userBoostMine[id]			=	str_to_num(szBoostMine)
		
		userMute[id] 				=	str_to_num(szMute);
		
		userHelpPoint[id]			=	str_to_num(szHelpPointAdmin)
		
		userSecretPoint[id]			=	str_to_num(szPointSecret)
	
		userLastAwardGot[id]			=	str_to_num(szLastAwardGot);
		userLastAwardFree[id]			=	str_to_num(szLastAwardFree);
		userLastAwardRow[id] 			=	str_to_num(szLastAwardRow);
		
		userChristmasStart[id] 			=	str_to_num(szChristmasStart);
		userChristmasMission[id] 		=	str_to_num(szChristmasMission);
		userChristmasType[id] 			=	str_to_num(szChristmasType);
		
		for(new i = 0; i < 4; i ++) userHat[id][i]	=	str_to_num(szHat[i]);
		
		userSelectHat[id] 			=	str_to_num(szSelectHat[0]);
		userSelectNewHat[id] 			=	str_to_num(szSelectHat[1]);
		
		if(userSelectHat[id] == 0 || userSelectNewHat[id] == 0){
			userSelectHat[id] = -1;
			userSelectNewHat[id] = -1;
		}
		
		userLoadVault[id]			= 	true;
		
	}else{

		userNuggetAll[id]		=	0;
		timeVip[id] 			= 	0;
		userLuzCoin[id]			= 	0;
		userClassHuman[id]		=	0;
		userNewClassHuman[id]		=	0;
		userNewClass[id]		=	0;
		userClass[id]			=	0;
		userGiveClassExp[id]		=	0;
		userHuman[id] 			= 	0;
		
		for(new i = 0 ; i < sizeof(userHuman[]); i ++ ){
			if(str_to_num(classesHuman[human_FREE][2]) == 0 )
				userHuman[id] |= (1<<i)
		}

		for(new i = 0 ; i < sizeof(userMission[]); i ++ )
			userMission[id][i] = 0
		for(new i = 0 ; i < sizeof(userMissionSecret[]); i ++ )
			userMissionSecret[id][i] = 0
		for(new i = 0 ; i < sizeof(userPro[]); i ++ )
			userPro[id][i] = 0
			
		userProSelected[id]		=	-1	
	
		userLastDay[id] 			=	day;
		
		userHud[id][PLAYER_HUD_RED]	= 	255//120;
		userHud[id][PLAYER_HUD_GREEN] 	= 	0//255;
		userHud[id][PLAYER_HUD_BLUE] 	= 	60;
		userJetpackSpeed[id]		= 	500;
		userMoverBlockColor[id]		=	0;
		
		
		userLastStaminaTime[id] 		= 	playedTime(id)
		userStaminaDayRefresh[id]	= 	day
		userMinePayGoblin[id]		=	true;
		
		for(new i = 0 ; i < sizeof(userUpgradeMine[]); i ++ )
			userUpgradeMine[id][i] = 0
		
		for(new i = 0 ; i < sizeof(userDigging[]); i ++ )
			userDigging[id][i] = 0
				
		userTypeMine[id]		=	0;
		userMineNugget[id]		=	0;
		
		
		userLastUpgradeTime[id]		=	0
		unlockCave[id]			=	0
		startUpgrade[id]		=	0
		
		userLastAwardTime[id]		=	playedTime(id)
		
		addOption(userSaveOption[id], save_MODELS)
		addOption(userSaveOption[id], save_CAVE)
		
		userAllAward[id]		=	0;
		userAllDmg[id]			=	0;
		
		userFov[id]			=	90;
		
		userScrollExp[id]		=	playedTime(id);
		userScrollNugget[id]		=	playedTime(id);
		userBoostMine[id]		=	playedTime(id);
		
		userMute[id] 			= 	0;
		
		userHelpPoint[id]		=	0;
		
		userSecretPoint[id] = 0;
			
		userLastAwardGot[id]		=	playedTime(id);
		userLastAwardFree[id]		=	get_systime();
		userLastAwardRow[id] 		=	0;
		userLoadVault[id]		= 	true;
		
		userChristmasStart[id] 		=	0;
		userChristmasMission[id] 	=	0
		userChristmasType[id] 		=	0
		
		for(new i = 0; i < 4; i ++) userHat[id][i]	=	0
		
		userSelectHat[id] 		=	-1;
		userSelectNewHat[id] 		=	-1;

	}
	
	return PLUGIN_CONTINUE;
}
public fVaultSave(id){

	if(!userLoadVault[id]) return PLUGIN_CONTINUE;
	
	new szData[756];

	new iLen = 0

	new szMission[mission_TOTAL*7]
	for(new i = 0; i < mission_TOTAL; i ++)		iLen += format(szMission[iLen], sizeof(szMission)-iLen-1, "%d%s", userMission[id][i], i == mission_TOTAL-1?"":"_")
	
	iLen = 0;
	new szMissionSecret[mission_secret_TOTAL*7]
	for(new i = 0; i < mission_secret_TOTAL; i ++)	iLen += format(szMissionSecret[iLen], sizeof(szMissionSecret)-iLen-1, "%d%s", userMissionSecret[id][i], i == mission_secret_TOTAL-1?"":"_")
	
	iLen=0
	new szPro[TOTAL_PRO*7]
	for(new i = 0; i < TOTAL_PRO; i ++)			iLen += format(szPro[iLen], sizeof(szPro)-iLen-1, "%d%s", userPro[id][i], i == TOTAL_PRO-1?"":"_")

	iLen=0
	new szUpgradeMine[up_TOTAL*7]
	for(new i = 0; i < up_TOTAL; i ++)			iLen += format(szUpgradeMine[iLen], sizeof(szUpgradeMine)-iLen-1, "%d%s",  userUpgradeMine[id][i], i == up_TOTAL-1?"":"_")
	
	iLen=0
	new szDigging[mine_TOTAL*7]
	for(new i = 0; i < mine_TOTAL; i ++)		iLen += format(szDigging[iLen], sizeof(szDigging)-iLen-1, "%d%s", userDigging[id][i], i == mine_TOTAL-1?"":"_")

	
	format(szData, sizeof(szData)-1,"%d %d %d %d %d %s %s %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %s %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
		userLuzCoin[id],
		timeVip[id],
		userSaveOption[id],
		userNuggetAll[id],
		userHuman[id],
		szMission,
		szMissionSecret,
		szPro,
		userProSelected[id],
		userClassHuman[id],
		userNewClassHuman[id],
		userNewClass[id],
		userClass[id],
		userGiveClassExp[id],
		userLastDay[id],
		userHud[id][PLAYER_HUD_RED],
		userHud[id][PLAYER_HUD_GREEN],
		userHud[id][PLAYER_HUD_BLUE],
		userJetpackSpeed[id],
		userMoverBlockColor[id],
		userLastStaminaTime[id],
		userStaminaDayRefresh[id],
		szUpgradeMine,
		szDigging,
		userTypeMine[id],
		userMineNugget[id],
		userMinePayGoblin[id],
		userLastUpgradeTime[id],
		unlockCave[id],
		startUpgrade[id],
		userLastAwardTime[id],
		userAllAward[id],
		userAllDmg[id],
		userFov[id],
		userScrollExp[id],
		userScrollNugget[id],
		userBoostMine[id],
		userMute[id],
		userMaxHelp[id],
		userDayHelp[id],
		userHelpPoint[id],
		userSecretPoint[id],
		userLastAwardGot[id],
		userLastAwardFree[id],
		userLastAwardRow[id],
		userChristmasStart[id],
		userChristmasMission[id],
		userChristmasType[id],
		userHat[id][0],
		userHat[id][1],
		userHat[id][2],
		userHat[id][3],
		userSelectHat[id],
		userSelectNewHat[id]

		
	);
	fvault_set_data(fVAULTFILE, userName[id], szData);
	
	userLoadVault[id]	= false;
	
	return PLUGIN_CONTINUE
}
stock explode(const string[],const character,output[][],const maxs,const maxlen){
	new iDo = 0, len = strlen(string), oLen = 0;
	do{
		oLen += (1+copyc(output[iDo++],maxlen,string[oLen],character))
	}while(oLen < len && iDo < maxs)
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
