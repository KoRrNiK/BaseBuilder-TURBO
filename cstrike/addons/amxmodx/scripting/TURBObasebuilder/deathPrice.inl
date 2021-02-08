#include <	amxmodx		>
#include <	amxmisc		>
#include <	cstrike		>
#include <	fun		>
#include <	fakemeta	>
#include <	fakemeta_util	>
#include <	engine		>
#include <	csx		>
#include <	colorchat	>


public resetPriceDefault(id){
	userDeathPrice[id][PRICE_DEATH] = 0;
	userDeathPrice[id][PRICE_CLASS] = class_TOTAL;
	userDeathPrice[id][PRICE_GOD] = false;
	userDeathPrice[id][PRICE_TIME] = 0;
	userDeathPrice[id][PRICE_BUYGOD] = false;
	userDeathPrice[id][PRICE_LOST] = false;	
	userDeathPrice[id][PRICE_START] = false;
}

public deathPriceMenu(id){
	
	if(get_user_team(id) != 2){
		ColorChat(id, GREEN, "---^x01 Musisz byc budowniczym aby placic za swoja smierc!^x04 ---");
		return
	}	
	if(!gameTime){
		ColorChat(id, GREEN, "---^x01 Dostepne tylko podczas rundy!^x04 ---");
		return
	}	
	if(userDeathPrice[id][PRICE_LOST]){
		ColorChat(id, GREEN, "---^x01 Dostepne w nastepnej rundzie! Zginales!^x04 ---");
		return;
	}	
	if(userDeathPrice[id][PRICE_START]){
		ColorChat(id, RED, "^x04---^x03 Placisz juz w tej rundzie!^x04 ---");
		new gText[160], iLen = 0;
	
		iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x04---");
		iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x03 %s Brylek^x04 |", formatNumber(userDeathPrice[id][PRICE_DEATH]));
		iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x03 %s^x04 |", !userDeathPrice[id][PRICE_GOD] ? "Zakaz Godowania" : "Pozwolone Godowanie");
		if(userDeathPrice[id][PRICE_CLASS] == class_TOTAL)
			iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x03 Wszystkie Zombie^x04 |");
		else iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x03 Zombie: %s^x04 |", classesZombies[userDeathPrice[id][PRICE_CLASS]][0]);
		if(userDeathPrice[id][PRICE_TIME] == 0)
			iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x03 Czas: Cala Runda");
		else iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x03 Czas: %d:%s%d",userDeathPrice[id][PRICE_TIME]/60, (userDeathPrice[id][PRICE_TIME]%60 < 10 ? "0" : ""), userDeathPrice[id][PRICE_TIME]%60)
		iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x04 ---");
			
		ColorChat(id, RED, "%s",gText);
	
		
		return;
	}	
	
	new menu = menu_create("\r[BaseBuilder]\y Plac za swoja Smierc!^n\r*\d Jesli wyjdziesz podczas placenia: Stracisz swoje Brylki!", "deathPriceMenu_2");
	
	if(userDeathPrice[id][PRICE_DEATH] == 0)
	menu_additem(menu, "\yIle place:\d Wpisz ilosc")
	else if(userDeathPrice[id][PRICE_DEATH] > userNugget[id])
		menu_additem(menu, formatm("\yIle place:\w %s Brylek\r ( Nie posiadasz tylu brylek! )", formatNumber(userDeathPrice[id][PRICE_DEATH])))
	else menu_additem(menu, formatm("\yIle place:\w %s Brylek", formatNumber(userDeathPrice[id][PRICE_DEATH])))
	
	if(userDeathPrice[id][PRICE_CLASS] == class_TOTAL)
		menu_additem(menu, "\yJakie Zombi:\r Wszystkie");
	else menu_additem(menu, formatm("\yJakie Zombi:\w %s", classesZombies[userDeathPrice[id][PRICE_CLASS]][0]));
	
	menu_additem(menu, formatm("\yGodowanie:\w %s", !userDeathPrice[id][PRICE_GOD] ? "Zakazane" : "Pozwolone"))
	
	if(userDeathPrice[id][PRICE_TIME] == 0)
		menu_additem(menu, "\yCzas trawnia:\r Cala Runda^n")
	else if(userDeathPrice[id][PRICE_TIME] > gTime)
		menu_additem(menu, formatm("\yCzas trawnia:\w %d:%s%d\r ( Minal juz ten czas | Zmien go! )^n",userDeathPrice[id][PRICE_TIME]/60, (userDeathPrice[id][PRICE_TIME]%60 < 10 ? "0" : ""), userDeathPrice[id][PRICE_TIME]%60))
	else menu_additem(menu, formatm("\yCzas trawnia:\w %d:%s%d^n",userDeathPrice[id][PRICE_TIME]/60, (userDeathPrice[id][PRICE_TIME]%60 < 10 ? "0" : ""), userDeathPrice[id][PRICE_TIME]%60))
	
	
	menu_additem(menu, "\rZATWIERDZ USTAWIENIA")
	
	menu_display(id, menu, 0);
}


public deathPriceMenu_2(id, menu, item){
	
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return;
	}
	
	switch(item){
		
		case 0:{
			cmd_execute(id, "messagemode IlePlaceZaZgona")
		}
		case 1:{
			if (++userDeathPrice[id][PRICE_CLASS] >= sizeof(classesZombies)+1) {
				userDeathPrice[id][PRICE_CLASS] = 0;
			}
		}
		case 2:{
			userDeathPrice[id][PRICE_GOD] =! userDeathPrice[id][PRICE_GOD];
		}
		case 3:{
		
			cmd_execute(id, "messagemode DoKiedyMozeZabic")
			
		}
		case 4:{
			
			if(userDeathPrice[id][PRICE_DEATH] == 0){
				ColorChat(id, GREEN, "---^x01 Wpisz ile chcesz placic!^x04 ---");
			} else if(userDeathPrice[id][PRICE_DEATH] > userNugget[id]){
				ColorChat(id, GREEN, "---^x01 Nie posiadasz tylu brylek!^x04 ---");
			} else if(userDeathPrice[id][PRICE_TIME] > gTime) {
				ColorChat(id, GREEN, "---^x01 Zmien czas! Runda bedzie trwac juz krocej!^x04 ---");
			} else {
				userNugget[id] -= userDeathPrice[id][PRICE_DEATH]
				userDeathPrice[id][PRICE_START] = true;
				
				new gText[157], iLen = 0;
				
				ColorChat(0, RED, "^x04---^x03 Gracz^x04 %s^x03 wyznacza nagrode za zabicie jego!^x04 ---", userName[id]);
				
				iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x04---");
				iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x03 %s Brylek^x04 |", formatNumber(userDeathPrice[id][PRICE_DEATH]));
				iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x03 %s^x04 |", !userDeathPrice[id][PRICE_GOD] ? "Zakaz Godowania" : "Pozwolone Godowanie");
				if(userDeathPrice[id][PRICE_CLASS] == class_TOTAL)
					iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x03 Wszystkie Zombie^x04 |");
				else iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x03 Zombie: %s^x04 |", classesZombies[userDeathPrice[id][PRICE_CLASS]][0]);
				if(userDeathPrice[id][PRICE_TIME] == 0)
					iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x03 Czas: Cala Runda");
				else iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x03 Czas: %d:%s%d",userDeathPrice[id][PRICE_TIME]/60, (userDeathPrice[id][PRICE_TIME]%60 < 10 ? "0" : ""), userDeathPrice[id][PRICE_TIME]%60)
				iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^x04 ---");
			
				ColorChat(0, RED, "%s",gText);
				return;
			}
		}
	}
	deathPriceMenu(id);
	
}

public deathPlayerPrice(attacker, victim){
	if(get_user_team(attacker) == 1 && get_user_team(victim) == 2){
		if(userDeathPrice[victim][PRICE_START]){
			if((!userDeathPrice[victim][PRICE_GOD]) || (!userDeathPrice[attacker][PRICE_BUYGOD] && userDeathPrice[victim][PRICE_GOD])){
				if((userDeathPrice[victim][PRICE_CLASS] == userClass[attacker]) || (userDeathPrice[victim][PRICE_CLASS] == class_TOTAL)){
					if((userDeathPrice[victim][PRICE_TIME] == 0) || userDeathPrice[victim][PRICE_TIME] < gTime){
						ColorChat(0, GREEN, "---^x01 Brawo!^x03 %s^x01 zabil^x03 %s^x01 otrzymal za to:^x03 %s Brylek^x04 ---", userName[attacker], userName[victim], formatNumber(userDeathPrice[victim][PRICE_DEATH]));
						userDeathPrice[victim][PRICE_LOST] = true;
						userNugget[attacker] += userDeathPrice[victim][PRICE_DEATH]
					} else deathPlayerWin(victim)
				} else deathPlayerWin(victim)
			} else deathPlayerWin(victim)
		}
	}
}
public deathPlayerWin(id){
	if(userDeathPrice[id][PRICE_START] && !userDeathPrice[id][PRICE_LOST]){
		userDeathPrice[id][PRICE_LOST] = true;
		userNugget[id] += userDeathPrice[id][PRICE_DEATH]
		ColorChat(id, GREEN, "---^x01 Otrzymales swoje^x03 brylki^x01 ktore postawiles za swoje zycie!^x04 ---");
	}	
}	

public priceDeath(id){
	
	new szArg[9]
	read_argv(1, szArg, sizeof(szArg))
	remove_quotes(szArg)
	new value = abs(str_to_num(szArg))
	
	
	
	userDeathPrice[id][PRICE_DEATH] = value
	
	deathPriceMenu(id)
}
public timeDeath(id){
	
	new szArg[9]
	read_argv(1, szArg, sizeof(szArg))
	remove_quotes(szArg)
	new value = abs(str_to_num(szArg))
	
	userDeathPrice[id][PRICE_TIME] = value % gTime
	
	deathPriceMenu(id)
}

/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
