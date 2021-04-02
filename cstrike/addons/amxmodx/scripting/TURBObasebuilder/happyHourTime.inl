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
	if(!isSuperAdmin(id)) return;
	
	new gText[128];
	
	new nameNew[64];
	
	copy(nameNew, sizeof(nameNew), happyHourDesc[randomHappyHour][0]);
	replace_all(nameNew, sizeof(nameNew) - 1, "%%", "%");
	
	format(gText, sizeof(gText), "\r[BaseBuilder]\y Menu HappyHour^n\y%s^t^t\dTeraz:\r %s\w | \d", symbolsCustom[SYMBOL_DR_ARROW], hourTime ? nameNew : "Wlacz najpierw");
	new menu = menu_create(gText, "happyMenu_2");
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
		menu_destroy(menu);
		return;
	}
	
	pageHappy[id] = item;

	switch(item){
		case 0:{
			hourTime =! hourTime;
			randomHappyHour = random(happy_TOTAL);
			chatPrint(0, PREFIX_LINE, "Admin^3 %s^1 %s^3 HappyTime", userName[id], hourTime ? "wlaczy" : "wylaczyl" );
			happyMenu(id);
		}
		case 1:{
			if(!hourTime){
				chatPrint(id, PREFIX_LINE, "Wlacz najpierw HappyTime");
				happyMenu(id);
				return;
			}
			new randomHappy = random(happy_TOTAL);
			
			chatPrint(0, PREFIX_LINE, "Admin^3 %s^1 zmienil^3 HappyTime", userName[id]);
			chatPrint(0, PREFIX_LINE, "Teraz jest:^3 %s", happyHourDesc[randomHappy][0]);
			
			randomHappyHour = randomHappy;
			happyMenu(id);
		}
		default:{
			if(!hourTime){
				chatPrint(id, PREFIX_LINE, "Wlacz najpierw HappyTime");
				happyMenu(id);
				return;
			}
			if(randomHappyHour == item-2){
				chatPrint(id, PREFIX_LINE, "Juz jest wlaczone!");
				happyMenu(id);
				return;
			}
			randomHappyHour = item-2;
			chatPrint(0, PREFIX_LINE, "Admin^3 %s^1 wlaczyl^3 HappyTime", userName[id]);
			chatPrint(0, PREFIX_LINE, "Teraz jest:^3 %s", happyHourDesc[item-2][0]);
			happyMenu(id);
		}
	}
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
