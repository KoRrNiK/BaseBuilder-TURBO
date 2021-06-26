#include <amxmodx>
#include <amxmisc>

public logCreate(){
	new szDir[128];
	get_basedir(szDir,sizeof(szDir));
	
	new firstFolder[64];
	format(firstFolder, sizeof(firstFolder) - 1, "%s/%s", szDir, folderLogs);
	
	if(!dir_exists(firstFolder)){
		log_amx("=== Stworzono glowny folder logow: %s ===", folderLogs);
		mkdir(firstFolder);
	}

	new logFolder[90];
	new foldersLogs[][] = {
		  "shopSmsLog"
		,"transferLog"
		,"awardLog"
		,"accountLog"
		,"missionLog"
		,"classLog"
		,"muteLog"
		,"afkLog"
		,"caveLog"
		,"conLog"
		,"chatLog"
		,"costume"
		,"warnings"
	};
	
	for(new i = 0; i < sizeof(foldersLogs); i++){
		
		format(logFolder, sizeof(logFolder) - 1, "%s/%s", firstFolder, foldersLogs[i]);
		
		if(dir_exists(logFolder)) continue;
		
		mkdir(logFolder);
		
		log_amx("=== Stworzono folder logow: %s ===", foldersLogs[i]);
	}
	
	
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
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
