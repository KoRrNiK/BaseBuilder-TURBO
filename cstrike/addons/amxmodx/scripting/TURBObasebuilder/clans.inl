#include <amxmodx>
#include <sqlx>
#include <regex>

new maxSymbolClan = 12;
new minSymbolClan = 3;
	
new const MOTDNONE[]	=	"d8y8puu";	
	
public cfgClan(){
	new bbClan[clanInfo];
	bbClan[CLAN_NAME] = "Brak";
	ArrayPushArray(bbClans, bbClan);
}
public endClan(){
	ArrayDestroy(bbClans);
}

public globalClanMenu(id){	
	if( !playerLogged(id)){
		chatPrint(id, PREFIX_NORMAL, "Zaloguj sie aby kozystac z klanow!");
		return;
	}
	if (clan[id]){
		pageClan[id] = false;
		showClanMenu(id, false);
	} else createClanMenu(id);	
}
public createClanMenu(id){
	
	new gText[512], iLen = 0;	
	new availableNugget[256], availableLevel[256], availableLuzaczki[256]; 
	
	format(availableNugget, 		sizeof(availableNugget),  	"\w|\d Potrzebujesz jeszcze:\r %d Brylek!", 	bbCvar[cvarClansCostNugget]-userNugget[id]);
	format(availableLevel,		sizeof(availableLevel),  	"\w|\d Potrzebujesz jeszcze:\r %d Level!", 	bbCvar[cvarClansCostLevel]-userLevel[id]);
	format(availableLuzaczki, 	sizeof(availableLuzaczki),  	"\w|\d Potrzebujesz jeszcze:\r %d Luzaczkow!", 	bbCvar[cvarClansCostLuzaczki]-userLuzCoin[id]);
	
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "\r[BaseBuilder]\y Stworz klan:^n");
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y-^t^t\dNazwa klanu musi miec co najmniej\r %d\d znaki", minSymbolClan);
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y-^t^t\dNazwa nie moze miec wiecej niz\r %d\d znakow", maxSymbolClan);
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y-^t^t\dZakaz\r obrazliwych\d nazw klanow");
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y-^t^t\dDozwolone znaki\r [A-Z0-9_]^n");
	
	if(bbCvar[cvarClansCostNugget]) 		iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t^t\dBrylki:\r %d %s",symbolsCustom[SYMBOL_DR_ARROW], bbCvar[cvarClansCostNugget], (bbCvar[cvarClansCostNugget] > userNugget[id]) ? availableNugget : "\w| \yWymaganie spelnione!");
	if(bbCvar[cvarClansCostLevel]) 		iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t^t\dPoziom:\r %d %s", symbolsCustom[SYMBOL_DR_ARROW], bbCvar[cvarClansCostLevel], (bbCvar[cvarClansCostLevel] > userLevel[id]) ? availableLevel : "\w| \yWymaganie spelnione!");
	if(bbCvar[cvarClansCostLuzaczki]) 	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t^t\dLuzaczki:\r %d %s", symbolsCustom[SYMBOL_DR_ARROW], bbCvar[cvarClansCostLuzaczki], (bbCvar[cvarClansCostLuzaczki] > userLuzCoin[id]) ? availableLuzaczki : "\w| \yWymaganie spelnione!");

	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\r1.\w Stworz Klan");
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\r2.\w Top 10 Klanow");
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\r3.\w Lista Klanow");
	
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\r0.\y Wyjdz");
	
	showMenu(id, B1 | B2 | B3 | B0  , gText, -1, "createClanMenu", 1);
}
public createClanMenu_2(id, item){
	if (!is_user_connected(id)) return;
	
	switch(item){
		case 0: {
			if (userNugget[id] < bbCvar[cvarClansCostNugget]) {
				chatPrint(id, PREFIX_NORMAL, "Brakuje Ci Brylek!");
				return;
			}
			if(userLevel[id] < bbCvar[cvarClansCostLevel]){
				chatPrint(id, PREFIX_NORMAL, "Brakuje Ci Poziomu!");
				return;
			}
			if(userLuzCoin[id] < bbCvar[cvarClansCostLuzaczki]){
				chatPrint(id, PREFIX_NORMAL, "Brakuje Ci Luzaczkow!");
				return;
			}
			
			client_cmd(id, "messagemode PodajNazweKlanu");
		}
		case 1: loadStatsSql(id,5 );
		case 2: loadStatsSql(id,7 );
		
		default:{}
	}
}
public listClanMenu_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return;
	}
	
	new target = userViewClan[id][item];
	userViewClanInfo[id] = target;
	
	loadDescClan(id);
}

public loadDescClan(id){
	
	if(is_user_hltv(id)) return PLUGIN_CONTINUE;
		
	if (!sqlConnected) {
		log_amx("[SQL-LOG] Brak polaczenia");
		return PLUGIN_CONTINUE;
	}

	
	new queryData[512];
	new tempId[3];
	
	tempId[0] = id;
	tempId[1] = userViewClanInfo[id];

	format(queryData, sizeof(queryData), "\
		SELECT `a`.`idmotd`, `a`.`idclans` AS 'idclan', `a`.`name` AS `clan`, `a`.`kills`,`a`.`members`,`a`.`level`, `a`.`createDate`, `b`.`name` AS 'LIDER'\
		FROM `clans` `a`\
		JOIN `players` `b` ON `a`.`idclans` = `b`.`clan`\
		WHERE `a`.`idclans`='%d' AND `flag`='%d'", tempId[1], STATUS_LEADER);
	
	SQL_ThreadQuery(sql, "loadClanInfoDesc_2", queryData, tempId, sizeof(tempId));

	return PLUGIN_CONTINUE;
}
public loadClanInfoDesc_2(failState, Handle:query, error[], errorNum, tempId[], dataSize){
	
	new id 		= 	tempId[0];
	
	if (failState){
		if (failState == TQUERY_CONNECT_FAILED) log_amx("[SQL-LOG] Nie mozna polaczyc siê z baz¹ danych SQL. Blad: %s (%d)", error, errorNum);
		else if (failState == TQUERY_QUERY_FAILED) log_amx("[SQL-LOG] Zapytanie watkowe nie powiodlo sie. Blad: %s (%d)", error, errorNum);
		return PLUGIN_HANDLED;
	}	
	
	new gText[1756], iLen = 0;
	new szName[33], szDate[33], szLider[33], motd[21];

	SQL_ReadResult(query, SQL_FieldNameToNum(query, "idmotd"), motd, sizeof(motd)-1);
	SQL_ReadResult(query, SQL_FieldNameToNum(query, "clan"), szName, sizeof(szName)-1);
	SQL_ReadResult(query, SQL_FieldNameToNum(query, "createDate"), szDate, sizeof(szDate)-1);
	SQL_ReadResult(query, SQL_FieldNameToNum(query, "LIDER"), szLider, sizeof(szLider)-1);
	new kills = SQL_ReadResult(query, SQL_FieldNameToNum(query, "kills"));
	new level = SQL_ReadResult(query, SQL_FieldNameToNum(query, "level"));
	new members = SQL_ReadResult(query, SQL_FieldNameToNum(query, "members"));

	new skillbar[256];
	
	skillBarMenu(skillbar, sizeof(skillbar), level, bbCvar[cvarClansLevelMax], symbolsCustom[SYMBOL_SMALL_DOT], symbolsCustom[SYMBOL_SMALL_DOT]);	
	
	iLen = format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Informacje o Klanie!^n");
	
	iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s\w %s\d %s\w %s\y %s^n", symbolsCustom[SYMBOL_R_ARROW], symbolsCustom[SYMBOL_VERTICAL_LINE], skillbar, symbolsCustom[SYMBOL_VERTICAL_LINE], symbolsCustom[SYMBOL_L_ARROW]);
	
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\y%s^t^t\dNazwa Klanu:\r %s", symbolsCustom[SYMBOL_DR_ARROW], szName);
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\y%s^t^t\dPrzywodca Klanu:\r %s", symbolsCustom[SYMBOL_DR_ARROW], szLider);
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\y%s^t^t\dCzlonkowie:\r %d/%d", symbolsCustom[SYMBOL_DR_ARROW], members,(  level / 3 ) + bbCvar[cvarClansStartMember]);
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\y%s^t^t\dZabojstwa:\r %d", symbolsCustom[SYMBOL_DR_ARROW], kills);
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\y%s^t^t\dPoziom:\r %d^n", symbolsCustom[SYMBOL_DR_ARROW], level);
	
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\y%s^t^t\dData Stworzenia:\r %s", symbolsCustom[SYMBOL_DR_ARROW], szDate);

	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n^n\r7.\r Zdjecie Informacyjne");
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\r8.\w Czlonkowie^n");
	
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\r9.\y Wroc");
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\r0.\y Wyjdz");
	
	showMenu(id, B7 | B8 | B9| B0, gText, -1, "clanInfoDesc")	;
	return PLUGIN_HANDLED;
}
public clanInfoDesc_2(id, item){
	
	switch(item){
		case 6: loadMotdClan(id);
		case 7: loadMembersClan(id);
		case 8: loadStatsSql(id,7);
		default:{ return;}
	}
}
public loadMotdClan(id){
	
	if(is_user_hltv(id)) return PLUGIN_CONTINUE;
		
	if (!sqlConnected) {
		log_amx("[SQL-LOG] Brak polaczenia");
		return PLUGIN_CONTINUE;
	}

	
	new queryData[512];
	new tempId[3];
	
	tempId[0] = id;
	tempId[1] = userViewClanInfo[id];

	format(queryData, sizeof(queryData), "\
		SELECT `idmotd`, `name` FROM `clans`\
		WHERE `idclans`='%d'", tempId[1]);
	
	SQL_ThreadQuery(sql, "loadMotdClan_2", queryData, tempId, sizeof(tempId));

	return PLUGIN_CONTINUE;
}
public loadMotdClan_2(failState, Handle:query, error[], errorNum, tempId[], dataSize){
	
	new id = tempId[0];
	
	new gMotd[21], clanName[12];
	SQL_ReadResult(query, SQL_FieldNameToNum(query, "idmotd"), gMotd, sizeof(gMotd)-1);
	SQL_ReadResult(query, SQL_FieldNameToNum(query, "name"), clanName, sizeof(clanName)-1);
	
	
	new gText[2048], iLen=0;
	new sizeText = sizeof(gText)-iLen-1;
				
	iLen += format(gText[iLen], sizeText, "<html>");
	iLen += format(gText[iLen], sizeText, "<body style=^"background: url('https://i.imgur.com/%s.png') top center #000000;margin:0px;padding:0px;background-size:100%% 100%%;background-repeat:no-repeat;^"></body>", strlen(gMotd) > 1 ? formatm("%s", gMotd) : formatm("%s", MOTDNONE));				
	iLen += format(gText[iLen], sizeText, "</html>");
	show_motd(id, gText, formatm("Klan: %s", clanName));
}

public loadMembersClan(id){
	
	if(is_user_hltv(id)) return PLUGIN_CONTINUE;
		
	if (!sqlConnected) {
		log_amx("[SQL-LOG] Brak polaczenia");
		return PLUGIN_CONTINUE;
	}

	new queryData[512];
	new tempId[3];
	
	tempId[0] = id;
	tempId[1] = userViewClanInfo[id];

	
	format(queryData, sizeof(queryData), "\
		SELECT * FROM `players`\
		WHERE clan = '%d'", tempId[1]);
	
	SQL_ThreadQuery(sql, "loadMembersClan_2", queryData, tempId, sizeof(tempId));

	return PLUGIN_CONTINUE;
}

public loadMembersClan_2(failState, Handle:query, error[], errorNum, tempId[], dataSize){
	
	new id 		= 	tempId[0];
	
	if (failState){
		if (failState == TQUERY_CONNECT_FAILED) log_amx("[SQL-LOG] Nie mozna polaczyc siê z baz¹ danych SQL. Blad: %s (%d)", error, errorNum);
		else if (failState == TQUERY_QUERY_FAILED) log_amx("[SQL-LOG] Zapytanie watkowe nie powiodlo sie. Blad: %s (%d)", error, errorNum);
		return PLUGIN_HANDLED;
	}	
	
	new gText[1756], iLen = 0;
	new szName[33];
	new copyName[11];
	new status;
	
	iLen = format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Lista Czlonkow!^n");
	
	while (SQL_MoreResults(query)) {
		SQL_ReadResult(query, SQL_FieldNameToNum(query, "name"), szName, sizeof(szName));
		status = SQL_ReadResult(query, SQL_FieldNameToNum(query, "flag"));
		copy(copyName, sizeof(copyName)-1, szName);
		
		
		if(strlen(szName) > 10 && status <= STATUS_DEPUTY)
			iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\w-\d %s...", copyName);
		else iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\w-%s %s", status == STATUS_LEADER ? "\y [Przywodca]\r" : "\d", szName);

		SQL_NextRow(query);
	}
	
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n^n\r9.\y Wroc");
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\r0.\y Wyjdz");
	
	showMenu(id, B9| B0, gText, -1, "clanInfoMember");
	return PLUGIN_HANDLED;
}

public clanInfoMember_2(id, item){
	switch(item){
		case 8: loadDescClan(id);
		default:{ return; }
	}
}

public showClanMenu(id, next){	
	if (!is_user_connected(id))  return;
	
	if( !playerLogged(id)){
		chatPrint(id, PREFIX_NORMAL, "Zaloguj sie aby kozystac z klanow!");
		return;
	}
	
	pageClan[id] = false;
	
	new bbClan[clanInfo], gText[1536], iLen;
	new skillbar[256];
	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
		
	skillBarMenu(skillbar, sizeof(skillbar), bbClan[CLAN_LEVEL], bbCvar[cvarClansLevelMax], symbolsCustom[SYMBOL_SMALL_DOT], symbolsCustom[SYMBOL_SMALL_DOT]);	
	
	iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "\r[BaseBuilder]\y Menu Klanu\d [\w %s/2\d ]^n", !next ? "1" : "2");
	
	if(!next){
		
		iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dOgloszenie:\r %s",symbolsCustom[SYMBOL_DR_ARROW],bbClan[CLAN_INFO]);
	
		iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dNazwa:\r %s", symbolsCustom[SYMBOL_DR_ARROW], bbClan[CLAN_NAME]);
		iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dCzlonkowie:\r %d/%d",symbolsCustom[SYMBOL_DR_ARROW],  bbClan[CLAN_MEMBERS],(  bbClan[CLAN_LEVEL] / 3 ) + bbCvar[cvarClansStartMember]);
		iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dPoziom:\r %d",symbolsCustom[SYMBOL_DR_ARROW], bbClan[CLAN_LEVEL]);
		iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dBrylki:\r %s",symbolsCustom[SYMBOL_DR_ARROW],formatNumber(bbClan[CLAN_NUGGET]));
		iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dZabojstwa:\r %s",symbolsCustom[SYMBOL_DR_ARROW],formatNumber(bbClan[CLAN_KILLS]));
	
		
		if (get_user_status(id) > STATUS_MEMBER){
			if(bbClan[CLAN_LEVEL] >= bbCvar[cvarClansLevelMax] ) iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\d1. Awansuj: Maksymalny Poziom");
			else iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\r1.\w Awansuj");
		} else {
			if(bbClan[CLAN_LEVEL] >= bbCvar[cvarClansLevelMax] ) iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\d[ Awans Klanu: Maksymalny Poziom ]");
			else iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\w[\d Awans Klanu");
		}
		
		iLen 	 += 	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n%s2.%s Zarzadzaj Klanem", (get_user_status(id) > STATUS_MEMBER) ? "\r" : "\d", (get_user_status(id) > STATUS_MEMBER) ? "\w" : "\d");
		iLen 	 +=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n%s3.%s Opusc Klan", (get_user_status(id) == STATUS_LEADER) ? "\d" : "\r", (get_user_status(id) == STATUS_LEADER) ? "\d" : "\w");
		iLen  	 +=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\r4.\w Wplac Brylki");
		
		
		pageClan[id] = true;
	} else {
		iLen 	 +=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\r1.\w Czlonkowie Online");
		iLen 	 +=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\r2.\w Lista Wplacajacych");
		iLen  	 +=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\r3.\w Top 10 Klanow");
		iLen 	 +=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\r4.\w Lista Klanow");
		iLen 	 +=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\r5.\w Zdjecie Informacyjne");
	}
	
	
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\r9.\y %s" ,next ? "Wroc" : "Dalej");	
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\r0.\y Wyjdz");
	
	
	showMenu(id, B1 | B2 | B3 | B4 | B5 | B6 | B7 | B8 | B9 | B0, gText, -1, "showClanMenu"  );
	
}
public showClanMenu_2(id, item){
	if (!is_user_connected(id)) return;
	
	switch (item) {
		
		case 0: {
			if(pageClan[id]){
				if (get_user_status(id) > STATUS_MEMBER) avansClanMenu(id);
				else globalClanMenu(id);
				
			} 
			else membersOnlineMenu(id);
		}
		case 1: {
			if(pageClan[id]){
				if (get_user_status(id) > STATUS_MEMBER) {
					userAcceptRestore[id] = false;
					leaderMenu(id);
				} else {
					globalClanMenu(id);
					chatPrint(id, PREFIX_NORMAL, "Nie posiadasz dostepu!");	
				}
			}else loadStatsSql(id,6 );	
		}
		case 2: {
			if(pageClan[id]){
				
				if (get_user_status(id) == STATUS_LEADER) {
					chatPrint(id, PREFIX_NORMAL, "Oddaj przywodctwo klanu jednemu z czlonkow zanim go upuscisz");	
					globalClanMenu(id);
				} else leaveConfimMenu(id);	
			} else loadStatsSql(id,5 );	
		}
		case 3: {
			if(pageClan[id]){
				client_cmd(id, "messagemode IloscBrylekKlan");
				client_print(id, print_center, "Wpisz ilosc Brylek, ktora chcesz wplacic");
				chatPrint(id, PREFIX_NORMAL, "Wpisz ilosc Brylek, ktora chcesz wplacic.");
			} else loadStatsSql(id,7 );
		}
		case 4:{
			if(!pageClan[id]){
			
				new bbClan[clanInfo];
				ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
					
				new gText[2048], iLen=0;
				new sizeText = sizeof(gText)-iLen-1;
				
				iLen += format(gText[iLen], sizeText, "<html>");
				iLen += format(gText[iLen], sizeText, "<body style=^"background: url('https://i.imgur.com/%s.png') top center #000000;margin:0px;padding:0px;background-size:100%% 100%%;background-repeat:no-repeat;^"></body>", strlen(bbClan[CLAN_MOTD]) > 1 ? formatm("%s", bbClan[CLAN_MOTD]) : formatm("%s", MOTDNONE));				
				iLen += format(gText[iLen], sizeText, "</html>");
				show_motd(id, gText, formatm("Klan: %s", bbClan[CLAN_NAME]));
			} else showClanMenu(id, true);	
		}
		case 5..7:pageClan[id] ? showClanMenu(id, false) : showClanMenu(id, true);
		case 8: pageClan[id] ? showClanMenu(id, true) : showClanMenu(id, false);
	}
}
public avansClanMenu(id){
	
	new bbClan[clanInfo], gText[1536], iLen;
	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
	new costNugget = (bbCvar[cvarClansCostLuzaczki] + bbCvar[cvarClansStartMember] * bbClan[CLAN_LEVEL]);
	new killsClan =  (bbCvar[cvarClansLevelKill] + bbCvar[cvarClansLevelKillNext] * bbClan[CLAN_LEVEL]);
	new timeUpgrade =  calcAvansClan(id);

	new availableNugget[256], availableKill[256]; 
	
	format(availableNugget, 		sizeof(availableNugget),"\w|\d Potrzebujesz:\r %d Brylek!", 	costNugget-bbClan[CLAN_NUGGET]);
	format(availableKill,		sizeof(availableKill),  	"\w|\d Potrzebujesz:\r %d Zabojstw!", 	killsClan-bbClan[CLAN_KILLS]);
	
	iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r[BaseBuilder]\y Awansowanie Klanu^n");
	
	iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dPoziom Klanu:\r %d",symbolsCustom[SYMBOL_DR_ARROW],bbClan[CLAN_LEVEL]);
	iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dBrylki Klanowe:\r %s",symbolsCustom[SYMBOL_DR_ARROW],formatNumber(bbClan[CLAN_NUGGET]));
	iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dZabojstwa Klanowe:\r %s",symbolsCustom[SYMBOL_DR_ARROW],formatNumber(bbClan[CLAN_KILLS]));
	
	
	new upgradeLeft = (bbClan[CLAN_UPGRADETIME] - get_systime());
			
	if(bbClan[CLAN_LEVEL] < bbCvar[cvarClansLevelMax]){
		
		iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\w- Potrzebne wymagania do Awansu Klanu");
		
		iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dPotrzebne Brylki:\r %s %s",symbolsCustom[SYMBOL_DR_ARROW], formatNumber(costNugget), (costNugget > bbClan[CLAN_NUGGET]) ? availableNugget : "\w| \yWymaganie spelnione!");
		iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dPotrzebne Zabojstwa:\r %s %s", symbolsCustom[SYMBOL_DR_ARROW], formatNumber(killsClan), (killsClan > bbClan[CLAN_KILLS]) ? availableKill: "\w| \yWymaganie spelnione!");
				
		if(upgradeLeft <= 0){
			if((timeUpgrade) / DAY > 0 )  	iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dCzas Oczekiwania:\r %d dzien", symbolsCustom[SYMBOL_DR_ARROW], (timeUpgrade / HOUR) % HOUR);
			else if((timeUpgrade) / HOUR > 0 )  iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dCzas Oczekiwania:\r %d godz %s%d min", symbolsCustom[SYMBOL_DR_ARROW], (timeUpgrade) / HOUR, ( timeUpgrade / MINUTE % MINUTE )<10?"0":"", ((timeUpgrade) / MINUTE) %MINUTE);
			else if( timeUpgrade / MINUTE > 0 ) iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dCzas Oczekiwania:\r %d min", symbolsCustom[SYMBOL_DR_ARROW], (timeUpgrade / MINUTE) %MINUTE);
			else iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dCzas Oczekiwania:\r %d sek", symbolsCustom[SYMBOL_DR_ARROW], timeUpgrade);
		} else {
			if((upgradeLeft) / DAY > 0 )  	iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dOdczekaj:\r %d dzien", symbolsCustom[SYMBOL_DR_ARROW], (upgradeLeft / HOUR) % HOUR);
			else if((upgradeLeft) / HOUR > 0 )  iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dOdczekaj:\r %d godz %s%d min", symbolsCustom[SYMBOL_DR_ARROW], (upgradeLeft) / HOUR, ( upgradeLeft / MINUTE % MINUTE )<10?"0":"", ((upgradeLeft) / MINUTE) %MINUTE);
			else if( upgradeLeft / MINUTE > 0 ) iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dOdczekaj:\r %d min", symbolsCustom[SYMBOL_DR_ARROW], (upgradeLeft / MINUTE) %MINUTE);
			else iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dOdczekaj:\r %d sek", symbolsCustom[SYMBOL_DR_ARROW], upgradeLeft);
		}
	} else {
		iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\w[ Twoj klan jest juz ulepszony Maksymalnie ]");
	}
	
	if(upgradeLeft > 0 || bbClan[CLAN_LEVEL] >= bbCvar[cvarClansLevelMax])
		iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\d1. Zacznij Ulepszac %s", upgradeLeft > 0 ? "[ ODCZEKAJ ]" : bbClan[CLAN_LEVEL] >= bbCvar[cvarClansLevelMax] ? "[ MAX ]" : "");
	else iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\r1.\w Zacznij Ulepszac");
	
	
	iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\d2. Reset czasu");
	
	
	iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\r0.\y Wyjdz");
	
	showMenu(id, B1 | B2 | B3 | B0  , gText, -1, "avansClanMenu"  );
}
public avansClanMenu_2(id, item){
	switch(item){
		case 0:{
			new bbClan[clanInfo];
			ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
			new upgradeLeft = 0;
			
			if (bbClan[CLAN_LEVEL] >= bbCvar[cvarClansLevelMax]) {
				chatPrint(id, PREFIX_NORMAL, "Twoj klan ma juz maksymalny Poziom.");
				avansClanMenu(id);
				return;
			}
			upgradeLeft = (bbClan[CLAN_UPGRADETIME] - get_systime());
					
			if(upgradeLeft > 0){
				new gText[128], iLen = 0;
				
				iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "Nie mozesz ulepszyc!^4 |^1 Musisz poczekac^3 ");
				
				if((upgradeLeft) / DAY > 0 ) iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "%d dzien", (upgradeLeft / HOUR) % HOUR);
				else if((upgradeLeft) / HOUR > 0 ) iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "%d godz %s%d min", (upgradeLeft) / HOUR, ( upgradeLeft / MINUTE % MINUTE )<10?"0":"", ((upgradeLeft) / MINUTE) %MINUTE);
				else if( upgradeLeft / MINUTE > 0 ) iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "%d min", (upgradeLeft / MINUTE) %MINUTE);
				else  iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "%d sek", upgradeLeft);

				
				chatPrint(id, PREFIX_LINE, gText);
				avansClanMenu(id);
				return;
			}	
			new costNugget = bbClan[CLAN_NUGGET] - (bbCvar[cvarClansCostLuzaczki] + bbCvar[cvarClansStartMember] * bbClan[CLAN_LEVEL]);
			new killsClan = bbClan[CLAN_KILLS] - (bbCvar[cvarClansLevelKill] + bbCvar[cvarClansLevelKillNext] * bbClan[CLAN_LEVEL]);
					
			if (costNugget < 0) {		
				chatPrint(id, PREFIX_NORMAL, "Twoj klan nie ma wystarczajacej ilosci Brylek."  );
				avansClanMenu(id);
				return;
			}		
			if (killsClan < 0) {		
				chatPrint(id, PREFIX_NORMAL, "Twoj klan nie ma wystarczajacej ilosci Zabojstw."  );
				avansClanMenu(id);
				return;
			}
			new upgradeTime = calcAvansClan(id);
			
			bbClan[CLAN_UPGRADETIME]	= 	max( bbClan[CLAN_UPGRADETIME] + (upgradeTime), get_systime() + (upgradeTime) );
					
			bbClan[CLAN_LEVEL] ++;
			bbClan[CLAN_NUGGET] = costNugget;
			bbClan[CLAN_POINTS] += 2;
					
			ArraySetArray(bbClans, get_clan_id(clan[id]), bbClan);
			save_clan(get_clan_id(clan[id]));
					
			
			new clanName[64];
			get_clan_info(clan[id], CLAN_NAME, clanName, sizeof(clanName));		
			logBB(id, LOG_CLAN, "promotion", "awansowal na poziom: %d | klan: %s", bbClan[CLAN_LEVEL], clanName);
		
			chatPrint(0, PREFIX_NORMAL, "^4^3 %s^1 ulepszyl klan^4 %s^1 na poziom^3 %d", userName[id],bbClan[CLAN_NAME], bbClan[CLAN_LEVEL]);
			avansClanMenu(id);
			return;
			
		}
		case 1:{
			new bbClan[clanInfo];
			ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
			bbClan[CLAN_UPGRADETIME]	= 	 get_systime();
			ArraySetArray(bbClans, get_clan_id(clan[id]), bbClan);
			save_clan(get_clan_id(clan[id]));
			
			avansClanMenu(id);
			return;
		}
	}
}
public calcAvansClan(id){
	new bbClan[clanInfo];
	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
	return (bbCvar[cvarClansLevelTimeUpgrade] * (bbClan[CLAN_LEVEL]+1));
}
public createClanHandle(id){
	if (!is_user_connected(id)  ||  !playerLogged(id) || clan[id]) return;
		
	
	if (userNugget[id] < bbCvar[cvarClansCostNugget]) {
		chatPrint(id, PREFIX_NORMAL, "Brakuje Ci Brylek!");
		return;
	}
	if(userLevel[id] < bbCvar[cvarClansCostLevel]){
		chatPrint(id, PREFIX_NORMAL, "Brakuje Ci Poziomu!");
		return;
	}
	if(userLuzCoin[id] < bbCvar[cvarClansCostLuzaczki]){
		chatPrint(id, PREFIX_NORMAL, "Brakuje Ci Luzaczkow!");
		return;
	}
	
	new clanName[64];
	
	read_args(clanName, sizeof(clanName));
	remove_quotes(clanName);
	trim(clanName);
	
	new Regex:reg, iRet, szError[32];
	reg = regex_match(clanName,"[\w]+$", iRet, szError, sizeof(szError));
		
	switch(reg) {
		case REGEX_MATCH_FAIL: {
			chatPrint(id, PREFIX_NORMAL, "Dozwolone znaki to^3 [A-Z0-9_]");
			globalClanMenu(id);
			return;
		}
		case REGEX_PATTERN_FAIL: {
			chatPrint(id, PREFIX_NORMAL, "Dozwolone znaki to^3 [A-Z0-9_]");
			globalClanMenu(id);
			return;
		}
		case REGEX_NO_MATCH: {
			chatPrint(id, PREFIX_NORMAL, "Dozwolone znaki to^3 [A-Z0-9_]");
			globalClanMenu(id);
			return;
		}
	}
	if (	equal(clanName, "*", 1)   ||
		equal(clanName, "#", 1)   ||
		equal(clanName, "$", 1)   ||
		equal(clanName, "^"", 1) ||
		equal(clanName, "(", 1)   ||
		equal(clanName, ")", 1)   ||
		equal(clanName, "+", 1)   ||
		equal(clanName, "/", 1)   ||
		equal(clanName, "\", 1) ||
		equal(clanName, "-", 1)) {
			
		chatPrint(id, PREFIX_NORMAL, "Dozwolone znaki to^3 [A-Z0-9_]");
		globalClanMenu(id);
		return ;
	}	
	if (equal(clanName, "")) {
		chatPrint(id, PREFIX_NORMAL, "Nie wpisales nazwy klanu");
		globalClanMenu(id);
		return ;
	}
	if (strlen(clanName) >  maxSymbolClan) {
		chatPrint(id, PREFIX_NORMAL, "Nazwa klanu nie moze miec wiecej niz %d znakow", maxSymbolClan);
		globalClanMenu(id);
		return ;
	}
	if (strlen(clanName) < minSymbolClan) {
		chatPrint(id, PREFIX_NORMAL, "Nazwa klanu musi miec co najmniej %d znaki", minSymbolClan);
		globalClanMenu(id);
		return ;
	}
	if(equali(clanName, "admin", 3) || equali(clanName, "opiekun serwera", 7) || equali(clanName, "Wlasciciel", 10)){
		chatPrint(id, PREFIX_NORMAL, "Zakaz nazwy kalnu: Admin");
		globalClanMenu(id);
		return;
	}
	if (check_clan_name(clanName)) {
		chatPrint(id, PREFIX_NORMAL, "Klan z taka nazwa juz istnieje");
		globalClanMenu(id);
		return ;
	}

	createClan(id, clanName);
	
	logBB(id, LOG_CLAN, "create", "stworzyl klan %s", clanName);

	chatPrint(0, PREFIX_LINE, "Gracz^3 %s^1 zalozyl klan:^4 [^3 %s^4 ] ", userName[id], clanName);
	
	userNugget[id] -= bbCvar[cvarClansCostNugget];
	userLuzCoin[id] -= bbCvar[cvarClansCostLuzaczki];
	
	globalClanMenu(id);

}

public leaveConfimMenu(id){
	if (!is_user_connected(id) || !clan[id]) return;
	
	new menu = menu_create("\r[BaseBuilder]\y Jestes pewien, ze chcesz opuscic klan?", "leaveConfimMenu_2");
	
	menu_additem(menu, "Tak");
	menu_additem(menu, "Nie");
	
	menu_setprop(menu, MPROP_EXITNAME, "Wyjscie");
	
	menu_display(id, menu, 0);

}

public leaveConfimMenu_2(id, menu, item){
	if (!is_user_connected(id) || !clan[id]) return;
	
	if (item == MENU_EXIT) {
		menu_destroy(menu);
		return;
	}
	switch (item) {
		case 0: {
			if (get_user_status(id) == STATUS_LEADER) {
				chatPrint(id, PREFIX_NORMAL, "Oddaj przywodctwo klanu jednemu z czlonkow zanim go upuscisz");
				globalClanMenu(id);
				return;
			}
			chatPrint(id, PREFIX_NORMAL, "Opusciles swoj klan");
			
			new clanName[64];
			get_clan_info(clan[id], CLAN_NAME, clanName, sizeof(clanName));	
			logBB(id, LOG_CLAN, "leave", "opuscil klan %s",  clanName);
	
			set_user_clan(id);
			globalClanMenu(id);
		}
		case 1: globalClanMenu(id);
	}
}

public membersOnlineMenu(id){
	if (!is_user_connected(id) || !clan[id]) return;
	
	new gText[128], online = 0;
	
	new menu = menu_create("\r[BaseBuilder]\y Czlonkowie Online:", "membersOnlineMenu_2");
	
	for (new i = 1; i < maxPlayers; i++) {
		if (!is_user_connected(id) || clan[id] != clan[i])  continue;
			
		online++;
		
		switch (get_user_status(i)) {
			case STATUS_MEMBER: format(gText, sizeof(gText), "%s\d [Czlonek]", userName[i]);
			case STATUS_DEPUTY: format(gText, sizeof(gText), "%s\y [Zastepca]", userName[i]);
			case STATUS_LEADER: format(gText, sizeof(gText), "%s\r [Przywodca]", userName[i]);
		}
		menu_additem(menu, gText);
	}
	menu_setprop(menu, MPROP_EXITNAME, "Wyjscie");
	menu_setprop(menu, MPROP_BACKNAME, "Poprzednie");
	menu_setprop(menu, MPROP_NEXTNAME, "Nastepne");
	
	if (!online) chatPrint(id, PREFIX_NORMAL, "Na serwerze nie ma zadnego czlonka twojego klanu!");
	else menu_display(id, menu);
}

public membersOnlineMenu_2(id, menu, item){
	if (!is_user_connected(id) || !clan[id]) return;
	
	if (item == MENU_EXIT) {
		menu_destroy(menu);
		globalClanMenu(id);
		return;
	}
	membersOnlineMenu(id);
}
public leaderMenu(id){
	if (!is_user_connected(id) || !clan[id]) return;
	
	new gText[128];
	
	new menu = menu_create("\r[BaseBuilder]\y Zarzadzaj Klanem", "leaderMenu_2");

	menu_additem(menu, "\wRozwiaz Klan");
	menu_additem(menu, "\wUlepsz Umiejetnosci");
	menu_additem(menu, "\wZapros Gracza");
	menu_additem(menu, "\wZarzadzaj Czlonkami");
	
	format(gText, sizeof(gText), "\wZresetuj Umiejetnosci:\d [\r %d Brylek\d ] %s", bbCvar[cvarClansRestore], userAcceptRestore[id] ? "\r[POTWIERDZ]" : "");
	menu_additem(menu, gText);
	
	menu_additem(menu, "Ogloszenie Klanowe\d (\r MAX 42 symbole\d )");
	menu_additem(menu, "MOTD KLANU");
	
	menu_setprop(menu, MPROP_EXITNAME, "Wyjscie");
	
	menu_display(id, menu, 0);

	return;
}
public leaderMenu_2(id, menu, item){
	if (!is_user_connected(id) || !clan[id]) return;
	
	if (item == MENU_EXIT) {
		menu_destroy(menu);
		globalClanMenu(id);
		return;
	}
	
	switch (item) {
		case 0: {
			if(get_user_status(id) == STATUS_LEADER) disbandMenu(id);
			else chatPrint(id, PREFIX_NORMAL, "Tylko wlasciciel klanu moze to zrobic!");
		}
		case 1: {
			if(get_user_status(id) > STATUS_MEMBER) upgradeClanMenu(id);
			else chatPrint(id, PREFIX_NORMAL, "Tylko wlasciciel oraz zastepca klanu moze ulepszac!");
		}
		case 2: inviteMenu(id);
		case 3: loadStatsSql(id,8);
		case 4:{
			if(get_user_status(id) == STATUS_LEADER){
				new bbClan[clanInfo];
				ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
				
				if(userAcceptRestore[id]){
				
					if(bbClan[CLAN_NUGGET] < bbCvar[cvarClansRestore]){
						chatPrint(id, PREFIX_NORMAL, "Brakuje Brylek");
						userAcceptRestore[id] = false;
						return;
					}
					
					bbClan[CLAN_NUGGET] -= bbCvar[cvarClansRestore];
					bbClan[CLAN_POINTS] = bbClan[CLAN_LEVEL] * 2;
					
					bbClan[CLAN_HEALTH] = 0;
					bbClan[CLAN_DAMAGECLASS] = 0;
					bbClan[CLAN_DAMAGEPLAYER] = 0;
					bbClan[CLAN_CRITIC] = 0;
					bbClan[CLAN_COOLDOWN] = 0;
					bbClan[CLAN_EXPDROP] = 0;
					bbClan[CLAN_NUGGETDROP] = 0;
					
					chatPrint(id, PREFIX_NORMAL, "Zresetowales Umiejetnosci!");
				
					ArraySetArray(bbClans, get_clan_id(clan[id]), bbClan);
					save_clan(get_clan_id(clan[id]));
					userAcceptRestore[id] = false;
					
					new clanName[64];
					get_clan_info(clan[id], CLAN_NAME, clanName, sizeof(clanName));	
					logBB(id, LOG_CLAN, "reset", "zresetowal statystyki | klan: %s",  clanName);
	
					
					return;
				}
				userAcceptRestore[id] = true;
				leaderMenu(id);
				return;
	
			} else chatPrint(id, PREFIX_NORMAL, "Tylko wlasciciel klanu moze to zrobic!");
		}
		case 5:{
			if(get_user_status(id) == STATUS_LEADER){
				client_print(id, print_center, "Wpisz teraz nowe ogloszenie dla Klanowiczow!");
				cmd_execute(id, "messagemode Ogloszenie_Klanu");
				leaderMenu(id);
			} else chatPrint(id, PREFIX_NORMAL, "Tylko wlasciciel klanu moze to zrobic!");
			
		}
		case 6:{
			if(get_user_status(id) > STATUS_MEMBER){
				motdClanMenu(id);
			} else chatPrint(id, PREFIX_NORMAL, "Tylko wlasciciel oraz zastepca klanu moze ulepszac!");
		}
	}
}
public motdClanMenu(id){
	
	if (!is_user_connected(id) || !clan[id] || !(get_user_status(id) >  STATUS_MEMBER)) return;
	
	new gText[512], iLen = 0;
	
	new bbClan[clanInfo];
	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
		
	iLen 	 +=	 format(gText[iLen], sizeof(gText) - iLen - 1, "\r[BaseBuilder]\y MOTD KLANU^n");
	iLen 	 +=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\dWpisz symbole z linku do zdjecia\r ( imgur.com )");
	iLen 	 +=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\yhttps://imgur.com/\rxyz\w ( podajesz te symbole po\r /...\w )");
	iLen 	 +=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\dCzyli w tym przypadku tylko wpisujesz\r xyz^n");

	if(equal(bbClan[CLAN_MOTD], "_") || strlen(bbClan[CLAN_MOTD]) <  1)
		iLen 	 +=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dTwoj Aktualny kod:\r %s", symbolsCustom[SYMBOL_DR_ARROW], "Nie wpisales jeszcze");
	else iLen 	 +=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dTwoj Aktualny kod:\y %s", symbolsCustom[SYMBOL_DR_ARROW], bbClan[CLAN_MOTD]);


	new menu = menu_create(gText, "motdClanMenu_2");
	
	menu_additem(menu, "Wpisz kod do zdjecia");
	menu_additem(menu, "Resetuj");
	
	menu_display(id, menu, 0);
}	


public motdClanMenu_2(id, menu, item){
	if (!is_user_connected(id) || !clan[id] || !(get_user_status(id) >  STATUS_MEMBER)) return;
	
	if(item == MENU_EXIT){
		menu_destroy(menu);
		globalClanMenu(id);
		return;
	}
	
	switch(item){
		case 0:{
			cmd_execute(id, "messagemode Kod_Motd_Klanu");
			motdClanMenu(id);
		}
		case 1:{
			new bbClan[clanInfo];
			ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
			
			bbClan[CLAN_MOTD] = "_";
			
			chatPrint(id, PREFIX_LINE, "Zresetowales^3 MOTD^1 klanu!");
			
			ArraySetArray(bbClans, get_clan_id(clan[id]), bbClan);
			
			save_clan(get_clan_id(clan[id]));
			
			motdClanMenu(id);
		}
	}
}
public checkCodeMotd(id){
	if(!(get_user_status(id) > STATUS_MEMBER || is_user_connected(id) || clan[id])) return PLUGIN_CONTINUE;
	
	new bbClan[clanInfo];
	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
	
	new codeMotd[20];
	read_args(codeMotd, sizeof(codeMotd));
	remove_quotes(codeMotd);
	trim(codeMotd);
	
	if (equal(codeMotd, "") || strlen(codeMotd) <  1 ) {
		chatPrint(id, PREFIX_LINE, "Nic nie wpisales!");
		return PLUGIN_HANDLED;
		
	}
	if (strlen(codeMotd) >= 20+1 ) {
		chatPrint(id, PREFIX_LINE, "Za dlugi kod");
		return PLUGIN_HANDLED;
	}
	
	bbClan[CLAN_MOTD] = codeMotd;
	
	chatPrint(id, PREFIX_LINE, "Ustawiles^3 MOTD^1 klanu!^3 (^4%s^3)", codeMotd);
	
	ArraySetArray(bbClans, get_clan_id(clan[id]), bbClan);
	
	save_clan(get_clan_id(clan[id]));
	
	motdClanMenu(id);
	
	return PLUGIN_HANDLED;
}
public disbandMenu(id){
	if (!is_user_connected(id) || !clan[id]) return;
	
	new menu = menu_create("\r[BaseBuilder]\y Czy na pewno chcesz rozwiazac klan?", "disbandMenu_2");
	
	menu_additem(menu, "Tak");
	menu_additem(menu, "Nie");
	
	menu_setprop(menu, MPROP_EXITNAME, "Wyjscie");
	
	menu_display(id, menu);

}

public disbandMenu_2(id, menu, item){
	if (!is_user_connected(id) || !clan[id]) return;
	
	if (item == MENU_EXIT) {
		menu_destroy(menu);
		return;
	}

	switch (item) {
		case 0: {
			chatPrint(id, PREFIX_NORMAL, "Wpisz swoja nazwe klanu aby go usunac!");
			client_print(id, print_center, "Wpisz swoja nazwe klanu aby go usunac!");
			client_cmd(id, "messagemode Potwierdz_Nazwe_Klanu");

		}
		case 1: globalClanMenu(id);
	}
}

public checkNameClan(id){
	if(!(get_user_status(id) == STATUS_LEADER || is_user_connected(id) || clan[id])) return;
	
	new nameClan[17];
	read_args(nameClan, sizeof(nameClan));
	remove_quotes(nameClan);
	trim(nameClan);
	
	new bbClan[clanInfo];
	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
				
	if(equal(nameClan, bbClan[CLAN_NAME])){
		chatPrint(id, PREFIX_NORMAL, "Rozwiazales swoj klan!");
	
		new clanName[64];
		get_clan_info(clan[id], CLAN_NAME, clanName, sizeof(clanName));	
		logBB(id, LOG_CLAN, "delete", "usunal klan: %s",  clanName);
		
		removeClan(id);
	} else  chatPrint(id, PREFIX_NORMAL, "Wpisz poprawna nazwe klanu!");
}

public upgradeClanMenu(id){
	if (!is_user_connected(id) || !clan[id]) return;
	
	new bbClan[clanInfo], gText[1536], iLen = 0;

	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
	
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r[BaseBuilder]\y Ulepszanie^n^n");
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\y%s^t^t\dKoszt:\r 1 punkt^n", symbolsCustom[SYMBOL_DR_ARROW]);
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\y%s^t^t\dPunkty:\r %d\w^n^n", symbolsCustom[SYMBOL_DR_ARROW], bbClan[CLAN_POINTS]);


	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r1.\w Exp\r [\d%d/%d\r]^n", bbClan[CLAN_EXPDROP], bbCvar[cvarClansUpgradeExp]);
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r2.\w Brylki\r [\d%d/%d\r]^n", bbClan[CLAN_NUGGETDROP], bbCvar[cvarClansUpgradeNugget]);
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r3.\w HP\r [\d%d/%d\r]^n", bbClan[CLAN_HEALTH], bbCvar[cvarClansUpgradeHealth]);
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r4.\w Kryt\r [\d%d/%d\r]^n",  bbClan[CLAN_CRITIC], bbCvar[cvarClansUpgradeCritic]);
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r5.\w DMG\r [\d%d/%d\r]^n", bbClan[CLAN_DAMAGEPLAYER], bbCvar[cvarClansUpgradeDamagePlayer]);
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r6.\w DMG Klasy\r [\d%d/%d\r]^n",  bbClan[CLAN_DAMAGECLASS], bbCvar[cvarClansUpgradeDamageClass]);
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r7.\w CoolDown\r [\d%d/%d\r] ^n", bbClan[CLAN_COOLDOWN], bbCvar[cvarClansUpgradeCoolDown]);
	
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\r0.\y Wyjscie");
	
	showMenu(id, B1 | B2 | B3 | B4 | B5 | B6 | B7 |  B9 | B0, gText, -1, "upgradeClanMenu"  );

}

public upgradeClanMenu_2(id, item){
	if (!is_user_connected(id) || !clan[id]) return;
	
	new bbClan[clanInfo], upgradedSkill, upgradedSkillName[33];	
	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);

	if ((item ==  0 || item ==  1 || item ==  2 || item ==  3 || item ==  4 || item ==  5 || item ==  6 || item ==  7 || item ==  8 ) && bbClan[CLAN_POINTS] < 1) {
		chatPrint(id, PREFIX_NORMAL, "Twoj klan nie ma wystarczajacej ilosci Punktow");
		upgradeClanMenu(id);
		return;
	}
	switch (item) {
		case 0: {
			upgradedSkill = CLAN_EXPDROP;
			if (bbClan[upgradedSkill] == bbCvar[cvarClansUpgradeExp]) {
				chatPrint(id, PREFIX_NORMAL, "Twoj klan ma juz maksymalny poziom tej umiejetnosci");
				upgradeClanMenu(id);
				return;
			}
			upgradedSkillName = "Dodatkowy EXP";
			bbClan[upgradedSkill]++;
		
			chatPrint(id, PREFIX_NORMAL, "Ulepszyles umiejetnosc^3 %s^1 na^3 %d^1 poziom!", upgradedSkillName, bbClan[upgradedSkill]);
		}
		case 1: {
			upgradedSkill = CLAN_NUGGETDROP;
			if (bbClan[upgradedSkill] == bbCvar[cvarClansUpgradeNugget]) {
				chatPrint(id, PREFIX_NORMAL, "Twoj klan ma juz maksymalny poziom tej umiejetnosci");
				upgradeClanMenu(id);
				return;
			}
			upgradedSkillName = "Dodatkowe Brylki";
			bbClan[upgradedSkill]++;
		
			chatPrint(id, PREFIX_NORMAL, "Ulepszyles umiejetnosc^3 %s^1 na^3 %d^1 poziom!", upgradedSkillName, bbClan[upgradedSkill]);
		}
		case 2: {
			upgradedSkill = CLAN_HEALTH;
			if (bbClan[upgradedSkill] == bbCvar[cvarClansUpgradeHealth]) {
				chatPrint(id, PREFIX_NORMAL, "Twoj klan ma juz maksymalny poziom tej umiejetnosci");
				upgradeClanMenu(id);
				return;
			}
			upgradedSkillName = "Dodatkowe Zycie";
			bbClan[upgradedSkill]++;
		
			chatPrint(id, PREFIX_NORMAL, "Ulepszyles umiejetnosc^3 %s^1 na^3 %d^1 poziom!", upgradedSkillName, bbClan[upgradedSkill]);
		}
		case 3: {
			upgradedSkill = CLAN_CRITIC;
			if (bbClan[upgradedSkill] == bbCvar[cvarClansUpgradeCritic]) {
				chatPrint(id, PREFIX_NORMAL, "Twoj klan ma juz maksymalny poziom tej umiejetnosci");
				upgradeClanMenu(id);
				return;
			}
			upgradedSkillName = "Krytyk";
			bbClan[upgradedSkill]++;
		
			chatPrint(id, PREFIX_NORMAL, "Ulepszyles umiejetnosc^3 %s^1 na^3 %d^1 poziom!", upgradedSkillName, bbClan[upgradedSkill]);
		}
		case 4: {
			upgradedSkill = CLAN_DAMAGEPLAYER;
			if (bbClan[upgradedSkill] == bbCvar[cvarClansUpgradeDamagePlayer]) {
				chatPrint(id, PREFIX_NORMAL, "Twoj klan ma juz maksymalny poziom tej umiejetnosci");
				upgradeClanMenu(id);
				return;
			}
			upgradedSkillName = "Obrazenia Postaci";
			bbClan[upgradedSkill]++;
		
			chatPrint(id, PREFIX_NORMAL, "Ulepszyles umiejetnosc^3 %s^1 na^3 %d^1 poziom!", upgradedSkillName, bbClan[upgradedSkill]);
		}
		case 5: {
			upgradedSkill = CLAN_DAMAGECLASS;
			if (bbClan[upgradedSkill] == bbCvar[cvarClansUpgradeDamageClass]) {
				chatPrint(id, PREFIX_NORMAL, "Twoj klan ma juz maksymalny poziom tej umiejetnosci");
				upgradeClanMenu(id);
				return;
			}
			upgradedSkillName = "Obrazenia Klasy";
			bbClan[upgradedSkill]++;
		
			chatPrint(id, PREFIX_NORMAL, "Ulepszyles umiejetnosc^3 %s^1 na^3 %d^1 poziom!", upgradedSkillName, bbClan[upgradedSkill]);
		}
		case 6: {
			upgradedSkill = CLAN_COOLDOWN;
			if (bbClan[upgradedSkill] == bbCvar[cvarClansUpgradeCoolDown]) {
				chatPrint(id, PREFIX_NORMAL, "Twoj klan ma juz maksymalny poziom tej umiejetnosci");
				upgradeClanMenu(id);
				return;
			}
			upgradedSkillName = "Czas odnowy";
			bbClan[upgradedSkill]++;

			chatPrint(id, PREFIX_NORMAL, "Ulepszyles umiejetnosc^3 %s^1 na^3 %d^1 poziom!", upgradedSkillName, bbClan[upgradedSkill]);
		}
		
		default:{ return; }
	}
	
	bbClan[CLAN_POINTS]--;
	
	ArraySetArray(bbClans, get_clan_id(clan[id]), bbClan);

	save_clan(get_clan_id(clan[id]));
	
	
	
	for (new i = 1; i < maxPlayers; i++) {
		if (!is_user_connected(id) || i == id || clan[i] != clan[id]) continue;
			
		chatPrint(i,PREFIX_NORMAL, "%s^1 ulepszyl %s na^3 %d Poziom^1!", userName[id], upgradedSkillName, bbClan[upgradedSkill]);
	}
	
	
	new clanName[64];
	get_clan_info(clan[id], CLAN_NAME, clanName, sizeof(clanName));	
	logBB(id, LOG_CLAN, "upgrade", "ulepszyl %s na poziom %d | klan: %s", upgradedSkillName, bbClan[upgradedSkill], clanName);
	
	upgradeClanMenu(id);
	
	return;
}
public inviteMenu(id){	
	if (!is_user_connected(id) || !clan[id]) return;
	
	new gText[128], online = 0;
	
	new menu = menu_create("\yWybierz \rGracza \ydo zaproszenia:", "inviteMenu_2");
	
	for (new i = 1, x = 0; i < maxPlayers; i++) {
		if (!is_user_connected(i) || is_user_hltv(i) || is_user_bot(id) || i == id || clan[i]) continue;

		online++;
		
		format(gText, sizeof(gText), "%s", userName[i]);
		menu_additem(menu, gText);
		
		menuList[id][x++] = i;
	}	
	
	if (!online) chatPrint(id, PREFIX_NORMAL, "Na serwerze nie ma gracza, ktorego moglbys zaprosic!");
	else menu_display(id, menu ,0);
}

public inviteMenu_2(id, menu, item){
	if (item == MENU_EXIT) {
		menu_destroy(menu);
		globalClanMenu(id);
		return ;
	}

	new target = menuList[id][item];

	if (!is_user_connected(target)) {
		chatPrint(id, PREFIX_NORMAL, "Wybranego gracza nie ma juz na serwerze.");
		return;
	} 
	userClanSend[target] = id;
	
	inviteConfirmMenu(target);

	chatPrint(id, PREFIX_NORMAL, "Zaprosiles^3 %s^1 do do twojego klanu.", userName[target]);
	
	globalClanMenu(id);
}

public inviteConfirmMenu(id){	
	
	if(!is_user_connected(id)) return;

	new gText[128], clanName[64];
	get_clan_info(clan[userClanSend[id]], CLAN_NAME, clanName, sizeof(clanName));
	
	format(gText, sizeof(gText), "\r[BaseBuilder]\y Zostales zaproszony do klnau:\r %s\y przez\r %s", clanName, userName[userClanSend[id]]);
	
	new menu = menu_create(gText, "inviteConfirmMenu_2");

	menu_additem(menu, "Dolacz");
	menu_additem(menu, "Odrzuc");
	
	menu_display(id, menu, 0);	
}

public inviteConfirmMenu_2(id, menu, item){
	if (item == MENU_EXIT) {
		menu_destroy(menu);
		return;
	}
	
	new target = userClanSend[id];
	
	switch(item){
		case 0:{
			if (!is_user_connected(target)) {
				chatPrint(id, PREFIX_NORMAL, "Gracza, ktory cie zaprosil nie ma juz na serwerze");
				return;
			}
		
			if (((get_clan_info(clan[target], CLAN_LEVEL) / 3 ) + bbCvar[cvarClansStartMember]) <= get_clan_info(clan[target], CLAN_MEMBERS)) {
				chatPrint(id, PREFIX_NORMAL, "Niestety, w tym klanie nie ma juz wolnego miejsca.");
				return;
			}
		
			new clanName[64];
		
			get_clan_info(clan[target], CLAN_NAME, clanName, sizeof(clanName));
			
			set_user_clan(id, clan[target]);
			
			chatPrint(id, PREFIX_NORMAL, "Dolaczyles do klanu^3 %s^01.", clanName);
			
			logBB(id, LOG_CLAN, "add", "dodal: %s | klan: %s", userName[id],  clanName);
	
			
		}
		default:{}
	}
}

public memberMenu_2(id, menu, item){
	if (!is_user_connected(id) || get_user_status(id) < STATUS_DEPUTY) return PLUGIN_HANDLED;

	if (item == MENU_EXIT) {
		menu_destroy(menu);
		return PLUGIN_HANDLED;
	}

	new itemData[33], szName[33], tempFlag[6], itemAccess, itemCallback;

	menu_item_getinfo(menu, item, itemAccess, itemData, sizeof(itemData), _, _, itemCallback);

	menu_destroy(menu);

	strtok(itemData, szName, sizeof(szName), tempFlag, sizeof(tempFlag), '#');

	new flag = str_to_num(tempFlag), userId = get_user_index(szName);

	if (userId == id) {
		chatPrint(id, PREFIX_NORMAL, "Nie mozesz zarzadzac soba!");

		loadStatsSql(id, 8);

		return PLUGIN_HANDLED;
	}

	if (clan[userId]) chosenId[id] = get_user_userid(userId);

	if (flag == STATUS_LEADER) {
		chatPrint(id, PREFIX_NORMAL, "Nie mozna zarzadzac przywodca klanu!");

		loadStatsSql(id, 8);

		return PLUGIN_HANDLED;
	}

	if(get_user_status(id) == STATUS_DEPUTY && get_user_status(userId) == STATUS_DEPUTY){
		chatPrint(id, PREFIX_NORMAL, "Nie mozna zarzadzac osoba co ma taka sama range!");
		loadStatsSql(id, 8);
		return PLUGIN_HANDLED;
	}
	
	format(chosenName[id], sizeof(chosenName), szName);

	new menu = menu_create("\r[BaseBuilder]\y Opcje Gracza", "member_options_menu_handle");

	if (get_user_status(id) == STATUS_LEADER) {
		menu_additem(menu, "Przekaz Przywodctwo", "1");

		if (flag == STATUS_MEMBER) menu_additem(menu, "Mianuj \yZastepce", "2");
		if (flag == STATUS_DEPUTY) menu_additem(menu, "Degraduj \yZastepce", "3");
	}

	menu_additem(menu, "Wyrzuc \yGracza", "4");

	menu_setprop(menu, MPROP_EXITNAME, "Wyjscie");

	menu_display(id, menu, 0);

	return PLUGIN_CONTINUE;
}


public member_options_menu_handle(id, menu, item){
	if (!is_user_connected(id) || get_user_status(id) < STATUS_DEPUTY) return PLUGIN_HANDLED;
	
	if (item == MENU_EXIT) {
		
		globalClanMenu(id);

		menu_destroy(menu);

		return PLUGIN_HANDLED;
	}
	new itemData[6], itemAccess, itemCallback;

	menu_item_getinfo(menu, item, itemAccess, itemData, sizeof(itemData), _, _, itemCallback);

	
	switch (str_to_num(itemData)) {
		case 1:   update_member(id, STATUS_LEADER);
		case 2:	update_member(id, STATUS_DEPUTY);
		case 3:	update_member(id, STATUS_MEMBER);
		case 4:   update_member(id, STATUS_NONE);
			
		
	}

	return PLUGIN_CONTINUE;
}

public update_member(id, status){
	new bool:playerOnline;
	new logText[128];

	new clanName[64];
	get_clan_info(clan[id], CLAN_NAME, clanName, sizeof(clanName));
	
	
	for (new i = 1; i <= maxPlayers; i++) {
		if (!is_user_connected(i) || clan[i] != clan[id]) continue;
	
		if (get_user_userid(i) == chosenId[id]) {
			switch (status) {
				case STATUS_LEADER: {
					set_user_status(id, STATUS_DEPUTY);
					set_user_status(i, STATUS_LEADER);
			
					chatPrint(i, PREFIX_NORMAL, "Zostales mianowany przywodca klanu.");
					
					format(logText, sizeof(logText), "mianowal nowego przywodce klanu: %s |  klan: %s", userName[i],  clanName);
					
				}
				case STATUS_DEPUTY: {
					set_user_status(i, STATUS_DEPUTY);
	
					chatPrint(i, PREFIX_NORMAL, "Zostales zastepca przywodcy klanu!");		
						
					format(logText, sizeof(logText), "mianowal nowego zastepce klanu: %s |  klan: %s", userName[i],  clanName);
						
				}
				case STATUS_MEMBER: {
					set_user_status(i, STATUS_MEMBER);
	
					chatPrint(i, PREFIX_NORMAL, "Zostales zdegradowany do rangi czlonka klanu.");
			
					format(logText, sizeof(logText), "zdegradowal: %s do czlonka klanu | klan: %s", userName[i],  clanName);	
				}
				case STATUS_NONE: {
					set_user_clan(i);
	
					chatPrint(i, PREFIX_NORMAL, "Zostales wyrzucony z klanu.");
						
					format(logText, sizeof(logText), "usunal gracza: %s | klan: %s", userName[i],  clanName);
				}
			}
	
			playerOnline = true;
	
			continue;
		}
			
		switch (status) {
			case STATUS_LEADER: chatPrint(i, PREFIX_NORMAL, "%s^01 zostal nowym przywodca klanu.", chosenName[id]);
			case STATUS_DEPUTY: chatPrint(i, PREFIX_NORMAL, "%s^1 zostal zastepca przywodcy klanu.",  chosenName[id]);
			case STATUS_MEMBER: chatPrint(i, PREFIX_NORMAL, "%s^1 zostal zdegradowany do rangi czlonka klanu.", chosenName[id]);
			case STATUS_NONE: chatPrint(i, PREFIX_NORMAL, "%s^01 zostal wyrzucony z klanu.", chosenName[id]);
		}
	}
	
	if (!playerOnline) {
		save_member(id, status, _, chosenName[id]);
		
		if (status == STATUS_NONE) set_clan_info(clan[id], CLAN_MEMBERS, get_clan_info(clan[id], CLAN_MEMBERS) - 1);

		if (status == STATUS_LEADER) set_user_status(id, STATUS_DEPUTY);
	}

	logBB(id, LOG_CLAN, "manage", logText);
	
	globalClanMenu(id);
	
	return PLUGIN_HANDLED;
}
public depositNuggetHandle(id){
	if (!is_user_connected(id) || !clan[id]|| !playerLogged(id)) return PLUGIN_HANDLED;

	
	new nuggetData[16], nuggetAmount;
	
	read_args(nuggetData, sizeof(nuggetData));
	remove_quotes(nuggetData);

	nuggetAmount = str_to_num(nuggetData);

	if (nuggetAmount <= 0) { 
		chatPrint(id, PREFIX_NORMAL, "Nie mozesz wplacic mniej niz^3 1 Brylek^1!");
		return PLUGIN_HANDLED;
	}
	
	if (userNugget[id] < nuggetAmount) { 
		chatPrint(id, PREFIX_NORMAL, "Nie masz tyle^3 Brylek^1!");
		return PLUGIN_HANDLED;
	}

	userNugget[id] -= nuggetAmount;
	
	set_clan_info(clan[id], CLAN_NUGGET, get_clan_info(clan[id], CLAN_NUGGET) + nuggetAmount);

	add_deposited_nugget(id, nuggetAmount);
	
	chatPrint(id, PREFIX_NORMAL, "Wplaciles^3 %s^1 Brylek do klanu.", formatNumber(nuggetAmount));
	chatPrint(id, PREFIX_NORMAL, "Aktualnie twoj klan ma^3 %s^1 Brylek.", formatNumber(get_clan_info(clan[id], CLAN_NUGGET)));
	

	new clanName[64];	

	logBB(id, LOG_CLAN, "deposit", "wplacil [ %d Brylek ( Aktualnie: %d ) ] | klan: %s", nuggetAmount, get_clan_info(clan[id], CLAN_NUGGET), clanName);
	
	return PLUGIN_HANDLED;
}




public  updateInfo(id){
	if (!is_user_connected(id) || !clan[id]|| !playerLogged(id)) return;

	
	new bbClan[clanInfo];
	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);

	new szArg[MAXLEN];
	
	read_args(szArg, sizeof(szArg));
	remove_quotes(szArg);
	trim(szArg);
	
	if (equal(szArg, "") || strlen(szArg) <  1 ) {
		chatPrint(id, PREFIX_LINE, "Nic nie wpisales!");
		return ;
		
	}
	if (strlen(szArg) >= 43 ) {
		chatPrint(id, PREFIX_LINE, "Za dluga wiadomosc");
		return ;
	}
	bbClan[CLAN_INFO] = szArg;

	ArraySetArray(bbClans, get_clan_id(clan[id]), bbClan);
	save_clan(get_clan_id(clan[id]));
	
	chatPrint(id, PREFIX_LINE, "Nowe ogloszenie:^3 %s", szArg);
	
}

stock set_user_clan(id, playerClan = 0, owner = 0){
	if (!is_user_connected(id)) return;
	
	if (playerClan == 0) {
		set_clan_info(clan[id], CLAN_MEMBERS, get_clan_info(clan[id], CLAN_MEMBERS) - 1);

		TrieDeleteKey(Trie:get_clan_info(clan[id], CLAN_STATUS), userName[id]);
		
		save_member(id, STATUS_NONE);
		
		clan[id] = 0;
	} else {
		clan[id] = playerClan;
		
		set_clan_info(clan[id], CLAN_MEMBERS, get_clan_info(clan[id], CLAN_MEMBERS) + 1);

		TrieSetCell(Trie:get_clan_info(clan[id], CLAN_STATUS), userName[id], owner ? STATUS_LEADER : STATUS_MEMBER);
		
		save_member(id, owner ? STATUS_LEADER : STATUS_MEMBER, 1);
	}
}
stock set_user_status(id, status){
	if (!is_user_connected(id) || !clan[id]) return;

	TrieSetCell(Trie:get_clan_info(clan[id], CLAN_STATUS), userName[id], status);
	
	save_member(id, status);
}
stock get_user_status(id){
	if (!is_user_connected(id) || !clan[id]) return STATUS_NONE;
	
	new status;

	TrieGetCell(Trie:get_clan_info(clan[id], CLAN_STATUS), userName[id], status);
	
	return status;
}
stock save_member(id, status = 0, change = 0, const name[] = ""){
	new queryData[128], safeName[64];

	if (strlen(name)) replace_all_string(name, safeName, sizeof(safeName));
	else copy(safeName, sizeof(safeName), userName[id]);

	if (status) {
		if (change) format(queryData, sizeof(queryData), "UPDATE `players` SET clan = '%d', flag = '%d' WHERE name = '%s'", clan[id], status, safeName);
		else format(queryData, sizeof(queryData), "UPDATE `players` SET flag = '%d' WHERE name = '%s'", status, safeName);
	} else format(queryData, sizeof(queryData), "UPDATE `players` SET clan = '0', flag = '0', clannugget = '0' WHERE name = '%s'", safeName);

	SQL_ThreadQuery(sql, "saveStatsHandlerSql", queryData);

}
stock add_deposited_nugget(id, nugget){
	new queryData[128];

	format(queryData, sizeof(queryData), "UPDATE `players` SET clannugget = clannugget + %d WHERE name = '%s'", nugget, userName[id]);

	SQL_ThreadQuery(sql, "saveStatsHandlerSql", queryData);
}
stock update_clan_name(clan, clanName[], clanNameLength){
	new queryData[512], oldClanName[MAX_NAME], safeOldClanName[MAX_SAFE_NAME], safeClanName[MAX_SAFE_NAME];

	get_clan_info(clan, CLAN_NAME, oldClanName, sizeof(oldClanName));
	set_clan_info(clan, CLAN_NAME, _, clanName, clanNameLength);

	replace_all_string(oldClanName, safeOldClanName, sizeof(safeOldClanName));
	replace_all_string(clanName, safeClanName, sizeof(safeClanName));

	format(queryData, sizeof(queryData), "UPDATE `clans` SET `name` = ^"%s^" WHERE `name` = ^"%s^"", safeClanName, safeOldClanName);

	SQL_ThreadQuery(sql, "saveStatsHandlerSql", queryData);
}
stock check_clan_name(const clanName[]){
	new queryData[128], safeClanName[64], error[128], errorNum, bool:foundClan;

	replace_all_string(clanName, safeClanName, sizeof(safeClanName));
	
	format(queryData, sizeof(queryData), "SELECT * FROM `clans` WHERE `name` = '%s'", safeClanName);
	
	new Handle:connectHandle = SQL_Connect(sql, errorNum, error, sizeof(error));
	
	if (errorNum) {
		log_amx("[SQL-LOG] Blad: %s (%d)", error, errorNum);
		return false;
	}
	
	new Handle:query = SQL_PrepareQuery(connectHandle, queryData);
	
	SQL_Execute(query);
	
	if (SQL_NumResults(query)) foundClan = true;

	SQL_FreeHandle(query);
	SQL_FreeHandle(connectHandle);
	
	return foundClan;
}
stock check_user_clan(const userName[]){
	new queryData[128], safeUserName[64], error[128], errorNum, bool:foundClan;

	replace_all_string(userName, safeUserName, sizeof(safeUserName));
	
	format(queryData, sizeof(queryData), "SELECT * FROM `players` WHERE `name` = '%s' AND clan > 0", userName);
	
	new Handle:connectHandle = SQL_Connect(sql, errorNum, error, sizeof(error));
	
	if (errorNum) {
		log_amx("[SQL-LOG] Blad: %s (%d)", error);
		return false;
	}
	
	new Handle:query = SQL_PrepareQuery(connectHandle, queryData);
	
	SQL_Execute(query);
	
	if (SQL_NumResults(query)) foundClan = true;

	SQL_FreeHandle(query);
	SQL_FreeHandle(connectHandle);
	
	return foundClan;
}
stock createClan(id, const clanName[]){
	new bbClan[clanInfo], queryData[128], safeClanName[64], error[128], errorNum;

	replace_all_string(clanName, safeClanName, sizeof(safeClanName));
	
	format(queryData, sizeof(queryData), "INSERT INTO `clans` (`name`, `createDate`, `info`, `upgradetime`) VALUES ('%s', NOW(), 'Brak Ogloszenia', '%d');", safeClanName, get_systime());
	
	new Handle:connectHandle = SQL_Connect(sql, errorNum, error, sizeof(error));
	
	if (errorNum) {
		log_amx("[SQL-LOG] Blad: %s (%d)", error, errorNum);
		return;
	}

	new Handle:query = SQL_PrepareQuery(connectHandle, queryData);
	
	SQL_Execute(query);

	format(queryData, sizeof(queryData), "SELECT idclans FROM `clans` WHERE name = '%s';", safeClanName);
	
	connectHandle = SQL_Connect(sql, errorNum, error, sizeof(error));
	
	if (errorNum) {
		log_amx("[SQL-LOG] Blad: %s (%d)", error, errorNum);
		return;
	}

	query = SQL_PrepareQuery(connectHandle, queryData);
	
	SQL_Execute(query);
	
	if (SQL_NumResults(query)) clan[id] = SQL_ReadResult(query, 0);

	copy(bbClan[CLAN_NAME], sizeof(bbClan[CLAN_NAME]), clanName);
	bbClan[CLAN_STATUS] = _:TrieCreate();
	bbClan[CLAN_ID] = clan[id];
	
	ArrayPushArray(bbClans, bbClan);

	set_user_clan(id, clan[id], 1);
	set_user_status(id, STATUS_LEADER);

	SQL_FreeHandle(query);
	SQL_FreeHandle(connectHandle);
}
stock removeClan(id){
	for (new i = 1; i < maxPlayers; i++) {
		if (!is_user_connected(i) || is_user_hltv(i) || is_user_bot(i) || i == id) continue;
		if (clan[i] == clan[id]) {
			clan[i] = 0;
			chatPrint(i, PREFIX_NORMAL, "Twoj klan zostal rozwiazany.");
		}
	}

	ArrayDeleteItem(bbClans, get_clan_id(clan[id]));
	new queryData[192], tempId[1];
	
	tempId[0] = clan[id];
	
	format(queryData, sizeof(queryData), "DELETE FROM `clans` WHERE idclans = '%d'", clan[id]);
	SQL_ThreadQuery(sql, "saveStatsHandlerSql", queryData, tempId, sizeof(tempId));
	
	format(queryData, sizeof(queryData), "UPDATE `players` SET flag = '%d', clan = '0', clannugget = '0' WHERE clan = '%d'", STATUS_NONE, clan[id]);
	SQL_ThreadQuery(sql, "saveStatsHandlerSql", queryData, tempId, sizeof(tempId));
	
	clan[id] = 0;
	
}
stock check_clan_loaded(clan){
	static bbClan[clanInfo];
	for (new i = 1; i < ArraySize(bbClans); i++) {
		ArrayGetArray(bbClans, i, bbClan);
		
		if (clan == bbClan[CLAN_ID]) return true;
	}
	return false;
}
stock get_clan_id(clan){
	static bbClan[clanInfo];
	for (new i = 1; i < ArraySize(bbClans); i++) {
		ArrayGetArray(bbClans, i, bbClan);
		
		if (clan == bbClan[CLAN_ID]) return i;
	}
	return 0;
}

stock get_clan_info(clan, info, dataReturn[] = "", dataLength = 0){
	static bbClan[clanInfo];

	for (new i = 0; i < ArraySize(bbClans); i++) {
		ArrayGetArray(bbClans, i, bbClan);
		
		if (bbClan[CLAN_ID] != clan) continue;
		if (info == CLAN_NAME) {
			copy(dataReturn, dataLength, bbClan[info]);
			return 0;
		}
		return bbClan[info];
	}
	return 0;
}
stock set_clan_info(clan, info, value = 0, dataSet[] = "", dataLength = 0){
	static bbClan[clanInfo];

	for (new i = 1; i < ArraySize(bbClans); i++) {
		ArrayGetArray(bbClans, i, bbClan);

		if (bbClan[CLAN_ID] != clan) continue;

		if (info == CLAN_NAME) format(bbClan[info], dataLength, dataSet);
		else bbClan[info] = value;

		ArraySetArray(bbClans, i, bbClan);

		save_clan(i);

		break;
	}
}
public replace_all_string(const source[], dest[], length) sql_string(source, dest, length);
stock sql_string(const source[], dest[], length){
	new const invalidChars[][] ={ "/", "\", "", "*", ":", "?", "^"", "<", ">", "'"};
	
	if(!(equal(source, "_")))	
		copy(dest, length, source);
	
	for(new i = 0; i < sizeof(invalidChars); i++)
		replace_all(dest, length, invalidChars[i], "");
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
