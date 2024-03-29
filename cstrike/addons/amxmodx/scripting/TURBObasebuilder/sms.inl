#include <amxmodx>
#include <amxmisc>
#include <http2>

public smsMainMenu(id){
	
	if( !playerLogged(id) ){
		chatPrint(id, PREFIX_NORMAL, "Zaloguj sie aby moc korzystac z tego menu!");
		return PLUGIN_HANDLED;
	}
	
	new gText[512];
	new iLen;
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "Witaj w sklepie\r %s", userName[id]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^nAby kupowac w sklepie musisz pierw doladowac swoje Luzaczki.");
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^nLuzaczki mozesz jeszcze zdobyc z\y /nagroda.\r Powodzenia!");
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\y1.\w Doladuj swoje\r Luzaczki\w !");		
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\y2.\w Kupuj za\r Luzaczki\w !");	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\y3.\w Twoje Uslugi\w !");
	
	if( isSuperAdmin(id)){
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\y6.\w Dodaj\r Waluty\w!^n");
	}
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\y0.\w Wyjscie");	
	showMenu(id, B1 | B2 | B3 | B6 | B0  , gText, -1, "smsMainMenu"  );
	return PLUGIN_HANDLED;
}
public smsMainMenu_2(id, item){
	switch(item){
		case 0: addCash(id);
		case 1: luzaczkiShop(id);
		case 2: yourBuyService(id);	
		case 5:{
			if(!isSuperAdmin(id)){
				smsMainMenu(id);
				return PLUGIN_CONTINUE;
			}
			addValueMenu(id);
		}
		
	}
	return PLUGIN_CONTINUE;
}
public addValueMenu(id){
	
	if(!is_user_connected(id)) return;
	
	new menu = menu_create("\r[BaseBuilder]\y Dodawanie Walut", "addValueMenu_2");	
	
	menu_additem(menu, "Dodaj VIP'a");
	menu_additem(menu, "Dodaj SVIP'a");
	
	menu_additem(menu, "Dodaj Luzaczki");
	menu_additem(menu, "Dodaj Brylki");
	menu_additem(menu, "Dodaj Lvl");
	menu_additem(menu, "Dodaj Czas");
	menu_additem(menu, "Dodaj Reset");
	menu_additem(menu, "Dodaj EXP'a");
	menu_additem(menu, "\dZresetuj nagrode");
	menu_additem(menu, "Dodaj Kosci");
	menu_additem(menu, "Skrzynki");
	
	menu_additem(menu, "Dodaj wszystkim EXP'a");
	menu_additem(menu, "Dodaj wszystkim Kosci");
	menu_additem(menu, "Dodaj wszystkim Brylki");
	menu_additem(menu, "Dodaj wszystkim VIP'a");
	menu_additem(menu, "Dodaj wszystkim SIP'a");
	
	
	menu_display(id, menu, 0);
}

public addValueMenu_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return PLUGIN_CONTINUE;
	}
	
	if(!isSuperAdmin(id)) return PLUGIN_CONTINUE;
	
	
	switch(item){
		case 0:{
			userMenuPlayer[id] = MENU_GIVING_VIP;
			choosePlayer(id, 0);
		}
		case 1:{
			userMenuPlayer[id] = MENU_GIVING_SVIP;
			choosePlayer(id, 0);
		}
		case 2:{
			userMenuPlayer[id] = MENU_GIVING_LUZCOIN;
			choosePlayer(id, 0);
		}
		case 3:{
			userMenuPlayer[id] = MENU_GIVING_NUGGET;
			choosePlayer(id, 0);
		}
		case 4:{
			userMenuPlayer[id] = MENU_GIVING_LVL;
			choosePlayer(id, 0);
		}
		case 5:{
			userMenuPlayer[id] = MENU_GIVING_TIME;
			choosePlayer(id, 0);
		}
		case 6:{
			userMenuPlayer[id] = MENU_GIVING_RESET;
			choosePlayer(id, 0);
		}
		case 7:{
			userMenuPlayer[id] = MENU_GIVING_EXP;
			choosePlayer(id, 0);
		}
		case 8:{
			userMenuPlayer[id] = MENU_PLAYER_AWARD;
			choosePlayer(id, 0);
		}
		case 9:{
			userMenuPlayer[id] = MENU_GIVING_BONES;
			choosePlayer(id, 0);
		}
		case 10:{
			menuCreateCase(id);
		}
		case 11:{
			userMenuPlayer[id] = MENU_GIVING_EXP_ALL;
			cmd_execute(id, "messagemode Ilosc");
		}
		case 12:{
			userMenuPlayer[id] = MENU_GIVING_BONES_ALL;
			cmd_execute(id, "messagemode Ilosc");
		}
		case 13:{
			userMenuPlayer[id] = MENU_GIVING_NUGGET_ALL;
			cmd_execute(id, "messagemode Ilosc");
		}
		case 14:{
			userMenuPlayer[id] = MENU_GIVING_VIP_ALL;
			cmd_execute(id, "messagemode Ilosc");
		}
		case 15:{
			userMenuPlayer[id] = MENU_GIVING_SVIP_ALL;
			cmd_execute(id, "messagemode Ilosc");
		}
	}
	return PLUGIN_CONTINUE;
}
public addCash(id){

	if(!is_user_connected(id)) return;

	new gText[128];
	format(gText, sizeof(gText), "Ile chcesz doladowac\r Luzaczkow?^n\dPosiadasz\r %d\d Luzaczkow", userLuzCoin[id]);
	new menu = menu_create(gText, "addCash_2");
	for(new i = 0; i < sizeof(pricesMenu) ; i ++ ){
		
		format(gText, sizeof(gText), "\y%s\r Luzaczkow\d [\w %s PLN\d | SMS ]", pricesMenu[i][4], pricesMenu[i][2] );
		menu_additem(menu, gText);
	}
	menu_setprop(menu,MPROP_NEXTNAME,"Dalej");
	menu_setprop(menu,MPROP_BACKNAME,"Wroc");
	menu_setprop(menu,MPROP_EXITNAME,"Wyjscie");
	
	menu_display(id, menu, 0);
}
public addCash_2(id, menu, item){
	if( item == MENU_EXIT ){
		smsMainMenu(id);
		return PLUGIN_HANDLED;
	}
	userService[id] = item;
	displayBuy(id);
	return PLUGIN_HANDLED;
}
public displayBuy(id){
	new item = userService[id] ;
	new gText[512];
	new iLen;
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "SMS na doladowanie\y %s\r Luzaczkow", pricesMenu[item][4] );
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\wCena:\r %s PLN", pricesMenu[item][2] );
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\wWyslij SMS o tresci\r %s\w na numer\r %s\w", pricesMenu[item][0], pricesMenu[item][1] );
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^nUpewnij sie ze wpisales dobrze\r tresc\w i\r numer" );
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\n\y1.\w Wpisz kod zwrotny z\r SMS" );
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\y2.\w Wroc" );
	showMenu(id, B1 | B2 , gText, -1, "displayBuy");
}
public displayBuy_2(id, item){
	switch(item){
		case 0:{
			cmd_execute(id, "messagemode KodZwrotny");
			displayBuy(id);
		}
		case 1: addCash(id);
	}
}
public yourBuyService(id){
	new gText[512];
	new iLen;
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dWitaj\r %s\d oto twoj czas uslug^n^n", userName[id]);
	
	new daysLeft = 0;
	if(!isSVip(id)){
		daysLeft = (timeVip[id] - get_systime());
		if( daysLeft > 0 || isVip(id) ){
			if( has_flag(id, flagVip)) iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dPozostalo\y [Bez Limitu Vip]^n");
			else iLen += format(gText[iLen], sizeof(gText)-iLen-1,"\dPozostalo\y [%d:%s%d:%s%d]\d Vipa^n",( daysLeft / HOUR ),(daysLeft / MINUTE % MINUTE)<10?"0":"", ( daysLeft / MINUTE % MINUTE ), (daysLeft%MINUTE)<10?"0":"", ( daysLeft %MINUTE ));	
		} else iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dPozostalo\y [Brak Vipa]^n");
	} else {
		daysLeft = (timeSVip[id] - get_systime());
		if( daysLeft > 0 || isSVip(id) ){
			if( has_flag(id, flagSVip)) iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dPozostalo\y [Bez Limitu SVip]^n");
			else iLen += format(gText[iLen], sizeof(gText)-iLen-1,"\dPozostalo\y [%d:%s%d:%s%d]\d SVipa^n",( daysLeft / HOUR ),(daysLeft / MINUTE % MINUTE)<10?"0":"", ( daysLeft / MINUTE % MINUTE ), (daysLeft%MINUTE)<10?"0":"", ( daysLeft %MINUTE ));	
		} else iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dPozostalo\y [Brak SVipa]^n");
	}
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r0.\w Wyjscie");	
	showMenu(id,  B0  , gText, -1, "yourBuyService"  );
	
}
public yourBuyService_2(id, item){
	switch(item){
		default:{ return;}
	}
}
public addValue(id){
	if(!isSuperAdmin(id)) return PLUGIN_CONTINUE;
	new szArg[9];
	read_argv(1, szArg, sizeof(szArg));
	remove_quotes(szArg);
	new target = userService[id];
	new value = str_to_num(szArg);
	
	
	new gText[128];
	
	switch(userMenuPlayer[id]){
				
		case MENU_GIVING_VIP:{
				
			if(has_flag(target, flagVip)){
				chatPrint(id, PREFIX_LINE, "Gracz:^3 %s^1 posiada flage^3 't'^1 i posiada^1 VIP'a^1 bez limitu", userName[target]);
				choosePlayer(id, 0);
				return PLUGIN_CONTINUE;
			}
			timeVip[target] = max( timeVip[target] + (value*60), get_systime() + (value*60) );
			new daysLeft = (timeVip[target] - get_systime());
			userVip[target] = !!(daysLeft>0);
			
			chatPrint(id, PREFIX_LINE, "Dodales^4 [^3%d min VIP'a^4][^3Graczowi %s^4] ", value, userName[target]);
			chatPrint(target, PREFIX_LINE, "Otrzymales^4 [^3%d min VIP'a^4][^3Od Admina %s^4] ", value, userName[id]);
			format(gText, sizeof(gText), "dodal Vipa [%s] na [%d min]", userName[target], (daysLeft /60) );	
		} 
		case MENU_GIVING_SVIP:{
			if(has_flag(target, flagSVip)){
				chatPrint(id, PREFIX_LINE, "Gracz:^3 %s^1 posiada flage^3 's'^1 i posiada^1 SVIP'a^1 bez limitu", userName[target]);
				choosePlayer(id, 0);
				return PLUGIN_CONTINUE;
			}
			timeSVip[target] = max( timeSVip[target] + (value*60), get_systime() + (value*60) );
			new daysLeft = (timeSVip[target] - get_systime());
			userSVip[target] = !!(daysLeft>0);
			
			chatPrint(id, PREFIX_LINE, "Dodales^4 [^3%d min VIP'a^4][^3Graczowi %s^4] ", value, userName[target]);
			chatPrint(target, PREFIX_LINE, "Otrzymales^4 [^3%d min VIP'a^4][^3Od Admina %s^4] ", value, userName[id]);
			format(gText, sizeof(gText), "dodal Vipa [%s] na [%d min]", userName[target], (daysLeft /60) );	
		}
		case MENU_GIVING_LUZCOIN:{
			userLuzCoin[target] += value;
			chatPrint(id, PREFIX_LINE, "Dodales^4 [^3%d Luzaczkow^4][^3Graczowi %s^4] ", value, userName[target]);
			chatPrint(target, PREFIX_LINE, "Otrzymales^4 [^3%d Luzaczkow^4][^3Od Admina %s^4] ", value, userName[id]);
			format(gText, sizeof(gText), "dodal Luzaczki [%s] ilosc [%d]", userName[target], value);
		}
		case MENU_GIVING_NUGGET:{
			addNuggetToFinal(target, value);
			//userNugget[target] += value
			chatPrint(id, PREFIX_LINE, "Dodales^4 [^3%d Brylek^4][^3Graczowi %s^4] ", value, userName[target]);
			chatPrint(target, PREFIX_LINE, "Otrzymales^4 [^3%d Brylek^4][^3Od Admina %s^4] ", value, userName[id]);
			format(gText, sizeof(gText), "dodal Brylki [%s] ilosc [%d]", userName[target], value);	
		}
		case MENU_GIVING_LVL:{
			userLevel[target] += value;
			userLevel[target] = clamp(userLevel[target], 0, MAXLVL);
			chatPrint(id, PREFIX_LINE, "Dodales^4 [^3%d Lv.^4][^3Graczowi %s^4] ",value, userName[target]);
			chatPrint(target, PREFIX_LINE, "Otrzymales^4 [^3%d Lv.^4][^3Od Admina %s^4] ", value, userName[id]);
			format(gText, sizeof(gText), "dodal Lv. [%s] ilosc [%d ]", userName[target], value);	
		}
		case MENU_GIVING_TIME:{
			userTime[target] += value *60;
			chatPrint(id, PREFIX_LINE, "Dodales^4 [^3%d min Czasu Gry^4][^3Graczowi %s^4] ", value, userName[target]);			
			chatPrint(target, PREFIX_LINE, "Otrzymales^4 [^3%d min Czasu Gry^4][^3Od Admina %s^4] ", value, userName[id]);
			format(gText, sizeof(gText), "dodal Czas Gry [%s] na [%d min]", userName[target], value);
		}
		case MENU_GIVING_RESET:{
			userReset[target] += value;
			chatPrint(id, PREFIX_LINE, "Dodales^4 [^3%d Reset^4][^3Graczowi %s^4] ", value, userName[target]);
			chatPrint(target, PREFIX_LINE, "Otrzymales^4 [^3%d Reset^4][^3Od Admina %s^4] ", value, userName[id]);	
			format(gText, sizeof(gText), "dodal Reset [%s] ilosc [%d]", userName[target], value);
		}
		case MENU_GIVING_EXP:{
			addExpToFinal(target, float(value));
			//userExp[target] += float(value);
			chatPrint(id, PREFIX_LINE, "Dodales^4 [^3%d Xp.^4][^3Graczowi %s^4] ",value, userName[target]);
			chatPrint(target, PREFIX_LINE, "Otrzymales^4 [^3%d Xp.^4][^3Od Admina %s^4] ", value, userName[id]);
			format(gText, sizeof(gText), "dodal Xp. [%s] ilosc [%d]", userName[target], value);
		}
		case MENU_GIVING_BONES:{
			userBone[target] += value;
			chatPrint(id, PREFIX_LINE, "Dodales^4 [^3%d Kosci^4][^3Graczowi %s^4] ", value, userName[target]);
			chatPrint(target, PREFIX_LINE, "Otrzymales^4 [^3%d Kosci^4][^3Od Admina %s^4] ", value, userName[id]);
			format(gText, sizeof(gText), "dodal Kosci [%s] ilosc [%d]", userName[target], value);	
		}
		case MENU_GIVING_EXP_ALL, MENU_GIVING_BONES_ALL, MENU_GIVING_NUGGET_ALL, MENU_GIVING_VIP_ALL, MENU_GIVING_SVIP_ALL:{
			for(new i = 1; i < maxPlayers; i ++){
				if(!is_user_connected(i) || is_user_bot(i) || is_user_hltv(i)) continue;
				
				if(userMenuPlayer[id] == MENU_GIVING_EXP_ALL)
					addExpToFinal(i, float(value));
				else if(userMenuPlayer[id] == MENU_GIVING_BONES_ALL)
					addBoneToFinal(i, value);
				else if(userMenuPlayer[id] == MENU_GIVING_NUGGET_ALL)
					addNuggetToFinal(i, value);
				else if(userMenuPlayer[id] == MENU_GIVING_VIP_ALL){
					if(!has_flag(i, flagVip)){
						timeVip[i] = max( timeVip[i] + (value*60), get_systime() + (value*60) );
						new daysLeft = (timeVip[i] - get_systime());	
						userVip[i] = !!(daysLeft>0);
					}
				} else if(userMenuPlayer[id] == MENU_GIVING_SVIP_ALL){
					if(!has_flag(i, flagSVip)){
						timeSVip[i] = max( timeSVip[i] + (value*60), get_systime() + (value*60) );
						new daysLeft = (timeSVip[i] - get_systime());	
						userSVip[i] = !!(daysLeft>0);
					}
				}
						
			}
			if(userMenuPlayer[id] == MENU_GIVING_EXP_ALL){
				chatPrint(0, PREFIX_LINE, "Wszyscy otrzymali^4 [^3%d Xp.^4][^3Od Admina %s^4] ", value, userName[id]);
				format(gText, sizeof(gText), "dodal Xp. [%s] ilosc [%d]", "Wszystkim", value);
			}
			else if(userMenuPlayer[id] == MENU_GIVING_BONES_ALL){
				chatPrint(0, PREFIX_LINE, "Wszyscy otrzymali^4 [^3%d Kosci^4][^3Od Admina %s^4] ", value, userName[id]);
				format(gText, sizeof(gText), "dodal Kosci [%s] ilosc [%d]", "Wszystkim", value);
			}
			else if(userMenuPlayer[id] == MENU_GIVING_NUGGET_ALL){
				chatPrint(0, PREFIX_LINE, "Wszyscy otrzymali^4 [^3%d Brylki^4][^3Od Admina %s^4] ", value, userName[id]);
				format(gText, sizeof(gText), "dodal Brylek [%s] ilosc [%d]", "Wszystkim", value);
			}
			else if(userMenuPlayer[id] == MENU_GIVING_VIP_ALL){
				chatPrint(0, PREFIX_LINE, "Wszyscy otrzymali^4 [^3%dmin VIP'a^4][^3Od Admina %s^4] ", value, userName[id]);
				format(gText, sizeof(gText), "dodal VIP'a [%s] ilosc [%d min]", "Wszystkim", value);
			}
			else if(userMenuPlayer[id] == MENU_GIVING_SVIP_ALL){
				chatPrint(0, PREFIX_LINE, "Wszyscy otrzymali^4 [^3%dmin SVIP'a^4][^3Od Admina %s^4] ", value, userName[id]);
				format(gText, sizeof(gText), "dodal SVIP'a [%s] ilosc [%d min]", "Wszystkim", value);
			}
				
			addValueMenu(id);
			return PLUGIN_CONTINUE;
		}
	}
	logBB(id, LOG_SMS, "add", gText);
	
	choosePlayer(id, 0);
	return PLUGIN_CONTINUE;
}
public addValueTransfer(id){
	new szArg[9];
	read_argv(1, szArg, sizeof(szArg));
	remove_quotes(szArg);
	new target = userService[id];
	new value = str_to_num(szArg);
	
	if(!is_user_connected(target)){
		chatPrint(id, PREFIX_LINE, "Nie ma gracza na serwerze ktoremu chciales dac Brylki");
		return PLUGIN_CONTINUE;
	}
	switch(userMenuPlayer[id]){	
		case MENU_PLAYER_NUGGET:{
				
			value = abs(str_to_num(szArg));
			if( value == 0){
				chatPrint(id, PREFIX_NORMAL, "Nie mozesz dac 0 Brylek!");
				return PLUGIN_CONTINUE;
			}
			if( value > userNugget[id] ){
				chatPrint(id, PREFIX_NORMAL, "Nie masz tyle Brylek!");
				return PLUGIN_CONTINUE;
			}
	
			logBB(id, LOG_TRANSFER, "send", "wyslal [%d] brylek graczowi [%s]", value, userName[target]);
			
			userNugget[id]		-=	value;
			userNugget[target]	+=	value;
			chatPrint(id, PREFIX_NORMAL, "Wyslales^3 %s^1 Brylek do gracza^4 %s!", formatNumber(value), userName[target] ) ;
			chatPrint(target, PREFIX_NORMAL, "%s Otrzymales^3 %s^1 Brylek od gracza^4 %s!", formatNumber(value), userName[id] ) ;
		}
	}
	choosePlayer(id, 0);
	return PLUGIN_CONTINUE;
}

public codeCheck(id){
	if( get_gametime() - fCodeCheckTime < 5.0 ){		
		chatPrint(id, PREFIX_NONE, "[SKLEP]^1 Luzaczki wlasnie sprawdzaja juz kod, poczekaj chwile!");
		return PLUGIN_HANDLED;
	}
	fCodeCheckTime=get_gametime();
	read_argv(1, szSmsCode, sizeof(szSmsCode));
	remove_quotes(szSmsCode);
	
	lastBuyer=id;
	
	new API[33];
	get_cvar_string("bb_api_cssetti", API, sizeof(API));
	
	chatPrint(id, PREFIX_NONE, "[SKLEP]^1 Luzaczki sprawdzaja kod, prosze czekac...");
	new szLink[512];	
	format(szLink, sizeof(szLink), "http://cssetti.pl/Api/SmsApiV2CheckCode.php?UserId=%s&Code=%s", API, szSmsCode);

	HTTP2_Download(szLink, _, _, "checkCodeSmsSeti");
	return PLUGIN_CONTINUE;
}
public checkCodeSmsSeti(idHandler, error){	
	
	if(!is_user_connected(lastBuyer)){
		
		logBB(lastBuyer, LOG_SMS, "disconnect", "wpisal kod SMS i nie ma go na serwerze!");
			
		return PLUGIN_HANDLED;
	}
	
	fCodeCheckTime	=	0.0;
	new szData[64], iLen;
	
	HTTP2_getData(szData, sizeof(szData)-1, iLen);
	log_amx("Odpowiedz : %s | %", szData, userName[lastBuyer]);
	
	if( equal(szData, "-3", 2)){
		cmd_execute(lastBuyer, "messagemode KodZwrotny");
		chatPrint(lastBuyer, PREFIX_NONE, "[SKLEP]^1 Kod SMS pusty! Sprobuj ponownie.");			
		return PLUGIN_HANDLED;
	}
	else if( equal(szData, "-2", 2) ){
		cmd_execute(lastBuyer, "messagemode KodZwrotny");
		chatPrint(lastBuyer, PREFIX_NONE, "[SKLEP]^1 Kod SMS posiada nieprawidlowe znaki! Sprobuj ponownie.");			
		return PLUGIN_HANDLED;
	}
	else if( equal(szData, "-1", 2) ){
		chatPrint(lastBuyer, PREFIX_NONE, "[SKLEP]^1 Luzaczkowy Sklep napotkal problemy, sprobuj ponownie pozniej.");
		log_amx("Sklep niepoprawny user ID");			
		return PLUGIN_HANDLED;
	}
	else if( equal(szData, "0", 1) ){
		cmd_execute(lastBuyer, "messagemode KodZwrotny");
		chatPrint(lastBuyer, PREFIX_NONE, "[SKLEP]^1 Kod SMS jest nieprawidlowy! Sprobuj ponownie.");
		return PLUGIN_HANDLED;
	}
	else {
		for( new i = sizeof(pricesMenu)-1 ; i >= 0 ; i -- ){
			if( str_to_float(pricesMenu[i][3]) == str_to_float(szData) ){
				chatPrint(lastBuyer, PREFIX_NONE, "(:^1 Doladowales^4 %s^1 Luzaczkow^4 :)", pricesMenu[i][4]);
				chatPrint(0, PREFIX_NORMAL, "(:^1 Dziekujemy!^3 %s^1 kupil^3 Luzaczki^4 :)", userName[lastBuyer]);
				userLuzCoin[lastBuyer] += str_to_num(pricesMenu[i][4]);
				
				logBB(lastBuyer, LOG_SMS, "buy", "doladowanie [%s PLN][%s Cena][Kod: %s][Ilosc: %s]", pricesMenu[i][3], pricesMenu[i][2], szSmsCode, pricesMenu[i][4]);
				
				return PLUGIN_HANDLED;
			}
		}
	}
	return PLUGIN_HANDLED;
}
public luzaczkiShop(id){

	if(!is_user_connected(id)) return PLUGIN_CONTINUE;

	if( !playerLogged(id) ){
		chatPrint(id, PREFIX_NORMAL, "Zaloguj sie aby moc korzystac z tego menu!");
		return PLUGIN_HANDLED;
	}
	
	new gText[128];
	
	if(!isSVip(id)){
		new daysLeft = (timeVip[id] - get_systime());	
		if( daysLeft > 0 ||isVip(id)){
			if(has_flag(id, flagVip)) format(gText, sizeof(gText), "\dPosiadasz\r %d\d Luzaczkow^nPozostalo\y [Bez Limitu Vip]", userLuzCoin[id]);
			else format(gText, sizeof(gText), "\dPosiadasz\r %d\d Luzaczkow^nPozostalo\y %d:%s%d:%s%d\d Vipa",userLuzCoin[id], ( daysLeft / HOUR ),(daysLeft / MINUTE % MINUTE)<10?"0":"", ( daysLeft / MINUTE % MINUTE ), (daysLeft%MINUTE)<10?"0":"", ( daysLeft %MINUTE ));
		} else  format(gText, sizeof(gText), "\dPosiadasz\r %d\d Luzaczkow^nPozostalo\y [Brak Vipa]", userLuzCoin[id]);
	} else {
		new daysLeft = (timeSVip[id] - get_systime());	
		if( daysLeft > 0 ||isSVip(id)){
			if(has_flag(id, flagSVip)) format(gText, sizeof(gText), "\dPosiadasz\r %d\d Luzaczkow^nPozostalo\y [Bez Limitu SVip]", userLuzCoin[id]);
			else format(gText, sizeof(gText), "\dPosiadasz\r %d\d Luzaczkow^nPozostalo\y %d:%s%d:%s%d\d SVipa",userLuzCoin[id], ( daysLeft / HOUR ),(daysLeft / MINUTE % MINUTE)<10?"0":"", ( daysLeft / MINUTE % MINUTE ), (daysLeft%MINUTE)<10?"0":"", ( daysLeft %MINUTE ));
		} else  format(gText, sizeof(gText), "\dPosiadasz\r %d\d Luzaczkow^nPozostalo\y [Brak SVipa]", userLuzCoin[id]);
	}

	
	new menu = menu_create(gText, "luzaczkiShop_2");
	
	for( new i = 0; i < sizeof(itemsShop); i ++ ){
		format(gText, sizeof(gText), "%s\d [\y %s\r Luzaczkow\d ]", itemsShop[i][0], itemsShop[i][1]);
		menu_additem(menu, gText);
	}
	
	menu_setprop(menu,MPROP_NEXTNAME,"Dalej");
	menu_setprop(menu,MPROP_BACKNAME,"Wroc");
	menu_setprop(menu,MPROP_EXITNAME,"Wyjscie");
	
	menu_display(id, menu, 0);
	return PLUGIN_CONTINUE;
}
public luzaczkiShop_2(id, menu, item){
	if( item == MENU_EXIT ){
		smsMainMenu(id);
		return PLUGIN_HANDLED;
	}
	if( userLuzCoin[id] < str_to_num(itemsShop[item][1]) ){
		chatPrint(id, PREFIX_LINE, "Niestety nie masz na to^3 Luzaczkow");
		luzaczkiShop(id);
		return PLUGIN_HANDLED;
	}
	userService[id] = item;
	luzaczkiFinal(id);
	return PLUGIN_CONTINUE;
}

public luzaczkiFinal(id){
	new item = userService[id] ;
	new gText[512];
	new iLen;
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "Czy na pewno chcesz kupic:^n^n");
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dPrzedmiot:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW],itemsShop[item][0]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dCena:\r %s Luzakow^n", symbolsCustom[SYMBOL_DR_ARROW],itemsShop[item][1]);
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dOpis:\r %s",symbolsCustom[SYMBOL_DR_ARROW], itemsShop[item][2]);
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\y1.\w Tak ! Biore !");
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\y2.\r Nie ! Absolutenie !");
	
	showMenu(id, B1 | B2 | B3, gText, -1, "luzaczkiFinal");
}


public luzaczkiFinal_2(id, item){
	if( item != 0 ){
		luzaczkiShop(id);
		return PLUGIN_CONTINUE;
	}
	if( !playerLogged(id) ){	
		chatPrint(id, PREFIX_NONE, "[SKLEP]^1 Zaloguj sie aby kupowac!");
		return PLUGIN_CONTINUE;
	}
	
	item = userService[id];
	switch(item){
		case 0:{
			timeVip[id]	= 	max( timeVip[id] + (DAY*30), get_systime() + (DAY*30) );
		}	
		case 1:{
			timeSVip[id]	= 	max( timeSVip[id] + (DAY*30), get_systime() + (DAY*30) );
		}
		case 2:{
			new newLeftExp  =  userScrollExp[id] - playedTime(id);
			if(newLeftExp <= 0){
				userScrollExp[id] = playedTime(id);
				userScrollExp[id] += (HOUR*6);	
			} else userScrollExp[id] += (HOUR*6);		
	
		}
		case 3:{
			new newLeftNugget  =  userScrollNugget[id] - playedTime(id);
			if(newLeftNugget <= 0){
				userScrollNugget[id] = playedTime(id);
				userScrollNugget[id] += (HOUR*6);	
			} else userScrollNugget[id] += (HOUR*6);		
			
		}
		case 4:{
			
			new newLeftMine  =  userBoostMine[id] - playedTime(id);
			if(newLeftMine <= 0){
				userBoostMine[id] = playedTime(id);
				userBoostMine[id] += (MINUTE*55);
			} else userBoostMine[id] += (MINUTE*55);
			
		}
	}
	userLuzCoin[id] -= str_to_num(itemsShop[item][1]);
	chatPrint(id, PREFIX_LINE, "Kupiles^3 %s^1 za^3 %s Luzaczkow", itemsShop[item][0], itemsShop[item][1] );
	Display_Fade(id, 4096,4096, 4096, 64, 255, 64, 80);
	set_dhudmessage(30, 160, 30, -1.0, 0.20, 0, 0.5, 2.0, 0.5, 0.5);
	show_dhudmessage(id, "Kupiles %s za %s Luzaczkow", itemsShop[item][0], itemsShop[item][1]);
	
	logBB(id, LOG_SMS, "buy", "kupil [%s] za [%s Luzaczkow]", itemsShop[item][0], itemsShop[item][1]);
			
	
	luzaczkiShop(id);
	
	
	
	
	return PLUGIN_HANDLED;
	
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
