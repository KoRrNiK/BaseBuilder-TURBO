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
#include <	bbTurbo 	>

new const CLASS_COUNTERFALSE[] 	= 	"counterFalse";
new const CLASS_COUNTERTRUE[] 	= 	"counterTrue";
new const SPRITE_COUNTERFALSE[] 	= 	"sprites/basebuildervt/falseCounter.spr";
new const SPRITE_COUNTERTRUE[] 	= 	"sprites/basebuildervt/trueCounter.spr";

enum _:statusOX{	
	OX_START,
	OX_QUESTION[64],
	OX_TIME,
	OX_ANSWER,
	OX_TASK,
	bool:OX_MICRO,
	bool:OX_INVIS,
	bool:OX_ROUNDSTART,
};
new OX[statusOX];

new falseCounter;
new trueCounter;
new Array:entOXPlace;

new userOX[33];
new Float:OriginRound[3];

public menuOX(id){
	
	if(!is_user_connected(id)) return;
	
	if(!isSuperAdmin(id)) return;
	
	new menu = menu_create("\r[BaseBuilder]\y OX", "menuOX_2");
	
	menu_additem(menu, "Rozpocznij gre\r OX'a");
	if(OX[OX_START]){
		menu_additem(menu, "Przenies wszystkich na\y OX'a");
		menu_additem(menu, "Usun aktualne pytanie");
		menu_additem(menu, "\rWyrzuc wszystkich z OX'a");
		menu_additem(menu, formatm("Niewidzialnosc:\w [ %s%s\w ]", OX[OX_INVIS] ? "\r" : "\d", symbolsCustom[SYMBOL_DOT] ));
	}
	menu_display(id, menu, 0);
}
public menuOX_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return;
	}
	
	switch(item){
		case 0:{
			userOX[id] = -2;

			if(!ArraySize(entOXPlace)){
				chatPrint(id, PREFIX_NORMAL, "Na tej mapie nie mozna zagrac w OX");
			}else{
				OX[OX_START] = true;
	
				new ent = find_ent_by_class(-1, "info_bomb_target");
				if( pev_valid(ent) ) remove_entity(ent);		
			
				chatPrint(0, PREFIX_LINE, "Rozpoczynamy zabawe OX !!!");
				server_cmd("sv_restartround 1");

				createSprites();
				new Float:fColor[3];
				fColor[0] = 255.0;
				fColor[1] = 255.0;
				fColor[2] = 255.0;
				changeRender(kRenderFxNone, fColor, 100.0, 0);
				
				OX[OX_TIME] = 0;
				OX[OX_ROUNDSTART] = false;
				OX[OX_MICRO] = false;
				OX[OX_INVIS] = false;
				OX[OX_QUESTION] = " ";
				OX[OX_ANSWER] = 0;
				
			}
		}
		case 1:{
			if(OX[OX_START]){
				transferPlayersToPlaceOX(id);
			} else chatPrint(id, PREFIX_NORMAL, "Pierw rozpocznij konkurs^4 OX");
		}
		case 2:{
			if(OX[OX_TIME] && OX[OX_START]){
				remove_task(OX_TASK);
				new  Float:fColor[3];
				fColor[0] = 255.0;
				fColor[1] = 255.0;
				fColor[2] = 255.0;
				changeRender(kRenderFxNone, fColor, 100.0, 2);
				OX[OX_TIME] = 0;
				OX[OX_ROUNDSTART] = false;
				chatPrint(id, PREFIX_NORMAL, "Admin^3 %s^1 anulowal aktualne pytanie na OX'ie!", userName[id]);
				for( new i = 1  ; i < maxPlayers; i ++ ){
					if( !is_user_connected(i) || is_user_hltv(i)) continue;
					tutorMake(i, TUTOR_BLUE, 2.0, "Prawidlowa odpowiedz: Anulowana");
				}
			} else chatPrint(id, PREFIX_NORMAL, "Aktualnie nie ma odpalonego pytania!");
		}
		case 3:{
			for( new i = 1  ; i < maxPlayers; i ++ ){
				if( !is_user_connected(i) || is_user_hltv(i)) continue;
				if(userOX[i] == -2) continue;
				
				playerDropFromBox(i);
			}
		}
		case 4:{
			if(OX[OX_START]){
				if(OX[OX_INVIS]) OX[OX_INVIS] = false;
				else OX[OX_INVIS] = true;
				chatPrint(id, PREFIX_NORMAL, "%s niewidzialnosc osobom na OX'ie", OX[OX_INVIS] ? "Wlaczyles" : "Wylaczyles");
			} else chatPrint(id, PREFIX_NORMAL, "Pierw rozpocznij konkurs^4 OX");
		}
	}
	menuOX(id);
}
public changeRender(render, Float:fColor[3], Float:amt, origin){
	new findRender = find_ent_by_tname(-1, "round");
	if( !pev_valid(findRender) ) return;
	
	new Float:fOrigin[3];
	
	pev(findRender, pev_origin, fOrigin);
	
	if(origin == 0){
		fOrigin[0] = OriginRound[0];
		fOrigin[1] = OriginRound[1];
		fOrigin[2] = OriginRound[2];
		set_pev(findRender, pev_origin, fOrigin);
	} else if(origin == 1 ){
		fOrigin[2] += 15.0;
		set_pev(findRender, pev_origin, fOrigin);
	} else if(origin == 2 ) set_pev(findRender, pev_origin, OriginRound);
	
	set_pev(findRender,pev_rendermode,kRenderTransColor);
	set_pev(findRender,pev_rendercolor, fColor );	
	set_pev(findRender,pev_vuser2, fColor);
	set_pev(findRender,pev_renderamt, 100.0 );
	set_pev(findRender,pev_renderfx, render);
}
public createSprites(){
	createCounter(0);
	createCounter(1);
}
public createCounter(type){
		
	new Float:fAngles[3];
	new Float:fOrigin[3];
	new Float:ANGLE = 90.0;	
	new Float:SCALE = 0.35;	
	new Float:R_AMT = 255.0;
	new R_MODE = 5;
	
	if(type == 0 ){
		
		new findPlace = find_ent_by_tname(-1, "oxFalse");
		if( !pev_valid(findPlace) ) return;
		
		falseCounter = create_entity("info_target");
		if( !pev_valid(falseCounter) ) return;
		
		entity_set_model(falseCounter,			SPRITE_COUNTERFALSE);	
		set_pev(falseCounter, pev_classname, 		CLASS_COUNTERFALSE);
		set_pev(falseCounter, pev_solid, 		SOLID_NOT);
		set_pev(falseCounter, pev_movetype, 		MOVETYPE_FLY);	
		
		fAngles[0] = 0.0; fAngles[1] = ANGLE; fAngles[2] = 0.0;
		
		entity_set_vector(falseCounter, 	EV_VEC_angles, 		fAngles);	
		entity_set_float(falseCounter, 	EV_FL_scale, 		SCALE);							
		entity_set_int(falseCounter, 	EV_INT_rendermode, 	R_MODE);
		entity_set_float(falseCounter, 	EV_FL_renderamt, 	R_AMT);		
		entity_get_vector(findPlace, 	EV_VEC_origin, 		fOrigin);			
		entity_set_vector(falseCounter, 	EV_VEC_origin,		fOrigin);
	
	}
	if(type == 1 ){
		
		new findPlace = find_ent_by_tname(-1, "oxTrue");
		if( !pev_valid(findPlace) ) return;
		
		trueCounter = create_entity("info_target");
		if( !pev_valid(trueCounter) ) return;
			
		entity_set_model(trueCounter,			SPRITE_COUNTERTRUE);	
		set_pev(trueCounter, pev_classname, 		CLASS_COUNTERTRUE);
		set_pev(trueCounter, pev_solid, 			SOLID_NOT);
		set_pev(trueCounter, pev_movetype, 		MOVETYPE_FLY);
		
		fAngles[0] = 0.0; fAngles[1] = ANGLE*3; fAngles[2] = 0.0;
		
		entity_set_vector(trueCounter, 	EV_VEC_angles, 		fAngles);
		entity_set_float(trueCounter, 	EV_FL_scale, 		SCALE);							
		entity_set_int(trueCounter, 	EV_INT_rendermode, 	R_MODE);
		entity_set_float(trueCounter, 	EV_FL_renderamt, 	R_AMT);		
		entity_get_vector(findPlace, 	EV_VEC_origin, 		fOrigin);
		entity_set_vector(trueCounter,  	EV_VEC_origin,		fOrigin);
	}
}

public askQuestion(id){	
	if(!isSuperAdmin(id)) return PLUGIN_CONTINUE;
	if(!OX[OX_START]) return PLUGIN_CONTINUE;
	
	if(OX[OX_ROUNDSTART] || OX[OX_TIME] ){
		client_print(id, print_console, "Aktualnie trwa pytanie!");
		return PLUGIN_HANDLED;
	}
	
	client_print(id, print_console, "______________________________________________________^n");
	client_print(id, print_console, "Jak uzyc komendy?");
	client_print(id, print_console, "Schemat: bb_ox ^"Pytanie^" odpowiedz czas");
	client_print(id, print_console, "Przyklad: bb_ox ^"Tak/Nie^" 2 15");
	client_print(id, print_console, "Odpowiedz: 0 - falsz | 1 - prawda | 2> - losowe");
	client_print(id, print_console, "Jesli nie wpiszesz czasu defaultowa liczba jest: 15");
	client_print(id, print_console, "______________________________________________________^n");
	
	new szArg[128];
	read_argv(1, szArg, sizeof(szArg));
	remove_quotes(szArg);
	
	if(equal(szArg, "")){
		client_print(id, print_console, "Zle uzycie komendy!");
		return PLUGIN_HANDLED;
	}
	
	copy(OX[OX_QUESTION], sizeof(OX[OX_QUESTION])-1, szArg);
	
	new szAnswer[4];
	read_argv(2, szAnswer, sizeof(szAnswer));

	OX[OX_ANSWER] = str_to_num(szAnswer);
	if(OX[OX_ANSWER] > 1) OX[OX_ANSWER] = random(2);
	
	new szTime[4];
	read_argv(3, szTime, sizeof(szTime));		
	OX[OX_TIME] = str_to_num(szTime);
	
	if( OX[OX_TIME] <= 0 ) OX[OX_TIME] = 15;
	
	OX[OX_MICRO] = true;
	OX[OX_ROUNDSTART] = true;
	noMenu();
	taskCountOx();
	new  Float:fColor[3];
	fColor[0] = 255.0;
	fColor[1] = 255.0;
	fColor[2] = 255.0;
	changeRender(kRenderFxPulseFast, fColor, 150.0, 2);
	client_print(id, print_console, "Pytanie: %s", OX[OX_QUESTION]);
	client_print(id, print_console, "Czas: %d", OX[OX_TIME]);
	client_print(id, print_console, "Odpowiedz: %s",OX[OX_ANSWER] == 0 ? "Falsz" : OX[OX_ANSWER] == 1 ? "Prawda" : "Losowa" );
	client_print(id, print_console, "______________________________________________________^n");
	return PLUGIN_HANDLED;
}

public taskCountOx(){	
	if( OX[OX_TIME] > 0 ){
		refreshSpritesCounter();
		set_dhudmessage(OX[OX_TIME] % 2 == 0 ? 180 : 0, 255, 0, -1.0, 0.30, 0, 5.0, 0.85);
		replace_all(OX[OX_QUESTION], sizeof(OX[OX_QUESTION]), "\n", "^n");
		
		if( OX[OX_TIME] >=5){
			show_dhudmessage(0, "%s^n%s Czas: %d Sek %s",OX[OX_QUESTION], symbolsCustom[SYMBOL_R_ARROW], OX[OX_TIME], symbolsCustom[SYMBOL_L_ARROW]);
		}else show_dhudmessage(0, "%s^n%s Czas: ? Sek %s",OX[OX_QUESTION], symbolsCustom[SYMBOL_R_ARROW], symbolsCustom[SYMBOL_L_ARROW] );
		
		OX[OX_TIME] --;
		set_task(0.9, "taskCountOx", OX_TASK);
	} else {
		for( new i = 1 ; i < maxPlayers;i ++ ){
			if( is_user_hltv(i) ) continue;
			if( !is_user_connected(i) ) continue;
			if( !get_user_team(i) ) continue;
			
			tutorMake(i, OX[OX_ANSWER] == 0 ? TUTOR_RED : TUTOR_GREEN, 2.0, "Prawidlowa odpowiedz: %s", OX[OX_ANSWER] == 0 ? "Falsz" : "Prawda");
			
			if( userOX[i] == -2 ) continue;
			
			if( userOX[i]== OX[OX_ANSWER] || ( OX[OX_ANSWER] <0 && userOX[i] != abs(OX[OX_ANSWER]) && userOX[i] != -1) ){				
				set_dhudmessage(127, 255, 85, -1.0, 0.20, 0, 6.0, 1.0) ;
				show_dhudmessage(i, "Gratulacje przechodzisz dalej!");
				client_cmd(i, "spk basebuildervt/correct.wav");
			}else{			
				playerDropFromBox(i);
				client_cmd(i, "spk basebuilder/fail.wav");
			}
		}
		new  Float:fColor[3];
		if(OX[OX_ANSWER] == 0){
			fColor[0] = 255.0;
			fColor[1] = 0.0;
			fColor[2] = 0.0;
		} else {
			fColor[0] = 0.0;
			fColor[1] = 255.0;
			fColor[2] = 0.0;
		}
		changeRender(kRenderFxNone, fColor, 100.0, 1);
		OX[OX_MICRO] = false;
		OX[OX_ROUNDSTART] = false;
	}
}
public refreshSpritesCounter(){
	if( OX[OX_TIME] >= 5 ) changeSpritesCounter( OX[OX_TIME] - 4 );
	else changeSpritesCounter( 0 );
}
public changeSpritesCounter(frame){	
	
	if( OX[OX_TIME] % 2 == 0 ) emit_sound(falseCounter, CHAN_WEAPON, "basebuilder/thock.wav", 1.0, ATTN_NORM, 0, PITCH_NORM);
	else emit_sound( falseCounter, CHAN_WEAPON, "basebuilder/thick.wav", 1.0, ATTN_NORM, 0, PITCH_NORM);

	entity_set_float(falseCounter, EV_FL_frame, float(frame));
	entity_set_float(trueCounter, EV_FL_frame, float(frame));
}
public prepareSpawns(){
	entOXPlace = ArrayCreate(1, maxPlayers);
	
	new gName[18];
	
	new ent=-1;	
	while(( ent = find_ent_by_class( ent, "info_target" )) != 0 ){
		entity_get_string(ent,EV_SZ_targetname,gName, sizeof(gName) );
		if( equal(gName, "oxPlace_", 8)) ArrayPushCell(entOXPlace, ent);
	}
}
public transferPlayersToPlaceOX(id){
	
	cs_set_user_team(id,1);
	
	new entList[maxPlayers];
	new entNum;
	for( new i = 0  ; i < ArraySize(entOXPlace); i ++ ){
		entList[entNum++] = ArrayGetCell(entOXPlace, i);
	}
	new playerList[maxPlayers];
	new playerNum;
	for( new i = 1  ; i < maxPlayers; i ++ ){
		if( !is_user_connected(i) || is_user_hltv(i)) continue;
		
		playerList[playerNum++] = i;

	}
	
	new shuffleRand = 0;
	new shuffleTemp = 0;
	for( new i = 0  ; i < entNum; i ++ ){		
		shuffleRand 		=	random(entNum);
		shuffleTemp 		=	entList[shuffleRand];
		entList[shuffleRand]	=	entList[i];
		entList[i]		=	shuffleTemp;	
	}
	
	for( new i = 0  ; i < playerNum; i ++ ){		
		shuffleRand 		=	random(playerNum);
		shuffleTemp 		=	playerList[shuffleRand];
		playerList[shuffleRand]	=	playerList[i];
		playerList[i]		=	shuffleTemp;		
	}
	
	new Float:fOrigin[3];
	new target = 0;
	for( new i = 0, place = 0; i < entNum; i ++ ){
		target		=	playerList[i];
		if( !is_user_connected(target) || !is_user_alive(target) || is_user_hltv(target) || target == id ) continue;
			
		entity_get_vector(entList[place++],	EV_VEC_origin,	fOrigin);
		entity_set_vector(target,		EV_VEC_origin,	fOrigin);	
		
		cs_set_user_team(target, 2);	
		userOX[target] = -1;
		drop_to_floor(target);

	}
	
	set_dhudmessage(25, 255, 0, -1.0, 0.30, 0, 7.0, 5.0);
	show_dhudmessage(0, "Odpowiadaj na pytania stajac na odpowiednie pola!");
		
}
public oxDropPlayersFromTheBox(id){
	for( new i = 1 ; i < maxPlayers;i ++ ){
		
		if( is_user_hltv(i) ) continue;
		if( !is_user_connected(i) || i == id ) continue;
		if( !get_user_team(i) ) continue;		
		if( userOX[i] == -2 ) continue;	
			
		playerDropFromBox(i);
	}
}
public playerDropFromBox(id){			
	userOX[id] = -2;
	set_dhudmessage(25, 255, 0, -1.0, 0.30, 0, 7.0, 5.0);
	show_dhudmessage(id, "Zostales wyrzucony z OX'a!");

	respawnPlayerAdmin(id);
}
public checkOxTouch(id,ent){
		
	if(!OX[OX_START]) return PLUGIN_CONTINUE;
		
	new szTarget[13];	
	entity_get_string(ent, EV_SZ_targetname, szTarget, sizeof(szTarget) - 1);
	
	if( equal(szTarget, "ignore_true") ){
		userOX[id] = 1;		
		Display_Fade(id,(1<<12),(1<<12),(1<<12),97, 255, 0, 40);
	}else if( equal(szTarget, "ignore_false") ){
		userOX[id] = 0;
		Display_Fade(id,(1<<12),(1<<12),(1<<12),255, 1, 0, 40);
	}else if( equal(szTarget, "ignore_dec")){
		userOX[id] = -1;
	}
	return PLUGIN_CONTINUE;
}

public noMenu(){
	for( new i = 1; i < maxPlayers; i ++){
		if(is_user_hltv(i)) continue;
		if(!is_user_connected(i)) continue;	
		if(!get_user_team(i)) continue;	
		if(userOX[i] == -2) continue;	
		show_menu(i, B1, " ", -1, "noMenu");
	}
}
public noMenu_2(id,item) return;
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
