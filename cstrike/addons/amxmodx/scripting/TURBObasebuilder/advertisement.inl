#include <amxmodx>
#include <amxmisc>
 
new Array:advertisements;

public advertisementLoad(){
	
	advertisements = ArrayCreate(196, 1);

	new filePath[128];

	get_localinfo("amxx_configsdir", filePath, sizeof(filePath) - 1);
	format(filePath, sizeof(filePath) - 1, "%s/bb_advertisement.ini", filePath);

	if (!file_exists(filePath)) {
		new error[128];
		format(error, sizeof(error) - 1, "=== Nie znaleziono pliku: %s ===", filePath);
		set_fail_state(error);
	}

	new advertisement[196], content[196], file = fopen(filePath, "r");

	while (!feof(file)) {
		
		fgets(file, content, sizeof(content) - 1);
		trim(content);

		if (content[0] == ';' || !content[0] || content[0] == ' ') continue;

		parse(content,
			advertisement, sizeof(advertisement) - 1
		);
		
		ArrayPushString(advertisements, advertisement);
		
	}
	fclose(file);
	
	log_amx("=== Zaladowano reklamy (%d) ===", ArraySize(advertisements));

}

public adsInfo(id){
	
	id -= TASK_ADVERTISMENT;
	
	new advertisement[196];
	new size = ArraySize(advertisements);
	
	if(!size){
		if(task_exists(id + TASK_ADVERTISMENT))
			remove_task(id + TASK_ADVERTISMENT);
		return;
	}
	
	ArrayGetString(advertisements, random(size), advertisement, sizeof(advertisement) - 1);
	
	new globalTime[9], globalDate[12], map[33], name[33]; 
	get_time("%H:%M:%S",globalTime,sizeof(globalTime) - 1);
	get_time("%Y-%m-%d",globalDate,sizeof(globalDate) - 1);
	get_mapname(map, sizeof(map) -1);
	get_user_name(id, name, sizeof(name) - 1 );
	
	new online = numPlayers(USER_ALL, false);

	
	replace_all(advertisement,sizeof(advertisement) - 1,"[green]",	"^4");
	replace_all(advertisement,sizeof(advertisement) - 1,"[team]",	"^3");
	replace_all(advertisement,sizeof(advertisement) - 1,"[normal]",	"^1");
	replace_all(advertisement,sizeof(advertisement) - 1,"[you]",	name);
	replace_all(advertisement,sizeof(advertisement) - 1,"[time]",	globalTime);
	replace_all(advertisement,sizeof(advertisement) - 1,"[date]",	globalDate);
	replace_all(advertisement,sizeof(advertisement) - 1,"[map]",	map);
	replace_all(advertisement,sizeof(advertisement) - 1,"[online]",	formatm("%d", online));
	
	format(advertisement, sizeof(advertisement) - 1, "%s", advertisement);
	
	chatPrint(0, PREFIX_NONE, advertisement);
	
	set_task(bbCvar[cvarAdsTime], "adsInfo", id + TASK_ADVERTISMENT);
}

/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1045\\ f0\\ fs16 \n\\ par }
*/
