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
	
	for(new i = 0; i < sizeof(foldersLogs); i++){
		
		format(logFolder, sizeof(logFolder) - 1, "%s/%s", firstFolder, foldersLogs[i]);
		
		if(dir_exists(logFolder)) continue;
		
		mkdir(logFolder);
		
		log_amx("=== Stworzono folder logow: %s ===", foldersLogs[i]);
	}
	
	
}

public logBB(id, type, const secondFile[], szText[], any:...){
	
	new logMessage[512];
	
	if (numargs() == 4) copy(logMessage, sizeof(logMessage) - 1, szText);
	else vformat(logMessage, sizeof(logMessage) - 1, szText, 5);
	
	new bool:error = false;
	if(type > enumLogs  || type <= LOG_ERROR) error = true;
	
	new szCurrentTime[9], szData[12];
	new szDir[128], firstFolder[64];
	
	get_time("%H:%M:%S",szCurrentTime,sizeof(szCurrentTime) - 1);
	get_time("%Y-%m-%d",szData,sizeof(szData) - 1);
	get_basedir(szDir,sizeof(szDir) - 1);
	
	format(firstFolder, sizeof(firstFolder) - 1, "%s/%s", szDir, folderLogs);
	format(szDir,sizeof(szDir) - 1,"%s/%s/%s_%s.html",firstFolder, foldersLogs[error ? LOG_ERROR : type], secondFile, szData);
	
	if(!file_exists(szDir)){
		
		new iLen = 0, gStyle[2048]; 
		iLen = format(gStyle[iLen], sizeof(gStyle)-iLen-1, "<head><link href=^"https://fonts.googleapis.com/css?family=Montserrat:100,200,300,400,500,600,700^" rel=^"stylesheet^"></head>");
				
		iLen += format(gStyle[iLen], sizeof(gStyle)-iLen-1, "<style type=^"text/css^">\
									*{ font-size: 15px; font-family: Montserrat; color: white; text-align: center; margin: 0;}\
									body{border: 1px solid %s; box-shadow: 0 0 5px %s inset; background: #111}\
									b{color:%s; text-shadow: 0 0 5px %s;}\
									td{color: #808080;}\
									table{border-collapse: collapse; margin-top: 20px;margin-left: auto;margin-right: auto;width:750%}\
									tr.tdh{border-bottom: 1px solid #292929;}\
									.tdh:hover{background: #0e0e0e; padding: 6px; color: #fff;}\
								</style>",accentMotdColor,accentMotdColor,accentMotdColor, accentMotdColor);
								
		iLen += format(gStyle[iLen], sizeof(gStyle)-iLen-1, "<p>Logi '%s' z dnia<b> %s</b></p><hr size=1 color=%s>",secondFile, szData, accentMotdColor);
		iLen += format(gStyle[iLen], sizeof(gStyle)-iLen-1, "<table>");	
		
		if(!error) iLen += format(gStyle[iLen], sizeof(gStyle)-iLen-1, "<tr><td><b>Godzina</b></td><td><b>Nazwa</b></td><td><b>IP</b></td><td><b>Tresc</b></td></tr>");
		else iLen += format(gStyle[iLen], sizeof(gStyle)-iLen-1, "<tr><td><b>Godzina</b></td><td><b>Plik</b></td><td><b>Typ</b></td></tr>"); 
		
		write_file(szDir, gStyle, 0);

	}
	
	new szMessage[1024];
	if(error){
		format(szMessage,sizeof(szMessage) - 1,"<tr class=^"tdh^">\
								<td>%s</td>\
								<td>%s</td>\
								<td>%d</td>\
							</tr>",szCurrentTime,secondFile, type);
	} else {
		
		new szIp[MAXBITIP];
		get_user_ip(id, szIp, sizeof(szIp)-1, 1);

		format(szMessage,sizeof(szMessage) - 1,"<tr class=^"tdh^">\
								<td>%s</td>\
								<td>%s</td>\
								<td>%s</td>\
								<td>%s</td>\
							</tr>",szCurrentTime, userName[id], szIp, logMessage);
	}
	
	write_file(szDir, szMessage);
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
