#include <amxmodx>
#include <amxmisc>


// sqlFile.inl
// adminOptions.inl 

// - FUNCTION

public muteTrie(){
	for (new i = 1; i < 33; i++) userMutes[i] = TrieCreate();
}


public mutePlayerMenu(id){
	new menu = menu_create("Menu mutowania:", "mutePlayerMenu_2");
	
	menu_additem(menu, "\wZmutuj \yGracza");
	menu_additem(menu, "\wOdmutuj \yGracza");

	menu_setprop(menu, MPROP_EXITNAME, "Wyjscie");

	menu_display(id, menu, 0);
	
}
public mutePlayerMenu_2(id, menu, item){
	if(item != MENU_EXIT){
		
		userMenuPlayer[id] = !item ? MENU_PLAYER_MUTE : MENU_PLAYER_UNMUTE;
		
		choosePlayer(id, 0);
		
			
	} else menu_destroy(menu);
}

public muteMenuType(id){

	new target = userService[id];

	new menu = menu_create(fmt("\yWybierz typ mute dla gracza\r %s", userName[target]), "muteMenuType_2");

	menu_additem(menu, "Na\y Mape");
	menu_additem(menu, "Na\r Zawsze");
		
	menu_setprop(menu, MPROP_EXITNAME, "Wyjscie");

	menu_display(id, menu, 0);
			
}
public muteMenuType_2(id, menu, item){
	
	
	if(item != MENU_EXIT){
		
		new target = userService[id];
			
		if (!is_user_connected(target)) {
			chatPrint(id, PREFIX_LINE, "Wybranego gracza nie ma juz na serwerze!");
			return;
		}
		
		switch(item){
			case 0:{
				TrieSetCell(userMutes[id], userName[target], 0);

				chatPrint(id, PREFIX_LINE, "Zmutowales^4 na mape^1 gracza^3 %s^1.", userName[target]);
			}
			case 1:{
				new queryData[128], tempId[2];
				tempId[0] = id;
				tempId[1] = target;
				
				format(queryData, sizeof(queryData) - 1, "INSERT INTO `mutedplayers` (`nameplayer`, `mutedplayer`) VALUES (^"%s^", ^"%s^");", userName[id], userName[target]);
	
				SQL_ThreadQuery(sql, "saveStatsHandlerSql", queryData, tempId, sizeof(tempId));
				
				TrieSetCell(userMutes[id], userName[target], 1);
	
				chatPrint(id, PREFIX_LINE,  "Zmutowales^4 na zawsze^1 gracza^3 %s^1.", userName[target]);
			}
		}
			
	} else menu_destroy(menu);
	
}

public fw_SetClientListening(iReceiver, iSender, bool:bListen){
	
	if (iReceiver == iSender || !is_user_connected(iSender)) return FMRES_IGNORED;
	
	if(TrieKeyExists(userMutes[iReceiver], userName[iSender])){
		engfunc(EngFunc_SetClientListening, iReceiver, iSender, false);
		forward_return(FMV_CELL, false);
		return FMRES_SUPERCEDE;
	}
	
	if (get_systime() < userMute[iSender]){
		engfunc(EngFunc_SetClientListening, iReceiver, iSender, false);
		forward_return(FMV_CELL, false);
		return FMRES_SUPERCEDE;
	}
	
	if(OX[OX_MICRO] && (!isSuperAdmin(iSender))){
		engfunc(EngFunc_SetClientListening, iReceiver, iSender, false);
		forward_return(FMV_CELL, false);
		return FMRES_SUPERCEDE;
	}
	
	engfunc(EngFunc_SetClientListening, iReceiver, iSender, true);
	forward_return(FMV_CELL, true);
	return FMRES_SUPERCEDE;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
