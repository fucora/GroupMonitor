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

//�����static,��ʾ���loggerֻ�б�cpp��Ч
static Logger logger("Ⱥ�ؼ��ʼ��");


EVE_Enable(Enable)
{
	//��ȡӦ������Ŀ¼
	AppFile = CQ::getAppDirectory();


	//dll_cq_fun();
	//��ȡ��Ҫ����
	Config_Read_main_dll read_main_ini(AppFile);
	read_main_ini.read_main();
	cfg_main = read_main_ini.value;

	//��ȡȫ����������
	Config_Read_dll read_ini(AppFile);
	read_ini.read_Alone();
	//����map
	for (auto new_map : read_ini.value)
	{
		cfg_alone[new_map.first] = new_map.second;
	}

	logger.Info("Ⱥ��ر�����");

	return 0;
}

EVE_Disable(Disable)
{
	//д����Ҫ����
	Config_Write_main_dll write_main(cfg_main, AppFile);
	write_main.write_main();

	//д��ȫ����������
	Config_Write_dll write_ini(cfg_alone, AppFile);
	write_ini.write_Alone();

	logger.Info("Ⱥ��ر�ͣ��");
	return 0;
}

MUST_AppInfo_RETURN(CQAPPID)