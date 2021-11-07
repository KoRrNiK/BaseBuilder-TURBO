#include <amxmodx>

enum TutorColor { TUTOR_RED = 1, TUTOR_BLUE, TUTOR_YELLOW, TUTOR_GREEN };

new const gTutorPrecache[][] = { 
	"gfx/career/icon_!.tga",
	"gfx/career/icon_!-bigger.tga",
	"gfx/career/icon_i.tga",
	"gfx/career/icon_i-bigger.tga", 
	"gfx/career/icon_skulls.tga",
	"gfx/career/round_corner_ne.tga",
	"gfx/career/round_corner_nw.tga",
	"gfx/career/round_corner_se.tga",
	"gfx/career/round_corner_sw.tga",
	"resource/TutorScheme.res",
	"resource/UI/TutorTextWindow.res" 
};
stock tutorPrecache(){
	for(new i = 0; i < sizeof(gTutorPrecache); i++)  precache_generic(gTutorPrecache[i]);
}
stock tutorMake(id, TutorColor:ColorSelect, Float:fTime = 0.0, const szText[], any:...){
	
	if(!is_user_connected(id) || is_user_bot(id) || is_user_hltv(id)) return; 
	
	static szMessage[192];
	vformat(szMessage, sizeof(szMessage) - 1, szText, 5);
	
	static msgTutorText;
	if (!msgTutorText) msgTutorText = get_user_msgid("TutorText");
	
	if(!id) message_begin(MSG_ALL, msgTutorText);
	else message_begin(MSG_ONE_UNRELIABLE, msgTutorText, .player = id);
	write_string( szMessage );
	write_byte( 0 );
	write_short( 0 );
	write_short( 0 );
	write_short( 1 << _:ColorSelect );
	message_end( );

	if(fTime != 0.0){
		if(!id){
			for(new i = 1; i < maxPlayers; i++){
				if(!is_user_connected(i) || is_user_bot(i) || is_user_hltv(i)) continue; 
				
				remove_task(i+TASK_TUTOR);
				set_task(fTime,"tutorClose",i + TASK_TUTOR);	
			}
		}else{
			remove_task(id+TASK_TUTOR);
			set_task(fTime,"tutorClose",id + TASK_TUTOR);
		}
	}
}
public tutorClose(id){
	
	id -= TASK_TUTOR;
	
	if(!is_user_connected(id) || is_user_bot(id) || is_user_hltv(id)) return; 
	
	static msgTutorClose;
	if (!msgTutorClose) msgTutorClose = get_user_msgid("TutorClose");
	
	if(!id) message_begin( MSG_ALL, msgTutorClose);
	else message_begin( MSG_ONE_UNRELIABLE, msgTutorClose, .player = id);
	
	message_end();
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
