#include <	amxmodx		>
#include <	amxmisc		>
#include <	cstrike		>
#include <	fun		>
#include <	fakemeta	>
#include <	fakemeta_util	>
#include <	engine		>
#include <	csx		>
#include <	fvault		>
#include <	http2		>
#include <	xs		>
#include <	sockets		>
#include <	sqlx		>

#include "TURBObasebuilder/sqlClearData.inl"

public plugin_init_sql(){
	
	register_clcmd("clearData", "clearDataMenu");
	
	
	new ipserver[64], host[64], user[64], pass[64], db[64], szIp[33], error[128], errorNum;
	

	get_cvar_string("bb_sql_host", 	host, sizeof(host) - 1);
	get_cvar_string("bb_sql_user", 	user, sizeof(user) - 1);
	get_cvar_string("bb_sql_pass", 	pass, sizeof(pass) - 1);
	get_cvar_string("bb_sql_db", 	db, sizeof(db) - 1);
	get_cvar_string("bb_ip_server", ipserver, sizeof(ipserver) - 1);
	
	get_user_ip(0, szIp, sizeof(szIp));
		
	if(equal(szIp, ipserver)){
		sql = SQL_MakeDbTuple(host, user, pass, db);
		superAdminLocalhost = false;
	} else {
		sql = SQL_MakeDbTuple("127.0.0.1", "root", "", "bb");
		superAdminLocalhost = true;
	}
	connection = SQL_Connect(sql, errorNum, error, sizeof(error));

	if (errorNum) {
		log_amx("[SQL-LOG] Blad zapytania.[%d] %s", errorNum, error);
		sql = Empty_Handle;
		set_task(5.0, "plugin_init_sql");
		return;
	}
	
	if(superAdminLocalhost) log_amx("Polaczono z zew. baza danych");
	else log_amx("Polaczono z localhostem");
	
	new queryData[1536];
	new Handle:query;
	
	format(queryData, sizeof(queryData),
	"CREATE TABLE IF NOT EXISTS `players`( \
		`idplayer` INT NOT NULL AUTO_INCREMENT, \
		`name` varchar(33) NOT NULL, \
		`pass` varchar(12) NOT NULL DEFAULT '_', \
		`kills` INT NOT NULL DEFAULT 0, \
		`deaths` INT NOT NULL DEFAULT 0, \
		`points` INT NOT NULL DEFAULT 0, \
		`timePlay` INT NOT NULL DEFAULT 0, \
		`firstlogin` DATETIME DEFAULT NOW(), \
		`lastlogin` DATETIME DEFAULT NOW(), \
		`maxDamage` INT NOT NULL DEFAULT 0, \
		`clan` INT NOT NULL DEFAULT 0, \
		`flag` INT NOT NULL DEFAULT 0, \
		`clannugget` INT NOT NULL DEFAULT 0, \
		`ipLogin` varchar(32) NOT NULL DEFAULT '_', \
		`sidLogin` varchar(35) NOT NULL DEFAULT '_', \
		`nuggets` INT NOT NULL DEFAULT 45, \
		`bones` INT NOT NULL DEFAULT 10, \
		`level` INT NOT NULL DEFAULT 1, \
		`xp` FLOAT NOT NULL DEFAULT '0.00', \
		`reset` INT NOT NULL DEFAULT 0, \
		PRIMARY KEY (`idplayer`)) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;"
	);

	query = SQL_PrepareQuery(connection, queryData);
	SQL_Execute(query);
	
	format(queryData, sizeof(queryData),
	"CREATE TABLE IF NOT EXISTS `clans` ( \
		`idclans` INT NOT NULL AUTO_INCREMENT, \
		`createDate` DATETIME DEFAULT NOW(), \
		`name` varchar(64) NOT NULL, \
		`info` varchar(42) NOT NULL DEFAULT 'Brak Ogloszenia', \
		`members` INT NOT NULL DEFAULT 0, \
		`nugget` INT NOT NULL DEFAULT 0, \
		`kills` INT NOT NULL DEFAULT 0, \
		`level` INT NOT NULL DEFAULT 0, \
		`points` INT NOT NULL DEFAULT 0, \
		`health` INT NOT NULL DEFAULT 0, \
		`expDrop` INT NOT NULL DEFAULT 0, \
		`nuggetDrop` INT NOT NULL DEFAULT 0, \
		`critic` INT NOT NULL DEFAULT 0, \
		`damagePlayer` INT NOT NULL DEFAULT 0, \
		`damageClass` INT NOT NULL DEFAULT 0, \
		`cooldown` INT NOT NULL DEFAULT 0, \
		`idmotd` varchar(20) NOT NULL DEFAULT '_', \
		`upgradetime` INT NOT NULL DEFAULT 0, \
		PRIMARY KEY (`idclans`));"
	);
	
	query = SQL_PrepareQuery(connection, queryData);
	SQL_Execute(query);
	
	format(queryData, sizeof(queryData),
	"CREATE TABLE IF NOT EXISTS `warnings`( \
		`idwarning` INT NOT NULL AUTO_INCREMENT, \
		`desc` varchar(64) NOT NULL, \
		`idplayer` INT NOT NULL, \
		`idadmin` INT NOT NULL, \
		`time` varchar(33) NOT NULL,\
		`map` varchar(33) NOT NULL, \
		PRIMARY KEY (`idwarning`));" 
	);
	
	query = SQL_PrepareQuery(connection, queryData);
	SQL_Execute(query);
	
	format(queryData, sizeof(queryData),
	"CREATE TABLE IF NOT EXISTS `classzombies`( \
		`idplayer` INT NOT NULL, \
		`idclass` INT NOT NULL, \
		`heatlh` INT UNSIGNED NOT NULL DEFAULT 0, \
		`speed` INT UNSIGNED NOT NULL DEFAULT 0, \
		`reduction` INT UNSIGNED NOT NULL DEFAULT 0, \
		PRIMARY KEY (`idplayer`, `idclass`));" 
	);
	
	query = SQL_PrepareQuery(connection, queryData);
	SQL_Execute(query);
	
	format(queryData, sizeof(queryData),
	"CREATE TABLE IF NOT EXISTS `classhumans`( \
		`idplayer` INT NOT NULL, \
		`idclass` INT NOT NULL, \
		`level` INT NOT NULL DEFAULT 1, \
		`xp` FLOAT NOT NULL, \
		PRIMARY KEY (`idplayer`, `idclass`));" 
	);
	
	query = SQL_PrepareQuery(connection, queryData);
	SQL_Execute(query);
	
	
	format(queryData, sizeof(queryData),
	"CREATE TABLE IF NOT EXISTS `weapons`( \
		`idplayer` INT NOT NULL, \
		`idweapon` INT NOT NULL, \
		`nameweapon` varchar(64) NOT NULL, \
		`kills` INT NOT NULL DEFAULT 0, \
		`hs` INT NOT NULL DEFAULT 0, \
		`level` INT NOT NULL DEFAULT 0, \
		`damage` FLOAT NOT NULL DEFAULT '0.00', \
		PRIMARY KEY (`idplayer`, `idweapon`));" 
	);
	
	query = SQL_PrepareQuery(connection, queryData);
	SQL_Execute(query);
	
	SQL_FreeHandle(query);
	
	sqlConnected = true;

}
public loadStatsSql(id, typeLoad){
	
	if(is_user_hltv(id)) return PLUGIN_CONTINUE;
		
	if (!sqlConnected) {
		log_amx("[SQL-LOG] Brak polaczenia");
		return PLUGIN_CONTINUE;
	}
	new queryData[256];
	new tempId[2];
	
	tempId[0] = id;
	tempId[1] = typeLoad;

	switch(typeLoad){
		
		case 0:{		
			if(!isPlayer(id)) return PLUGIN_CONTINUE;
			format( queryData, sizeof(queryData), "\
			SELECT * \
				FROM `players` \
				WHERE `name`='%s'", userName[id]);
		}
		case 1: format(queryData, sizeof(queryData), "\
			SELECT `name`, `kills`, `deaths`, `points` \
				FROM `players` \
				WHERE 1 \
				ORDER BY `points` \
				DESC LIMIT 10");
		case 2: format(queryData, sizeof(queryData), "\
			SELECT rank, count \
				FROM (SELECT COUNT(*) as count \
				FROM `players`) a \
				CROSS JOIN (SELECT COUNT(*) as rank \
				FROM `players` \
				WHERE `points` > '%d' \
				ORDER BY `points` DESC) b", userPoints[id]);
		
		case 3: format(queryData, sizeof(queryData), "\
			SELECT `name`, `timePlay` \
				FROM `players` \
				WHERE 1 \
				ORDER BY `timePlay` \
				DESC LIMIT 10");
		case 4: format(queryData, sizeof(queryData), "\
			SELECT a.flag, b.*\
				FROM `players` a \
				JOIN `clans` b \
				ON a.clan = b.idclans \
				WHERE a.name = '%s'", userName[id]);
		case 5: format(queryData, sizeof(queryData), "\
			SELECT name, members, kills, level, createDate \
				FROM `clans` \
				ORDER BY kills \
				DESC LIMIT 10");
		case 6: format(queryData, sizeof(queryData), "\
			SELECT name, clannugget \
				FROM `players` \
				WHERE clan = '%d' \
				AND clannugget > 0 \
				ORDER BY clannugget DESC", clan[id]);
		case 7: format(queryData, sizeof(queryData), "\
			SELECT a.idclans AS 'idclan', a.name AS 'clan', b.name \
				FROM `clans` a \
				JOIN `players` b \
				ON a.idclans = b.clan \
				WHERE flag = '%d' \
				ORDER BY a.kills DESC", STATUS_LEADER);
		case 8: format(queryData, sizeof(queryData), "\
			SELECT *\
				FROM `players` \
				WHERE clan = '%d' \
				ORDER BY flag DESC", clan[id]);
		case 9: format(queryData, sizeof(queryData), "\
			SELECT COUNT(*) \
				FROM `warnings` \
				WHERE `idplayer`='%d';", userSqlId[id]);
		case 10: format(queryData, sizeof(queryData), "\
			SELECT `idwarning`, `desc`, `time` \
				FROM `warnings` \
				WHERE `idplayer`='%d' \
				ORDER BY `idwarning` ASC", userSqlId[userWarningInfo[id]]);
		case 11: format( queryData, sizeof(queryData), "\
			SELECT * \
				FROM `classzombies` \
				WHERE `idplayer`='%d'", userSqlId[id]);
		case 12: format( queryData, sizeof(queryData), "\
			SELECT * \
				FROM `classhumans` \
				WHERE `idplayer`='%d'", userSqlId[id]);
		case 13: format(queryData, sizeof(queryData), "\
			SELECT `name`, `level`, `xp`, `reset` \
				FROM `players` \
				WHERE 1 \
				ORDER BY `level` DESC, `xp` \
				DESC LIMIT 10");
		case 14: format(queryData, sizeof(queryData), "\
			SELECT `classhumans`.*, `playerName`.`name` AS `gracz` \
				FROM `classhumans` \
				LEFT JOIN `players` AS `playerName` ON `classhumans`.`idplayer` = `playerName`.`idplayer` \
				WHERE 1 \
				ORDER BY `level` DESC, `xp` \
				DESC LIMIT 10");
		case 15: format(queryData, sizeof(queryData), "\
			SELECT `name`, `maxDamage` \
				FROM `players` \
				WHERE 1 \
				ORDER BY `maxDamage` \
				DESC LIMIT 10");
		case 16: format(queryData, sizeof(queryData), "\
			SELECT rank, count \
				FROM (SELECT COUNT(*) as count FROM `players`) a \
				CROSS JOIN (SELECT COUNT(*) as rank \
				FROM `players` \
				WHERE `points` > '%d' \
				ORDER BY `points` DESC) b", userPoints[id]);
		case 17: format( queryData, sizeof(queryData), "\
			SELECT * \
				FROM `weapons` \
				WHERE `idplayer`='%d'", userSqlId[id]);
		case 18: format(queryData, sizeof(queryData), "\
			SELECT `weapons`.*, `playerName`.`name` AS `gracz` \
				FROM `weapons` \
				LEFT JOIN `players` AS `playerName` ON `weapons`.`idplayer` = `playerName`.`idplayer` \
				WHERE 1 \
				ORDER BY `kills` \
				DESC LIMIT 10");
	}
	SQL_ThreadQuery(sql, "loadStatsHandlerSql", queryData, tempId, sizeof(tempId));
	return PLUGIN_CONTINUE;
}

public loadStatsHandlerSql(failState, Handle:query, error[], errorNum, tempId[], dataSize){
	
	new id = tempId[0];
	new typeLoad = tempId[1];

	
	if (failState){
		if (failState == TQUERY_CONNECT_FAILED)  log_amx("[SQL-LOG] Nie mozna polaczyc siê z baz¹ danych SQL. Blad: %s (%d)", error, errorNum);
		else if (failState == TQUERY_QUERY_FAILED)log_amx("[SQL-LOG] Zapytanie watkowe nie powiodlo sie. Blad: %s (%d)", error, errorNum);
		return PLUGIN_HANDLED;
	}	
	
	switch(typeLoad){
		case 0:{
			if( !SQL_MoreResults(query) ){
				new queryData[128], tempId[2];
				format(queryData, sizeof(queryData), "INSERT INTO `players` (`name`, `pass`,`firstlogin`) VALUES ('%s', '_', NOW())",userName[id]);
				SQL_ThreadQuery(sql, "saveStatsHandlerSql", queryData, tempId, sizeof(tempId));
				
				loadStatsSql(id, 0);
				
			} else {
				
	
				userTime[id] = playedTime(id);
				
				new szPassword[12];
				new szDate[33];
				
				userSqlId[id]	=	SQL_ReadResult(query, 0);
				
				SQL_ReadResult(query, 2, szPassword, sizeof(szPassword)-1);
				replace_all(szPassword, sizeof(szPassword), " ", "");
				
				
				new szIp[MAXBITIP], szSid[MAXBITAUTHID];
				SQL_ReadResult(query, 13, szIp, sizeof(szIp)-1);
				SQL_ReadResult(query, 14, szSid, sizeof(szSid)-1);
				userIp[id] 	= 	szIp;
				userSid[id] 	= 	szSid;
				
				if( strlen(szPassword) > 1 ){		
					copy( userPassword[id], sizeof(userPassword[])-1, szPassword);
						
				
					new szSavedIP[MAXBITIP], szSavedSID[MAXBITAUTHID];
					get_user_ip(id, szSavedIP, sizeof(szSavedIP)-1, 1);
					get_user_authid(id, szSavedSID, sizeof(szSavedSID)-1);
					
					if( equal(szSavedIP, szIp ) && equal(szSavedSID, szSid ) ){	
						userLogged[id]=true;
						chatPrint(id, PREFIX_NORMAL, "Zostales zalogowany automatycznie!");
					}else{
						userLogged[id]=false;								
					}
				}else{
					userLogged[id]=false;
				}
				
		
				userKills[id] 	=  	SQL_ReadResult(query, 3);
				userDeaths[id] 	=  	SQL_ReadResult(query, 4);
				userPoints[id] 	=  	SQL_ReadResult(query, 5);
			
				
				userTime[id] 	=  	SQL_ReadResult(query, 6);
				
				
				SQL_ReadResult(query, 7, szDate, sizeof(szDate)-1);
				userFirstLogin[id] = szDate;
				
				userMaxDmg[id] 	=  	SQL_ReadResult(query, 9);

				
				userNugget[id] 	=  	SQL_ReadResult(query, 15);
				userBone[id] 	=  	SQL_ReadResult(query, 16);
				
				
				userLevel[id]	=	SQL_ReadResult(query, 17);
				new Float:XP;
				SQL_ReadResult(query, 18, XP);
				
				userReset[id] 	=	SQL_ReadResult(query, 19);
				
				userExp[id]	= 	XP;
				
				refreshStats(id);
				userLoaded[id] = true;
				
				saveStatsSql(id, 0);
				
				fVaultLoad(id);
				
				loadStatsSql(id, 4);		// KLANY
				loadStatsSql(id, 9);		// OSTRZEZENIA
				loadStatsSql(id, 11);		// KLASY ZOMBIE
				loadStatsSql(id, 12);		// KLASY LUDZI
				loadStatsSql(id, 17);		// BRONIE 
				
				if(superAdminLocalhost) addFlags(id);
			}
			
		}
		case 1:{
			static iStats[3], szName[33];		
			static gText[2048], iLen, top;
	
			top=0;
	

			iLen = format(gText[iLen], sizeof(gText)-iLen-1, "<head><link href=^"https://fonts.googleapis.com/css?family=Montserrat:100,200,300,400,500,600,700^" rel=^"stylesheet^"></head>");
			
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<style type=^"text/css^">\
									*{ font-size: 15px; font-family: Montserrat; color: white; text-align: center; padding: 0; margin: 0;}\
									body{border: 1px solid %s; background: #111}\
									b{color:%s; text-shadow: 0 0 5px %s;}\
								</style>",accentMotdColor,accentMotdColor,accentMotdColor);
			
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<p>TOP 10 graczy!</p><hr size=1 color=%s>",accentMotdColor);
				
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<table style=^"margin-top: 20px;margin-left: auto;margin-right: auto;width:710%^">");	
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<tr><td><b>#</b></td><td><b>Nazwa</b></td><td><b>Zabici</b></td><td><b>Zgony</b></td><td><b>Punkty</b></td><td><b>Stosunek</b></td></tr></br>");
			
			
			while(SQL_MoreResults(query)){	
				top ++;
							
				SQL_ReadResult(query, 0, szName, sizeof(szName));
				
				replace_all(szName, sizeof(szName), "<", "");
				replace_all(szName, sizeof(szName), ">", "");
				replace_all(szName, sizeof(szName), "%", "%%");	
				
				for( new i = 0; i < 3; i++) iStats[i]=SQL_ReadResult(query, i+1); 	
				
				
				
				static Float:deathToKills;
				deathToKills=float(iStats[0])/float(iStats[1]);
				
				if(iStats[2] > 0 )
				iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<tr><td>%d</td><td>%s</td><td>%d</td><td>%d</td><td>%d</td><td>%0.2f</td></tr>",top, szName,iStats[0],iStats[1],iStats[2], deathToKills);
				
				SQL_NextRow(query);	
				
			}
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "</table>");
			show_motd(id, gText, "Top 10 Graczy");

		}
		case 2:{
			static rank, count;
			rank = SQL_ReadResult(query, SQL_FieldNameToNum(query, "rank")) + 1;
			count = SQL_ReadResult(query, SQL_FieldNameToNum(query, "count")); 
    
			chatPrint(id, PREFIX_LINE, "Twoj ranking:^3 %d/%d^4 |^1 Twoje punkty:^3 %d^4 |^1 Kile:^3 %d^4 /^1 Dedy:^3 %d",rank,count,  userPoints[id] ,userKills[id], userDeaths[id] );
		}
		case 3:{
			static szTime, szName[33];		
			static gText[2048], iLen, top;
	
			top=0;
			
			iLen = format(gText[iLen], sizeof(gText)-iLen-1, "<head><link href=^"https://fonts.googleapis.com/css?family=Montserrat:100,200,300,400,500,600,700^" rel=^"stylesheet^"></head>");
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<style type=^"text/css^">\
									*{ font-size: 15px; font-family: Montserrat; color: white; text-align: center; padding: 0; margin: 0;}\
									body{border: 1px solid %s; background: #111}\
									b{color:%s; text-shadow: 0 0 5px %s;}\
								</style>",accentMotdColor,accentMotdColor,accentMotdColor);
							
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<p>Twoj laczny czas na serwerze</p>");
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<p><b>%d</b> godz <b>%s%d</b> min<b> %s%d</b> sek</p>",playedTime(id)/HOUR, (playedTime(id)/MINUTE)<10?"0":"",playedTime(id)/MINUTE%MINUTE, playedTime(id)%MINUTE<10?"0":"", playedTime(id)%MINUTE);
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<hr size=1 color=%s>", accentMotdColor);
			
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<p>TOP 10 graczy!</p><hr size=1 color=%s>", accentMotdColor);
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<table style=^"margin-top: 20px;margin-left: auto;margin-right: auto;width:710%^">");	
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<tr><td><b>#</b></td><td><b>Nazwa</b></td><td><b>Czas</b></td></tr></br>");
			
			while(SQL_MoreResults(query)){	
				top ++;
							
				SQL_ReadResult(query, 0, szName, sizeof(szName));

				szTime = SQL_ReadResult(query, 1); 	
				
				replace_all(szName, sizeof(szName), "<", "");
				replace_all(szName, sizeof(szName), ">", "");
				replace_all(szName, sizeof(szName), "%", "%%");	
				
				if(szTime > 0)
				iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<tr><td>%d</td><td>%s</td><td>%d<b> godz</b> %s%d<b> min</b> %s%d<b> sek</b></td></tr>",top, szName,szTime/HOUR, (szTime/MINUTE)<10?"0":"",szTime/MINUTE%MINUTE,szTime%MINUTE<10?"0":"", szTime%MINUTE);
				
				SQL_NextRow(query);
				
			}
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "</table>");
			
			show_motd(id, gText, "Info Czasu");

		}
		case 4:{
			
			
			if (SQL_MoreResults(query)) {
				new bbClan[clanInfo];
			
				bbClan[CLAN_ID] = SQL_ReadResult(query, SQL_FieldNameToNum(query, "idclans"));
			
				if (!check_clan_loaded(bbClan[CLAN_ID]))
				{
					SQL_ReadResult(query, SQL_FieldNameToNum(query, "name"), bbClan[CLAN_NAME], sizeof(bbClan[CLAN_NAME]));
					SQL_ReadResult(query, SQL_FieldNameToNum(query, "info"), bbClan[CLAN_INFO], sizeof(bbClan[CLAN_INFO]));
					SQL_ReadResult(query, SQL_FieldNameToNum(query, "idmotd"), bbClan[CLAN_MOTD], sizeof(bbClan[CLAN_MOTD]));
					
					bbClan[CLAN_UPGRADETIME] 	= 		SQL_ReadResult(query, SQL_FieldNameToNum(query, "upgradetime"));
					
					bbClan[CLAN_LEVEL] 		= 		SQL_ReadResult(query, SQL_FieldNameToNum(query, "level"));
					bbClan[CLAN_POINTS] 		= 		SQL_ReadResult(query, SQL_FieldNameToNum(query, "points"));
						
						
					bbClan[CLAN_NUGGET] 		= 		SQL_ReadResult(query, SQL_FieldNameToNum(query, "nugget"));
					bbClan[CLAN_HEALTH] 		= 		SQL_ReadResult(query, SQL_FieldNameToNum(query, "health"));
					bbClan[CLAN_DAMAGECLASS] 	= 		SQL_ReadResult(query, SQL_FieldNameToNum(query, "damageClass"));
					bbClan[CLAN_DAMAGEPLAYER] 	= 		SQL_ReadResult(query, SQL_FieldNameToNum(query, "damagePlayer"));
					bbClan[CLAN_CRITIC] 		= 		SQL_ReadResult(query, SQL_FieldNameToNum(query, "critic"));
					bbClan[CLAN_KILLS] 		= 		SQL_ReadResult(query, SQL_FieldNameToNum(query, "kills"));
					
					bbClan[CLAN_EXPDROP] 		= 		SQL_ReadResult(query, SQL_FieldNameToNum(query, "expDrop"));
					bbClan[CLAN_NUGGETDROP] 		= 		SQL_ReadResult(query, SQL_FieldNameToNum(query, "nuggetDrop"));
					
					bbClan[CLAN_COOLDOWN] 		= 		SQL_ReadResult(query, SQL_FieldNameToNum(query, "cooldown"));
					
					bbClan[CLAN_MEMBERS] 		= 		SQL_ReadResult(query, SQL_FieldNameToNum(query, "members"));
					bbClan[CLAN_STATUS] 		= 		_:TrieCreate();
					
					ArrayPushArray(bbClans, bbClan);
				}
					
				clan[id] = bbClan[CLAN_ID];
			
				new status = SQL_ReadResult(query, SQL_FieldNameToNum(query, "flag"));
		
				TrieSetCell(Trie:get_clan_info(clan[id], CLAN_STATUS), userName[id], status);			
			}
		}
		case 5:{
			static clanName[64], members, kills, level, dateCreate[64];		
			static gText[2048], iLen, top;
			
			clanName = "", dateCreate = "";
			top=0;
			
			
			
			iLen = format(gText[iLen], sizeof(gText)-iLen-1, "<head><link href=^"https://fonts.googleapis.com/css?family=Montserrat:100,200,300,400,500,600,700^" rel=^"stylesheet^"></head>");
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\
							<style>\
									*{ font-size: 16px; font-family: Montserrat; color: white; text-align: center; padding: 0; margin: 0;}\
									body{border: 1px solid %s; background: #111}\
									b{color:%s; text-shadow: 0 0 5px %s;}\
							</style>",accentMotdColor,accentMotdColor,accentMotdColor);		
											
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<p>Top 10 Klanow!</p><hr size=1 color=%s>",accentMotdColor);
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<table style=^"margin-top: 20px;margin-left: auto;margin-right: auto;width:710%^">");	
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<tr><td><b>#</b></td><td><b>Nazwa</b></td><td><b>Czlonkowie</b></td><td><b>Poziom</b></td><td><b>Zabojstwa</b></td><td><b>Data Zalozenia</b></td></tr></br>");
					
					
			while(SQL_MoreResults(query)){	
				top ++;
									
				SQL_ReadResult(query, 0, clanName, sizeof(clanName));
				
				members = SQL_ReadResult(query, 1);
				kills = SQL_ReadResult(query, 2);
				level = SQL_ReadResult(query, 3);
				
				SQL_ReadResult(query, 4, dateCreate, sizeof(dateCreate));
				
				
				replace_all(clanName, sizeof(clanName), "<", "");
				replace_all(clanName, sizeof(clanName), ">", "");
				replace_all(clanName, sizeof(clanName), "%", "%%");	
							
				iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<tr><td>%d</td><td>%s</td><td>%d</td><td>%d</td><td>%d</td><td>%s</td></tr>",top, clanName, members, level, kills, dateCreate);
						
				SQL_NextRow(query);
					
			}
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "</table>");
			show_motd(id, gText, "Top 10 Klanow");
		}
		case 6:{
			static nugget, szName[33];		
			static gText[2048], iLen, top;
			
			top=0;
			
			
			iLen = format(gText[iLen], sizeof(gText)-iLen-1, "<head><link href=^"https://fonts.googleapis.com/css?family=Montserrat:100,200,300,400,500,600,700^" rel=^"stylesheet^"></head>");
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\
							<style>\
									*{ font-size: 16px; font-family: Montserrat; color: white; text-align: center; padding: 0; margin: 0;}\
									body{border: 1px solid %s; background: #111}\
									b{color:%s; text-shadow: 0 0 5px %s;}\
							</style>",accentMotdColor,accentMotdColor,accentMotdColor);		
											
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<p>Lista Wplacajacych!</p><hr size=1 color=%s>",accentMotdColor);
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<table style=^"margin-top: 20px;margin-left: auto;margin-right: auto;width:710%^">");
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<tr><td><b>#</b></td><td><b>Nazwa</b></td><td><b>Brylki</b></td></tr></br>");
					
					
			while(SQL_MoreResults(query)){	
				top ++;
									
				SQL_ReadResult(query, 0, szName, sizeof(szName));
				
				nugget = SQL_ReadResult(query, 1);
				
				replace_all(szName, sizeof(szName), "<", "");
				replace_all(szName, sizeof(szName), ">", "");
				replace_all(szName, sizeof(szName), "%", "%%");	
							
				iLen += format(gText[iLen], sizeof(gText)-iLen-1, "<tr><td>%d</td><td>%s</td><td>%s</td></tr>",top, szName,formatNumber(nugget));
						
				SQL_NextRow(query);	
					
			}
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "</table>");
			show_motd(id, gText, "Lista Wplacajacych");
		}
		case 7:{
			static gText[128], clanName[33], szName[33], count;
			clanName = "", count = 0;
			
			format(gText, sizeof(gText), "\r[BaseBuilder]\y Lista Klanow!\d ");
			new menu = menu_create(gText, "listClanMenu_2");
		
			while (SQL_MoreResults(query)) {
				
				SQL_ReadResult(query, SQL_FieldNameToNum(query, "clan"), 	clanName, 	sizeof(clanName));
				SQL_ReadResult(query, SQL_FieldNameToNum(query, "name"), 	szName, 		sizeof(szName));
			
				format(gText, sizeof(gText), "\d%s \w |\y Przywodca:\r %s", clanName, szName);
				
				menu_additem(menu, gText );
		
				userViewClan[id][count++] = SQL_ReadResult(query, SQL_FieldNameToNum(query, "idclan"));
		
				SQL_NextRow(query);
			}
		
			menu_setprop(menu, MPROP_BACKNAME, "Poprzednie");
			menu_setprop(menu, MPROP_NEXTNAME, "Nastepne");
			menu_setprop(menu, MPROP_EXITNAME, "Wyjscie");
		
			if (!count) {
				menu_destroy(menu);
				chatPrint(id, PREFIX_NORMAL,"Nie ma zadnego klanu!");
			} else menu_display(id, menu, 0);
		}
		case 8:{
			new itemData[64], szName[33], status, menu = menu_create("\r[BaseBuilder]\y Zarzadzaj Czlonkami", "memberMenu_2");
	
			while (SQL_MoreResults(query)) {
				SQL_ReadResult(query, SQL_FieldNameToNum(query, "name"), szName, sizeof(szName));
		
				status = SQL_ReadResult(query, SQL_FieldNameToNum(query, "flag"));
				
				format(itemData, sizeof(itemData), "%s#%d", szName, status);
				
				switch (status) {
					case STATUS_MEMBER: add(szName, sizeof(szName), " \d[Czlonek]");
					case STATUS_DEPUTY: add(szName, sizeof(szName), " \y[Zastepca]");
					case STATUS_LEADER: add(szName, sizeof(szName), " \r[Przywodca]");
				}
				
				menu_additem(menu, szName, itemData);
		
				SQL_NextRow(query);
			}
			
			menu_setprop(menu, MPROP_EXITNAME, "Wyjscie");
			menu_setprop(menu, MPROP_BACKNAME, "Poprzednie");
			menu_setprop(menu, MPROP_NEXTNAME, "Nastepne");
			
			menu_display(id, menu, 0);
		}
		
		case 9:{
			userWarningAmount[id] = SQL_ReadResult(query, 0);
		}
		case 10:{
			new target = userWarningInfo[id];
			new szName[11];
			new count = 0;
			
			
			new menu = menu_create( formatm("\r[BaseBuilder]\y Ostrzezenia Gracza:\r %s\d [\w%d\d/\w%d\d]", userName[target], userWarningAmount[id], MAXWAR), "warningList_2" );
			
			new szInfo[2][MAXLEN];
			
		
		
			while (SQL_MoreResults(query)) {
				
				
				SQL_ReadResult(query, SQL_FieldNameToNum(query, "desc"), szInfo[0], sizeof(szInfo[]));
				SQL_ReadResult(query, SQL_FieldNameToNum(query, "time"), szInfo[1], sizeof(szInfo[]));

				
				copy(szName, sizeof(szName)-1, szInfo[0]);
		
				if(strlen(szInfo[0]) > 10)
				menu_additem(menu, formatm("\d%s...\w |\y %s", szName, szInfo[1]));
				else menu_additem(menu, formatm("\d%s\w |\y %s", szInfo[0], szInfo[1]));
					
				
				userWarningMenu[id][count++] = SQL_ReadResult(query, 0);
				
				SQL_NextRow(query);
				
			
			}
			
			menu_setprop(menu, MPROP_EXITNAME, "Wyjscie");
			menu_setprop(menu, MPROP_BACKNAME, "Poprzednie");
			menu_setprop(menu, MPROP_NEXTNAME, "Nastepne");
			
			if(!count) {
				menu_destroy(menu);
				warningInfo(id);
				chatPrint(id, PREFIX_LINE, "Gracz nie posiada rzadnych ostrzezen!");
			} else menu_display(id, menu, 0 );
			
			
				
		}
		case 11:{
			
				
			while (SQL_MoreResults(query)){
				
				new class = SQL_ReadResult(query, 1);
					
				for(new type = 0; type < UM_ZM_TOTAL; type++){
					userZombie[id][class][type] = SQL_ReadResult(query, type + 2);
				}
				SQL_NextRow(query);
					
			}			
		}
		case 12:{
			
			new Float:XP;	
			while (SQL_MoreResults(query)){
				
				new class = SQL_ReadResult(query, 1);
					
			
				userHumanLevel[id][class] = SQL_ReadResult(query, 2);
				SQL_ReadResult(query, 3, XP);
				userExpClass[id][class] = XP;
				
				SQL_NextRow(query);
					
			}	
		}
		case 13:{
			
			new level, Float:exp, reset, szName[33];		
			new gText[2048], iLen=0, top=0;
			
			new sizeText = sizeof(gText)-iLen-1;
			
			iLen += format(gText[iLen], sizeText, "<head><link href=^"https://fonts.googleapis.com/css?family=Montserrat:100,200,300,400,500,600,700^" rel=^"stylesheet^"></head>");
			iLen += format(gText[iLen], sizeText, "\
							<style>\
									*{ font-size: 16px; font-family: Montserrat; color: white; text-align: center; padding: 0; margin: 0;}\
									body{border: 1px solid %s; background: #111}\
									b{color:%s; text-shadow: 0 0 5px %s;}\
							</style>",accentMotdColor,accentMotdColor,accentMotdColor);		
											
			iLen += format(gText[iLen], sizeText, "<p>TOP 10 najwiekszych poziomow Postaci!</p><hr size=1 color=%s>",accentMotdColor);
			iLen += format(gText[iLen], sizeText, "<table style=^"margin-top: 20px;margin-left: auto;margin-right: auto;width:710%^">");	
			iLen += format(gText[iLen], sizeText, "<tr><td><b>#</b></td><td><b>Nazwa</b></td><td><b>Poziom</b></td><td><b>XP</b></td></tr></br>");
					
					
			while(SQL_MoreResults(query)){	
				top ++;
									
				SQL_ReadResult(query, 0, szName, sizeof(szName));
				level = SQL_ReadResult(query, 1);
				SQL_ReadResult(query, 2, exp);
				reset = SQL_ReadResult(query, 3);
				
				replace_all(szName, sizeof(szName), "<", "");
				replace_all(szName, sizeof(szName), ">", "");
				replace_all(szName, sizeof(szName), "%", "%%");	
							
				iLen += format(gText[iLen], sizeText, "<tr><td>%d</td><td>%s</td><td>%d</td><td>%0.2f %%</td></tr>",top, szName,level, (exp*100.0/float(level * 45 + 45 * reset)));
						
				SQL_NextRow(query);	
					
			}
			iLen += format(gText[iLen], sizeText, "</table>");
			show_motd(id, gText, "Top 10 Postaci");
		}
		case 14:{
			
			new level, Float:exp, idclass, szName[33];		
			new gText[2048], iLen=0, top=0;
			
			new sizeText = sizeof(gText)-iLen-1;
			
			iLen += format(gText[iLen], sizeText, "<head><link href=^"https://fonts.googleapis.com/css?family=Montserrat:100,200,300,400,500,600,700^" rel=^"stylesheet^"></head>");
			iLen += format(gText[iLen], sizeText, "\
							<style>\
									*{ font-size: 16px; font-family: Montserrat; color: white; text-align: center; padding: 0; margin: 0;}\
									body{border: 1px solid %s; background: #111}\
									b{color:%s; text-shadow: 0 0 5px %s;}\
							</style>",accentMotdColor,accentMotdColor,accentMotdColor);		
											
			iLen += format(gText[iLen], sizeText, "<p>TOP 10 najwiekszych poziomow Klas!</p><hr size=1 color=%s>",accentMotdColor);
			iLen += format(gText[iLen], sizeText, "<table style=^"margin-top: 20px;margin-left: auto;margin-right: auto;width:710%^">");	
			iLen += format(gText[iLen], sizeText, "<tr><td><b>#</b></td><td><b>Nazwa</b></td><td><b>Klasa</b></td><td><b>Poziom</b></td><td><b>XP</b></td></tr></br>");
					
					
			while(SQL_MoreResults(query)){	
				top ++;
									
				SQL_ReadResult(query, SQL_FieldNameToNum(query, "gracz"), szName, sizeof(szName));
				SQL_ReadResult(query, SQL_FieldNameToNum(query, "xp"), exp);
				idclass	= 	SQL_ReadResult(query, SQL_FieldNameToNum(query, "idclass"));
				level 	= 	SQL_ReadResult(query, SQL_FieldNameToNum(query, "level"));
				
				replace_all(szName, sizeof(szName), "<", "");
				replace_all(szName, sizeof(szName), ">", "");
				replace_all(szName, sizeof(szName), "%", "%%");	
							
			
				iLen += format(gText[iLen], sizeText, "<tr><td>%d</td><td>%s</td><td>%s</td><td>%d</td><td>%0.2f %%</td></tr>",top, szName,classesHuman[idclass][0],level, (exp*100.0/float(level * 45)));
						
				SQL_NextRow(query);	
					
			}
			iLen += format(gText[iLen], sizeText, "</table>");
			show_motd(id, gText, "Top 10 Klas");
		}
		case 15:{
			
			new maxdmg, szName[33];		
			new gText[2048], iLen=0, top=0;
			
			new sizeText = sizeof(gText)-iLen-1;
			
			iLen += format(gText[iLen], sizeText, "<head><link href=^"https://fonts.googleapis.com/css?family=Montserrat:100,200,300,400,500,600,700^" rel=^"stylesheet^"></head>");
			iLen += format(gText[iLen], sizeText, "\
							<style>\
									*{ font-size: 16px; font-family: Montserrat; color: white; text-align: center; padding: 0; margin: 0;}\
									body{border: 1px solid %s; background: #111}\
									b{color:%s; text-shadow: 0 0 5px %s;}\
							</style>",accentMotdColor,accentMotdColor,accentMotdColor);		
											
			iLen += format(gText[iLen], sizeText, "<p>TOP 10 najwiekszych Obrazen!</p><hr size=1 color=%s>",accentMotdColor);
			iLen += format(gText[iLen], sizeText, "<table style=^"margin-top: 20px;margin-left: auto;margin-right: auto;width:710%^">");
			iLen += format(gText[iLen], sizeText, "<tr><td><b>#</b></td><td><b>Nazwa</b></td><td><b>Obrazenia</b></td></tr></br>");
					
					
			while(SQL_MoreResults(query)){	
				top ++;
									
				SQL_ReadResult(query, 0, szName, sizeof(szName));
				maxdmg = SQL_ReadResult(query, 1);
				
				replace_all(szName, sizeof(szName), "<", "");
				replace_all(szName, sizeof(szName), ">", "");
				replace_all(szName, sizeof(szName), "%", "%%");	
							
				iLen += format(gText[iLen], sizeText, "<tr><td>%d</td><td>%s</td><td>%d</td></tr>",top, szName,maxdmg);
						
				SQL_NextRow(query);	
					
			}
			iLen += format(gText[iLen], sizeText, "</table>");
			show_motd(id, gText, "Top 10 Obrazen");
		}
		case 16:{

			new rank = SQL_ReadResult(query, SQL_FieldNameToNum(query, "rank")) + 1;
			new count = SQL_ReadResult(query, SQL_FieldNameToNum(query, "count"));
			
			
			new menu = menu_create(formatm("\r[BaseBuilder]\y Topka Serwerowa"), "topMenu_2");
	
			menu_additem(menu, formatm("Twoj Rank\d [\r %d\y /\r %d\d ]", rank, count));
			menu_additem(menu, "Top 10 Punktow");
			menu_additem(menu, "Top 10 Czasu");
			menu_additem(menu, "Top 10 Postaci");
			menu_additem(menu, "Top 10 Klas");
			menu_additem(menu, "Top 10 Obrazen");
			menu_additem(menu, "Top 10 Klanow");
			menu_additem(menu, "Top 10 Zabojst Bronia");
		
			menu_display(id, menu, 0);
		
			menu_setprop(menu, MPROP_EXITNAME, "Wyjscie");
			menu_setprop(menu, MPROP_BACKNAME, "Poprzednie");
			menu_setprop(menu, MPROP_NEXTNAME, "Nastepne");
			
		}
		case 17:{
			new Float:DAMAGE;
			while (SQL_MoreResults(query)){
				
				new weapon = SQL_ReadResult(query, 1);
				
				userWeaponKill[id][weapon] = SQL_ReadResult(query, 3);
				userWeaponHs[id][weapon] = SQL_ReadResult(query, 4);
				userWeaponLevel[id][weapon] = SQL_ReadResult(query, 5);
				SQL_ReadResult(query, 6, DAMAGE);
				userWeaponDamage[id][weapon] = DAMAGE;
					
				
				SQL_NextRow(query);
					
			}
			
			
		}
		case 18:{
			
			new szWeapon[33], szName[33], kills, hs;		
			new gText[2048], iLen=0, top=0;
			
			new sizeText = sizeof(gText)-iLen-1;
			
			iLen += format(gText[iLen], sizeText, "<head><link href=^"https://fonts.googleapis.com/css?family=Montserrat:100,200,300,400,500,600,700^" rel=^"stylesheet^"></head>");
			iLen += format(gText[iLen], sizeText, "\
							<style>\
									*{ font-size: 16px; font-family: Montserrat; color: white; text-align: center; padding: 0; margin: 0;}\
									body{border: 1px solid %s; background: #111}\
									b{color:%s; text-shadow: 0 0 5px %s;}\
							</style>",accentMotdColor,accentMotdColor,accentMotdColor);		
											
			iLen += format(gText[iLen], sizeText, "<p>TOP 10 Zabojst Bronia!</p><hr size=1 color=%s>",accentMotdColor);
			iLen += format(gText[iLen], sizeText, "<table style=^"margin-top: 20px;margin-left: auto;margin-right: auto;width:710%^">");
			iLen += format(gText[iLen], sizeText, "<tr><td><b>#</b></td><td><b>Nazwa</b></td><td><b>Bron</b></td><td><b>Zabojstwa</b></td><td><b>Hs</b></td></tr></br>");
					
					
			while(SQL_MoreResults(query)){	
				top ++;
									
				SQL_ReadResult(query, SQL_FieldNameToNum(query, "gracz"), szName, sizeof(szName));
				SQL_ReadResult(query, SQL_FieldNameToNum(query, "nameweapon"), szWeapon, sizeof(szWeapon));
				kills	= 	SQL_ReadResult(query, SQL_FieldNameToNum(query, "kills"));
				hs	= 	SQL_ReadResult(query, SQL_FieldNameToNum(query, "hs"));
				
				
				new Float:hsToKills;
				hsToKills=float(hs)/float(kills);	
				
				replace_all(szName, sizeof(szName), "<", "");
				replace_all(szName, sizeof(szName), ">", "");
				replace_all(szName, sizeof(szName), "%", "%%");	
							
				iLen += format(gText[iLen], sizeText, "<tr><td>%d</td><td>%s</td><td>%s</td><td>%d</td><td>%0.1f%%</td></tr>",top, szName,szWeapon, kills, (hsToKills * 100.0));
						
				SQL_NextRow(query);	
					
			}
			iLen += format(gText[iLen], sizeText, "</table>");
			show_motd(id, gText, "TOP 10 Zabojst Bronia");
		}
		default:{ }
	}
	return PLUGIN_HANDLED;
}
public topMenu_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return;
	}
	switch(item){
		case 0: loadStatsSql(id, 2);
		case 1: loadStatsSql(id, 1);
		case 2: loadStatsSql(id, 3);
		case 3: loadStatsSql(id, 13);
		case 4: loadStatsSql(id, 14);
		case 5: loadStatsSql(id, 15);
		case 6: loadStatsSql(id,5);
		case 7: loadStatsSql(id,18);
	}
	loadStatsSql(id, 16);
}
public saveStatsSql(id, typeSave){
	
	if(is_user_hltv(id)) return PLUGIN_CONTINUE;
	if(!sqlConnected) return PLUGIN_CONTINUE;
	if(!userLoaded[id]) return PLUGIN_CONTINUE;
	
	new queryData[256], tempId[2];
	
	tempId[0] = id;
	
	switch(typeSave){
		case 0:{
			format(queryData, sizeof(queryData), "\
			UPDATE `players`\
				SET `kills`='%d', `deaths`='%d', `points`='%d', `maxDamage`='%d'\
				WHERE `name` ='%s'", userKills[id], userDeaths[id], userPoints[id], userMaxDmg[id], userName[id]);
		}
		case 1:{
			format(queryData, sizeof(queryData), "\
			UPDATE `players`\
				SET `pass`='%s', `ipLogin`='%s', `sidLogin`='%s'\
				WHERE `name` ='%s'", strlen(userPassword[id])>1?userPassword[id]:"_", strlen(userIp[id])>1?userIp[id]:"_", strlen(userSid[id])>1?userSid[id]:"_",  userName[id]);
		}
		case 2:{
			format(queryData, sizeof(queryData), "\
			UPDATE `players`\
				SET `timePlay`='%d', `lastlogin`= NOW()\
				WHERE `name` ='%s'", (floatround(userTime[id] +=  get_gametime()-userTimeConnectServer[id])), userName[id]);
		}
		case 3:{
			format(queryData, sizeof(queryData), "\
			UPDATE `players`\
				SET `level`='%d', `xp`='%0.2f'\
				WHERE `name` ='%s'", userLevel[id],userExp[id], userName[id]);
		}
		case 4:{
			format(queryData, sizeof(queryData), "\
			UPDATE `players`\
				SET `nuggets`='%d', `bones`='%d'\
				WHERE `name` ='%s'", userNugget[id], userBone[id], userName[id]);
		}
		
		
		default:{}
	}
	SQL_ThreadQuery(sql, "saveStatsHandlerSql", queryData, tempId, sizeof(tempId));
	return PLUGIN_CONTINUE;

}
public saveInsert(id, typeSave, typeSlot){
	
	if(is_user_hltv(id)) return PLUGIN_CONTINUE;
	if(!sqlConnected) return PLUGIN_CONTINUE;
	if(!userLoaded[id]) return PLUGIN_CONTINUE;
	
	new queryData[256], tempId[3];
	
	tempId[0] = id;
	tempId[1] = typeSave;
	tempId[2] = typeSlot;
	
	switch(typeSave){
		
		case 0:{
			for(new i = 0; i < human_TOTAL; i ++){
				if(!hasClassHuman(id, i)) continue;
				
				format(queryData, sizeof(queryData), "\
				INSERT INTO `classhumans` (`idplayer`, `idclass`, `level`, `xp`)\
					VALUES ('%d','%d','%d','%0.2f')\
					ON DUPLICATE KEY UPDATE `level`='%d', `xp`='%0.2f'", userSqlId[id], i, userHumanLevel[id][i], userExpClass[id][i], userHumanLevel[id][i], userExpClass[id][i]);
				
				SQL_ThreadQuery(sql, "saveStatsHandlerSql", queryData, tempId, sizeof(tempId));
			}
		}
		case 1:{
			format(queryData, sizeof(queryData), "\
			INSERT INTO `classzombies` (`idplayer`, `idclass`, `heatlh`, `speed`, `reduction`)\
				VALUES ('%d','%d','%d','%d','%d')\
				ON DUPLICATE KEY UPDATE `heatlh`='%d', `speed`='%d', `reduction`='%d'", userSqlId[id], userVarMenu[id], userZombie[id][userVarMenu[id]][UP_ZM_HEALTH], userZombie[id][userVarMenu[id]][UP_ZM_SPEED], userZombie[id][userVarMenu[id]][UP_ZM_REDUCTION], userZombie[id][userVarMenu[id]][UP_ZM_HEALTH], userZombie[id][userVarMenu[id]][UP_ZM_SPEED], userZombie[id][userVarMenu[id]][UP_ZM_REDUCTION]);
			
			SQL_ThreadQuery(sql, "saveStatsHandlerSql", queryData, tempId, sizeof(tempId));
		}
		case 2:{
			
			if(!userWeaponKill[id][typeSlot] && !userWeaponDamage[id][typeSlot]) return PLUGIN_CONTINUE;
			
			format(queryData, sizeof(queryData), "\
			INSERT INTO `weapons` (`idplayer`, `idweapon`, `nameweapon`, `kills`,`hs`, `level`, `damage`)\
				VALUES ('%d','%d','%s', '%d','%d','%d','%0.2f')\
				ON DUPLICATE KEY UPDATE `kills`='%d',`hs`='%d', `level`='%d', `damage`='%0.2f'", userSqlId[id], typeSlot, allGuns[typeSlot][1], userWeaponKill[id][typeSlot], userWeaponHs[id][typeSlot], userWeaponLevel[id][typeSlot], userWeaponDamage[id][typeSlot], userWeaponKill[id][typeSlot],userWeaponHs[id][typeSlot], userWeaponLevel[id][typeSlot], userWeaponDamage[id][typeSlot]);
			
			SQL_ThreadQuery(sql, "saveStatsHandlerSql", queryData, tempId, sizeof(tempId));
			
		}
		default:{ return PLUGIN_CONTINUE; }
	}
	return PLUGIN_CONTINUE;

}
public saveStatsHandlerSql(failState, Handle:query, error[], errorNum, data[], dataSize){
	if (failState){
		if (failState == TQUERY_CONNECT_FAILED) log_amx("[SQL-LOG] Nie mozna polaczyc siê z baz¹ danych SQL. Blad: %s (%d)", error, errorNum);
		else if (failState == TQUERY_QUERY_FAILED) log_amx("[SQL-LOG] Zapytanie watkowe nie powiodlo sie. Blad: %s (%d)", error, errorNum);
		return PLUGIN_HANDLED;
	}
	return PLUGIN_CONTINUE;
}
public save_clan(clan){
	static queryData[512], safeClanName[64],safeClanInfo[43], safeClanMotd[22], bbClan[clanInfo];
	
	ArrayGetArray(bbClans, clan, bbClan);

	replace_all_string(bbClan[CLAN_NAME], safeClanName, sizeof(safeClanName));
	replace_all_string(bbClan[CLAN_INFO], safeClanInfo, sizeof(safeClanInfo));
	replace_all_string(bbClan[CLAN_MOTD], safeClanMotd, sizeof(safeClanMotd));
	
	format(queryData, sizeof(queryData), "\
	UPDATE `clans`\
		SET name = '%s', info = '%s', level = '%d', points = '%d', nugget = '%d', kills = '%d', members = '%d', health = '%d', damagePlayer = '%d', damageClass = '%d', critic = '%d', expDrop = '%d', nuggetDrop = '%d', cooldown = '%d', idmotd='%s', upgradetime='%d' \
		WHERE name = '%s'", safeClanName,safeClanInfo, bbClan[CLAN_LEVEL], bbClan[CLAN_POINTS], bbClan[CLAN_NUGGET], bbClan[CLAN_KILLS], bbClan[CLAN_MEMBERS], bbClan[CLAN_HEALTH], bbClan[CLAN_DAMAGEPLAYER], bbClan[CLAN_DAMAGECLASS], bbClan[CLAN_CRITIC], bbClan[CLAN_EXPDROP],bbClan[CLAN_NUGGETDROP],bbClan[CLAN_COOLDOWN],safeClanMotd,bbClan[CLAN_UPGRADETIME] ,  safeClanName);
	
	SQL_ThreadQuery(sql, "saveStatsHandlerSql", queryData);
}

public mysqlSave(id){
	
	if(is_user_hltv(id)) return PLUGIN_CONTINUE;
	if(!sqlConnected) return PLUGIN_CONTINUE;
	if(!userLoaded[id]) return PLUGIN_CONTINUE;
	
	for(new i = 0; i < 5; i++) saveStatsSql(id, i);
	
	saveInsert(id, 0, 0);
	saveInsert(id, 2, userWeaponSelect[id]);

	userLoaded[id] = false;
	userLogged[id] = false;
	
	return PLUGIN_CONTINUE;
	
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
