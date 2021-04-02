#include <amxmodx>
#include <amxmisc>
#include <cstrike>
#include <fun>
#include <hamsandwich>
#include <fakemeta>
#include <fakemeta_util>
#include <engine>

new const costReset = 15000;
new const costResetMulti = 5000;

public menuWeapon(id){	
	
	if(!playerLogged(id)) return PLUGIN_CONTINUE;
	
	
	new gText[128], iLen = 0;
	
	iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "\r[BaseBuilder]\y Dostepne Bronie!^n^n");
	iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "\y%s^t^t\dLevel:\r %d\d /\r %d\d |^n", symbolsCustom[SYMBOL_DR_ARROW], userLevel[id], MAXLVL);
	iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "\y%s^t^t\dXp:\r %0.2f%%^t\d|\w", symbolsCustom[SYMBOL_DR_ARROW],  (userExp[id]*100.0/needXp(id, userLevel[id])));
	
	
	new menu = menu_create(gText, "menuWeapon_2");
	
	for( new i = 0; i < sizeof(allGuns); i++ ){
		if (str_to_num(allGuns[i][3]) > playedTime(id) || str_to_num(allGuns[i][2]) > userLevel[id])	
	
			format(gText, sizeof(gText), "\d%s\d [Niedostepna] - %s", allGuns[i][1],  allGuns[i][2]);

		else if (str_to_num(allGuns[i][2]) <= userLevel[id] ) {	
			if(userWeaponDamage[id][i])
				format(gText, sizeof(gText), "\w%s\d [\rDMG:\y +%0.2f%%\d |\r LV:\y%d\d ]",  allGuns[i][1], userWeaponDamage[id][i], userWeaponLevel[id][i]);
			else format(gText, sizeof(gText), "\w%s\d [...]",  allGuns[i][1]);
		}
		menu_additem(menu, gText);
	
	}
	menu_additem(menu, "\yResetowanie Poziomu!");

	menu_setprop(menu,MPROP_EXITNAME,"Wyjdz");
	menu_setprop(menu,MPROP_BACKNAME,"Wroc");
	menu_setprop(menu,MPROP_NEXTNAME,"Dalej");
	
	menu_display(id, menu, 0);
	return PLUGIN_CONTINUE;
}
public menuWeapon_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return PLUGIN_CONTINUE;
	}

	if(item == 24){
		resetMenu(id);
		return PLUGIN_CONTINUE;
	}
	
	userVarMenu[id] = item;
	
	selectWeapon(id, item);
		
	return PLUGIN_CONTINUE;
}
public selectWeapon(id, item){
	
	new gun = userVarMenu[id];
	new gText[512], iLen;
	
	new gamePlayUser =  str_to_num(allGuns[gun][3]) - playedTime(id);
	new gamePlayUser2 = str_to_num(allGuns[gun][3]);
	new availableWeaponHour[128], availableWeaponLevel[128]; 
	
	format(availableWeaponHour, sizeof(availableWeaponHour),  "\w|\d Dostepna za:\r %dh %s%dm %s%ds", gamePlayUser/HOUR, (gamePlayUser/MINUTE)<10?"0":"",gamePlayUser/MINUTE%MINUTE, gamePlayUser%MINUTE<10?"0":"", gamePlayUser%MINUTE);
	format(availableWeaponLevel, sizeof(availableWeaponLevel),  "\w|\d Dostepna za:\r %d lvl!", str_to_num(allGuns[gun][2])-userLevel[id]);
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Informacje o Broni!^n^n");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dBron:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], allGuns[gun][1]);
	if(str_to_num(allGuns[gun][2]) != 0 && str_to_num(allGuns[gun][3]) != 0){
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dPoziom:\r %s %s^n", symbolsCustom[SYMBOL_DR_ARROW], allGuns[gun][2], (str_to_num(allGuns[gun][2]) > userLevel[id]) ? availableWeaponLevel : "\w| \yWymaganie spelnione!");
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dGodziny:\r %dh %s%dm %s%ds %s^n",symbolsCustom[SYMBOL_DR_ARROW], gamePlayUser2/HOUR, (gamePlayUser2/MINUTE)<10?"0":"",gamePlayUser2/MINUTE%MINUTE, gamePlayUser2%MINUTE<10?"0":"", gamePlayUser2%MINUTE, (str_to_num(allGuns[gun][3]) >  playedTime(id)) ? availableWeaponHour : "\w| \yWymaganie spelnione!");
	}
	if(str_to_num(allGuns[item][3]) > playedTime(id) ||  str_to_num(allGuns[item][2]) > userLevel[id]){
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\d1. Wybierz");
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\d2. Ulepsz");	
	
	} else {
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r1.\w Wybierz");
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r2.\w Ulepsz");
	}
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\r0.\y Wroc");
	showMenu(id, B1 | B2 | B0 , gText, -1, "selectWeapon"  );
}

public selectWeapon_2(id, item){
	
	new gun = userVarMenu[id];
	
	switch(item){
		
		case 0:{
			if (str_to_num(allGuns[gun][3]) > playedTime(id) ||  str_to_num(allGuns[gun][2]) > userLevel[id]){	
				chatPrint(id, PREFIX_NORMAL, "Najpierw ja oblokuj");
				return PLUGIN_CONTINUE;
			}
			if(buildTime  || roundEnd){
				chatPrint(id, PREFIX_NORMAL, "Za wczesnie na to!");
				return PLUGIN_CONTINUE;
			}
			
			if(userSelectWeapon[id] >= 3 ){
				chatPrint(id, PREFIX_NORMAL, "Wykozystales swoj limit na ta runde zmian Broni!");
				return PLUGIN_CONTINUE;
			}
			if(get_user_team(id) != 2){
				menuWeapon(id);
				chatPrint(id, PREFIX_NORMAL, "Musisz byc budowniczym!");
				return PLUGIN_CONTINUE;
			}
			if(userWeaponDamage[id][gun])
				chatPrint(id, PREFIX_NORMAL, "Wybrales^4 %s^3 (^1 DMG:^4 +%0.2f%s^3 |^1 LV:^4 %d^3 )", allGuns[gun][1], userWeaponDamage[id][gun], "%%", userWeaponLevel[id][gun]);
			else chatPrint(id, PREFIX_NORMAL, "Wybrales^4 %s^3 (^1 ...^3 )", allGuns[gun][1]);
			
			saveInsert(id, 2, userWeaponSelect[id]);
			
			userWeaponSelect[id] = gun;
			
			giveWeapons(id, gun);
		}
		case 1:{
			if (str_to_num(allGuns[gun][3]) > playedTime(id) ||  str_to_num(allGuns[gun][2]) > userLevel[id]){	
				selectWeapon(id, gun);
				chatPrint(id, PREFIX_NORMAL, "Najpierw ja oblokuj");
			} else {
				upgradeWeapon(id);
			}
		}
		case 2:{
			menuWeapon(id);
		}
	}
	return PLUGIN_CONTINUE;
}
enum { damage_MIN = 0, damage_MAX };
public upgradeWeapon(id){
	
	new gun = userVarMenu[id];
	new gText[512], iLen;
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Ulepszenia Broni!^n^n");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dBrylki:\r %s^n^n", symbolsCustom[SYMBOL_DR_ARROW],formatNumber(userNugget[id]));
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dBron:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], allGuns[gun][1]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dPoziom:\r %d/%d^n", symbolsCustom[SYMBOL_DR_ARROW], userWeaponLevel[id][gun], str_to_num(allGuns[gun][4]));
	
	if( userWeaponLevel[id][gun] <  str_to_num(allGuns[gun][4]))
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dZabojstwa:\r %d %s %d^n", symbolsCustom[SYMBOL_DR_ARROW], userWeaponKill[id][gun], symbolsCustom[SYMBOL_LINE],  needKills(id , gun, userWeaponLevel[id][gun])-userWeaponKill[id][gun]);
	else iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dZabojstwa:\r %d^n", symbolsCustom[SYMBOL_DR_ARROW], userWeaponKill[id][gun]);
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dObrazenia:\r %0.2f%%^n", symbolsCustom[SYMBOL_DR_ARROW], userWeaponDamage[id][gun]);

	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\w- Przelicznik Miksowania!^n^n");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dCena:\r %d Brylek^n", symbolsCustom[SYMBOL_DR_ARROW], calcCostWeapon(id, gun));
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dMix:\r %0.2f%% - %0.2f%%^n", symbolsCustom[SYMBOL_DR_ARROW], calcDamageWeapon(id, gun, damage_MIN), calcDamageWeapon(id, gun, damage_MAX));
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r1.\w Miksuj");	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r0.\y Wroc");	
	
	showMenu(id, B1 | B2 | B0 , gText, -1, "upgradeWeapon"  );
}

public upgradeWeapon_2(id, item){
	
	new gun = userVarMenu[id];
	
	switch(item){
		case 0:{
			
			if(calcCostWeapon(id, gun) > userNugget[id]){
				chatPrint(id, PREFIX_LINE, "Nie posiadasz odpowiedniej ilosci^3 Brylek");
				upgradeWeapon(id);
				return;
			}
			new Float:DAMAGE = random_float(calcDamageWeapon(id, gun, damage_MIN), random(5) == 0 ? calcDamageWeapon(id, gun, damage_MAX) : calcDamageWeapon(id, gun, damage_MAX) * 0.75);
			userNugget[id] -= calcCostWeapon(id, gun);
			userWeaponDamage[id][gun] = DAMAGE;
			chatPrint(id, PREFIX_LINE, "Zmiksowales:^3 %0.2f%s^1 obrazen na broni:^3 %s", DAMAGE, "%%",allGuns[gun][1]);
			saveInsert(id, 2, gun);
			upgradeWeapon(id);
			
		}
		default:{
			selectWeapon(id, gun);
		}
	}

}
public Float:calcDamageWeapon(id, weapon, type){
	new Float:value;
	switch(type){
		case damage_MIN:value = userWeaponLevel[id][weapon] == 0 ? (paramWeaponFloat[weapon][2] / 2) : (paramWeaponFloat[weapon][2] * ( userWeaponLevel[id][weapon] * 0.5 ));
		case damage_MAX: value = floatmin(userWeaponLevel[id][weapon] == 0 ? (paramWeaponFloat[weapon][2] * 2) : (paramWeaponFloat[weapon][2] * userWeaponLevel[id][weapon]) * 2, paramWeaponFloat[weapon][0] );
	}
	return value;
}
public calcCostWeapon(id, weapon){
	return floatround((paramWeaponFloat[weapon][1] * (userWeaponDamage[id][weapon]+1 * 0.25 )) * (userWeaponLevel[id][weapon] == 0 ? (userWeaponLevel[id][weapon]+1) : (userWeaponLevel[id][weapon])));
}
public giveWeapons(id, item){
	if (!gameTime){
		chatPrint(id, PREFIX_NORMAL, "Otrzymasz bron przy wyjsciu zombie!");
		userWeaponBool[id] = true;
		return PLUGIN_CONTINUE;
	}
	if (get_user_team(id) != 2){
		chatPrint(id, PREFIX_NORMAL, "To jest tylko dla^4 Budowniczych");
		return PLUGIN_CONTINUE;
	}
	
	addMission(id, mission_SOLD, 1);

	StripWeapons(id, Primary);
	StripWeapons(id, Secondary);
	
	giveGranade(id);
	give_item(id, allGuns[item][0]);
	giveAmmo(id);
	
	#if defined CHRISTMAS_ADDON
		addChristmasMission(id, CH_WEAPON1, 1);
		addChristmasMission(id, CH_WEAPON2, 1);		
	#endif
	
	userGotGrenades[id] = 1;
	userSelectWeapon[id] ++;
	
	return PLUGIN_CONTINUE;
}

public giveGranade(id){
	if (get_user_team(id) != 2){
		chatPrint(id, PREFIX_NORMAL, "To jest tylko dla^4 Budowniczych");
		return PLUGIN_CONTINUE;
	}
	
	new bool:smoke;
	new bool:grenade;
	new weapons[32];
	new num;
	get_user_weapons(id, weapons, num);
	for(new i = 0; i < num; i++){
		if (weapons[i] == CSW_SMOKEGRENADE) smoke = true;
		else if (weapons[i] == CSW_HEGRENADE) grenade = true;
	}
	StripWeapons(id, Primary);
	StripWeapons(id, Secondary);

	if (!userGotGrenades[id] || smoke) give_item(id, "weapon_smokegrenade");
	if (!userGotGrenades[id] || grenade) give_item(id, "weapon_hegrenade");

	userGotGrenades[id] = 1;	
	return PLUGIN_CONTINUE;
	
}


public bool:blockWeapon(weapon) return weapon && weapon != CSW_KNIFE && weapon != CSW_HEGRENADE && weapon != CSW_SMOKEGRENADE && weapon != CSW_FLASHBANG && weapon != CSW_C4;
public giveAmmo(id){
	if (!is_user_alive(id) || !is_user_connected(id)) return PLUGIN_CONTINUE;
		
	new weapons[32];
	new num;
	get_user_weapons(id, weapons, num);
	
	for(new i = 0; i < num; i++){
		if (blockWeapon(weapons[i])){
			cs_set_user_bpammo(id, weapons[i], 200);
		}
	}
	return PLUGIN_CONTINUE;
}
public MSG_HideWeapon(MsgDEST,MsgID,id){
	if(!(get_msg_arg_int(1) & HUD_HIDE_FLASH ))
		set_msg_arg_int(1,ARG_BYTE,get_msg_arg_int(1) | HUD_HIDE_FLASH );
	
	if(!(get_msg_arg_int(1) & HUD_HIDE_TIMER ))
		set_msg_arg_int(1,ARG_BYTE,get_msg_arg_int(1) | HUD_HIDE_TIMER );	
	
	/*if(get_user_team(id) != 2){
		if(!(get_msg_arg_int(1) & HUD_HIDE_RHA))
			set_msg_arg_int(1,ARG_BYTE,get_msg_arg_int(1) | HUD_HIDE_RHA);	
	}*/
	
	if(!(get_msg_arg_int(1) & HUD_HIDE_MONEY))
		set_msg_arg_int(1,ARG_BYTE,get_msg_arg_int(1) | HUD_HIDE_MONEY);	
}
public resetMenu(id){
	
	if(!playerLogged(id)){
		mainMenuAccount(id);
		return PLUGIN_CONTINUE;
	}
	
	new gText[1756], iLen;
	
	new price = priceReset(id);
		
	new needLvl[60], needNugget[60];
	format(needLvl, sizeof(needLvl),  "\y |\d Potrzebujesz:\r %d Lv.", MAXLVL - userLevel[id]);
	format(needNugget, sizeof(needNugget),  "\y |\d Potrzebujesz:\r %d Brylek", price   - userNugget[id]);

	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dIlosc Resetow:\r %d^n", symbolsCustom[SYMBOL_DR_ARROW], userReset[id]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dAby zresetowac poziom i dostac bonusy potrzebujesz:^n", symbolsCustom[SYMBOL_DR_ARROW]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dPoziom:\r %d%s^n", symbolsCustom[SYMBOL_DR_ARROW], MAXLVL, 	MAXLVL > userLevel[id] ? needLvl : "\y |\r Wymaganie Spelnione");
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dBrylki:\r %d%s^n", symbolsCustom[SYMBOL_DR_ARROW], price, 	price > userNugget[id] ? needNugget : "\y |\r Wymaganie Spelnione");

	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\y%s Plusy:^n", symbolsCustom[SYMBOL_DOT]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\d-\r Wiecej EXP'a^n");
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\d-\r Wiecej Brylek^n");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\y%s Minusy:^n", symbolsCustom[SYMBOL_DOT]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\d-\r Drozsze itemy w sklepie^n");
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\d-\r Wiecej EXP'a na poziom^n");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\dW najblizszym czasie cos dodam jeszcze!^n");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r1.\w Biore!");
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r2.\w Rezygnuje");
	
	
	showMenu(id, B1 | B2 , gText, -1, "resetMenu"  );
	return PLUGIN_HANDLED;
}

public resetMenu_2(id, item){
	switch(item){
		case 0:{
			new price = priceReset(id);
			if(userNugget[id] <  price){
				chatPrint(id, PREFIX_LINE, "Brakuje ci Brylek!^3 [^4%d - brylek^3]", price - userNugget[id]);
				return PLUGIN_CONTINUE;
			}
			if(userLevel[id] < MAXLVL){
				chatPrint(id, PREFIX_LINE, "Brakuje ci poziomu!^3 [^4%d - poziomow^3]", MAXLVL - userLevel[id]);
				return PLUGIN_CONTINUE;
			}
			
			chatPrint(id, PREFIX_LINE, "Zresetowales swoj Poziom!^3 Otrzymales + 1 reset");
			userNugget[id] -= price;
			userLevel[id] = 1;
			userExp[id] = 0.0;
			userReset[id] ++;
			addMission(id, mission_NEW, 1);
			
			if(get_user_weapon(id) == CSW_GLOCK18) addSecretMission(id, mission_secret_NEWBEGINNING, 1);

		}
		case 1:{
			return PLUGIN_HANDLED;
		}	
	}
	return PLUGIN_CONTINUE;
}
public priceReset(id){
	return  userReset[id] == 0 ? costReset :  costReset + ( userReset[id] * costResetMulti );
}
public needKills(id, weapon, lvl) return ((lvl == 0) ? (lvl = (str_to_num(allGuns[weapon][5]) / 2)) : (lvl* (str_to_num(allGuns[weapon][5]) * userWeaponLevel[id][weapon])));
public addLevelKills(id, weapon, headShot){
			
	userWeaponKill[id][weapon] ++;
	if(headShot) userWeaponHs[id][weapon] ++;
	
	if(userWeaponLevel[id][weapon] ==  str_to_num(allGuns[weapon][4])) return;
		
	if(equal(allGuns[weapon][0], weapons[get_user_weapon(id)])){
		while(userWeaponKill[id][weapon] >= needKills(id, weapon, userWeaponLevel[id][weapon])){			
			if( userWeaponLevel[id][weapon] <  str_to_num(allGuns[userWeaponSelect[id]][4])){
				userWeaponLevel[id][weapon] ++;
				chatPrint(id, PREFIX_LINE, "Wbiles^3 %d Lv.^1 na broni:^3 %s" , userWeaponLevel[id][weapon] ,allGuns[userWeaponSelect[id]][1]);
				
				if(get_user_weapon(id) == CSW_GLOCK && (userWeaponLevel[id][weapon] ==  str_to_num(allGuns[weapon][4]))){
					addSecretMission(id, mission_secret_MASTER, 1);
				}
			}
		}
	}
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
