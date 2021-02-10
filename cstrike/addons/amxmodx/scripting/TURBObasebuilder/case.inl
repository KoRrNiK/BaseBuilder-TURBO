#include < amxmodx >
#include < amxmisc >
#include < fakemeta >
#include < engine >
#include < fun >
#include < cstrike >
#include < fakemeta_util > 

new activeMultiply;

new freeChest; 
new const createInfoMenu[][] = {
	"Kursor",
	"Gracz"
}

public Float:dropChest(id){
	
	return ((bbCvar[cvarNightMultiplyEnable] && activeMultiply ? bbCvar[cvarDropPercentCase]*bbCvar[cvarNightMultiply] : bbCvar[cvarDropPercentCase] ) +
	(userClassHuman[id] == human_SEARCH ? str_to_float(paramClassesHuman[human_SEARCH][0])* userHumanLevel[id][human_SEARCH] : 0.0) +
	(userClassHuman[id] == human_SEARCH  && didPro(id, pro_CASE) ? 0.5 : 0.0 ) ) *
	(( hourTime && bb_happy_hour() == happy_CASE ) ? 2.0 : 1.0);
	
}
public casePrecache(){
	precache_model(caseModel)	
}
public menuCreateCase(id){
	new menu = menu_create("\r[BaseBuilder]\y Menu tworzenia skrzynek", "menuCreateCase_2");
	
	if(caseCreate[id][CREATE_CASE_PLAYER] == 0)
		menu_additem(menu,"Wlasnosc:\y Serwera");
	else menu_additem(menu, formatm("Wlasnosc:\r %s", userName[caseCreate[id][CREATE_CASE_PLAYER]]));
	
	menu_additem(menu,formatm("Pozycja:\y %s", createInfoMenu[caseCreate[id][CREATE_CASE_TYPE]]));
	menu_additem(menu,formatm("Czas: %s", caseCreate[id][CREATE_CASE_TIME] ? "\rTAK" : "\dNIE"));
	menu_additem(menu,"Stworz^n");
	
	menu_additem(menu, "Anuluj");
	
	menu_display(id, menu, 0);
	
}
public menuCreateCase_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return;
	}
	switch(item){
		case 0:{
			userMenuPlayer[id] = MENU_CASE_CREATE
			choosePlayer(id, 0)
		}
		case 1:{
			
			caseCreate[id][CREATE_CASE_TYPE] = (caseCreate[id][CREATE_CASE_TYPE]+1) 	% sizeof(createInfoMenu);
	
			menuCreateCase(id)	
		}
		case 2:{
			caseCreate[id][CREATE_CASE_TIME] =! caseCreate[id][CREATE_CASE_TIME];
			menuCreateCase(id)
		}
		case 3:{
			if(caseCreate[id][CREATE_CASE_TYPE] && !caseCreate[id][CREATE_CASE_PLAYER]){
				ColorChat(id, GREEN, "---^x01 Nie mozliwe^x04 ---");
				menuCreateCase(id)
				return 
			}
			if(caseCreate[id][CREATE_CASE_TYPE]){
				new Float:fOrigin[3];
				pev(caseCreate[id][CREATE_CASE_PLAYER], pev_origin, fOrigin);	
				
				createCases(fOrigin, .disappear = caseCreate[id][CREATE_CASE_TIME], .owner = caseCreate[id][CREATE_CASE_PLAYER])
				
			} else {
				new iLook[3]
				new Float:fLook[3]
				get_user_origin(id, iLook, 3)
				IVecFVec(iLook, fLook)	
					
				fLook[2]+=10.0;
				createCases(fLook, .disappear = caseCreate[id][CREATE_CASE_TIME], .owner = caseCreate[id][CREATE_CASE_PLAYER])
			}
			menuCreateCase(id)
			
		}
		case 4:{
			caseCreate[id][CREATE_CASE_TYPE] = 0;
			caseCreate[id][CREATE_CASE_PLAYER] = 0;
			caseCreate[id][CREATE_CASE_TIME] = false;
			menuCreateCase(id)
		}
	}
}
public createCase(id, id2){
	
	new Float:fOrigin[3];
	pev(id2, pev_origin, fOrigin);
	
	#if defined CHRISTMAS_ADDON
		
		addChristmasMission(id, CH_DROPCHEST, 1);
				
	#endif
	
	createCases(fOrigin, .disappear = 1, .owner = id)

	
}
public addCaseEnt(){
	new slotCase = freeCaseSlot();
	if( slotCase!=-1){
		new ent = create_entity("info_target")
		if( !pev_valid(ent) )
			return 0;
		listCaseOnFloor[slotCase] = ent;
		set_pev(ent, pev_iuser1, slotCase)

		return ent;
	}
	return 0
}
public freeCaseSlot(){
	for( new i = 0 ; i < MAXCASESFLOOR; i ++ ){
		if( listCaseOnFloor[i] == 0 )
			return i;
	}
	return -1;
}
createCases(Float:fOrigin[3], owner = 0, disappear = 0){
	
	new ent = -1;
	
	ent = addCaseEnt();
	
	if( ent == 0) return;

	set_pev(ent, pev_classname, caseClass)
	
	entity_set_int(ent, 	EV_INT_solid,		SOLID_BBOX)
	entity_set_int(ent,	EV_INT_movetype, 	MOVETYPE_NONE)
	
	
	set_pev(ent, pev_fuser1, get_gametime())
	set_pev(ent, pev_fuser2, 15.0)
	set_pev(ent, pev_fuser3, get_gametime())
	set_pev(ent, pev_fuser4, 35.0)
	

	set_pev(ent, pev_iuser2, disappear)
	set_pev(ent, pev_owner, 	owner)
	
	set_rendering(ent, kRenderFxGlowShell, 	240, 133, 56, 	kRenderNormal, 15)
	
	new Float:fAngles[3]
	new Float:fMin[3] = {-4.0, -4.0, -16.0}
	new Float:fMax[3] = {  4.0,   4.0,   16.0}	
	
	fAngles[1] = random_float(0.0, 360.0);
	fOrigin[2] -= distanceToFloor(fOrigin)-5;
	
	set_pev(ent, pev_angles, fAngles)
	set_pev(ent, pev_origin, fOrigin)
	
	engfunc( EngFunc_SetSize, ent , fMin, fMax);
	
	entity_set_model(ent, 	caseModel)
	set_pev(ent, pev_nextthink, get_gametime()+0.1)
}
public chestThink(ent){
	if( !pev_valid(ent) ) return
	
	if( get_gametime() - pev(ent, pev_fuser3) > 1.5 ){
		new Float:fOrigin[3]
		pev(ent, pev_origin, fOrigin)
		makeLight(fOrigin,16,204, 102, 0,5, 10)
		BeamCylinder(fOrigin, 70.0, spriteBeam, 0, 5, 10, 1, 50, 204, 102, 0, 255, 1)
		set_pev(ent, pev_fuser3, get_gametime())
	}
	
	if((get_gametime() - pev(ent, pev_fuser1) > pev(ent, pev_fuser4)+1.0) && pev(ent, pev_iuser2) == 1){
		removeChestEnt(ent)
		
	}else set_pev(ent, pev_nextthink, get_gametime()+0.1)	
	
}

public caseInfo(id, ent, Float:cordx, Float:cordy){
	new gText[256], iLen
	
	new iAiming[3], Float:fAiming[3]
	get_user_origin(id, iAiming, 3);
	IVecFVec(iAiming, fAiming);
	
	new Float:fOrigin[3], Float:fOriginEnt[3]
	pev(id, pev_origin, fOrigin)
	new entClose = 0, entChest=0
	for( new i = 0 ; i < MAXCASESFLOOR ; i ++ ){
		entChest = listCaseOnFloor[i]
		if( !pev_valid(entChest) )
			continue;
		pev(entChest, pev_origin, fOriginEnt)
		if( get_distance_f(fOriginEnt, fAiming) > 40.0)
			continue
		entClose=entChest
		break;
	}
	
	if( entClose > 0 ){
		if( get_gametime()-userAimingHud[id] >= 0.2){
			iLen += format(gText[iLen], sizeof(gText)-1-iLen, "Skrzynia | Kliknij 'E' aby otworzyc^n");
			if(pev(entClose, pev_iuser2) == 1)
				iLen += format(gText[iLen], sizeof(gText)-1-iLen, "Za %d sek skrzynia zniknie!^n", floatround((pev(entClose, pev_fuser4)+1.0) - (get_gametime() - pev(entClose, pev_fuser3))));
			
			if( get_gametime() - pev(entClose, pev_fuser1) < pev(entClose, pev_fuser2)+1.0 && pev(entClose, pev_owner) != 0){
				iLen += format(gText[iLen], sizeof(gText)-1-iLen, "Przez %d sek moze otworzyc ja tylko: %s^n", floatround(pev(entClose, pev_fuser2)+1.0 - (get_gametime() - pev(entClose, pev_fuser3))),  userName[pev(entClose, pev_owner)])
				iLen += format(gText[iLen], sizeof(gText)-1-iLen, "Po tym czasie moze ja otworzyc kazdy!^n");
			}
			set_hudmessage(userHud[id][PLAYER_HUD_RED], userHud[id][PLAYER_HUD_GREEN], userHud[id][PLAYER_HUD_BLUE], cordx, cordy, 0, 0.1, 0.1, 0.1, 0.1)
			show_hudmessage(id, "%s", gText)
			
			userAimingHud[id] = get_gametime();
		}
		pev(id, pev_origin, fOrigin)
		if( pev(id, pev_button) & IN_USE ){
			if( !userPressOpen[id] ){
				if( get_distance_f(fOrigin, fOriginEnt) < 85.0){
					if(pev(entClose, pev_owner) == id || pev(entClose, pev_owner) == 0 ||  get_gametime() - pev(entClose, pev_fuser1) > pev(entClose, pev_fuser2)){
						openChest(id, entClose)
					}
					
				} else ColorChat(id, GREEN,"---^x01 Podejdz blizej zeby otworzyc^x04 ---")
			}
			
			userPressOpen[id] = true;
		}else userPressOpen[id] = false
	}
}

public openChest(id,ent){
	new Float:fOrigin[3];
	new RandomValue = 0;
	new gText[256], iLen = 0;
	
	pev(ent, pev_origin, fOrigin)
	
	if(pev(ent, pev_owner) == 0)
		ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 otworzyl^x04 skrzynie", PREFIXSAY, userName[id]);
	else ColorChat(0, GREEN, "%s Gracz^x03 %s^x01 otworzyl^x04 skrzynie^x03 |^x01 Wlasnosc:^x04 %s", PREFIXSAY, userName[id], userName[pev(ent, pev_owner)]);
	
	if(userClassHuman[id] == human_SEARCH) addPro(id, pro_CASE, 1)
	
	#if defined CHRISTMAS_ADDON
		
		addChristmasMission(id, CH_CHEST, 1);
				
	#endif
	
	addSecretMission(id, mission_secret_OPENER, 1)
	
	fOrigin[2] += 5.0
	
	iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "--------------------^n");
	
	for(new i = random_num(2, 10); i  > 0; i --){
	
		switch(random(5)){
			case 0:{
				RandomValue = random_num(0,1)
				createNuggetOrigin(fOrigin, 1,RandomValue, BLACK_NUGGET, .owner=id)
				if(RandomValue) iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "Czarna: %dx^n", RandomValue);
			}
			case 1:{
				RandomValue = random_num(1,2)
				createNuggetOrigin(fOrigin, 1,RandomValue, PINK_NUGGET, .owner=id)
				if(RandomValue) iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "Rozowa: %dx^n", RandomValue);
			}
			case 2:{
				RandomValue = random_num(2,3)
				createNuggetOrigin(fOrigin, 1,RandomValue, RED_NUGGET, .owner=id)
				if(RandomValue) iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "Czerwona: %dx^n", RandomValue);
			}
			case 3:{
				RandomValue = random_num(3,4)
				createNuggetOrigin(fOrigin, 1,RandomValue, YELLOW_NUGGET, .owner=id)
				if(RandomValue) iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "Zolta: %dx^n", RandomValue);
			}
			case 4:{
				RandomValue = random_num(4,5)
				createNuggetOrigin(fOrigin, 1,RandomValue, GREEN_NUGGET, .owner=id)
				if(RandomValue) iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "Zielona: %dx^n", RandomValue);
			}
			case 5:{
				RandomValue = random_num(6,7)
				createNuggetOrigin(fOrigin, 1,RandomValue, BLUE_NUGGET, .owner=id)
				if(RandomValue) iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "Niebieska: %dx^n", RandomValue);
			}
				
		}
	}
	
	new randomType[4];
	
	randomType[0] 	= 	random_num(500,1000)
	randomType[1] 	= 	random_num(100,500)
	randomType[2] 	= 	random(2)
	randomType[3]	=	randomType[2] == 1 ? randomType[0] : randomType[1]
	
	addNuggetToFinal(id,  randomType[3] )
	
	iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "Calosc: %d", randomType[3]);
	iLen += format(gText[iLen], sizeof(gText) - 1 - iLen, "^n--------------------");
	tutorBB(id, TUTOR_YELLOW, "%s", gText);
	
	ColorChat(id,GREEN, "%s Ekstra nagroda:^x03 %d Brylek", PREFIXSAY, randomType[3] )
	
	removeChestEnt(ent)
	
}
public bool:removeChestEnt(ent){
	if( !pev_valid(ent) )
		return false

	listCaseOnFloor[pev(ent, pev_iuser1)] = 0
	if( ent!= 0 ) remove_entity(ent)
	
	return true;
}

public freeChestCreate(){
	
	if(!freeChest) return;
	
	new Float:fOrigin[3];
	pev(freeChest, pev_origin, fOrigin);
	
	new ct = numPlayers(2, false);
	new tt = numPlayers(1, false);
	
	if( ct + tt > 4){
		
		if(random(100) < 10) createCases(fOrigin);
	}	
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
