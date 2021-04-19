#include <amxmodx>
#include <amxmisc>
#include <fun>
#include <cstrike>
#include <fakemeta>
#include <fakemeta_util>
#include <engine>
#include <hamsandwich>

#define GRZYBKITHINK 19582
#define TASK_SHROOMAFK 19258
#define shroomTASK 917529

new const Float:offsetGrzybek = 70.0;
new const grzybki[9][]=
{
	{-1,-1,0}, {0,-1,0}, {1,-1,0},
	{-1,0,0}, {0,0,0}, {1,0,0},
	{-1,1,0}, {0,1,0}, {1,1,0}
};	

	
new totalPointsMooshrom;	
new shroomPlayer;
new actualRecord;

new userOpenedEggs[33];	
new userShroomPoints[33];	
	
new grzybkiEnts[9];
new counterEnts[3];
new totalMax;
new que[33];
new shroomZone;
new Float:shroomBtn[33];

public menuGrzybki(id){

	if(!is_user_connected(id)) return;

	new menu=menu_create("Menu Grzybki", "menuGrzybki_2");
	
	menu_additem(menu,"Stworz grzybka");
	menu_additem(menu,"Digital Counter");
	menu_additem(menu,"Odswiez");
	
	
	menu_display(id, menu, 0);
}
public menuGrzybki_2(id,menu,item){	
	if( item == MENU_EXIT){
		menu_destroy(menu);
		return PLUGIN_CONTINUE;
	}
	
	switch(item){
		case 0: prepareShroom();
		case 1: prepareDigitalCounter();
		case 2:{
			totalPointsMooshrom=999;
			refreshCounter();
		}
	}
	menuGrzybki(id);
	return PLUGIN_CONTINUE;
}
public prepareShroom(){
	
	
	
	new shroomPlace = find_ent_by_tname(-1, "moshrooms");
	if( !pev_valid(shroomPlace) ) return PLUGIN_CONTINUE;
	
	new Float:fOrigin[3];	
	new ent;
	for( new i = 0; i < 9;i ++){		
		entity_get_vector(shroomPlace, EV_VEC_origin, fOrigin);
		
		fOrigin[2] += float(grzybki[i][2]);
		fOrigin[0] += (offsetGrzybek*grzybki[i][0]);
		fOrigin[1] += (offsetGrzybek*grzybki[i][1]);
		
		ent = create_entity("info_target");		
		entity_set_string(ent, EV_SZ_classname, "grzybek");
		entity_set_model(ent, "models/basebuildervt/grzybeku.mdl");	
		entity_set_int(ent, EV_INT_solid, SOLID_BBOX);
		entity_set_size(ent, Float:{-16.0,-16.0,-4.0}, Float:{16.0,16.0,18.0});
		entity_set_int(ent, EV_INT_skin, random(3));
		entity_set_float(ent, EV_FL_framerate, 1.0);
		
		entity_set_float(ent, EV_FL_takedamage, DAMAGE_YES);
		entity_set_float(ent, EV_FL_health, 2047483647.0);
		entity_set_int(ent,EV_INT_gamestate,1);
		entity_set_vector(ent, EV_VEC_angles, Float:{0.0, 180.0, 0.0});
		entity_set_origin(ent, fOrigin);	
		grzybkiEnts[i]=ent;
	}
	new hour;
	time(hour);
	if(hour < 1 || hour > 6) set_task(2.0,"grzybkiPlay", GRZYBKITHINK);
	else set_task(2.0,"grzybkiSleep", GRZYBKITHINK);
	return PLUGIN_CONTINUE;
}
stock bool:isSchroom(ent){
	new szClass[14];
	entity_get_string(ent, EV_SZ_classname, szClass, sizeof(szClass) );
	if(equal(szClass, "grzybek") )
		return true;		
	return false;
}
public prepareDigitalCounter(){
	new counterPlace = find_ent_by_tname(-1, "digitalCounter");
	
	if( !pev_valid(counterPlace) ) return PLUGIN_CONTINUE;	
	
	new Float:fOrigin[3];	
	entity_get_vector(counterPlace, EV_VEC_origin, fOrigin);
	fOrigin[1] -= 40.0;
	new ent;
	for( new i = 0; i < 3;i ++){	
		ent = create_entity("info_target");
		entity_set_string(ent, EV_SZ_classname, "digtalCounter");
		
		entity_set_vector(ent, EV_VEC_angles, Float:{0.0, 180.0, 0.0});
		entity_set_model(ent, "sprites/basebuildervt/digitalCounter.spr");
		entity_set_int(ent,EV_INT_rendermode, 5);
		entity_set_float(ent, EV_FL_renderamt, 255.0);
		entity_set_float(ent, EV_FL_scale, 0.7);
		entity_set_origin(ent, fOrigin);
		fOrigin[1]+=40.0;
		counterEnts[i]=ent;
	}
	refreshCounter();
	return PLUGIN_CONTINUE;
}
public refreshCounter(){	
	
	new frame = (totalPointsMooshrom/100)%10;
	entity_set_float(counterEnts[0], EV_FL_frame, float(frame));	
	frame = (totalPointsMooshrom/10)%10;
	entity_set_float(counterEnts[1], EV_FL_frame, float(frame));
	frame = totalPointsMooshrom%10;
	entity_set_float(counterEnts[2], EV_FL_frame, float(frame));
	
}
public grzybekThink(ent){
	if(!pev_valid(ent)) return PLUGIN_CONTINUE;
	
	new Float:fOrigin[3];
	pev(ent, pev_origin, fOrigin);
	fOrigin[2] += 10.0;
	switch(entity_get_int(ent, EV_INT_sequence) ){
		case 1:{
			set_pev(ent, pev_animtime, get_gametime());
			entity_set_int(ent, EV_INT_sequence, 2);
			entity_set_float(ent, EV_FL_nextthink, get_gametime()+0.7);	
			if( entity_get_int(ent, EV_INT_skin) == 3 ){
				fm_set_rendering(ent, kRenderFxGlowShell, 255, 255 , 255, kRenderNormal, 1);
			}
		}
		case 2:{
			
			set_pev(ent, pev_animtime, get_gametime());
			entity_set_int(ent, EV_INT_solid, SOLID_NOT);
			entity_set_int(ent, EV_INT_sequence, 3);
			entity_set_float(ent, EV_FL_nextthink, get_gametime()+0.1);	
		}
		case 3:{
		
			set_pev(ent, pev_animtime, get_gametime());
			if( entity_get_int(ent, EV_INT_skin) == 3 ){				
				fm_set_rendering(ent, kRenderFxNone, 255, 255 , 255, kRenderNormal,255);
			}
			entity_set_int(ent, EV_INT_sequence, 0);
		}
	}
	return PLUGIN_CONTINUE;
}
public grzybkiPlay(){
	new ent = grzybkiEnts[random(9)];
	while(entity_get_int(ent, EV_INT_sequence)!=0){
		ent = grzybkiEnts[random(9)];
	}
	new Float:fOrigin[3];
	entity_get_vector(ent, EV_VEC_origin, fOrigin);
	
	entity_set_int(ent, EV_INT_sequence, 1);
	if( userOpenedEggs[shroomPlayer]>0 && random(5) != 0 ){
		if( random(20) == 0 ){
			entity_set_int(ent, EV_INT_skin, 3);
		}else entity_set_int(ent, EV_INT_skin, random(2));
	}else{
		if( random(40) == 0 )
			entity_set_int(ent, EV_INT_skin, random(4));
		else entity_set_int(ent, EV_INT_skin, random(3));
	}
	entity_set_int(ent, EV_INT_solid, SOLID_BBOX);
	entity_set_size(ent, Float:{-16.0,-16.0,-4.0}, Float:{16.0,16.0,28.0});
	entity_set_float(ent, EV_FL_nextthink, get_gametime()+0.05);
	

	set_task(0.5,"grzybkiPlay", GRZYBKITHINK);
}
public grzybkiSleep(){
	
	new ent = grzybkiEnts[random(9)];
	while(entity_get_int(ent, EV_INT_sequence)!=0){
		ent = grzybkiEnts[random(9)];
	}
	new Float:fOrigin[3];
	entity_get_vector(ent, EV_VEC_origin, fOrigin);
	
	entity_set_float(ent, EV_FL_nextthink, get_gametime()+0.1);
	
	fOrigin[2] += 50.0;
	Particles(fOrigin, 3.0, sprite_sleepshroom, random_num(1,2), 2, random_num(2,3), random_num(4,10), 1);
	
	set_task(1.5,"grzybkiSleep", GRZYBKITHINK);
}
public grzybekDMG(id, ent, Float:damage){
	if( shroomPlayer != id ) return PLUGIN_CONTINUE;
	
	if( entity_get_int(ent, EV_INT_sequence) == 1 || entity_get_int(ent, EV_INT_sequence) == 2  ){
			
		if( entity_get_int(ent, EV_INT_skin) == 2 ){	
			 endShroomGame();
			 return PLUGIN_CONTINUE;			
		}
		
		if( task_exists(shroomPlayer+TASK_SHROOMAFK) )
			remove_task(shroomPlayer+TASK_SHROOMAFK);
			
		set_task(5.0, "endShroomGameAfk", shroomPlayer+TASK_SHROOMAFK);
		if( entity_get_int(ent, EV_INT_skin) == 3 ){
			totalPointsMooshrom += 10;			
			set_dhudmessage(255, 255, 85,  -1.0, 0.6, 0, 6.0, 0.4, 0.1, 0.1);
			show_dhudmessage(id, "+10");

			#if defined CHRISTMAS_ADDON
		
				addChristmasMission(id,CH_GOLDSCHROOM, 1);
							
			#endif
			
			
		}else if( entity_get_int(ent, EV_INT_skin) == 1 ){ 
			new ran = random_num(1,3);
			set_dhudmessage(153, 255, 102, -1.0, 0.6, 0, 6.0, 0.4, 0.1, 0.1);
			show_dhudmessage(id, "+%d", ran);
			totalPointsMooshrom += ran;	
			#if defined CHRISTMAS_ADDON
		
				addChristmasMission(id,CH_GREENSCHROOM, 1);
							
			#endif

		} else {
			set_dhudmessage(102, 255, 255, -1.0, 0.6, 0, 6.0, 0.4, 0.1, 0.1);
			show_dhudmessage(id, "+1");	
			totalPointsMooshrom ++;	
		}
		
		if(  totalMax < totalPointsMooshrom ){
			totalMax = totalPointsMooshrom;
		}
		if( totalMax > userShroomPoints[id] )
			userShroomPoints[id] = totalMax;
		if(  actualRecord < totalMax ){
			actualRecord=totalMax;
		}	
		entity_set_int(ent, EV_INT_sequence, 2);	
		entity_set_float(ent, EV_FL_nextthink, get_gametime());
		
		fm_set_rendering(ent, kRenderFxGlowShell, 255, 255 , 255, kRenderNormal, 10);
		set_task(0.1,"turnOffGlowShroom", ent);
		refreshCounter();
		fm_cs_get_weapon_ammo( get_pdata_cbase(id, 373) ,  7 );
		if(task_exists(GRZYBKITHINK) )
			remove_task(GRZYBKITHINK);
		set_task(0.2,"grzybkiPlay", GRZYBKITHINK);
		
	}
	return PLUGIN_CONTINUE;
}
public turnOffGlowShroom(ent){
	fm_set_rendering(ent, kRenderFxNone, 255, 255 , 255, kRenderNormal,255);
}
public peopleInQue(){
	new iNum = 0;
	for( new i = 0 ; i < sizeof(que); i ++ ){
		if( que[i] == 0 ){
			iNum++;
		}			
	}
	return iNum;
}
public isInQue(id){
	for( new i = 0 ; i < sizeof(que); i ++ ){
		if( que[i] == id ){
			que[i] = 0;
			return i;
		}			
	}
	return -1;
}
public removeFromQue(id){
	new placeInQue = isInQue(id);
	if( placeInQue != -1 ){
		for( new i = placeInQue ; i < sizeof(que)-1; i ++ ){
			que[i] = que[i+1];				
		}
		que[sizeof(que)-1] = 0;
	}
}
public addToQue(id){
	removeFromQue(id);
	for( new i = 0; i <sizeof(que); i ++ ){
		if( que[i] == 0 ){
			que[i] = id;
			return i;
		}
	}
	return -1;
}
public clearQue(){
	for( new i = 0; i <sizeof(que); i ++ ){
		que[i]=0;
	}
}
public startShroomGame(id){	
	if(get_gametime()-shroomBtn[id] < 2.0 ){
		shroomBtn[id] = get_gametime();		
		chatPrint(id, PREFIX_LINE, "Nie spamuj !");
		return PLUGIN_CONTINUE;
	}
	shroomZone = find_ent_by_tname(-1, "shroomZone");
	if( !pev_valid(shroomZone) ){
		chatPrint(id, PREFIX_LINE, "Nie mozna grac w grzybki :(");
		return PLUGIN_CONTINUE;		
	}
	if( shroomPlayer == id ){
		
		endGame();
		return PLUGIN_CONTINUE;
	}
	new placeInQue = addToQue(id);
	if(placeInQue != -1 ){
		if( placeInQue == 0 ){		
				
			new hour;
			time(hour);
			if(hour < 1|| hour > 6) {	
				remove_task(GRZYBKITHINK);
				grzybkiPlay();
			} else grzybkiSleep();
			
			startWithPlayer();
		}else{
			chatPrint(id, PREFIX_LINE, "Twoje miejsce w kolejce to:^4 %d ", placeInQue);
		}
	}
	return PLUGIN_CONTINUE;
}
public startWithPlayer(){
	shroomPlayer = que[0];
	if( shroomPlayer == 0 ) return PLUGIN_CONTINUE;
	
	shroomZone = find_ent_by_tname(-1, "shroomZone");
	if( !pev_valid(shroomZone) ) return PLUGIN_CONTINUE;	
	
	new Float:fOrigin[3];
	entity_get_vector(shroomZone, EV_VEC_origin, fOrigin);
	entity_set_vector(shroomPlayer, EV_VEC_origin, fOrigin);
	give_item(shroomPlayer, "weapon_deagle")	;
	set_task(5.0, "endShroomGameAfk", shroomPlayer+TASK_SHROOMAFK);
	
	totalPointsMooshrom = 0;
	totalMax = 0;
	refreshCounter();
	set_task(30.0, "endShroomGameTime", shroomTASK);
	return PLUGIN_CONTINUE;
}
public endShroomGameAfk(id){
	id -= shroomPlayer+TASK_SHROOMAFK;
	
	if( task_exists(shroomTASK) ) remove_task(shroomTASK);
		
	chatPrint(0, PREFIX_LINE, "AFK !^4 %s^1 ! Na liczniku^4 %d^1, rekord^4 %d ", userName[shroomPlayer],totalPointsMooshrom, userShroomPoints[shroomPlayer]);
	endGame();
}
public endShroomGame(){
	if( task_exists(shroomTASK)) remove_task(shroomTASK);
	chatPrint(0, PREFIX_LINE, "Skucha !^4 %s^1 ! Na liczniku^4 %d^1, rekord^4 %d ", userName[shroomPlayer],totalPointsMooshrom, userShroomPoints[shroomPlayer]);
	endGame();
}
public endShroomGameTime(){
	chatPrint(0, PREFIX_LINE, "Koniec Czasu^4 %s^1 ! Na liczniku^4 %d^1, rekord^4 %d ", userName[shroomPlayer],totalPointsMooshrom, userShroomPoints[shroomPlayer]);
	endGame();
}
public endShroomGameLeft(){
	if( task_exists(shroomPlayer+TASK_SHROOMAFK)) remove_task(shroomPlayer+TASK_SHROOMAFK);
		
	chatPrint(0, PREFIX_LINE, "Gracz^4 %s^1 ! Wyszedl ", userName[shroomPlayer]);
	endGame();
}
public endGame(){
	if( task_exists(shroomPlayer+TASK_SHROOMAFK) ) remove_task(shroomPlayer+TASK_SHROOMAFK);
		
	respawnPlayerAdmin(shroomPlayer);
	
	new award = totalPointsMooshrom/6;
	
	if( award > 0 ){
		new Float:fOrigin[3];
		entity_get_vector(shroomPlayer, EV_VEC_origin, fOrigin);
		
		switch(random(100)){
			case 0: 	{
				createNuggetOrigin(fOrigin, award,award, BLACK_NUGGET, 	.owner=shroomPlayer);
				chatPrint(0, PREFIX_LINE, "%s otrzymal^4 %d^1 Czarnych brylek ! Za ustrzelenie^4 %d^1 grzybkow", userName[shroomPlayer],award,totalPointsMooshrom);
			}
			case 1..5:{
				createNuggetOrigin(fOrigin, award,award, PINK_NUGGET, 	.owner=shroomPlayer);	
				chatPrint(0, PREFIX_LINE, "%s otrzymal^4 %d^1 Rozowych brylek ! Za ustrzelenie^4 %d^1 grzybkow", userName[shroomPlayer],award,totalPointsMooshrom);
				
			}
			case 6..16: {
				createNuggetOrigin(fOrigin, award,award, BLUE_NUGGET, 	.owner=shroomPlayer);
				chatPrint(0, PREFIX_LINE, "%s otrzymal^4 %d^1 Niebieskich brylek ! Za ustrzelenie^4 %d^1 grzybkow", userName[shroomPlayer],award,totalPointsMooshrom);
			}
			case 17..32:{
				createNuggetOrigin(fOrigin, award,award, YELLOW_NUGGET, 	.owner=shroomPlayer);
				chatPrint(0, PREFIX_LINE, "%s otrzymal^4 %d^1 Zoltych brylek ! Za ustrzelenie^4 %d^1 grzybkow", userName[shroomPlayer],award,totalPointsMooshrom);
			}
			case 33..53:{
				createNuggetOrigin(fOrigin, award,award, GREEN_NUGGET, 	.owner=shroomPlayer);
				chatPrint(0, PREFIX_LINE, "%s otrzymal^4 %d^1 Zielonych brylek ! Za ustrzelenie^4 %d^1 grzybkow", userName[shroomPlayer],award,totalPointsMooshrom);
			}
			default:{
				createNuggetOrigin(fOrigin, award,award, RED_NUGGET, 	.owner=shroomPlayer);
				chatPrint(0, PREFIX_LINE, "%s otrzymal^4 %d^1 Czerwonych brylek ! Za ustrzelenie^4 %d^1 grzybkow", userName[shroomPlayer],award,totalPointsMooshrom);
			}
		}		
	}
	totalPointsMooshrom = actualRecord;
	
	#if defined CHRISTMAS_ADDON
		if(totalPointsMooshrom >= 50) addChristmasMission(shroomPlayer,CH_POINTSCHROOM, 1);
							
	#endif
			
	removeFromQue(shroomPlayer);
	shroomPlayer=0;
	refreshCounter();
	if( task_exists( shroomTASK))
			remove_task( shroomTASK);
	if( ( prepTime ||  buildTime ) ){		
		startWithPlayer();
	}
}

/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
