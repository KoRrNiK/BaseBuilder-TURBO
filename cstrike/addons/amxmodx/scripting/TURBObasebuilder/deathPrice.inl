#include <	amxmodx		>
#include <	amxmisc		>
#include <	cstrike		>
#include <	fun		>
#include <	fakemeta	>
#include <	fakemeta_util	>
#include <	engine		>
#include <	csx		>


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
	
	if(!is_user_connected(id)) return;
	
	if(get_user_team(id) != USER_HUMAN){
		chatPrint(id, PREFIX_LINE, "Musisz byc budowniczym aby placic za swoja smierc!");
		return;
	}	
	if(!gameTime){
		chatPrint(id, PREFIX_LINE, "Dostepne tylko podczas rundy!");
		return;
	}	
	if(userDeathPrice[id][PRICE_LOST]){
		chatPrint(id, PREFIX_LINE, "Dostepne w nastepnej rundzie! Zginales!");
		return;
	}	
	if(userDeathPrice[id][PRICE_START]){
		chatPrint(id, PREFIX_LINE, "Placisz juz w tej rundzie!");
		new gText[160], iLen = 0;
	
		iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^3 %s Brylek^4 |", formatNumber(userDeathPrice[id][PRICE_DEATH]));
		iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^3 %s^4 |", !userDeathPrice[id][PRICE_GOD] ? "Zakaz Godowania" : "Pozwolone Godowanie");
		if(userDeathPrice[id][PRICE_CLASS] == class_TOTAL)
			iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^3 Wszystkie Zombie^4 |");
		else iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^3 Zombie: %s^4 |", classesZombies[userDeathPrice[id][PRICE_CLASS]][0]);
		if(userDeathPrice[id][PRICE_TIME] == 0)
			iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^3 Czas: Cala Runda");
		else iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^3 Czas: %d:%s%d",userDeathPrice[id][PRICE_TIME]/60, (userDeathPrice[id][PRICE_TIME]%60 < 10 ? "0" : ""), userDeathPrice[id][PRICE_TIME]%60);
			
		chatPrint(id, PREFIX_LINE, "%s",gText);
		return;
	}	
	new menu = menu_create("\r[BaseBuilder]\y Plac za swoja Smierc!^n\r*\d Jesli wyjdziesz podczas placenia: Stracisz swoje Brylki!", "deathPriceMenu_2");
	
	if(userDeathPrice[id][PRICE_DEATH] == 0)
	menu_additem(menu, "\yIle place:\d Wpisz ilosc");
	else if(userDeathPrice[id][PRICE_DEATH] > userNugget[id])
		menu_additem(menu, formatm("\yIle place:\w %s Brylek\r ( Nie posiadasz tylu brylek! )", formatNumber(userDeathPrice[id][PRICE_DEATH])));
	else menu_additem(menu, formatm("\yIle place:\w %s Brylek", formatNumber(userDeathPrice[id][PRICE_DEATH])));
	
	if(userDeathPrice[id][PRICE_CLASS] == class_TOTAL)
		menu_additem(menu, "\yJakie Zombi:\r Wszystkie");
	else menu_additem(menu, formatm("\yJakie Zombi:\w %s", classesZombies[userDeathPrice[id][PRICE_CLASS]][0]));
	
	menu_additem(menu, formatm("\yGodowanie:\w %s", !userDeathPrice[id][PRICE_GOD] ? "Zakazane" : "Pozwolone"));
	
	if(userDeathPrice[id][PRICE_TIME] == 0)
		menu_additem(menu, "\yCzas trawnia:\r Cala Runda^n");
	else if(userDeathPrice[id][PRICE_TIME] > gTime)
		menu_additem(menu, formatm("\yCzas trawnia:\w %d:%s%d\r ( Minal juz ten czas | Zmien go! )^n",userDeathPrice[id][PRICE_TIME]/60, (userDeathPrice[id][PRICE_TIME]%60 < 10 ? "0" : ""), userDeathPrice[id][PRICE_TIME]%60));
	else menu_additem(menu, formatm("\yCzas trawnia:\w %d:%s%d^n",userDeathPrice[id][PRICE_TIME]/60, (userDeathPrice[id][PRICE_TIME]%60 < 10 ? "0" : ""), userDeathPrice[id][PRICE_TIME]%60));
	
	menu_additem(menu, "\rZATWIERDZ USTAWIENIA");
	
	menu_display(id, menu, 0);
}
public deathPriceMenu_2(id, menu, item){
	
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return;
	}
	switch(item){
		case 0: cmd_execute(id, "messagemode IlePlaceZaZgona");
		case 1:{
			if (++userDeathPrice[id][PRICE_CLASS] >= sizeof(classesZombies)+1) {
				userDeathPrice[id][PRICE_CLASS] = 0;
			}
		}
		case 2:{
			userDeathPrice[id][PRICE_GOD] =! userDeathPrice[id][PRICE_GOD];
		}
		case 3: cmd_execute(id, "messagemode DoKiedyMozeZabic");
		case 4:{
			
			if(userDeathPrice[id][PRICE_DEATH] == 0){
				chatPrint(id, PREFIX_LINE, "Wpisz ile chcesz placic!");
			} else if(userDeathPrice[id][PRICE_DEATH] > userNugget[id]){
				chatPrint(id, PREFIX_LINE, "Nie posiadasz tylu brylek!");
			} else if(userDeathPrice[id][PRICE_TIME] > gTime) {
				chatPrint(id, PREFIX_LINE, "Zmien czas! Runda bedzie trwac juz krocej!");
			} else {
				userNugget[id] -= userDeathPrice[id][PRICE_DEATH];
				userDeathPrice[id][PRICE_START] = true;
				
				new gText[157], iLen = 0;
				
				chatPrint(0, PREFIX_LINE, "Gracz^4 %s^3 wyznacza nagrode za zabicie jego!", userName[id]);
				
				iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^3 %s Brylek^4 |", formatNumber(userDeathPrice[id][PRICE_DEATH]));
				iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^3 %s^4 |", !userDeathPrice[id][PRICE_GOD] ? "Zakaz Godowania" : "Pozwolone Godowanie");
				if(userDeathPrice[id][PRICE_CLASS] == class_TOTAL)
					iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^3 Wszystkie Zombie^4 |");
				else iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^3 Zombie: %s^4 |", classesZombies[userDeathPrice[id][PRICE_CLASS]][0]);
				if(userDeathPrice[id][PRICE_TIME] == 0)
					iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^3 Czas: Cala Runda");
				else iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^3 Czas: %d:%s%d",userDeathPrice[id][PRICE_TIME]/60, (userDeathPrice[id][PRICE_TIME]%60 < 10 ? "0" : ""), userDeathPrice[id][PRICE_TIME]%60);
			
				chatPrint(0, PREFIX_LINE, "%s",gText);
				return;
			}
		}
	}
	deathPriceMenu(id);
}

public deathPlayerPrice(attacker, victim){
	if(get_user_team(attacker) == USER_ZOMBIE && get_user_team(victim) == USER_HUMAN){
		if(userDeathPrice[victim][PRICE_START]){
			if((!userDeathPrice[victim][PRICE_GOD]) || (!userDeathPrice[attacker][PRICE_BUYGOD] && userDeathPrice[victim][PRICE_GOD])){
				if((userDeathPrice[victim][PRICE_CLASS] == userClass[attacker]) || (userDeathPrice[victim][PRICE_CLASS] == class_TOTAL)){
					if((userDeathPrice[victim][PRICE_TIME] == 0) || userDeathPrice[victim][PRICE_TIME] < gTime){
						chatPrint(0, PREFIX_LINE, "Brawo!^3 %s^1 zabil^3 %s^1 otrzymal za to:^3 %s Brylek", userName[attacker], userName[victim], formatNumber(userDeathPrice[victim][PRICE_DEATH]));
						userDeathPrice[victim][PRICE_LOST] = true;
						userNugget[attacker] += userDeathPrice[victim][PRICE_DEATH];
					} else deathPlayerWin(victim);
				} else deathPlayerWin(victim);
			} else deathPlayerWin(victim);
		}
	}
}
public deathPlayerWin(id){
	if(userDeathPrice[id][PRICE_START] && !userDeathPrice[id][PRICE_LOST]){
		userDeathPrice[id][PRICE_LOST] = true;
		userNugget[id] += userDeathPrice[id][PRICE_DEATH];
		chatPrint(id, PREFIX_LINE, "Otrzymales swoje^3 brylki^1 ktore postawiles za swoje zycie!");
	}	
}	
public priceDeath(id){
	new szArg[9];
	read_argv(1, szArg, sizeof(szArg));
	remove_quotes(szArg);
	new value = abs(str_to_num(szArg));
	userDeathPrice[id][PRICE_DEATH] = value;
	deathPriceMenu(id);
}
public timeDeath(id){
	new szArg[9];
	read_argv(1, szArg, sizeof(szArg));
	remove_quotes(szArg);
	new value = abs(str_to_num(szArg));
	userDeathPrice[id][PRICE_TIME] = value % gTime;
	deathPriceMenu(id);
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
