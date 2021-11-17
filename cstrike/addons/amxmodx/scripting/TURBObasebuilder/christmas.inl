#include <	amxmodx		>
#include <	amxmisc		>
#include <	cstrike		>
#include <	fun		>
#include <	fakemeta	>
#include <	fakemeta_util	>
#include <	engine		>
#include <	csx		>
#include <	fvault		>
#include <	http2		>
#include <	xs		>
#include <	sockets		>
#include <	sqlx		>
#include <	regex		>

enum { CHRISTMAS_V, CHRISTMAS_W, CHRISTMAS_ALL }; 	
new const modelGranade[CHRISTMAS_ALL][] = {
	"models/basebuildervt/christmas/v_snowball.mdl",
	"models/basebuildervt/christmas/w_snowball.mdl"
};

new const szChristmasTree[] 	= 	"christmasTree";
new const szTreeModel[]		=	"models/basebuildervt/christmas/treeNew.mdl";
new const szTreeSprite[]		=	"sprites/basebuildervt/christmas/treespot.spr";
new const szChristmasTreeSprite[]=	"christmasTreeSpr";

new userMenuChristmas[33];

new Float:distanceOpen = 75.5;
new entChristmas[1024];
new bool:christmas;
new Float:userUnstuckTree[33];

public addChristmasMission(id, mission, val){
	if(!userChristmasStart[id]) return;
	if(dayCount() != mission) return; 
	
	if( mission == userChristmasType[id]){
		if( userChristmasMission[id] != -1){
			userChristmasMission[id] = min(userChristmasMission[id]+val, str_to_num(christmasMission[mission][1]));	
		}
	}
}
public christmasPrecache(){
	
	create_entity("env_snow");

	for(new i = 1; i < sizeof(timePlayChristmas); i ++) precache_sound(formatm("basebuildervt/christmas/%d.mp3", i));
	for(new i = 0; i < CHRISTMAS_ALL; i++) precache_model(modelGranade[i]);
		
	precache_model(szTreeModel);
	precache_model(szTreeSprite);

}
public christmasTouchTree(id, ent){
	if( !pev_valid(ent)  ) return;

	new szClass[15];
	entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass));

	if(equal(szClass, szChristmasTree)){
		if(get_gametime() - userUnstuckTree[id] > 2.0){
			cmdUnstuckTree(id);
			userUnstuckTree[id] = get_gametime();
		}
	} 
}
public checkChristmas(){
	if(christmas) return true;
	else return false;
}	
public numTree(){
	new ent = -1, iNum = 0;
	while((ent = find_ent_by_class(ent, szChristmasTree)) > 0){
		iNum++;
		entChristmas[iNum] = ent;
	}
	return iNum;
}
public menuCreateChristmas(id){

	if(!is_user_connected(id)) return;

	if(!isSuperAdmin(id)) return;

	new gText[128];
	format(gText, sizeof(gText), "\r[BaseBuilder]\y Swieta!");
	new menu = menu_create(gText, "menuCreateChristmas_2");
	new cb = 	menu_makecallback("menuCreateChristmas_3");
	
	menu_additem(menu, "Sworz Choinke", .callback=cb);
	menu_additem(menu, "Usun", .callback=cb);
	menu_additem(menu, "Zapisz^n");
	menu_display(id, menu, 0);
	
}
public menuCreateChristmas_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return PLUGIN_CONTINUE;
	}
	
	new ent = -1;
	
	switch(item){
		
		case 0:{
		
			while((ent = find_ent_by_class(ent, szChristmasTree)) > 0){
				menuCreateChristmas(id);
				return PLUGIN_CONTINUE;
				
			}
			chatPrint(id, PREFIX_LINE, "Stworzyles Choinke");	
			new Float:fOrigin[3];
		
			entity_get_vector(id,	EV_VEC_origin, 	fOrigin );
		
			createTree(fOrigin, SOLID_NOT);
			new Float:fMin[3];
			entity_get_vector(id, EV_VEC_mins, fMin);
			if(fMin[2]==-18.0) fOrigin[2] -= 18.0;
			else fOrigin[2] -= 36.0;
			createSprite(fOrigin);
			menuCreateChristmas(id);
		}
		case 1:{
			while ((ent = find_ent_by_class(ent, szChristmasTree)) > 0) {
		
				if (ent > 0){
					remove_entity(ent);
					chatPrint(id, PREFIX_LINE, "Usunales Choinke!");
				}
			}
			while ((ent = find_ent_by_class(ent, szChristmasTreeSprite)) > 0) {
		
				if (ent > 0) remove_entity(ent);
				
			}
	
			menuCreateChristmas(id);
		}
		
		case 2:{
			saveChristmas();
			chatPrint(id, PREFIX_LINE, "Zapisales");
			menuCreateChristmas(id);
		}
	}

	return PLUGIN_CONTINUE;
}
public menuCreateChristmas_3(id, menu, item) {
	switch(item){
		case 0: {
			if(numTree() == 0) return ITEM_ENABLED;
			else return ITEM_DISABLED;	
		}
		case 1:{
			if(numTree() == 1) return ITEM_ENABLED;
			else return ITEM_DISABLED;
		}
		default: return ITEM_ENABLED;
	}	
	return ITEM_ENABLED;
}
public openMenuTree(id, ent, Float:cordx,  Float:cordy){

	if( !pev_valid(ent) || userMenuChristmas[id] || !isTree(ent)) return;		
	new button =  pev(id, pev_button);
	new Float:fOrigin[3], Float:fOriginEnt[3];
	entity_get_vector(id,	EV_VEC_origin, 	fOrigin);
	entity_get_vector(ent,	EV_VEC_origin, 	fOriginEnt);


	if( get_distance_f( fOrigin, fOriginEnt ) <= distanceOpen){		
		
		if( get_gametime()-userAimingHud[id] >= 0.2){
			set_dhudmessage(153, 255, 102, cordx, cordy, 0, 0.1, 0.1, 0.1, 0.1);
			show_dhudmessage(id, "^nAby sprawdzic dzisiejsze wyzwanie!^nKliknij: [ E ]^n");
								
			userAimingHud[id] = get_gametime();			
		}
	}	
	if( ( button & IN_USE ) && get_distance_f( fOrigin, fOriginEnt ) <= distanceOpen ){
		menuChristmas(id);
		userMenuChristmas[id] = true;
		
	}
}

public dayCount(){
	return (((get_systime() -  firstDayDecember ) / DAY));
}
public menuChristmas(id){	
	
	if(!is_user_connected(id)) return;
	
	new gText[712], iLen = 0;

	new mission = dayCount();

	if(dayCount() >= 0 && userChristmasType[id] != mission){
		
		userChristmasMission[id] = 0;
		userChristmasStart[id] = 0;
		userChristmasType[id] = mission;
		
		chatPrint(id, PREFIX_LINE, "Ustawiono nowe wyzwanie!");
	}
	if(dayCount() >= 0) iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "\r[\y AKTUALNIE MAMY:\w %d Dzien!\r ]^n^n", dayCount()+1);
	
	iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "\wWitaj wedrowcze!\w Co Ciebie do mnie sprowadza?^n");
	iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "\wChcesz wziac udzial w swiatecznym losowaniu?^n");

	
	if(dayCount() < 0 || dayCount() >= CHRISTMAS_TOTAL_MISSION){
		
		iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^n\dAktualnie nie ma przewidzianych nagrod na ten dzien!");
		
	} else {
	
		iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t^t\dOto co musisz dla mnie dzisiaj zrobic:\r %s!", symbolsCustom[SYMBOL_DR_ARROW], christmasMission[userChristmasType[id]][0]);
		
		if(userChristmasStart[id] && userChristmasMission[id] != -1){
			iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "^n\y%s^t^t\dPostep:\r %d/%d", symbolsCustom[SYMBOL_DR_ARROW], userChristmasMission[id], str_to_num(christmasMission[userChristmasType[id]][1]));
		}
	}
	new menu = menu_create(gText, "menuChristmas_2");
	
	if(dayCount() < 0 || dayCount() >= CHRISTMAS_TOTAL_MISSION){
		menu_additem(menu, "\dDzisiaj nic nie ma :(");
	} else {
		if(!userChristmasStart[id]){
			menu_additem(menu, "\wZacznij wykonywac wyzwanie!");
		} else {
			if(userChristmasMission[id] == -1)
				menu_additem(menu, "\dDzis juz losowales!");
			else if(userChristmasMission[id] < str_to_num(christmasMission[userChristmasType[id]][1]))
				menu_additem(menu, "\rWykonaj najpierw wyzwanie");
			else menu_additem(menu, "\yZacznij Losowac\d |\r POWODZENIA!");
		}
	}
	menu_display(id,menu, 0);
}


public menuChristmas_2(id, menu, item){
	
	if(item == MENU_EXIT){
		menu_destroy(menu);
		userMenuChristmas[id] = false;
		return;
		
	}
	new ent = find_ent_by_class(ent, szChristmasTree);

	new Float:fOrigin[3], Float:fOriginEnt[3];
	entity_get_vector(id,	EV_VEC_origin, 	fOrigin);
	entity_get_vector(ent,	EV_VEC_origin, 	fOriginEnt);
	
	if( get_distance_f( fOrigin, fOriginEnt ) >= distanceOpen+10.0){		
		chatPrint(id, PREFIX_LINE, "Wroc do choinki!");
		userMenuChristmas[id] = false;
		return;
	}
	switch(item){
		
		case 0:{
			if(dayCount() < 0 || dayCount() >= CHRISTMAS_TOTAL_MISSION){
				chatPrint(id, PREFIX_LINE, "Na dzisiaj nic nie jest przewidziane^3 |^1 Przepraszam!" );
				userMenuChristmas[id] = false;
				return;
				
			} else  if(!userChristmasStart[id]){
				userChristmasStart[id] = 1;
				chatPrint(id, PREFIX_LINE, "Dziekuje, ze podjoles sie^3 wyzwania!" );
				userMenuChristmas[id] = false;
				return;
				
			} else if( userChristmasMission[id] == -1){
				chatPrint(id, PREFIX_LINE, "Dzisiaj juz^4 losowales!^3 Poczekaj do jutra" );
				userMenuChristmas[id] = false;
				return;
					
			}else if( userChristmasMission[id] < str_to_num(christmasMission[userChristmasType[id]][1]) ){
				
				chatPrint(id, PREFIX_LINE, "Wykonaj najpierw twoje wyzwanie jakie masz zrobic!");
				chatPrint(id, PREFIX_LINE, "I jak zrobisz^3 wroc do mnie^1 zalosowac!");
				userMenuChristmas[id] = false;
				return;
					
			}
			new value;
			switch(userChristmasType[id]){
				case 0:{
			
					value = 1000;
					userNugget[id] 	+=  	value;
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Brylek", value);	
				}
				case 1:{
				
					value = 500;
					addExpToFinal(id, float(value)) ;
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Expa", value);
				}
				case 2:{
					value = random_num(1,10);
					userLuzCoin[id] 	+=  	value;
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Luzaczkow", value);
				}
				case 3:{
					value = random_num(24,48);
					timeVip[id]	= 	max( timeVip[id] + (HOUR*value), get_systime() + (HOUR*value) );
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d Godzin^1 VIPA");
				}
				case 4:{
					new userTimeScrool = (HOUR*random_num(1,4));
					new newLeftExp  =  userScrollExp[id] - playedTime(id);
					if(newLeftExp <= 0){
						userScrollExp[id] = playedTime(id);
						userScrollExp[id] += userTimeScrool;	
					} else userScrollExp[id] += userTimeScrool;	
					
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d Godzin^1 Zwoju Doswiadczenia", (userTimeScrool/HOUR), (userTimeScrool/HOUR) == 1  ? "godzine" : "godziny" );
				}
				case 5:{
					
					new userTimeScrool = (HOUR*random_num(1,4));
					new newLeftNugget  =  userScrollNugget[id] - playedTime(id);
					if(newLeftNugget <= 0){
						userScrollNugget[id] = playedTime(id);
						userScrollNugget[id] += userTimeScrool;
					} else userScrollNugget[id] += userTimeScrool;
					
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d Godzin^1 Zwoju Szczescia", (userTimeScrool/HOUR), (userTimeScrool/HOUR) == 1  ? "godzine" : "godziny" );
					
					if(random(100) < 20){
						addCostumes(id, HAT_AUREOLA);
						chatPrint(id, PREFIX_LINE, "Gratulacje! Udalo Ci sie zdobyc kostium!^4 |^3 Aureola");
					}
				
				}
				case 6:{
					value = 1500;
					userNugget[id] 	+=  	value;
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Brylek", value);
				}
				case 7:{
					
					value = random_num(24,48);
					timeVip[id]	= 	max( timeVip[id] + (HOUR*value), get_systime() + (HOUR*value) );
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d Godzin^1 VIPA", value);
				}
				case 8:{
					value = random_num(1,10);
					userLuzCoin[id] 	+=  	value;
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Luzaczkow", value);
				}
				case 9:{
					value = random_num(1,10);
					userLuzCoin[id] 	+=  	value;
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Luzaczkow", value);
				}
				case 10:{
					value = 900;
					addExpToFinal(id, float(value)); 
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Expa", value);
					
					
				}
				case 11:{
					value = 2000;
					userNugget[id] 	+=  	value;
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Brylek", value);
				}
				case 12:{
					new userTimeScrool = (HOUR*random_num(1,4));
					new newLeftNugget  =  userScrollNugget[id] - playedTime(id);
					if(newLeftNugget <= 0){
						userScrollNugget[id] = playedTime(id);
						userScrollNugget[id] += userTimeScrool;	
					} else userScrollNugget[id] += userTimeScrool;
					
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d Godzin^1 Zwoju Szczescia", (userTimeScrool/HOUR), (userTimeScrool/HOUR) == 1  ? "godzine" : "godziny" );
					
				}
				case 13:{
					value = 1700;
					userNugget[id] 	+=  	value;
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Brylek", value);
				}
				case 14:{
					value = random_num(24,48);
					timeVip[id]	= 	max( timeVip[id] + (HOUR*value), get_systime() + (HOUR*value) );
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d Godzin^1 VIPA", value);
					
					if(random(100) < 15){
						addCostumes(id, HAT_NIEDZWIEDZ);
						chatPrint(0, PREFIX_LINE, "Gracz^3 %s^1 zdobyl kostium!!", userName[id]);
						chatPrint(id, PREFIX_LINE, "Gratulacje! Udalo Ci sie zdobyc kostium!^4 |^3 Czapka Niedzwiedzia");
					}
					
				}
				case 15:{
					value = random_num(1,10);
					userLuzCoin[id] 	+=  	value;
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Luzaczkow", value);
				}
				case 16:{
					value = 950;
					addExpToFinal(id, float(value)); 
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Expa", value);
				}
				case 17:{
					value = 750;
					addExpToFinal(id, float(value)) ;
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Expa", value);
				}
				case 18:{
					value = 1900;
					userNugget[id] 	+=  	value;
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Brylek", value);
				}
				case 19:{
					new userTimeScrool = (HOUR*random_num(1,4));
					new newLeftNugget  =  userScrollNugget[id] - playedTime(id);
					if(newLeftNugget <= 0){
						userScrollNugget[id] = playedTime(id);
						userScrollNugget[id] += userTimeScrool;
					} else userScrollNugget[id] += userTimeScrool;
					
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d Godzin^1 Zwoju Szczescia", (userTimeScrool/HOUR), (userTimeScrool/HOUR) == 1  ? "godzine" : "godziny" );
					
				}
				case 20:{
					new userTimeScrool = (HOUR*random_num(1,4));
					new newLeftExp  =  userScrollExp[id] - playedTime(id);
					if(newLeftExp <= 0){
						userScrollExp[id] = playedTime(id);
						userScrollExp[id] += userTimeScrool;	
					} else userScrollExp[id] += userTimeScrool;	
					
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d Godzin^1 Zwoju Doswiadczenia", (userTimeScrool/HOUR), (userTimeScrool/HOUR) == 1  ? "godzine" : "godziny" );
				
					if(random(100) < 20){
						addCostumes(id, HAT_SWIATECZNASKARPETA);
						chatPrint(id, PREFIX_LINE, "Gratulacje! Udalo Ci sie zdobyc kostium!^4 |^3 Swiateczna Skarpeta");
					}
				}
				case 21:{
					value = random_num(24,48);
					timeVip[id]	= 	max( timeVip[id] + (HOUR*value), get_systime() + (HOUR*value) );
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d Godzin^1 VIPA", value);
				}
				case 22:{
					value = random_num(1,10);
					userLuzCoin[id] 	+=  	value;
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Luzaczkow", value);
				}
				case 23:{
					value = random_num(24,48);
					timeVip[id]	= 	max( timeVip[id] + (HOUR*value), get_systime() + (HOUR*value) );
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d Godzin^1 VIPA", value);
				}
				case 24:{
					value = 2000;
					userNugget[id] 	+=  	value;
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Brylek", value);
					
					if(random(100) < 20){
						addCostumes(id, HAT_BALWAN);
						chatPrint(0, PREFIX_LINE, "Gracz^3 %s^1 zdobyl kostium!!", userName[id]);
						chatPrint(id, PREFIX_LINE, "Gratulacje! Udalo Ci sie zdobyc kostium!^4 |^3 Glowa Balwana");
					}
				}
				case 25:{
					value = random_num(1,10);
					userLuzCoin[id] 	+=  	value;
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Luzaczkow", value);
				}
				case 26:{
					new userTimeScrool = (HOUR*random_num(1,4));
					new newLeftNugget  =  userScrollNugget[id] - playedTime(id);
					if(newLeftNugget <= 0){
						userScrollNugget[id] = playedTime(id);
						userScrollNugget[id] += userTimeScrool;
					} else userScrollNugget[id] += userTimeScrool;
					
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d Godzin^1 Zwoju Szczescia", (userTimeScrool/HOUR), (userTimeScrool/HOUR) == 1  ? "godzine" : "godziny" );
				}
				case 27:{
					new userTimeScrool = (HOUR*random_num(1,4));
					new newLeftNugget  =  userScrollNugget[id] - playedTime(id);
					if(newLeftNugget <= 0){
						userScrollNugget[id] = playedTime(id);
						userScrollNugget[id] += userTimeScrool;	
					} else userScrollNugget[id] += userTimeScrool;
					
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d Godzin^1 Zwoju Szczescia", (userTimeScrool/HOUR), (userTimeScrool/HOUR) == 1  ? "godzine" : "godziny" );
				}
				case 28:{
					value = random_num(1,10);
					userLuzCoin[id] 	+=  	value;
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d^1 Luzaczkow", value);
				}
				case 29:{
					value = random_num(24,48);
					timeVip[id]	= 	max( timeVip[id] + (HOUR*value), get_systime() + (HOUR*value) );
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d Godzin^1 VIPA", value);
				}
				case 30:{
					new userTimeScrool = (HOUR*random_num(1,4));
					new newLeftExp  =  userScrollExp[id] - playedTime(id);
					if(newLeftExp <= 0){
						userScrollExp[id] = playedTime(id);
						userScrollExp[id] += userTimeScrool;	
					} else userScrollExp[id] += userTimeScrool;	
					
					chatPrint(id, PREFIX_LINE, "Otrzymale^3 %d Godzin^1 Zwoju Doswiadczenia", (userTimeScrool/HOUR), (userTimeScrool/HOUR) == 1  ? "godzine" : "godziny" );
				
					if(random(100) < 20){
						addCostumes(id, HAT_PINGWINNAPLECACH);
						chatPrint(0, PREFIX_LINE, "Gracz^3 %s^1 zdobyl kostium!!", userName[id]);
						chatPrint(id, PREFIX_LINE, "Gratulacje! Udalo Ci sie zdobyc kostium!^4 |^3 Pingwin na Plecach");
					}
				}
			}
			userChristmasMission[id] = -1;
			
		}
	}
	userMenuChristmas[id] = false;
}
createTree(Float:fOrigin[3], solid){
	new ent = create_entity("info_target");
	
	if( !pev_valid(ent)) return 0;
	
	entity_set_string(	ent, 	EV_SZ_classname,	szChristmasTree);
	entity_set_model(	ent, 	szTreeModel);
	entity_set_vector(	ent, 	EV_VEC_origin, 		fOrigin );
	
	if(solid == SOLID_BBOX) entity_set_size(ent, 	Float:{-15.0, -15.0, -0.0}, Float:{15.0, 15.0, 125.0});
	
	entity_set_int(		ent, 	EV_INT_solid, 		solid );
	entity_set_int(		ent, 	EV_INT_movetype, 	MOVETYPE_FLY );	
	entity_set_int(		ent, 	EV_INT_sequence, 	1);
	entity_set_float(	ent,	EV_FL_nextthink,	get_gametime() + 0.1 );
	
	drop_to_floor(ent);
	return ent;
}

createSprite(Float:fOrigin[3]){
	new ent = create_entity("info_target");
	
	if (!pev_valid(ent)) return 0;
	
	fOrigin[2] += 1.0;
	
	entity_set_string(	ent, 	EV_SZ_classname, 	szChristmasTreeSprite);
	entity_set_model(	ent, 	szTreeSprite);
	entity_set_int(		ent, 	EV_INT_movetype, 	MOVETYPE_FLY);
	entity_set_int(		ent, 	EV_INT_solid, 		SOLID_NOT);
	entity_set_vector(	ent, 	EV_VEC_origin, 		fOrigin);
	entity_set_vector(	ent, 	EV_VEC_angles, 		Float:{90.0, 0.0, 0.0});
	entity_set_int(		ent, 	EV_INT_rendermode, 	5);
	entity_set_float(	ent, 	EV_FL_renderamt,	255.0);
	entity_set_int(		ent, 	EV_INT_renderfx,	5);
	entity_set_float(	ent,	EV_FL_frame, 		0.0 );	
	entity_set_float(	ent, 	EV_FL_scale,		0.45);
	entity_set_float(	ent, 	EV_FL_animtime,		get_gametime());
	entity_set_float(	ent, 	EV_FL_fuser1,		get_gametime());
	entity_set_float(	ent, 	EV_FL_framerate,	1.0);
	entity_set_float(	ent,	EV_FL_nextthink,	get_gametime() + 0.1 );

	drop_to_floor(ent);
	return ent;
}
public treeThink(ent){
	if( !pev_valid(ent) && !isTree(ent)) return;
	
	new Float:fAngles[3];
	new frame = floatround(entity_get_float( ent, EV_FL_frame) + 1.0 ) % 9;
	entity_set_float(	ent, 	EV_FL_frame, 		float(frame));

	new Float:fOriginTarget[3];

	entity_get_vector(ent, EV_VEC_origin, fOriginTarget);
	if(get_gametime() - entity_get_float(ent, EV_FL_fuser1) > 0.2 ){
		fOriginTarget[2] += 132.5;
		Particles(fOriginTarget, 3.0, sprite_pouIce, random_num(1,4), 2, random_num(2,3), random_num(4,10), 2);
		entity_set_float(ent, EV_FL_fuser1, get_gametime());
	}
	
	entity_get_vector(	ent, 	EV_VEC_angles, 		fAngles);
	fAngles[1] += 5.0;
	entity_set_vector(	ent, 	EV_VEC_angles, 		fAngles);
	entity_set_float(	ent,	EV_FL_nextthink,	get_gametime() + 0.1 );
}
stock bool:isTree(ent){
	new szClass[14];
	entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass) );
	if(equal(szClass, szChristmasTree) )
		return true;		
	return false;
}
saveChristmas( ) {

	if(file_exists(configFile )) delete_file(configFile);
	
	new file = fopen(configFile, "wt");
	
	new szData[128];
	new Float:fOrigin[ 3 ];
	new ent = -1;
	
	if(file){
		while( ( ent = find_ent_by_class( ent, szChristmasTree))) {
			
			if(!pev_valid(ent)) continue;
			
			entity_get_vector( ent, EV_VEC_origin, fOrigin );

			format(szData, sizeof(szData), "%f %f %f^n", fOrigin[ 0 ], fOrigin[ 1 ], fOrigin[ 2 ]);
			fputs(file, szData);
		}
		fclose(file);
	}
}

public createChristmasFolder(){
	new szDir[128];

	get_configsdir(szDir, sizeof(szDir));	
	
	new const christmasFolder[] = "christmas";
	
	new firstFolder[64];

	format(firstFolder, sizeof(firstFolder) - 1, "%s/%s", szDir, christmasFolder);
	
	if(!dir_exists(firstFolder)){
		log_amx("=== Stworzono glowny folder swiateczny: %s ===", christmasFolder);
		mkdir(firstFolder);
	}
	
	
}

loadChristmas(){
	
	new map[ 33 ];
	get_mapname(map,sizeof(map));
	
	format(configFile, sizeof(configFile), "addons/amxmodx/configs/christmas" );
	
	if(!dir_exists(configFile)){
		mkdir(configFile);
		format(configFile, sizeof(configFile), "%s/%s.txt", configFile, map );
		return PLUGIN_CONTINUE;
	}
	
	format(configFile, sizeof(configFile), "%s/%s.txt", configFile, map );
	
	if( !file_exists(configFile)) return PLUGIN_CONTINUE;
	
	new file = fopen(configFile, "rt" );
	
	if( !file ) return PLUGIN_CONTINUE;
	
	new szData[1024];
	
	new Float:fOrigin[3];

	new szOrigin[3][17];

	
	while(!feof(file)){
		fgets(file, szData, sizeof( szData ) );
		trim( szData );
		
		if( !szData[ 0 ] ) continue;
		
		parse(szData,
			szOrigin[0], 	sizeof(szOrigin[]),
			szOrigin[1], 	sizeof(szOrigin[]),
			szOrigin[2], 	sizeof(szOrigin[])
		);
		
		for( new i = 0;i <3; i ++ ) fOrigin[i]=str_to_float(szOrigin[i]);
		
		
		createTree( fOrigin, SOLID_BBOX );
		createSprite(fOrigin);
	}
	
	fclose(file);
	return PLUGIN_CONTINUE;
}
public granadeSetModel(id){
	new weapon = get_user_weapon(id);
	if(weapon == CSW_HEGRENADE || weapon == CSW_SMOKEGRENADE || weapon == CSW_FLASHBANG){
		entity_set_string(id , EV_SZ_viewmodel , modelGranade[CHRISTMAS_V]);
	}

}

public soundPrep(){
	if(gTime == timePlayChristmas[randomSoundChristmas] && !gameTime && !clockStop && prepTime){
		cmd_execute(0, "mp3 play sound/basebuildervt/christmas/%d.mp3", randomSoundChristmas+1);
	}
}
public cmdUnstuckTree(id){

	static Float:origin[3];
	static Float:mins[3], hull;
	static Float:vec[3];
	static o;
	if (is_user_connected(id) && is_user_alive(id)){
		pev(id, pev_origin, origin);
		hull = pev(id, pev_flags) & FL_DUCKING ? HULL_HEAD : HULL_HUMAN;
		if (!is_hull_vacant(origin, hull,id) && !get_user_noclip(id) && !(pev(id,pev_solid) & SOLID_NOT)) {
			++stuck[id];
			pev(id, pev_mins, mins);
			vec[2] = origin[2];
			for (o=0; o < sizeof size; ++o) {
				vec[0] = origin[0] - mins[0] * size[o][0];
				vec[1] = origin[1] - mins[1] * size[o][1];
				vec[2] = origin[2] - mins[2] * size[o][2];
				if (is_hull_vacant(vec, hull,id)) {
					engfunc(EngFunc_SetOrigin, id, vec);
					Display_Fade(id, 512,512,512, 255,32,32, 90);
					set_dhudmessage(255, 32, 32, -1.0, 0.3, 0, 0.5, 0.9, 0.5, 0.5);
					show_dhudmessage(id, "!! Odblokowano !!");
					set_pev(id,pev_velocity,{0.0,0.0,0.0});
					o = sizeof size;
				}	
			}
		}
		else stuck[id] = 0;	
	}
	return PLUGIN_CONTINUE;

}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ ansicpg1252\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}{\\ f1\\ fnil\\ fcharset0 Tahoma;}{\\ f2\\ fnil\\ fcharset238 Tahoma;}}\n{\\ colortbl ;\\ red0\\ green0\\ blue0;}\n\\ viewkind4\\ uc1\\ pard\\ cf1\\ lang1045\\ f0\\ fs16\\ tab\\ tab   \\ { "Zabij 100 zombie", \\ tab\\ tab\\ tab "100" \\ }\\ tab // 1\n\\ par \\ tab\\ tab ,\\ { "Podnies 500 brylek", \\ tab\\ tab\\ tab "500" \\ }\\ tab // 2\n\\ par \\ tab\\ tab ,\\ { "Zgin jako pierwszy 20 razy", \\ tab\\ tab "20" \\ }\\ tab // 3\n\\ par \\ tab\\ tab ,\\ { "Zabij 30 budowniczych",\\ tab\\ tab\\ tab "30" \\ }\\ tab // 4\n\\ par \\ tab\\ tab ,\\ { "Podnies 2 skrzynki",\\ tab\\ tab\\ tab "2" \\ }\\ tab // 5\n\\ par \\ tab\\ tab ,\\ { "Zdobadz 700 punktow ( TOP/RANK ) ", \\ tab "700" \\ } \\ tab // 6\\ tab\\ tab // AUREOLA\\ f1  \\ f2 // 5\\ f0 \n\\ par \\ tab\\ tab ,\\ { "Odbierz bron 20 razy",\\ tab\\ tab\\ tab "20" \\ }\\ tab // 7\n\\ par \\ tab\\ tab ,\\ { "Zabij 160 zombie", \\ tab\\ tab\\ tab "160" \\ }\\ tab // 8\n\\ par \\ tab\\ tab ,\\ { "Zgin 70 razy", \\ tab\\ tab\\ tab\\ tab "70" \\ }\\ tab // 9\n\\ par \\ tab\\ tab ,\\ { "Zabij 150 zombie", \\ tab\\ tab\\ tab "150" \\ }\\ tab // 10\n\\ par \\ tab\\ tab ,\\ { "Zabij 100 osob z glocka", \\ tab\\ tab\\ tab "100" \\ }\\ tab // 11\\ tab\\ tab // NIEDZWIEDZ\\ f1  \\ f2 // 10\\ f0 \n\\ par \\ tab\\ tab ,\\ { "Uzyj mocy 20 razy", \\ tab\\ tab\\ tab "20" \\ }\\ tab // 12\n\\ par \\ tab\\ tab ,\\ { "Zabij 40 szybkich zombie", \\ tab\\ tab "40" \\ }\\ tab // 13\n\\ par \\ tab\\ tab ,\\ { "Podnies 10 czarnych brylek",\\ tab\\ tab "10" \\ }\\ tab // 14\n\\ par \\ tab\\ tab ,\\ { "Zakup 20 apteczek", \\ tab\\ tab\\ tab "20" \\ }\\ tab // 15\n\\ par \\ tab\\ tab ,\\ { "Zgin jako druga osoba 10 razy",\\ tab\\ tab "10"\\ }\\ tab // 16\n\\ par \\ tab\\ tab ,\\ { "Zakup klase obojetnie jaka",\\ tab\\ tab "1"\\ }\\ tab // 17\n\\ par \\ tab\\ tab ,\\ { "Ustrzel 100 zielonych grzybkow", \\ tab\\ tab "100" \\ }\\ tab // 18\n\\ par \\ tab\\ tab ,\\ { "Zdobadz 50 punktow na grzybkach 5 razy",\\ tab "5" \\ }\\ tab // 19\n\\ par \\ tab\\ tab ,\\ { "Kup 20 razy granat ( Obojetnie )", \\ tab\\ tab "20" \\ }\\ tab // 20\n\\ par \\ tab\\ tab ,\\ { "Ustrzel 30 zlotych grzybkow",\\ tab\\ tab "30" \\ }\\ tab // 21\\ tab\\ tab // SKARPETA\\ f1  \\ f2 // 20\\ f0 \n\\ par \\ tab\\ tab ,\\ { "Zgin 100 razy jako klasyczne zombie",\\ tab "100" \\ }\\ tab // 22\n\\ par \\ tab\\ tab ,\\ { "Wydrop 5 skrzynek",\\ tab\\ tab  \\ tab "5" \\ }\\ tab // 23\n\\ par \\ tab\\ tab ,\\ { "Zdobacz 400 zlotych brylek",\\ tab\\ tab\\ tab "400" \\ }\\ tab\\ f1\\ tab\\ f0 \n\\ par \\ tab\\ tab ,\\ { "Tylko nie liczni znaja ta misje",\\ tab\\ tab  "1" \\ }\\ tab // 25\\ f2\\ tab\\ tab // BAWLWAN // 24\\ f0 \n\\ par \\ tab\\ tab ,\\ { "Otruj 20 budowniczych",\\ tab\\ tab\\ tab  "20" \\ }\\ tab // 26\n\\ par \\ tab\\ tab ,\\ { "Rozegraj 15 rund",\\ tab\\ tab\\ tab  "15" \\ }\\ tab // 27\n\\ par \\ tab\\ tab ,\\ { "Zadaj 50.000 obrazen",\\ tab\\ tab\\ tab  "50000" \\ }// 28\n\\ par \\ tab\\ tab ,\\ { "Zabij 200 zombie", \\ tab\\ tab\\ tab "200" \\ }\\ tab // 29\n\\ par \\ tab\\ tab ,\\ { "Zgin jako trzecia osoba 10 razy",\\ tab\\ tab "10"\\ }\\ tab // 30\n\\ par \\ tab\\ tab ,\\ { "Odbierz bron 30 razy",\\ tab\\ tab\\ tab "30" \\ }\\ tab // 31\\ tab\\ tab // PINGWIN NA PLECACH\\ f1  \\ f2 // 30\\ f0 \n\\ par }
*/
