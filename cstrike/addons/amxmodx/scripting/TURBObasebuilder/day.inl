#include <	amxmodx		>
#include <	amxmisc		>
#include <	cstrike		>
#include <	fun		>
#include <	fakemeta	>
#include <	fakemeta_util	>
#include <	engine		>
#include <	csx		>
#include <	fvault		>
#include <	http2		>
#include <	xs		>

public data(id){
	new day;
	date(_,_,day);
	
	if(userLastDay[id] != day){
		userNugget[id] += 10;
		chatPrint(id, PREFIX_NONE, "Otrzymales darmowe 10 Brylek");
		userLastDay[id] = day;
	} else {
		chatPrint(id, PREFIX_NONE, "Odebrales juz swoje darmowe brylki!");	
	}
	return PLUGIN_CONTINUE;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
