#include <amxmodx>
#include <amxmisc>
#include <cstrike>
#include <fun>
#include <hamsandwich>
#include <fakemeta>
#include <fakemeta_util>
#include <engine>

stock canBeMoved(id, ent){
	if( ent == gBarrier )
		return 0
	if( !pev_valid(ent)  )
		return 0
		
	
		
	new szClass[14], szTarget[7];
	entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass));
	entity_get_string(ent, EV_SZ_targetname, szTarget, sizeof(szTarget));
	if( isAdmin(id) ){
		if( isPlayer(ent) || ( equal(szClass, "func_wall") && containi(szTarget, "ignore")==-1 &&  !equal(szTarget, "barrier") && !equal(szTarget, "JUMP")))
			return ent
	}else 
		if( equal(szClass, "func_wall") && containi(szTarget, "ignore")==-1 &&  !equal(szTarget, "barrier") && !equal(szTarget, "JUMP") )
			return ent
	return 0;		
}
stock bool:canIMoveIt(id,ent){
	if( getOwner(ent) == 0 || getOwner(ent) == id || userClone[id] || ( isAdmin(id) && userAllowBuild[id] ) || userTeam[id] == getOwner(ent))
		return true;
	return false;

	
}
public buildingMain(id){	
	//openMenuNPC(id, ent, body);	
	buildingInfo(id)
	buildingBuild(id)
}
public buildingInfo(id){	
	new gText[256], iLen
	
	if(!playerLogged(id)){
		return PLUGIN_CONTINUE;
	}
	new ent;
	get_user_aiming(id, ent)

	new Float:cordy = 0.15;
	new Float:cordx = -1.0
	

	//openMenuTree(id, ent, cordx, cordy)

	#if defined CHRISTMAS_ADDON
		openMenuTree(id, ent, cordx, cordy)
	#endif
	
	caseInfo(id, ent, cordx, cordy)

	if( ent != 0 ){
		if( get_gametime()-userAimingHud[id] >= 0.2){
			
			if( canBeMoved(id, ent) && !isPlayer(ent)){
				
				if(userMoveAs[id] != id && isAdmin(id)) iLen += format(gText[iLen], sizeof(gText)-1-iLen,  "[ Przenosisz klocki jako: %s ]^n",userName[userMoveAs[id]] );
				
				if( getOwner(ent) == 0 ){
					iLen += format(gText[iLen], sizeof(gText)-1-iLen,  "Kliknij 'F' aby skopiowac^n");
					iLen += format(gText[iLen], sizeof(gText)-1-iLen, "-- ! --",pev(ent, pev_team) )
					
				}else{
					if( getMover(ent) == 0 ){
						iLen += format(gText[iLen], sizeof(gText)-1-iLen, "[ %s ]", userName[getOwner(ent)])
						
						iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n[ %s ]", userName[getLastMover(ent)])
					}else{
						if( !isPlayer(ent) && getRotateBlock(ent, 0)){							
							iLen += format(gText[iLen], sizeof(gText)-1-iLen, "Kliknij 'Q' aby obrocic^n")
						}
						iLen += format(gText[iLen], sizeof(gText)-1-iLen, "[ %s ]", userName[getMover(ent)])
					}
				}
				if( userClone[id] ){
					iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n[ Kopiujesz ]", userName[getMover(ent)])
				}	
				new speed[32]; format(speed, sizeof(speed), "%d razy wolniej", floatround(userPushBlock[id]))
				if(userPushBlock[id]) iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n[ %s ]", userPushBlock[id] > 0 ? speed : "");
				
				if( userBlocksUsed[id] > 0 && buildTime){
					iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n[ %d/%d ]", userBlocksUsed[id], isVip(id) ? maxBlockUseVip : maxBlockUse)
				}
				
						
				
				set_hudmessage(userHud[id][PLAYER_HUD_RED], userHud[id][PLAYER_HUD_GREEN], userHud[id][PLAYER_HUD_BLUE], cordx, cordy, 0, 0.1, 0.1, 0.1, 0.1)
				show_hudmessage(id, "%s", gText)
				
				
			}
			if(isPlayer(ent)){
				if( is_user_connected( ent) && is_user_alive( ent)){
			
					if(get_user_team( ent) == 2) set_hudmessage(25, 125, 255, cordx, cordy, 0, 0.1, 0.1, 0.1, 0.1)	
					else set_hudmessage(255, 125, 25, cordx, cordy, 0, 0.1, 0.1, 0.1, 0.1)
						
					new zombie[33], human[33];
					format(zombie, sizeof(zombie), "[ Klasa: %s ]", classesZombies[userClass[ent]][0]);
					format(human, sizeof(human), "[ Klasa: %s ]", classesHuman[userClassHuman[ent]][0]);
						
					iLen += format(gText[iLen], sizeof(gText)-iLen-1, "[ %s: %s | Zycie: %d ]^n%s", isVip(ent) ? "Vip" : "Gracz", userName[ent], get_user_health(ent), get_user_team( ent) == 1 ? zombie : human);
					if(has_flag(id, "a")) iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n~ [AFK: %0.2f%%] ~^n", userAfkValue[ ent ]);
						 
					
					if(userDeathPrice[ent][PRICE_START] && !userDeathPrice[ent][PRICE_LOST]){
						iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n[ Placi: %d Brylek | Godowanie: %s ]^n", userDeathPrice[ent][PRICE_DEATH], userDeathPrice[ent][PRICE_GOD] ? "Tak" : "Nie");
						if(userDeathPrice[ent][PRICE_CLASS] == class_TOTAL)
							iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "[ Wszystkie Zombie | ");
						else iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "[ Zombie: %s | ", classesZombies[userDeathPrice[ent][PRICE_CLASS]][0]);
						if(userDeathPrice[ent][PRICE_TIME] == 0)
							iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "Czas: Cala Runda ]^n");
						else iLen += format(gText[iLen], sizeof(gText) - iLen - 1, "Czas: %d:%s%d ]^n",userDeathPrice[ent][PRICE_TIME]/60, (userDeathPrice[ent][PRICE_TIME]%60 < 10 ? "0" : ""), userDeathPrice[ent][PRICE_TIME]%60)
					}
					show_hudmessage(id, "%s", gText)
				}
			}
			
			
			
			userAimingHud[id] = get_gametime();
		}
	}
	return PLUGIN_CONTINUE;
}
public hookEnt(id){
	new ent, body;
	get_user_aiming(id, ent, body)
	
	if(!buildTime){
		if(!userAllowBuild[id])
		return PLUGIN_CONTINUE;
	}
	
	if (id != userMoveAs[id] && userMoveAs[id] && userAllowBuild[id]){
		ColorChat(id, GREEN, "%s Przenosisz klocki jako^x03 %s", PREFIXSAY, userName[userMoveAs[id]]);
	}
	
	
	if( canBeMoved(id, ent) && canIMoveIt(id,ent)){
		
		if( !isPlayer(ent) ){
			
			/*if( Pokolorowany[ ent ] )
			{
				set_pev(ent,pev_rendermode,kRenderNormal);
				Pokolorowany[ ent ] = 0;
				remove_entity( KolorEnt[ ent ] );
				hookEnt(id)
				return PLUGIN_CONTINUE;

			}
			*/
			if( getOwner(ent) == 0 || userClone[id] ){
				if( userBlocksUsed[id] >= (isVip(id) ? maxBlockUseVip : maxBlockUse) ){
					set_dhudmessage(255, 0, 0, -1.0, 0.40, 0, 6.0, 3.0)
					show_dhudmessage(id, "--- Uzyto maxymalna ilosc blokow ---")					
					return PLUGIN_CONTINUE;
				}
				if(!isSuperAdmin(id) && buildTime)
					userBlocksUsed[id] ++ 

			}
			
			if( userClone[id] || getLock(ent) == 2 ||  extraClone ){
				new cloneEnt = createClone(ent)
				if( pev_valid(cloneEnt) ){	
					new mover;
					if (userMoveAs[id])  mover = userMoveAs[id];
					else mover = id;
					setOwner(cloneEnt, getOwner(mover))
					addMission(id, mission_CLONE, 1);
					ent = cloneEnt					
				}
				
				
				userClone[id]=false;
			}
			
			
			
			
		
			if( getOwner(ent) == 0 ){
				new mover;
				if (userMoveAs[id])  mover = userMoveAs[id];
				else mover = id;
				setOwner(ent, mover)
			}
			
			setMover(ent, id)
		
		}
		userGrab[id] = ent
		grabEnt(id, ent, body)	
		emitGameSound(id, sound_MOVESTART)
		
	}
	
	return PLUGIN_CONTINUE;
}

public selectAimingMoveAs(id){
	if (isAdmin(id)){
		new ent, body;
		
		get_user_aiming(id, ent, body, 9999);
		if (!isPlayer(ent)){
			new szClass[10], szTarget[7];
			entity_get_string(ent, EV_SZ_classname, szClass, 9);
			entity_get_string(ent, EV_SZ_targetname, szTarget, 6);
			if (equal(szClass, "func_wall") && !equal(szTarget, "ignore"))
				if (getOwner(ent))
					ent = getOwner(ent);
				else ent = id;
			else ent = id;
		}
		if (userMoveAs[id] != ent){
			userMoveAs[id] = ent;
			ColorChat(id, GREEN, "%s Przenosisz klocki jako^x03 %s", PREFIXSAY, userName[userMoveAs[id]]);
		}
		return PLUGIN_CONTINUE;
	}
	return PLUGIN_CONTINUE;
}

public grabEnt(id, ent, body){
	
	new Float:fOrigin[3], iAiming[3], Float:fAiming[3]
	
	
	get_user_origin(id, iAiming, 3);
	IVecFVec(iAiming, fAiming);
	
	entity_get_vector(ent, EV_VEC_origin, fOrigin);
	entity_set_vector(ent, EV_VEC_vuser4, fOrigin)
	
	
	
	fOffset[id][0] = fOrigin[0] - fAiming[0];
	fOffset[id][1] = fOrigin[1] - fAiming[1];
	fOffset[id][2] = fOrigin[2] - fAiming[2];
	
	new ent2, body2;
	userLength[id] = get_user_aiming(id, ent2, body2);
	
	if( userPushBlock[id] > 0 )
		entity_get_vector(ent, EV_VEC_origin, fOriginSave[id])
	
	if( !isPlayer(ent) ){
		set_pev(ent, pev_movetype, 	MOVETYPE_FLY)
		set_pev(ent, pev_solid, 	SOLID_SLIDEBOX)	
		addMission(id, mission_BUILD, 1);
		colorBlock(id)
	}else{
		userHudMoving[ent] = id
		set_user_noclip(ent, 1)
	}		
}
public colorBlock(id){
	new ent = userGrab[id]
	if(userMoverBlockColor[id] == BLOCK_COLOR){
		
		new Float:fColor[3]
		fColor[0] = random_float(0.0, 255.0);
		fColor[1] = random_float(0.0, 255.0);
		fColor[2] = random_float(0.0, 255.0);
			
		//set_rendering(ent, kRenderFxPulseFastWide, floatround(colorAmount[color][0]), floatround(colorAmount[color][1]),floatround(colorAmount[color][2]), kRenderTransAdd, floatround(colorAmount[color][3]))	
			
		set_pev(ent,pev_renderfx, kRenderFxPulseFast)	
		set_pev(ent,pev_rendermode,kRenderTransColor)
		set_pev(ent,pev_rendercolor, fColor )	
		set_pev(ent,pev_vuser2, fColor)
		set_pev(ent,pev_renderamt, 200.0 )
		
	} else if(userMoverBlockColor[id] == BLOCK_RENDER){
		
		set_pev(ent,pev_renderfx, kRenderFxNone)
		set_pev(ent,pev_rendermode,kRenderTransTexture)
		set_pev(ent,pev_renderamt, 200.0 )
		
	} else if(userMoverBlockColor[id] == BLOCK_NORENDER){
		set_pev(ent,pev_renderfx, kRenderFxNone)
		set_pev(ent,pev_rendermode,kRenderNormal)	
	
	}
}
public moveEnt(id){

	
	
	new iOrigin[3], iLook[3], Float:fOrigin[3], Float:fLook[3], Float:vMoveTo[3], Float:fLength
	    
	get_user_origin(id, iOrigin, 1);
	IVecFVec(iOrigin, fOrigin);
	get_user_origin(id, iLook, 3);
	IVecFVec(iLook, fLook);
		
	fLength = get_distance_f(fLook, fOrigin);
	if (fLength == 0.0) fLength = 1.0;

	vMoveTo[0] = (fOrigin[0] + (fLook[0] - fOrigin[0]) * userLength[id] / fLength) + fOffset[id][0];
	vMoveTo[1] = (fOrigin[1] + (fLook[1] - fOrigin[1]) * userLength[id] / fLength) + fOffset[id][1];
	vMoveTo[2] = (fOrigin[2] + (fLook[2] - fOrigin[2]) * userLength[id] / fLength) + fOffset[id][2];
	vMoveTo[2] = float(floatround(vMoveTo[2], floatround_floor));

	
	if( userPushBlock[id] > 0 ){
		new Float:vecMoved[3];
		vecMoved[0]	=	fOriginSave[id][0]+((vMoveTo[0]-fOriginSave[id][0])/userPushBlock[id])
		vecMoved[1]	=	fOriginSave[id][1]+((vMoveTo[1]-fOriginSave[id][1])/userPushBlock[id])
		vecMoved[2]	=	fOriginSave[id][2]+((vMoveTo[2]-fOriginSave[id][2])/userPushBlock[id])
		
		entity_set_origin(userGrab[id], vecMoved)
	}else entity_set_origin(userGrab[id], vMoveTo);
	
	
}
public stopEnt(id){
	if( userGrab[id] != 0 ){
		
		new ent=userGrab[id];		
		userGrab[id] = 0
					
		
		if( pev_valid(ent) && !isPlayer(ent)){
			
			set_pev(ent,pev_rendermode,kRenderNormal)		
				
			delMover(ent)
			setLastMover(ent, id)
		}
		
			
		if( !isPlayer(ent) ){	
			set_task(0.1, "prev", ent)		
		}else{
			
			userHudMoving[ent] = 0
			set_user_noclip(ent, 0)
		}
		emitGameSound(id, sound_MOVESTOP)
	}
}
public prev(ent){
	if( !pev_valid(ent) )
		return;
	if( ent == 0 )
		return;
	if( getMover(ent) == 0 ){	
		
		set_pev(ent, pev_movetype, MOVETYPE_PUSH)
		set_pev(ent, pev_solid, SOLID_BSP)		
	}
}

public buildingBuild(id){	
	
	if( !userAllowBuild[id] ){
		if( ( get_user_team(id) != 2 || !buildTime ) )
			return PLUGIN_CONTINUE
	}
	if( pev(id, pev_button) & IN_USE ) {	
		
		if( userGrab[id] == 0 ){		
			if( !userCanGrab[id] )
				hookEnt(id)
		}else{
			
			if( userClone[id] && !isPlayer(userGrab[id])){
				
				if( userBlocksUsed[id] >= (isVip(id) ? maxBlockUseVip : maxBlockUse)){
					set_dhudmessage(255, 0, 0, -1.0, 0.40, 0, 6.0, 3.0)
					show_dhudmessage(id, "--- Uzyto maxymalna ilosc blokow ---")	
				}else {
					
					new cloneEnt = createClone(userGrab[id])
				
					if( pev_valid(cloneEnt) ){				
						new ent;
						get_user_aiming(id, ent)
					
						new mover;
						if (userMoveAs[id])  mover = userMoveAs[id];
						else mover = id;
						stopEnt(id)
						ent = cloneEnt					
						userGrab[id] = ent
						
						if(!isSuperAdmin(id) && buildTime)
							userBlocksUsed[id] ++ 
							
						setOwner(ent, mover)
						setMover(ent, mover)	
						colorBlock(id)
						addMission(id, mission_CLONE, 1);
					}
					
					
					
				}
				userClone[id]=false;
			}
			moveEnt(id)
			
		}
		
		userCanGrab[id] = true
	}else{
		userCanGrab[id] = false
		if (userGrab[id] != 0 ){
			stopEnt(id)
		}
	}
	if( (pev(id, pev_button)& IN_RELOAD) ){
		if( userGrab[id] != 0 )
			userLength[id] = 150.0;
	}
	if( (pev(id, pev_button)& IN_ATTACK) ){
		if( userGrab[id] != 0 ){
			client_print(id, print_center, "--Oddalanie--")
			userLength[id] += userLength[id] > 150 ? (userLength[id] > 700? 10.0:7.0) : 1.5;
		}
	}
	if( (pev(id, pev_button) & IN_ATTACK2) ){
		if( userGrab[id] != 0 ){
			client_print(id, print_center, "--Przyblizanie--")
			userLength[id] -= userLength[id] > 150 ? (userLength[id] > 700? 10.0:7.0) : 1.5;		
		}
	}
	return PLUGIN_CONTINUE

}
public createClone(entView){
	new ent=create_entity("func_wall")
	if( !pev_valid(ent) ){
		return -1;
	}
	new szClassName[16]
	pev(entView, pev_classname, szClassName, sizeof(szClassName))
	set_pev(ent,pev_classname, szClassName)
	
	pev(entView, pev_model, szClassName, sizeof(szClassName))
	set_pev(ent,pev_model, szClassName)
	
	
	set_pev(ent,pev_solid, pev(entView, pev_solid))
	set_pev(ent,pev_movetype, pev(entView, pev_movetype))
	set_pev(ent,pev_modelindex, pev(entView, pev_modelindex))
	set_pev(ent,pev_body, pev(entView, pev_body))
	set_pev(ent,pev_skin, pev(entView, pev_skin))
	set_pev(ent,pev_flags, pev(entView, pev_flags))
	set_pev(ent,pev_spawnflags, pev(entView, pev_spawnflags))
	set_pev(ent,pev_team, pev(entView, pev_team))
	
	new Float:fFloat[3]
	pev(entView, pev_mins, fFloat)
	set_pev(ent, pev_mins, fFloat)
	
	pev(entView, pev_maxs, fFloat)
	set_pev(ent, pev_maxs, fFloat)	
	
	pev(entView, pev_vuser3, fFloat)
	set_pev(ent, pev_vuser3, fFloat)	
	
	pev(entView, pev_vuser1, fFloat)
	set_pev(ent, pev_vuser1, fFloat)	
	
	setOwner(ent, getOwner(entView))
	pev(entView, pev_origin, fFloat)
	entity_set_origin(ent, fFloat)
	setLock(ent, 3)

	return ent;	
}

public impulsePush(id){
	userPushBlock[id] += 2.0;
	if(userPushBlock[id] > 6.0)
		userPushBlock[id]  = 0.0;
	
	if(userPushBlock[id]  == 0.0)
		ColorChat(id, GREEN, "---^x01 Przesuwasz klocki normalnie ^x04 ---");
	else ColorChat(id, GREEN, "---^x01 Przesuwasz klocki wolniej o^x03 %d razy^x04 ---", floatround(userPushBlock[id]));
	
	return PLUGIN_HANDLED;
}


public impulseClone(id){
	
	
	//if (1 <= userGrab[id] && userGrab[id] < maxPlayers) return PLUGIN_CONTINUE;
	if(isPlayer(userGrab[id])) return PLUGIN_CONTINUE;
	
	if (!isAdmin(id) && userCloned[id] >= (isVip(id) ? maxBlockUseVip : maxBlockUse)){
		ColorChat(id, GREEN, "%s Osiagnales juz limit blokow!", PREFIXSAY);
		return PLUGIN_HANDLED;
	}
	userClone[id] = !userClone[id];
	return PLUGIN_HANDLED;
}

stock unSetBlock(ent){
	delOwner(ent)
	delLastMover(ent)
	delMover(ent)
}

stock setOwner(ent,id)		set_pev(ent, pev_iuser1, id)
stock getOwner(ent)		return pev(ent, pev_iuser1) 		
stock delOwner(ent)		set_pev(ent, pev_iuser1, 0)

stock setLastMover(ent,id)	set_pev(ent, pev_iuser2, id)
stock getLastMover(ent)		return pev(ent, pev_iuser2)
stock delLastMover(ent)		set_pev(ent, pev_iuser2, 0) 

stock setMover(ent,id)		set_pev(ent, pev_iuser3, id)
stock getMover(ent)		return pev(ent, pev_iuser3) 
stock delMover(ent)		set_pev(ent, pev_iuser3, 0)

stock setLock(ent, type)		set_pev(ent, pev_iuser4, type);
stock getLock(ent)		return pev(ent, pev_iuser4)
stock delLock(ent)		set_pev(ent, pev_iuser4, 0)

stock getRotateBlock(ent, type)	return pev(ent, pev_team) != type



public checkRemove(ent){
	if( !pev_valid(ent) )
		return PLUGIN_CONTINUE
	if(  getMover(ent) == 0  ){		
		set_pev(ent,pev_rendermode,kRenderNormal)	
	}else{		
		new Float:fColors[3]		
		pev(ent,pev_vuser2,fColors)
		set_pev(ent,pev_rendermode,kRenderTransColor)
		set_pev(ent,pev_rendercolor, fColors )
		set_pev(ent,pev_renderamt, 200.0 )
		
	}
	return PLUGIN_CONTINUE
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
