#include <amxmodx>
#include <sqlx>
#include <colorchat>
#include <regex>


new gClanNugget		=	100000;
new gClanLevel		=	10;	
new gClanLuzaczki	=	15;

new gClanStartMenber	=	5;

new gClanMaxLevel	=	30;

new gClanLevelCost	=	2925;
new gClanLevelNextCost	=	1550;

new gClanLevelKill	=	100;
new gClanLevelNextKill	=	200;


 new gClanLevelTime	=	1800;		// SEC    30MIN

new gClanUpExp 		= 	12
new gClanUpNugget 	= 	12
new gClanUpHealth	=	12
new gClanUpCrit 		= 	4
new gClanUpDamageP 	= 	5
new gClanUpDamageC 	= 	5
new gClanUpCoolDown 	= 	10//5
//new gClanUpGoblin 	= 	0//5

new gClanRestoreUp	=	20000;
	
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
		ColorChat(id, GREEN, "%s Zaloguj sie aby kozystac z klanow!", PREFIXSAY);
		return;
	}
	if (clan[id]){
		pageClan[id] = false;
		showClanMenu(id, false)
	} else createClanMenu(id)		
}
public createClanMenu(id){
	
	new gText[512], iLen = 0;	
	new availableNugget[256], availableLevel[256], availableLuzaczki[256]; 
	
	format(availableNugget, 		sizeof(availableNugget),  	"\w|\d Potrzebujesz jeszcze:\r %d Brylek!", 	gClanNugget-userNugget[id]);
	format(availableLevel,		sizeof(availableLevel),  	"\w|\d Potrzebujesz jeszcze:\r %d Level!", 	gClanLevel-userLevel[id]);
	format(availableLuzaczki, 	sizeof(availableLuzaczki),  	"\w|\d Potrzebujesz jeszcze:\r %d Luzaczkow!", 	gClanLuzaczki-userLuzCoin[id]);
	
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "\r[BaseBuilder]\y Stworz klan:^n");
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y-^t^t\dNazwa klanu musi miec co najmniej\r %d\d znaki", minSymbolClan)
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y-^t^t\dNazwa nie moze miec wiecej niz\r %d\d znakow", maxSymbolClan)
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y-^t^t\dZakaz\r obrazliwych\d nazw klanow")
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y-^t^t\dDozwolone znaki\r [A-Z0-9_]^n")
	
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t^t\dBrylki:\r %d %s",symbolsCustom[SYMBOL_DR_ARROW], gClanNugget, (gClanNugget > userNugget[id]) ? availableNugget : "\w| \yWymaganie spelnione!");
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t^t\dPoziom:\r %d %s", symbolsCustom[SYMBOL_DR_ARROW], gClanLevel, (gClanLevel > userLevel[id]) ? availableLevel : "\w| \yWymaganie spelnione!");
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t^t\dLuzaczki:\r %d %s", symbolsCustom[SYMBOL_DR_ARROW], gClanLuzaczki, (gClanLuzaczki > userLuzCoin[id]) ? availableLuzaczki : "\w| \yWymaganie spelnione!");

	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\r1.\w Stworz Klan");
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\r2.\w Top 10 Klanow");
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\r3.\w Lista Klanow");
	
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\r0.\y Wyjdz");
	
	show_menu(id, B1 | B2 | B3 | B0  , gText, -1, "createClanMenu"  )
}
public createClanMenu_2(id, item){
	if (!is_user_connected(id)) return;
	
	switch(item){
		case 0: {
			if (userNugget[id] < gClanNugget) {
				ColorChat(id, GREEN, "%s Brakuje Ci Brylek!", PREFIXSAY);
				return;
			}
			if(userLevel[id] < gClanLevel){
				ColorChat(id, GREEN, "%s Brakuje Ci Poziomu!", PREFIXSAY);
				return;
			}
			if(userLuzCoin[id] < gClanLuzaczki){
				ColorChat(id, GREEN, "%s Brakuje Ci Luzaczkow!", PREFIXSAY);
				return;
			}
			
			client_cmd(id, "messagemode PodajNazweKlanu");
		}
		case 1: loadStatsSql(id,5 );
		case 2: loadStatsSql(id,7 )
		
		default:{}
	}
}


public listClanMenu_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return
	}
	
	new target = userViewClan[id][item];
	userViewClanInfo[id] = target;
	
	loadDescClan(id)
}

public loadDescClan(id){
	
	if(is_user_hltv(id)) return PLUGIN_CONTINUE;
		
	if (!sqlConnected) {
		log_amx("[SQL-LOG] Brak polaczenia");
		return PLUGIN_CONTINUE
	}

	
	new queryData[512]
	new tempId[3];
	
	tempId[0] = id;
	tempId[1] = userViewClanInfo[id];

	format(queryData, sizeof(queryData), "\
		SELECT `a`.`idmotd`, `a`.`idclans` AS 'idclan', `a`.`name` AS `clan`, `a`.`kills`,`a`.`members`,`a`.`level`, `a`.`createDate`, `b`.`name` AS 'LIDER'\
		FROM `clans` `a`\
		JOIN `players` `b` ON `a`.`idclans` = `b`.`clan`\
		WHERE `a`.`idclans`='%d' AND `flag`='%d'", tempId[1], STATUS_LEADER)
	
	SQL_ThreadQuery(sql, "loadClanInfoDesc_2", queryData, tempId, sizeof(tempId));

	return PLUGIN_CONTINUE;
}
public loadClanInfoDesc_2(failState, Handle:query, error[], errorNum, tempId[], dataSize){
	
	new id 		= 	tempId[0]
	
	if (failState){
		if (failState == TQUERY_CONNECT_FAILED) log_amx("[SQL-LOG] Nie mozna polaczyc siê z baz¹ danych SQL. Blad: %s (%d)", error, errorNum)
		else if (failState == TQUERY_QUERY_FAILED) log_amx("[SQL-LOG] Zapytanie watkowe nie powiodlo sie. Blad: %s (%d)", error, errorNum)
		return PLUGIN_HANDLED;
	}	
	
	new gText[1756], iLen = 0
	new szName[33], szDate[33], szLider[33], motd[21];

	SQL_ReadResult(query, SQL_FieldNameToNum(query, "idmotd"), motd, sizeof(motd)-1);
	SQL_ReadResult(query, SQL_FieldNameToNum(query, "clan"), szName, sizeof(szName)-1);
	SQL_ReadResult(query, SQL_FieldNameToNum(query, "createDate"), szDate, sizeof(szDate)-1);
	SQL_ReadResult(query, SQL_FieldNameToNum(query, "LIDER"), szLider, sizeof(szLider)-1);
	new kills = SQL_ReadResult(query, SQL_FieldNameToNum(query, "kills"));
	new level = SQL_ReadResult(query, SQL_FieldNameToNum(query, "level"));
	new members = SQL_ReadResult(query, SQL_FieldNameToNum(query, "members"));

	new skillbar[256];
	
	skillBarMenu(skillbar, sizeof(skillbar), level, gClanMaxLevel, symbolsCustom[SYMBOL_SMALL_DOT], symbolsCustom[SYMBOL_SMALL_DOT])	
	
	iLen = format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Informacje o Klanie!^n")
	
	iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s\w %s\d %s\w %s\y %s^n", symbolsCustom[SYMBOL_R_ARROW], symbolsCustom[SYMBOL_VERTICAL_LINE], skillbar, symbolsCustom[SYMBOL_VERTICAL_LINE], symbolsCustom[SYMBOL_L_ARROW]);
	
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\y%s^t^t\dNazwa Klanu:\r %s", symbolsCustom[SYMBOL_DR_ARROW], szName)
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\y%s^t^t\dPrzywodca Klanu:\r %s", symbolsCustom[SYMBOL_DR_ARROW], szLider)
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\y%s^t^t\dCzlonkowie:\r %d/%d", symbolsCustom[SYMBOL_DR_ARROW], members,(  level / 3 ) + gClanStartMenber);
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\y%s^t^t\dZabojstwa:\r %d", symbolsCustom[SYMBOL_DR_ARROW], kills)
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\y%s^t^t\dPoziom:\r %d^n", symbolsCustom[SYMBOL_DR_ARROW], level)
	
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\y%s^t^t\dData Stworzenia:\r %s", symbolsCustom[SYMBOL_DR_ARROW], szDate)

	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n^n\r7.\r Zdjecie Informacyjne");
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\r8.\w Czlonkowie^n");
	
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\r9.\y Wroc");
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\r0.\y Wyjdz");
	
	show_menu(id, B7 | B8 | B9| B0, gText, -1, "clanInfoDesc")	
	return PLUGIN_HANDLED;
}
public clanInfoDesc_2(id, item){
	
	switch(item){
		case 6:{
			loadMotdClan(id)
		}
		case 7:{
			loadMembersClan(id)
		}
		case 8:{
			loadStatsSql(id,7 )
		}
		default:{ return; }
	}
}
public loadMotdClan(id){
	
	if(is_user_hltv(id)) return PLUGIN_CONTINUE;
		
	if (!sqlConnected) {
		log_amx("[SQL-LOG] Brak polaczenia");
		return PLUGIN_CONTINUE
	}

	
	new queryData[512]
	new tempId[3];
	
	tempId[0] = id;
	tempId[1] = userViewClanInfo[id];

	format(queryData, sizeof(queryData), "\
		SELECT `idmotd`, `name` FROM `clans`\
		WHERE `idclans`='%d'", tempId[1])
	
	SQL_ThreadQuery(sql, "loadMotdClan_2", queryData, tempId, sizeof(tempId));

	return PLUGIN_CONTINUE;
}
public loadMotdClan_2(failState, Handle:query, error[], errorNum, tempId[], dataSize){
	
	new id = tempId[0];
	
	new gMotd[21], clanName[12];
	SQL_ReadResult(query, SQL_FieldNameToNum(query, "idmotd"), gMotd, sizeof(gMotd)-1);
	SQL_ReadResult(query, SQL_FieldNameToNum(query, "name"), clanName, sizeof(clanName)-1);
	
	
	new gText[2048], iLen=0
	new sizeText = sizeof(gText)-iLen-1;
				
	iLen += format(gText[iLen], sizeText, "<html>")
	iLen += format(gText[iLen], sizeText, "<body style=^"background: url('https://i.imgur.com/%s.png') top center #000000;margin:0px;padding:0px;background-size:100%% 100%%;background-repeat:no-repeat;^"></body>", strlen(gMotd) > 1 ? formatm("%s", gMotd) : formatm("%s", MOTDNONE));				
	iLen += format(gText[iLen], sizeText, "</html>")
	show_motd(id, gText, formatm("Klan: %s", clanName))
	
}


public loadMembersClan(id){
	
	if(is_user_hltv(id)) return PLUGIN_CONTINUE;
		
	if (!sqlConnected) {
		log_amx("[SQL-LOG] Brak polaczenia");
		return PLUGIN_CONTINUE
	}

	new queryData[512]
	new tempId[3];
	
	tempId[0] = id;
	tempId[1] = userViewClanInfo[id];

	
	format(queryData, sizeof(queryData), "\
		SELECT * FROM `players`\
		WHERE clan = '%d'", tempId[1])
	
	SQL_ThreadQuery(sql, "loadMembersClan_2", queryData, tempId, sizeof(tempId));

	return PLUGIN_CONTINUE;
}

public loadMembersClan_2(failState, Handle:query, error[], errorNum, tempId[], dataSize){
	
	new id 		= 	tempId[0]
	
	if (failState){
		if (failState == TQUERY_CONNECT_FAILED){
			log_amx("[SQL-LOG] Nie mozna polaczyc siê z baz¹ danych SQL. Blad: %s (%d)", error, errorNum)
		}
		else if (failState == TQUERY_QUERY_FAILED){
			log_amx("[SQL-LOG] Zapytanie watkowe nie powiodlo sie. Blad: %s (%d)", error, errorNum)
		}
		return PLUGIN_HANDLED;
	}	
	
	new gText[1756], iLen = 0
	new szName[33]
	new copyName[11];
	new status;
	
	iLen = format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Lista Czlonkow!^n")
	
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
	
	show_menu(id, B9| B0, gText, -1, "clanInfoMember")
	return PLUGIN_HANDLED;
}

public clanInfoMember_2(id, item){
	switch(item){
		case 8:{
			loadDescClan(id)
		}
		default:{ return; }
	}
}

public showClanMenu(id, next){	
	if (!is_user_connected(id)) 
		return;
	
	if( !playerLogged(id)){
		ColorChat(id, GREEN, "%s Zaloguj sie aby kozystac z klanow!", PREFIXSAY);
		return;
	}
	
	pageClan[id] = false;
	
	new bbClan[clanInfo], gText[1536], iLen;
	new skillbar[256];
	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
		
	skillBarMenu(skillbar, sizeof(skillbar), bbClan[CLAN_LEVEL], gClanMaxLevel, symbolsCustom[SYMBOL_SMALL_DOT], symbolsCustom[SYMBOL_SMALL_DOT])	
	
	iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "\r[BaseBuilder]\y Menu Klanu\d [\w %s/2\d ]^n", !next ? "1" : "2");
	
	if(!next){
		
		iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dOgloszenie:\r %s",symbolsCustom[SYMBOL_DR_ARROW],bbClan[CLAN_INFO]);
	
		iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dNazwa:\r %s", symbolsCustom[SYMBOL_DR_ARROW], bbClan[CLAN_NAME]);
		iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dCzlonkowie:\r %d/%d",symbolsCustom[SYMBOL_DR_ARROW],  bbClan[CLAN_MEMBERS],(  bbClan[CLAN_LEVEL] / 3 ) + gClanStartMenber);
		iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dPoziom:\r %d",symbolsCustom[SYMBOL_DR_ARROW], bbClan[CLAN_LEVEL]);
		iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dBrylki:\r %s",symbolsCustom[SYMBOL_DR_ARROW],formatNumber(bbClan[CLAN_NUGGET]));
		iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dZabojstwa:\r %s",symbolsCustom[SYMBOL_DR_ARROW],formatNumber(bbClan[CLAN_KILLS]));
	
		
		if (get_user_status(id) > STATUS_MEMBER){
			if(bbClan[CLAN_LEVEL] >= gClanMaxLevel ) iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\d1. Awansuj: Maksymalny Poziom");
			else iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\r1.\w Awansuj");
		} else {
			if(bbClan[CLAN_LEVEL] >= gClanMaxLevel ) iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\d[ Awans Klanu: Maksymalny Poziom ]");
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
	
	
	show_menu(id, B1 | B2 | B3 | B4 | B5 | B6 | B7 | B8 | B9 | B0, gText, -1, "showClanMenu"  )
	
}
public showClanMenu_2(id, item){
	if (!is_user_connected(id)) return;
	
	

	
	switch (item) {
		
		case 0: {
			if(pageClan[id]){
				if (get_user_status(id) > STATUS_MEMBER) avansClanMenu(id);
				else globalClanMenu(id)
				
			} 
			else membersOnlineMenu(id);
		}
		case 1: {
			if(pageClan[id]){
				if (get_user_status(id) > STATUS_MEMBER) {
					userAcceptRestore[id] = false;
					leaderMenu(id);
				} else {
					globalClanMenu(id)
					ColorChat(id, GREEN, "%s Nie posiadasz dostepu!", PREFIXSAY);	
				}
			}else {
				loadStatsSql(id,6 );	
			}
		}
		case 2: {
			if(pageClan[id]){
				
				if (get_user_status(id) == STATUS_LEADER) {
					ColorChat(id, GREEN, "%s Oddaj przywodctwo klanu jednemu z czlonkow zanim go upuscisz", PREFIXSAY);	
					globalClanMenu(id)
				} else {
					leaveConfimMenu(id);	
				}
			} else {
				loadStatsSql(id,5 );
			}
		}
		case 3: {
			if(pageClan[id]){
				client_cmd(id, "messagemode IloscBrylekKlan");
				client_print(id, print_center, "Wpisz ilosc Brylek, ktora chcesz wplacic");
				ColorChat(id, GREEN, "%s Wpisz ilosc Brylek, ktora chcesz wplacic.", PREFIXSAY);
			} else {
				loadStatsSql(id,7 );
			}
		}
		case 4:{
			if(!pageClan[id]){
			
				new bbClan[clanInfo];
				ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
					
				new gText[2048], iLen=0
				new sizeText = sizeof(gText)-iLen-1;
				
				iLen += format(gText[iLen], sizeText, "<html>")
				iLen += format(gText[iLen], sizeText, "<body style=^"background: url('https://i.imgur.com/%s.png') top center #000000;margin:0px;padding:0px;background-size:100%% 100%%;background-repeat:no-repeat;^"></body>", strlen(bbClan[CLAN_MOTD]) > 1 ? formatm("%s", bbClan[CLAN_MOTD]) : formatm("%s", MOTDNONE));				
				iLen += format(gText[iLen], sizeText, "</html>")
				show_motd(id, gText, formatm("Klan: %s", bbClan[CLAN_NAME]))
			} else {
				showClanMenu(id, true)	
			}
		}
		case 5..7:pageClan[id] ? showClanMenu(id, false) : showClanMenu(id, true);
		case 8: pageClan[id] ? showClanMenu(id, true) : showClanMenu(id, false);
	}
}
public avansClanMenu(id){
	
	new bbClan[clanInfo], gText[1536], iLen;
	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
	new costNugget = (gClanLevelCost + gClanLevelNextCost * bbClan[CLAN_LEVEL]);
	new killsClan =  (gClanLevelKill + gClanLevelNextKill * bbClan[CLAN_LEVEL]);
	new timeUpgrade =  calcAvansClan(id)

	new availableNugget[256], availableKill[256]; 
	
	format(availableNugget, 		sizeof(availableNugget),"\w|\d Potrzebujesz:\r %d Brylek!", 	costNugget-bbClan[CLAN_NUGGET]);
	format(availableKill,		sizeof(availableKill),  	"\w|\d Potrzebujesz:\r %d Zabojstw!", 	killsClan-bbClan[CLAN_KILLS]);
	
	iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r[BaseBuilder]\y Awansowanie Klanu^n");
	
	iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dPoziom Klanu:\r %d",symbolsCustom[SYMBOL_DR_ARROW],bbClan[CLAN_LEVEL]);
	iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dBrylki Klanowe:\r %s",symbolsCustom[SYMBOL_DR_ARROW],formatNumber(bbClan[CLAN_NUGGET]));
	iLen 	+= 	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t\dZabojstwa Klanowe:\r %s",symbolsCustom[SYMBOL_DR_ARROW],formatNumber(bbClan[CLAN_KILLS]));
	
	
	new upgradeLeft = (bbClan[CLAN_UPGRADETIME] - get_systime())
			
	if(bbClan[CLAN_LEVEL] < gClanMaxLevel){
		
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
	
	if(upgradeLeft > 0 || bbClan[CLAN_LEVEL] >= gClanMaxLevel)
		iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\d1. Zacznij Ulepszac %s", upgradeLeft > 0 ? "[ ODCZEKAJ ]" : bbClan[CLAN_LEVEL] >= gClanMaxLevel ? "[ MAX ]" : "");
	else iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\r1.\w Zacznij Ulepszac");
	
	
	iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\d2. Reset czasu");
	
	
	iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n^n\r0.\y Wyjdz");
	
	show_menu(id, B1 | B2 | B3 | B0  , gText, -1, "avansClanMenu"  )
}
public avansClanMenu_2(id, item){
	switch(item){
		case 0:{
			new bbClan[clanInfo];
			ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
			new upgradeLeft = 0;
			
			if (bbClan[CLAN_LEVEL] >= gClanMaxLevel) {
				ColorChat(id, GREEN, "%s Twoj klan ma juz maksymalny Poziom.", PREFIXSAY);
				avansClanMenu(id)
				return;
			}
			upgradeLeft = (bbClan[CLAN_UPGRADETIME] - get_systime())
					
			if(upgradeLeft > 0){
				new gText[128], iLen = 0;
				
				iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "---^x01 Nie mozesz ulepszyc!^x04 |^x01 Musisz poczekac^x03 ");
				
				if((upgradeLeft) / DAY > 0 ) iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "%d dzien", (upgradeLeft / HOUR) % HOUR);
				else if((upgradeLeft) / HOUR > 0 ) iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "%d godz %s%d min", (upgradeLeft) / HOUR, ( upgradeLeft / MINUTE % MINUTE )<10?"0":"", ((upgradeLeft) / MINUTE) %MINUTE);
				else if( upgradeLeft / MINUTE > 0 ) iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "%d min", (upgradeLeft / MINUTE) %MINUTE);
				else  iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "%d sek", upgradeLeft);
				
				iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "^x04 ---");
				
				ColorChat(id, GREEN, gText)
				avansClanMenu(id)
				return
			}	
			new costNugget = bbClan[CLAN_NUGGET] - (gClanLevelCost + gClanLevelNextCost * bbClan[CLAN_LEVEL]);
			new killsClan = bbClan[CLAN_KILLS] - (gClanLevelKill + gClanLevelNextKill * bbClan[CLAN_LEVEL]);
					
			if (costNugget < 0) {		
				ColorChat(id, GREEN, "%s Twoj klan nie ma wystarczajacej ilosci Brylek." ,PREFIXSAY );
				avansClanMenu(id)
				return;
			}		
			if (killsClan < 0) {		
				ColorChat(id, GREEN, "%s Twoj klan nie ma wystarczajacej ilosci Zabojstw." ,PREFIXSAY );
				avansClanMenu(id)
				return;
			}
			new upgradeTime = calcAvansClan(id)
			
			bbClan[CLAN_UPGRADETIME]	= 	max( bbClan[CLAN_UPGRADETIME] + (upgradeTime), get_systime() + (upgradeTime) )
					
			bbClan[CLAN_LEVEL] ++;
			bbClan[CLAN_NUGGET] = costNugget;
			bbClan[CLAN_POINTS] += 2;
					
			ArraySetArray(bbClans, get_clan_id(clan[id]), bbClan);
			save_clan(get_clan_id(clan[id]));
					
			new logText[128];
			logType[id] = LOG_CLAN_PROMOTION;
			if(logType[id] == LOG_CLAN_PROMOTION){
				new clanName[64];
						
				get_clan_info(clan[id], CLAN_NAME, clanName, sizeof(clanName));	
				format(logText, sizeof(logText), "awansowal na poziom: %d | klan: %s", bbClan[CLAN_LEVEL], clanName);
									
				logBB(id, logText)
			}		
					
			ColorChat(0, GREEN, "^x04---^x03 %s^x01 ulepszyl klan^x04 %s^x01 na poziom^x03 %d^x04 ---", userName[id],bbClan[CLAN_NAME], bbClan[CLAN_LEVEL]);
			avansClanMenu(id)
			return;
			
		}
		case 1:{
			new bbClan[clanInfo];
			ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
			bbClan[CLAN_UPGRADETIME]	= 	 get_systime()
			ArraySetArray(bbClans, get_clan_id(clan[id]), bbClan);
			save_clan(get_clan_id(clan[id]));
			
			avansClanMenu(id)
			return;
		}
	}
}
public calcAvansClan(id){
	new bbClan[clanInfo];
	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
	return (gClanLevelTime * (bbClan[CLAN_LEVEL]+1))
}
public createClanHandle(id){
	if (!is_user_connected(id)  ||  !playerLogged(id) || clan[id]) return;
		
	
	if (userNugget[id] < gClanNugget) {
		ColorChat(id, GREEN, "%s Brakuje Ci Brylek!", PREFIXSAY);
		return;
	}
	if(userLevel[id] < gClanLevel){
		ColorChat(id, GREEN, "%s Brakuje Ci Poziomu!", PREFIXSAY);
		return;
	}
	if(userLuzCoin[id] < gClanLuzaczki){
		ColorChat(id, GREEN, "%s Brakuje Ci Luzaczkow!", PREFIXSAY);
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
			ColorChat(id, GREEN, "%s Dozwolone znaki to^x03 [A-Z0-9_]", PREFIXSAY);
			globalClanMenu(id);
			return;
		}
		case REGEX_PATTERN_FAIL: {
			ColorChat(id, GREEN, "%s Dozwolone znaki to^x03 [A-Z0-9_]", PREFIXSAY);
			globalClanMenu(id);
			return;
		}
		case REGEX_NO_MATCH: {
			ColorChat(id, GREEN, "%s Dozwolone znaki to^x03 [A-Z0-9_]", PREFIXSAY);
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
			
		ColorChat(id, GREEN, "%s Dozwolone znaki to^x03 [A-Z0-9_]", PREFIXSAY);
		globalClanMenu(id);
		return ;
	}	
	if (equal(clanName, "")) {
		ColorChat(id, GREEN, "%s Nie wpisales nazwy klanu", PREFIXSAY);
		globalClanMenu(id);
		return ;
	}
	if (strlen(clanName) >  maxSymbolClan) {
		ColorChat(id, GREEN, "%s Nazwa klanu nie moze miec wiecej niz %d znakow", PREFIXSAY, maxSymbolClan);
		globalClanMenu(id);
		return ;
	}
	if (strlen(clanName) < minSymbolClan) {
		ColorChat(id, GREEN, "%s Nazwa klanu musi miec co najmniej %d znaki", PREFIXSAY, minSymbolClan);
		globalClanMenu(id);
		return ;
	}
	if(equali(clanName, "admin", 3) || equali(clanName, "opiekun serwera", 7) || equali(clanName, "Wlasciciel", 10)){
		ColorChat(id, GREEN, "%s Zakaz nazwy kalnu: Admin", PREFIXSAY);
		globalClanMenu(id);
		return;
	}
	if (check_clan_name(clanName)) {
		ColorChat(id, GREEN, "%s Klan z taka nazwa juz istnieje", PREFIXSAY);
		globalClanMenu(id);
		return ;
	}

	createClan(id, clanName)
	new logText[128];	
	logType[id] = LOG_CLAN_CREATE;
	if(logType[id] == LOG_CLAN_CREATE){
		
		format(logText, sizeof(logText), "stworzyl klan %s", clanName);
		logBB(id, logText)
	}

	ColorChat(0, GREEN, "---^x01 Gracz^x03 %s^x01 zalozyl klan:^x04 [^x03 %s^x04 ] ---", userName[id], clanName);
	
	
	userNugget[id] -= gClanNugget
	userLuzCoin[id] -= gClanLuzaczki
	
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
				ColorChat(id, GREEN, "%s Oddaj przywodctwo klanu jednemu z czlonkow zanim go upuscisz", PREFIXSAY);
				globalClanMenu(id);
				return;
			}
			ColorChat(id, GREEN, "%s Opusciles swoj klan", PREFIXSAY);
			
			new logText[128];
			logType[id] = LOG_CLAN_LEAVE;
			if(logType[id] == LOG_CLAN_LEAVE){
				new clanName[64];
						
				get_clan_info(clan[id], CLAN_NAME, clanName, sizeof(clanName));	
				format(logText, sizeof(logText), "opuscil klan %s",  clanName);
									
				
			}
			logBB(id, logText)
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
		if (!is_user_connected(id) || clan[id] != clan[i]) 
			continue;
			
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
	
	if (!online) ColorChat(id, GREEN, "%s Na serwerze nie ma zadnego czlonka twojego klanu!", PREFIXSAY)
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
	
	format(gText, sizeof(gText), "\wZresetuj Umiejetnosci:\d [\r %d Brylek\d ] %s", gClanRestoreUp, userAcceptRestore[id] ? "\r[POTWIERDZ]" : "");
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
			else ColorChat(id, GREEN, "%s Tylko wlasciciel klanu moze to zrobic!", PREFIXSAY);
		}
		case 1: {
			if(get_user_status(id) > STATUS_MEMBER) upgradeClanMenu(id);
			else ColorChat(id, GREEN, "%s Tylko wlasciciel oraz zastepca klanu moze ulepszac!", PREFIXSAY);
		}
		case 2: inviteMenu(id);
		case 3: loadStatsSql(id,8)
		case 4:{
			if(get_user_status(id) == STATUS_LEADER){
				new bbClan[clanInfo];
				ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
				
				if(userAcceptRestore[id]){
				
					if(bbClan[CLAN_NUGGET] < gClanRestoreUp){
						ColorChat(id, GREEN, "%s Brakuje Brylek", PREFIXSAY);
						userAcceptRestore[id] = false;
						return;
					}
					
					bbClan[CLAN_NUGGET] -= gClanRestoreUp
					bbClan[CLAN_POINTS] = bbClan[CLAN_LEVEL] * 2
					
					bbClan[CLAN_HEALTH] = 0;
					bbClan[CLAN_DAMAGECLASS] = 0;
					bbClan[CLAN_DAMAGEPLAYER] = 0;
					bbClan[CLAN_CRITIC] = 0;
					bbClan[CLAN_COOLDOWN] = 0;
					bbClan[CLAN_EXPDROP] = 0;
					bbClan[CLAN_NUGGETDROP] = 0;
					
					ColorChat(id, GREEN, "%s Zresetowales Umiejetnosci!", PREFIXSAY);
				
					ArraySetArray(bbClans, get_clan_id(clan[id]), bbClan);
					save_clan(get_clan_id(clan[id]));
					userAcceptRestore[id] = false;
					
					new logText[128];
					new clanName[64];
					logType[id] = LOG_CLAN_RESET;
					if(logType[id] == LOG_CLAN_RESET){
						get_clan_info(clan[id], CLAN_NAME, clanName, sizeof(clanName));	
						format(logText, sizeof(logText), "zresetowal statystyki | klan: %s",  clanName);
						logBB(id, logText)
					}
					return;
				}
				userAcceptRestore[id] = true;
				leaderMenu(id)
				return;
	
			} else ColorChat(id, GREEN, "%s Tylko wlasciciel klanu moze to zrobic!", PREFIXSAY);
		}
		case 5:{
			if(get_user_status(id) == STATUS_LEADER){
				client_print(id, print_center, "Wpisz teraz nowe ogloszenie dla Klanowiczow!");
				cmd_execute(id, "messagemode Ogloszenie_Klanu")
				leaderMenu(id)
			} else ColorChat(id, GREEN, "%s Tylko wlasciciel klanu moze to zrobic!", PREFIXSAY);
			
		}
		case 6:{
			if(get_user_status(id) > STATUS_MEMBER){
				motdClanMenu(id)
			} else ColorChat(id, GREEN, "%s Tylko wlasciciel oraz zastepca klanu moze ulepszac!", PREFIXSAY);
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
			cmd_execute(id, "messagemode Kod_Motd_Klanu")
			motdClanMenu(id)
		}
		case 1:{
			new bbClan[clanInfo];
			ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
			
			bbClan[CLAN_MOTD] = "_";
			
			ColorChat(id, GREEN, "---^x01 Zresetowales^x03 MOTD^x01 klanu!^x04 ---");
			
			ArraySetArray(bbClans, get_clan_id(clan[id]), bbClan);
			
			save_clan(get_clan_id(clan[id]));
			
			motdClanMenu(id)
		}
	}
}
public checkCodeMotd(id){
	if(!(get_user_status(id) > STATUS_MEMBER || is_user_connected(id) || clan[id])) return PLUGIN_CONTINUE;
	
	new bbClan[clanInfo];
	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
	
	new codeMotd[20]
	read_args(codeMotd, sizeof(codeMotd));
	remove_quotes(codeMotd);
	trim(codeMotd);
	
	if (equal(codeMotd, "") || strlen(codeMotd) <  1 ) {
		ColorChat(id, GREEN, "---^x01 Nic nie wpisales!^x04 ---");
		return PLUGIN_HANDLED;
		
	}
	if (strlen(codeMotd) >= 20+1 ) {
		ColorChat(id, GREEN, "---^x01 Za dlugi kod^x04 ---");
		return PLUGIN_HANDLED;
	}
	
	bbClan[CLAN_MOTD] = codeMotd;
	
	ColorChat(id, GREEN, "---^x01 Ustawiles^x03 MOTD^x01 klanu!^x03 (^x04%s^x03)^x04 ---", codeMotd);
	
	ArraySetArray(bbClans, get_clan_id(clan[id]), bbClan);
	
	save_clan(get_clan_id(clan[id]));
	
	motdClanMenu(id)	
	
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
			ColorChat(id, GREEN, "%s Wpisz swoja nazwe klanu aby go usunac!", PREFIXSAY);
			client_print(id, print_center, "Wpisz swoja nazwe klanu aby go usunac!");
			client_cmd(id, "messagemode Potwierdz_Nazwe_Klanu");

		}
		case 1: globalClanMenu(id);
	}

}

public checkNameClan(id){
	if(!(get_user_status(id) == STATUS_LEADER || is_user_connected(id) || clan[id])) return;
	
	new nameClan[17]
	read_args(nameClan, sizeof(nameClan));
	remove_quotes(nameClan);
	trim(nameClan);
	
	new bbClan[clanInfo];
	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
				
	if(equal(nameClan, bbClan[CLAN_NAME])){
		ColorChat(id, GREEN, "%s Rozwiazales swoj klan!", PREFIXSAY)
		
		new logText[128];
		logType[id] = LOG_CLAN_DELETE;
		if(logType[id] == LOG_CLAN_DELETE){
			new clanName[64];
		
			get_clan_info(clan[id], CLAN_NAME, clanName, sizeof(clanName));	
			format(logText, sizeof(logText), "usunal klan: %s",  clanName);
			logBB(id, logText)			
		}
		
		removeClan(id);
					
		
	} else  ColorChat(id, GREEN, "%s Wpisz poprawna nazwe klanu!", PREFIXSAY)	
}

public upgradeClanMenu(id){
	if (!is_user_connected(id) || !clan[id]) return;
	
	new bbClan[clanInfo], gText[1536], iLen = 0;

	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);
	
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r[BaseBuilder]\y Ulepszanie^n^n");
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\y%s^t^t\dKoszt:\r 1 punkt^n", symbolsCustom[SYMBOL_DR_ARROW]);
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\y%s^t^t\dPunkty:\r %d\w^n^n", symbolsCustom[SYMBOL_DR_ARROW], bbClan[CLAN_POINTS]);


	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r1.\w Exp\r [\d%d/%d\r]^n", bbClan[CLAN_EXPDROP], gClanUpExp);
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r2.\w Brylki\r [\d%d/%d\r]^n", bbClan[CLAN_NUGGETDROP], gClanUpNugget);
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r3.\w HP\r [\d%d/%d\r]^n", bbClan[CLAN_HEALTH], gClanUpHealth);
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r4.\w Kryt\r [\d%d/%d\r]^n",  bbClan[CLAN_CRITIC], gClanUpCrit);
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r5.\w DMG\r [\d%d/%d\r]^n", bbClan[CLAN_DAMAGEPLAYER], gClanUpDamageP);
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r6.\w DMG Klasy\r [\d%d/%d\r]^n",  bbClan[CLAN_DAMAGECLASS], gClanUpDamageC);
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r7.\w CoolDown\r [\d%d/%d\r] ^n", bbClan[CLAN_COOLDOWN], gClanUpCoolDown);
	
	//iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "\r8.\w Dodatkowy Goblin\y |\d %d/%d^n", bbClan[CLAN_GOBLIN], gClanUpGoblin);
	
	iLen	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "^n\r0.\y Wyjscie");
	
	show_menu(id, B1 | B2 | B3 | B4 | B5 | B6 | B7 |/* B8 |*/ B9 | B0, gText, -1, "upgradeClanMenu"  )

}

public upgradeClanMenu_2(id, item){
	if (!is_user_connected(id) || !clan[id]) return;
	
	new bbClan[clanInfo], upgradedSkill, upgradedSkillName[33];	
	ArrayGetArray(bbClans, get_clan_id(clan[id]), bbClan);

	if ((item ==  0 || item ==  1 || item ==  2 || item ==  3 || item ==  4 || item ==  5 || item ==  6 || item ==  7 || item ==  8 ) && bbClan[CLAN_POINTS] < 1) {
		ColorChat(id, GREEN, "%s Twoj klan nie ma wystarczajacej ilosci Punktow", PREFIXSAY);
		upgradeClanMenu(id);
		return;
	}
	switch (item) {
		case 0: {
			upgradedSkill = CLAN_EXPDROP;
			if (bbClan[upgradedSkill] == gClanUpExp) {
				ColorChat(id, GREEN, "%s Twoj klan ma juz maksymalny poziom tej umiejetnosci", PREFIXSAY);
				upgradeClanMenu(id);
				return;
			}
			upgradedSkillName = "Dodatkowy EXP";
			bbClan[upgradedSkill]++;
		
			ColorChat(id, GREEN, "%s Ulepszyles umiejetnosc^x03 %s^x01 na^x03 %d^x01 poziom!", PREFIXSAY, upgradedSkillName, bbClan[upgradedSkill]);
		}
		case 1: {
			upgradedSkill = CLAN_NUGGETDROP;
			if (bbClan[upgradedSkill] == gClanUpNugget) {
				ColorChat(id, GREEN, "%s Twoj klan ma juz maksymalny poziom tej umiejetnosci", PREFIXSAY);
				upgradeClanMenu(id);
				return;
			}
			upgradedSkillName = "Dodatkowe Brylki";
			bbClan[upgradedSkill]++;
		
			ColorChat(id, GREEN, "%s Ulepszyles umiejetnosc^x03 %s^x01 na^x03 %d^x01 poziom!", PREFIXSAY, upgradedSkillName, bbClan[upgradedSkill]);
		}
		case 2: {
			upgradedSkill = CLAN_HEALTH;
			if (bbClan[upgradedSkill] == gClanUpHealth) {
				ColorChat(id, GREEN, "%s Twoj klan ma juz maksymalny poziom tej umiejetnosci", PREFIXSAY);
				upgradeClanMenu(id);
				return;
			}
			upgradedSkillName = "Dodatkowe Zycie";
			bbClan[upgradedSkill]++;
		
			ColorChat(id, GREEN, "%s Ulepszyles umiejetnosc^x03 %s^x01 na^x03 %d^x01 poziom!", PREFIXSAY, upgradedSkillName, bbClan[upgradedSkill]);
		}
		case 3: {
			upgradedSkill = CLAN_CRITIC;
			if (bbClan[upgradedSkill] == gClanUpCrit) {
				ColorChat(id, GREEN, "%s Twoj klan ma juz maksymalny poziom tej umiejetnosci", PREFIXSAY);
				upgradeClanMenu(id);
				return;
			}
			upgradedSkillName = "Krytyk";
			bbClan[upgradedSkill]++;
		
			ColorChat(id, GREEN, "%s Ulepszyles umiejetnosc^x03 %s^x01 na^x03 %d^x01 poziom!", PREFIXSAY, upgradedSkillName, bbClan[upgradedSkill]);
		}
		case 4: {
			upgradedSkill = CLAN_DAMAGEPLAYER;
			if (bbClan[upgradedSkill] == gClanUpDamageP) {
				ColorChat(id, GREEN, "%s Twoj klan ma juz maksymalny poziom tej umiejetnosci", PREFIXSAY);
				upgradeClanMenu(id);
				return;
			}
			upgradedSkillName = "Obrazenia Postaci";
			bbClan[upgradedSkill]++;
		
			ColorChat(id, GREEN, "%s Ulepszyles umiejetnosc^x03 %s^x01 na^x03 %d^x01 poziom!", PREFIXSAY, upgradedSkillName, bbClan[upgradedSkill]);
		}
		case 5: {
			upgradedSkill = CLAN_DAMAGECLASS;
			if (bbClan[upgradedSkill] == gClanUpDamageC) {
				ColorChat(id, GREEN, "%s Twoj klan ma juz maksymalny poziom tej umiejetnosci", PREFIXSAY);
				upgradeClanMenu(id);
				return;
			}
			upgradedSkillName = "Obrazenia Klasy";
			bbClan[upgradedSkill]++;
		
			ColorChat(id, GREEN, "%s Ulepszyles umiejetnosc^x03 %s^x01 na^x03 %d^x01 poziom!", PREFIXSAY, upgradedSkillName, bbClan[upgradedSkill]);
		}
		case 6: {
			upgradedSkill = CLAN_COOLDOWN;
			if (bbClan[upgradedSkill] == gClanUpCoolDown) {
				ColorChat(id, GREEN, "%s Twoj klan ma juz maksymalny poziom tej umiejetnosci", PREFIXSAY);
				upgradeClanMenu(id);
				return;
			}
			upgradedSkillName = "Czas odnowy";
			bbClan[upgradedSkill]++;

			ColorChat(id, GREEN, "%s Ulepszyles umiejetnosc^x03 %s^x01 na^x03 %d^x01 poziom!", PREFIXSAY, upgradedSkillName, bbClan[upgradedSkill]);
		}
		/*case 7: {
			upgradedSkill = CLAN_GOBLIN;
			if (bbClan[upgradedSkill] == gClanUpGoblin) {
				ColorChat(id, GREEN, "%s Twoj klan ma juz maksymalny poziom tej umiejetnosci", PREFIXSAY);
				upgradeClanMenu(id);
				return;
			}
			upgradedSkillName = "Dodatkowy Goblin";
			bbClan[upgradedSkill]++;

			ColorChat(id, GREEN, "%s Ulepszyles umiejetnosc^x03 %s^x01 na^x03 %d^x01 poziom!", PREFIXSAY, upgradedSkillName, bbClan[upgradedSkill]);
		}
		*/
		default:{
			return;
		}
	}
	
	bbClan[CLAN_POINTS]--;
	
	ArraySetArray(bbClans, get_clan_id(clan[id]), bbClan);

	save_clan(get_clan_id(clan[id]));
	
	
	
	for (new i = 1; i < maxPlayers; i++) {
		if (!is_user_connected(id) || i == id || clan[i] != clan[id]) 
			continue;
			
		ColorChat(i,GREEN, "%s^x03 %s^x01 ulepszyl %s na^x03 %d Poziom^x01!",PREFIXSAY, userName[id], upgradedSkillName, bbClan[upgradedSkill]);
	}
	
	new logText[128];
	logType[id] = LOG_CLAN_UPGRADE;
	if(logType[id] == LOG_CLAN_UPGRADE){
		new clanName[64];
		
		get_clan_info(clan[id], CLAN_NAME, clanName, sizeof(clanName));	
		format(logText, sizeof(logText), "ulepszyl %s na poziom %d | klan: %s", upgradedSkillName, bbClan[upgradedSkill], clanName);	
		logBB(id, logText)
	}
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
	
	if (!online) ColorChat(id, GREEN, "%s Na serwerze nie ma gracza, ktorego moglbys zaprosic!", PREFIXSAY);
	else menu_display(id, menu ,0);
}

public inviteMenu_2(id, menu, item){
	if (item == MENU_EXIT) {
		menu_destroy(menu)
		globalClanMenu(id);
		return ;
	}

	new target = menuList[id][item];

	if (!is_user_connected(target)) {
		ColorChat(id, GREEN, "%s Wybranego gracza nie ma juz na serwerze.", PREFIXSAY);
		return;
	} 
	userClanSend[target] = id
	
	inviteConfirmMenu(target);

	ColorChat(id, GREEN, "%s Zaprosiles^x03 %s^x01 do do twojego klanu.", PREFIXSAY, userName[target]);
	
	globalClanMenu(id);
}

public inviteConfirmMenu(id){	
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
				ColorChat(id, GREEN, "%s Gracza, ktory cie zaprosil nie ma juz na serwerze", PREFIXSAY);
				return;
			}
		
			if (((get_clan_info(clan[target], CLAN_LEVEL) / 3 ) + gClanStartMenber) <= get_clan_info(clan[target], CLAN_MEMBERS)) {
				ColorChat(id, GREEN, "%s Niestety, w tym klanie nie ma juz wolnego miejsca.", PREFIXSAY);
				return;
			}
		
			new clanName[64];
		
			get_clan_info(clan[target], CLAN_NAME, clanName, sizeof(clanName));
			
			set_user_clan(id, clan[target]);
			
			ColorChat(id, GREEN, "%s Dolaczyles do klanu^x03 %s^01.", PREFIXSAY, clanName);
			new logText[128];
			logType[target] = LOG_CLAN_ADD;
			if(logType[target] == LOG_CLAN_ADD){
				
				
				format(logText, sizeof(logText), "dodal: %s | klan: %s", userName[id],  clanName);
				
				logBB(target, logText)
			}
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
		ColorChat(id, GREEN, "%s Nie mozesz zarzadzac soba!", PREFIXSAY);

		loadStatsSql(id, 8);

		return PLUGIN_HANDLED;
	}

	if (clan[userId]) chosenId[id] = get_user_userid(userId);

	if (flag == STATUS_LEADER) {
		ColorChat(id, GREEN, "%s Nie mozna zarzadzac przywodca klanu!", PREFIXSAY);

		loadStatsSql(id, 8);

		return PLUGIN_HANDLED;
	}

	if(get_user_status(id) == STATUS_DEPUTY && get_user_status(userId) == STATUS_DEPUTY){
		ColorChat(id, GREEN, "%s Nie mozna zarzadzac osoba co ma taka sama range!", PREFIXSAY);
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
	logType[id] = LOG_CLAN_MANAGE;
	if(logType[id] == LOG_CLAN_MANAGE){
		new clanName[64];
		get_clan_info(clan[id], CLAN_NAME, clanName, sizeof(clanName));
	
	
		for (new i = 1; i <= maxPlayers; i++) {
			if (!is_user_connected(i) || clan[i] != clan[id]) continue;
		
			if (get_user_userid(i) == chosenId[id]) {
				switch (status) {
					case STATUS_LEADER: {
						set_user_status(id, STATUS_DEPUTY);
						set_user_status(i, STATUS_LEADER);
	
						ColorChat(i, GREEN,  "%s Zostales mianowany przywodca klanu.", PREFIXSAY);
						
						format(logText, sizeof(logText), "mianowal nowego przywodce klanu: %s |  klan: %s", userName[i],  clanName);
						
					}
					case STATUS_DEPUTY: {
						set_user_status(i, STATUS_DEPUTY);
	
						ColorChat(i, GREEN,  "%s Zostales zastepca przywodcy klanu!", PREFIXSAY);		
						
						format(logText, sizeof(logText), "mianowal nowego zastepce klanu: %s |  klan: %s", userName[i],  clanName);
						
					}
					case STATUS_MEMBER: {
						set_user_status(i, STATUS_MEMBER);
	
						ColorChat(i, GREEN,   "%s Zostales zdegradowany do rangi czlonka klanu.", PREFIXSAY);
			
						format(logText, sizeof(logText), "zdegradowal: %s do czlonka klanu | klan: %s", userName[i],  clanName);	
					}
					case STATUS_NONE: {
						set_user_clan(i);
	
						ColorChat(i, GREEN,  "%s Zostales wyrzucony z klanu.", PREFIXSAY);
						
						format(logText, sizeof(logText), "usunal gracza: %s | klan: %s", userName[i],  clanName);
					}
				}
	
				playerOnline = true;
	
				continue;
			}
			
			switch (status) {
				case STATUS_LEADER: ColorChat(i, GREEN, "%s^x03 %s^01 zostal nowym przywodca klanu.", PREFIXSAY, chosenName[id]);
				case STATUS_DEPUTY: ColorChat(i, GREEN,  "%s^x03 %s^x01 zostal zastepca przywodcy klanu.",PREFIXSAY,  chosenName[id]);
				case STATUS_MEMBER: ColorChat(i, GREEN, "%s^x03 %s^x01 zostal zdegradowany do rangi czlonka klanu.", PREFIXSAY, chosenName[id]);
				case STATUS_NONE: ColorChat(i, GREEN, "%s^x03 %s^01 zostal wyrzucony z klanu.", PREFIXSAY, chosenName[id]);
			}
		}
	
		if (!playerOnline) {
			save_member(id, status, _, chosenName[id]);
			
			if (status == STATUS_NONE) set_clan_info(clan[id], CLAN_MEMBERS, get_clan_info(clan[id], CLAN_MEMBERS) - 1);
	
			if (status == STATUS_LEADER) set_user_status(id, STATUS_DEPUTY);
		}
		logBB(id, logText)
	}
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
		ColorChat(id, GREEN, "%s Nie mozesz wplacic mniej niz^x03 1 Brylek^x01!", PREFIXSAY);

		return PLUGIN_HANDLED;
	}
	
	if (userNugget[id] < nuggetAmount) { 
		ColorChat(id, GREEN, "%s Nie masz tyle^x03 Brylek^x01!", PREFIXSAY);

		return PLUGIN_HANDLED;
	}

	userNugget[id] -= nuggetAmount;
	
	set_clan_info(clan[id], CLAN_NUGGET, get_clan_info(clan[id], CLAN_NUGGET) + nuggetAmount);

	add_deposited_nugget(id, nuggetAmount);
	
	ColorChat(id, GREEN,"%s Wplaciles^x03 %s^x01 Brylek do klanu.", PREFIXSAY, formatNumber(nuggetAmount));
	ColorChat(id, GREEN,"%s Aktualnie twoj klan ma^x03 %s^x01 Brylek.", PREFIXSAY, formatNumber(get_clan_info(clan[id], CLAN_NUGGET)));
	
	
	new logText[128];
	new clanName[64];	
	logType[id] = LOG_CLAN_DEPOSIT;
	if(logType[id] == LOG_CLAN_DEPOSIT){
		
		get_clan_info(clan[id], CLAN_NAME, clanName, sizeof(clanName));	
		format(logText, sizeof(logText), "wplacil [ %d Brylek ( Aktualnie: %d ) ] | klan: %s", nuggetAmount, get_clan_info(clan[id], CLAN_NUGGET), clanName);
					
		
	}
	logBB(id, logText)
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
		ColorChat(id, GREEN, "---^x01 Nic nie wpisales!^x04 ---");
		return ;
		
	}
	if (strlen(szArg) >= 43 ) {
		ColorChat(id, GREEN, "---^x01 Za dluga wiadomosc^x04 ---");
		return ;
	}
	bbClan[CLAN_INFO] = szArg;

	ArraySetArray(bbClans, get_clan_id(clan[id]), bbClan);
	save_clan(get_clan_id(clan[id]));
	
	ColorChat(id, GREEN, "---^x01 Nowe ogloszenie:^x03 %s^x04 ---", szArg);
	
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
			ColorChat(i, GREEN,"%s Twoj klan zostal rozwiazany.", PREFIXSAY);
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
