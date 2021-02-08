#include <amxmodx>
#include <amxmisc>
#include <cstrike>
#include <fun>
#include <hamsandwich>
#include <fakemeta>
#include <fakemeta_util>
#include <engine>
#include <colorchat>

public menuTeamOption(id){
	new gText[256], iLen = 0;
	if(userTeam[id] == 0){
		ColorChat(id, GREEN, "---^x01 Nie posiadasz z tym graczem juz teamu!^x04 ---");
		userTeam[id] ++;
		return;	
	}
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r[BaseBuilder]\y Menu Druzyny^n^n");
	
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dAktualnie posiadasz team z:\r %s^n", symbolsCustom[SYMBOL_DR_ARROW], userName[userTeam[id]])
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dBonus Team:\r %0.2f%%\d wiecej EXP'a^n", symbolsCustom[SYMBOL_DR_ARROW], bonusExpTeam)
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dCo runde jest losowana inna wartosc!^n", symbolsCustom[SYMBOL_DR_ARROW])
	
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "^n\y%s^t^t\dAby sie przeteleportowac\r ( Kucnij oraz kliknij 1 )", symbolsCustom[SYMBOL_DR_ARROW])
	
	new menu = menu_create(gText, "menuTeamOption_2")
	
	menu_additem(menu, "Teleport\d [\y Kucnij\d ]")
	
	format(gText,sizeof(gText), "\wExp:\r %s", typeExpParty[userTeamExp[id]][0]);
	menu_additem(menu, gText)
	
	format(gText,sizeof(gText), "\wBrylki:\r %s^n", typeNuggetParty[userTeamNugget[id]][0]);
	menu_additem(menu, gText)
	
	if( buildTime || prepTime || (get_user_team(id) != get_user_team(userTeam[id])))
		menu_additem(menu, "Rozlacz")
	else menu_additem(menu, "\dRozlacz")
	
	menu_display(id, menu, 0)
}
public menuTeamOption_2(id, menu, item){
	if( item == MENU_EXIT){
		menu_destroy(menu)
		return;
	}
	switch(item){
		case 0:{
			if (buildTime || prepTime){
				if (userTeam[id] && get_user_team(id) == 2){
					if (is_user_alive(userTeam[id]) && is_user_connected(userTeam[id]) && get_user_team(userTeam[id]) == get_user_team(id)){
						if (pev(id, pev_button) & IN_DUCK){
							new Float:fOrigin[3] = 0.0;
							pev(userTeam[id], pev_origin, fOrigin);
							set_pev(id, pev_origin, fOrigin);
							menuTeamOption(id);
							return;
						}
						menuTeamOption(id)
						ColorChat(id, GREEN, "%s Kucnij aby uzyc teleportu", PREFIXSAY);
					}
				}
			} else {
				ColorChat(id, GREEN, "%s Teleport jest wylaczony podczas rundy!", PREFIXSAY);
				menuTeamOption(id)
			}
		}
		case 1:{
			userTeamExp[id] 		= (userTeamExp[id]+1) 	% sizeof(typeExpParty);
			ColorChat(userTeam[id], 	GREEN, "%s Gracz:^x03 %s^x01 %s", PREFIXSAY, userName[id], typeExpParty[userTeamExp[id]][1]);
			menuTeamOption(id);	
		}
		case 2:{
			userTeamNugget[id] 	= (userTeamNugget[id]+1) % sizeof(typeNuggetParty);
			ColorChat(userTeam[id], 	GREEN, "%s Gracz:^x03 %s^x01 %s", PREFIXSAY, userName[id], typeNuggetParty[userTeamNugget[id]][1]);
			menuTeamOption(id);	
		}
		case 3:{
			if( buildTime || prepTime ||(get_user_team(id) != get_user_team(userTeam[id]))){
				new target = userTeam[id]
				
				if( target == 0 ) return;
				
				userTeam[target] = 0
				userTeam[id] 	 = 0
				
				ColorChat(id, GREEN,"---^x01 Twoja druzyna zostala rozlaczona^x04 ---")
				ColorChat(target, GREEN,"---^x01 Twoja druzyna zostala rozlaczona^x04 ---")
			}else{
				ColorChat(id, GREEN,"---^x01 Nie mozna rozlaczyc w trakcie rundy^x04 ---")
			}	
		}
	}
}
public teamOption(id){
	if( userTeam[id] == 0 ){
		if( menuTeam(id) == 0 ){				
			ColorChat(id, GREEN,"---^x01 Brakuje graczy^x04 ---")
			return
		}	
	} else menuTeamOption(id);
}
public menuTeam(id){
	new gText[128]
	new menu = menu_create("\r[BaseBuilder]\y Kogo chcesz zaprosic?", "menuTeam_2")
	
	format(gText, sizeof(gText), "\yZablokuj druzyne: %s^n", hasOption(userSaveOption[id], save_TEAM) ? "\rTAK" : "\dNIE");
	menu_additem(menu, gText);
	
	new x = 1
	for( new i = 1; i < maxPlayers; i++ ){
		if( !is_user_connected(i) || !is_user_alive(i) || i == id || userRoundTeam[id] != userRoundTeam[i] || userTeamBlock[i] || hasOption(userSaveOption[i], save_TEAM)|| userTeam[i] != 0 || get_user_team(id) != get_user_team(i)) 
			continue
		
		format(gText, sizeof(gText), "%s", userName[i] )
		menu_additem(menu, gText)
		userVarList[id][x++] = i
	}
	menu_display(id, menu, 0)
	return x;
}
public menuTeam_2(id,menu,item){
	if( item == MENU_EXIT ){
		menu_destroy(menu)
		return
	}
	switch(item){
		case 0:{
			if( hasOption(userSaveOption[id], save_TEAM) ){
				removeOption(userSaveOption[id], save_TEAM)
				ColorChat(id, GREEN, "---^x01 Druzyna odblokowana^x04 ---")
			}else{
				addOption(userSaveOption[id], save_TEAM)
				ColorChat(id, GREEN, "---^x01 Druzyna zablokowana^x04 ---")
			}
			menuTeam(id)
		}
		default:{
			userMenuId++;
			new target = userVarList[id][item]
			
			if( !is_user_connected(target) || userRoundTeam[target] != userRoundTeam[id] || userTeamBlock[target] || userTeam[target] != 0 || get_user_team(target) != get_user_team(id)) {
				ColorChat(id, GREEN, "%s Nie mozna stworzyc druzyny z tym graczem!", PREFIXSAY);
				menuTeam(id);
			}
			userTeamMenu[id] 	= userMenuId
			userTeamMenu[target] 	= userMenuId
			userTeamSend[target] 	= id
			
			ColorChat(id, GREEN, "---^x01 Wyslales zaproszenie do druzyny graczowi:^x03 %s^x04 ---", userName[target]);
			ColorChat(target, GREEN, "---^x01 Otrzymales zaproszenie do druzyny od gracza:^x03 %s^x04 ---", userName[id]);
			
			menuConfirmationTeam(target)
		}
	}
}
public menuConfirmationTeam(id){
	new gText[256], iLen = 0;
	new target = userTeamSend[id];
	
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\r[BaseBuilder]\y Dostales zaproszenie do druzyny!^n^n")
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dZaproszenie do druzyny zostalo wyslane przez\r %s!^n", symbolsCustom[SYMBOL_DR_ARROW], userName[target] )
	iLen += format(gText[iLen], sizeof(gText)-1-iLen, "\y%s^t^t\dCzy chcesz stworzyc druzyne?^n", symbolsCustom[SYMBOL_DR_ARROW])

	new menu = menu_create(gText, "menuConfirmationTeam_2")
	
	menu_additem(menu, "Tak, Pewnie ")
	menu_additem(menu, "Nie, Wole sam")
	menu_display(id, menu, 0)
}
public menuConfirmationTeam_2(id,menu,item){
	
	new target = userTeamSend[id]
	
	if( item == MENU_EXIT ){
		menu_destroy(menu)
		return
	}
	switch(item){
		case 0:{
			if( userTeamMenu[id] != userTeamMenu[target] ){
				ColorChat(id, GREEN,"---^x01 Za pozno zeby zaakceptowac^x04 ---")
				return
			}
			
			if(get_user_team(id) != get_user_team(target)){
				ColorChat(id, GREEN,"---^x01 Osoba jest w innej druzynie^x04 ---")
				return
			}
			if(userTeam[id] == 0 && userTeam[target] == 0){
			
				userTeam[id]		= target
				userTeam[target] 	= id	
				
				ColorChat(id, GREEN,"---^x01 Druzyna z^x04 %s^x01 aktywna^x04 ---", userName[target])
				ColorChat(target, GREEN,"---^x01 Druzyna z^x04 %s^x01 aktywna^x04 ---", userName[id])
				
				menuTeamOption(id)
				menuTeamOption(target)
				
			} else {
				ColorChat(id, GREEN,"---^x01 Posiadasz juz team^x04 ---")
			}
		}
		case 1:{
			ColorChat(id, GREEN,"---^x01 Druzyna od^x04 %s^x01 zostala odrzucona^x04 ---", userName[target])
			ColorChat(target, GREEN,"---^x01 Druzyna z^x04 %s^x01 odrzucona^x04 ---", userName[id])
			userTeam[id]		= 0
			userTeam[target] 	= 0
		}
	}
}
public leaveParty(id){
	new target = userTeam[id]
	if( target == 0 ) return;
	userTeam[id] 		= 0 ;
	userTeam[target] 	= 0
	reWriteBlocksParty(id, target)
	ColorChat(target, GREEN,"---^x01 Druzyna rozlaczona^x04 ---")
}
public teamLineOrSprite(id){
	if (userTeam[id] && !gameTime && !hasOption(userSaveOption[id], save_INVIS) ){
		if (teamWorks(id) && get_user_team(id) == 2){
			new Float:fOriginId[3] = 0.0;
			new Float:fOriginTeam[3] = 0.0;
			pev(id, pev_origin, fOriginId);
			pev(userTeam[id], pev_origin, fOriginTeam);
			if (get_distance_f(fOriginId, fOriginTeam) > 350) Create_TE_PLAYERATTACHMENT(id, userTeam[id], 40, team_spr, 10);
			if (floatsub(get_gametime(), userTeamLine[id]) > 3.55){
				userTeamLine[id] = get_gametime();
				if (get_distance_f(fOriginId, fOriginTeam) > 350) drawLine(id, fOriginId, fOriginTeam, 25,65,170, 3, 10, 0);
			}
		}
	}
}
public bool:teamWorks(id){
	return userTeam[id] && get_user_team(userTeam[id]) == get_user_team(id);
}
public bool:isWithParty(id, id2){
	if(!pev_valid(id) || !pev_valid(id2)) return false;
	if (id2 != userTeam[id]) return false;
	if (id != userTeam[id2]) return false;
	return true;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
