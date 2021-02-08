public soundPrecache(){
	for ( new i = 0; i < sizeof(soundsGame); i ++ ){
		precache_sound(soundsGame[i])
	}
	
	for ( new i = 0; i < sizeof(zombieSound); i ++ ){
		precache_sound(zombieSound[i])
	}
	
	for ( new i = 0; i < sizeof(bonusSound); i ++ ){
		precache_sound(bonusSound[i])
	}
	
	
		
	
	
}
public emitGameSound(id, sound){
	emit_sound(id, CHAN_WEAPON, soundsGame[sound], 1.0, ATTN_NORM, 0, PITCH_NORM)
}


public emitBonusSound(id, sound){
	emit_sound(id, CHAN_WEAPON, bonusSound[sound], 1.0, ATTN_NORM, 0, PITCH_NORM)
}

public spkGameSound(id, sound){
	cmd_execute(id, "spk %s", soundsGame[sound])
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
