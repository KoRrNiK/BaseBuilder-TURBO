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
	
	new month;
	date(_,month,_);
	
	if(userLastDay[id] != month){
		new ran = random(100) * ( userLevel[id] == 0 ? userLevel[id]+1 :userLevel[id] + 1 ) * 2;
		userNugget[id] += ran;
		chatPrint(id, PREFIX_LINE, "Otrzymales darmowe %s Brylek", formatNumber(ran));
		userLastDay[id] = month;
	
	} else chatPrint(id, PREFIX_LINE, "Odebrales juz swoje darmowe brylki!");	
	
	chatPrint(id, PREFIX_NONE, "^3Nastepna nagroda w kolejnym miesiacu");	
	
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
