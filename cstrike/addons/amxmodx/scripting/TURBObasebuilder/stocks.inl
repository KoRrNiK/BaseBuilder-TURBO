#include <amxmodx>
#include <amxmisc>
#include <fakemeta>
#include <hamsandwich>	

stock cmd_execute(id, const text[], any:...){
	
	#pragma unused text

	new message[192];
	format_args(message, charsmax(message), 1);
	message_begin(id == 0 ? MSG_ALL : MSG_ONE, SVC_DIRECTOR, _, id);
	write_byte(strlen(message) + 2); write_byte(10); write_string(message);
	message_end();
}
stock Display_Fade(id,duration,holdtime,fadetype,red,green,blue,alpha){
	if (!is_user_alive(id)) return;

	static msgScreenFade;

	if (!msgScreenFade) msgScreenFade = get_user_msgid("ScreenFade");

	message_begin(MSG_ONE, msgScreenFade, {0, 0, 0}, id);
	write_short(duration);
	write_short(holdtime);
	write_short(fadetype);
	write_byte(red);
	write_byte(green);
	write_byte(blue);
	write_byte(alpha);
	message_end();
}


public barTime(id, timeBonus){
	if (!is_user_alive(id)) return;

	static msgBarTime;
	
	if (!msgBarTime) msgBarTime = get_user_msgid("BarTime");
	
	message_begin(MSG_ONE_UNRELIABLE, msgBarTime, .player=id);
	write_short(timeBonus);
	message_end();
}

stock bool:is_hull_vacant(const Float:origin[3], hull,id) {
	static tr;
	engfunc(EngFunc_TraceHull, origin, origin, 0, hull, id, tr);
	
	if (!get_tr2(tr, TR_StartSolid) || !get_tr2(tr, TR_AllSolid))
		return true;
	return false;
}
stock BeamCylinder(Float:fOrigin[3], Float:height, sprite, startframe, framerate,life,width,noise,red,green,blue,bright,speed){
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte(TE_BEAMCYLINDER);
	engfunc(EngFunc_WriteCoord,fOrigin[0]);
	engfunc(EngFunc_WriteCoord,fOrigin[1]);
	engfunc(EngFunc_WriteCoord,fOrigin[2]);
	engfunc(EngFunc_WriteCoord,fOrigin[0]);
	engfunc(EngFunc_WriteCoord,fOrigin[1]);
	engfunc(EngFunc_WriteCoord,fOrigin[2]+height);	
	write_short(sprite); // sprite
	write_byte(startframe); // startframe
	write_byte(framerate); // framerate
	write_byte(life); // life
	write_byte(width); // width
	write_byte(noise); // noise
	write_byte(red); // red
	write_byte(green); // green
	write_byte(blue); // blue
	write_byte(bright); // brightness
	write_byte(speed); // speed
	message_end();
}
stock BeamLight(Float:fOriginStart[3], Float:fOriginEnd[3], sprite, framestart, framerate, life, width, noise, r, g, b, bright, scroll){
	message_begin(MSG_BROADCAST,SVC_TEMPENTITY) ;
	write_byte(TE_BEAMPOINTS);
	engfunc(EngFunc_WriteCoord,fOriginStart[0]);
	engfunc(EngFunc_WriteCoord,fOriginStart[1]);
	engfunc(EngFunc_WriteCoord,fOriginStart[2]);
	engfunc(EngFunc_WriteCoord,fOriginEnd[0]);
	engfunc(EngFunc_WriteCoord,fOriginEnd[1]);
	engfunc(EngFunc_WriteCoord,fOriginEnd[2]);
	write_short(sprite);
	write_byte(framestart);
	write_byte(framerate);
	write_byte(life) ;
	write_byte(width); 
	write_byte(noise);
	write_byte(r);
	write_byte(g);
	write_byte(b);
	write_byte(bright);
	write_byte(scroll);
	message_end();
}
stock Create_TE_PLAYERATTACHMENT(id, entity, vOffset, iSprite, life){

	if(!id) message_begin(MSG_ALL, SVC_TEMPENTITY);
	else message_begin(MSG_ONE_UNRELIABLE, SVC_TEMPENTITY, _, id);
	
	write_byte(TE_PLAYERATTACHMENT);
	write_byte(entity);			// entity
	write_coord(vOffset);			// vertical offset ( attachment origin.z = player origin.z + vertical offset )
	write_short(iSprite);			// model index
	write_short(life);			// (life * 10 )
	message_end();
}

public takeWeapon(id, weaponName[]){
	if(!equal(weaponName, "weapon_", 7))  return 0;
		
	new weaponId = get_weaponid(weaponName);
	if(!weaponId)  return 0;
		
	new ent;
	while((ent = engfunc(EngFunc_FindEntityByString,ent,"classname", weaponName)) && pev(ent, pev_owner) != id) {}
	if(!weaponId)  return 0;
	
	if(get_user_weapon(id) == weaponId)  ExecuteHamB(Ham_Weapon_RetireWeapon, ent);
	
	if(!ExecuteHamB(Ham_RemovePlayerItem, id, ent))  return 0;
	ExecuteHamB(Ham_Item_Kill, ent);
	
	set_pev(id, pev_weapons, pev(id, pev_weapons) & ~(1<<weaponId));
	return 1;
}

public drawLine(id, Float:fOriginStart[3], Float:fOriginEnd[3],red, green, blue, life, width, noise){
	message_begin(MSG_ONE,SVC_TEMPENTITY, _, id) ;
	write_byte(TE_BEAMPOINTS);
	engfunc(EngFunc_WriteCoord,fOriginStart[0]);
	engfunc(EngFunc_WriteCoord,fOriginStart[1]);
	engfunc(EngFunc_WriteCoord,fOriginStart[2]);
	engfunc(EngFunc_WriteCoord,fOriginEnd[0]);
	engfunc(EngFunc_WriteCoord,fOriginEnd[1]);
	engfunc(EngFunc_WriteCoord,fOriginEnd[2]);
	write_short(sprite_bluez);
	write_byte(0);
	write_byte(25);
	write_byte(life);
	write_byte(width);
	write_byte(noise);
	write_byte(red);
	write_byte(green);
	write_byte(blue);
	write_byte(255);
	write_byte(255);
	message_end();
}
stock showBeam( fOriginStart[3], fOriginEnd[3], sprite, life, width,noise, red,green,blue)
{
	message_begin( MSG_BROADCAST, SVC_TEMPENTITY );
	write_byte( TE_BEAMPOINTS );
	write_coord( fOriginStart[ 0 ] );
	write_coord( fOriginStart[ 1 ] );
	write_coord( fOriginStart[ 2 ] );
	write_coord( fOriginEnd[ 0 ] );
	write_coord( fOriginEnd[ 1 ] );
	write_coord( fOriginEnd[ 2 ] );
	write_short( sprite );
	write_byte( 1 );
	write_byte( 1 );
	write_byte( life );
	write_byte( width);
	write_byte( noise );
	write_byte( red );
	write_byte( green );
	write_byte( blue );
	write_byte( 200 );
	write_byte( 0 );
	message_end();
}

public refreshStats(id){
	if( !is_user_connected(id) ) return;

	message_begin(MSG_ALL, get_user_msgid("ScoreInfo"));
	write_byte(id);
	write_short(get_user_frags(id));
	write_short(cs_get_user_deaths(id));
	write_short(0);
	write_short(get_user_team(id));
	message_end();
}

stock makeLight(Float:fOrigin[3], radius, r,g,b,life, bright){
	engfunc(EngFunc_MessageBegin, MSG_PVS, SVC_TEMPENTITY, fOrigin, 0);
	write_byte(TE_DLIGHT);
	engfunc(EngFunc_WriteCoord,fOrigin[0]);
	engfunc(EngFunc_WriteCoord,fOrigin[1]);
	engfunc(EngFunc_WriteCoord,fOrigin[2]);					
	write_byte(radius);	
	write_byte(r);			//r
	write_byte(g);			//g
	write_byte(b);			//b				
	write_byte(life);		//life		
	write_byte(bright);		//
	message_end();
}
stock makePou(fOrigin[3], sprite){
	message_begin(MSG_BROADCAST,SVC_TEMPENTITY, fOrigin, 0);
	write_byte (TE_SPRITETRAIL);		
	write_coord(fOrigin[0] -  random(20));
	write_coord(fOrigin[1]);
	write_coord(fOrigin[2] - random_num(10,70));
	write_coord(fOrigin[0]);
	write_coord(fOrigin[1]);
	write_coord(fOrigin[2] + random(20));
	write_short(sprite);
	write_byte(random_num(5,15)) ;
	write_byte(random_num(1,5));
	write_byte(random_num(1,2));
	write_byte(random_num(15,30));
	write_byte(random(10));
	message_end();
}
public laserMaker(id, Float:fOrigin[3], sprite, startframe, framerate ,life, size, noise,r , g, b, brightness){

	message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte (TE_BEAMENTPOINT);
	write_short(id | 0x1000);
	engfunc(EngFunc_WriteCoord, fOrigin[0]);
	engfunc(EngFunc_WriteCoord, fOrigin[1]);
	engfunc(EngFunc_WriteCoord, fOrigin[2]);
	write_short(sprite);
	write_byte(startframe); // startframe
	write_byte(framerate); // framerate 
	write_byte(life); // life
	write_byte(size); // wielkoœæ
	write_byte(noise); // noise
	write_byte(r);     // Czerwony
	write_byte(g);  // Zielony
	write_byte(b); // Niebieski
	write_byte(brightness); // brightness
	write_byte(0); // speed
	message_end();
}

stock Particles(Float:fOrigin[3], Float:randomm, sprite, count, life, scale, velocity, randompart){
	message_begin(MSG_BROADCAST,SVC_TEMPENTITY);
	write_byte (15);	// line of moving glow sprites with gravity, fadeout, and collisions			
	engfunc(EngFunc_WriteCoord,fOrigin[0]);
	engfunc(EngFunc_WriteCoord,fOrigin[1]);		
	engfunc(EngFunc_WriteCoord,fOrigin[2] - random_float(-randomm, randomm));
	engfunc(EngFunc_WriteCoord,fOrigin[0]);			
	engfunc(EngFunc_WriteCoord,fOrigin[1]);
	engfunc(EngFunc_WriteCoord,fOrigin[2] + random_float(-randomm, randomm));
	// +random_float(-random, random)
	write_short(sprite); // (sprite index)
	write_byte(count); 
	write_byte(life); // (life in 0.1's) 
	write_byte(scale); // byte (scale in 0.1's) 
	write_byte(velocity); // (velocity along vector in 10's)
	write_byte(randompart) ;// (randomness of velocity in 10's)
	message_end();
}

stock makeTrail(id, sprite, life, widht, r, g, b, bright){	
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte(22);	// TE_BEAMFOLLOW
	write_short(id);
	write_short(sprite);
	write_byte(life);
	write_byte(widht);
	write_byte(r);
	write_byte(g);
	write_byte(b);
	write_byte(bright);
	message_end();

}
showMenu(id, keys, const menu[], time_show = 0, const tittle[], vis = 0){
	
	new gText[1536], iLen = 0;
	
	if(vis) iLen += format(gText[iLen], sizeof(gText)-iLen-1, formatm("%s^n", symbolsCustom[SYMBOL_BB]));
	iLen += format(gText[iLen], sizeof(gText)-iLen-1, "%s", menu);
	
	show_menu(id, keys, gText, time_show, tittle );
}
stock Float:distanceToFloor(Float:start[3], ignoremonsters = 1){
	new Float:dest[3], Float:end[3];
	dest[0] = start[0];
	dest[1] = start[1];
	dest[2] = -8191.0;
	engfunc(EngFunc_TraceLine, start, dest, ignoremonsters, 0, 0);
	get_tr2(0, TR_vecEndPos, end);
	new Float:ret = start[2] - end[2];
	return ret > 0 ? ret : 0.0;
}
stock screenShake(id, amplitude, duration, frequency){
	if (!is_user_alive(id)) return;

	static msgScreenShake;

	if (!msgScreenShake) msgScreenShake = get_user_msgid("ScreenShake");
	
	message_begin(MSG_ONE, msgScreenShake, {0, 0, 0}, id);
	write_short(amplitude << 14);
	write_short(duration << 14);
	write_short(frequency << 14);
	message_end();
}

stock setFov(id, value){
	if (!is_user_alive(id)) return;

	static msgSetFov;

	if (!msgSetFov) msgSetFov = get_user_msgid("SetFOV");
	
	message_begin( MSG_ONE, msgSetFov, { 0, 0, 0 }, id );
	write_byte( value );
	message_end( ) ;
}
stock createBarText(gText[], iLen, const symbol[], coolDown, seconds=10){
	new iAmt = max(1, coolDown), timeAdd;
	while(((iAmt-=seconds)>=(1>>seconds)) && add(gText, iLen, symbol));
		timeAdd++;
	return timeAdd;
}
stock setGlow(id, r,g,b,d){
	if (!is_user_alive(id)) return;
	
	set_rendering(id, kRenderFxGlowShell, 	r,g,b,	kRenderNormal, 	d);
}
public removeGlow(id){
	if (!is_user_alive(id)) return;
	
	setGlow(id,0,0,0,0);
}
stock bool:hasOption(var, option){
	return !!(var&(1<<option));
}
stock addOption(&var, option){
	var |= (1<<option);
}
stock removeOption(&var, option){
	var &= ~(1<<option);
}
stock formatNumber(num){
	
	
	new value[16][5];
	new szVal[32];
	new szNum[32];
		
	
	num_to_str(num >= 0 ? num : abs(num),	szVal,sizeof(szVal));
	
	new iLen = strlen(szVal);
	
	switch(iLen){
		
		case 1,2,3:  format(szNum, sizeof(szNum), "%s%s", num < 0 ? "-" : "", szVal);
		case 4,5,6:{
			copy(value[2],	iLen - 3, 	szVal);
			copy(value[1],	3, 		szVal[iLen - 3]);
			format(szNum, 	sizeof(szNum), "%s%s.%s",  num < 0 ? "-" : "",value[2], value[1]);
		}
		case 7,8,9:{
			copy(value[3],	iLen - 6, 	szVal);
			copy(value[2],	3, 		szVal[iLen - 6]);
			copy(value[1],	3, 		szVal[iLen - 3]);
			format(szNum, 	sizeof(szNum), "%s%s.%s.%s", num < 0 ? "-" : "",value[3], value[2], value[1]);	
		}
		case 10,11,12:{
			copy(value[4],	iLen - 9, 	szVal);
			copy(value[3],	3, 		szVal[iLen - 9]);
			copy(value[2],	3, 		szVal[iLen - 6]);
			copy(value[1],	3, 		szVal[iLen - 3]);
			format(szNum, 	sizeof(szNum), "%s%s.%s.%s.%s",  num < 0 ? "-" : "",value[4], value[3], value[2], value[1]);
		}
		default: format(szNum, sizeof(szNum), "%s%s", num < 0 ? "-" : "", szVal);
			
	}
	return szNum;
}
stock skillBarMenu(gText[], iLen, type, amount, const symbolOne[], const symbolTwo[]){
	new line = 0;
	for(new i = 0; i < type; i++) line += format(gText[line], iLen - line - 1, "\r%s\d", symbolOne);
	for(new i = 0; i < amount-type; i++)  line += format(gText[line], iLen - line - 1, "%s", symbolTwo);	
}
stock infoTabRemove(id){
	if (!is_user_alive(id)) return;

	static msgScore;
	
	if (!msgScore) msgScore = get_user_msgid("ScoreAttrib");
	
	message_begin(MSG_ALL, msgScore);
	write_byte(id);
	write_byte(0);
	message_end();
}
stock formatm(const format[], any:...){
	static gText[256];
	vformat(gText, sizeof(gText) -1 , format, 2);
	return gText;
}

stock bool:isOX(){
	new szMap[20];
	get_mapname(szMap, sizeof(szMap) );
	if(containi(szMap, "ox") != -1) return true;
	return false;
}


enum _:chatPREFIX{ PREFIX_NONE, PREFIX_NORMAL, PREFIX_LINE }
	
stock chatPrint(id, prefix,  const text[], any:...){
	new message[192];

	if (numargs() == 3) copy(message, sizeof(message) - 1, text);
	else vformat(message, sizeof(message) - 1, text, 4);

	if(prefix == PREFIX_NORMAL) client_print_color(id, id, "^4%s^1 %s", PREFIXSAY_NORMAL, message);
	else if(prefix == PREFIX_LINE) client_print_color(id, id, "^4%s^1 %s^4 %s", PREFIXSAY_LINE, message, PREFIXSAY_LINE);
	else client_print_color(id, id, "%s", message);
}

enum /* Weapon types */
{
	Primary = 1
	, Secondary
	, Knife
	, Grenades
	, C4
};

stock StripWeapons(id, type, bool:switchIfActive = true){
	if (is_user_alive(id)) {
		new ent, weapon;
		while ((weapon = get_weapon_from_slot(id, type, ent)) > 0) ham_strip_user_weapon(id, weapon, type, switchIfActive);
	}
}
stock get_weapon_from_slot(id, slot, &ent){
	
	if (!(1 <= slot <= 5)) return 0;

	ent = get_pdata_cbase(id, OFFSET_ITEM_SLOT + slot , OFFSET_LINUX_PLAYER);

	return (ent > 0) ? get_pdata_int(ent, OFFSET_ID , OFFSET_LINUX_WEAPONS) : 0;
}
stock ham_strip_user_weapon(id, weaponId, slot = 0, bool:switchIfActive = true){
	static const weaponsSlots[] = { -1, 2, -1, 1, 4, 1, 5, 1, 1, 4, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 4, 2, 1, 1, 3, 1 };

	if (!slot) slot = weaponsSlots[weaponId];

	new weapon = get_pdata_cbase(id, OFFSET_ITEM_SLOT + slot, OFFSET_LINUX_PLAYER);

	while (weapon > 0) {
		if (get_pdata_int(weapon, OFFSET_ID, OFFSET_LINUX_WEAPONS) == weaponId) break;

		weapon = get_pdata_cbase(weapon, OFFSET_NEXT, OFFSET_LINUX_WEAPONS);
	}

	if (weapon > 0) {
		if (switchIfActive && get_pdata_cbase(id, OFFSET_ACTIVE_ITEM, OFFSET_LINUX_PLAYER) == weapon) ExecuteHamB(Ham_Weapon_RetireWeapon, weapon);

		if (ExecuteHamB(Ham_RemovePlayerItem, id, weapon)) {
			user_has_weapon(id, weaponId, 0);

			ExecuteHamB(Ham_Item_Kill, weapon);

			return 1;
		}
	}

	return 0;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
