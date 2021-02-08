#include <amxmodx>
#include <amxmisc>
#include <cstrike>
#include <fun>
#include <hamsandwich>
#include <fakemeta>
#include <fakemeta_util>
#include <engine>


public skillsMenu(id){
	new gText[128];
	format(gText, sizeof(gText), "\r[BaseBuilder]\y Umiejetnosci:^n\dTwoje punkty:\r %d", userClassPoint[id])
	new menu = menu_create(gText, "skillsMenu_2");
	
	
	
	
	for(new i = 0; i < sizeof(skillDesc); i++){
		
		if(userSkill[id][i] == str_to_num(skillParam[i][1]))
			format(gText, sizeof(gText), "%s\d [\yUlepszone\d]", skillDesc[i][0]);
		else if(equal(skillDesc[i][2], "bool")) format(gText, sizeof(gText), "%s\d [%s\d]", skillDesc[i][0],  userSkill[id][i] == 0 ? "\rNieaktywne" : "\yAktywne");
		else format(gText, sizeof(gText), "%s\d [\r%d/%s\d]", skillDesc[i][0],  userSkill[id][i] ,skillParam[i][1]);
			
		menu_additem(menu, gText);
	}
	menu_display(id, menu, 0);
	return PLUGIN_CONTINUE;
}

public skillsMenu_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu)
		return PLUGIN_CONTINUE;
	}
	userVarMenu[id] = item
	acceptSkill(id, item);
	return PLUGIN_CONTINUE;
}




public acceptSkill(id, item){
	new skill = userVarMenu[id]
	
	new gText[512], iLen

	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\w[ Informacje ]^n")
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dUmiejetnosc:\r %s^n", skillDesc[skill][0])
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dOpis:\r %s^n", skillDesc[skill][1])
	
	if(equal(skillDesc[skill][4], "1")) 	
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dDruzyna:\r Budowiczy^n")
	else if(equal(skillDesc[skill][4], "2")) 	
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dDruzyna:\r Zombi^n")
	else if(equal(skillDesc[skill][4], "3")) 	
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dDruzyna:\r Budnowniczy + Zombi^n")
	
	if(userSkill[id][skill] == str_to_num(skillParam[skill][1]))
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dPostep:\y Ulepszone^n^n")
	else iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dPostep:\r %d/%s\w |\r ( %0.1f%% )^n^n", userSkill[id][skill], skillParam[skill][1],  userSkill[id][skill]*100.0/str_to_num(skillParam[skill][1]))
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dCena:\r %s PKT^n", skillParam[skill][3])
	
	if(!equal(skillDesc[skill][2], "bool"))			// NIE WYSWIETLA SIE 
		if(equal(skillDesc[skill][2], "percent")) 		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dIle dodaje:\r %s%%^n", skillParam[skill][2])
		else if(equal(skillDesc[skill][2], "float")) 	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dIle dodaje:\r +%s^n", skillParam[skill][2])
		else if(equal(skillDesc[skill][2], "int")) 	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dIle dodaje:\r +%d^n", floatround(str_to_float(skillParam[skill][2])))
			
	if(!(userSkill[id][skill] == 0)){

		if(equal(skillDesc[skill][2], "bool")) 		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dUlepszenie:\r %s^n", userSkill[id][skill] == 0 ? "Nieaktywne" : "Aktywne")
		else if(equal(skillDesc[skill][2], "percent")) 	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dAktualna Szansa:\r %0.2f%%^n", str_to_float(skillParam[skill][2]) * userSkill[id][skill] + 0.00001)
		else if(equal(skillDesc[skill][2], "float")) 	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dUlepszenie Dodaje:\r +%0.2f^n", str_to_float(skillParam[skill][2])* userSkill[id][skill] + 0.00001)
		else if(equal(skillDesc[skill][2], "int")) 	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dUlepszenie Dodaje:\r +%d^n", userSkill[id][skill])
	}
	if(!(userSkill[id][skill] == 0)){
		if(equal(skillDesc[skill][2], "solid"))		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dSzansa:\r %d%%^n", str_to_num(skillParam[skill][5]))
		if(equal(skillDesc[skill][3], "damage")) 	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\dObrazenia:\r %d^n", str_to_num(skillParam[skill][4]) * userSkill[id][skill])
	}
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r1.\w Ulepsz^n")
	
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "\r2.\w %s^n",  userSkill[id][skill] == 0 ? "Rezygnuje" : "Wroc")
	
	if(userSkill[id][skill] >= 1){
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n\r*\d Usuwa jeden punkt z ulepszenia!^n\r*\d Nie da sie go odzyskac!")
		iLen += format(gText[iLen], sizeof(gText)-iLen-1, "^n^n\r7.\y Usun Ulepszenie\d (\r -1 PKT\d )^n")
	}
	show_menu(id, B1 | B2 | B7 , gText, -1, "acceptSkill")
	return PLUGIN_CONTINUE;
}

public acceptSkill_2(id, item){
	
	new skill = userVarMenu[id]
	
	new cost = str_to_num(skillParam[skill][3])
	
	switch(item){
		
		case 0:{
			if(userSkill[id][skill] >= str_to_num(skillParam[skill][1])){
				ColorChat(id, GREEN, "---^x01 Ulepszyles juz na maxa ta umiejetnosc!^x04 ---");
				acceptSkill(id, skill)
				return PLUGIN_CONTINUE;
			}
	
			if(userClassPoint[id] < cost){
				ColorChat(id, GREEN, "%s Nie posiadasz wystarczajacej ilosci punktow!", PREFIXSAY);
				return PLUGIN_CONTINUE;
			}
			userSkill[id][skill] += 1
			userClassPoint[id] -= cost
			ColorChat(0, GREEN, "---^x01 Gracz:^x03 %s^x01 ulepszyl umiejetnosc:^x03 %s^x04^x01 na poziom^x04 [^x03%d/%s^x04]^x04 ---", userName[id], skillDesc[skill][0], userSkill[id][skill], skillParam[skill][1]);
			acceptSkill(id, skill)
		
		}
		case 1:{
			skillsMenu(id)
		}
		case 6:{
			if(userSkill[id][skill] >= 1){
				if(userSkill[id][skill] <= 0 ){
					ColorChat(id, GREEN, "%s Nie mozesz usunac na -!", PREFIXSAY);
					acceptSkill(id, skill)
					return PLUGIN_CONTINUE;	
				}
				
				userSkill[id][skill] -= 1
				ColorChat(id, GREEN, "%s Usunales jeden pkt!", PREFIXSAY);
				acceptSkill(id, skill)
				return PLUGIN_CONTINUE;
			}
			acceptSkill(id, skill);
			return PLUGIN_CONTINUE;
			
		}
		default: {}

	}
	return PLUGIN_CONTINUE
}
	
public Float:bb_bonus_skill(id, skill){

	if(equal(skillDesc[skill][2], "float")) 		return str_to_float(skillParam[skill][2]) * userSkill[id][skill]
	else if(equal(skillDesc[skill][2], "percent"))	return str_to_float(skillParam[skill][2]) * userSkill[id][skill]
	else if(equal(skillDesc[skill][2], "int")) 	return str_to_float(skillParam[skill][2]) * userSkill[id][skill]
	return 0.1
}

public bb_percent_skill(id, skill){
	
	if(equal(skillDesc[skill][2], "percent"))
		return (random_float(0.00, 100.00) <=  str_to_float(skillParam[skill][2]) * userSkill[id][skill] )
	else if(equal(skillDesc[skill][2], "solid"))
		return (random_float(0.00, 100.00) <=  str_to_float(skillParam[skill][5]))
	return bb_percent_skill(id, skill)
}


public bb_skill(id, skill){
	return userSkill[id][skill] 	
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
