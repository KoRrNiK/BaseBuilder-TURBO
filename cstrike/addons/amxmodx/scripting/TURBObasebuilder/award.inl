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
	
	if(!is_user_connected(id)) return;
	
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


public checkForAward(id){
	
	if( !playerLogged(id)){
		chatPrint(id, PREFIX_NORMAL, "Zaloguj sie aby odebrac nagrode!");
		return PLUGIN_CONTINUE;
	}
	
	new newAwardLeft = userLastAwardTime[id] + userAwardTime - playedTime(id);
	
	if( userLastAwardTime[id] + userAwardTime > userTime[id] && newAwardLeft > 0){
		chatPrint(id, PREFIX_NORMAL, "Jeszcze nie mozna odebrac nagrody! Nagroda za^4 %d:%s%d:%s%d", ( newAwardLeft / HOUR ), ( newAwardLeft / MINUTE % MINUTE )<10?"0":"", ( newAwardLeft / MINUTE % MINUTE ), (newAwardLeft%MINUTE)<10?"0":"", ( newAwardLeft %MINUTE ));
		return PLUGIN_CONTINUE;
	}
	
	new bool:empty	=	true;
	
	new gText[128], iLen;
	logType[id] = LOG_AWARD;
	if(logType[id] == LOG_AWARD){
		if(isSVip(id)){
			switch(random_num(1,6)){
				case 1:{
					
					new vipRandom = random_num(1000,2500);
					addNuggetToFinal(id, vipRandom);
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 %d Brylek^1 z nagrody.", userName[id],vipRandom);
					iLen += format(gText[iLen], sizeof(gText)-1-iLen,"dostal %d brylek z Nagrody!",vipRandom);
					empty	=	false;
				}
				case 2:{
					new Float:expVipRandom = random_float(300.0, 700.0);
					addExpToFinal(id, expVipRandom);
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 %0.1f EXP'a^1 z nagrody.", userName[id],expVipRandom);
					iLen += format(gText[iLen], sizeof(gText)-1-iLen, "dostal %0.1f EXP'a z Nagrody!",expVipRandom);
					empty	=	false;
				}
				case 3:{
					new svipRandomVip = random_num(1,3);
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 SVIP'a^1 na^4 %d %s^1 z nagrody!", userName[id], svipRandomVip, svipRandomVip == 1 ? "dzien" : "dni" );
					timeSVip[id]	= 	max( timeSVip[id] + (DAY*svipRandomVip), get_systime() + (DAY*svipRandomVip) );
					empty	=	false;
				}
				case 4:{
					new Float:fOrigin[3];
					pev(id, pev_origin, fOrigin);
					
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 Skrzynke^1 z nagrody.", userName[id]);
					createCases(fOrigin, .disappear = 1, .owner = id);
					empty	=	false;
				}
				case 5:{
					new svipTimeScrool = (HOUR*random_num(2,6));
					new newLeftExp  =  userScrollExp[id] - playedTime(id);
					if(newLeftExp <= 0){
						userScrollExp[id] = playedTime(id);
						userScrollExp[id] += svipTimeScrool;
					} else userScrollExp[id] += svipTimeScrool;	
					
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 Zwoj Doswiadczenia^1 na^4 %d %s^1 z nagrody!", userName[id], (svipTimeScrool/HOUR), (svipTimeScrool/HOUR) == 1  ? "godzine" : (svipTimeScrool/HOUR) ==  5 ? "godzin" : "godziny" );
					empty	=	false;
				}
				case 6:{
					new svipTimeScrool = (HOUR*random_num(2,6));
					new newLeftNugget  =  userScrollNugget[id] - playedTime(id);
					if(newLeftNugget <= 0){
						userScrollNugget[id] = playedTime(id);
						userScrollNugget[id] += svipTimeScrool;	
					} else userScrollNugget[id] += svipTimeScrool;
					
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 Zwoj Szczescia^1 na^4 %d %s^1 z nagrody!", userName[id], (svipTimeScrool/HOUR), (svipTimeScrool/HOUR) == 1  ? "godzine" : (svipTimeScrool/HOUR) ==  5 ? "godzin" : "godziny" );
					empty	=	false;
				}	
			}
		} else if( isVip(id)){
			switch(random_num(1,6)){
				case 1:{
					
					new vipRandom = random_num(750,2000);
					addNuggetToFinal(id, vipRandom);
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 %d Brylek^1 z nagrody.", userName[id],vipRandom);
					iLen += format(gText[iLen], sizeof(gText)-1-iLen,"dostal %d brylek z Nagrody!",vipRandom);
					empty	=	false;
				}
				case 2:{
					new Float:expVipRandom = random_float(200.0, 500.0);
					addExpToFinal(id, expVipRandom);
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 %0.1f EXP'a^1 z nagrody.", userName[id],expVipRandom);
					iLen += format(gText[iLen], sizeof(gText)-1-iLen, "dostal %0.1f EXP'a z Nagrody!",expVipRandom);
					empty	=	false;
				}
				case 3:{
					new vipRandomVip = random_num(1,10);
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 VIP'a^1 na^4 %d %s^1 z nagrody!", userName[id], vipRandomVip, vipRandomVip == 1 ? "dzien" : "dni" );
					timeVip[id]	= 	max( timeVip[id] + (DAY*vipRandomVip), get_systime() + (DAY*vipRandomVip) );
					empty	=	false;
				}
				case 4:{
					new Float:fOrigin[3];
					pev(id, pev_origin, fOrigin);
					
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 Skrzynke^1 z nagrody.", userName[id]);
					createCases(fOrigin, .disappear = 1, .owner = id);
					empty	=	false;
				}
				case 5:{
					new vipTimeScrool = (HOUR*random_num(1,5));
					new newLeftExp  =  userScrollExp[id] - playedTime(id);
					if(newLeftExp <= 0){
						userScrollExp[id] = playedTime(id);
						userScrollExp[id] += vipTimeScrool;
					} else userScrollExp[id] += vipTimeScrool;	
					
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 Zwoj Doswiadczenia^1 na^4 %d %s^1 z nagrody!", userName[id], (vipTimeScrool/HOUR), (vipTimeScrool/HOUR) == 1  ? "godzine" : (vipTimeScrool/HOUR) ==  5 ? "godzin" : "godziny" );
					empty	=	false;
				}
				case 6:{
					new vipTimeScrool = (HOUR*random_num(1,5));
					new newLeftNugget  =  userScrollNugget[id] - playedTime(id);
					if(newLeftNugget <= 0){
						userScrollNugget[id] = playedTime(id);
						userScrollNugget[id] += vipTimeScrool;	
					} else userScrollNugget[id] += vipTimeScrool;
					
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 Zwoj Szczescia^1 na^4 %d %s^1 z nagrody!", userName[id], (vipTimeScrool/HOUR), (vipTimeScrool/HOUR) == 1  ? "godzine" : (vipTimeScrool/HOUR) ==  5 ? "godzin" : "godziny" );
					empty	=	false;
				}	
			}
		}else{	
			switch(random_num(1,7)){
				case 1:{
					new userRandom = random_num(500,1000);
					addNuggetToFinal(id, userRandom);
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 %d Brylek^1 z nagrody.", userName[id],userRandom);
					iLen += format(gText[iLen], sizeof(gText)-1-iLen, "dostal %d brylek z Nagrody!",userRandom);
					empty	=	false;
				}
				case 2:{
					new Float:expRandom = random_float(100.0, 300.0);
					addExpToFinal(id, expRandom);
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 %0.1f EXP'a^1 z nagrody.", userName[id],expRandom);
					iLen += format(gText[iLen], sizeof(gText)-1-iLen, "dostal %0.1f EXP'a z Nagrody!",expRandom);
					empty	=	false;
				} 
				case 3:{
					new userRandomVip = random_num(1,5);
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 VIP'a^1 na^4 %d %s^1 z nagrody!", userName[id], userRandomVip, userRandomVip == 1 ? "dzien" : "dni" );
					timeVip[id]	= 	max( timeVip[id] + (DAY*userRandomVip), get_systime() + (DAY*userRandomVip) );
					empty	=	false;
				}
				case 4:{
					new Float:fOrigin[3];
					pev(id, pev_origin, fOrigin);
					
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 Skrzynke^1 z nagrody.", userName[id]);
					createCases(fOrigin, .disappear = 1, .owner = id);
					empty	=	false;
				}
				case 5:{
					new userTimeScrool = (HOUR*random_num(1,3));
					new newLeftExp  =  userScrollExp[id] - playedTime(id);
					if(newLeftExp <= 0){
						userScrollExp[id] = playedTime(id);
						userScrollExp[id] += userTimeScrool;	
					} else userScrollExp[id] += userTimeScrool;
					
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 Zwoj Doswiadczenia^1 na^4 %d %s^1 z nagrody!", userName[id], (userTimeScrool/HOUR), (userTimeScrool/HOUR) == 1  ? "godzine" : "godziny" );
					empty	=	false;
				}
				case 6:{
		
					new userTimeScrool = (HOUR*random_num(1,3));
					new newLeftNugget  =  userScrollNugget[id] - playedTime(id);
					if(newLeftNugget <= 0){
						userScrollNugget[id] = playedTime(id);
						userScrollNugget[id] += userTimeScrool;
					} else userScrollNugget[id] += userTimeScrool;
					
					chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 otrzymal^4 Zwoj Szczescia^1 na^4 %d %s^1 z nagrody!", userName[id], (userTimeScrool/HOUR), (userTimeScrool/HOUR) == 1  ? "godzine" : "godziny" );
					empty	=	false;
				}
			}
		}
		if( empty ){
			chatPrint(id, PREFIX_NORMAL, "Ojejku :( Nic nie dostales.");
			iLen += format(gText[iLen], sizeof(gText)-1-iLen, "nic nie dostal z Nagrody!");
		}
		logBB(id, gText);
	}
	userAllAward[id] ++;
	userLastAwardTime[id]=playedTime(id);
	return PLUGIN_CONTINUE;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
