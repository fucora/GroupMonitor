#include <CQEVE.h>
#include <CQLogger.h>
#include <CQAPI_EX.h>
#include "APPINFO.h"

#include "fun.h"
#include "Configini.h"

using namespace std;
using namespace CQ;


extern map<long long, CFG_ALONE_DLL_ALL> cfg_alone;
extern CFG_MAIN_DLL cfg_main;
extern string AppFile;

//extern void dll_cq_fun();

//请加上static,表示这个logger只有本cpp有效
static Logger logger("群关键词监控");


EVE_Enable(Enable)
{
	//获取应用数据目录
	AppFile = CQ::getAppDirectory();


	//dll_cq_fun();
	//读取主要配置
	Config_Read_main_dll read_main_ini(AppFile);
	read_main_ini.read_main();
	cfg_main = read_main_ini.value;

	//读取全部单独配置
	Config_Read_dll read_ini(AppFile);
	read_ini.read_Alone();
	//拷贝map
	for (auto new_map : read_ini.value)
	{
		cfg_alone[new_map.first] = new_map.second;
	}

	logger.Info("群监控被启用");

	return 0;
}

EVE_Disable(Disable)
{
	//写入主要配置
	Config_Write_main_dll write_main(cfg_main, AppFile);
	write_main.write_main();

	//写入全部单独配置
	Config_Write_dll write_ini(cfg_alone, AppFile);
	write_ini.write_Alone();

	logger.Info("群监控被停用");
	return 0;
}

MUST_AppInfo_RETURN(CQAPPID)