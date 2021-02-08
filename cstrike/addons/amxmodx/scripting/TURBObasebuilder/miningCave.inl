#include <amxmodx>
#include <amxmisc>
#include <cstrike>
#include <fun>
#include <hamsandwich>
#include <fakemeta>
#include <fakemeta_util>
#include <engine>


public globalMenuCave(id){	
	if(unlockCave[id]) miningMenu(id)	
	else buyCave(id)
	
}
public buyCave(id){
	new gText[556], iLen;
	new size = sizeof(gText)-iLen-1
	
	new newUpgradeLeft  =  userLastUpgradeTime[id] + upgradeCave - playedTime(id);
	iLen += format(gText[iLen], size, "\dAby odblokowac kopalnie trzeba najpierw ja kupic!^n")
	iLen += format(gText[iLen], size, "\dZakup trwa:\r %dgodz %s%dmin %s%dsek\d, po tym czasie^n", ( upgradeCave / HOUR ), ( upgradeCave / MINUTE % MINUTE )<10?"0":"", ( upgradeCave / MINUTE % MINUTE ), (upgradeCave%MINUTE)<10?"0":"", ( upgradeCave %MINUTE ))
	iLen += format(gText[iLen], size, "\dKopalnia jest przypisana do twojego konta i mozesz zaczac zarabiac!^n^n")
	
	if(newUpgradeLeft > 0) iLen += format(gText[iLen], size, "\dCzas zakupu:\r %dgodz %s%dmin %s%dsek^n",  ( newUpgradeLeft / HOUR ), ( newUpgradeLeft / MINUTE % MINUTE )<10?"0":"", ( newUpgradeLeft / MINUTE % MINUTE ), (newUpgradeLeft%MINUTE)<10?"0":"", ( newUpgradeLeft %MINUTE ))
	else if(startUpgrade[id] && newUpgradeLeft <= 0) iLen += format(gText[iLen], size, "\wKoplania zostala odblokowana, aby ja odebrac kliknij\r (1.)^n^n");
	
	iLen += format(gText[iLen], size, "\dWymagany Lv:\r %d^n",levelMiner)
	iLen += format(gText[iLen], size, "\dKoszt:\r %d Brylek^n", costMiner)
	
	if(startUpgrade[id] && newUpgradeLeft <= 0) 
		iLen += format(gText[iLen], size, "^n\r1.\y Odbierz pozwolenie")
	else if(newUpgradeLeft > 0 && startUpgrade[id])  
		iLen += format(gText[iLen], size, "^n\d1. Zablokowane [ KUPUJE ]")
	else if(userLevel[id] < levelMiner && userNugget[id] < costMiner)
		iLen += format(gText[iLen], size, "^n\d1. Zablokowane [ Brakuje %d lvl, %d brylek ]", levelMiner, costMiner)
	else if(userLevel[id] < levelMiner )
		iLen += format(gText[iLen], size, "^n\d1. Zablokowane [ Brakuje %d lvl ]", levelMiner)
	else if(userNugget[id] < costMiner )
		iLen += format(gText[iLen], size, "^n\d1. Zablokowane [ Brakuje %d brylek ]", costMiner)
	else
		iLen += format(gText[iLen], size, "^n\r1.\y Kup Kopalnie")
	
	iLen += format(gText[iLen], size, "^n\r2.\y Wroc")	
	
	show_menu(id, B1 | B2, gText, -1, "buyCave" )
}
public buyCave_2(id, item){
	new newUpgradeLeft  =  userLastUpgradeTime[id] + upgradeCave - playedTime(id);
	switch(item){
		case 0:{
			if(startUpgrade[id] && newUpgradeLeft <= 0){
				userLastStaminaTime[id] 	= playedTime(id)
				unlockCave[id] = 1;
				
				new gText[128];
				logType[id] = LOG_CAVE;
				if(logType[id] == LOG_CAVE) format(gText, sizeof(gText), "odblokowal kopalnie")
				logBB(id, gText)
				
				ColorChat(id, GREEN, "%s Odblokowales Kopalnie!", PREFIXSAY);
				return PLUGIN_CONTINUE;
			}
			else if(newUpgradeLeft > 0 && startUpgrade[id]){
				ColorChat(id, GREEN, "%s Teraz sie ulepsza!", PREFIXSAY);
				return PLUGIN_CONTINUE;
			}
			else if(userLevel[id] < levelMiner && userNugget[id] < costMiner){
				ColorChat(id, GREEN, "%s Nie mozesz tego ulepszyc!^x04 |^x03 Brakuje ci %d lvl oraz %d brylek!", PREFIXSAY, levelMiner, costMiner);
				return PLUGIN_CONTINUE;
			}
			else if(userLevel[id] < levelMiner){
				ColorChat(id, GREEN, "%s Nie mozesz tego ulepszyc!^x04 |^x03 Brakuje ci %d lvl!", PREFIXSAY, levelMiner);
				return PLUGIN_CONTINUE;
			}
			else if(userNugget[id] < costMiner){
				ColorChat(id, GREEN, "%s Nie mozesz tego ulepszyc!^x04 |^x03 Brakuje ci brylek!", PREFIXSAY);
				return PLUGIN_CONTINUE;
			}
			globalMenuCave(id)
			userNugget[id] -= costMiner;
			startUpgrade[id] = 1;
			ColorChat(id, GREEN, "%s Kopalnia odblokuje sie za:^x03 %dgodz %s%dmin %s%dsek", PREFIXSAY,  ( upgradeCave / HOUR ), ( upgradeCave / MINUTE % MINUTE )<10?"0":"", ( upgradeCave / MINUTE % MINUTE ), (upgradeCave%MINUTE)<10?"0":"", ( upgradeCave %MINUTE ));
			userLastUpgradeTime[id] = playedTime(id)
		}
	}
	return PLUGIN_CONTINUE;
}
public miningMenu(id){
	new gText[556], iLen;
	new size = sizeof(gText)-iLen-1
	iLen += format(gText[iLen], size, "\
					\dGobliny musisz oplacac codziennie aby Kopaly!^n\
					Wyczerpanie sie konczy nawet jak nie oplacisz Goblinow^n^n"
	);
	new newLeftMine  =  userBoostMine[id] - playedTime(id);
	new newStaminaLeft  =  userLastStaminaTime[id] + staminaTime + ( userUpgradeMine[id][up_STAMINA] * 720  ) - playedTime(id);
	if(newLeftMine > 0 )
	iLen += format(gText[iLen], size, "\yTwoj BOOST:\w %dgodz %s%dmin %s%dsek^n^n", ( newLeftMine / HOUR ), ( newLeftMine / MINUTE % MINUTE )<10?"0":"", ( newLeftMine / MINUTE % MINUTE ), (newLeftMine%MINUTE)<10?"0":"", ( newLeftMine %MINUTE ))
	
	if( newStaminaLeft > 0)
		iLen += format(gText[iLen], size, "\dTwoje wyczerpanie:\r %dgodz %s%dmin %s%dsek^n", ( newStaminaLeft / HOUR ), ( newStaminaLeft / MINUTE % MINUTE )<10?"0":"", ( newStaminaLeft / MINUTE % MINUTE ), (newStaminaLeft%MINUTE)<10?"0":"", ( newStaminaLeft %MINUTE ))
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
	 
	show_menu(id, B1 | B2 | B3 | B4 | B5 | B6 | B7 | B8 | B9 | B0, gText, -1, "miningMenu" )
}

public miningMenu_2(id, item){
	new newStaminaLeft  =  userLastStaminaTime[id] + staminaTime + ( userUpgradeMine[id][up_STAMINA] * 720 )  - playedTime(id);
	switch(item){
		
		case 0:{
			if(userMineNugget[id] == 0){
				ColorChat(id, GREEN, "%s Nic nie wykopaly twoje Gobliny", PREFIXSAY);
				return PLUGIN_CONTINUE;
			}
			if(newStaminaLeft < 0){
				ColorChat(id, GREEN, "%s Twoje Gobliny teraz odpoczywaja! Jutro mozesz sprzedac Mineraly!", PREFIXSAY);	
				return PLUGIN_CONTINUE;
			}
			ColorChat(id, GREEN, "%s Sprzedales Mineraly!^x04 |^x01 Otrzymales:^x03 %d Brylek", PREFIXSAY, userMineNugget[id]);
			new value = userMineNugget[id];
			userNugget[id] += value;
			userNuggetAll[id] += value
			userMineNugget[id] = 0;
			miningMenu(id)
		}
		case 1:{
			miningUpgrade(id);
		}
		case 2:{
			new cost = 25  * (userUpgradeMine[id][up_GOBLIN] + 1 )
			if(userNugget[id] < cost ){
				ColorChat(id, GREEN, "%s Nie posiadasz Brylek na oplacenie Goblinow", PREFIXSAY)
				miningMenu(id)
				return PLUGIN_CONTINUE;
			}
			if(userMinePayGoblin[id] ){
				userNugget[id] -= cost;
				userMinePayGoblin[id] = false
				ColorChat(id, GREEN, "%s Oplaciles swoje Gobliny!^x04 |^x01 Kosztowalo ciebie to:^x03 %d Brylek!", PREFIXSAY, cost)
				miningMenu(id)
				return PLUGIN_CONTINUE;
			}
			ColorChat(id, GREEN, "%s Oplaciles juz swoje Gobliny", PREFIXSAY)
			miningMenu(id)
		}
		case 3.. 8:{
			miningMenu(id)
		}
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
		miningMenu(id)
		return PLUGIN_CONTINUE;
	}

	userVarMenu[id] = item
	
	viewUpgradeMine(id, item)
	
	return PLUGIN_CONTINUE;	
}

public viewUpgradeMine(id, item){	
	new gText[556], iLen;
	new size = sizeof(gText)-iLen-1
	new target = userVarMenu[id]
	
	iLen += format(gText[iLen], size, "\r[BaseBuilder]\y Kopalnia:^n");
	
	iLen += format(gText[iLen], size, "\dNazwa:\r %s^n", upgradeMine[target][0]);
	iLen += format(gText[iLen], size, "\dOpis:\r %s^n", upgradeMine[target][1]);
	iLen += format(gText[iLen], size, "\dCena:\r %s^n", upgradeMine[target][3]);
	iLen += format(gText[iLen], size, "\dUlepszenie:\r %d/%s^n",userUpgradeMine[id][target],  upgradeMine[target][2]);
	
	iLen += format(gText[iLen], size, "^n\r1.\y Ulepsz")
	iLen += format(gText[iLen], size, "^n\r2.\y Wroc")	
	
	show_menu(id, B1 | B2, gText, -1, "viewUpgradeMine" )
}
public viewUpgradeMine_2(id, item){
	new target = userVarMenu[id]
	switch(item){
		case 0:{
			
			if(userUpgradeMine[id][target] >= str_to_num(upgradeMine[target][2])){
				ColorChat(id, GREEN, "%s Ulepszyles juz na maksymalny poziom^x03 '%s'!", PREFIXSAY, upgradeMine[target][0]);
				viewUpgradeMine(id, target)
				return PLUGIN_CONTINUE;
			}
			if(userNugget[id] < str_to_num(upgradeMine[target][3])){
				ColorChat(id, GREEN, "%s Nie mozesz tego ulepszyc! Brakuje ci brylek!", PREFIXSAY);
				viewUpgradeMine(id, target)
				return PLUGIN_CONTINUE;
			}
			
			userUpgradeMine[id][target] += 1;
			userNugget[id] -= str_to_num(upgradeMine[target][3])
			ColorChat(id, GREEN, "%s Ulepszyles^x03 %s^x01 na poziom:^x03 %d/%s", PREFIXSAY,  upgradeMine[target][0], userUpgradeMine[id][target],  upgradeMine[target][2]);
			new gText[128];
			logType[id] = LOG_CAVE;
			if(logType[id] == LOG_CAVE) format(gText, sizeof(gText), "ulepszyl [%s] na poziom [%d/%d] ",upgradeMine[target][0], userUpgradeMine[id][target],  upgradeMine[target][2])
			logBB(id, gText)

			viewUpgradeMine(id, target)
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
								
					removePointCave(id)
					
					userDigging[id][userTypeMine[id]] += powerMine + ( userUpgradeMine[id][up_POWER] * 2);
					userDigging[id][userTypeMine[id]] = clamp(userDigging[id][userTypeMine[id]], 0, str_to_num(mineOre[userTypeMine[id]][1]));
						
					if(str_to_num(mineOre[userTypeMine[id]][1]) == userDigging[id][userTypeMine[id]]){
						new value, bonusValue;
						switch(userTypeMine[id]){
							case mine_STONE:{
								value = random_num(10,50)
							}
							case mine_COAL:{
								value = random_num(20,100)
							}
							case mine_IRON:{
								value = random_num(30,150)
							}
							case mine_GOLD:{
								value = random_num(40,200)
							}
							case mine_DIAMOND:{
								value = random_num(50,250)	
							}
						}
						if(userUpgradeMine[id][up_PICKAXE]){
							bonusValue += userUpgradeMine[id][up_PICKAXE] * random_num(1,3);
						}
						
						if( userUpgradeMine[id][up_LUCK]){
							if(random(100) <= userUpgradeMine[id][up_LUCK]){
								if( hasOption(userSaveOption[id], save_CAVE) ){
									ColorChat(id, GREEN, "%s Znalazles bardzo obfita rude Mineralow^x03 ( x4 )", PREFIXSAY);
								}
								value *= 4;
							}
						}
						userMineNugget[id] += value + bonusValue
						userMineNugget[id] = clamp(userMineNugget[id], 0, mineralMax + ( userUpgradeMine[id][up_TRUCK] * truckMine));
						
						if( hasOption(userSaveOption[id], save_CAVE) ){
							ColorChat(id, GREEN, "%s Wykopales^x03 %s^x04 |^x01 Mineralow:^x03 %d", PREFIXSAY, mineOre[userTypeMine[id]][0], value);
							if(bonusValue) ColorChat(id, GREEN, "%s Otrzymales w bonusie Mineraly:^x03 %d", PREFIXSAY, bonusValue);
						}
						userDigging[id][userTypeMine[id]] = 0;
							
							
						new typeMine = random_num(0, userUpgradeMine[id][up_SEARCH])
						userTypeMine[id] = typeMine
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
				case 0:{
					ColorChat(id, GREEN, "%s Gobliny zlamaly kilof^x04 |^x03 ( -1poziom 'Kilof' )", PREFIXSAY);
				}
				case 1:{
					ColorChat(id, GREEN, "%s Goblin zlamal swoje lozko^x04 |^x03 ( -1poziom 'Wyczerpanie' )", PREFIXSAY);
				}
				case 2:{
					ColorChat(id, GREEN, "%s Goblin zgubil swoja lupe^x04 |^x03 ( -1poziom 'Szczescie' )", PREFIXSAY);
				}
				case 3:{
					ColorChat(id, GREEN, "%s Goblin spadl z wysokosci^x04 |^x03 ( -1poziom 'Goblin' )", PREFIXSAY);	
				}
				case 4:{
					ColorChat(id, GREEN, "%s Goblin ulamal kawalek kilofa^x04 |^x03 ( -1poziom 'Koncowka Kilofa' )", PREFIXSAY);
				}
				case 5:{
					ColorChat(id, GREEN, "%s Goblin zachorowal na skleroze i nie pamieta gdzie kopac^x04 |^x03 ( -1poziom 'Szukanie' )", PREFIXSAY);
				}
				case 6:{
					ColorChat(id, GREEN, "%s Goblin wywrocil wozek^x04 |^x03 ( -1poziom 'Wozek' )", PREFIXSAY);	
				}
							
			}
		}
		new gText[128];
		logType[id] = LOG_CAVE;
		if(logType[id] == LOG_CAVE){
			format(gText, sizeof(gText), "stracil [1 poziom] z ulepszenia [%s]",upgradeMine[randomRemove][0])
			logBB(id, gText)
		}
	
		userUpgradeMine[id][randomRemove] --
					
	}
	return PLUGIN_CONTINUE;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
