#include <amxmodx>
#include <amxmisc>
#include <cstrike>
#include <fun>
#include <hamsandwich>
#include <fakemeta>
#include <fakemeta_util>
#include <engine>

public zombiePrecache(){
	new gText[128];
	for( new i = 0; i < sizeof(classesZombies) ; i ++ ){
		format(gText, sizeof(gText), "models/player/%s/%s.mdl", classesZombies[i][5], classesZombies[i][5]);
		precache_model(gText);
		precache_model(classesZombies[i][6]);
	}
}
public classZombie(id){

	if(!is_user_connected(id)) return;

	new gText[170];
	format(gText, sizeof(gText), "\r[BaseBuilder]\y Klasy Zombie!^n^n\y%s^t^t\dLicznik Smierci:\r %d\w |\y", symbolsCustom[SYMBOL_DR_ARROW], userDeathNum[id]);
						
	new menu = menu_create(gText, "classZombie_2");
	
	for( new i = 0;  i < sizeof(classesZombies); i ++ ){
		
		if(str_to_num(classesZombies[i][4]) > userTime[id] || str_to_num(classesZombies[i][3]) > userLevel[id]) format(gText, sizeof(gText), "\d%s [ HP: \r%s\d Speed:\r %s\d ]", classesZombies[i][0], classesZombies[i][1], classesZombies[i][2]);  		
		else if( userNewClass[id] == i && userNewClass[id] != userClass[id] )  format(gText, sizeof(gText), "\r*%s\d [ HP: \r%s\d Speed:\r %s\d ]", classesZombies[i][0], classesZombies[i][1], classesZombies[i][2]);  
		else format(gText, sizeof(gText), "%s%s\d [ HP: \r%s\d Speed:\r %s\d ]", userClass[id] == i ? "\r" :"\w", classesZombies[i][0], classesZombies[i][1], classesZombies[i][2]);  
		menu_additem(menu, gText);
	}
	
	menu_display(id, menu, 0);
}
public classZombie_2(id, menu, item){	
	if( item == MENU_EXIT ){
		menu_destroy(menu);
		return PLUGIN_CONTINUE;
	}
	userVarMenu[id] = item;
	selectClass(id, item);
	return PLUGIN_CONTINUE;
}
public selectClass(id, item){
	
	new class = userVarMenu[id];
	new gText[512], iLen;

	new gamePlayUser =  str_to_num(classesZombies[class][4]) - playedTime(id);
	new gamePlayUser2 =  str_to_num(classesZombies[class][4]);
	new availableClassHour[128], availableClassLevel[128]; 
	
	format(availableClassHour, sizeof(availableClassHour),  "\w|\d Dostepna za:\r %dh %s%dm %s%ds", gamePlayUser/HOUR, (gamePlayUser/MINUTE)<10?"0":"",gamePlayUser/MINUTE%MINUTE, gamePlayUser%MINUTE<10?"0":"", gamePlayUser%MINUTE);
	format(availableClassLevel, sizeof(availableClassLevel),  "\w|\d Dostepna za:\r %d lvl!", str_to_num(classesZombies[class][3])-userLevel[id]);
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Informacje o Zombie!^n^n");

	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dZombie:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW],classesZombies[class][0]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dOpis:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW],classesZombies[class][7]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dHP:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW],classesZombies[class][1]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dSpeed:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW],classesZombies[class][2]);
	
	if(class != class_CLASSIC){
		
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\w[ Wymagania ]^n");
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dPoziom Postaci:\r %s %s^n", symbolsCustom[SYMBOL_DR_ARROW],classesZombies[class][3], (str_to_num(classesZombies[class][3]) > userLevel[id]) ? availableClassLevel : "\w| \yWymaganie spelnione!");
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dGodziny:\r %dh %s%dm %s%ds %s^n", symbolsCustom[SYMBOL_DR_ARROW],gamePlayUser2/HOUR, (gamePlayUser2/MINUTE)<10?"0":"",gamePlayUser2/MINUTE%MINUTE, gamePlayUser2%MINUTE<10?"0":"", gamePlayUser2%MINUTE, (str_to_num(classesZombies[class][4]) > userTime[id]) ? availableClassHour : "\w| \yWymaganie spelnione!");
	}
	if(str_to_num(classesZombies[class][4]) > userTime[id] || str_to_num(classesZombies[class][3]) > userLevel[id]){
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\d1. Wybierz");
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\d2. Ulepsz");
	} else {
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r1. Wybierz");	
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r2. Ulepsz");
	}
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\r0.\y Anuluj");

	showMenu(id, B1 | B2 | B0   , gText, -1, "selectClass"  );
	return PLUGIN_CONTINUE;
}
public selectClass_2(id, item){

	new class = userVarMenu[id];
	new gamePlayUser =  str_to_num(classesZombies[class][4]) - playedTime(id);
	
	switch(item){
		case 0:{
			if(str_to_num(classesZombies[class][4]) > userTime[id] && str_to_num(classesZombies[class][3]) > userLevel[id]){
				chatPrint(id, PREFIX_LINE, "Nie mozesz wybrac tego Zombie...^1 Musisz wbic jeszcze^3 %d poziomow!^4 |^1 przegrac^3 %d:%s%d:%s%d godz!", str_to_num(classesZombies[class][3])-userLevel[id],gamePlayUser/HOUR, (gamePlayUser/MINUTE)<10?"0":"",gamePlayUser/MINUTE%MINUTE, gamePlayUser%MINUTE<10?"0":"", gamePlayUser%MINUTE);
				classZombie(id);
				return PLUGIN_CONTINUE;
			} else if(str_to_num(classesZombies[class][3]) > userLevel[id]){
				chatPrint(id, PREFIX_LINE, "Nie mozesz wybrac tego Zombie...^1 Musisz wbic jeszcze^3 %d poziomow!", str_to_num(classesZombies[class][3])-userLevel[id]);
				classZombie(id);
				return PLUGIN_CONTINUE;
			}else if(str_to_num(classesZombies[class][4]) > userTime[id] && str_to_num(classesZombies[class][3]) <= userLevel[id] ){
				chatPrint(id, PREFIX_LINE, "Nie mozesz wybrac tego Zombie...^1 Musisz jeszcze przegrac^3 %d:%s%d:%s%d godz!", gamePlayUser/HOUR, (gamePlayUser/MINUTE)<10?"0":"",gamePlayUser/MINUTE%MINUTE, gamePlayUser%MINUTE<10?"0":"", gamePlayUser%MINUTE);
				classZombie(id);
				return PLUGIN_CONTINUE;
			}
			if( !(str_to_num(classesZombies[class][4]) > userTime[id] && str_to_num(classesZombies[class][3]) > userLevel[id]) ){
				if( userClass[id] != class ){
					if( buildTime || prepTime ){
						if(userDeathNum[id]) chatPrint(id, PREFIX_LINE, "Twoj licznikch smierci z tej rundy zostal zresetowany^3 (^4%d^3)", userDeathNum[id]);
						userDeathNum[id] = 0;
						chatPrint(id, PREFIX_LINE, "Twoja nowa klasa:^4 %s", classesZombies[class][0] );
						
						userClass[id] = class;
						userNewClass[id] = class;
						setZombieClass(id);
					}else{
						if(userDeathNum[id]) chatPrint(id, PREFIX_LINE, "Twoj licznikch smierci z tej rundy zostal zresetowany^3 (^4%d^3)", userDeathNum[id]);
						userDeathNum[id] = 0;
						
						userNewClass[id] = class;
						chatPrint(id, PREFIX_LINE, "Twoja klasa zmieni sie po odrodzeniu na:^4 %s ", classesZombies[class][0] );
					}	
				}else chatPrint(id, PREFIX_LINE, "Aktualnie grasz ta klasa");
			}
		}
		case 1:{
			if(str_to_num(classesZombies[class][4]) > userTime[id] || str_to_num(classesZombies[class][3]) > userLevel[id]){
				chatPrint(id, PREFIX_LINE, "Nie mozesz ulepszyc tego Zombie jeszcze!");
			} else {
				upgradeZombieClass(id);
				
			}
		}
		default: classZombie(id);
	}
	return PLUGIN_CONTINUE;
}
public upgradeZombieClass(id){
	
	new class = userVarMenu[id];
	new gText[1024], iLen;

	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Ulepszenia Zombie!^n^n");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dKosci:\r %s^n^n", symbolsCustom[SYMBOL_DR_ARROW],formatNumber(userBone[id]));
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dZombie:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW],classesZombies[class][0]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dHP:\r %d^n", symbolsCustom[SYMBOL_DR_ARROW], calcZombieUpgradeHealth(id, class) + str_to_num(classesZombies[class][1]));
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dSpeed:\r %d^n", symbolsCustom[SYMBOL_DR_ARROW], calcZombieUpgradeSpeed(id, class) + str_to_num(classesZombies[class][2]));
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dRedukcja obrazen:\r %0.1f%%^n", symbolsCustom[SYMBOL_DR_ARROW], calcZombieUpgradeReduction(id, class));

	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r1.\w HP\y [\d%d\w/\d%d\y]\d | Cena:\r %d", userZombie[id][class][UP_ZM_HEALTH], str_to_num(upgradeClasses[class][3]), calcZombieUpgradeCost(id, class, UP_ZM_HEALTH))	;
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r2.\w Speed\y [\d%d\w/\d%d\y]\d | Cena:\r %d", userZombie[id][class][UP_ZM_SPEED], str_to_num(upgradeClasses[class][4]), calcZombieUpgradeCost(id, class, UP_ZM_SPEED));	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r3.\w Redukcja\y [\d%d\w/\d%d\y]\d | Cena:\r %d", userZombie[id][class][UP_ZM_REDUCTION], str_to_num(upgradeClasses[class][5]), calcZombieUpgradeCost(id, class, UP_ZM_REDUCTION));
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\r0.\y Wroc");

	showMenu(id, B1 | B2 | B3 | B0   , gText, -1, "upgradeZombieClass"  );
	return PLUGIN_CONTINUE;
}
public upgradeZombieClass_2(id, item){
	new class = userVarMenu[id];
	
	switch(item){
		case 0:{
			new cost = calcZombieUpgradeCost(id, class, UP_ZM_HEALTH);
			if(userZombie[id][class][UP_ZM_HEALTH] >=  str_to_num(upgradeClasses[class][3])){
				chatPrint(id, PREFIX_LINE, "Posiadasz juz maksymalny poziom ulepszenia^3 Zycia^1 [^4%s^1]", classesZombies[class][0]);
				upgradeZombieClass(id);
				return;
			}
			if(cost > userBone[id]){
				chatPrint(id, PREFIX_LINE, "Nie posiadasz^3 kosci^1 aby ulepszyc^3 Zycie^1 [^4%s^1]", classesZombies[class][0]);
				upgradeZombieClass(id);
				return;
			}
	
			userZombie[id][class][UP_ZM_HEALTH] ++;
			userBone[id] -= cost;
		}
		case 1:{
			new cost = calcZombieUpgradeCost(id, class, UP_ZM_SPEED);
			if(userZombie[id][class][UP_ZM_SPEED] >=  str_to_num(upgradeClasses[class][4])){
				chatPrint(id, PREFIX_LINE, "Posiadasz juz maksymalny poziom ulepszenia^3 Predkosci^1 [^4%s^1]", classesZombies[class][0]);
				upgradeZombieClass(id);
				return;
			}
			if(cost > userBone[id]){
				chatPrint(id, PREFIX_LINE, "Nie posiadasz^3 kosci^1 aby ulepszyc^3 Predkosc^1 [^4%s^1]", classesZombies[class][0]);
				upgradeZombieClass(id);
				return;
			}
			userZombie[id][class][UP_ZM_SPEED] ++;
			userBone[id] -= cost;
		}
		case 2:{
			new cost = calcZombieUpgradeCost(id, class, UP_ZM_REDUCTION);
			if(userZombie[id][class][UP_ZM_REDUCTION] >=  str_to_num(upgradeClasses[class][5])){
				chatPrint(id, PREFIX_LINE, "Posiadasz juz maksymalny poziom ulepszenia^3 Redukcja obrazen^1 [^4%s^1]", classesZombies[class][0]);
				upgradeZombieClass(id);
				return;
			}
			if(cost > userBone[id]){
				chatPrint(id, PREFIX_LINE, "Nie posiadasz^3 kosci^1 aby ulepszyc^3 Redukcje obrazen^1 [^4%s^1]", classesZombies[class][0]);
				upgradeZombieClass(id);
				return;
			}
			userZombie[id][class][UP_ZM_REDUCTION] ++;
			userBone[id] -= cost;
		}
		default:{
			selectClass(id, item);
			return;
		}
	}
	upgradeZombieClass(id);
	saveInsert(id, 1, 0);
}
public calcZombieUpgradeHealth(id, class) return (userZombie[id][class][UP_ZM_HEALTH] * str_to_num(upgradeClasses[class][0]));
public calcZombieUpgradeSpeed(id, class) return (userZombie[id][class][UP_ZM_SPEED] * str_to_num(upgradeClasses[class][1]));
public Float:calcZombieUpgradeReduction(id, class) return (userZombie[id][class][UP_ZM_REDUCTION] * str_to_float(upgradeClasses[class][2]));
public calcZombieUpgradeCost(id, class, type) return (userZombie[id][class][type] * str_to_num(upgradeClasses[class][6 + type])) + 15;
public setZombieClass(id){
	
	switch(get_user_team(id)){
		case USER_ZOMBIE:{			
			if( userNewClass[id] != userClass[id] )
				userClass[id] = userNewClass[id];
			new class = userClass[id];
			
			new additionalHP = floatround( str_to_float( classesZombies[class][1] ) *0.07) *userDeathNum[id];
			additionalHP += userHPAddRound[id];
			
			userMaxHealth[id]	= str_to_num( classesZombies[class][1] ) + additionalHP + ( isSVip(id) ? hpSVipZombi : isVip(id) ?  hpVipZombi : 0 ) + calcZombieUpgradeHealth(id, class);
			userMaxSpeed[id] 	= str_to_float( classesZombies[class][2] ) + calcZombieUpgradeSpeed(id, class);
			
			new clanHealth = 0;
			
			if(clan[id]) clanHealth = (get_clan_info(clan[id], CLAN_HEALTH) * 50);
			
			set_user_health(id, userMaxHealth[id] + clanHealth);
			set_user_maxspeed(id, userMaxSpeed[id]);
			
			userDraculaUsed[id] = false;		
		}
	}
	
	if( task_exists(id+TASK_MODEL) )
		remove_task(id+TASK_MODEL);
	set_task(1.0,"refreshModel", id+TASK_MODEL);
	return PLUGIN_CONTINUE;
}
public refreshModel(id){
	id -= TASK_MODEL;
	
	StripWeapons(id, C4);
	new class = userClass[id];

	switch(get_user_team(id) ){
		case USER_ZOMBIE:{		
			cs_set_user_model(id, classesZombies[class][5]);	
			entity_set_string( id , EV_SZ_viewmodel , classesZombies[class][6]);  
			entity_set_string( id , EV_SZ_weaponmodel , "" );
		}
		case USER_HUMAN:{
			setModelsKnifeHuman(id);
			cs_set_user_model(id, "sas") ;
		}
	}
}
public setModelsKnifeHuman(id){
	new humanClass = userClassHuman[id] ;

	if(!gameTime){
		if(hasOption(userSaveOption[id], save_MODELS)) entity_set_string(id, EV_SZ_viewmodel, MODELHANDS);
		else entity_set_string( id , EV_SZ_viewmodel , "models/v_knife.mdl");  
		
		entity_set_string(id,EV_SZ_weaponmodel, "");
	} else {
		if(!(strlen(classHumanKnifeModel[humanClass][0])>0 || strlen(classHumanKnifeModel[humanClass][1])>0)){	
			entity_set_string( id , EV_SZ_viewmodel , "models/v_knife.mdl");  
			entity_set_string( id , EV_SZ_weaponmodel , "models/p_knife.mdl" ); 
		} else {
			if(hasOption(userSaveOption[id], save_MODELS)) entity_set_string( id , EV_SZ_viewmodel , classHumanKnifeModel[humanClass][0]);  
			else entity_set_string( id , EV_SZ_viewmodel , "models/v_knife.mdl");  
			
			entity_set_string( id , EV_SZ_weaponmodel ,  classHumanKnifeModel[humanClass][1]);
		}
		
	}
	
	return PLUGIN_CONTINUE;
}


public setUserSpeed(id){
	if(!is_user_alive(id)) return;
	set_user_maxspeed(id, userMaxSpeed[id]-userSlow[id]-userSpeedAdmin[id]);	
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
