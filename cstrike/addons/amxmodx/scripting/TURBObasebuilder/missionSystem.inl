#include <amxmodx>
#include <amxmisc>
#include <cstrike>
#include <fun>
#include <hamsandwich>
#include <fakemeta>
#include <fakemeta_util>
#include <engine>

public completeMission(id){
	Display_Fade(id, 1024,1024,1024, 255, 64, 127, 90);
	set_dhudmessage(255, 64, 127, -1.0, 0.20, 0, 0.5, 3.0, 0.5, 0.5);
	show_dhudmessage(id,  "!! WYKONALES MISJE !!");
}
public addMission(id, mission, value){
	if( userMission[id][mission] != -1 ){
		new missionUser = userMission[id][mission];    
		userMission[id][mission] = min(userMission[id][mission]+value, str_to_num(missionDesc[mission][2]));
		if( str_to_num(missionDesc[mission][2]) == userMission[id][mission] && missionUser != userMission[id][mission] ) {      
			ColorChat(0, GREEN, "---^x01 Gracz^x03 %s^x01 wykonal misje^x03 %s^x01 ---", userName[id], missionDesc[mission][0] );
			
			completeMission(id);
		}
	}
}
public addSecretMission(id, mission, value){
	if( userMissionSecret[id][mission] != -1 ){
		new missionSecretUser = userMissionSecret[id][mission];    
		userMissionSecret[id][mission] = min(userMissionSecret[id][mission]+value, str_to_num(missionDescSecret[mission][2]));       
		if( str_to_num(missionDescSecret[mission][2]) == userMissionSecret[id][mission] && missionSecretUser != userMissionSecret[id][mission] ) {      
			ColorChat(0, GREEN, "---^x01 Gracz^x03 %s^x01 wykonal sekretna misje^x03 %c^x01 ---", userName[id], missionDescSecret[mission][0] );
			
			#if defined CHRISTMAS_ADDON
		
				addChristmasMission(id, CH_SECRET, 1);
				
			#endif
				
			completeMission(id);
		}
	}
}
public addPro(id, mission, val){
	if( userPro[id][mission] != -1){
		if( mission != userProSelected[id] ) return;
		userPro[id][mission] = min(userPro[id][mission]+val, str_to_num(proDesc[mission][4]));
	}
}
public bool:didPro(id, mission) return !!((userPro[id][mission] == -1));
public menuMissionAll(id){
	
	new gText[512], iLen;
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Misje^n");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r1.\w Podstawowe\d [Aktywne caly czas]");	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r2.\w Sekretne\d [Aktywne caly czas]");	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r3.\w Profesjonalne");	
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\dWykonujesz:");
	if( userProSelected[id] == -1 )
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\d- Nic nie wykonujesz");	
	else iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\d- %s\r [%d/%s]", proDesc[ userProSelected[id] ][0], userPro[id][userProSelected[id]], proDesc[ userProSelected[id] ][4] );
	
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\r0.\y Wyjdz");
	
	showMenu(id, B1 | B2 | B3 | B0 , gText, -1, "menuMissionAll"  );
}

public menuMissionAll_2(id, item){
	switch(item){
		case 0:{
			missionMenu[id] = MISSION_MENU_BASIC;
			menuMission(id);
		}
		case 1:{
			missionMenu[id] = MISSION_MENU_SECRET;
			menuMission(id);
		}
		case 2:{
			missionMenu[id] = MISSION_MENU_PRO;	
			menuMission(id);
		}
	}
}

public menuMission(id){
	new gText[128], menu;

	if(missionMenu[id] == MISSION_MENU_BASIC){
		menu = menu_create("Misje Podstawowe", "menuMission_2");
		for( new i = 0 ; i < mission_TOTAL; i ++ ){
			if( userMission[id][i] == -1 )
				format(gText, sizeof(gText), "\d%s [Nagroda Odebrana]", missionDesc[i][0], userMission[id][i], missionDesc[i][2]);
			else if( userMission[id][i] == str_to_num(missionDesc[i][2]) ) 
				format(gText, sizeof(gText), "\d%s\y [Wykonano]", missionDesc[i][0], userMission[id][i], missionDesc[i][2]);
			else format(gText, sizeof(gText), "\d%s\r [%d/%s]", missionDesc[i][0], userMission[id][i], missionDesc[i][2]);
			
			menu_additem( menu, gText) ;
		}
	} else if(missionMenu[id] == MISSION_MENU_SECRET){
		menu = menu_create("Misje Sekretne", "menuMission_2");
		
		for( new i = 0; i < mission_secret_TOTAL; i ++ ){
		
			if( userMissionSecret[id][i] == -1 )
				format(gText, sizeof(gText), "\d%s [Nagroda Odebrana]", missionDescSecret[i][0], userMissionSecret[id][i], missionDescSecret[i][2]);
			else if( userMissionSecret[id][i] == str_to_num(missionDescSecret[i][2]) ) 
				format(gText, sizeof(gText), "\d%s\y [Wykonano]", missionDescSecret[i][0], userMissionSecret[id][i], missionDescSecret[i][2]);
			else format(gText, sizeof(gText), "\d%c\r [%d/%s]",missionDescSecret[i][0], userMissionSecret[id][i], missionDescSecret[i][2]);
			menu_additem( menu, gText);      
		}	
	} else if(missionMenu[id] == MISSION_MENU_PRO){
		menu = menu_create("Misje Profesjonalne", "menuMission_2");
	
		for(new i = 0 ; i < sizeof(proDesc); i ++ ){
			if( userPro[id][i] == -1)
				format(gText, sizeof(gText), "\d%s [Nagroda Odebrana]",proDesc[i][0]);
			else if( userPro[id][i] == str_to_num(proDesc[i][4]) )
				format(gText, sizeof(gText), "\d%s\y [Wykonane]",proDesc[i][0]);
			else{
				format(gText, sizeof(gText), "%s%s\r [%d/%s]", userProSelected[id] == i ? "\w" :"\d",proDesc[i][0], userPro[id][i], proDesc[i][4]);
			}
				
			menu_additem(menu, gText);
		}
		
	}
	menu_display(id, menu, 0);
}
public menuMission_2(id, menu, item){
	if( item == MENU_EXIT){
		menuMissionAll(id);
		return PLUGIN_CONTINUE;
	}
	
	isMisisonMenu[id] = item;
	
	missionMenuDesc(id);
	return PLUGIN_CONTINUE;
}
	
public missionMenuDesc(id){
	
	new gText[512], iLen;
	
	new item = isMisisonMenu[id];
	
	if(missionMenu[id] == MISSION_MENU_BASIC){
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r[\d Misja Podstawowa\r ]^n^n");
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dMisja:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW],  missionDesc[item][0]);
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dOpis:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW],  returnMissionDesc(item));
		
		if( userMission[id][item] == -1 ) iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dPostep:\r Nagroda Odebrana^n", symbolsCustom[SYMBOL_DR_ARROW]);
		else if( userMission[id][item] == str_to_num(missionDesc[item][2]))  iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dPostep:\r Wykonano\d (\w%d/%s\d)^n", symbolsCustom[SYMBOL_DR_ARROW], userMission[id][item], missionDesc[item][2]);
		else iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dPostep:\r %d/%s\d -\r %0.1f%%^n", symbolsCustom[SYMBOL_DR_ARROW], userMission[id][item], missionDesc[item][2], userMission[id][item]*100.0/str_to_num(missionDesc[item][2]));
		
		iLen += format(gText[iLen], sizeof(gText)-iLen-1,"^n\w[Nagroda]^n\y%s^t^t", symbolsCustom[SYMBOL_DR_ARROW]);
	
		iLen += format(gText[iLen], sizeof(gText)-iLen-1,"%s", missionNameAward(id));
		
		new pageOneSelect[22];
		format(pageOneSelect, sizeof(pageOneSelect), "%s", (  userMission[id][item] == -1) ? "Nagroda odebrana" : (userMission[id][item] == str_to_num(missionDesc[item][2])  ? "Odbierz nagrode" : "Wykonaj pierw misje"));
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n%s1.%s %s^n", userMission[id][item] == -1? "\d" : "\r", userMission[id][item] == -1 ? "\d" : "\y", pageOneSelect);	
	
	} else if(missionMenu[id] == MISSION_MENU_SECRET){
		
		
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r[\d Misja Sekretna\r ]^n^n");
		
		if(userMissionSecret[id][item] == str_to_num(missionDescSecret[item][2]) || userMissionSecret[id][item] == -1){
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dMisja:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW],  missionDescSecret[item][0]);
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dOpis:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW],  returnMissionSecretDesc(item));
		} else {
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dMisja:\r %c^n", symbolsCustom[SYMBOL_DR_ARROW],  missionDescSecret[item][0]);
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dOpis:\r Tylko nie liczni znaja ta misje!^n", symbolsCustom[SYMBOL_DR_ARROW]);
		}
		
		if( userMissionSecret[id][item] == -1 ) iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dPostep:\r Nagroda Odebrana^n", symbolsCustom[SYMBOL_DR_ARROW]);
		else if( userMissionSecret[id][item] == str_to_num(missionDescSecret[item][2]))  iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dPostep:\r Wykonano\d (\w%d/%s\d)^n", symbolsCustom[SYMBOL_DR_ARROW], userMissionSecret[id][item], missionDescSecret[item][2]);
		else iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dPostep:\r %d/%s\d -\r %0.1f%%^n", symbolsCustom[SYMBOL_DR_ARROW], userMissionSecret[id][item], missionDescSecret[item][2],  userMissionSecret[id][item]*100.0/str_to_num(missionDescSecret[item][2]));
		
		iLen += format(gText[iLen], sizeof(gText)-iLen-1,"^n\w[Nagroda]^n\y%s^t^t", symbolsCustom[SYMBOL_DR_ARROW]);
		
		iLen += format(gText[iLen], sizeof(gText)-iLen-1,"%s", missionNameAward(id));
		
		new pageOneSelect[22];
		format(pageOneSelect, sizeof(pageOneSelect), "%s", ( userMissionSecret[id][item] == -1 ) ? "Nagroda odebrana" : (userMissionSecret[id][item] == str_to_num(missionDescSecret[item][2])  ? "Odbierz nagrode" : "Wykonaj pierw misje"));
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n%s1.%s %s^n", userMissionSecret[id][item] == -1? "\d" : "\r", userMissionSecret[id][item] == -1 ? "\d" : "\y", pageOneSelect);	
	
	} else if(missionMenu[id] == MISSION_MENU_PRO){
		
		
		iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dMisja:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], proDesc[item][0]);
		iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dOpis:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], proDesc[item][1]);
		iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dNagroda:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], proDesc[item][3]);
		
		
		if(  userPro[id][item] == -1 ) iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dPostep:\r Nagroda Odebrana^n", symbolsCustom[SYMBOL_DR_ARROW]);
		else if( userPro[id][item] == str_to_num(proDesc[item][4]))  iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dPostep:\r Wykonano\d (\w%d/%s\d)^n", symbolsCustom[SYMBOL_DR_ARROW],  userPro[id][item], proDesc[item][4]);
		else iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dPostep:\r %d/%s\d -\r %0.1f%%^n", symbolsCustom[SYMBOL_DR_ARROW],  userPro[id][item], proDesc[item][4], userPro[id][item]*100.0/str_to_num(proDesc[item][4]));
		new pageOneSelect[22];
		format(pageOneSelect, sizeof(pageOneSelect), "%s", ( userPro[id][item] == -1 ) ? "Nagroda odebrana" : (userPro[id][item] == str_to_num(proDesc[item][4])  ? "Odbierz nagrode" : userProSelected[id] == item ? "Wykonujesz te misje" : "Aktywuj Misje"));
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n%s1.%s %s^n", userPro[id][item] == -1? "\d" : "\r", userPro[id][item] == -1 ? "\d" : "\y", pageOneSelect);	

	}
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r2.\y Wroc");

	showMenu(id, B1 | B2  , gText, -1, "missionMenuDesc"  );
	return PLUGIN_HANDLED;
}
public missionMenuDesc_2(id, item){
	
	new target = isMisisonMenu[id];

	
	switch(item){
		case 0:{
			if(missionMenu[id] == MISSION_MENU_BASIC){
				if( ( userMission[id][target] == -1 )){
					ColorChat(id, GREEN, "%s Juz odebrales misje:^x03 %s", PREFIXSAY,   missionDesc[target][0] );	
				} else if( userMission[id][target] == str_to_num(missionDesc[target][2])){
					missionGiveAward(id);
				} else {
					ColorChat(id, GREEN, "%s Nie mozesz jeszcze odebrac misji:^x03 %d/%s", PREFIXSAY,userMission[id][target],missionDesc[target][2]);
				}
			
			} else if(missionMenu[id] == MISSION_MENU_SECRET){
				
				if( ( userMissionSecret[id][target] == -1 )){
					ColorChat(id, GREEN, "%s Juz odebrales misje:^x03 %s", PREFIXSAY,  missionDescSecret[target][0]);	
				} else if( userMissionSecret[id][target] == str_to_num(missionDescSecret[target][2])){
					missionGiveAward(id);
				} else {
					ColorChat(id, GREEN, "%s Nie mozesz jeszcze odebrac misji:^x03 %d/%s", PREFIXSAY,userMissionSecret[id][target], missionDescSecret[target][2]);
				}	
			} else if(missionMenu[id] == MISSION_MENU_PRO){
				
				if( userPro[id][target] == -1){
					ColorChat(id, GREEN, "%s Nagroda juz zostala odebrana", PREFIXSAY );
					return PLUGIN_CONTINUE;
					
				}else if( userPro[id][target] < str_to_num( proDesc[target][4] ) ){
					if( userProSelected[id] == target )					
						ColorChat(id, GREEN, "%s Juz wykonujesz te misje", PREFIXSAY);
					else {
						ColorChat(id, GREEN, "%s Misja zostala aktywowana", PREFIXSAY);
						userProSelected[id] = target ;
					}
					return PLUGIN_CONTINUE;
					
				}
				new gText[128];
				logType[id] = LOG_MISSION;
				if(logType[id] == LOG_MISSION){
					format(gText, sizeof(gText), "odebral  profesjonalna misje [%s]", proDesc[target][0]);
					logBB(id,gText);
				}
				
				userPro[id][target] = -1;
				userProSelected[id] = -1;
				ColorChat(id, GREEN, "%s Otrzymujesz nagrode:^x04 %s", PREFIXSAY,  proDesc[target][3]);
			}
		}
		case 1: menuMission(id);
	}
	return PLUGIN_CONTINUE;
}


public returnMissionDesc(mission){	
	new missionReturn[78], missionVar[15];

	
	format(missionVar, sizeof(missionVar), "%d", str_to_num(missionDesc[mission][2]));
	copy(missionReturn, sizeof(missionReturn), missionDesc[mission][1]);
	replace_all(missionReturn, sizeof(missionReturn), "%d", missionVar);
	return missionReturn;
}
public returnMissionSecretDesc(mission){	
	new missionReturn[78], missionVar[15];
	format(missionVar, sizeof(missionVar), "%d", str_to_num(missionDescSecret[mission][2]));
	copy(missionReturn, sizeof(missionReturn), missionDescSecret[mission][1]);
	replace_all(missionReturn, sizeof(missionReturn), "%d", missionVar);
	return missionReturn;
}
public missionNameAward(id){

	new item 	= ( isMisisonMenu[id] );
	new value 	= ( missionMenu[id] == MISSION_MENU_BASIC ) ? missionAward[item][1] : missionSecretAward[item][1];
	new type 	= ( missionMenu[id] == MISSION_MENU_BASIC ) ? missionAward[item][0] : missionSecretAward[item][0];
	
	new gText[256], iLen = 0;
	
	iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen -1,"\d%s:\r %d", nameItems[type][0], value);
	
	return gText;
			
}
public missionGiveAward(id){


	if(userKills[id] < 200 && userDeaths[id] < 200){
		ColorChat(id, GREEN, "---^x01 Musisz posiadac^x03 200 zabojstw^x01 oraz^x03 200 zgonow^x01 aby odebrac misje^x04 ---");
		return PLUGIN_CONTINUE;
	}

	new item 	= ( isMisisonMenu[id] );
	new value 	= ( missionMenu[id] == MISSION_MENU_BASIC ) ? missionAward[item][1] : missionSecretAward[item][1];
	new type 	= ( missionMenu[id] == MISSION_MENU_BASIC ) ? missionAward[item][0] : missionSecretAward[item][0];

	if(missionMenu[id] == MISSION_MENU_BASIC){
		new gText[128];
		logType[id] = LOG_MISSION;
		if(logType[id] == LOG_MISSION){
			format(gText, sizeof(gText), "odebral podstawowa misje [%s]", missionDesc[item][0]);
			logBB(id,gText);
		}
						
		ColorChat(id, GREEN, "%s Gracz:^x04 %s^x01 odebral misje -^x03 %s", PREFIXSAY, userName[id], missionDesc[item][0]);
		userMission[id][item] = -1;
		
	}  else if(missionMenu[id] == MISSION_MENU_SECRET){
		new gText[128];
		logType[id] = LOG_MISSION;
		if(logType[id] == LOG_MISSION){
			format(gText, sizeof(gText), "odebral sekretna misje [%c] (%s)", missionDescSecret[item][0], missionDescSecret[item][0] );
			logBB(id,gText);
		}

		ColorChat(id, GREEN, "%s Gracz:^x04 %s^x01 odebral sekretna misje -^x03 %c", PREFIXSAY, userName[id], missionDescSecret[item][0]);
		userMissionSecret[id][item] = -1;
		
	}
	
	switch( type ){
		case EXP:	addExpToFinal(id, float(value)) ;
		case NUGGET:	userNugget[id] 	+=  	value;
		case VIP:	timeVip[id]	= 	max( timeVip[id] + (HOUR*value), get_systime() + (HOUR*value) );
		case LUZACZKI:	userLuzCoin[id] 	+=  	value;
		
	}
	return value;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
