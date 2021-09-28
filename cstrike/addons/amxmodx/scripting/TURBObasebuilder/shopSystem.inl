#include <amxmodx>
#include <amxmisc>
#include <cstrike>
#include <fun>
#include <hamsandwich>
#include <fakemeta>
#include <fakemeta_util>
#include <engine>

new bool:granadeMission[33][4];

public shopGlobalMenu(id){
	userTeamOpen[id] = get_user_team(id);
			
	switch(get_user_team(id)){
		case USER_ZOMBIE:shopZombie(id);
		case USER_HUMAN:shopBuilder(id);	
	}
}
public shopBuilder(id){
	
	if(!is_user_connected(id)) return PLUGIN_CONTINUE;
	
	new gText[128];
	format(gText, sizeof(gText), "\r[BaseBuilder]\y Sklep Budowniczych:^n^n\y%s^t^t\dPosiadasz:\r %d Brylek", symbolsCustom[SYMBOL_DR_ARROW], userNugget[id]);
	new menu = menu_create(gText, "shopBuilder_2");
	for(new i = 0; i < sizeof(shopDescBuilder); i++){
		new cost =  str_to_num(shopDescBuilder[i][3]) + 50 * userReset[id];
		format(gText, sizeof(gText), "%s%s\d | Cena:\r %d\d | %s%d/%s", userShopBuilder[id][i] >=  str_to_num(shopDescBuilder[i][2]) ? "\d" : "\w", shopDescBuilder[i][0], cost, userShopBuilder[id][i] >=  str_to_num(shopDescBuilder[i][2]) ? "\d" : "\w" , userShopBuilder[id][i], shopDescBuilder[i][2]);
		menu_additem(menu, gText);
	}
	menu_display(id, menu, 0);
	return PLUGIN_CONTINUE; 
}
public shopBuilder_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return PLUGIN_CONTINUE;
	}
	if(buildTime  || roundEnd || !gameTime ){
		chatPrint(id, PREFIX_NORMAL, "Za wczesnie na to!");
		return PLUGIN_CONTINUE;
	}
	if( !is_user_alive(id) ){
		chatPrint(id, PREFIX_NORMAL, "Nie zyjesz!");
		shopGlobalMenu(id);
		return PLUGIN_CONTINUE;
	}
	if( get_user_team(id) != USER_HUMAN ){
		chatPrint(id, PREFIX_LINE, "Twoja druzyna sie zmienila! ");
		shopGlobalMenu(id);
		return PLUGIN_CONTINUE;	
	}
	new cost = str_to_num(shopDescBuilder[item][3]) + 50 * userReset[id];
	new weapons[32];
	new num; 
	get_user_weapons(id, weapons, num);

	if(userNugget[id] < cost){
		chatPrint(id, PREFIX_NORMAL, "Nie posiadasz wystarczajacej ilosci Brylek!");
		return PLUGIN_CONTINUE;
	}
	if(userShopBuilder[id][item] >= str_to_num(shopDescBuilder[item][2])){
		chatPrint(id, PREFIX_NORMAL, "Kupiles juz wszystkie na ta runde!");
		shopBuilder(id);
		return PLUGIN_CONTINUE;
	}
	switch(item){
		case shopB_HP:{
			set_user_health(id, min(userMaxHealth[id], get_user_health(id) + str_to_num(shopDescBuilder[item][1])));
			chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 kupil:^4 +%sHP!", userName[id],shopDescBuilder[item][1]);
			addMission(id, mission_BUYHEALTH, 1);
			addPro(id, pro_BETON, 1);
			#if defined CHRISTMAS_ADDON
				addChristmasMission(id,CH_HEAL, 1);				
			#endif

		}
		case shopB_AMMO:{
		
			if( get_gametime() < userUnlimited[id] ){				
				chatPrint(id, PREFIX_LINE, "Juz masz^3 Bezlik Ammo^1 !");
				shopBuilder(id);
				return PLUGIN_CONTINUE;
			}
			userUnlimited[id] = get_gametime() + str_to_float(shopDescBuilder[item][1]);	
			chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 kupil:^4 Bezlik na %s Sekund", userName[id], shopDescBuilder[item][1]);
		}
		case shopB_DMG:{
			if( userExtraDmg[id]){
				
				chatPrint(id, PREFIX_LINE, "Juz masz^3 Dodatkowy DMG!");
				shopBuilder(id);
				return PLUGIN_CONTINUE;
			}
			userExtraDmg[id] = true;
			chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 kupil:^4 +%sDMG", userName[id], shopDescBuilder[item][1]);
		}
		
		
		case shopB_RECOIL:{
			if( userNoRecoil[id] ){
							
				chatPrint(id, PREFIX_LINE, "Juz masz^3 NoRecoil!");
				shopBuilder(id);
				return PLUGIN_CONTINUE;
			}
			
			userNoRecoil[id] = true;
			chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 kupil:^4 NoRecoil",  userName[id]);
		}
		
		case shopB_NADE:{
			new bool:grenade=false;
			for(new i = 0; i < num; i++){
				if (weapons[i] == CSW_HEGRENADE){
					grenade = true;
					break;
				
				}
			}
			if(grenade){
				chatPrint(id, PREFIX_LINE, "Juz masz^3 Granat Podpalajacy^1 !");
				return PLUGIN_CONTINUE;
			}
			chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 kupil:^4 Granat Podpalajacy",  userName[id]);
			give_item(id, "weapon_hegrenade");
			
			granadeMission[id][0] = true;
			
			#if defined CHRISTMAS_ADDON
				addChristmasMission(id,CH_GRANADE, 1);		
			#endif
			
		}
		
		
		case shopB_FROST:{
			new bool:smoke = false;
			
			for(new i = 0; i < num; i++){
				if (weapons[i] == CSW_SMOKEGRENADE){
					smoke = true;
					break;
				}
			}
			
			if(smoke){
				chatPrint(id, PREFIX_LINE, "Juz masz^3 Granat Zamrazajacy^1 !");
				return PLUGIN_CONTINUE;
			}
			
			
			chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 kupil:^4 Granat Zamrazajacy",  userName[id]);
			give_item(id, "weapon_smokegrenade");
			
			granadeMission[id][1] = true;
			
			#if defined CHRISTMAS_ADDON
		
				addChristmasMission(id,CH_GRANADE, 1);
							
			#endif
			
		}
		case shopB_PUSH:{
			new bool:flash = false;
			
			for(new i = 0; i < num; i++){
				if (weapons[i] == CSW_FLASHBANG){
					flash = true;
					break;
				}
			}
			
			if(flash){
				chatPrint(id, PREFIX_LINE, "Juz masz^3 Granat Odpychajacy^1 !");
				return PLUGIN_CONTINUE;
			}
			
			
			chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 kupil:^4 Granat Odpychajacy",  userName[id]);
			give_item(id, "weapon_flashbang");
			
			granadeMission[id][3] = true;
			
			#if defined CHRISTMAS_ADDON
		
				addChristmasMission(id,CH_GRANADE, 1);
							
			#endif
			
		}
		case shopB_AUTOKAMPA:{
			new bool:autoCamp = false;
			
			for(new i = 0; i < num; i++){
				if (weapons[i] == CSW_G3SG1 || weapons[i] == CSW_SG550 ){
					autoCamp = true;
					break;
				}
			}
			if(autoCamp){
				chatPrint(id, PREFIX_LINE, "Juz masz^3 Auto Kampe^1 !");
				return PLUGIN_CONTINUE;
			}
					
			give_item(id, random(2) == 0 ? "weapon_sg550" :  "weapon_g3sg1");
			
			userAutoSniper[id] = get_gametime() + str_to_float(shopDescBuilder[item][1]);
			set_task(str_to_float(shopDescBuilder[item][1]), "removeWeapon", id);
			chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 kupil:^4 AutoKampe na %ssekund",  userName[id], shopDescBuilder[item][1]);
				
		}
		case shopB_KROWA:{
			new bool:machineGun = false;
			for(new i = 0; i < num; i++){
				if (weapons[i] == CSW_M249 ){
					machineGun = true;
					break;
				}
			}
			if(machineGun){
				chatPrint(id, PREFIX_LINE, "Juz masz^3 Krowe^1 !");
				return PLUGIN_CONTINUE;
			}
			give_item(id, "weapon_m249");
			
			userMiniGun[id] = get_gametime() + str_to_float(shopDescBuilder[item][1]);
			set_task(str_to_float(shopDescBuilder[item][1]), "removeWeapon", id);
			chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 kupil:^4 Krowe na %ssekund",  userName[id], shopDescBuilder[item][1]);
		}
		
	}

	if(granadeMission[id][0] && granadeMission[id][1] && !granadeMission[id][2] && granadeMission[id][3]){
		addSecretMission(id, mission_secret_GRENADIER, 1);
		granadeMission[id][2] = true;
	}
		
	userShopBuilder[id][item]++;
	userNugget[id] -= cost;
	userNuggetCollectedRound[id] -= cost;
	
	shopBuilder(id);
	return PLUGIN_CONTINUE;
}
public removeWeapon(id){
	new weapons[32]; new num; get_user_weapons(id, weapons, num);
	for(new i = 0; i < num; i++){
		if (weapons[i] == CSW_G3SG1)takeWeapon(id, "weapon_g3sg1");
		else if ( weapons[i] == CSW_SG550 ) takeWeapon(id, "weapon_sg550");
		else if ( weapons[i] == CSW_M249 ) takeWeapon(id, "weapon_m249");
	}
}
public shopZombie(id){
	
	if(!is_user_connected(id)) return PLUGIN_CONTINUE;
	
	new gText[128];
	format(gText, sizeof(gText), "\r[BaseBuilder]\y Sklep Zobmie:^n^n\y%s^t^t\dPosiadasz:\r %d Brylek",symbolsCustom[SYMBOL_DR_ARROW], userNugget[id]);
	new menu = menu_create(gText, "shopZombie_2");
	for(new i = 0; i < sizeof(shopDescZombie); i++){ 
		new cost = str_to_num(shopDescZombie[i][3]) + 50 * userReset[id];
		format(gText, sizeof(gText), "%s%s\d | Cena:\r %d\d | %s%d/%s", userShopZombie[id][i] >=  str_to_num(shopDescZombie[i][2]) ? "\d" : "\w", shopDescZombie[i][0], cost, userShopZombie[id][i] >=  str_to_num(shopDescZombie[i][2]) ? "\d" : "\w" , userShopZombie[id][i], shopDescZombie[i][2]);
		menu_additem(menu, gText);
		
	}
	menu_display(id, menu, 0);
	return PLUGIN_CONTINUE;
}

public shopZombie_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu);
		return PLUGIN_CONTINUE;
	}
	if(buildTime  || roundEnd || !gameTime ){
		chatPrint(id, PREFIX_NORMAL, "Za wczesnie na to!");
		return PLUGIN_CONTINUE;
	}
	if( get_user_team(id) != 1 ){
		chatPrint(id, PREFIX_LINE, "Twoja druzyna sie zmienila! ");
		shopGlobalMenu(id);
		return PLUGIN_CONTINUE;	
	}
	new cost = str_to_num(shopDescZombie[item][3]) + 50 * userReset[id];
	
	if(userNugget[id] < cost){
		chatPrint(id, PREFIX_NORMAL, "Nie posiadasz wystarczajacej ilosci Brylek!");
		return PLUGIN_CONTINUE;
	}
	if(userShopZombie[id][item] >= str_to_num(shopDescZombie[item][2])){
		chatPrint(id, PREFIX_NORMAL, "Kupiles juz wszystkie na ta runde!");
		shopZombie(id);
		return PLUGIN_CONTINUE;
	}
	switch(item){
		
		case 0:{
			
			userHPAddRound[id] += str_to_num(shopDescZombie[item][1]);
			set_user_health(id, get_user_health(id) + str_to_num(shopDescZombie[item][1]));
			userMaxHealth[id] +=  str_to_num(shopDescZombie[item][1]);
			chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 kupil:^4 %sHP^1 na cala runde!^4 [^3 %dHP^4 ]",  userName[id], shopDescZombie[item][1], get_user_health(id));
		}
		
		case 1:{
			if(buildTime || prepTime){
				chatPrint(id, PREFIX_NORMAL, "Za wczesnie na to!");
				return PLUGIN_CONTINUE;
			}
			userMaxHealth[id] += str_to_num(shopDescZombie[item][1]);
			set_user_health(id, get_user_health(id) + str_to_num(shopDescZombie[item][1]));
			chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 kupil:^4 %sHP! [^3 %dHP^4 ]",  userName[id], shopDescZombie[item][1],  get_user_health(id));
		}
		
		case 2:{
			if(buildTime || prepTime){
				chatPrint(id, PREFIX_NORMAL, "Za wczesnie na to!");
				return PLUGIN_CONTINUE;
			}
			
			if(userClass[id] == class_HEALTH){
				chatPrint(id, PREFIX_LINE, "Zmien Zombie! Nie mozna tego kupic majac Pielegniarke!");
				return PLUGIN_CONTINUE;
			}
			
			if(userHPRegen[id] ){
				chatPrint(id, PREFIX_LINE, "Juz masz^3 Regeneracje^1 !");
				shopZombie(id);
				return PLUGIN_CONTINUE;
			}
			chatPrint(0, PREFIX_NORMAL, "Gracz^3 %s^1 kupil:^4 Regeneracje HP!",  userName[id]);
			userHPRegen[id] = true;
			set_task(1.0, "regenHP", id);
			
		}
	}
	
	userDeathPrice[id][PRICE_BUYGOD] = true;
	
	userShopZombie[id][item]++;
	userNugget[id] -= cost;
	userNuggetCollectedRound[id] -= cost;
	shopZombie(id);
	return PLUGIN_CONTINUE;
}

public regenHP(id){
	if(!is_user_connected(id) || !userHPRegen[id]) return;
		
	if(is_user_alive(id))
		set_user_health(id, min(userMaxHealth[id], get_user_health(id) + str_to_num(shopDescZombie[shopZ_REGENERACJA][1])));
		
	set_task(1.0, "regenHP", id);
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
