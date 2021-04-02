#include <amxmodx>
#include <amxmisc>
#include <cstrike>
#include <fun>
#include <hamsandwich>
#include <fakemeta>
#include <fakemeta_util>
#include <engine>


public globalMenuCave(id){	
	if(unlockCave[id]) miningMenu(id);	
	else buyCave(id);
	
}
public buyCave(id){
	new gText[556], iLen;
	new size = sizeof(gText)-iLen-1;
	
	new newUpgradeLeft  =  userLastUpgradeTime[id] + upgradeCave - playedTime(id);
	iLen += format(gText[iLen], size, "\dAby odblokowac kopalnie trzeba najpierw ja kupic!^n");
	iLen += format(gText[iLen], size, "\dZakup trwa:\r %dgodz %s%dmin %s%dsek\d, po tym czasie^n", ( upgradeCave / HOUR ), ( upgradeCave / MINUTE % MINUTE )<10?"0":"", ( upgradeCave / MINUTE % MINUTE ), (upgradeCave%MINUTE)<10?"0":"", ( upgradeCave %MINUTE ));
	iLen += format(gText[iLen], size, "\dKopalnia jest przypisana do twojego konta i mozesz zaczac zarabiac!^n^n");
	
	if(newUpgradeLeft > 0) iLen += format(gText[iLen], size, "\dCzas zakupu:\r %dgodz %s%dmin %s%dsek^n",  ( newUpgradeLeft / HOUR ), ( newUpgradeLeft / MINUTE % MINUTE )<10?"0":"", ( newUpgradeLeft / MINUTE % MINUTE ), (newUpgradeLeft%MINUTE)<10?"0":"", ( newUpgradeLeft %MINUTE ));
	else if(startUpgrade[id] && newUpgradeLeft <= 0) iLen += format(gText[iLen], size, "\wKoplania zostala odblokowana, aby ja odebrac kliknij\r (1.)^n^n");
	
	iLen += format(gText[iLen], size, "\dWymagany Lv:\r %d^n",levelMiner);
	iLen += format(gText[iLen], size, "\dKoszt:\r %d Brylek^n", costMiner);
	
	if(startUpgrade[id] && newUpgradeLeft <= 0) 
		iLen += format(gText[iLen], size, "^n\r1.\y Odbierz pozwolenie");
	else if(newUpgradeLeft > 0 && startUpgrade[id])  
		iLen += format(gText[iLen], size, "^n\d1. Zablokowane [ KUPUJE ]");
	else if(userLevel[id] < levelMiner && userNugget[id] < costMiner)
		iLen += format(gText[iLen], size, "^n\d1. Zablokowane [ Brakuje %d lvl, %d brylek ]", levelMiner, costMiner);
	else if(userLevel[id] < levelMiner )
		iLen += format(gText[iLen], size, "^n\d1. Zablokowane [ Brakuje %d lvl ]", levelMiner);
	else if(userNugget[id] < costMiner )
		iLen += format(gText[iLen], size, "^n\d1. Zablokowane [ Brakuje %d brylek ]", costMiner);
	else
		iLen += format(gText[iLen], size, "^n\r1.\y Kup Kopalnie");
	
	iLen += format(gText[iLen], size, "^n\r2.\y Wroc");	
	
	showMenu(id, B1 | B2, gText, -1, "buyCave" );
}
public buyCave_2(id, item){
	new newUpgradeLeft  =  userLastUpgradeTime[id] + upgradeCave - playedTime(id);
	switch(item){
		case 0:{
			if(startUpgrade[id] && newUpgradeLeft <= 0){
				userLastStaminaTime[id] 	= playedTime(id);
				unlockCave[id] = 1;
				
				new gText[128];
				logType[id] = LOG_CAVE;
				if(logType[id] == LOG_CAVE) {
					format(gText, sizeof(gText), "odblokowal kopalnie");
					logBB(id, gText);
				}
				
				chatPrint(id, PREFIX_NORMAL, "Odblokowales Kopalnie!");
				return PLUGIN_CONTINUE;
			}
			else if(newUpgradeLeft > 0 && startUpgrade[id]){
				chatPrint(id, PREFIX_NORMAL, "Teraz sie ulepsza!");
				return PLUGIN_CONTINUE;
			}
			else if(userLevel[id] < levelMiner && userNugget[id] < costMiner){
				chatPrint(id, PREFIX_NORMAL, "Nie mozesz tego ulepszyc!^4 |^3 Brakuje ci %d lvl oraz %d brylek!", levelMiner, costMiner);
				return PLUGIN_CONTINUE;
			}
			else if(userLevel[id] < levelMiner){
				chatPrint(id, PREFIX_NORMAL, "Nie mozesz tego ulepszyc!^4 |^3 Brakuje ci %d lvl!", levelMiner);
				return PLUGIN_CONTINUE;
			}
			else if(userNugget[id] < costMiner){
				chatPrint(id, PREFIX_NORMAL, "Nie mozesz tego ulepszyc!^4 |^3 Brakuje ci brylek!");
				return PLUGIN_CONTINUE;
			}
			globalMenuCave(id);
			userNugget[id] -= costMiner;
			startUpgrade[id] = 1;
			chatPrint(id, PREFIX_NORMAL, "Kopalnia odblokuje sie za:^3 %dgodz %s%dmin %s%dsek",  ( upgradeCave / HOUR ), ( upgradeCave / MINUTE % MINUTE )<10?"0":"", ( upgradeCave / MINUTE % MINUTE ), (upgradeCave%MINUTE)<10?"0":"", ( upgradeCave %MINUTE ));
			userLastUpgradeTime[id] = playedTime(id);
		}
	}
	return PLUGIN_CONTINUE;
}
public miningMenu(id){
	new gText[556], iLen;
	new size = sizeof(gText)-iLen-1;
	iLen += format(gText[iLen], size, "\
					\dGobliny musisz oplacac codziennie aby Kopaly!^n\
					Wyczerpanie sie konczy nawet jak nie oplacisz Goblinow^n^n"
	);
	new newLeftMine  =  userBoostMine[id] - playedTime(id);
	new newStaminaLeft  =  userLastStaminaTime[id] + staminaTime + ( userUpgradeMine[id][up_STAMINA] * 720  ) - playedTime(id);
	if(newLeftMine > 0 )
	iLen += format(gText[iLen], size, "\yTwoj BOOST:\w %dgodz %s%dmin %s%dsek^n^n", ( newLeftMine / HOUR ), ( newLeftMine / MINUTE % MINUTE )<10?"0":"", ( newLeftMine / MINUTE % MINUTE ), (newLeftMine%MINUTE)<10?"0":"", ( newLeftMine %MINUTE ));
	
	if( newStaminaLeft > 0)
		iLen += format(gText[iLen], size, "\dTwoje wyczerpanie:\r %dgodz %s%dmin %s%dsek^n", ( newStaminaLeft / HOUR ), ( newStaminaLeft / MINUTE % MINUTE )<10?"0":"", ( newStaminaLeft / MINUTE % MINUTE ), (newStaminaLeft%MINUTE)<10?"0":"", ( newStaminaLeft %MINUTE ));
	else iLen += format(gText[iLen], size, "\dTwoje wyczerpanie:\r Gobliny poszly spac! O 24:00 beda mogly znow pracowac!^n");
	
	iLen += format(gText[iLen], size, "\dKopiesz Teraz:\r %s^n", mineOre[userTypeMine[id]][0]);
	iLen += format(gText[iLen], size, "\dRude wykopiesz za:\r %d/%d\w |\r (\y %0.1f%%\r )^n", userDigging[id][userTypeMine[id]], str_to_num(mineOre[userTypeMine[id]][1]), userDigging[id][userTypeMine[id]] * 100.0 / str_to_num(mineOre[userTypeMine[id]][1]));
	iLen += format(gText[iLen], size, "\dMoc kopania:\r %d^n", powerMine + ( userUpgradeMine[id][up_POWER] * 2) );
	
	if(newLeftMine > 0 )  iLen += format(gText[iLen], size, "\dSzybkosc kopania:\r %0.1f sek\y ( BOOST )^n", speedMine/boostAmount - float(userUpgradeMine[id][up_GOBLIN]));
	else iLen += format(gText[iLen], size, "\dSzybkosc kopania:\r %0.1f sek^n", speedMine - float(userUpgradeMine[id][up_GOBLIN]) );
	
	iLen += format(gText[iLen], size, "\dIlosc Goblinow:\r %d^n", userUpgradeMine[id][up_GOBLIN] + 1);
	
	iLen += format(gText[iLen], size, "\wWykopane Mineraly:\r %d/%d^n", userMineNugget[id],  mineralMax + ( userUpgradeMine[id][up_TRUCK] * truckMine));
	
	iLen += format(gText[iLen], size, "^n%s1.%s Sprzedaj Mineraly!^n", userMineNugget[id] == 0 ? "\d" : "\r", userMineNugget[id] == 0 ? "\d" : "\y");
	iLen += format(gText[iLen], size, "\r2.\y Ulepszenia!^n");
	
	if(userMinePayGoblin[id]) iLen += format(gText[iLen], size, "\r3.\y Oplac swoje Gobliny: %d Brylek!^n", 25 * (userUpgradeMine[id][up_GOBLIN] + 1 ));		
	else  iLen += format(gText[iLen], size, "\d3. Oplac swoje Gobliny: %d Brylek!^n",25 * (userUpgradeMine[id][up_GOBLIN] + 1 ));
	 
	iLen += format(gText[iLen], size, "^n\r0.\y Wyjdz!");
	 
	showMenu(id, B1 | B2 | B3 | B4 | B5 | B6 | B7 | B8 | B9 | B0, gText, -1, "miningMenu" );
}

public miningMenu_2(id, item){
	new newStaminaLeft  =  userLastStaminaTime[id] + staminaTime + ( userUpgradeMine[id][up_STAMINA] * 720 )  - playedTime(id);
	switch(item){
		
		case 0:{
			if(userMineNugget[id] == 0){
				chatPrint(id, PREFIX_NORMAL, "Nic nie wykopaly twoje Gobliny");
				return PLUGIN_CONTINUE;
			}
			if(newStaminaLeft < 0){
				chatPrint(id, PREFIX_NORMAL, "Twoje Gobliny teraz odpoczywaja! Jutro mozesz sprzedac Mineraly!");	
				return PLUGIN_CONTINUE;
			}
			chatPrint(id, PREFIX_NORMAL, "Sprzedales Mineraly!^4 |^1 Otrzymales:^3 %d Brylek", userMineNugget[id]);
			new value = userMineNugget[id];
			userNugget[id] += value;
			userNuggetAll[id] += value;
			userMineNugget[id] = 0;
			miningMenu(id);
		}
		case 1:miningUpgrade(id);
		case 2:{
			new cost = 25  * (userUpgradeMine[id][up_GOBLIN] + 1 );
			if(userNugget[id] < cost ){
				chatPrint(id, PREFIX_NORMAL, "Nie posiadasz Brylek na oplacenie Goblinow");
				miningMenu(id);
				return PLUGIN_CONTINUE;
			}
			if(userMinePayGoblin[id] ){
				userNugget[id] -= cost;
				userMinePayGoblin[id] = false;
				chatPrint(id, PREFIX_NORMAL, "Oplaciles swoje Gobliny!^4 |^1 Kosztowalo ciebie to:^3 %d Brylek!", cost);
				miningMenu(id);
				return PLUGIN_CONTINUE;
			}
			chatPrint(id, PREFIX_NORMAL, "Oplaciles juz swoje Gobliny");
			miningMenu(id);
		}
		case 3.. 8: miningMenu(id);
		default:{ return PLUGIN_HANDLED; }
	}
	return PLUGIN_CONTINUE;
}
public miningUpgrade(id){
	new menu = menu_create("\r[BaseBuilder]\y Ulepszenia Kopalnia:", "miningUpgrade_2");
	new gText[128];
	for(new i = 0;i < sizeof(upgradeMine); i++){
		format(gText, sizeof(gText), "\y%s\d [\r%d/%s\d]", upgradeMine[i][0],userUpgradeMine[id][i],  upgradeMine[i][2]);
		menu_additem(menu, gText);
		
	}
	menu_display(id, menu, 0);
	
}
public miningUpgrade_2(id,menu, item){
	if(item == MENU_EXIT){
		miningMenu(id);
		return PLUGIN_CONTINUE;
	}

	userVarMenu[id] = item;
	
	viewUpgradeMine(id, item);
	
	return PLUGIN_CONTINUE;	
}

public viewUpgradeMine(id, item){	
	new gText[556], iLen;
	new size = sizeof(gText)-iLen-1;
	new target = userVarMenu[id];
	
	iLen += format(gText[iLen], size, "\r[BaseBuilder]\y Kopalnia:^n");
	
	iLen += format(gText[iLen], size, "\dNazwa:\r %s^n", upgradeMine[target][0]);
	iLen += format(gText[iLen], size, "\dOpis:\r %s^n", upgradeMine[target][1]);
	iLen += format(gText[iLen], size, "\dCena:\r %s^n", upgradeMine[target][3]);
	iLen += format(gText[iLen], size, "\dUlepszenie:\r %d/%s^n",userUpgradeMine[id][target],  upgradeMine[target][2]);
	
	iLen += format(gText[iLen], size, "^n\r1.\y Ulepsz");
	iLen += format(gText[iLen], size, "^n\r2.\y Wroc");	
	
	showMenu(id, B1 | B2, gText, -1, "viewUpgradeMine" );
}
public viewUpgradeMine_2(id, item){
	new target = userVarMenu[id];
	switch(item){
		case 0:{
			
			if(userUpgradeMine[id][target] >= str_to_num(upgradeMine[target][2])){
				chatPrint(id, PREFIX_NORMAL, "Ulepszyles juz na maksymalny poziom^3 '%s'!", upgradeMine[target][0]);
				viewUpgradeMine(id, target);
				return PLUGIN_CONTINUE;
			}
			if(userNugget[id] < str_to_num(upgradeMine[target][3])){
				chatPrint(id, PREFIX_NORMAL, "Nie mozesz tego ulepszyc! Brakuje ci brylek!");
				viewUpgradeMine(id, target);
				return PLUGIN_CONTINUE;
			}
			
			userUpgradeMine[id][target] += 1;
			userNugget[id] -= str_to_num(upgradeMine[target][3]);
			chatPrint(id, PREFIX_NORMAL, "Ulepszyles^3 %s^1 na poziom:^3 %d/%s",  upgradeMine[target][0], userUpgradeMine[id][target],  upgradeMine[target][2]);
			new gText[128];
			logType[id] = LOG_CAVE;
			if(logType[id] == LOG_CAVE){
				format(gText, sizeof(gText), "ulepszyl [%s] na poziom [%d/%d] ",upgradeMine[target][0], userUpgradeMine[id][target],  upgradeMine[target][2]);
				logBB(id, gText);
			}

			viewUpgradeMine(id, target);
		}
		case 1: miningMenu(id);
		default:{}
	}
	return PLUGIN_CONTINUE;
}
public miningNugget(id){
	id -= TASK_MINER;
	
	if(unlockCave[id]){
		if(userMineNugget[id] < mineralMax + ( userUpgradeMine[id][up_TRUCK] * truckMine)){
			if(!userMinePayGoblin[id]){	
				new newStaminaLeft  =  userLastStaminaTime[id] + staminaTime + ( userUpgradeMine[id][up_STAMINA] * 720  ) - playedTime(id);
				if(newStaminaLeft > 0){
								
					removePointCave(id);
					
					userDigging[id][userTypeMine[id]] += powerMine + ( userUpgradeMine[id][up_POWER] * 2);
					userDigging[id][userTypeMine[id]] = clamp(userDigging[id][userTypeMine[id]], 0, str_to_num(mineOre[userTypeMine[id]][1]));
						
					if(str_to_num(mineOre[userTypeMine[id]][1]) == userDigging[id][userTypeMine[id]]){
						new value, bonusValue;
						switch(userTypeMine[id]){
							case mine_STONE: value = random_num(10,50);
							case mine_COAL: value = random_num(20,100);
							case mine_IRON: value = random_num(30,150);
							case mine_GOLD: value = random_num(40,200);
							case mine_DIAMOND: value = random_num(50,250);
							
						}
						if(userUpgradeMine[id][up_PICKAXE]){
							bonusValue += userUpgradeMine[id][up_PICKAXE] * random_num(1,3);
						}
						
						if( userUpgradeMine[id][up_LUCK]){
							if(random(100) <= userUpgradeMine[id][up_LUCK]){
								if( hasOption(userSaveOption[id], save_CAVE) ){
									chatPrint(id, PREFIX_NORMAL, "Znalazles bardzo obfita rude Mineralow^3 ( x4 )");
								}
								value *= 4;
							}
						}
						userMineNugget[id] += value + bonusValue;
						userMineNugget[id] = clamp(userMineNugget[id], 0, mineralMax + ( userUpgradeMine[id][up_TRUCK] * truckMine));
						
						if( hasOption(userSaveOption[id], save_CAVE) ){
							chatPrint(id, PREFIX_NORMAL, "Wykopales^3 %s^4 |^1 Mineralow:^3 %d", mineOre[userTypeMine[id]][0], value);
							if(bonusValue) chatPrint(id, PREFIX_NORMAL, "Otrzymales w bonusie Mineraly:^3 %d", bonusValue);
						}
						userDigging[id][userTypeMine[id]] = 0;
							
							
						new typeMine = random_num(0, userUpgradeMine[id][up_SEARCH]);
						userTypeMine[id] = typeMine;
					}
				}
			}
		}
	}
	new newLeftMine  =  userBoostMine[id] - playedTime(id);
	if(newLeftMine > 0 ) set_task(speedMine/boostAmount - float(userUpgradeMine[id][up_GOBLIN]), "miningNugget", id + TASK_MINER);
	else set_task(speedMine - float(userUpgradeMine[id][up_GOBLIN]), "miningNugget", id + TASK_MINER);
}
public removePointCave(id){
	if(random_float(0.00, 100.00) <= 0.05){
					
		new randomRemove = random(up_TOTAL);
					
		if(userUpgradeMine[id][randomRemove] <= 0)		
			return PLUGIN_CONTINUE;			
		
		
		if( hasOption(userSaveOption[id], save_CAVE) ){		
			switch(randomRemove){
				case 0: chatPrint(id, PREFIX_NORMAL, "Gobliny zlamaly kilof^4 |^3 ( -1poziom 'Kilof' )");
				case 1: chatPrint(id, PREFIX_NORMAL, "Goblin zlamal swoje lozko^4 |^3 ( -1poziom 'Wyczerpanie' )");
				case 2: chatPrint(id, PREFIX_NORMAL, "Goblin zgubil swoja lupe^4 |^3 ( -1poziom 'Szczescie' )");
				case 3:chatPrint(id, PREFIX_NORMAL, "Goblin spadl z wysokosci^4 |^3 ( -1poziom 'Goblin' )");	
				case 4:chatPrint(id, PREFIX_NORMAL, "Goblin ulamal kawalek kilofa^4 |^3 ( -1poziom 'Koncowka Kilofa' )");
				case 5:chatPrint(id, PREFIX_NORMAL, "Goblin zachorowal na skleroze i nie pamieta gdzie kopac^4 |^3 ( -1poziom 'Szukanie' )");
				case 6:chatPrint(id, PREFIX_NORMAL, "Goblin wywrocil wozek^4 |^3 ( -1poziom 'Wozek' )");	
			}
		}
		new gText[128];
		logType[id] = LOG_CAVE;
		if(logType[id] == LOG_CAVE){
			format(gText, sizeof(gText), "stracil [1 poziom] z ulepszenia [%s]",upgradeMine[randomRemove][0]);
			logBB(id, gText);
		}
	
		userUpgradeMine[id][randomRemove] --;
					
	}
	return PLUGIN_CONTINUE;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
