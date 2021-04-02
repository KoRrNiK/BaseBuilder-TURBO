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
	
	if(userClassHuman[id] == human_FREE && didPro(id,  pro_LAPCZYWIEC) && get_user_team(id) == 2) value += random_num(5,10);
	
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
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
