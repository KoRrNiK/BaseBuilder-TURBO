  
#if defined _bbTurbo_included
	#endinput
#endif

#define _bbTurbo_included

/**
	 * Returns player classes zombie.
	 *
	 * @param id		Player index
	 *
	 * @return		Player class zombie
**/
native bb_get_class_zombie(id);

/**
	 * Returns player classes human.
	 *
	 * @param id		Player index
	 *
	 * @return		Player class human
**/
native bb_get_class_human(id);

/**
	 * Set final ( counter ) exp.
	 *
	 * @param id		Player index
	 * @param value		Exp amount
	 *
	 * @return		Player exp
**/
native Float:bb_set_final_exp(id, Float:value);

/**
	 * Set ( no counter ) exp.
	 *
	 * @param id		Player index
	 * @param value		Exp amount
	 *
	 * @return		Player exp
**/
native Float:bb_set_exp(id, Float:value);

/**
	 * Set final ( counter ) nugget.
	 *
	 * @param id		Player index
	 * @param value		Nugget amount
	 *
	 * @return		Player nugget
**/
native bb_set_final_nugget(id, value);

/**
	* Set ( no counter ) nugget.
	*
	* @param id		Player index
	* @param value		Nugget amount
	*
	* @return		Player nugget
**/
native bb_set_nugget(id, value);

/**
	* Set final ( counter ) bone.
	*
	* @param id		Player index
	* @param value		Bone amount
	*
	* @return		Player bone
**/
native bb_set_final_bone(id, value);

/**
	* Set ( no counter ) bone.
	*
	* @param id		Player index
	* @param value		Bone amount
	*
	* @return		Player bone
*/
native bb_set_bone(id, value);

/**
	 * Returns player nugget.
	 *
	 * @param id        	Player index
	 *
	 * @return          	Player nugget
**/
native bb_get_nugget(id);

/**
	 * Returns player bone.
	 *
	 * @param id        	Player index
	 *
	 * @return          	Player bone
**/
native bb_get_bone(id);

/**
	 * Returns round preparation
	 *
	 * @return		Round type preparation
**/
native bb_prep();

/**
	 * Returns round build
	 *
	 * @return		Round type build
**/
native bb_build();

/**
	 * Returns round game
	 *
	 * @return		Round type game
**/
native bb_game();

/**
	 * Returns player luzaczki ( sms value ).
	 *
	 * @param id        	Player index
	 *
	 * @return          	Player luzaczki
**/
native bb_get_luzaczki(id);

/**
	 * Set luzaczki.
	 *
	 * @param id        	Player index
	 * @param value     	Luzaczki amount
	 *
	 * @return          	Player luzaczki
**/
native bb_set_luzaczki(id, value);

/**
	 * Returns player user id ( sql id ).
	 *
	 * @param id        	Player index
	 *
	 * @return          	Player id
**/
native bb_get_user_id(id);

/**
	 * Returns player logged.
	 *
	 * @param id        	Player index
	 *
	 * @return          	Player logged
**/
native bb_get_login(id);

/**
	 * Multiforward called on player spawn.
	 *
	 * @param id        	Player index
	 * @param first	    	Spawn player
	 *			firts == true  ( first spawn on map )
	 *			fisrt == false ( normal spawn no first )
	 *
	 * @noreturn
**/
forward bb_spawned(id, first);

/**
	 * Multiforward called on new round event.
	 *
	 * @noreturn
**/
forward bb_new_round();

/**
	 * Multiforward called on round start event.
	 *
	 * @noreturn
**/
forward bb_start_round();

/**
	 * Multiforward called on round restart event.
	 *
	 * @noreturn
**/
forward bb_restart_round();

/**
	 * Multiforward called on round end event.
	 *
	 * @noreturn
**/
forward bb_end_round();

/**
	 * Multiforward called on end game round.
	 *
	 * @noreturn
**/
forward bb_end_game();

/**
	* Multiforward called on start game.
	*
	* @noreturn
**/
forward bb_start_game();

/**
	* Multiforward called on start preparation.
	*
	* @noreturn
**/
forward bb_start_prep();

/**
	* Multiforward called on start build.
	*
	* @noreturn
**/
forward bb_start_build();

/**
	* Multiforward called on start release.
	*
	* @noreturn
**/
forward bb_start_release();
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
