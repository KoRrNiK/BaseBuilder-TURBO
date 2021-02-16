#include <amxmodx>
#include <amxmisc>
#include <fun>
#include <cstrike>
#include <fakemeta>
#include <fakemeta_util>
#include <engine>
#include <hamsandwich>
#include <ColorChat>
#include <sqlx>

new userWarningAccept[33];

public warningInfo(id){
	
	if(!isAdmin(id)) return PLUGIN_CONTINUE;
	
	userWarningAccept[id] = false;
	
	new target = userWarningInfo[id];
	
	
	new gText[1756], iLen;

	iLen = format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Informacje o graczu:\r %s:^n", userName[target]);
	
	if( userWarningAmount[target] == 0)
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r1.\y Lista Ostrzezen\d [\rBrak\d]");
	else iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r1.\y Lista Ostrzezen\d [\w%d\d/\w%d\d]", userWarningAmount[target], MAXWAR);
	
	if( userWarningAmount[target] >= MAXWAR)
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\d2. Dodaj Ostrzezenie");
	else iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r2.\w Dodaj Ostrzezenie");
	
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\r0.\w Wyjdz");
	
	showMenu(id, B1 | B2 | B0, gText, -1, "warningInfo"  );
	return PLUGIN_HANDLED;
}

public warningInfo_2(id, item){
	new target = userWarningInfo[id];
	switch(item){
		case 0:{
			loadStatsSql(id,9);
			loadStatsSql(target,9);
			loadStatsSql(id, 10);
		}
		case 1:{
			if(target == id && !isSuperAdmin(id)){
				ColorChat(id, GREEN, "---^x01 Nie mozesz sobie nadac ostrzezenia!^x04 ---");
				warningInfo(id);
				return;
			}
			if(userWarningAmount[target] >= MAXWAR){
				ColorChat(id, GREEN, "---^x01 Gracz Posiada juz^x03 %d ostrzezen^x01! Musisz jakies usunac aby dodac nowe!^x04 ---", MAXWAR);
				warningInfo(id);
				return;
			}
			client_print(id, print_center, "Wpisz teraz opis ostrzezenia!");
			client_cmd(id, "messagemode warning");
		}		
	}	
}
public writeWarning(id){
	
	new target = userWarningInfo[id];
	
	new szArg[MAXLEN];
	
	read_args(szArg, sizeof(szArg));
	remove_quotes(szArg);
	trim(szArg);
	
	if(userWarningAmount[target] >= MAXWAR){
		ColorChat(id, GREEN, "---^x01 Gracz Posiada juz^x03 %d ostrzezen^x01! Musisz jakies usunac aby dodac nowe!^x04 ---", MAXWAR);
		warningInfo(id);
		return;
	}
	if (equal(szArg, "") || strlen(szArg) <  1 ) {
		ColorChat(id, GREEN, "---^x01 Nic nie wpisales!^x04 ---");
		warningInfo(id);
		return;
	}
	new globalTime[32], map[33]; 
	
	get_time("%d.%m.%Y - %H:%M:%S",globalTime,sizeof(globalTime));
	get_mapname(map, sizeof(map));
	
	userWarningName[target] = szArg;
	userWarningTime[target] = globalTime;
	userWarningMap[target] = map;
	userWarningPlayer[target] = userName[target];
	userWarningAdmin[target] = userName[id];

	warningAddMenu(id);
}
public warningAddMenu(id){
	
	new target = userWarningInfo[id];
	
	new gText[1756], iLen;

	iLen = format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Informacje o ostrzezeniu!^n^n");
	
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dGracz:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], userWarningPlayer[target]);
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dAdmin:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], userWarningAdmin[target]);
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dOpis:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], userWarningName[target]);
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dMapa:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], userWarningMap[target]);
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dGodzina:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], userWarningTime[target]);

	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r1.\w Zapisz %s", userWarningAccept[id] ? "\r[POTWIERDZ]":"");
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r2.\y Zmien Opis");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\r0.\w Wyjdz");
	
	showMenu(id, B1 | B2| B0, gText, -1, "warningAddMenu"  );
	return PLUGIN_HANDLED;
}
public warningAddMenu_2(id, item){
	new target = userWarningInfo[id];
	
	switch(item){
		case 0:{
			if(!userWarningAccept[id]){
				userWarningAccept[id] = true;
				warningAddMenu(id);
				return;
			}
			
			ColorChat(target, GREEN, "---^x01 Dostales ostrzezenie od^x03 %s^x01!^x04 ---", userName[id]); 
			ColorChat(target, GREEN, "---^x01 Opis:^x03 %s^x04 ---", userWarningName[target]); 
			ColorChat(id, GREEN, "---^x01 Dales ostrzezenie graczowi:^x03 %s^x04 ---", userName[target]); 
				
			userWarningAmount[target] ++;
			
			saveWarning(id, target);
		}
		case 1:{
			client_print(id, print_center, "Wpisz teraz opis ostrzezenia");
			warningAddMenu(id);
			client_cmd(id, "messagemode warning");
		}
	}
}
public warningList_2(id, menu, item){
	
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return;
		
	}	
	new target = userWarningMenu[id][item];
	userWarningItem[id] = target;

	loadDescWarning(id);
}
public loadDescWarning(id){
	
	if(is_user_hltv(id)) return PLUGIN_CONTINUE;
		
	if (!sqlConnected) {
		log_amx("[SQL-LOG] Brak polaczenia");
		return PLUGIN_CONTINUE;
	}
	
	new queryData[512];
	new tempId[3];
	
	tempId[0] = id;
	tempId[1] = userWarningInfo[id];
	tempId[2] = userWarningItem[id];

	format(queryData, sizeof(queryData), "\
		SELECT `warnings`.*, `playerWarning`.`name` AS `gracz`, `adminWarning`.`name` AS `admin`\
		FROM `warnings`\
		LEFT JOIN `players` AS `playerWarning` ON `warnings`.`idplayer` = `playerWarning`.`idplayer`\
		LEFT JOIN `players` AS `adminWarning` ON `warnings`.`idadmin` = `adminWarning`.`idplayer`\
		WHERE `warnings`.`idwarning`='%d'", tempId[2]);

	SQL_ThreadQuery(sql, "loadDescWarning_2", queryData, tempId, sizeof(tempId));

	return PLUGIN_CONTINUE;
}
public loadDescWarning_2(failState, Handle:query, error[], errorNum, tempId[], dataSize){
	
	new id 		= 	tempId[0];

	if (failState){
		if (failState == TQUERY_CONNECT_FAILED) log_amx("[SQL-LOG] Nie mozna polaczyc sie z baz1 danych SQL. Blad: %s (%d)", error, errorNum);
		else if (failState == TQUERY_QUERY_FAILED) log_amx("[SQL-LOG] Zapytanie watkowe nie powiodlo sie. Blad: %s (%d)", error, errorNum);
		return PLUGIN_HANDLED;
	}	
	
	new gText[1756], iLen = 0, szInfo[5][70];
	
	SQL_ReadResult(query, SQL_FieldNameToNum(query, "desc"), szInfo[0], sizeof(szInfo[]));
	SQL_ReadResult(query, SQL_FieldNameToNum(query, "gracz"), szInfo[1], sizeof(szInfo[]));
	SQL_ReadResult(query, SQL_FieldNameToNum(query, "admin"), szInfo[2], sizeof(szInfo[]));
	SQL_ReadResult(query, SQL_FieldNameToNum(query, "time"), szInfo[3], sizeof(szInfo[]));
	SQL_ReadResult(query, SQL_FieldNameToNum(query, "map"), szInfo[4], sizeof(szInfo[]));

	iLen = format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Informacje o ostrzezeniu!^n^n");
	
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dGracz:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], szInfo[1]);
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dAdmin:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], szInfo[2]);
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dOpis:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], szInfo[0]);
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dMapa:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], szInfo[4]);
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dGodzina:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], szInfo[3]);

	if(isSuperAdmin(id)){
		iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\r1.\w Usun %s", userWarningAccept[id] ? "\r[POTWIERDZ]":"");
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r2.\y Zmien Opis");
	} else {
		iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\d1. Usun");
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\d2. Zmien Opis");
	}
	
	
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n^n\r0.\w Wyjdz");
	
	
	showMenu(id, B1 | B2| B0, gText, -1, "warningDesc");	
	return PLUGIN_HANDLED;
}
public warningDesc_2(id, item){
	switch(item){
		case 0:{
			
			if(!isSuperAdmin(id)) {
				ColorChat(id, GREEN, "---^x01 Brak dostepu!^x04 ---");
				loadDescWarning(id);
				return;
			}
			if(!userWarningAccept[id]){
				userWarningAccept[id] = true;
				loadDescWarning(id);
				return;
			}
			removeWarning(id);
		}
		case 1:{
			if(!isSuperAdmin(id)) {
				ColorChat(id, GREEN, "---^x01 Brak dostepu!^x04 ---");
				loadDescWarning(id);
				return;
			}
			loadDescWarning(id);
			client_print(id, print_center, "Wpisz teraz nowy opis ostrzezenia!");
			client_cmd(id, "messagemode change_desc");
	
		}
	}
}
public  updateWarning(id){
	
	
	
	new queryData[256], tempId[3];
	
	tempId[0] = id;
	tempId[1] = userWarningInfo[id];
	tempId[2] = userWarningItem[id];
	
	new szArg[MAXLEN];
	
	read_args(szArg, sizeof(szArg));
	remove_quotes(szArg);
	trim(szArg);
	
	if (equal(szArg, "") || strlen(szArg) <  1 ) {
		ColorChat(id, GREEN, "---^x01 Nic nie wpisales!^x04 ---");
		return ;
	}


	if(is_user_connected(tempId[1])){
		ColorChat(tempId[1], GREEN,"---^x01 Admin^x03 %s^x01 zmienil opis twojego ostrzezenia!^x04 ---", userName[id]);
		ColorChat(id, GREEN,"---^x01 Zmieniles opis ostrzezenia graczowi^x03 %s^x01 ---", userName[tempId[1]]);
	} else{
		ColorChat(tempId[1], GREEN,"---^x01 Nie ma na serwerze gracza ktoremu chciales zmienic opis ostrzezenia^x04 ---");
		return;
	}
	
	format(queryData, sizeof(queryData), "UPDATE `warnings` SET `desc`='%s' WHERE `idplayer`='%d' AND `idwarning`='%d'", szArg, userSqlId[tempId[1]], tempId[2]);
	SQL_ThreadQuery(sql, "saveStatsHandlerSql", queryData, tempId, sizeof(tempId));
	
	new gText[128];
	logType[id] = LOG_WARNING_CHANGE;
	if(logType[id] == LOG_WARNING_CHANGE){
		format(gText, sizeof(gText), "zaktualizowal ostrzezenia gracza [%s] nowy opis [%s]", userName[tempId[1]], szArg);
		logBB(id,gText);
	}

			
	loadDescWarning(id);
}
public  removeWarning(id){
	
	
	
	new queryData[256], tempId[3];
	
	tempId[0] = id;
	tempId[1] = userWarningInfo[id];
	tempId[2] = userWarningItem[id];
	
	if(is_user_connected(tempId[1])){
		ColorChat(tempId[1], GREEN,"---^x01 Admin^x03 %s^x01 usunal tobie ostrzezenie!^x04 ---", userName[id]);
		ColorChat(id, GREEN,"---^x01 Usunales ostrzezenie graczowi^x03 %s^x01 ---", userName[tempId[1]]);
	} else{
		ColorChat(tempId[1], GREEN,"---^x01 Nie ma na serwerze gracza ktoremu chciales usunac ostrzezenie^x04 ---");
		return;
	}
	
	userWarningAmount[tempId[1]] --;
	
	
	
	format(queryData, sizeof(queryData), "DELETE FROM `warnings` WHERE `idplayer`='%d' AND `idwarning`='%d'", userSqlId[tempId[1]], tempId[2]);
	SQL_ThreadQuery(sql, "saveStatsHandlerSql", queryData, tempId, sizeof(tempId));
	
	new gText[128];
	logType[id] = LOG_WARNING_REMOVE;
	if(logType[id] == LOG_WARNING_REMOVE){
		format(gText, sizeof(gText), "usunal ostrzezenie graczowi [%s] ID [%d]", userName[tempId[1]], tempId[2]);
		logBB(id,gText);	
	}
	
	userWarningAccept[id] = false;
	warningInfo(id);
}
public saveWarning(id, target){
	new queryData[256], tempId[2];
	
	tempId[0] = id;
	tempId[1] = target;
			
	format(queryData, sizeof(queryData), "\
				INSERT INTO `warnings` (`idplayer`, `idadmin`, `desc`, `time`, `map`)\
				VALUES (%d,%d,'%s','%s','%s')", userSqlId[target], userSqlId[id], userWarningName[target], userWarningTime[target], userWarningMap[target]);
	
	SQL_ThreadQuery(sql, "saveStatsHandlerSql", queryData, tempId, sizeof(tempId));

	new gText[128];
	logType[id] = LOG_WARNING_ADD;
	if(logType[id] == LOG_WARNING_ADD){
		format(gText, sizeof(gText), "dodal ostrzezenie graczowi [%s] opis [%s] godzina [%s] mapa [%s]", userName[tempId[1]], userWarningName[target], userWarningTime[target], userWarningMap[target]);
		logBB(id,gText);
	}
	
	userWarningAccept[id] = false;
	warningInfo(id);
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
