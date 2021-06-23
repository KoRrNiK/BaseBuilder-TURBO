#include <amxmodx>
#include <amxmisc>
#include <engine>
#include <fakemeta>

public mainMenuAccount(id){
	
	if(!is_user_connected(id)) return;
	
	new gText[256];
	
	new hidePass[12]; copy(hidePass, sizeof(userPassword[])-1, userPassword[id]);
	for(new i = 0;  i < strlen(hidePass); i ++){
		if (i % 3 || i < 2) hidePass[i] = '*';
	}
	new hideIp[32]; copy(hideIp, sizeof(userIp[])-1, userIp[id]);
	for(new i = 0;  i < strlen(hideIp); i ++){
		if (i % 3 || i < 2) hideIp[i] = '*';
	}
	if(!userLogged[id] && strlen(userPassword[id]) > 1){
		format(gText, sizeof(gText), "\r[BaseBuilder]\y System Kont:^n\wZaloguj sie aby kozystac z konta^n");
	} else {
		if( 1 >= strlen(userPassword[id])){
			format(gText, sizeof(gText), "\r[BaseBuilder]\y System Kont:^n\wZarejestruj sie aby kozystac z konta^n");
		} else {
			format(gText, sizeof(gText), "\r[BaseBuilder]\y System Kont:^n^n\
						  \dWitaj\y [\r%s\y]\d Tutaj znajdziesz ustawienia do swojego konta!^n^n\
						  \y%s^t^t\dTwoj Sid:\r %s^n\y%s^t^t\dTwoje ip:\r %s^n\y%s^t^t\dTwoje Haslo:\r %s", userName[id], symbolsCustom[SYMBOL_DR_ARROW], userSid[id],symbolsCustom[SYMBOL_DR_ARROW], hideIp, symbolsCustom[SYMBOL_DR_ARROW],  hidePass);
		}
	}
	new menu = menu_create(gText, "mainMenuAccount_2");

	if(!userLogged[id] && strlen(userPassword[id]) > 1){
		format(gText, sizeof(gText), "Zaloguj");
	} else {
		if( 1 >= strlen(userPassword[id])) format(gText, sizeof(gText), "Zarejestruj");
		else format(gText, sizeof(gText), "Zmien haslo");
	}
	menu_additem(menu, gText);
	
	if(playerLogged(id)){
		menu_additem(menu, "Wyloguj Sie");
		menu_additem(menu, "Ustawienia^n");
		new szSavedIP[MAXBITIP], szSavedSID[MAXBITAUTHID];
		get_user_ip(id, szSavedIP, sizeof(szSavedIP)-1, 1);
		get_user_authid(id, szSavedSID, sizeof(szSavedSID)-1);
					
		if( equal(szSavedIP, userIp[id] ) && equal(szSavedSID, userSid[id] ) )	
			menu_additem(menu, "\rAutomatyczne Logowanie");
		else menu_additem(menu, "\yAutomatyczne Logowanie\d |\w Wylaczone");
		
	}
	menu_display(id,menu,0);
}
public mainMenuAccount_2(id, menu, item){
	new gText[128];
	if( item == MENU_EXIT ){
		menu_destroy(menu);
		return PLUGIN_HANDLED;
	}
	
	switch(item){
		case 0: cmd_execute(id, "messagemode Haslo");
		case 1:{	
			if( !playerLogged(id) ){
				mainMenuAccount(id);
				return PLUGIN_HANDLED;
			}
			userLogged[id] = false;
			
			logType[id] = LOG_LOGOUT;
			if(logType[id] == LOG_LOGOUT){
				format(gText, sizeof(gText), "wylogowal sie !!");
				logBB(id, gText);
			} 
			chatPrint(id, PREFIX_NORMAL, "Wylogowales sie!" );
		}
		case 2:{
			settingAccount(id);	
		}
		case 3:{
			if(userLogged[id]){
				new szSavedIP[MAXBITIP], szSavedSID[MAXBITAUTHID];
				get_user_ip(id, szSavedIP, sizeof(szSavedIP)-1, 1);
				get_user_authid(id, szSavedSID, sizeof(szSavedSID)-1);
				if(equal(szSavedIP, userIp[id] ) && equal(szSavedSID, userSid[id] ) ){
					chatPrint(id, PREFIX_LINE, "Masz juz wlaczone^3 Automatyczne^1 Logowanie!");
					mainMenuAccount(id);
					return PLUGIN_CONTINUE; 
				}
				
				chatPrint(id, PREFIX_LINE, "Zaktualizowales swoje^3 Automatyczne^1 Logowanie!");
				new ip[MAXBITIP], sid[MAXBITAUTHID];
				get_user_ip(id, ip, sizeof(ip)-1, 1);
				get_user_authid(id, sid, sizeof(sid)-1);
				userIp[id] = ip;
				userSid[id] = sid;
				mainMenuAccount(id);
			}
		}
		default:  return PLUGIN_CONTINUE;
	}
	return PLUGIN_HANDLED;
}
public readPassword(id){		
	
	new szArg[12];
	new gText[128];
	read_argv(1, szArg, sizeof(szArg));
	replace_all(szArg, sizeof(szArg), "^"", "");
	replace_all(szArg, sizeof(szArg), " ", "");
	
	if( strlen(szArg) < 3 ){
		chatPrint(id, PREFIX_NORMAL, "Podane haslo jest za krotkie^3 ( Limit 11 symboli )" );
		mainMenuAccount(id);
		return PLUGIN_CONTINUE;
	}
	if( strlen(szArg) > 11 ){
		chatPrint(id, PREFIX_NORMAL, "Podane haslo jest zadlugie" );
		mainMenuAccount(id);
		return PLUGIN_CONTINUE;
	}
	if (!userLogged[id] && strlen(userPassword[id]) > 1){
		if( equal(szArg, userPassword[id]) ){
			userLogged[id]=true;			
			set_user_info(id, "_pw", szArg );
			cmd_execute(id, "setinfo _pw %s",szArg);
			
			logType[id] = LOG_LOGIN;
			if(logType[id] == LOG_LOGIN){
				format(gText, sizeof(gText), "zalogowal sie, uzyl - [Aktualne haslo: %s]", szArg);
				logBB(id, gText);
			}
			
			chatPrint(id, PREFIX_NORMAL, "Zalogowano pomyslnie" );
			mainMenuAccount(id);
		}else{
			logType[id] = LOG_ERROR;
			if(logType[id] == LOG_ERROR){
				format(gText, sizeof(gText), "bledne haslo !! [Wpisal: %s][Aktualne haslo: %s]",szArg, userPassword[id]);
				logBB(id, gText);
			}
			chatPrint(id, PREFIX_NORMAL, "Podales zle haslo:^3 %s", szArg );
		}
	} else {
		userLogged[id]=true;	
		chatPrint(id, PREFIX_NORMAL, "Ustawiles haslo:^4 %s", szArg );		
		cmd_execute(id, "setinfo _pw %s",szArg);
		
		logType[id] = LOG_LOGIN;
		if(logType[id] == LOG_LOGIN){
			format(gText, sizeof(gText), "ustawil [Haslo: %s]", szArg);
			logBB(id, gText);
		}
		copy(userPassword[id], sizeof(userPassword[]), szArg);
		mainMenuAccount(id);
	}
	
	return PLUGIN_CONTINUE;
}
new bool:userViewCamera[33];
public settingAccount(id){
	
	if(!is_user_connected(id)) return;
	
	new gText[64];
	
	format(gText, sizeof(gText), "\r[BaseBuilder]\y Ustawienia Kont:");
	new menu = menu_create(gText, "settingAccount_2");	
	
	menu_additem(menu, "Ustawienia Hudu");
	
	
	format(gText, sizeof(gText), "JetPack Speed:\r %d", userJetpackSpeed[id]);
	menu_additem(menu, gText);
	
	
	format(gText, sizeof(gText), "Rendering: %s", userMoverBlockColor[id] == BLOCK_COLOR ? "\rKolor" : userMoverBlockColor[id] == BLOCK_RENDER ? "\yRendering" : "\dTekstura");
	menu_additem(menu, gText);
	
	format(gText, sizeof(gText), "Modele Kosy: %s", hasOption(userSaveOption[id], save_MODELS) ? "\rWlaczone" : "\dWylaczone");
	menu_additem(menu, gText);
	
	if(userFov[id] == 90) format(gText, sizeof(gText), "Fov Broni:\y Normalny^n");
	else format(gText, sizeof(gText), "Fov Broni:\r %d^n", userFov[id]);
	menu_additem(menu, gText);
	
	//format(gText, sizeof(gText), "Wiadomosci Kopalni: %s", hasOption(userSaveOption[id], save_CAVE) ? "\rWlaczone" : "\dWylaczone");
	//menu_additem(menu, gText);
	
	format(gText, sizeof(gText), "Ludzie: %s", hasOption(userSaveOption[id], save_INVIS) ? "\rWlaczone" : "\dWylaczone");
	menu_additem(menu, gText);
	
	
	format(gText, sizeof(gText), "Widok z trzeciej osoby: %s", userViewCamera[id] ? "\rWlaczone" : "\dWylaczone");
	menu_additem(menu, gText);
	
	menu_display(id, menu, 0);
	return;
}




public settingAccount_2(id, menu, item){
	if( item == MENU_EXIT){
		menu_destroy(menu);
		return PLUGIN_CONTINUE;
	}	
	switch(item){
		case 0: {
			changeHud(id);
		}
		case 1:{
			
			if ((userJetpackSpeed[id] += 100) > 1000) 
				userJetpackSpeed[id] = 100;
			
			settingAccount(id);
		}
		case 2:{
			
			userMoverBlockColor[id] 	= (userMoverBlockColor[id]+1) 	% 3;
			settingAccount(id);
		}
		case 3:{
			if( hasOption(userSaveOption[id], save_MODELS) ){
				removeOption(userSaveOption[id], save_MODELS);
				chatPrint(id, PREFIX_LINE, "Modele do kosy^3 wlaczone");
			}else{
				addOption(userSaveOption[id], save_MODELS);
				chatPrint(id, PREFIX_LINE, "Modele do kosy^3 wylaczone");
			}
			settingAccount(id);
		}
		case 4:{
			if(cs_get_user_zoom(id) == CS_SET_AUGSG552_ZOOM || cs_get_user_zoom(id) == CS_SET_FIRST_ZOOM || cs_get_user_zoom(id)  == CS_SET_SECOND_ZOOM){
				chatPrint(id, PREFIX_LINE, "Wylacz^3 ZOOM'a^1 aby zmienic^3 FOV'a");
				settingAccount(id);
				return PLUGIN_CONTINUE;
			}
			
			if ((userFov[id] += 5) > 150 || userFov[id] < 90) 
				userFov[id] = 90;
		
			setFov(id, userFov[id]);
			
			settingAccount(id);
		}
		case 5:{
			if( hasOption(userSaveOption[id], save_INVIS) ){
				removeOption(userSaveOption[id], save_INVIS);
				chatPrint(id, PREFIX_LINE, "Widzenie osob wlaczone");
			}else{
				addOption(userSaveOption[id], save_INVIS);
				chatPrint(id, PREFIX_LINE, "Widzenie osob wylaczone");
			}
			settingAccount(id);
			
		}
		case 6:{
			userViewCamera[id] =! userViewCamera[id];
			if(userViewCamera[id])
				set_view(id,CAMERA_3RDPERSON);
			else set_view(id, CAMERA_NONE);
			settingAccount(id);
		}
		
	}
	return PLUGIN_CONTINUE;
}
public startLogin(id){
		
	id -= TASK_LOGIN;
	if( !is_user_connected(id) ) return;
			
	if(!playerLogged(id)){
		if(!is_user_hltv(id)){
			userSpeedAdmin[id] = 1000.0;
			
			if(!userWarningHudStart[id]){
				Display_Fade(id, 4096,4096, 4096,1, 1, 1,255);
				set_dhudmessage(255, 102, 0, -1.0, 0.2, 0, 0.5, 0.9, 0.5, 0.5);
				
				if( strlen(userPassword[id]) < 2 )
					show_dhudmessage(id, "[ Zarejestruj Sie ]^n[ Wpisz /konto | /zarejestruj ]");
				else show_dhudmessage(id, "[ Zaloguj Sie ]^n[ Wpisz /konto | /login ]");
			}
		} else {
			
			new szCurrentTime[9];
			new szData[11];
			
			get_time("%H:%M:%S",szCurrentTime,sizeof(szCurrentTime));
			get_time("%d.%m.%Y",szData,sizeof(szData));
	
			set_dhudmessage(255, 102, 0, -1.0, 0.0, 0, 0.5, 0.9, 0.5, 0.5);
			show_dhudmessage(id, "^n[ %s - %s ]^n[ BaseBuilder ]", szCurrentTime, szData);
		}
		
			
	} else {
		userSpeedAdmin[id] = 0.0;
		remove_task(id + TASK_LOGIN);
		
	}
	set_task(0.85, "startLogin", id + TASK_LOGIN);
}

new const pointAdd[] = { 1, 3, 5, 10, 15, 25, 255 };
new userHudAdd[33][4];

public changeHud(id){
	if (!is_user_connected(id)) return PLUGIN_HANDLED;

	missionHud(id);
		
	new gText[128];
	new menu 	= 	menu_create("\r[BaseBuilder]\y Konfiguracja HUD\w", "changeHud_2");
	new cb 	= 		menu_makecallback("changeHud_3");
	
	format(gText, sizeof(gText), "\wKolor \yCzerwony: \r%d", userHud[id][PLAYER_HUD_RED]);
	menu_additem(menu, gText, .callback=cb);

	format(gText, sizeof(gText), "\wKolor \yZielony: \r%d", userHud[id][PLAYER_HUD_GREEN]);
	menu_additem(menu, gText, .callback=cb);

	format(gText, sizeof(gText), "\wKolor \yNiebieski: \r%d^n", userHud[id][PLAYER_HUD_BLUE]);
	menu_additem(menu, gText, .callback=cb);
	
	format(gText, sizeof(gText), "\wSposob dodawania: \r%s", userHudAdd[id][2] == 1 ? "Klikaj" : "Wpisz");
	menu_additem(menu, gText, .callback=cb);
	
	format(gText, sizeof(gText), "\%cWartosc dodawania: \%c%d",  userHudAdd[id][2] == 1 ? 'w' : 'd',  userHudAdd[id][2] == 1 ? 'r' : 'd', pointAdd[userHudAdd[id][0]]);
	menu_additem(menu, gText, .callback=cb);
	
	format(gText, sizeof(gText), "\%cTyp dodawania: \%c%s^n", userHudAdd[id][2] == 1 ? 'w' : 'd', userHudAdd[id][2] == 1 ? 'r' : 'd',  userHudAdd[id][1] == 0 ? "Odejmij" : "Dodaj");
	menu_additem(menu, gText, .callback=cb);
	
	format(gText, sizeof(gText), "\yDomyslne Ustawienia");
	menu_additem(menu, gText, .callback=cb);


	menu_setprop(menu, MPROP_EXITNAME, "Wyjscie");

	menu_display(id, menu);

	return PLUGIN_HANDLED;
}
public changeHud_3(id, menu, item) {
	switch(item){
		case 4,5: {
			if(userHudAdd[id][2] == 1)
				return ITEM_ENABLED;
			else return ITEM_DISABLED;	
		}
	}	
	return ITEM_ENABLED;
}
public changeHud_2(id, menu, item){
	if (!is_user_connected(id)) return PLUGIN_HANDLED;

	if (item == MENU_EXIT) {
		menu_destroy(menu);
		return PLUGIN_HANDLED;
	}
	
	new addHud =  pointAdd[userHudAdd[id][0]];
	
	switch (item) {
		case 0: {
			if(userHudAdd[id][2]){
				if(userHudAdd[id][1]){
					if ((userHud[id][PLAYER_HUD_RED] += addHud) > 255) 
						userHud[id][PLAYER_HUD_RED] = 0;
				} else {
					if ((userHud[id][PLAYER_HUD_RED] -= addHud) < 0) 
						userHud[id][PLAYER_HUD_RED] = 255;
				}
			} else {
				userHudAdd[id][3] =0;
				client_cmd(id, "messagemode WartoscKoloruHud");
			}
			changeHud(id);
		}
		case 1:{
			if(userHudAdd[id][2]){
				if(userHudAdd[id][1]){
					if ((userHud[id][PLAYER_HUD_GREEN] += addHud) > 255) 
						userHud[id][PLAYER_HUD_GREEN] = 0;
				} else {
					if ((userHud[id][PLAYER_HUD_GREEN] -= addHud) < 0) 
						userHud[id][PLAYER_HUD_GREEN] = 255;
				} 
			} else {
				userHudAdd[id][3] =1;
				client_cmd(id, "messagemode WartoscKoloruHud");
			}
			changeHud(id);
		}
		case 2:{
			if(userHudAdd[id][2]){
				if(userHudAdd[id][1]){
					if ((userHud[id][PLAYER_HUD_BLUE] += addHud) > 255) 
						userHud[id][PLAYER_HUD_BLUE] = 0;
				} else {
					if ((userHud[id][PLAYER_HUD_BLUE] -= addHud) < 0) 
						userHud[id][PLAYER_HUD_BLUE] = 255;
				}
			} else {
				userHudAdd[id][3] =2;
				client_cmd(id, "messagemode WartoscKoloruHud");
			}
			changeHud(id);
		}
		case 3:{
			if(userHudAdd[id][2] == 0){
				userHudAdd[id][2] ++;
			} else {
				userHudAdd[id][2] --;
			}
			
			changeHud(id);
		}
		case 4:{
			if (++userHudAdd[id][0] >= sizeof(pointAdd)) {
				userHudAdd[id][0] = 0;
			}
			changeHud(id);
		}
		case 5:{
			if(userHudAdd[id][1] == 0){
				userHudAdd[id][1] ++;
			} else {
				userHudAdd[id][1] --;
			}
			
			changeHud(id);
		}
		case 6: {
			userHud[id][PLAYER_HUD_RED] = 255;//120;
			userHud[id][PLAYER_HUD_GREEN] = 0;// 255;
			userHud[id][PLAYER_HUD_BLUE] = 60;//60;
			
			userHudAdd[id][0] = 4;
			userHudAdd[id][1] = 1;
			userHudAdd[id][2] = 1;
			userHudAdd[id][3] = 0;
			
			changeHud(id);
		}
	}
	menu_destroy(menu);
	return PLUGIN_CONTINUE;
}
public readColor(id){

	new szArg[7];
	read_argv(1, szArg, sizeof(szArg)-3);
	
	new color = str_to_num(szArg)%256;
	
	switch(userHudAdd[id][3]){
		case 0: userHud[id][PLAYER_HUD_RED] = color;
		case 1: userHud[id][PLAYER_HUD_GREEN] = color;		
		case 2: userHud[id][PLAYER_HUD_BLUE] = color;		
	}
	changeHud(id);
}
public missionHud(id){
	if(userHud[id][PLAYER_HUD_RED] == 1 && userHud[id][PLAYER_HUD_GREEN] == 1 && userHud[id][PLAYER_HUD_BLUE] == 1 ){
		addSecretMission(id, mission_secret_COLOR, 1);
	}
	
}


public bool:playerLogged(id){
	if(!userLoaded[id] || !userLogged[id] ||  strlen(userPassword[id]) < 2)
		return false;
	return true;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
