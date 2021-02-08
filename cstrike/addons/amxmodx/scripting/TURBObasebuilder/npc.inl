new const npcClass[]	=	"npcVillage";
new const npcModel[]	=	"models/basebuildervt/tree.mdl";
new bool:userNpc[33];
new solidNPC;
new configFile[128];

public npcPrecache(){
	precache_model(npcModel)
	
	loadNpc()
	
	register_clcmd("npccreate", "menuNPCCreate");
	
}

public menuNPCCreate(id){
	if(!has_flag(id, "a")) return;
	new menu = menu_create("\r[BaseBuilder]\y Tworzenie NPC!", "menuNPCCreate_2");
	//new gText[128]
	
	menu_additem(menu, "Sworz NPC'a")
	menu_additem(menu, "Usun")
	menu_additem(menu, "Zapisz")
	menu_display(id, menu, 0);
	
}

public menuNPCCreate_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return PLUGIN_CONTINUE;
	}
	
	new ent = -1;
	
	switch(item){
		
		case 0:{
			/*solidNPC = true
			while((ent = find_ent_by_class(ent, npcClass)) > 0){
				menuNPCCreate(id)
				ColorChat(id, GREEN, "---^x01 Limit NPC'tow^x04 ---");
				return PLUGIN_CONTINUE;
				
			}*/
			ColorChat(id, GREEN, "---^x01 Stworzyles NPC'ta^x04 ---");	
			menuNPCCreate(id)
			new Float:fOrigin[3];
			new Float:fAngles[3];
			
			entity_get_vector(id,	EV_VEC_origin, 	fOrigin );
			entity_get_vector(id, 	EV_VEC_angles, 	fAngles );
	
			createLudzik(fOrigin, fAngles)
		}
		case 1:{
			
			
			while ((ent = find_ent_by_class(ent, npcClass)) > 0) {
				if (ent > 0){
					remove_entity(ent);
					menuNPCCreate(id)
					ColorChat(id, GREEN, "---^x01 Usunales NPC'ta^x04 ---");
				}
			}
				
				
			
		}
		
		case 2:{
			saveNpc( )
			ColorChat(id, GREEN, "---^x01 Zapisales^x04 ---");
		}
	}

	return PLUGIN_CONTINUE;
}


public openMenuNPC(id, ent, body){
	
	
	
	if( !pev_valid(ent) || !isEntNpc(ent) || userNpc[id])
		return PLUGIN_CONTINUE;		
	
	
	new button = pev(id, pev_button)
	
	new Float:fOrigin[3], Float:fOriginEnt[3]
	entity_get_vector(id,	EV_VEC_origin, 	fOrigin)
	entity_get_vector(ent,	EV_VEC_origin, 	fOriginEnt)
	
	new Float:distanceOpen = 75.5
	
	static Float:fTime[ 33 ]
	new Float:fTimeNow = get_gametime();

	if( get_distance_f( fOrigin, fOriginEnt ) <= distanceOpen){		
			
		if( ( fTimeNow - fTime[ id ] ) >= 0.15 ){
	
			set_dhudmessage(42, 255, 212, -1.0, 0.60, 0, 0.09, 0.1, 0.09, 0.09)
			show_dhudmessage(id, "^nAby rozmawiac kliknij: E^n^n")
			fTime[ id ] = fTimeNow 
			
		}
		
	}

	
	if( ( button & IN_USE ) && get_distance_f( fOrigin, fOriginEnt ) <= distanceOpen){
		
		spkGameSound(id, sound_HELLO)
		//miningMenu(id)
		menuNpc(id)
		userNpc[id] = true;
		return PLUGIN_HANDLED;
	}
		
	return PLUGIN_CONTINUE;	
}
public menuNpc(id){
	
	
	new menu = menu_create("", "menuNpc_2");
	
	menu_additem(menu, "Test");
	
	
	menu_display(id,menu,0)
}

public menuNpc_2(id, menu, item){
	
	if(item == MENU_EXIT){
		menu_destroy(menu)
		userNpc[id] = false;
		return PLUGIN_CONTINUE;
	}
	
	new ent = find_ent_by_class(ent, npcClass)

	new Float:fOrigin[3], Float:fOriginEnt[3]
	entity_get_vector(id,	EV_VEC_origin, 	fOrigin)
	entity_get_vector(ent,	EV_VEC_origin, 	fOriginEnt)
	
	if( get_distance_f( fOrigin, fOriginEnt ) >= 75.5){		
		ColorChat(id, GREEN, "---^x01 Wroc do podzroznika aby kontynuowac rozmowe^x04 ---");
		userNpc[id] = false;
		return PLUGIN_CONTINUE;
	}
	
	switch(item){
		case 0:{
			ColorChat(id, GREEN, "---^x01 Sklepik!^x04 ---");
			userNpc[id] = false;
		}
	}
	
	return PLUGIN_CONTINUE;
}
public createLudzik(Float:fOrigin[3], Float:fAngles[3]){
	new ent = create_entity("info_target");
	
	if( !pev_valid(ent) || ent == 0 ) return PLUGIN_CONTINUE;
	
	entity_set_string(ent, 	EV_SZ_classname, npcClass);
	entity_set_model(ent, 	npcModel);
	

	if(solidNPC){
		new Float:upModel = 33.0;
		fOrigin[2] += distanceToFloor(fOrigin)  - 	upModel;
	}
	entity_set_vector(ent, 	EV_VEC_angles, 	fAngles );
	entity_set_vector(ent, 	EV_VEC_origin, 	fOrigin );
	entity_set_float(ent, 	EV_FL_framerate, random_float(1.0, 3.0))
	//entity_set_size(ent, 	Float:{0.0,0.0, 0.0},Float:{20.0,60.0,20.0} )
	
	if(solidNPC)	entity_set_int(ent, 	EV_INT_solid, 	SOLID_NOT );
	else 		entity_set_int(ent, 	EV_INT_solid, 	SOLID_BBOX );
	
	entity_set_int(ent, 	EV_INT_movetype, MOVETYPE_FLY )	
	entity_set_int(ent, 	EV_INT_sequence, 1)
	
	set_pev(ent, pev_nextthink, get_gametime()+0.1)
	
	drop_to_floor(ent);
	
	//fm_set_rendering(ent, kRenderFxGlowShell, 255,255,255, kRenderNormal, 1);
	
	return PLUGIN_CONTINUE;
}
public npcThink (ent){
	if( !pev_valid(ent) )
		return
			
	new Float:fOrigin[3]
	pev(ent, pev_origin, fOrigin)
	
	
	makeLight(fOrigin,25, 240, 133, 56,15, 30)	
	fOrigin[2] -= 38.5;
	BeamCylinder(fOrigin, 50.0, spriteBeam, 10, 0, 10, 1, 0, 240, 133, 56, 255, 0)
	
	
	makeLight(fOrigin,15,255,255,0,5, 10)
	set_pev(ent, pev_nextthink, get_gametime()+ 1.5)	
}

stock bool:isEntNpc(ent){
	new szClass[12]
	entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass) )
	if( equal(szClass, npcClass) )
		return true;		
	return false;
}



saveNpc( ) {
	if(file_exists(configFile )) delete_file(configFile);
	
	new file = fopen(configFile, "wt");
	
	new szData[128];
	new Float:fOrigin[ 3 ]
	new Float:fAngles[3]
	new ent;
	
	if(file){
		while( ( ent = find_ent_by_class( ent, npcClass ) ) > 0 ) {
			
			entity_get_vector( ent, EV_VEC_origin, fOrigin );
			entity_get_vector(ent, EV_VEC_angles, 	fAngles );
			
			format(szData, sizeof(szData), "%f %f %f %f %f %f^n", fOrigin[ 0 ], fOrigin[ 1 ], fOrigin[ 2 ], fAngles[0], fAngles[1], fAngles[2] );
			fputs(file, szData)
		}
		fclose(file);
	}
}
loadNpc(){
	new map[ 33 ];
	get_mapname(map,sizeof(map));
	
	format(configFile, sizeof(configFile), "addons/amxmodx/configs/npc" );
	
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
	
	new Float:fOrigin[3]
	new Float:fAngles[3]
	new szOrigin[3][17]
	new szAngles[3][17]
	
	while(!feof(file)){
		fgets(file, szData, sizeof( szData ) );
		
		parse(szData,
			szOrigin[0], 	sizeof(szOrigin[]),
			szOrigin[1], 	sizeof(szOrigin[]),
			szOrigin[2], 	sizeof(szOrigin[]),
			szAngles[0], 	sizeof(szAngles[]),
			szAngles[1], 	sizeof(szAngles[]),
			szAngles[2], 	sizeof(szAngles[])
		);
		
		for( new i = 0;i <3; i ++ ){
			fOrigin[i]=str_to_float(szOrigin[i])
			fAngles[i]=str_to_float(szAngles[i])
		}
	
		createLudzik( fOrigin, fAngles );
	}
	
	fclose(file);
	return PLUGIN_CONTINUE;
}
/*
public createRandomMetin(chance)
{
	new num;
	new Float:fOrigin[3] = 0.0;
	new szClass[10];
	new ent = 28;
	while (ent < maxEnts)
	{
		if (pev_valid(ent))
		{
			entity_get_string(ent, "bl", szClass, 10);
			if (equal(szClass, "metinSpot", "bl"))
			{
				if (chance > random(100))
				{
					entity_get_vector(ent, "bl", fOrigin);
					new var1 = fOrigin[2];
					var1 = floatadd(1108344832, var1);
					createMetin(fOrigin);
					num++;
				}
			}
		}
		ent++;
	}
	return num;
}*/
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
