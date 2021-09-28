#include <amxmodx>
#include <amxmisc>
#include <cstrike>
#include <fun>
#include <hamsandwich>
#include <fakemeta>
#include <fakemeta_util>
#include <engine>

/* EXP */

public addShotExp(id, Float:damage){
	
	if( !is_user_connected(id) || is_user_bot(id) || is_user_hltv(id)) return PLUGIN_CONTINUE;
		
	new newLeftExp  		=  	userScrollExp[id] - playedTime(id);
	
	if(newLeftExp > 0) damage *= 2.0;
	
	if(hourTime){
		if(bb_happy_hour() == happy_EXP || bb_happy_hour() == happy_ALL_EXP_NUGGET)
			damage *= 2.0;
	}
	
	if(userTeam[id]) damage *= 1.0 + (bonusExpTeam / 100.0);
	

	damage = floatabs(damage/8);

	
	if(clan[id]) damage += 5 * (get_clan_info(clan[id], CLAN_EXPDROP) / 2.25);
	
	damage += 10 * userReset[id];
	damage *= isSVip(id) ? 0.011 :isVip(id) ? 0.009 : 0.006;
	damage += 0.01;

	if(teamWorks(id)){
		switch(userTeamExp[id]){
			case 0: {
				addExpToFinal(id, damage);
			}
			case 1: {
				addExpToFinal(id, damage/1.75);
				addExpToFinal(userTeam[id], damage/1.75);
				
			}
			case 2: {
				addExpToFinal(id, damage/2.0);
				addExpToFinal(userTeam[id], damage/2.0);
			}
			case 3:{
				addExpToFinal(id, damage/2.25);
				addExpToFinal(userTeam[id], damage/2.25);
			}
			case 4: {
				addExpToFinal(userTeam[id], damage);
			}
		}
	} else {
		addExpToFinal(id, damage);
	}
	return PLUGIN_CONTINUE;
	
}
public addExpToFinal(id, Float:damage){
	userExpShow[id] += damage;
	
	if(get_gametime() - userLastExpDealt[id] < 2.0 ){
		if(task_exists( id + TASK_ADDEXP ))
			remove_task( id + TASK_ADDEXP);
	}
	userLastExpDealt[id] = get_gametime();
	set_task(1.0, "addFinalExp", id+ TASK_ADDEXP);
}

public addFinalExp(id){
	id -= TASK_ADDEXP;
	
	if (userExpShow[id] >= 500.0)
		chatPrint(0, PREFIX_LINE, "Gracz^3 %s^1 mial^4 %0.2f EXP'a^1 na liczniku!", userName[id], userExpShow[id]);
	
	addExp(id, userExpShow[id]);
	userExpShow[id] = 0.0;
}

public addExp(id, Float:value){
	
	switch(userGiveClassExp[id]){
		case 0: {
			addLevel(id, value);
		}
		case 1: {
			addLevel(id, value/1.75);
			addLevelClass(id, value/1.75);
		}
		case 2: {
			addLevel(id, value/2.0);
			addLevelClass(id, value/2.0);
		}
		case 3:{
			
			addLevel(id, value/2.25);
			addLevelClass(id, value/2.25);
		}
		case 4: {
			addLevelClass(id, value);
		}
	}
	
	return PLUGIN_CONTINUE;
}
/* BRYLKI */
public addKillNugget(id, value){
	
	if( !is_user_connected(id) || is_user_bot(id) || is_user_hltv(id)) return PLUGIN_CONTINUE;
	

	if((userScrollNugget[id] - playedTime(id)) > 0 ) value *= 2;
	
	if(userClassHuman[id] == human_FREE && didPro(id,  pro_LAPCZYWIEC) && get_user_team(id) == USER_HUMAN) value += random_num(5,10);
	
	if(isSVip(id)) value += random(6);
	else if(isVip(id)) value += random(2);
	
	if(clan[id]) if(get_clan_info(clan[id], CLAN_NUGGETDROP) >= 1 ) value += random_num(get_clan_info(clan[id], CLAN_NUGGETDROP) / 2 , get_clan_info(clan[id], CLAN_NUGGETDROP) + 3);	
	
	if(hourTime) if(bb_happy_hour() == happy_NUGGETS  || bb_happy_hour() == happy_ALL_EXP_NUGGET) value *= 2;
	
	if(userClassHuman[id] == human_FREE)  addPro(id, pro_LAPCZYWIEC, value);
	
	value += userReset[id] * 2;
	
	userNuggetAll[id] += value;
	
	if(teamWorks(id)){
		switch(userTeamNugget[id]){
			case 0: {
				addNuggetToFinal(id, value);
			}
			case 1: {
				addNuggetToFinal(id, floatround(value/1.75));
				addNuggetToFinal(userTeam[id], floatround(value/1.75));
			}
			case 2: {
				addNuggetToFinal(id, floatround(value/2.0));
				addNuggetToFinal(userTeam[id], floatround(value/2.0));
			}
			case 3:{
				addNuggetToFinal(id, floatround(value/2.25));
				addNuggetToFinal(userTeam[id], floatround(value/2.25));
			}
			case 4: {
				addNuggetToFinal(userTeam[id], value);
			}
		}
	} else {
		addNuggetToFinal(id, value);
	}
	
	return PLUGIN_CONTINUE;
}
public addNuggetToFinal(id, value){
	userNuggetShow[id] += value;
	
	if(get_gametime() - userLastNuggetDealt[id] < 2.0 ){
		if(task_exists( id + TASK_ADDNUGGET ))
			remove_task( id + TASK_ADDNUGGET);
	}
	userLastNuggetDealt[id] = get_gametime();
	set_task(1.0, "addFinalNugget", id+ TASK_ADDNUGGET);
}
public addFinalNugget(id){
	id -= TASK_ADDNUGGET;
	
	if (userNuggetShow[id] >= 750)
		chatPrint(0, PREFIX_LINE, "Gracz^3 %s^1 mial^4 %d Brylek^1 na liczniku!", userName[id], userNuggetShow[id]);
	
	
	addNugget(id, userNuggetShow[id]);
	userNuggetShow[id] = 0;
}
public addNugget(id, value){
	userNugget[id] += value;
	userNuggetCollectedRound[id] += value;
	return PLUGIN_CONTINUE;
}
/* BONES */
public addKillBone(id, value){
	
	if( !is_user_connected(id) || is_user_bot(id) || is_user_hltv(id)) return PLUGIN_CONTINUE;
	
	if(hourTime){
		if(bb_happy_hour() == happy_BONES || bb_happy_hour() == happy_ALL_EXP_NUGGET)
			value *= 2.0;
	}
	
	addBoneToFinal(id, value);
	
	return PLUGIN_CONTINUE;
}
public addBoneToFinal(id, value){
	userBoneShow[id] += value;
	
	if(get_gametime() - userLastBoneDealt[id] < 2.0 ){
		if(task_exists( id + TASK_ADDBONE ))
			remove_task( id + TASK_ADDBONE);
	}
	userLastBoneDealt[id] = get_gametime();
	set_task(1.0, "addFinalBone", id+ TASK_ADDBONE);
}
public addFinalBone(id){
	id -= TASK_ADDBONE;

	if (userBoneShow[id] >= 50)
		chatPrint(0, PREFIX_LINE, "Gracz^3 %s^1 mial^4 %d Kosci^1 na liczniku!", userName[id], userBoneShow[id]);
	
	
	addBone(id, userBoneShow[id]);
	userBoneShow[id] = 0;
}
public addBone(id, value){
	userBone[id] += value;
	return PLUGIN_CONTINUE;
}

public Float:needXp(id, lvl) return float(lvl * 45 + 45 * userReset[id]);

public addLevel(id, Float:Exp){		
	userExp[id]+=Exp;
	while(userExp[id]>=needXp(id, userLevel[id]) ){			
		userExp[id] -= needXp(id, userLevel[id]);
		if( userLevel[id] < MAXLVL ){
			userLevel[id] ++;
			chatPrint(0, PREFIX_LINE, "Gracz^3 %s^1 wbil^3 %d^1 lv.^3 [^4 Postac^3 ]", userName[id] , userLevel[id]);

		} else {
			chatPrint(id, PREFIX_LINE, "Posiadasz^4 maksymalny Lv.^1 otrzymales w nagrode^3 + 1 secret Point");
			userSecretPoint[id] ++;	
		}
	}
}
public Float:needXpClass(lvl){
	return float(lvl * 45);
}
public addLevelClass(id, Float:Exp){
		
	new class = userClassHuman[id];
	
	userExpClass[id][class]+=Exp;
	while(userExpClass[id][class] >= needXpClass(userHumanLevel[id][class])){			
		userExpClass[id][class] -= needXpClass(userHumanLevel[id][class]);
		
		if( userHumanLevel[id][class] < MAXLVLCLASS ){
			userHumanLevel[id][class] ++;
			chatPrint(0, PREFIX_LINE, "Gracz^3 %s^1 wbil^3 %d^1 lv.^3 [^4 %s^3 ]", userName[id] , userHumanLevel[id][class], classesHuman[class][0]);
		}else {
			chatPrint(id, PREFIX_LINE, "Posiadasz^4 maksymalny Lv.^1 na klasie^3 %s^1 otrzymales w nagrode^3 + 1 secret Point^4 ", classesHuman[class][0]);
			userSecretPoint[id] ++;
			
		}
	}
}

public createNugget(id, id2){
	static Float:fOrigin[3];
	pev(id2, pev_origin, fOrigin);
	
	
	for(new i = 0; i < random(5); i ++){
	
		switch(random(100)){
			case 0: 		createNuggetOrigin(fOrigin, 1,1, BLACK_NUGGET, 	.owner=id);		
			case 1..5: 	createNuggetOrigin(fOrigin, 1,1, PINK_NUGGET, 	.owner=id);		
			case 6..16: 	createNuggetOrigin(fOrigin, 1,1, BLUE_NUGGET, 	.owner=id);		
			case 17..32:	createNuggetOrigin(fOrigin, 1,1, YELLOW_NUGGET, 	.owner=id);		
			case 33..53:	createNuggetOrigin(fOrigin, 1,1, GREEN_NUGGET, 	.owner=id);		
			default:	createNuggetOrigin(fOrigin, 1,1, RED_NUGGET, 	.owner=id);
		}
	}
}

public nuggetThink(ent){
	if( !pev_valid(ent) ) return;
		
	engfunc( EngFunc_SetSize, ent , Float:{-16.0, -16.0, -16.0}, Float:{16.0,16.0,16.0});
	static Float:fAngles[3];
	pev(ent, pev_angles, fAngles);
	fAngles[1] += pev(ent, pev_fuser2);
	
	set_pev(ent, pev_angles, fAngles);
	if( get_gametime() - pev(ent, pev_fuser3) > delayEffectNugget[pev(ent, pev_body)] ){
		new Float:fOrigin[3];
		pev(ent, pev_origin, fOrigin);
		fOrigin[2] += 20.0;
		
		
		if( get_gametime() - pev(ent, pev_fuser1) > 3.0 ){
			if( pev(ent, pev_flags) & FL_ONGROUND ){
				static Float:fVelocity[3];
				pev(ent, pev_velocity, fVelocity);
				fVelocity[2] = random_float(45.0, 65.0);			
				set_pev(ent, pev_velocity, fVelocity);
			}
		}
		set_pev(ent, pev_fuser3, get_gametime());
	}
	
	if( get_gametime() - pev(ent, pev_fuser1) > 10.0 ){
		
		listNuggetOnFloor[pev(ent, pev_iuser1)] = 0;
		if( ent!= 0 ) remove_entity(ent);
		
	}else set_pev(ent, pev_nextthink, get_gametime()+0.1);	
}



createNuggetOrigin(Float:fOrigin[3], minN, maxN, type, owner = 0){

	static Float:fVelocity[3];
			
	new ent = -1;
	for( new i = 0 ; i < random_num(minN,maxN); i ++ ){		
		ent = addNuggetEnt();
		if( ent == 0) return;
	
		set_pev(ent, pev_classname, "nugget");
		
		set_pev(ent, pev_solid, SOLID_TRIGGER);
		set_pev(ent, pev_movetype, MOVETYPE_TOSS);
		set_pev(ent, pev_fuser1, get_gametime() );
		set_pev(ent, pev_fuser2, random_float(1.0, 50.0));
		set_pev(ent, pev_fuser3, get_gametime());
		
		set_pev(ent, pev_iuser3, owner);
		set_pev(ent, pev_skin, type);

		set_pev(ent, pev_origin, fOrigin);
		engfunc( EngFunc_SetSize, ent , Float:{-16.0, -16.0, -16.0}, Float:{16.0,16.0,16.0});
		
		fVelocity[0] = random_float(-150.0, 150.0);
		fVelocity[1] = random_float(-150.0, 150.0);
		fVelocity[2] = random_float(100.0, 300.0);
		set_pev(ent, pev_velocity, fVelocity);


		set_rendering(ent, kRenderFxGlowShell, 	colorNugget[type][0], colorNugget[type][1] ,     colorNugget[type][2], 	kRenderNormal, 5);

		entity_set_model(ent, modelNuggetDrop);
		set_pev(ent, pev_nextthink, get_gametime()+0.1);
		
		if(is_user_connected(owner)){
			message_begin(MSG_ONE, SVC_TEMPENTITY, _ , owner);
			write_byte(22) ;
			write_short(ent) ;
			write_short(thunder) ;
			write_byte(5) ;
			write_byte(8) ;
			write_byte(colorNugget[type][0]) ;
			write_byte(colorNugget[type][1]) ;
			write_byte(colorNugget[type][2]) ;
			write_byte(75);
			message_end();
		}
	}
}


public findCoinArround(id){
	
	static Float:fOrigin[3];
	static Float:fOriginCoin[3];
	new pickUp = false;
	
	pev(id, pev_origin, fOrigin);
	new ent = 0;
	new value = 0;
	
	for( new i = 0 ; i < MAXNUGGETSFLOOR; i ++ ){
		ent = listNuggetOnFloor[i];
		if( !pev_valid(ent) ) continue;	
		if( get_gametime() - pev(ent, pev_fuser1) < random_float(1.50,2.50) ) continue;
		
	
		if(teamWorks(id)){
			id = userTeam[id];
			
			if(pev(ent, pev_iuser3) != id) continue;
		}
		
		if(pev(ent, pev_iuser3) != id)  continue;
			
		pev(ent, pev_origin, fOriginCoin);
		
		if(!pickUp){
			if( get_distance_f(fOrigin, fOriginCoin) > 155.0){
					
				if( get_gametime() - pev(ent, pev_fuser1) < random_float(1.50,2.50) ) continue;
				if( get_distance_f(fOrigin, fOriginCoin)  < 300.0 ){
					new Float:fVeloc[3];
					xs_vec_sub(fOrigin, fOriginCoin, fVeloc)	;			
					xs_vec_normalize( fVeloc , fVeloc );				
					xs_vec_mul_scalar( fVeloc , 300.0 , fVeloc );
					set_pev(ent, pev_velocity, fVeloc);
					
					
				} continue;
			}
		}
		
		switch(pev(ent, pev_skin)){
			case RED_NUGGET:  {
				value = random_num(1,3);
			}
			
			case GREEN_NUGGET: {
				value = 4;
			}
			case YELLOW_NUGGET: {
				value = 5;
				addSecretMission(id, mission_secret_GOLDNUGGET, value);
				
				#if defined CHRISTMAS_ADDON
		
					addChristmasMission(id, CH_PICKUPGOLD, 1);
				
				#endif	
			}
			case BLUE_NUGGET:{
				if(userNugget[id] == 0 ) addSecretMission(id, mission_secret_BLUE, 1);
				value = 6;
			}
			case PINK_NUGGET:  {
				value = 9;
			}
			case BLACK_NUGGET:  {
				value = random_num(12,28);
				addSecretMission(id, mission_secret_LUCK, 1);
				#if defined CHRISTMAS_ADDON
		
					addChristmasMission(id, CH_BLACKNUGGET, 1);
				
				#endif
			}
		}
		
		
		if(foundedHat(id, HAT_AUREOLA)) {
			new Float:valRound = 0.0;
			valRound = float(value);
			valRound *= 1.5;
			value = floatround(valRound);
			
		}
	
		
		addKillNugget(id, value);
		
		#if defined CHRISTMAS_ADDON
		
			addChristmasMission(id, CH_PICKUP, 1);
		
		#endif
		
		spkGameSound(id, sound_PICKUP);
		listNuggetOnFloor[pev(ent, pev_iuser1)] = 0;
		remove_entity(ent);
	}
}

public addNuggetEnt(){
	new slotCoin = freeNgugetSlot();
	if( slotCoin!=-1){
		new ent = create_entity("info_target");
		if( !pev_valid(ent) ) return 0;
		listNuggetOnFloor[slotCoin] = ent;
		set_pev(ent, pev_iuser1, slotCoin);
		return ent;
	}
	return 0;
}
public freeNgugetSlot(){
	for( new i = 0 ; i < MAXNUGGETSFLOOR; i ++ ){
		if( listNuggetOnFloor[i] == 0 )
			return i;
	}
	return -1;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
