#define maxPlayers 27

#include <amxmodx>
#include <amxmisc>
#include <fakemeta>
#include <engine>
#include <fakemeta_util>
#include <fun>
#include <hamsandwich>
#include <bbTurbo>

#define TASK_GRANADE 555

new const PLUGIN[] 		= 	"Push Granade"
new const VERSION[] 		= 	"v1.1"
new const AUTHOR[] 		= 	"KoRrNiK" 	

new const colorGranade[] 		= 	{ 255, 255, 10 }

new const Float:powerGranade 	= 	900.0

new const granadeSprite[]	=	"sprites/basebuildervt/laserbeam.spr";
new const granadeWeapon[]	=	"models/w_flashbang.mdl";
new const soundGranade[]		=	"basebuildervt/push.wav";
new spriteBeam;

public plugin_init() {
	register_plugin(PLUGIN, VERSION, AUTHOR)
	register_forward(FM_SetModel, "fw_SetModel")
}

public plugin_precache(){
	spriteBeam = precache_model( granadeSprite )
	precache_sound(soundGranade);
}
public fw_SetModel(ent, const model[]){
	
	static Float:dmgtime;
	pev(ent, pev_dmgtime, dmgtime)
	
	if(dmgtime == 0.0) return FMRES_IGNORED;
	
	if(!equal(model, granadeWeapon) ) return FMRES_IGNORED;
	
	static id;
	id = pev(ent, pev_owner);	

	set_pev(ent, pev_dmgtime, get_gametime() + 999.0)	
		
	set_rendering(ent, kRenderFxGlowShell, 	colorGranade[0], colorGranade[1], colorGranade[2], kRenderNormal, 	5)
	makeTrail(ent,spriteBeam, colorGranade[0], colorGranade[1], colorGranade[2])
		
	static tempId[2];
	tempId[0] = ent;
	tempId[1] = id;
		
	set_task(1.5, "explodeGranade", TASK_GRANADE, tempId, sizeof(tempId));
	
	return FMRES_IGNORED;
}
public explodeGranade(const tempId[2]){
	
	new ent = tempId[0]
	new owner = tempId[1];
	
	if (!pev_valid(ent))  return;
	
	new Float:fOrigin[3] = 0.0
	new Float:fOriginTarget[3] = 0.0
	new Float:fVelocity[3] = 0.0;
	new szModel[33];
	
	pev(ent, pev_model, szModel, sizeof(szModel))
	pev(ent, pev_origin, fOrigin);
	
	emit_sound(ent, CHAN_WEAPON, soundGranade, 1.0, ATTN_NORM, 0, PITCH_NORM)
	
	for(new i = 0 ; i < 3;  i ++) BeamCylinder(fOrigin, i == 0 ? 300.0 : i == 1 ? 470.0 : 550.0 , spriteBeam, 0, 0, 4, 60, 0, colorGranade[0], colorGranade[1], colorGranade[2],200,0)
	makeLight(fOrigin, 45, colorGranade[0], colorGranade[1], colorGranade[2],30, 255)	
	
	for(new i = 1; i < maxPlayers; i ++){
			
		if(!is_user_alive(i)) continue;
		if( owner == i ) continue
		if( get_user_team(owner) == get_user_team(i) ) continue			
		if( get_user_godmode(i)) continue
	
		pev(i, pev_origin, fOriginTarget);
		
		if( get_distance_f(fOriginTarget, fOrigin) >= 210.0) continue;
		
		fOriginTarget[2] = fOrigin[2]
		xs_vec_sub(fOrigin, fOriginTarget, fVelocity)
		xs_vec_normalize( fVelocity , fVelocity );	
			
		xs_vec_mul_scalar( fVelocity , -powerGranade , fVelocity );
		fVelocity[2] *= 1.5;
		entity_set_vector(i, EV_VEC_velocity, fVelocity)
		DisplayFade(i, 2048, 2048, 2048, colorGranade[0], colorGranade[1], colorGranade[2], 200)	
	}
	remove_entity(ent)
}
stock BeamCylinder(Float:fOrigin[3], Float:height, sprite, startframe, framerate,life,width,noise,red,green,blue,bright,speed){
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte(TE_BEAMCYLINDER);
	engfunc(EngFunc_WriteCoord,fOrigin[0])
	engfunc(EngFunc_WriteCoord,fOrigin[1])
	engfunc(EngFunc_WriteCoord,fOrigin[2])	
	engfunc(EngFunc_WriteCoord,fOrigin[0])
	engfunc(EngFunc_WriteCoord,fOrigin[1])
	engfunc(EngFunc_WriteCoord,fOrigin[2]+height)	
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
stock makeLight(Float:fOrigin[3], radius, r,g,b,life, bright){
	engfunc(EngFunc_MessageBegin, MSG_PVS, SVC_TEMPENTITY, fOrigin, 0)
	write_byte(TE_DLIGHT)	
	engfunc(EngFunc_WriteCoord,fOrigin[0])
	engfunc(EngFunc_WriteCoord,fOrigin[1])
	engfunc(EngFunc_WriteCoord,fOrigin[2]+40.0)					
	write_byte(radius)	
	write_byte(r)			//r
	write_byte(g)			//g
	write_byte(b)			//b				
	write_byte(life)		//life		
	write_byte(bright)		//
	message_end()
}
stock makeTrail(entity, sprite, r, g, b){
	message_begin(MSG_BROADCAST, SVC_TEMPENTITY) 
	write_byte(22) 
	write_short(entity) 
	write_short(sprite) 
	write_byte(45) 
	write_byte(4) 
	write_byte(r) 
	write_byte(g) 
	write_byte(b) 
	write_byte(64)
	message_end() 
}
stock DisplayFade(id,duration,holdtime,fadetype,red,green,blue,alpha){
	if (!is_user_alive(id)) return;

	static msgScreenFade;
	if (!msgScreenFade) msgScreenFade = get_user_msgid("ScreenFade");
	
	message_begin(MSG_ONE, msgScreenFade, {0, 0, 0}, id);
	write_short(duration); write_short(holdtime); write_short(fadetype); write_byte(red); write_byte(green); write_byte(blue); write_byte(alpha);
	message_end();
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
