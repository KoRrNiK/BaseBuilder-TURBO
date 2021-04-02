#include <	amxmodx		>
#include <	amxmisc		>
#include <	cstrike		>
#include <	fun		>
#include <	fakemeta	>
#include <	fakemeta_util	>
#include <	engine		>

new bool:userBuyClassAccept[33];
new userVarClass[33][33];
new userRocketItem[33];
new userRocketCamera[33];

public classHuman(id){
	new gText[170];
	format(gText, sizeof(gText), "\dKliknij na Czlowieka aby sprawdzic co posiada! ");
				
				
	new menu = menu_create(gText, "classHuman_2");
	
	menu_additem(menu, formatm("\
					%sZakup losowa klase %s^n\
					\y%s^t^t\dCena klasy:\r %d^n",
	userBuyClassAccept[id] ? "\r" : "\w",userBuyClassAccept[id] ? "\r[POTWIERDZ]":"",symbolsCustom[SYMBOL_DR_ARROW], (allClassHumman(id)+ defaultCostClass)));
	
	new iNum = 1;
	for( new i = 0;  i < sizeof(classesHuman); i ++ ){

		if(!(hasClassHuman(id, i))) 		
			format(gText, sizeof(gText), "\d%s\r [Zablokowana]", classesHuman[i][0]); 	
		else if( userNewClassHuman[id] == i && userNewClassHuman[id] != userClassHuman[id])	
			format(gText, sizeof(gText), "\r*%s\y [Zakupiona]\d (\rLv. %d\d)", classesHuman[i][0], userHumanLevel[id][i]);	
		else 	format(gText, sizeof(gText), "%s%s\y [Zakupiona]\d (\rLv. %d\d)",  userClassHuman[id] == i ? "\r" :"\w", classesHuman[i][0], userHumanLevel[id][i]);  
		
		userVarClass[id][iNum++] = i;	
			
		menu_additem(menu, gText);
	
	}
	menu_display(id, menu, 0);
}
public classHuman_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return PLUGIN_CONTINUE;
	}

	userVarMenu[id] = userVarClass[id][item];
	
	switch(item){
		case 0: randomClassAll(id);		
		default:{
			if(hasClassHuman(id, userVarMenu[id])) upgradeClass(id, userVarMenu[id]);	
			else buyClass(id, userVarMenu[id]);		
		}
	}
	return PLUGIN_CONTINUE;
}
public randomClassAll(id){
	new all = 0;
	for( new i = 0;  i < sizeof(classesHuman); i ++ ){
		if(!hasClassHuman(id, i)) continue;
		all ++;
	}
	if(all == sizeof(classesHuman)){
		chatPrint(id, PREFIX_LINE, "Masz juz kupione wszystkie klasy!");
		return PLUGIN_CONTINUE;
	}
	
	new class = random(human_TOTAL - 1) + 1;
		
	if(hasClassHuman(id, class)){
		randomClassAll(id);
		return PLUGIN_CONTINUE;
	}	
			
	new cost = defaultCostClass + (allClassHumman(id));
		
	if(userNugget[id] < cost){
		chatPrint(id, PREFIX_LINE, "Nie posiadasz Brylek aby kupic ta klase!");
		return PLUGIN_CONTINUE;
	}
	if(!userBuyClassAccept[id]){
		userBuyClassAccept[id] = true;
		classHuman(id);
		return PLUGIN_CONTINUE;
	}
	new gText[128];
	logType[id] = LOG_CLASS;
	if(logType[id] == LOG_CLASS){
		format(gText, sizeof(gText), "kupil klase [%s]", classesHuman[class][0]);
		logBB(id,gText);
	}
					
	addClassHuman(id, class);
	chatPrint(id, PREFIX_LINE, "Zakupiles klase^3 %s!", classesHuman[class][0]);
	userNugget[id] -= cost;
	userHumanLevel[id][class] ++;
	userBuyClassAccept[id] = false;
	classHuman(id);
	return PLUGIN_CONTINUE;
}

public Float:coolDownClass(id, class, bonus)
	return (((isSVip(id) ? 0.90 : isVip(id) ? 0.95 : 1.0) - ( (clan[id] && get_clan_info(clan[id], CLAN_COOLDOWN) >= 1) ? 0.01 * get_clan_info(clan[id], CLAN_COOLDOWN) : 0.0 )) *str_to_float( bonusClass[bonus][2]))  - ( userHumanLevel[id][class]  * str_to_float( bonusClass[bonus][3]));
	
	
public upgradeClass(id, item){
	new class = userVarMenu[id];
		
	if(!playerLogged(id)){
		mainMenuAccount(id);
		return PLUGIN_CONTINUE;
	}
	
	new gText[1756], iLen;

	
	new lvl = userHumanLevel[id][class];
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Informacje o Klasie!^n");
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\d| Dodatki z Misji nie pojawiaja sie tutaj |^n^n");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dNazwa:\r %s^n",   symbolsCustom[SYMBOL_DR_ARROW], classesHuman[class][0]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dOpis:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], classesHuman[class][1]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dLevel:\r %d^n", symbolsCustom[SYMBOL_DR_ARROW], userHumanLevel[id][class]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dXp:\r %0.2f / %0.2f^n^n",  symbolsCustom[SYMBOL_DR_ARROW],userExpClass[id][class],needXpClass(lvl));
	
	new bonus = str_to_num(classesHuman[class][4]);
	
	if(!(bonus == -1)){
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\w- Moc^n");
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\d%s:\r %0.1f odnowa^n",  bonusClass[bonus][0],  coolDownClass(id, class, bonus));

		
		if( bonus == bonus_HEALTH){
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dLeczenie opszarowe:\r %d HP^n", str_to_num( bonusClass[bonus][4]) *  userHumanLevel[id][class]);
		}	
		if(bonus == bonus_BOTTLE){
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dObrazenia z Fiolki:\r %d^n", str_to_num( bonusClass[bonus][4]) *  userHumanLevel[id][class]);
		}
		if(bonus == bonus_TRAP){
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dObrazenia z Pulapki:\r %d^n", str_to_num( bonusClass[bonus][4]) *  userHumanLevel[id][class]);	
		}
		if(bonus == bonus_FIELD){
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dObrazenia z Stojaka:\r %d^n", str_to_num( bonusClass[bonus][4]) *  userHumanLevel[id][class]);	
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dCzas Stania:\r %0.2f sek^n", float(2  + (userHumanLevel[id][human_ELEKTRYK]/2)));	
		}
		if(bonus == bonus_AMMO){	
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dCzas Stania:\r %0.2f sek^n", float(1  + (userHumanLevel[id][human_SHOOTER]/3)));	
		}
		if(bonus == bonus_PUSH){	
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dMoc odepchniecia:\r %d^n",  str_to_num( bonusClass[bonus][4]) *  userHumanLevel[id][class]);	
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dObrazenia z Pierscienia:\r %d^n",  200);	
		}
		if(bonus == bonus_FIREBALL ){
			new minDmg	= 	( str_to_num( bonusClass[bonus_FIREBALL][4]) *  userHumanLevel[id][human_MAG] ) / 2;
			new maxDmg	=	( str_to_num( bonusClass[bonus_FIREBALL][4]) *  userHumanLevel[id][human_MAG] );
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dObrazenia z Kuli Ognia:\r %d - %d^n", minDmg, maxDmg);
			
		}
		if(bonus == bonus_ICEBOLT ){
			new minDmg	= 	( str_to_num( bonusClass[bonus_ICEBOLT][4]) *  userHumanLevel[id][human_ICEMAG] ) / 2;
			new maxDmg	=	( str_to_num( bonusClass[bonus_ICEBOLT][4]) *  userHumanLevel[id][human_ICEMAG] ); 
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dObrazenia z Kuli Lodu:\r %d - %d^n", minDmg, maxDmg);
			
		}
		if(bonus == bonus_HS){
			new timeBonus = str_to_num( bonusClass[bonus_HS][4]) *  (userHumanLevel[id][human_AIM] / 4) + 1;
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dCzas Trwania:\r %0.2f sek^n", float(timeBonus));	
		}
		if(bonus == bonus_BOMB){
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dObrazenia z Bombki:\r %d^n", str_to_num( bonusClass[bonus][4]) *  userHumanLevel[id][class]);	
		}
		if(bonus == bonus_DAMAGE){
			new timeBonus = str_to_num( bonusClass[bonus_DAMAGE][4]) *  (userHumanLevel[id][human_TRUPOSZ] / 4) + 1;
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dCzas Trwania:\r %0.2f sek^n", float(timeBonus));	
		}
		
		if(bonus == bonus_SPEED){
			new timeBonus = str_to_num( bonusClass[bonus_SPEED][4]) *  (userHumanLevel[id][human_MINER] / 4) + 1;
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dCzas Trwania:\r %0.2f sek^n", float(timeBonus));	
		}
		
		if(bonus == bonus_ROCKET){
			new timeBonus = 10;
			
			new minDmg	= 	( str_to_num( bonusClass[bonus_ROCKET][4]) *  userHumanLevel[id][human_SEARCH] ) / 2;
			new maxDmg	=	( str_to_num( bonusClass[bonus_ROCKET][4]) *  userHumanLevel[id][human_SEARCH] ); 
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dObrazenia z Rakiety:\r %d - %d^n", minDmg, maxDmg);
			
			iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dCzas Trwania:\r %0.2f sek^n", float(timeBonus));	
		}
	}
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\w- Bonus^n");
	if( class == human_FREE ){
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dSzansa na Brylke:\r %0.2f%%^n",  str_to_float(paramClassesHuman[class][0])*lvl+ 0.00001);
	}
	
	
	if( class == human_HEALER ){
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dZycie:\r +%d HP^n",  str_to_num(paramClassesHuman[class][1])*lvl);
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dObrazenia:\r +%d DMG^n", str_to_num(paramClassesHuman[class][3])*lvl);	
	}
	
	if( class == human_ELEKTRYK ){
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dSzansa na Piorun:\r %0.2f%%^n",  str_to_float(paramClassesHuman[class][0])*lvl+ 0.00001);
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dObrazenia z Pioruna:\r %d^n", str_to_num(paramClassesHuman[class][1])*lvl);
	}
	if( class == human_LAB ){
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dSzansa na Odowienie:\r %0.2f%%^n",  str_to_float(paramClassesHuman[class][0])*lvl+ 0.00001);
	}
	if( class == human_HUNTER ){
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dSzansa na Spowolnienie:\r %0.2f%%^n",  str_to_float(paramClassesHuman[class][0])*lvl+ 0.00001);
	}
	
	if( class == human_SHOOTER ) {
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dSzansa na Dodatkowa amunicje:\r %0.2f%%^n",  str_to_float(paramClassesHuman[class][0])*lvl+ 0.00001);
	}	
	if( class == human_BULLDOZER ) {
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dSzansa na Odepchniecie:\r %0.2f%%^n",  str_to_float(paramClassesHuman[class][0])*lvl+ 0.00001);
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dMoc odepchniecia:\r %d^n",  str_to_num(paramClassesHuman[class][2])*lvl);
	}
	
	if( class == human_MAG ) {
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dSzansa na Podpalenie:\r %0.2f%%^n",  str_to_float(paramClassesHuman[class][0])*lvl+ 0.00001);
	}

	if( class == human_ICEMAG ) {
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dSzansa na Zamrozenie:\r %0.2f%%^n",  str_to_float(paramClassesHuman[class][0])*lvl+ 0.00001);
	}
	if( class == human_AIM ) {
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dSzansa na 100%HS:\r %0.2f%%^n",  str_to_float(paramClassesHuman[class][0])*lvl+ 0.00001);
	}
	if( class == human_BOOMBERMA ) {
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dSzansa na Pulapke:\r %0.2f%%^n",  str_to_float(paramClassesHuman[class][0])*lvl+ 0.00001);
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dObrazenia z Pulapki:\r %d^n", str_to_num(paramClassesHuman[class][1])*lvl);
	}

	if( class == human_TRUPOSZ ) {
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dSzansa na Rykoszet:\r %0.2f%%^n",  str_to_float(paramClassesHuman[class][0])*lvl+ 0.00001);
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dSzansa na Krytyk:\r %0.2f%%^n",  str_to_float(paramClassesHuman[class][2])*lvl+ 0.00001);
	}
	if( class == human_SEARCH ) {
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dWieksza szansa na skrzynke:\r +%0.2f%%^n",  str_to_float(paramClassesHuman[class][0])*lvl+ 0.00001);
	}
	
	if( class == human_MINER ) {
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dSzansa na brylke:\r %0.2f%%^n",  str_to_float(paramClassesHuman[class][0])*lvl+ 0.00001);
	}
	
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r1.\w Wybierz!");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r2.\w Wroc");
	
	showMenu(id, B1 | B2, gText, -1, "upgradeClass");
	return PLUGIN_HANDLED;
	
}
public upgradeClass_2(id, item){
	new class = userVarMenu[id];
	
	switch(item){
		case 0:{
			if( userClassHuman[id] != class ){
				if( buildTime || prepTime ){
					
					chatPrint(id, PREFIX_LINE, "Twoja nowa klasa:^3 %s", classesHuman[class][0] );
							
					userClassHuman[id] = class;
					userNewClassHuman[id] = class;
					setHumanClass(id);
				}else{
					userNewClassHuman[id] = class;
					chatPrint(id, PREFIX_LINE, "Twoja klasa zmieni sie po odrodzeniu na:^3 %s", classesHuman[class][0] );
				}
					
			} else { 
				chatPrint(id, PREFIX_LINE, "Aktualnie grasz ta klasa"); 
				classHuman(id);
			}
		}
		case 1: classHuman(id);
		
	}
	return PLUGIN_CONTINUE;
}


public buyClass(id, item){
	new class = userVarMenu[id];
		
	if(!playerLogged(id)){
		mainMenuAccount(id);
		return PLUGIN_CONTINUE;
	}
	
	new gText[1756], iLen;
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Informacje o Klasie!^n^n");

	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dNazwa:\r %s^n",  symbolsCustom[SYMBOL_DR_ARROW],classesHuman[class][0]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dOpis:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], classesHuman[class][1]);

	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r2.\w Wroc");
	
	
	showMenu(id, B1 | B2 , gText, -1, "buyClass"  );
	return PLUGIN_HANDLED;
	
}

public buyClass_2(id, item){
	new class = userVarMenu[id];

	switch(item){
		
		case 0: buyClass(id, class);
		case 1: classHuman(id);
	}
	return PLUGIN_CONTINUE;
}

public setHumanClass(id){
	
	switch(get_user_team(id)){
		case 2:{
			new class = userClassHuman[id];
			
			userMaxSpeed[id] 	= 250.0;
			set_user_maxspeed(id, userMaxSpeed[id]);
			
			if(hasClassHuman(id, class)){
				
			
				new bool:isProHealth = didPro(id, pro_BETON);
				
				new hpCT;
				
				if( userNewClassHuman[id] != userClassHuman[id] )
					userClassHuman[id] = userNewClassHuman[id];
					
				
				if(userClassHuman[id] == human_HEALER){
					hpCT += str_to_num(paramClassesHuman[human_HEALER][1])*userHumanLevel[id][human_HEALER] + (isProHealth ? 10 : 0 );
				}
				
				
				userMaxArmor[id]	= userLevel[id] * 2;
				userMaxHealth[id]	= 100 + hpCT + ( isSVip(id) ? hpSVipHuman : isVip(id) ?  hpVipHuman : 0 );
				
				new clanHealth = 0;
			
				if(clan[id]) clanHealth = (get_clan_info(clan[id], CLAN_HEALTH) * 5);
			
				set_user_health(id, userMaxHealth[id] + clanHealth);
				
				set_user_health(id, userMaxHealth[id]);
				set_user_armor(id, userMaxArmor[id]);
			}
			
			
		}
	}
	if( task_exists(id+TASK_MODEL) )
		remove_task(id+TASK_MODEL);
	set_task(0.1,"refreshModel", id+TASK_MODEL);
	return PLUGIN_CONTINUE;
}


public bool:hasClassHuman(id, class){
	return (userHuman[id] & (1<<class))?true:false;
}
public addClassHuman(id, class){
	userHuman[id] |= (1<<class);
}
public removeClassHuman(id, class){
	userHuman[id] &= ~(1<<class);
}

public allClassHumman(id){
	new iNum = 0;
	for(new i = 0 ; i < human_TOTAL; i ++){
		if(!hasClassHuman(id, i)) continue;
		
		iNum ++;
	}
	return iNum * 1000 ;
}

public atributeMenu(id){
	
	if(get_user_team(id) != 2){
		chatPrint(id, PREFIX_NORMAL, "Moce sa przeznaczone tylko dla klas Budowniczych!");
		return PLUGIN_CONTINUE;
	}
	
	new menu = menu_create("\r[BaseBuilder]\y Twoje Moce!", "atributeMenu_2");
	
	new bonus = str_to_num(classesHuman[userClassHuman[id]][4]);
	
	new gText[512];
	
	new Float:coolDown = 0.0;
	
	if(!(bonus == -1)){
		coolDown = floatsub(userClassUsed[id][bonus], get_gametime());
		if (!gameTime){
			format(gText, sizeof(gText), "\w%s:\r [\y Poczekaj na runde\r ]", bonusClass[bonus][0]);
		}else {
			if(userClassLast[id][bonus] > get_gametime()){
				format(gText, sizeof(gText), "\r\w%s:\r [\y W trakcie\r ]",  bonusClass[bonus][0]);
			}
			
			if(userSkillLast[id][bonus] > get_gametime() ) format(gText, sizeof(gText), "\r\w%s:\r [\y W trakcjie\r ]",  bonusClass[bonus][0]);
			else if( coolDown <= 0.0 )format(gText, sizeof(gText), "\r\w%s:\r [\y Do uzycia\r ]",  bonusClass[bonus][0]);
			else format(gText, sizeof(gText), "\r\d%s:\r %0.1f sek",  bonusClass[bonus][0], coolDown);
		}
	} else format(gText, sizeof(gText), "\rBrak Mocy");
	
	menu_additem(menu, gText);
		
	menu_display(id, menu, 0);
	return PLUGIN_CONTINUE;
}

public atributeMenu_2(id, menu, item){	
	if( item == MENU_EXIT ){
		menu_destroy(menu);
		return PLUGIN_CONTINUE;
	}
	
	
	new bonus = str_to_num(classesHuman[userClassHuman[id]][4]);
	
	if(get_user_team(id) != 2){
		chatPrint(id, PREFIX_NORMAL, "Moce sa przeznaczone tylko dla klas Budowniczych!");
		return PLUGIN_CONTINUE;
	}
	
	
	if(bonus == -1){
		chatPrint(id, PREFIX_NORMAL, "Ta klasa nie posiada dodatkowych mocy!");
		return PLUGIN_CONTINUE;
	}
	usePower(id, bonus);
	atributeMenu(id);	
	
	return PLUGIN_CONTINUE;
}

public bindPower(id){
	new bonus = str_to_num(classesHuman[userClassHuman[id]][4]);
	
	if(get_user_team(id) != 2) return PLUGIN_HANDLED;
	if(bonus == -1) return PLUGIN_HANDLED;
	
	usePower(id, bonus);
	
	return PLUGIN_HANDLED;
}
public bool:usePower(id, bonus){

	if (0 >  bonus) return false;
	
	if (!gameTime){
		chatPrint(id, PREFIX_NORMAL, "Poczekaj az rozpocznie sie runda");
		return false;
	}
	
	if(get_user_team(id) != 2){
		chatPrint(id, PREFIX_NORMAL, "Moce sa przeznaczone tylko dla klas Budowniczych!");
		return false;
	}
	
	
	if (userClassUsed[id][bonus] > get_gametime()){
		chatPrint(id, PREFIX_NORMAL, "Moc^3 %s^1 odnowi sie za:^3 %0.1f sek", bonusClass[bonus][0], floatsub(userClassUsed[id][bonus], get_gametime()));
		return false;
	}

	new class = userClassHuman[id];
	new Float:offsetTime = useBonus(id, bonus, false);
	if (offsetTime >= 0.0){
		new Float:gTime = coolDownClass(id, class, bonus);
		userClassLast[id][bonus] = floatadd(get_gametime(), offsetTime);
		userClassUsed[id][bonus] = floatadd(get_gametime(), gTime);
		return true;
	}
	return false;
}
public Float:useBonus(id, idBonus, bool:staticTime){
	
	if(get_user_team(id) != 2){
		chatPrint(id, PREFIX_NORMAL, "Moce sa przeznaczone tylko dla klas Budowniczych!");
		return -1.0;
	}
	if(!is_user_alive(id)){
		chatPrint(id, PREFIX_NORMAL, "Musisz byc zywy aby uzyc Mocy!");
		return -1.0;
	}
	
	switch(idBonus){
		case 0: poisonBottle(id);
		case 1: trapCreate(id);
		case 2: electricPower(id);
		case 3: healerBonus(id);
		case 4: ammoCreateBonus(id);
		case 5:{
			
			entity_set_float(	id,	EV_FL_fuser1,		get_gametime() );
			entity_set_float(	id, 	EV_FL_fuser3,		get_gametime() );
			entity_set_float(	id, 	EV_FL_fuser4,		1.5);
			
			pushBonus(id);
		}
		case 6:{
			new bool:isProAdd = didPro(id, pro_FIRE);
			if(isProAdd) set_task(1.0,"fireCreate" ,id);
			fireCreate(id);
		}
		case 7:{
			new bool:isProAdd = didPro(id, pro_ICEFIRE);	
			if(isProAdd) set_task(1.0,"iceBoltCreate" ,id);
			iceBoltCreate(id);
		}
		case 8:{
			
			new bool:isProTime = didPro(id, pro_TARGET);
			
			new timeBonus = str_to_num( bonusClass[bonus_HS][4]) *  (userHumanLevel[id][human_AIM] / 4) + 1 + ( isProTime ? 3 : 0 );
			
			userHitOnlyHs[id] = floatadd(get_gametime(), float(timeBonus));
		
			
			barTime(id, timeBonus);
			
			new Float:fOrigin[3];
			entity_get_vector(id, EV_VEC_origin, fOrigin);
			makeLight(fOrigin,35, 255, 255, 255, 5, 10);
			addPro(id,pro_TARGET, 1);
		}
		case 9: bombCreate(id);
		case 10:{
			
			new bool:isProTime = didPro(id, pro_TRUPOSZ);
			
			new timeBonus = str_to_num( bonusClass[bonus_DAMAGE][4]) * ( userHumanLevel[id][human_TRUPOSZ] / 4 ) + 1 + ( isProTime ? 1 : 0 );
			
			userDamagexTwo[id] = floatadd(get_gametime(), float(timeBonus));
		
				
			barTime(id, timeBonus);
			
			
			emitBonusSound(id, bonus_sound_QUADDAMAGE);
			new Float:fOrigin[3];
			entity_get_vector(id, EV_VEC_origin, fOrigin);
			makeLight(fOrigin, 64, 32, 32, 255, 255, 255);
			setGlow(id, 32,32,255, 10);
			set_task(float(timeBonus), "removeGlow", id);
		}
		case 11: rocketCreate(id);
		case 12:{
			
			new timeBonus = str_to_num( bonusClass[bonus_SPEED][4]) * ( userHumanLevel[id][human_MINER] / 4 ) + 1;
			
			userSpeedFire[id] = floatadd(get_gametime(), float(timeBonus));
		

			barTime(id, timeBonus);
		}
	}
	#if defined CHRISTMAS_ADDON
		
		addChristmasMission(id, CH_POWER, 1);
				
	#endif

	addMission(id, mission_MAGIC, 1);
	chatPrint(id, PREFIX_NORMAL, "Uzyto:^3 %s", bonusClass[idBonus][0]);
	return 0.0;
}


public Float:resetTime(id){
	if(isSuperAdmin(id)){	
		new bonus = str_to_num(classesHuman[userClassHuman[id]][4]);	
		userClassLast[id][bonus] = 0.0;
		userClassUsed[id][bonus] = 0.0;
	}	
}


public fw_touch(ent, toucher){	
	if(!pev_valid(ent)) return HAM_IGNORED;
	
	new szClass[18]; 
	new szClassTouched[18];
	new szTarget[10];
	
	entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass));
	entity_get_string(toucher, EV_SZ_classname, szClassTouched, sizeof(szClassTouched));
	entity_get_string(ent, EV_SZ_targetname, szTarget, sizeof(szTarget));
	
	if(equal(szClass, classPoison)) poisonTouch(ent, toucher);
	if(equal(szClass, classFireBall)) fireBallTouch(ent, toucher);
	if(equal(szClass, classIceBolt)) iceBoltTouch(ent, toucher);
	if(equal(szClass, classBomb)) bombTouch(ent, toucher);

	return HAM_IGNORED;
}

public poisonBottle(id){
	new Float:fVelocity[3], Float:fOrigin[3];

	new ent = create_entity("info_target");
	
	entity_set_string(	ent, 	EV_SZ_classname, 	classPoison);
	
	entity_set_int(		ent, 	EV_INT_solid,		SOLID_TRIGGER);
	entity_set_int(		ent, 	EV_INT_movetype,	MOVETYPE_TOSS);	

	entity_set_model(	ent, 	modelPoison);
	entity_set_float(	ent, 	EV_FL_gravity,		1.0);
	entity_get_vector(	id, 	EV_VEC_origin,		fOrigin);
	fOrigin[2] += 5.0;
	
	entity_set_vector(	ent, 	EV_VEC_origin,		fOrigin);
	velocity_by_aim(	id,	500,			fVelocity);	
	
	entity_set_vector(	ent, 	EV_VEC_velocity, 	fVelocity);

	new Float:fAngles[3];
	fAngles[0] = -140.0; fAngles[1] = 125.0; fAngles[2] = 64.5;
	
	entity_set_vector(	ent, 	EV_VEC_angles, 		fAngles);
	entity_set_int(		ent, 	EV_INT_iuser1,		id );
}
public poisonTouch(ent, toucher){
	
	if(!pev_valid(ent)) return PLUGIN_CONTINUE;	
	
	new owner = entity_get_int(ent, EV_INT_iuser1);
	if(owner == toucher) return PLUGIN_CONTINUE;
	
	new Float:fOrigin[3], Float:fOriginTarget[3];
	new bool:isProDamage= didPro(owner, pro_RADIO);	
	new dmg = str_to_num( bonusClass[bonus_BOTTLE][4]) *  userHumanLevel[owner][human_LAB] + ( isProDamage ? 100 : 0) + floatround(damageClassClan(owner));
	
	
	entity_get_vector(ent, EV_VEC_origin, fOrigin);
	fOrigin[2] += 3.5;
	for( new i = 1 ; i < maxPlayers;i ++ ){
					
		if( !is_user_connected(i) || !is_user_alive(i)|| get_user_godmode(i) || i == owner || get_user_team(i) != 1) continue;			
		entity_get_vector(i, EV_VEC_origin, fOriginTarget);

		if( get_distance_f(fOrigin, fOriginTarget) < 150.0){
			if(userClass[i] == class_POISON ){
				set_user_health(i, min(userMaxHealth[i], get_user_health(i) + dmg));
				Display_Fade(i, 2048, 2048, 2048, 64, 250, 64, 60);
				set_dhudmessage(33, 255,32, -1.0, -1.0, 0, 0.3, 0.8, 0.3);
				show_dhudmessage(i, "++ Zostales Ulczony Trucizna ++");
							
			} else  {
				ExecuteHamB( Ham_TakeDamage, i, owner, owner, float(dmg), DMG_POISON );
				addPro(owner, pro_RADIO,dmg);
			}
		}
	}
	emitBonusSound(ent, bonus_sound_POISON);			
	
	message_begin(MSG_BROADCAST ,SVC_TEMPENTITY);
	write_byte(TE_SPRITE);
	engfunc(EngFunc_WriteCoord,fOrigin[0]);
	engfunc(EngFunc_WriteCoord,fOrigin[1]);
	engfunc(EngFunc_WriteCoord,fOrigin[2]);
	write_short(sprite_explode);
	write_byte(8);
	write_byte(255);
	message_end();
	BeamCylinder(fOrigin, 50.0, spriteBeam, 10, 0, 10, 1, 0,128, 255, 56, 255, 0);
	makeLight(fOrigin,25, 128, 255, 56,15, 30);
	remove_entity(ent);
	return PLUGIN_CONTINUE;
}


public trapCreate(id){
	
	new Float:fOrigin[3];

	new ent = create_entity("info_target");
	
	entity_set_string(	ent, 	EV_SZ_classname, 	classTrap);
	
	entity_set_int(		ent, 	EV_INT_movetype, 	MOVETYPE_TOSS);
	entity_set_int(		ent, 	EV_INT_solid, 		SOLID_BBOX);
	entity_set_model(	ent, 	modelTrap);
	entity_get_vector(	id, 	EV_VEC_origin,		fOrigin);
	fOrigin[2] += -20;
	entity_set_vector(	ent, 	EV_VEC_origin,		fOrigin);
	set_rendering(		ent, 	kRenderFxGlowShell, 	153, 153, 102, 	kRenderNormal, 	1);
	entity_set_int(		ent, 	EV_INT_iuser1,		id );
	
	addPro(id, pro_BEAR,1);
	
	set_task(0.1, "trapThink", ent);
}


public trapThink(ent){
	if( !pev_valid(ent)) return PLUGIN_CONTINUE;

	new szClass[18];
	entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass));
	
	if(!equal(szClass, classTrap)) return PLUGIN_CONTINUE;
	
	new owner = entity_get_int(ent, EV_INT_iuser1);
	
	new dmg = str_to_num( bonusClass[bonus_TRAP][4]) *  userHumanLevel[owner][human_HUNTER] + floatround(damageClassClan(owner));

	if(gameTime  &&  entity_get_int(ent, EV_INT_iuser1)){
		new Float:fOrigin[3];
		new Float:fOriginTarget[3];
		entity_get_vector(ent, EV_VEC_origin, fOrigin);
		new bool:closeTrap;

		for(new i = 1; i < maxPlayers; i++){
			
			if(get_user_team(i) != 1) continue;
			entity_get_vector(i, EV_VEC_origin, fOriginTarget);
			
			if (get_distance_f(fOrigin, fOriginTarget) < 40.0) closeTrap = true;
			
		}
		if(closeTrap){
			emitBonusSound(ent, bonus_sound_TRAP);		
			
			for(new i = 1; i < maxPlayers; i ++){
						
				if(!is_user_alive(i) || !is_user_connected(i) || get_user_team(i) != 1 || i == owner) continue;
					
				entity_get_vector(i, EV_VEC_origin, fOriginTarget);
				
				
				if (get_distance_f(fOrigin, fOriginTarget) < 40.0){
					ExecuteHamB( Ham_TakeDamage, i, owner, owner, float(dmg), DMG_SHOCK );
					addPro(owner, pro_JELLY, i);
					userSlow[i] = 500.0;
					Display_Fade(i, 2048, 2048, 2048, 255, 255, 64, 60);
					set_dhudmessage(255, 255,32, -1.0, -1.0, 0, 0.3, 0.8, 0.3);
					show_dhudmessage(i, "!! Zostales Zatrzymany !!");
					new bool:isProTime = didPro(owner, pro_BEAR);
					new Float:timeTrap = 1.5 + ( isProTime ? 2.0 : 0.0);
					set_task(timeTrap, "removeSlow", i);
				}				
			}			
			remove_entity(ent);
		}
	}
	if (pev_valid(ent)) set_task(0.1, "trapThink", ent);
	return PLUGIN_CONTINUE;
}
public Float:removeSlow(id){
	userSlow[id] = 0.0;
}

public electricPower(id){	
	new Float:fOrigin[3];
	
	new ent = create_entity("info_target");
	entity_set_string(	ent, 	EV_SZ_classname, 	classField);
	
	entity_set_int(		ent, 	EV_INT_movetype, 	MOVETYPE_FLY);
	entity_set_int(		ent, 	EV_INT_solid, 		SOLID_NOT);
	entity_set_model(	ent, 	modelField);
	entity_get_vector(	id, 	EV_VEC_origin,		fOrigin);
	fOrigin[2] 	-= 	distanceToFloor(fOrigin)-6;
	entity_set_vector(	ent, 	EV_VEC_origin,		fOrigin);
	entity_set_float(	ent,	EV_FL_fuser1,		get_gametime() );
	entity_set_float(	ent, 	EV_FL_fuser3,		get_gametime() );
	
	new bool:isProTime = didPro(id, pro_TIMEEFIELD);
	entity_set_float(	ent, 	EV_FL_fuser4,		float(2  + ( userHumanLevel[id][human_ELEKTRYK]/2) + ( isProTime ? 2 : 0)));
	entity_set_int(		ent, 	EV_INT_iuser1,		id );

	set_task(0.1, "electricThink", ent);	
	
}

public electricThink(ent){
	if( !pev_valid(ent) ) return PLUGIN_CONTINUE;

	new Float:gTime = ((get_gametime()-entity_get_float(ent, EV_FL_fuser1))/entity_get_float(ent, EV_FL_fuser4));
	
	
	new Float:fOriginTarget[3];
	entity_get_vector(ent, EV_VEC_origin, fOriginTarget);
	
	new Float:fOriginEnd[3];
	fOriginEnd[0]=fOriginTarget[0];
	fOriginEnd[1]=fOriginTarget[1];
	fOriginTarget[2] += 45.0;
	fOriginEnd[2]=fOriginTarget[2]+2048.0;
	BeamLight(fOriginTarget, fOriginEnd, thunder, 0, 20, 2, 20, 1, 255, 0, 108, 255, 40);
	makeLight(fOriginTarget, 20, 255, 0, 108, 255, 255);
	
	set_task(1.0, "electroDamage", ent);	
	
	if( gTime > 1.0 ) remove_entity(ent);
	else set_task(0.1, "electricThink", ent);
	
	return PLUGIN_CONTINUE;
}
public electroDamage(id, ent){
	
	if(!pev_valid(id) )return PLUGIN_CONTINUE;
	if(get_gametime() - entity_get_float(id, EV_FL_fuser2) < 0.5 ||  get_gametime() - entity_get_float(id, EV_FL_fuser3) < 0.5 ) return PLUGIN_CONTINUE;
		
		
	new Float:fOrigin[3], Float:fOriginTarget[3], iOrigin[3];
	
	entity_get_vector(id, EV_VEC_origin, fOrigin);
	fOrigin[2] -= 5.0;
	BeamCylinder(fOrigin, 50.0, spriteBeam, 0, 5, 30, 1, 90, 255, 0, 108, 255, 1);
	
	new owner= entity_get_int(id, EV_INT_iuser1);
	new bool:isProDamage = didPro(owner, pro_DAMAGEFIELD);
		
	new dmg = str_to_num( bonusClass[bonus_FIELD][4]) *  userHumanLevel[owner][human_ELEKTRYK] + ( isProDamage ? 40 : 0 ) + floatround(damageClassClan(owner));
				
	for( new i = 1; i < maxPlayers; i ++ ){
		if( !is_user_alive(i) || i == owner || get_user_team(i) != 1) continue;
			
		entity_get_vector(i, EV_VEC_origin, fOriginTarget);
		
		if( get_distance_f(fOriginTarget, fOrigin) >= 150.0) continue;
		
		fOrigin[2] += 45.0;
		BeamLight(fOrigin, fOriginTarget, thunder, 0, 20, 2, 50, 20, 255, 0, 108, 255, 40);
		
		addPro(owner, pro_DAMAGEFIELD, dmg);
		
		
		
		ExecuteHamB( Ham_TakeDamage, i, owner, owner, float(dmg), DMG_ENERGYBEAM );
		
		
		
		if( dmg > pev(i, pev_health) ) addPro(owner, pro_TIMEEFIELD, 1);
			
		
		
		emitBonusSound(ent, bonus_sound_ELECTRO);
		
	}
	FVecIVec(fOrigin, iOrigin);
	entity_set_float(id, EV_FL_fuser2, get_gametime());
	return PLUGIN_CONTINUE;
}

public healerBonus(id){
	
	new Float:fOrigin[3], Float:fOriginTarget[3];
	
	entity_get_vector(id, EV_VEC_origin, fOrigin);
	fOrigin[2] -= 34.0;
	BeamCylinder(fOrigin, 50.0, spriteBeam, 0, 5, 30, 1, 60, 32,255, 32, 255, 1);
	
	new bool:isProHealth = didPro(id, pro_HIN);
		
	new health = str_to_num( bonusClass[bonus_HEALTH][4]) *  userHumanLevel[id][human_HEALER] + ( isProHealth ? 25 : 0 );
		
	
	for( new i = 1; i < maxPlayers; i ++ ){
		if( !is_user_alive(i) || get_user_team(i) != 2) continue;
			
		entity_get_vector(i, EV_VEC_origin, fOriginTarget);
		
		if( get_distance_f(fOriginTarget, fOrigin) >= 150.0) continue;
		
		
		addPro(id, pro_HIN, (i * health));
		set_user_health(i, min(userMaxHealth[i], get_user_health(i) + health ));
	}
}
public ammoCreateBonus(id){
	new Float:fOrigin[3];
	
	new ent = create_entity("info_target");
	entity_set_string(	ent, 	EV_SZ_classname, 	classAmmo);
	
	entity_set_int(		ent, 	EV_INT_movetype, 	MOVETYPE_FLY);
	entity_set_int(		ent, 	EV_INT_solid, 		SOLID_NOT);
	entity_set_model(	ent, 	modelAmmo);
	entity_get_vector(	id, 	EV_VEC_origin,		fOrigin);
	fOrigin[2] 	-= 	distanceToFloor(fOrigin)-3;
	entity_set_vector(	ent, 	EV_VEC_origin,		fOrigin);
	entity_set_float(	ent,	 EV_FL_fuser1,		get_gametime() );
	entity_set_float(	ent, 	EV_FL_fuser3,		get_gametime() );
	
	new bool:isProTime = didPro(id, pro_ECOBOX);
	
	entity_set_float(	ent, 	EV_FL_fuser4,		float(1  + ( userHumanLevel[id][human_SHOOTER]/3) + ( isProTime ? 5 : 0)));
	entity_set_int(		ent, 	EV_INT_iuser1,		id );
	addPro(id, pro_ECOBOX,1);
	
	set_task(0.1, "ammoThinkBonus", ent);	
	
}
public ammoThinkBonus(ent){
	if( !pev_valid(ent) ) return PLUGIN_CONTINUE;

	new Float:gTime = ((get_gametime()-entity_get_float(ent, EV_FL_fuser1))/entity_get_float(ent, EV_FL_fuser4));	
	new Float:fOrigin[3], Float:fOriginTarget[3];
	
	entity_get_vector(ent, EV_VEC_origin, fOrigin);
	
	for( new i = 1; i <= maxPlayers; i ++ ){
		if( !is_user_alive(i) || get_user_team(i) != 2) continue;
				
		entity_get_vector(i, EV_VEC_origin, fOriginTarget);
			
		if( get_distance_f(fOriginTarget, fOrigin) >= 150.0) continue;
				

		if( checkWeapon(get_user_weapon(i))){
			fm_cs_get_weapon_ammo( get_pdata_cbase(i, 373), g_MaxClipAmmo[get_user_weapon(i)]);
		}
	}
	if( gTime > 1.0 ) remove_entity(ent);
	else set_task(0.1, "ammoThinkBonus", ent);
	return PLUGIN_CONTINUE;
}


public pushBonus(id){
	

	new Float:gTime = ((get_gametime()-entity_get_float(id, EV_FL_fuser1))/entity_get_float(id, EV_FL_fuser4));
	
	new Float:fOrigin[3], Float:fOriginTarget[3], iOrigin[3];
			
	entity_get_vector(id, EV_VEC_origin, fOrigin);
	fOrigin[2] += 11.0;
	BeamCylinder(fOrigin, 50.0, spriteBeam, 0, 5, 15, 1, 60, 255, 255, 10, 255, 1);
		
	new Float:fVelocity[3];
	new bool:isProPush = didPro(id, pro_CAT);
			
	new powerPush =  str_to_num( bonusClass[bonus_PUSH][4]) *  userHumanLevel[id][human_BULLDOZER] + ( isProPush ? 50 :0 );
		
	for( new i = 1; i < maxPlayers; i ++ ){
		if( !is_user_alive(i) || get_user_team(i) != 1) 
			continue;
			
		entity_get_vector(i, EV_VEC_origin, fOriginTarget);
		
		
		if( get_distance_f(fOriginTarget, fOrigin) >= 100.0) continue;
			
		
		get_user_origin(id, iOrigin, 2);
		IVecFVec(iOrigin, fOrigin);
		
		entity_get_vector(id, 	EV_VEC_origin, 	fOrigin);
		
		
		entity_get_vector(i, 	EV_VEC_origin, 	fOriginTarget);
		fOriginTarget[2] = fOrigin[2];
		xs_vec_sub(fOrigin, fOriginTarget, fVelocity);
		xs_vec_normalize( fVelocity , fVelocity );	
			
			       
		xs_vec_mul_scalar( fVelocity , -float(powerPush) , fVelocity );
		fVelocity[2] *= 1.5;
		entity_set_vector(i, 	EV_VEC_velocity, 	fVelocity);
		ExecuteHamB( Ham_TakeDamage, i, id, id, float(200), DMG_BLAST );
		addPro(id,pro_CAT, i);
	}
	if( gTime > 1.0 ) return;
	else set_task(0.5, "pushBonus", id);
}

public fireCreate(id){

	new Float:fVelocity[3], Float:fOrigin[3], Float:fAngles[3];

	
	entity_get_vector(	id, 	EV_VEC_v_angle, 		fAngles);
	entity_get_vector(	id, 	EV_VEC_origin,		fOrigin);

	new ent = create_entity("info_target");
	
	entity_set_string(	ent, 	EV_SZ_classname, 	classFireBall);
	
	entity_set_int(		ent, 	EV_INT_solid,		SOLID_TRIGGER);
	entity_set_int(		ent, 	EV_INT_movetype,	MOVETYPE_FLY);	
	
	entity_set_model(	ent,	modelFire);
	
	
	fOrigin[2] += 20.0;
	fAngles[0] *= -1.0;
	
	entity_set_vector(	ent, 	EV_VEC_origin,		fOrigin);
	entity_set_vector(	ent, 	EV_VEC_angles, 		fAngles);
	velocity_by_aim(	id,	2000,			fVelocity);

	entity_set_vector(	ent, 	EV_VEC_velocity, 	fVelocity);
	entity_set_int(		ent, 	EV_INT_rendermode, 	5);
	entity_set_float(	ent, 	EV_FL_renderamt, 	255.0);
	
	entity_set_int(		ent, 	EV_INT_iuser1,		id );
	
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte(22);
	write_short(ent); 
	write_short(spriteBeam);
	write_byte(45);
	write_byte(4);
	write_byte(180); 
	write_byte(110);
	write_byte(30);
	write_byte(64);
	message_end();
	
	fireThink(ent);
}

public fireThink(ent){
	if(!pev_valid(ent)) return PLUGIN_CONTINUE;
		
	
	new frame = floatround(entity_get_float(ent, EV_FL_frame) + 1.0 ) % 4;
	entity_set_float(ent, EV_FL_frame, float(frame));
	new Float:fOrigin[3], iOrigin[3];
	
	entity_get_vector(ent, EV_VEC_origin, fOrigin);
	FVecIVec(fOrigin, iOrigin);
	
	makePou(iOrigin, sprite_pouFire);
	
	set_task(0.1, "fireThink", ent);
	
	return PLUGIN_CONTINUE;
	
}
public fireBallTouch(ent, toucher){
	if(!pev_valid(ent)) return PLUGIN_CONTINUE;
	 
	new szClass[18];
	entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass));
	
	new owner = entity_get_int(ent, EV_INT_iuser1);
	
	if(owner == toucher) return PLUGIN_CONTINUE;

	new Float:fOrigin[3], Float:fOriginTarget[3];
	entity_get_vector(ent, EV_VEC_origin, fOrigin);
	fOrigin[2] += 3.5;
	
	new minDmg	= 	( str_to_num( bonusClass[bonus_FIREBALL][4]) *  userHumanLevel[owner][human_MAG] ) / 2;
	new maxDmg	=	( str_to_num( bonusClass[bonus_FIREBALL][4]) *  userHumanLevel[owner][human_MAG] );
	new bool:isProDamage= didPro(owner, pro_MAGICFIRE);		
	new damageAll	=	(random_num(minDmg, maxDmg) + (isProDamage ? 100 : 0)) + floatround(damageClassClan(owner));
			
	for( new i = 1 ; i < maxPlayers;i ++ ){			
		
		if( !is_user_connected(i) || !is_user_alive(i)|| get_user_godmode(i) || i == owner || get_user_team(i) != 1) continue;			
		
		entity_get_vector(i, EV_VEC_origin, fOriginTarget);
		
		if( get_distance_f(fOrigin, fOriginTarget) < 150.0){
			
			if(userClass[i] != class_DEVIL){
				bb_set_in_fire(owner, i, 30);
			}
			addPro(owner, pro_FIRE,damageAll);
			
			if( damageAll >= get_user_health(i)) addPro(owner, pro_MAGICFIRE,  1);
			
			ExecuteHamB( Ham_TakeDamage, i, owner, owner, float(damageAll), DMG_BLAST );
			
		}
	}
	
	new sprite = sprite_expFireball;
	
	message_begin(MSG_BROADCAST ,SVC_TEMPENTITY);
	write_byte(TE_SPRITE);
	engfunc(EngFunc_WriteCoord,fOrigin[0]);
	engfunc(EngFunc_WriteCoord,fOrigin[1]);
	engfunc(EngFunc_WriteCoord,fOrigin[2]);
	write_short(sprite);
	write_byte(10);
	write_byte(255);
	message_end();
	makeLight(fOrigin,25, 255, 150, 56,15, 30);
	
	
	emitBonusSound(ent, bonus_sound_FIRE);	
			
	remove_entity(ent);
	return PLUGIN_CONTINUE;
}
public iceBoltCreate(id){

	new Float:fVelocity[3], Float:fOrigin[3], Float:fAngles[3];

	new ent = create_entity("info_target");
	
	entity_set_string(	ent, 	EV_SZ_classname, 	classIceBolt);
	
	entity_set_int(		ent, 	EV_INT_solid,		SOLID_TRIGGER);
	entity_set_int(		ent, 	EV_INT_movetype,	MOVETYPE_FLY);	
	entity_set_model(	ent,	modelIce);

	entity_get_vector(	id, 	EV_VEC_v_angle, 		fAngles);
	entity_get_vector(	id, 	EV_VEC_origin,		fOrigin);

	fOrigin[2] += 20.0;
	fAngles[0] *= -1.0;
	
	entity_set_vector(	ent, 	EV_VEC_origin,		fOrigin);
	entity_set_vector(	ent, 	EV_VEC_angles, 		fAngles);
	velocity_by_aim(	id,	2000,			fVelocity);

	entity_set_vector(	ent, 	EV_VEC_velocity, 	fVelocity);
	entity_set_int(		ent, 	EV_INT_rendermode, 	5);
	entity_set_float(	ent, 	EV_FL_renderamt, 	150.0);
	
	entity_set_int(		ent, 	EV_INT_iuser1,		id );
	
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte(22);
	write_short(ent);
	write_short(spriteBeam);
	write_byte(45);
	write_byte(4);
	write_byte(30);
	write_byte(110);
	write_byte(180); 
	write_byte(64);
	message_end();
	
	iceBoltThink(ent);
}

public iceBoltThink(ent){
	if(!pev_valid(ent)) return PLUGIN_CONTINUE;

	new Float:fOrigin[3], iOrigin[3];
	
	entity_get_vector(ent, EV_VEC_origin, fOrigin);
	FVecIVec(fOrigin, iOrigin);
	
	makePou(iOrigin, sprite_pouIce);

	set_task(0.1, "iceBoltThink", ent);
	
	return PLUGIN_CONTINUE;
	
}
public iceBoltTouch(ent, toucher){
	if( !pev_valid(ent) || ent == 0) return PLUGIN_CONTINUE;
		
	new szClass[18];
	entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass));
	
	if(!equal(szClass, classIceBolt)) return PLUGIN_CONTINUE;
	
	new owner = entity_get_int(ent, EV_INT_iuser1);
	
	if(owner == toucher) return PLUGIN_CONTINUE;
	
	entity_get_string(toucher, EV_SZ_classname, szClass, sizeof(szClass));
	if(toucher == ent) return PLUGIN_CONTINUE;
	
	new Float:fOrigin[3], Float:fOriginTarget[3];
	entity_get_vector(ent, EV_VEC_origin, fOrigin);
	fOrigin[2] += 3.5;
	
	new bool:isProDamage = didPro(owner,  pro_ICE);	
	new bool:isProTime = didPro(owner,  pro_ICE);
	
	new minDmg	= 	( str_to_num( bonusClass[bonus_ICEBOLT][4]) *  userHumanLevel[owner][human_ICEMAG] ) / 2;
	new maxDmg	=	( str_to_num( bonusClass[bonus_ICEBOLT][4]) *  userHumanLevel[owner][human_ICEMAG] );
	new damageAll 	= 	( random_num(minDmg, maxDmg) + (isProDamage ? 50 : 0)) + floatround(damageClassClan(owner) );
			
	for( new i = 1 ; i < maxPlayers;i ++ ){
					
		if( !is_user_connected(i) || !is_user_alive(i)|| get_user_godmode(i) || i == owner || get_user_team(i) != 1) continue;			
		
		entity_get_vector(i, EV_VEC_origin, fOriginTarget);
		
		
		if( get_distance_f(fOrigin, fOriginTarget) < 150.0){
			
				
			if(userClass[i] != class_SNOWMAN){
				bb_set_in_ice(owner, i, 3 + (isProTime ? 1 : 0 ));
			}
			
			addPro(owner, pro_ICEFIRE, damageAll);
			
			
			if( damageAll >= get_user_health(i)){
				addPro(owner, pro_ICE, 1);
			}
			
			ExecuteHamB( Ham_TakeDamage, i, owner, owner, float(damageAll), DMG_FREEZE );

		}
	}

	message_begin(MSG_BROADCAST ,SVC_TEMPENTITY);
	write_byte(TE_SPRITE);
	engfunc(EngFunc_WriteCoord,fOrigin[0]);
	engfunc(EngFunc_WriteCoord,fOrigin[1]);
	engfunc(EngFunc_WriteCoord,fOrigin[2]);
	write_short(sprite_expIceBolt);
	write_byte(10);
	write_byte(255);
	message_end();
	
	makeLight(fOrigin,25, 56, 150, 255,15, 30);
	
	emitBonusSound(ent, bonus_sound_ICEBOLT);	
			
	remove_entity(ent);
	return PLUGIN_CONTINUE;
}

public returnClosestOne(id, distMax, team){
	new Float:minDist=99999.9;
	new Float:dist=0.0;
	new Float:fOrigin[3], Float:fOriginTarget[3];
	pev(id, pev_origin, fOrigin);
	new idClosest=-1;
	for( new i = 1; i<maxPlayers; i ++ ){
		if( i == id ) continue;
		if( !is_user_connected(i) || !is_user_alive(i) ) continue;
		if( get_user_team(i) != team ) continue;
		if(bb_is_in_barrier(i) || get_user_godmode(i)) continue;
		
		pev(i, pev_origin, fOriginTarget);
		dist = get_distance_f(fOriginTarget, fOrigin);
		if( dist > float(distMax) ) continue;
		
		if( dist < minDist ){
			minDist=dist;
			idClosest=i;
		}
	}
	return idClosest;
}

public bombCreate(id){
	new Float:fVelocity[3], Float:fOrigin[3];

	new ent = create_entity("info_target");
	
	entity_set_string(	ent, 	EV_SZ_classname, 	classBomb);
	
	entity_set_int(		ent, 	EV_INT_solid,		SOLID_TRIGGER);
	entity_set_int(		ent, 	EV_INT_movetype,	MOVETYPE_BOUNCE);	

	entity_set_model(	ent, 	modelBomb);
	entity_set_float(	ent, 	EV_FL_gravity,		0.9);
	entity_get_vector(	id, 	EV_VEC_origin,		fOrigin);
	fOrigin[2] += 5;
	
	entity_set_vector(	ent, 	EV_VEC_origin,		fOrigin);
	velocity_by_aim(	id,	350,			fVelocity);	
	
	fVelocity[2] += 50.0;
	
	entity_set_vector(	ent, 	EV_VEC_velocity, 	fVelocity);

	entity_set_float(	ent,	 EV_FL_fuser1,		get_gametime() );
	entity_set_float(	ent, 	EV_FL_fuser3,		get_gametime() );
	entity_set_float(	ent, 	EV_FL_fuser4,		3.0);

	entity_set_int(		ent, 	EV_INT_iuser1,		id );
	set_rendering(		ent, 	kRenderFxGlowShell, 	255, 0, 0, 	kRenderNormal, 	5);
	
	bombThink(ent);
}
public bombThink(ent){
	if( !pev_valid(ent) ) return;

	new Float:percent = ((get_gametime()-entity_get_float(ent, EV_FL_fuser1)	)/entity_get_float(ent, EV_FL_fuser4));
	
	new Float:fVelocity[3];
	
	entity_get_vector(	ent, 	EV_VEC_velocity,	fVelocity);
	new Float:fOrigin[3] = 0.0;
	pev(ent, 118, fOrigin);
	new closest = returnClosestOne(ent, 500, 1);
	if (closest != -1){
					
		new Float:fOriginDirect[3] = 0.0;
		pev(closest, 118, fOriginDirect);
		new Float:fVelocityStart[3] = 0.0;
		pev(ent, 120, fVelocityStart);
		new Float:fVeloc[3] = 0.0;
		xs_vec_sub(fOriginDirect, fOrigin, fVeloc);
		xs_vec_normalize(fVeloc, fVeloc);
		xs_vec_mul_scalar(fVeloc, 1200.0, fVeloc);
		fVeloc[2] = fVelocityStart[2];
		set_pev(ent, 120, fVeloc);
		set_pev(ent, 103, closest);
	}
	
	entity_set_vector(	ent, 	EV_VEC_velocity,	fVelocity);
	if( percent > 1.0 ) bombExplode(ent);	
	else set_task(0.1, "bombThink", ent);
}
public bombExplode(ent){
	if( !pev_valid(ent) || ent == 0) return PLUGIN_CONTINUE;
		
	new szClass[18];
	entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass));

	new owner = entity_get_int(ent, EV_INT_iuser1);

	new Float:fOrigin[3], Float:fOriginTarget[3];
	entity_get_vector(ent, EV_VEC_origin, fOrigin);
	fOrigin[2] += 3.5;
	new dmg = (str_to_num( bonusClass[bonus_BOMB][4]) *  userHumanLevel[owner][human_BOOMBERMA]) + floatround(damageClassClan(owner));
	for( new i = 1 ; i < maxPlayers;i ++ ){
					
		if( !is_user_connected(i) || !is_user_alive(i)|| get_user_godmode(i) || i == owner || get_user_team(i) != 1) continue;		
		
		entity_get_vector(i, EV_VEC_origin, fOriginTarget);
		
		
		if( get_distance_f(fOrigin, fOriginTarget) < 150.0){
			
			ExecuteHamB( Ham_TakeDamage, i, owner, owner, float(dmg), DMG_MORTAR );
			
		}
	}

	message_begin(MSG_BROADCAST ,SVC_TEMPENTITY);
	write_byte(TE_SPRITE);
	engfunc(EngFunc_WriteCoord,fOrigin[0]);
	engfunc(EngFunc_WriteCoord,fOrigin[1]);
	engfunc(EngFunc_WriteCoord,fOrigin[2]);
	write_short(sprite_expFireball);
	write_byte(10);
	write_byte(255);
	message_end();
	
	message_begin(MSG_BROADCAST,SVC_TEMPENTITY); 
	write_byte(TE_EXPLOSION2); 
	engfunc(EngFunc_WriteCoord,fOrigin[0]);
	engfunc(EngFunc_WriteCoord,fOrigin[1]);
	engfunc(EngFunc_WriteCoord,fOrigin[2]);
	write_byte(185); 
	write_byte(10); 
	message_end();
	
	makeLight(fOrigin,25, 255, 0, 0,15, 30);

	remove_entity(ent);
	return PLUGIN_CONTINUE;
}
public bombTouch(ent, toucher){
	if( !pev_valid(ent) || ent == 0) return PLUGIN_CONTINUE;
		
	new szClass[18];
	entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass));
	
	if(equal(szClass, classBomb) && !(get_user_team(toucher) == 1))
		return PLUGIN_CONTINUE;
	
	new owner = entity_get_int(ent, EV_INT_iuser1);
	
	if(owner == toucher) return PLUGIN_CONTINUE;
	
	if (equal(szClass, "player")){
		if (get_user_team(owner) == get_user_team(toucher)) return PLUGIN_CONTINUE;
		
		bombExplode(ent);
	} else {
		if (0 >= pev(ent, 100))
			bombExplode(ent);
		
	}
	
	bombExplode(ent);
	
	return PLUGIN_CONTINUE;
}
public bombTrapCreate(id,Float: fOrigin[3]){

	new ent = create_entity("info_target");
	
	entity_set_string(	ent, 	EV_SZ_classname, 	classbombTrap);
	
	entity_set_int(		ent, 	EV_INT_movetype, 	MOVETYPE_FLY);
	entity_set_int(		ent, 	EV_INT_solid, 		SOLID_NOT);
	entity_set_model(	ent, 	modelTrapBomb);
	
	fOrigin[2] 	-= 	distanceToFloor(fOrigin)-7;
	
	entity_set_vector(	ent, 	EV_VEC_origin,		fOrigin);
	set_rendering(		ent, 	kRenderFxGlowShell, 	255, 56, 56, 	kRenderNormal, 	3);
	entity_set_int(		ent, 	EV_INT_iuser1,		id);
	
	set_task(0.2, "bombTrapThink", ent);
	bombTrapEffect(ent);
}

public bombTrapEffect(ent){
	if( !pev_valid(ent)) return PLUGIN_CONTINUE;
	
	new Float:fOrigin[3];
	new iOrigin[3];
	entity_get_vector(ent, EV_VEC_origin, fOrigin);
	
	FVecIVec(fOrigin, iOrigin);
	makePou(iOrigin, sprite_pouFire);
	makeLight(fOrigin,25, 255, 56, 56,15, 30);
	
	if (pev_valid(ent)) set_task(1.0, "bombTrapEffect", ent);
	
	return PLUGIN_CONTINUE;	
}
public bombTrapThink(ent){
	if( !pev_valid(ent)) return PLUGIN_CONTINUE;

	new szClass[18];
	entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass));
	
	if(!equal(szClass, classbombTrap)) return PLUGIN_CONTINUE;
	
	new owner = entity_get_int(ent, EV_INT_iuser1);
	
	new Float:fOrigin[3];
	new Float:fOriginTarget[3];
	entity_get_vector(ent, EV_VEC_origin, fOrigin);
		
	if(gameTime  &&  entity_get_int(ent, EV_INT_iuser1)){
		
		new bool:closeTrap;

		for(new i = 1; i < maxPlayers; i++){
			
			if(!is_user_alive(i) || !is_user_connected(i) || get_user_team(i) != 1 || i == owner) continue;
					
			entity_get_vector(i, EV_VEC_origin, fOriginTarget);
			if (get_distance_f(fOrigin, fOriginTarget) < 40.0){
				closeTrap = true;
			}
		}
		if(closeTrap) trapBomberExplode(ent);
	}
	if (pev_valid(ent)) set_task(0.1, "bombTrapThink", ent);
	
	return PLUGIN_CONTINUE;
}
public trapBomberExplode(ent){
	if( !pev_valid(ent) || ent == 0)
		return PLUGIN_CONTINUE;
		
	new szClass[18];
	entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass));

	new owner = entity_get_int(ent, EV_INT_iuser1);

	new Float:fOrigin[3], Float:fOriginTarget[3];
	entity_get_vector(ent, EV_VEC_origin, fOrigin);
	fOrigin[2] += 3.5;
	new dmg =(str_to_num(paramClassesHuman[human_BOOMBERMA][1]) *  userHumanLevel[owner][human_BOOMBERMA]) + floatround(damageClassClan(owner));
	for( new i = 1 ; i < maxPlayers;i ++ ){
					
		if( !is_user_connected(i) || !is_user_alive(i)|| get_user_godmode(i) || i == owner || get_user_team(i) != 1) continue;			
		
		entity_get_vector(i, EV_VEC_origin, fOriginTarget);
	
		if( get_distance_f(fOrigin, fOriginTarget) < 150.0){
			ExecuteHamB( Ham_TakeDamage, i, owner, owner, float(dmg), DMG_MORTAR );
		}
	}
	message_begin(MSG_BROADCAST ,SVC_TEMPENTITY);
	write_byte(TE_SPRITE);
	engfunc(EngFunc_WriteCoord,fOrigin[0]);
	engfunc(EngFunc_WriteCoord,fOrigin[1]);
	engfunc(EngFunc_WriteCoord,fOrigin[2]);
	write_short(sprite_expFireball);
	write_byte(10);
	write_byte(255);
	message_end();
	
	message_begin(MSG_BROADCAST,SVC_TEMPENTITY); 
	write_byte(TE_EXPLOSION2); 
	engfunc(EngFunc_WriteCoord,fOrigin[0]);
	engfunc(EngFunc_WriteCoord,fOrigin[1]);
	engfunc(EngFunc_WriteCoord,fOrigin[2]);
	write_byte(185); 
	write_byte(10); 
	message_end();
	
	makeLight(fOrigin,25, 255, 0, 0,15, 30);

	remove_entity(ent);
	return PLUGIN_CONTINUE;
}



public Float:damageClassClan(id){
	new Float:damage;
	
	if(clan[id]){
		if(get_clan_info(clan[id], CLAN_DAMAGECLASS)){
			new userLevelAdd = 5;
			damage += get_clan_info(clan[id], CLAN_DAMAGECLASS) * userLevelAdd;
		}
	}
	return damage;
}
public rocketCreate(id){

	new Float:fTime = 10.0;

	new Float:fOrigin[3];
	entity_get_vector(	id, EV_VEC_origin, 	fOrigin);
	
	new ent = create_entity("info_target");
	entity_set_string(	ent, EV_SZ_classname, 	rocketClass);
	entity_set_int(		ent, EV_INT_movetype,	MOVETYPE_FLY);
	entity_set_int(		ent, EV_INT_solid,	SOLID_TRIGGER);
	entity_set_int(		ent, EV_INT_iuser1,	id);
	entity_set_float(	ent, EV_FL_fuser1,	get_gametime());
	entity_set_float(	ent, EV_FL_fuser2,	fTime);
	
	entity_set_model(	ent, modelRocket);
	entity_set_vector(	ent, EV_VEC_origin, 	fOrigin);	
	
	
	new camera = create_entity("info_target");
	entity_set_string(	camera, EV_SZ_classname, 	"cameraRocket");
	entity_set_int(		camera, EV_INT_movetype,		MOVETYPE_FLY);
	entity_set_int(		camera, EV_INT_solid,		SOLID_TRIGGER);
	entity_set_int(		camera, EV_INT_iuser1,		id);
	
	set_rendering(		camera, kRenderFxNone, 255, 0 ,0, kRenderTransAlpha, 0);
	entity_set_model(	camera, modelRocket);
	entity_set_vector(	camera, EV_VEC_origin, 		fOrigin);
		
	
	userRocketItem[id] 	= 	ent;
	userRocketCamera[id]	=	camera;
	
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte(22) ;
	write_short(ent); 
	write_short(spriteBeam);
	write_byte(45);
	write_byte(4);
	write_byte(255);
	write_byte(30);
	write_byte(30);
	write_byte(64);
	message_end();
	

	message_begin(MSG_ONE_UNRELIABLE, get_user_msgid("BarTime"), .player=id);
	write_short(floatround(fTime));
	message_end();
	
	
	set_rendering(ent, kRenderFxGlowShell, 255, 0 ,0, kRenderNormal, 10);
	
	attach_view(id, camera);
	if( task_exists(id+TASK_EXPLODETORPED) )
		remove_task(id+TASK_EXPLODETORPED);
	set_task(fTime, "explodeRocket", id+TASK_EXPLODETORPED);

}
public explodeRocket(id){
	id -= TASK_EXPLODETORPED;
	
	new ent = userRocketItem[id];
	if( !pev_valid(ent) || ent == 0 ) return;
	
	addPro(id, pro_TORPEDE, 1);
	
	new Float:fOrigin[3];
	entity_get_vector(ent, EV_VEC_origin, fOrigin);
	message_begin(MSG_BROADCAST ,SVC_TEMPENTITY);
	write_byte(TE_EXPLOSION);
	engfunc(EngFunc_WriteCoord,fOrigin[0]);
	engfunc(EngFunc_WriteCoord,fOrigin[1]);
	engfunc(EngFunc_WriteCoord,fOrigin[2]+40.0);
	write_short(sprite_expFireball);
	write_byte(20);
	write_byte(20);
	write_byte(0);
	message_end();

	
	new Float:fOriginTarget[3];
	entity_get_vector(ent, EV_VEC_origin, fOrigin);
	
	new bool:isProDamage = didPro(id, pro_TORPEDE);
	
	new minDmg	= 	( str_to_num( bonusClass[bonus_ROCKET][4]) *  userHumanLevel[id][human_SEARCH] ) / 2;
	new maxDmg	=	( str_to_num( bonusClass[bonus_ROCKET][4]) *  userHumanLevel[id][human_SEARCH] );
	new damageAll 	= 	(random_num(minDmg, maxDmg) + (isProDamage ? 100 : 0) + floatround(damageClassClan(id)));
			
			
	for( new i = 1 ; i < maxPlayers;i ++ ){
					
		if( !is_user_connected(i) || !is_user_alive(i)|| get_user_godmode(i) || i == id || get_user_team(i) != 1) continue;	
		
		entity_get_vector(i, EV_VEC_origin, fOriginTarget);
		
		if( get_distance_f(fOrigin, fOriginTarget) < 150.0){

			ExecuteHamB( Ham_TakeDamage, i, id, id, float(damageAll), DMG_BLAST );
			
			
		}
	}
	
	releaseView(id);
	remove_entity(ent);
}
public releaseView(id){
	if( userRocketItem[id] != 0 ){
		message_begin(MSG_ONE_UNRELIABLE, get_user_msgid("BarTime"), .player=id);
		write_short(0);
		message_end();
		attach_view(id, id);
		userRocketItem[id] = 0;
	}
}
public rocketTouch(ent, id){
	if( !pev_valid(ent) ) return;
	if( get_gametime() - entity_get_float( ent, EV_FL_fuser1 ) < 0.5)  return;		
		
	explodeRocket(entity_get_int(ent, EV_INT_iuser1)+TASK_EXPLODETORPED);
	return;
}
public moveRocket(id){
	
	new tempId[2];
	tempId[0] = userRocketItem[id];
	tempId[1] = userRocketCamera[id];
	
	if( tempId[0] != 0 ){
		if(pev_valid(tempId[0]) ){
			new Float:fAngles[3];
			entity_get_vector(id,		EV_VEC_angles, fAngles);
			entity_set_vector(tempId[1],	EV_VEC_angles, fAngles);
			
			new Float:fRocketAngles[3];
			entity_get_vector(tempId[0],	EV_VEC_angles, fRocketAngles);
			fRocketAngles[2] += 5.0;
			fAngles[2] = fRocketAngles[2];
			entity_set_vector(tempId[0],	EV_VEC_angles, fAngles);
			
			new Float:fVelocity[3];
			velocity_by_aim(id, 	450, 	fVelocity);
			entity_set_vector(tempId[0], 	EV_VEC_velocity, 	fVelocity);

			new Float:fOrigin[3];
			entity_get_vector(tempId[0], EV_VEC_origin, fOrigin);
			
			fOrigin[0] -= 110.0*floatcos(fAngles[1], degrees);
			fOrigin[1] -= 110.0*floatsin(fAngles[1], degrees);
			fOrigin[2] -= 30.0*floatsin(fAngles[1], degrees);
			
			entity_set_origin(tempId[1], fOrigin);
		}
	}
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n{\\ colortbl ;\\ red0\\ green0\\ blue0;}\n\\ viewkind4\\ uc1\\ pard\\ cf1\\ lang1045\\ b\\ f0\\ fs16 \n\\ par }
*/
