#include <amxmodx>
#include <amxmisc>
#include <sqlx>

enum _:eTypeData { DATA_CLEAR_TYPE_ALL, DATA_CLEAR_TYPE_SQL, DATA_CLEAR_TYPE_FVAULT };
enum _:eClearData { bool:DATA_ON, DATA_TIME, DATA_TYPE};

new clearData[eClearData]; 
new userClearData[33];


public clearDataMenu(id){
	
	if(!isSuperAdmin(id)) return PLUGIN_CONTINUE;	
	
	new menu = menu_create("Restart Serwera", "clearDataMenu_2");
	
	menu_additem(menu, formatm("Typ resetu: %s", userClearData[id] == DATA_CLEAR_TYPE_ALL ? "\rSQL + FVAULT" : userClearData[id] == DATA_CLEAR_TYPE_SQL ? "\ySQL" : "\dFVAULT" ));
	menu_additem(menu, "Wykonaj reset!");
	
	menu_display(id, menu, 0);
	
	return PLUGIN_HANDLED;
}

public clearDataMenu_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return;
	}
	
	switch(item){
		case 0:{
			
			userClearData[id] = (userClearData[id]+1) % eTypeData;
			clearDataMenu(id);
			
		}
		case 1:{
			if(!clearData[DATA_ON]) resetAllDatabase(id);
			else chatPrint(id, PREFIX_LINE, "Reset zostal juz odpalony! Poczekaj!");
		}
	}
}

public backUP_fVault(){
	
	new szData[11];		get_time("%Y-%m-%d",szData,sizeof(szData) - 1);
	new folder[128];
	new firstFolder[128];
	
	format(folder, sizeof(folder) - 1, "%s/%s/%s-%s", _vault_dir, fVAULTFILE_BACKUP, szData, fVAULTFILE);
	format(firstFolder, sizeof(firstFolder) - 1, "%s/%s", _vault_dir, fVAULTFILE_BACKUP);
	
	if( !dir_exists(firstFolder)) mkdir(firstFolder);
	
	if( !dir_exists(folder)) mkdir(folder);
		
	
	new szCurrentTime[3];
	get_time("%H",szCurrentTime,sizeof(szCurrentTime));
	
	new newName[128];	
	format(newName, sizeof(newName) - 1, "%s/~%s %s.txt", folder, szCurrentTime, fVAULTFILE);	
	
	if(!file_exists(newName)) {
		
		new filename[128];
		_FormatVaultName(fVAULTFILE, filename, sizeof(filename) - 1);
		
		log_amx("=== Wykonano backup fvaulta: o godzinie: ~%s (%d) ===", szCurrentTime, fvault_size(fVAULTFILE));
		fcopy(filename, newName, 0);
	} else log_amx("=== Plik backup'owy juz jest stworzony! ===");
	
}

stock fcopy(const originalFile[], const copyFile[], deleteOriginal=0){ 

	static buffer[1024] ;
	static readsize ;
	
	new fileRead 	= fopen(originalFile, "rb");
	new fileWrite 	= fopen(copyFile, "wb");
	
	if (!fileRead) return;

	fseek(fileRead, 0, SEEK_END); 
	new size = ftell(fileRead); 
	fseek(fileRead, 0, SEEK_SET); 

	for (new j = 0; j < size; j += 1024) { 
		readsize = fread_blocks(fileRead, buffer, 1024, BLOCK_CHAR); 
		fwrite_blocks(fileWrite, buffer, readsize, BLOCK_CHAR); 
	} 
	
	fclose(fileRead) ;
	fclose(fileWrite) ;
	
	if(deleteOriginal) delete_file(originalFile);

}


public resetAllDatabase(id){
	
	if(!isSuperAdmin(id)) return PLUGIN_CONTINUE;
	if(clearData[DATA_ON]) return PLUGIN_CONTINUE;
	
	clearData[DATA_ON] = true;
	clearData[DATA_TIME] = 10;
	clearData[DATA_TYPE] = userClearData[id];
	
	chatPrint(0, PREFIX_LINE, "Zostal przeprowadzony restart^4 %s^1 przez^3 %s!", clearData[DATA_TYPE] == DATA_CLEAR_TYPE_ALL ? "SQL + FVAULT" : clearData[DATA_TYPE] == DATA_CLEAR_TYPE_SQL ? "SQL" : "FVAULT", userName[id]);
	
	clearDatabaseTimer();
	
	return PLUGIN_HANDLED;
}

public clearDatabaseTimer(){
	
	chatPrint(0, PREFIX_LINE, "Zmiana mapy nastapi za^3 %d^1 sekund", clearData[DATA_TIME]);
	
	clearData[DATA_TIME] --;
	
	if(clearData[DATA_TIME] > 0) set_task(1.0, "clearDatabaseTimer");
	else {
		
		if(clearData[DATA_TYPE] == DATA_CLEAR_TYPE_ALL){
			clearDataFVAULT();
			clearDatabase();
		} else if(clearData[DATA_TYPE] == DATA_CLEAR_TYPE_SQL) clearDatabase();
		else clearDataFVAULT();
		
		set_task(2.0, "restartMap");
		
	}
}

public clearDataFVAULT(){
	for (new i = 1; i < 33; i++){
		if(!is_user_connected(i)) continue;
		userLoadVault[i] = false;
	}
	
	
	fvault_clear(fVAULTFILE);
	
}

public clearDatabase(){
	for (new i = 1; i < 33; i++){
		if(!is_user_connected(i)) continue;
		userLoaded[i] = false;
	}
	
	sqlConnected = false;
	
	new tempData[512];
	
	format(tempData, sizeof(tempData) - 1, "DROP TABLE `players`, `clans`, `classhumans`, `classzombies`, `warnings`, `weapons`;");
	
	SQL_ThreadQuery(sql, "saveStatsHandlerSql", tempData);
	
}

public restartMap(){
	
	new currentMap[64];
	get_mapname(currentMap, sizeof(currentMap) - 1);
	server_cmd("map ^"%s^"", currentMap);
	
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
