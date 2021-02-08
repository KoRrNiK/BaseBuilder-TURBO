#include <	amxmodx		>
#include <	amxmisc		>
#include <	cstrike		>
#include <	fun		>
#include <	fakemeta	>
#include <	fakemeta_util	>
#include <	engine		>
#include <	csx		>
#include <	fvault		>
#include <	StripWeapons	>	
#include <	http2		>
#include <	xs		>
#include <	sockets		>
#include <	tutor		>
#include <	sqlx		>
#include <	regex		>
#include <	colorchat	>

public costumePrecache(){

	
	for(new i = 0; i < 4; i ++){
		precache_model(modelsCostumes[i][0])
	}

	
}

stock bool:hasCostumes(id, hat){
	new type = typeHat(hat)
	return !!(userHat[id][type] & (1<< ( hat - 32 * type )));
}
stock addCostumes(id, hat){
	new type = typeHat(hat)
	userHat[id][type] |= (1<< ( hat - 32 * type))
	
	new gText[128]
	logType[id] = LOG_HAT_ADD;
	if(logType[id] == LOG_HAT_ADD){
		if(is_user_connected(id) || !is_user_bot(id) || !is_user_hltv(id)){
			format(gText, sizeof(gText), "otrzymal kostium: %s od: %s", costumesNames[hat][0], userName[id]);
		}
	}
	logBB(id,gText)
		
	
}
stock removeCostumes(id, hat){
	new type = typeHat(hat)
	userHat[id][type]  &= ~(1<< ( hat - 32 * type))
	
	new gText[128]
	logType[id] = LOG_HAT_REMOVE;
	if(logType[id] == LOG_HAT_REMOVE){
		if(is_user_connected(id) || !is_user_bot(id) || !is_user_hltv(id)){
			format(gText, sizeof(gText), "usunal kostium: %s garczowi: %s", costumesNames[hat][0], userName[id]);
		}
	}
	logBB(id,gText)
}
stock typeHat(i){
	//return i >= 32 ? 1 : 0
	return i >= 96 ? 3 : i >= 64 ? 2 : i >= 32 ? 1 : 0
}
public allunlockHat(id){
	new iNum = 0;
	for(new i = 0; i < MAXHAT; i ++){
		if(!hasCostumes(id, i)) continue
		iNum ++;
	}
	return iNum;
}

public globalMenuCostumes(id){
	
	if(!isSuperAdmin(id)){
		menuCostumes(id);
		return;
	}
		
	
	
	
	new menu = menu_create("\r[BaseBuilder]\y Kostiumy!", "globalMenuCostumes_2");
	
	menu_additem(menu, "Twoje Kostiumy");
	menu_additem(menu, "Wszystkie Kostiumy");
	
	menu_display(id, menu, 0);
	
}
public globalMenuCostumes_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return;
	}
	
	switch(item){
		case 0:{
			menuCostumes(id);
		}
		case 1:{
			menuCostumesAll(id)
		}
		
	}
	

}	

public menuCostumes(id){
	
	new gText[512], iLen = 0;
	iLen 	+=	format(gText[iLen], sizeof(gText) - iLen - 1, "\r[BaseBuilder]\y Twoje Kostiumy!\d [ %d/%d ]^n^n",allunlockHat(id), MAXHAT);
	
	if(userSelectHat[id] != -1)
		iLen 	+=	format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dAktualny Kostium:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], costumesNames[userSelectHat[id]][0])
	else iLen 	+=	format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dAktualny Kostium:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], "Nie ubrane")
	
	if((userSelectNewHat[id] != userSelectHat[id])) 
		iLen 	+=	format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dNowy Kostium:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], costumesNames[userSelectNewHat[id]][0])
		
	iLen 	+=	format(gText[iLen], sizeof(gText)-iLen-1, "^n\d---\y Twoja lista kostiumow\w ")	
		
	new menu = menu_create(gText, "menuCostumes_2");
	
	new x= 0;
	for(new i = 0; i < MAXHAT; i ++){
	
		if(!hasCostumes(id, i))		
			continue;
			
		if((userSelectNewHat[id] == i) && (userSelectNewHat[id] != userSelectHat[id])) {	
			format(gText, sizeof(gText), "\r*%s", costumesNames[i][0])	
		} else {
			format(gText, sizeof(gText), "%s%s",  userSelectHat[id]  == i ? "\r" :"\w", costumesNames[i][0])  
		}
		
		
		menu_additem(menu, gText);
		
		selectHat[id][x++] = i
	
		
	}
	
	if(!x) ColorChat(id, GREEN, "---^x01 Nie posiadasz kostiumow!^x04 ---");
	else menu_display(id, menu, menuPage[id][0]/7);
	
}
public menuCostumes_2(id, menu, item){
	
	if(item == MENU_EXIT){
		menu_destroy(menu)
		return;
	}
	
	menuPage[id][0] = item;
	
	
	
	new hat = selectHat[id][item]
	userVarMenu[id] = hat;

	/*if( userSelectHat[id] != hat ){
		if( buildTime || prepTime ){
			ColorChat(id, GREEN,"---^x01 Twoj nowy kostium:^x03 %s^x04 ---", costumesNames[hat] )
						
			userSelectHat[id] = hat;
			userSelectNewHat[id] = hat
			chechHat(id, hat)
		}else{
			userSelectNewHat[id] = hat
			ColorChat(id, GREEN,"---^x01 Twoj kostium zmieni sie po odrodzeniu na:^x03 %s^x04 ---",  costumesNames[hat]  )
		}
					
	} else {
		if( buildTime || prepTime ){	
			userSelectHat[id] = -1;
			userSelectNewHat[id] = -1
			removeHat(id)
			ColorChat(id, GREEN,"---^x01 Zdjales kostium^x04 ---"); 
		} else {
			if(userSelectHat[id] == hat){
				ColorChat(id, GREEN,"---^x01 Aktualnie masz to ubrane!^x04 ---"); 
				ColorChat(id, GREEN,"---^x01 Zdejmowac kostiumy mozesz tylko podczas^x03 budowania^x01 i^x03 przygotowania!^x04 ---"); 
			}
		}
	}
	*/
	infoCostumes(id)
	
	
}




public infoCostumes(id){
	
	new hat = userVarMenu[id]
	new gText[512], iLen

	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Informacje o Kostiumie!^n^n")
	
	//iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\w[ Informacje ]^n")
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dKostium:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW],costumesNames[hat][0])
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dBonus:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW],costumesNames[hat][1])
	
	if(userSelectHat[id] != -1) iLen 	+=	format(gText[iLen], sizeof(gText)-iLen-1, "\y%s^t^t\dAktualnie ubrany masz:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], costumesNames[userSelectHat[id]][0])
	
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r1. Zaloz")	
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r2.\y Wroc")

	show_menu(id, B1 | B2  , gText, -1, "infoCostumes"  )
	return PLUGIN_CONTINUE;
}
public infoCostumes_2(id, item){

	new hat = userVarMenu[id]

	switch(item){
		
		case 0:{
			if( userSelectHat[id] != hat ){
				if( buildTime || prepTime ){
					ColorChat(id, GREEN,"---^x01 Twoj nowy kostium:^x03 %s^x04 ---", costumesNames[hat][0] )
								
					userSelectHat[id] = hat;
					userSelectNewHat[id] = hat
					chechHat(id, hat)
				}else{
					userSelectNewHat[id] = hat
					ColorChat(id, GREEN,"---^x01 Twoj kostium zmieni sie po odrodzeniu na:^x03 %s^x04 ---",  costumesNames[hat][0]  )
				}
							
			} else {
				if( buildTime || prepTime ){	
					userSelectHat[id] = -1;
					userSelectNewHat[id] = -1
					removeHat(id)
					ColorChat(id, GREEN,"---^x01 Zdjales kostium^x04 ---"); 
				} else {
					if(userSelectHat[id] == hat){
						ColorChat(id, GREEN,"---^x01 Aktualnie masz to ubrane!^x04 ---"); 
						ColorChat(id, GREEN,"---^x01 Zdejmowac kostiumy mozesz tylko podczas^x03 budowania^x01 i^x03 przygotowania!^x04 ---"); 
					}
				}
			}
		}
		default:{ 
			menuCostumes(id)
		}
	}
	return PLUGIN_CONTINUE;
}

public menuCostumesAll(id){
	
	new gText[128], menu = menu_create("\r[BaseBuilder]\y Kostiumy!", "menuCostumesAll_2");
	
	for(new i = 0; i < MAXHAT; i ++){
		
		format(gText, sizeof(gText), "%s^t%s\d |\w ID TYPE:\r %d", costumesNames[i][0], hasCostumes(id, i) ? "\r[ Posiadasz ]" : "", typeHat(i));
		menu_additem(menu, gText);
		
	}
	menu_display(id, menu, menuPage[id][1]/7);
	
	
}
public menuCostumesAll_2(id, menu, item){
	
	if(item == MENU_EXIT){
		menu_destroy(menu)
		return;
	}
	menuPage[id][1] = item;
	
	if(hasCostumes(id, item)){
		removeCostumes(id, item)
		ColorChat(id, GREEN, "Usunales sobie kostuim: %s", costumesNames[item][0]);
	} else {
		ColorChat(id, GREEN, "Dodales sobie kostuim: %s", costumesNames[item][0]);
		addCostumes(id, item)
	}
	menuCostumesAll(id)
	
}
public createHat(id, anim){
	if(!is_user_alive(id) || pev_valid(fm_find_ent_by_owner(-1, "player_hat", id)))
		return PLUGIN_CONTINUE
		
	new ent = engfunc(EngFunc_CreateNamedEntity, engfunc(EngFunc_AllocString, "info_target"))
	set_pev(ent, pev_classname, "player_hat")
	
	new model[64];
	for(new i = 0; i < ALLFILEMODELS; i ++){
		if(anim >= str_to_num(modelsCostumes[i][1]) && anim <= str_to_num(modelsCostumes[i][2])){
			format(model, sizeof(model), modelsCostumes[i][0]);
			anim -= str_to_num(modelsCostumes[i][1]); 
		}
	}
	
	engfunc(EngFunc_SetModel, ent, model)
	
	set_pev(ent, pev_movetype, MOVETYPE_FOLLOW)
	set_pev(ent, pev_aiment, id)
	set_pev(ent, pev_owner, id)
	set_pev(ent, pev_rendermode, kRenderNormal)
	set_pev(ent, pev_body, anim)
	set_pev(ent, pev_sequence, anim)
	set_pev(ent, pev_animtime, get_gametime())
	set_pev(ent, pev_framerate, 1.0)
	
	return PLUGIN_CONTINUE

}
public removeHat(id){
            new ent = fm_find_ent_by_owner(-1, "player_hat", id)
            if(pev_valid(ent)) engfunc(EngFunc_RemoveEntity, ent)
}
public setHat(id){
	
	if( userSelectNewHat[id] != userSelectHat[id] )
		userSelectHat[id] = userSelectNewHat[id]
			
	if(get_user_team(id) == 2 && userSelectHat[id] !=  -1){
		chechHat(id, userSelectHat[id])
	} else removeHat(id)
}
public chechHat(id, &hat){
	if(get_user_team(id) == 2 && userSelectHat[id] !=  -1){
		removeHat(id)		
		createHat(id, hat)
	} else removeHat(id)
}

public bool:foundedHat(id, hat){
	if(get_user_team(id) == 2 && userSelectHat[id] == hat)
		return true;
	return false;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
