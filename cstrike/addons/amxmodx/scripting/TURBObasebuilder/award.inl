#include < amxmodx >
#include < amxmisc >

new const freeAwards[10][]= {
	"700 Brylek",
	"200 Expa",
	"200 Kosci",
	"Skrzynka",
	"Vip 1 godz.",
	"1000 Brylek",
	"Zwoj doswiadczenia 30 min",
	"400 Expa",
	"Zwoj szczescia 30 min",
	"300 Kosci",

};
public menuFreeAward(id){
	
	if(  get_systime() - userLastAwardFree[id]  > (DAY*2)-HOUR){
		userLastAwardRow[id] = 0;		
	}
	new gText[256], iLen;
	new awardWithin = get_systime() - userLastAwardFree[id];
	new awardPlay = playedTime(id) - userLastAwardGot[id];
	
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "\dOdbieraj codziennie nagrode aby odbierac coraz lepsze!^n");
	iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "\dAby moc odebrac nagrode, przegraj godzine dziennie na serwerze!^n");
	
	
	if( awardWithin < DAY )
		iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "\y%s^t^t\dNastepna nagroda za:\r %d Minut", symbolsCustom[SYMBOL_DR_ARROW], (DAY-awardWithin)/MINUTE);
	else if( playedTime(id) - userLastAwardGot[id] < HOUR )
		iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "\y%s^t^t\dMusisz przegrac:\r %d Min", symbolsCustom[SYMBOL_DR_ARROW], (HOUR-awardPlay)/MINUTE);		
	else iLen 	+=	 format(gText[iLen], sizeof(gText) - iLen - 1, "\y%s^t^t\dNagroda:\r Do odebrania!", symbolsCustom[SYMBOL_DR_ARROW]);		
	
	new menu = menu_create(gText, "menuFreeAward_2");
	
	for( new i = 0 ; i < min(sizeof(freeAwards), userLastAwardRow[id]+1 ); i++){
		if((awardWithin < DAY) || (playedTime(id) - userLastAwardGot[id] < HOUR )) format(gText, sizeof(gText),"\dDzien\r %d:\d %s", (i+1), freeAwards[i] );
		else format(gText, sizeof(gText),"\dDzien\r %d:\w %s", (i+1), freeAwards[i] );
		
		menu_additem(menu, gText);
	}
	menu_display(id, menu, 0);
}
public menuFreeAward_2(id, menu, item){
	if( item == MENU_EXIT ){
		menu_destroy(menu);
		return PLUGIN_CONTINUE;
	}
	
	new awardPlay = playedTime(id) - userLastAwardGot[id];
	new awardWithin = get_systime() - userLastAwardFree[id];
	if( awardWithin < DAY ){
		chatPrint(id, PREFIX_NORMAL, "Nagroda dostepna za^3 %d Minut", (DAY-awardWithin)/MINUTE);
		return PLUGIN_CONTINUE;
	}
	if( playedTime(id) - userLastAwardGot[id] < HOUR ){
		chatPrint(id, PREFIX_NORMAL, "Musisz przegrac^3 %d Minut^1 aby moc odebrac^4 nagrode", (HOUR-awardPlay)/MINUTE);
		return PLUGIN_CONTINUE;
	}
	switch(item){
		case 0: addNugget(id, 700);
		case 1: addExpToFinal(id, 200.0);
		case 2: addBoneToFinal(id, 200);
		
		case 3:{
			new Float:fOrigin[3];
			pev(id, pev_origin, fOrigin);		
			createCases(fOrigin, .disappear = 1, .owner = id);
		}
		case 4: timeVip[id]	= 	max( timeVip[id] + (HOUR*1), get_systime() + (HOUR*1) );
		case 5: addNugget(id, 1000);
		
		case 6:{
			new timeFree = (MINUTE*30);
			new newLeftExp  =  userScrollExp[id] - playedTime(id);
			if(newLeftExp <= 0){
				userScrollExp[id] = playedTime(id);
				userScrollExp[id] += timeFree;	
			} else userScrollExp[id] += timeFree;	
		}
		case 7: addExpToFinal(id, 400.0);
		case 8:{
			new timeFree = (MINUTE*30);
			new newLeftNugget  =  userScrollNugget[id] - playedTime(id);
			if(newLeftNugget <= 0){
				userScrollNugget[id] = playedTime(id);
				userScrollNugget[id] += timeFree;
			} else userScrollNugget[id] += timeFree;
					
		}
		case 9: addExpToFinal(id, 300.0);
	}
	new logText[128];
	logType[id] = LOG_AWARD;
	if(logType[id] == LOG_AWARD){		
		format(logText, sizeof(logText), "odebral darmowa nagrode: %s | Dzien: %d",  freeAwards[item], userLastAwardRow[id]+1 );
		logBB(id, logText);
	}	
	
	userLastAwardGot[id]	=	playedTime(id);
	userLastAwardFree[id]	=	get_systime();
	userLastAwardRow[id] ++;
	
	chatPrint(id, PREFIX_NORMAL, "Nagroda odebrana^4 %d^1 raz z rzedu", userLastAwardRow[id]);
	chatPrint(id, PREFIX_NORMAL, "Nastepna za:^3 %d Minut", (HOUR-awardPlay)/MINUTE);
	return PLUGIN_CONTINUE;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
