#include <	amxmodx		>
#include <	amxmisc		>
#include <	fakemeta	>
#include <	fakemeta_util	>
#include <	engine		>

public createBarHp(id){	
	if( userBarHp[id] != 0 ) return 0;
	
	new ent = create_entity("info_target");
		

	if( !pev_valid(ent) ) return 0;
	
	set_pev(ent, pev_classname, "spriteBarHp");
	set_pev(ent, pev_frame, 0.0);
	set_pev(ent, pev_movetype, MOVETYPE_NOCLIP);
	set_pev(ent, pev_solid, SOLID_NOT);
	set_pev(ent, pev_iuser1, id);
	
	entity_set_model(ent, BARHP);	
	set_pev(ent, pev_scale, 0.2);
	set_pev(ent, pev_fuser1, 23.0);
	
	
	entity_set_int(ent, EV_INT_rendermode, 5);
	entity_set_float(ent, EV_FL_renderamt, 255.0);
	DispatchSpawn(ent);
	userBarHp[id] = ent;
	return ent;
}

public removeBarHp(id){
	new ent = userBarHp[id];
	
	if( ent == 0 ) return 0;
	if( !pev_valid(ent) ) return 0;
		
	remove_entity(ent);
	userBarHp[id]=0;
	
	return 1;
}

public moveBarHp(id){
	
	new ent = userBarHp[id];
	
	if( !pev_valid(ent) ) return 0;
		
	new Float:fOrigin[3];
	new Float:fSize[3];
	pev(id, pev_origin, fOrigin);
	pev(id, pev_maxs, fSize);
	fOrigin[2]+=floatabs(fSize[2])+10.0;
	set_pev(ent, pev_origin, fOrigin);
	
	new Float:percent=float(get_user_health(id))/float(userMaxHealth[id]);
	new Float:frame= pev(ent, pev_fuser1)-(pev(ent, pev_fuser1)*percent);
	set_pev(ent, pev_frame, frame);
	
	return 1;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
