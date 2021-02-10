#include <amxmodx>
#include <fakemeta_util>
#include <fun>
#include <bbTurbo>
new gCvarColor;
new gScreenfade
new gTrail;
new gGlass;
new gExplotion;
new bool:bEnable = true;

new const gTaskFrostnade = 3256
new const gModelGlass[] = "models/glassgibs.mdl"
new const gModelTrail[] = "sprites/lgtning.spr"
new const gModelExplotion[] = "sprites/shockwave.spr"
new const gSoundWave[] = "warcraft3/frostnova.wav";
new const gSoundFrosted[] = "warcraft3/impalehit.wav";
new const gSoundBreak[] = "warcraft3/impalelaunch1.wav";

new bool:gIsFrosted[33];
new bool:gRestartAttempt[33];

new iColors[3];

enum{class_CLASSIC, class_SPEED, class_FAT, class_TANK, class_DRACULA, class_SNOWMAN,  class_DEVIL, class_HEALTH, class_POISON, class_DEATH, class_TERMINATOR, class_DEMON, class_TOTAL}

public plugin_init() 
{
	register_plugin("FrostNade", "1.2", "author")
	
	gCvarColor = register_cvar("hns_fn_color", "0 128 255")
	gScreenfade = get_user_msgid("ScreenFade")
	
	GetColor(iColors[0], iColors[1], iColors[2])
	
	register_logevent("logeventRoundEnd", 2, "1=Round_End");
	register_event("TextMsg", "event_RestartAttempt", "a", "2=#Game_will_restart_in");
	register_event("ResetHUD", "event_ResetHud", "be");
	register_event("DeathMsg","event_DeathMsg","a");
	register_event( "HLTV", "eventRoundStart", "a", "1=0", "2=0" );
	
	register_forward(FM_PlayerPreThink,"fwd_PlayerPreThink");
	register_forward(FM_SetModel, "fwd_SetModel");
	
	register_forward(FM_TraceLine, 				"fw_TraceLine", 			1)
	register_forward(FM_TraceHull, 				"fw_TraceHull", 			1)
	
	
}
public plugin_natives(){
	register_native("bb_set_in_ice", "native_set_in_ice", 1)
	register_native("bb_is_frozen", "native_is_frozen", 1)
	register_native("bb_explode_origin", "ExplodeFrostByOirigin", 1)
}	

public bool:native_is_frozen(id){
	return gIsFrosted[id]
}
public native_set_in_ice(id,victim, duration){
	if( !gIsFrosted[victim]){
		new origin[3]
		get_user_origin(victim, origin,0)
		message_begin(MSG_PVS, SVC_TEMPENTITY, origin, 0)
		write_byte(TE_DLIGHT)			
		write_coord(origin[0]); // start X
		write_coord(origin[1]); // start Y
		write_coord(origin[2]); // start Z			
		write_byte(25)	
	
		
		write_byte(iColors[0]); // red
		write_byte(iColors[1]); // green
		write_byte(iColors[2]); // blue
		
		write_byte(10)			
		write_byte(50)			
		message_end()
		
		fm_set_rendering(victim, kRenderFxGlowShell, iColors[0], iColors[1], iColors[2], kRenderNormal,25)
		engfunc(EngFunc_EmitSound, victim, CHAN_WEAPON, gSoundFrosted, 1.0, ATTN_NORM, 0, PITCH_NORM)
				
		message_begin(MSG_ONE, gScreenfade, _, victim);
		write_short(~0); // duration
		write_short(~0); // hold time
		write_short(0x0004); // flags: FFADE_STAYOUT
		write_byte(iColors[0]); // red
		write_byte(iColors[1]); // green
		write_byte(iColors[2]); // blue
		write_byte(150); // alpha
		message_end();
				
				
		if(pev(victim, pev_flags) & FL_ONGROUND)
			set_pev(victim, pev_gravity, 999999.9) 
		
		else
			set_pev(victim, pev_gravity, 0.000001) 
		gIsFrosted[victim] = true;
		
		
		set_task(float(duration), "RemoveFrost", victim)
		
		
	}
}
public plugin_precache()
{
	gTrail = precache_model(gModelTrail)
	gExplotion = precache_model(gModelExplotion)
	gGlass = precache_model(gModelGlass)
	
	precache_sound(gSoundWave)
	precache_sound(gSoundFrosted)
	precache_sound(gSoundBreak)
}

public eventRoundStart()
	set_task(2.0,"taskEnable");

public taskEnable()
	bEnable = true;

public logeventRoundEnd()
	bEnable = false;

public event_RestartAttempt()
{
	new players[32], num;
	get_players(players, num, "a");
	
	for (new i; i < num; ++i)
		gRestartAttempt[players[i]] = true;
}

public event_ResetHud(id)
{
	if (gRestartAttempt[id])
	{
		gRestartAttempt[id] = false;
		return;
	}
	event_PlayerSpawn(id);
}

public event_PlayerSpawn(id)
{
	if(gIsFrosted[id]) 
		RemoveFrost(id);
}

public event_DeathMsg()
{
	new id = read_data(2);
	
	if(gIsFrosted[id])
		RemoveFrost(id)
}

public fwd_PlayerPreThink(id)
{
	if(gIsFrosted[id]){
		set_pev(id, pev_velocity, Float:{0.0,0.0,0.0})		
		set_pev(id, pev_flags, pev(id, pev_flags) | FL_FROZEN); 
	}
}

public fwd_SetModel(entity, const model[])
{
	static id
	id = pev(entity, pev_owner);
	
	if (!is_user_connected(id))
		return;
	
	if(equal(model,"models/w_smokegrenade.mdl"))
	{
		fm_set_rendering(entity,kRenderFxGlowShell, iColors[0], iColors[1], iColors[2], kRenderNormal, 16);
		
		message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
		write_byte(TE_BEAMFOLLOW);
		write_short(entity);	// entity
		write_short(gTrail);	// sprite
		write_byte(20);		// life
		write_byte(10);		// width
		write_byte(iColors[0]); // red
		write_byte(iColors[1]); // green
		write_byte(iColors[2]); // blue
		write_byte(255);	// brightness
		message_end();
		
		set_pev(entity, pev_nextthink, get_gametime() + 10.0);
		
		static args[2]
		args[0] = entity;
		args[1] = id;
		
		set_task(1.5, "ExplodeFrost", gTaskFrostnade, args, sizeof args)
	}
}
public ExplodeFrostByOirigin(ent){
		
	new Float:fOrigin[3]
	pev(ent, pev_origin, fOrigin)
	new iOrigin[3]
	iOrigin[0]	=	floatround(fOrigin[0])
	iOrigin[1]	=	floatround(fOrigin[1])
	iOrigin[2]	=	floatround(fOrigin[2])
	
	CreateBlast(iOrigin);
	
	engfunc(EngFunc_EmitSound, ent, CHAN_WEAPON, gSoundWave, 1.0, ATTN_NORM, 0, PITCH_NORM)
	
	static victim
	victim = -1;
	
	while((victim = engfunc(EngFunc_FindEntityInSphere, victim, fOrigin, 240.0)) != 0)
	{
		
		if(!is_user_alive(victim) || gIsFrosted[victim]  )
			continue;
		
		if( get_user_team(victim) != 1 )
			continue			
		
		fm_set_rendering(victim, kRenderFxGlowShell, iColors[0], iColors[1], iColors[2], kRenderNormal,25)
		engfunc(EngFunc_EmitSound, victim, CHAN_WEAPON, gSoundFrosted, 1.0, ATTN_NORM, 0, PITCH_NORM)
		
		message_begin(MSG_ONE, gScreenfade, _, victim);
		write_short(~0); // duration
		write_short(~0); // hold time
		write_short(0x0004); // flags: FFADE_STAYOUT
		write_byte(iColors[0]); // red
		write_byte(iColors[1]); // green
		write_byte(iColors[2]); // blue
		write_byte(150); // alpha
		message_end();
		
		
		if(pev(victim, pev_flags) & FL_ONGROUND)
			set_pev(victim, pev_gravity, 999999.9) 
		
		else
			set_pev(victim, pev_gravity, 0.000001) 
		gIsFrosted[victim] = true;	
		set_task(6.5, "RemoveFrost", victim)
	}
		
}
public ExplodeFrost(const args[2])
{ 	
	if(bEnable)
	{
		static ent
		ent = args[0]
		
		new id = args[1];
		
		if (!pev_valid(ent)) 
			return;
		
		static origin[3], Float:originF[3]
		pev(ent, pev_origin, originF);
		FVecIVec(originF, origin);
		
		CreateBlast(origin);
		
		engfunc(EngFunc_EmitSound, ent, CHAN_WEAPON, gSoundWave, 1.0, ATTN_NORM, 0, PITCH_NORM)
		
		static victim
		victim = -1;
		new zamrozonych = 0;
		
		while((victim = engfunc(EngFunc_FindEntityInSphere, victim, originF, 240.0)) != 0)
		{
			
			if(!is_user_alive(victim) || gIsFrosted[victim]  )
				continue;
			
			if( id == victim )
				continue
			
			
			if( bb_get_class_zombie(victim) == class_SNOWMAN )
				continue
			
			if( get_user_team(id) == get_user_team(victim) )
				continue			
			if( get_user_godmode(victim) )
				continue
			fm_set_rendering(victim, kRenderFxGlowShell, iColors[0], iColors[1], iColors[2], kRenderNormal,25)
			engfunc(EngFunc_EmitSound, victim, CHAN_WEAPON, gSoundFrosted, 1.0, ATTN_NORM, 0, PITCH_NORM)
			
			message_begin(MSG_ONE, gScreenfade, _, victim);
			write_short(~0); // duration
			write_short(~0); // hold time
			write_short(0x0004); // flags: FFADE_STAYOUT
			write_byte(iColors[0]); // red
			write_byte(iColors[1]); // green
			write_byte(iColors[2]); // blue
			write_byte(150); // alpha
			message_end();
			
			zamrozonych ++
			if(pev(victim, pev_flags) & FL_ONGROUND)
				set_pev(victim, pev_gravity, 999999.9) 
			
			else
				set_pev(victim, pev_gravity, 0.000001) 
			gIsFrosted[victim] = true;	
			set_task(6.5, "RemoveFrost", victim)
		}
		engfunc(EngFunc_RemoveEntity, ent)
	}
}

public RemoveFrost(id){
	if(!gIsFrosted[id])
		return;
	
	gIsFrosted[id] = false;
	set_pev(id, pev_flags, pev(id, pev_flags) & ~FL_FROZEN);
	set_pev(id, pev_gravity, 1.0)
	engfunc(EngFunc_EmitSound, id, CHAN_VOICE, gSoundBreak, 1.0, ATTN_NORM, 0, PITCH_NORM)
	
	fm_set_rendering(id)
	
	message_begin(MSG_ONE, gScreenfade, _, id);
	write_short(0); // duration
	write_short(0); // hold time
	write_short(0); // flags
	write_byte(0); // red
	write_byte(0); // green
	write_byte(0); // blue
	write_byte(0); // alpha
	message_end();
	
	static origin[3], Float:originF[3]
	pev(id, pev_origin, originF)
	FVecIVec(originF, origin)
	
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte(TE_BREAKMODEL);
	write_coord(origin[0]);		// x
	write_coord(origin[1]);		// y
	write_coord(origin[2] + 24);	// z
	write_coord(16);		// size x
	write_coord(16);		// size y
	write_coord(16);		// size z
	write_coord(random_num(-50,50));// velocity x
	write_coord(random_num(-50,50));// velocity y
	write_coord(25);		// velocity z
	write_byte(10);			// random velocity
	write_short(gGlass);		// model
	write_byte(10);			// count
	write_byte(25);			// life
	write_byte(0x01);		// flags: BREAK_GLASS
	message_end();
	set_pev( id, pev_gravity, 1.0 )
}

CreateBlast(const origin[3])
{
	
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte(TE_BEAMCYLINDER);
	write_coord(origin[0]); // start X
	write_coord(origin[1]); // start Y
	write_coord(origin[2]); // start Z
	write_coord(origin[0]); // something X
	write_coord(origin[1]); // something Y
	write_coord(origin[2] + 385); // something Z
	write_short(gExplotion); // sprite
	write_byte(0); // startframe
	write_byte(0); // framerate
	write_byte(4); // life
	write_byte(60); // width
	write_byte(0); // noise
	write_byte(iColors[0]); // red
	write_byte(iColors[1]); // green
	write_byte(iColors[2]); // blue
	write_byte(200); // brightness
	write_byte(0); // speed
	message_end();
	
	
	// medium ring
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte(TE_BEAMCYLINDER);
	write_coord(origin[0]); // start X
	write_coord(origin[1]); // start Y
	write_coord(origin[2]); // start Z
	write_coord(origin[0]); // something X
	write_coord(origin[1]); // something Y
	write_coord(origin[2] + 470); // something Z
	write_short(gExplotion); // sprite
	write_byte(0); // startframe
	write_byte(0); // framerate
	write_byte(4); // life
	write_byte(60); // width
	write_byte(0); // noise
	write_byte(iColors[0]); // red
	write_byte(iColors[1]); // green
	write_byte(iColors[2]); // blue
	write_byte(200); // brightness
	write_byte(0); // speed
	message_end();
	
	// largest ring
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte(TE_BEAMCYLINDER);
	write_coord(origin[0]); // start X
	write_coord(origin[1]); // start Y
	write_coord(origin[2]); // start Z
	write_coord(origin[0]); // something X
	write_coord(origin[1]); // something Y
	write_coord(origin[2] + 555); // something Z
	write_short(gExplotion); // sprite
	write_byte(0); // startframe
	write_byte(0); // framerate
	write_byte(4); // life
	write_byte(60); // width
	write_byte(0); // noise
	write_byte(iColors[0]); // red
	write_byte(iColors[1]); // green
	write_byte(iColors[2]); // blue
	write_byte(200); // brightness
	write_byte(0); // speed
	message_end();
	
	message_begin(MSG_PVS, SVC_TEMPENTITY, origin, 0)
	write_byte(TE_DLIGHT)			
	write_coord(origin[0]); // start X
	write_coord(origin[1]); // start Y
	write_coord(origin[2]); // start Z			
	write_byte(25)	

	
	write_byte(iColors[0]); // red
	write_byte(iColors[1]); // green
	write_byte(iColors[2]); // blue
	
	write_byte(10)			
	write_byte(50)			
	message_end()
}


public fw_TraceLine(Float:start[3], Float:end[3], conditions, id, trace) {
	return traceShot(id,trace)
}
public fw_TraceHull(Float:start[3], Float:end[3], conditions, hull, id, trace){
	return traceShot(id, trace)
}

traceShot(id, trace){

	
	if(!is_user_alive(id ))
		return FMRES_IGNORED;

	new target = get_tr2(trace, TR_pHit)
		
	if (!is_user_alive(target) ||  !gIsFrosted[target]) return FMRES_IGNORED
		
	new Float:origin[3], Float:angles[3]
	engfunc(EngFunc_GetBonePosition, target, 8, origin, angles)
	set_tr2(trace, TR_vecEndPos, origin)
	set_tr2(trace, TR_iHitgroup, HIT_HEAD)
	
	return FMRES_IGNORED
}

GetColor(&r, &g, &b)
{
	new Color[16], Red[4], Green[4], Blue[4];
	get_pcvar_string(gCvarColor, Color, 15)
	parse(Color, Red, 3, Green, 3, Blue, 3)
	
	r = str_to_num(Red)
	g = str_to_num(Green)
	b = str_to_num(Blue)
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
