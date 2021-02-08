#include <amxmodx>
#include <amxmisc>

happyStart(){
	if( random(100)  <= (happyHourChange)){
		randomHappyHour = random(happy_TOTAL);
		hourTime = true;
	} else hourTime = false;
}
public bb_happy_hour(){
	return randomHappyHour;
}

new pageHappy[33];

public happyMenu(id){
	if(!isSuperAdmin(id))
		return;
	
	new gText[128];
	
	new nameNew[64];
	
	copy(nameNew, sizeof(nameNew), happyHourDesc[randomHappyHour][0]);
	replace_all(nameNew, sizeof(nameNew) - 1, "%%", "%");
	
	format(gText, sizeof(gText), "\r[BaseBuilder]\y Menu HappyHour^n\y%s^t^t\dTeraz:\r %s\w | \d", symbolsCustom[SYMBOL_DR_ARROW], hourTime ? nameNew : "Wlacz najpierw");
	new menu = menu_create(gText, "happyMenu_2")
	format(gText, sizeof(gText), "\yHappy:\w [ %s%s\w ]", hourTime ? "\r" : "\d", symbolsCustom[SYMBOL_DOT]);
	menu_additem(menu, gText);
	
	menu_additem(menu, "\rLosowy Happy^n");
	
	for(new i = 0; i < sizeof(happyHourDesc); i++){
		format(gText, sizeof(gText), "%s", happyHourDesc[i][1]);
		menu_additem(menu, gText);
	}
	menu_display(id, menu, pageHappy[id]/7);
}

public happyMenu_2(id, menu, item){
	if(item == MENU_EXIT){
		menu_destroy(menu)
		return;
	}
	
	pageHappy[id] = item;

	switch(item){
		case 0:{
			hourTime =! hourTime
			randomHappyHour = random(happy_TOTAL);
			ColorChat(0, GREEN, "---^x01 Admin^x03 %s^x01 %s^x03 HappyTime^x04 ---", userName[id], hourTime ? "wlaczy" : "wylaczyl" );
			happyMenu(id)
		}
		case 1:{
			if(!hourTime){
				ColorChat(id, GREEN, "---^x01 Wlacz najpierw HappyTime^x04 ---")
				happyMenu(id)
				return;
			}
			new randomHappy = random(happy_TOTAL)
			
			ColorChat(0, GREEN, "---^x01 Admin^x03 %s^x01 zmienil^x03 HappyTime^x04 ---", userName[id]);
			ColorChat(0, GREEN, "---^x01 Teraz jest:^x03 %s^x04 ---", happyHourDesc[randomHappy][0]);
			
			randomHappyHour = randomHappy;
			happyMenu(id)
		}
		default:{
			if(!hourTime){
				ColorChat(id, GREEN, "---^x01 Wlacz najpierw HappyTime^x04 ---")
				happyMenu(id)
				return;
			}
			if(randomHappyHour == item-2){
				ColorChat(id, GREEN, "---^x01 Juz jest wlaczone!^x04 ---")
				happyMenu(id)
				return;
			}
			randomHappyHour = item-2;
			ColorChat(0, GREEN, "---^x01 Admin^x03 %s^x01 wlaczyl^x03 HappyTime^x04 ---", userName[id]);
			ColorChat(0, GREEN, "---^x01 Teraz jest:^x03 %s^x04 ---", happyHourDesc[item-2][0]);
			happyMenu(id)
		}
	}
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
