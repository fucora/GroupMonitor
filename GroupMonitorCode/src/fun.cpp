#include <CQEVE.h>
#include <CQAPI.h>
#include <CQAPI_EX.h>
#include <CQLogger.h>

//���ݽṹ
#include <vector>
#include <map>
#include <string>

//ϵͳ
#include <Windows.h>
#include <direct.h>
#include <time.h>
#include <thread>
#include <regex>

#include "fun.h"

using namespace std;
static CQ::Logger logger("Ⱥ�ؼ��ʼ��");

extern string replace_all_distinct(string& str, string old_value, string new_value);

string AppFile;
map<long long, CFG_ALONE_DLL_ALL> cfg_alone;
CFG_MAIN_DLL cfg_main;

////��ȡ��Q api����
//void dll_cq_fun()
//{
//	dll_cq.getGroupList = CQ::getGroupList;
//}

//д������
bool Write_ini()
{
	//д����Ҫ����
	Config_Write_main_dll write_main(cfg_main, AppFile);
	if (!write_main.write_main())
	{
		logger.Info(("������д��ʧ��" + AppFile).c_str());
		return false;
	}


	//д��ȫ����������
	Config_Write_dll write_ini(cfg_alone, AppFile);
	if (!write_ini.write_Alone())
	{
		logger.Info(("������д��ʧ��" + AppFile).c_str());
		return false;
	}
	return true;
}

//��ȡ����
bool Read_ini()
{
	//��ȡ��Ҫ����
	Config_Read_main_dll read_main_ini(AppFile);
	if (!read_main_ini.read_main())
	{
		logger.Info(("�������ȡʧ��" + AppFile).c_str());
		return false;
	}

	cfg_main = read_main_ini.value;

	//��ȡȫ����������
	Config_Read_dll read_ini(AppFile);
	if (!read_ini.read_Alone())
	{
		logger.Info(("�������ȡʧ��" + AppFile).c_str());
		return false;
	}

	//����map
	for (auto new_map : read_ini.value)
	{
		cfg_alone[new_map.first] = new_map.second;
	}
	return true;
}

//д�벢��ȡ����
bool ReInit()
{
	if (!Write_ini())
	{
		return false;
	}
	if (!Read_ini())
	{
		return false;
	}

	return true;
}


/*˽��ָ��*/

//˽����Ϣ����
void Private::funthing(const char* msg)
{
	std::vector<int64_t>::iterator iter;
	iter = find(cfg_main.root.begin(), cfg_main.root.end(), fromQQ);

	if (iter == cfg_main.root.end())
	{
		//��������ʱ����Ϣת��������
		MsgRelayFun(msg);
	}
	else
	{
		//Ϊ����ʱ����ָ���
		instruct(msg);
	}
}

//��Ϣת��
void Private::MsgRelayFun(const char* msg)
{
	if (cfg_main.MsgRelay)
	{
		std::string SendMsg;

		//��ȡQQ����
		CQ::StrangerInfo QQInf = CQ::getStrangerInfo(fromQQ);

		//������Ϣ
		SendMsg = "����" + QQInf.nick + "(" + std::to_string(fromQQ) + ")��˽����Ϣ:\n\n";
		SendMsg += msg;
		SendMsg += "\n\n(�ظ��뷢�� �ظ�" + std::to_string(fromQQ) + ")";

		//���͸�����
		for (long long root : cfg_main.root)
		{
			CQ::sendPrivateMsg(root, SendMsg);
		}
	}
}

//ָ��Ի�
void Private::funcute(const char* msg)
{
	switch (x)
	{
	case 5:
	{
		ReplyGroup = atoll(msg);
		CQ::sendPrivateMsg(fromQQ, "�뷢�ͻظ�����");
		x = 6;
		break;
	}
	case 6:
	{
		char ch[100] = { '\0' };
		int t;
		t = CQ::sendGroupMsg(ReplyGroup, msg);
		if (t > 0)
			sprintf(ch, "���͸�QQȺ%lld�ɹ�", ReplyGroup);
		else
			sprintf(ch, "���͸�QQȺ%lldʧ��\n������룺%d", ReplyGroup, t);
		CQ::sendPrivateMsg(fromQQ, ch);
		ReplyGroup = 0;
		x = 0;
		break;
	}
	case 7:						//��ӹؼ���
	{
		WKEYWORD Temp_add;
		Temp_add.Word = msg;
		cfg_alone[0].WKeyWord.push_back(Temp_add);
		ReInit();

		CQ::sendPrivateMsg(fromQQ, "���Ĭ�Ϲؼ��ʳɹ�");

		x = 0;
		break;
	}
	case 8:						//ɾ���ؼ���
	{
		bool del_bool = false;

		auto it = cfg_alone[0].WKeyWord.begin();

		while (it != cfg_alone[0].WKeyWord.end())
		{
			if (it->Word == msg)
			{
				auto del = it;

				it = cfg_alone[0].WKeyWord.erase(del);
				del_bool = true;
			}
			else
			{
				it++;
			}

		}

		ReInit();

		if (del_bool)
			CQ::sendPrivateMsg(fromQQ, "ɾ��Ĭ�Ϲؼ��ʳɹ�");
		else
			CQ::sendPrivateMsg(fromQQ, "ɾ��Ĭ�Ϲؼ���ʧ��");

		x = 0;
		break;
	}
	case 11:					//��Ӱ�����
	{
		cfg_alone[0].WhiteList.push_back(atoll(msg));
		ReInit();

		CQ::sendPrivateMsg(fromQQ, "���Ĭ�ϰ������ɹ�");

		x = 0;
		break;
	}
	case 12:					//ɾ��������
	{
		bool del_bool = false;

		long long del_QQId = 0;
		del_QQId = atoll(msg);

		if (del_QQId == 0)
		{
			CQ::sendPrivateMsg(fromQQ, "��ʽ����");
			x = 0;
			break;
		}

		auto it = cfg_alone[0].WhiteList.begin();

		while (it != cfg_alone[0].WhiteList.end())
		{
			if (*it == del_QQId)
			{
				auto del = it;

				it = cfg_alone[0].WhiteList.erase(del);
				del_bool = true;
			}
			else
			{
				it++;
			}

		}

		ReInit();

		if (del_bool)
			CQ::sendPrivateMsg(fromQQ, "ɾ��Ĭ�ϰ������ɹ�");
		else
			CQ::sendPrivateMsg(fromQQ, "ɾ��Ĭ�ϰ�����ʧ��");

		x = 0;
		break;
	}
	case 13:					//��Ӽ��Ⱥ
	{
		cfg_alone[0].GroupList.push_back(atoll(msg));
		ReInit();

		CQ::sendPrivateMsg(fromQQ, "���Ĭ�ϼ��Ⱥ�ɹ�");
		x = 0;
		break;
	}
	case 14:					//ɾ�����Ⱥ
	{
		bool del_bool = false;

		long long del_GourpId = 0;
		del_GourpId = atoll(msg);

		if (del_GourpId == 0)
		{
			CQ::sendPrivateMsg(fromQQ, "��ʽ����");
			x = 0;
			break;
		}

		auto it = cfg_alone[0].GroupList.begin();

		while (it != cfg_alone[0].GroupList.end())
		{
			if (*it == del_GourpId)
			{
				auto del = it;

				it = cfg_alone[0].GroupList.erase(del);
				del_bool = true;
			}
			else
			{
				it++;
			}

		}

		ReInit();

		if (del_bool)
			CQ::sendPrivateMsg(fromQQ, "ɾ��Ĭ�ϼ��Ⱥ�ɹ�");
		else
			CQ::sendPrivateMsg(fromQQ, "ɾ��Ĭ�ϼ��Ⱥʧ��");
		x = 0;
		break;
	}
	case 15:
	{
		char buf[100] = { '\0' };
		int t;
		t = CQ::sendPrivateMsg(ReplyQQ, msg);
		if (t > 0)
			sprintf(buf, "���͸�QQ%lld�ɹ�", ReplyQQ);
		else
			sprintf(buf, "���͸�QQ%lldʧ��\n������룺%d", ReplyQQ, t);
		CQ::sendPrivateMsg(fromQQ, buf);
		ReplyQQ = 0;
		x = 0;
		break;
	}//�ظ�QQ
	}

}

//ָ����
int Private::instruct(const char* msg)
{
	if (x != 0)
	{
		funcute(msg);
		return 0;
	}


	if (!strcmp(msg, "Ⱥ��ز˵�") || !strcmp(msg, "Ⱥ��ع���") || !strcmp(msg, "�鿴Ⱥ��ز˵�") || !strcmp(msg, "�鿴Ⱥ��ع���"))
	{
		std::string menu =
			"**������(�Զ������ǰ׺)**\n"
			"[CQ:emoji,id=9999]��ӹؼ���\n"
			"[CQ:emoji,id=9999]�鿴�ؼ���\n"
			"[CQ:emoji,id=9999]ɾ���ؼ���\n"
			"[CQ:emoji,id=127984]��Ӽ��Ⱥ\n"
			"[CQ:emoji,id=127984]ɾ�����Ⱥ\n"
			"[CQ:emoji,id=127984]�鿴���Ⱥ\n"
			"[CQ:emoji,id=128203]��Ӱ�����\n"
			"[CQ:emoji,id=128203]ɾ��������\n"
			"[CQ:emoji,id=128203]�鿴������\n"
			"\n**������(�Զ������ǰ׺)**\n"
			"[CQ:emoji,id=127380]�ظ�Ⱥ+QQȺ��\n"
			"[CQ:emoji,id=127380]�ظ�+QQ��\n"
			//"[CQ:emoji,id=127380]�˳�Ⱥ+QQȺ��\n"
			"�뷢�͹������ƣ����������飩";
		CQ::sendPrivateMsg(fromQQ, menu);
		x = 0;
	}

	else if (!std::string(cfg_main.Prefix + "�鿴�ؼ���").compare(msg))
	{
		string SendMsg = "ȫ������Ĭ�Ϲؼ���\n";

		for (auto KeyWord : cfg_alone[0].WKeyWord)
			SendMsg += KeyWord.Word + "\n";

		CQ::sendPrivateMsg(fromQQ, SendMsg);
		x = 0;
	}

	else if (cfg_main.root.empty())
	{
		CQ::sendPrivateMsg(fromQQ, "����Ⱥ������ˣ�����ʹ�ý������Ⱥ�������");
		return -1;
	}

	else if (!std::string(cfg_main.Prefix + "��Ӱ�����").compare(msg))
	{

		CQ::sendPrivateMsg(fromQQ, "�뷢����ӵİ�����QQ");
		x = 11;
	}
	else if (!std::string(cfg_main.Prefix + "ɾ��������").compare(msg))
	{
		std::string SendMsg = "ȫ��Ĭ�ϰ�������\n";

		for (auto QQ : cfg_alone[0].WhiteList)
		{
			auto QQ_inf = CQ::getStrangerInfo(QQ);
			SendMsg += QQ_inf.nick + "(" + to_string(QQ) + ")\n";
		}

		CQ::sendPrivateMsg(fromQQ, SendMsg);
		CQ::sendPrivateMsg(fromQQ, "�뷢��Ҫɾ������������");
		x = 12;
	}
	else if (!std::string(cfg_main.Prefix + "�鿴������").compare(msg))
	{
		std::string SendMsg = "ȫ��Ĭ�ϰ�������\n";

		for (auto QQ : cfg_alone[0].WhiteList)
		{
			auto QQ_inf = CQ::getStrangerInfo(QQ);
			SendMsg += QQ_inf.nick + "(" + to_string(QQ) + ")\n";
		}

		CQ::sendPrivateMsg(fromQQ, SendMsg);

		x = 0;
	}
	else if (!std::string(cfg_main.Prefix + "��Ӽ��Ⱥ").compare(msg))
	{
		CQ::sendPrivateMsg(fromQQ, "�뷢����ӵ�Ⱥ����");
		x = 13;
	}
	else if (!std::string(cfg_main.Prefix + "ɾ�����Ⱥ").compare(msg))
	{
		std::string SendMsg = "ȫ��Ĭ�ϼ��Ⱥ��\n";
		auto temp_GourpList = CQ::getGroupList();

		for (auto GroupId : cfg_alone[0].GroupList)
		{

			SendMsg += temp_GourpList[GroupId] + "(" + to_string(GroupId) + ")\n";
		}

		CQ::sendPrivateMsg(fromQQ, SendMsg);
		CQ::sendPrivateMsg(fromQQ, "�뷢��Ҫɾ��Ⱥ����");
		x = 14;
	}
	else if (!std::string(cfg_main.Prefix + "�鿴���Ⱥ").compare(msg))
	{
		std::string SendMsg = "ȫ��Ĭ�ϼ��Ⱥ��\n";
		auto temp_GourpList = CQ::getGroupList();

		for (auto GroupId : cfg_alone[0].GroupList)
		{

			SendMsg += temp_GourpList[GroupId] + "(" + to_string(GroupId) + ")\n";
		}

		CQ::sendPrivateMsg(fromQQ, SendMsg);

		x = 0;
	}
	else if (!std::string(cfg_main.Prefix + "�ظ�Ⱥ").compare(msg))
	{
		CQ::sendPrivateMsg(fromQQ, "�뷢��Ҫ�ظ���QQȺ��");
		x = 5;
	}
	else if (std::string(msg).find(cfg_main.Prefix + "�ظ�Ⱥ") != string::npos)
	{
		ReplyGroup = atoll(msg + std::string(cfg_main.Prefix + "�ظ�Ⱥ").length());

		if (ReplyGroup == 0)
		{
			sscanf(msg, "�ظ�Ⱥ��%lld", ReplyGroup);
			if (ReplyGroup == 0)
			{
				CQ::sendPrivateMsg(fromQQ, "���������밴�ո�ʽ���·���");
				return -1;
			}
		}
		CQ::sendPrivateMsg(fromQQ, "�뷢�ͻظ�����");
		x = 6;
	}
	else if (!std::string(cfg_main.Prefix + "��ӹؼ���").compare(msg))
	{
		CQ::sendPrivateMsg(fromQQ, "�뷢����ӵĹؼ���");
		x = 7;
	}
	else if (!std::string(cfg_main.Prefix + "ɾ���ؼ���").compare(msg))
	{
		std::string SendMsg = "ȫ��Ĭ�Ϲؼ��ʣ�\n";

		for (auto WKeyWord : cfg_alone[0].WKeyWord)
		{
			SendMsg += WKeyWord.Word + "\n";
		}

		CQ::sendPrivateMsg(fromQQ, SendMsg);

		CQ::sendPrivateMsg(fromQQ, "�뷢��׼��ɾ���Ĺؼ���");
		x = 8;
	}
	//else if (std::string(msg).find(cfg_main.Prefix + "�˳�Ⱥ") != string::npos)
	//{
	//	int64_t GroupNum = atoll(msg + std::string(cfg_main.Prefix + "�˳�Ⱥ").length());
	//	if (GroupNum != 0)
	//	{
	//		CQ::setGroupLeave(GroupNum);
	//		CQ::sendPrivateMsg(fromQQ, ("���˳�Ⱥ" + std::to_string(GroupNum)).c_str());
	//	}
	//	else
	//	{
	//		CQ::sendPrivateMsg(fromQQ, "���������뷢�� �˳�Ⱥ+QQȺ��");
	//	}
	//	x = 0;
	//}
	else if (std::string(msg).find(cfg_main.Prefix + "�ظ�") != string::npos)
	{
		ReplyQQ = atoll(msg + std::string(cfg_main.Prefix + "�ظ�").length());
		if (ReplyQQ != 0)
		{
			CQ::sendPrivateMsg(fromQQ, "�뷢��Ҫ�ظ�����Ϣ");
			x = 15;
		}
		else
		{
			CQ::sendPrivateMsg(fromQQ, "���������뷢�� �ظ�+QQ��");
			x = 0;
		}

	}
	return x;
}


//string ת wstring
wstring OperateMsg::string2wstring(string str)
{
	wstring result;
	//��ȡ��������С��������ռ䣬��������С���ַ�����  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //����ַ�����β  
	//ɾ��������������ֵ  
	result.append(buffer);
	delete[] buffer;
	return result;
}

/*��Ϣ����*/

//�����ؼ��ʲ���
void OperateMsg::KeyWrodWarn(std::string& SendMsg_root, int Way, int WayTime, bool Revoke, bool GroupWarn, string &WordKeyWarn, string& KeyWord, string REWord)
{

	CQ::GroupMemberInfo QQInf = CQ::getGroupMemberInfo(fromGroup, fromQQ);

	std::string GroupAt;//Ⱥ����


	SendMsg_root += "����Ⱥ" + std::to_string(fromGroup) + "��\n";
	SendMsg_root += "QQ����:" + std::to_string(fromQQ) + "\n";
	SendMsg_root += "QQ����:" + QQInf.�ǳ� + "\n";
	SendMsg_root += "QQȺ��Ƭ:" + QQInf.��Ƭ + "\n";
	SendMsg_root += "��������:\n\n";
	SendMsg_root += msg;
	SendMsg_root += "\n";

	if (!REWord.empty())
	{
		SendMsg_root += "\n������ʽ:";
		SendMsg_root += REWord;
	}

	SendMsg_root += "\n�����˹ؼ���:";
	SendMsg_root += KeyWord;
	SendMsg_root += "\n";


	if (Revoke)
		CQ::deleteMsg(msgId);

	if (Way == 1)
	{
		CQ::setGroupBan(fromGroup, fromQQ, WayTime * 60);

		std::string TimeStr;

		int TimeDay = 0;
		int TimeHour = 0;
		int TimeMin = 0;

		TimeDay = WayTime / 1440;
		TimeHour = (WayTime - TimeDay * 1440) / 60;
		TimeMin = WayTime % 60;

		if (TimeDay)
		{
			TimeStr = std::to_string(TimeDay) + "��";
		}

		if (TimeHour)
		{
			TimeStr += std::to_string(TimeHour) + "Сʱ";
		}

		if (TimeMin)
		{
			TimeStr += std::to_string(TimeMin) + "����";
		}


		SendMsg_root += "������������" + TimeStr;
		SendMsg_root += "\n(�ظ��뷢�ͣ��ظ�Ⱥ" + std::to_string(fromGroup) + ")";

		GroupAt = "[CQ:at,qq=" + std::to_string(fromQQ) + "]�����˹ؼ�����ѱ�����" + TimeStr;

	}
	else if (Way == 2)
	{
		CQ::setGroupKick(fromGroup, fromQQ);


		SendMsg_root += "���������߳�";
		SendMsg_root += "\n(�ظ��뷢�ͣ��ظ�Ⱥ" + std::to_string(fromGroup) + ")";

		GroupAt = "[CQ:at,qq=" + std::to_string(fromQQ) + "]�����˹ؼ�����ѱ��߳���Ⱥ";
	}
	else if (Way == 3)
	{
		CQ::setGroupKick(fromGroup, fromQQ, 1);


		SendMsg_root += "���������߳�������";
		SendMsg_root += "\n(�ظ��뷢�ͣ��ظ�Ⱥ" + std::to_string(fromGroup) + ")";

		GroupAt = "[CQ:at,qq=" + std::to_string(fromQQ) + "]�����˹ؼ�����ѱ��߳���Ⱥ������";
	}
	else
	{
		SendMsg_root += "��������δ������";
		SendMsg_root += "\n(�ظ��뷢�ͣ��ظ�Ⱥ" + std::to_string(fromGroup) + ")";

		GroupAt = "[CQ:at,qq=" + std::to_string(fromQQ) + "]�����˹ؼ�����";
	}

	if (GroupWarn)
	{

		if (!WordKeyWarn.empty())
		{
			string SendMsg = WordKeyWarn;
			KeyWordWarnMsg(SendMsg, Way, WayTime);

			CQ::sendGroupMsg(fromGroup, SendMsg);
		}
		else
		{
			CQ::sendGroupMsg(fromGroup, GroupAt);
		}
	}


	for (long long root : cfg_main.root)
	{
		CQ::sendPrivateMsg(root, SendMsg_root);
	}

}

//�Զ��崥���ؼ�������
void OperateMsg::KeyWordWarnMsg(std::string& str, int Way, int WayTime)
{
	//����
	str = replace_all_distinct(str, "{at}", "[CQ:at,qq=" + std::to_string(fromQQ) + "]");

	//��ȡ��ǰʱ��
	SYSTEMTIME sys;
	GetLocalTime(&sys);

	char date[20];
	char time[20];
	char Week[20];

	sprintf(date, "%4d��%02d��%02d��", sys.wYear, sys.wMonth, sys.wDay);
	sprintf(time, "%02d:%02d:%02d.%03d", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	sprintf(Week, "����%d", sys.wDayOfWeek);

	//����
	str = replace_all_distinct(str, "{����}", date);

	//ʱ��
	str = replace_all_distinct(str, "{ʱ��}", time);

	//����
	str = replace_all_distinct(str, "{����}", Week);

	//����ʽ
	if (Way == 1)
	{
		std::string TimeStr = "����";

		int TimeDay = 0;
		int TimeHour = 0;
		int TimeMin = 0;

		TimeDay = WayTime / 1440;
		TimeHour = (WayTime - TimeDay * 1440) / 60;
		TimeMin = WayTime % 60;

		if (TimeDay)
		{
			TimeStr += std::to_string(TimeDay) + "��";
		}

		if (TimeHour)
		{
			TimeStr += std::to_string(TimeHour) + "Сʱ";
		}

		if (TimeMin)
		{
			TimeStr += std::to_string(TimeMin) + "����";
		}


		str = replace_all_distinct(str, "{����ʽ}", TimeStr);
	}
	else if (Way == 2)
	{
		str = replace_all_distinct(str, "{����ʽ}", "�߳���Ⱥ");
	}
	else if (Way == 3)
	{
		str = replace_all_distinct(str, "{����ʽ}", "�߳�������");
	}
	else
	{
		str = replace_all_distinct(str, "{����ʽ}", "δ������");
	}

}

//����������
bool OperateMsg::AloneFun(string alone_name, CFG_ALONE_DLL alone)
{
	////����Ƿ�Ϊ������ ���Ϊ���������˳�����
	//if (find(alone.WhiteList.begin(), alone.WhiteList.end(), fromQQ) != alone.WhiteList.end())
	//{
	//	return;
	//}

	////����ؼ���Ϊ������ ���˳�����
	//if (KeyWordFun(alone.KeyWordWhite, false, NULL))
	//{
	//	return;
	//}


	string KeyWordWarn;

	//��������˹ؼ���
	if (KeyWordFun(alone.WKeyWord, alone.Streng, &KeyWordWarn))
	{
		string SendMsg_root = "�����˵��������еġ�" + alone_name + "��\n";
		KeyWrodWarn(SendMsg_root, alone.Way, alone.WayTime, alone.Revoke, alone.GroupWarn, alone.WordKeyWarn, KeyWordWarn);

		return true;
	}

	string REWrod;
	//���������������ʽ
	if (REKeyKordFun(alone.REKeyWord, &KeyWordWarn, &REWrod))
	{
		string SendMsg_root = "�����˵��������еġ�" + alone_name + "��������ʽ\n";
		KeyWrodWarn(SendMsg_root, alone.Way, alone.WayTime, alone.Revoke, alone.GroupWarn, alone.WordKeyWarn, KeyWordWarn, REWrod);

		return true;
	}

	return false;
}

//�ؼ��ʶԱ�  KeyWord�ؼ������� Strengǿ����� KeyWordWarn��¼�����ؼ���
bool OperateMsg::KeyWordFun(vector<WKEYWORD> KeyWord, bool Streng, string *KeyWordWarn)
{
	//�жϹؼ����Ƿ�Ϊ��
	if (KeyWord.empty() == true)
	{
		return false;
	}

	//��ͨ�ؼ��ʼ��

	for (auto KeyWord_one : KeyWord)
	{
		if (Wmsg.find(KeyWord_one.WWord) != std::wstring::npos)
		{

			if (KeyWordWarn != NULL)
				*KeyWordWarn = KeyWord_one.Word;

			return true;
		}

	}

	//ǿ�����
	if (Streng)
	{

		for (auto KeyWord_one : KeyWord)
		{
			//��¼�ؼ���������ƥ����
			int temp_num = 0, temp_num_find = 0;


			int t_len = KeyWord_one.WWord.size();
			int g_len = Wmsg.size();


			//�ж���Ϣ�͹ؼ����Ƿ�Ϊ��
			if (t_len == 0 || g_len == 0)
			{
				return false;
			}


			//��ʼǿ�����   ����ֶԱ�
			for (int t = 0; t < t_len; t++)
			{
				temp_num++;
				for (int g = 0; g < g_len; g++)
				{
					if (KeyWord_one.WWord[t] == Wmsg[g])
					{
						temp_num_find++;
						break;
					}
				}
			}


			//����ǿ������
			if (temp_num_find >= temp_num)
			{

				if (KeyWordWarn != NULL)
					*KeyWordWarn = KeyWord_one.Word;

				return true;
			}
		}
	}

	return false;
}

//������ʽ
bool OperateMsg::REKeyKordFun(vector<string> REKeyWord, string *REKeyWordWarn, string *REWord)
{
	for (auto alone : REKeyWord)
	{
		try {

			std::regex re(alone);
			std::smatch RE;

			bool rec = std::regex_search(msg, RE, re);

			if (rec)
			{
				*REKeyWordWarn = RE.str().c_str();
				*REWord = alone;

				return true;
			}

		}
		catch (...)
		{
			string SendMsg;
			SendMsg += "������ʽ����\n";
			SendMsg += "���ʽ:";
			SendMsg += alone;
			SendMsg += "��Ϣ:\n\n";
			SendMsg += msg;

			for (long long root : cfg_main.root)
			{
				CQ::sendPrivateMsg(root, SendMsg);
			}
		}
	}
	return false;
}

//���ݴ�����
void OperateMsg::funthing()
{
	//�����Ƿ�ΪĬ�ϼ��Ⱥ
	auto list_bool = find(cfg_alone[0].GroupList.begin(), cfg_alone[0].GroupList.end(), fromGroup);

	//����ҵ��� ΪĬ�ϼ��Ⱥ
	if (list_bool != cfg_alone[0].GroupList.end())
	{
		//����Ƿ�Ϊ������ ���Ϊ���������˳�����
		if (find(cfg_alone[0].WhiteList.begin(), cfg_alone[0].WhiteList.end(), fromQQ) != cfg_alone[0].WhiteList.end())
		{
			return;
		}

		//����ؼ���Ϊ������ ���˳�����
		if (KeyWordFun(cfg_alone[0].KeyWordWhite, false, NULL))
		{
			return;
		}


		string KeyWordWarn;

		//��������˹ؼ���
		if (KeyWordFun(cfg_alone[0].WKeyWord, cfg_alone[0].Streng, &KeyWordWarn))
		{
			string SendMsg_root = "������ȫ�������е�Ĭ�����ùؼ���\n";
			KeyWrodWarn(SendMsg_root, cfg_alone[0].Way, cfg_alone[0].WayTime, cfg_alone[0].Revoke, cfg_alone[0].GroupWarn, cfg_alone[0].WordKeyWarn, KeyWordWarn);

			return;
		}

		string REWord;
		//���������������ʽ�ؼ���
		if (REKeyKordFun(cfg_alone[0].REKeyWord, &KeyWordWarn, &REWord))
		{
			string SendMsg_root = "������ȫ�������е�Ĭ������������ʽ\n";
			KeyWrodWarn(SendMsg_root, cfg_alone[0].Way, cfg_alone[0].WayTime, cfg_alone[0].Revoke, cfg_alone[0].GroupWarn, cfg_alone[0].WordKeyWarn, KeyWordWarn, REWord);

			return;
		}

		for (auto alone_map : cfg_alone[0].Alone)
		{
			if (AloneFun(alone_map.first, alone_map.second))
				return;
		}
	}

	//���Ⱥ��������ִ��
	if (cfg_alone[fromGroup].Switch)
	{

		//����Ƿ�Ϊ������ ���Ϊ���������˳�����
		if (find(cfg_alone[fromGroup].WhiteList.begin(), cfg_alone[fromGroup].WhiteList.end(), fromQQ) != cfg_alone[fromGroup].WhiteList.end())
		{
			return;
		}

		//����ؼ���Ϊ������ ���˳�����
		if (KeyWordFun(cfg_alone[fromGroup].KeyWordWhite, false, NULL))
		{
			return;
		}


		string KeyWordWarn;

		//��������˹ؼ���
		if (KeyWordFun(cfg_alone[fromGroup].WKeyWord, cfg_alone[fromGroup].Streng, &KeyWordWarn))
		{
			string SendMsg_root = "�����˵��������е�Ĭ�����ùؼ���\n";
			KeyWrodWarn(SendMsg_root, cfg_alone[fromGroup].Way, cfg_alone[fromGroup].WayTime, cfg_alone[fromGroup].Revoke, cfg_alone[fromGroup].GroupWarn, cfg_alone[fromGroup].WordKeyWarn, KeyWordWarn);

			return;
		}

		string REWord;
		//���������������ʽ�ؼ���
		if (REKeyKordFun(cfg_alone[fromGroup].REKeyWord, &KeyWordWarn, &REWord))
		{
			string SendMsg_root = "�����˵��������е�Ĭ������������ʽ\n";
			KeyWrodWarn(SendMsg_root, cfg_alone[fromGroup].Way, cfg_alone[fromGroup].WayTime, cfg_alone[fromGroup].Revoke, cfg_alone[fromGroup].GroupWarn, cfg_alone[fromGroup].WordKeyWarn, KeyWordWarn, REWord);

			return;
		}

		for (auto alone_map : cfg_alone[fromGroup].Alone)
		{
			if (AloneFun(alone_map.first, alone_map.second))
				return;

		}
	}

}

OperateMsg::OperateMsg(int64_t fromQQ, int64_t fromGroup, int64_t msgId, const char* msg)
{
	this->fromQQ = fromQQ;
	this->fromGroup = fromGroup;
	this->msgId = msgId;
	this->msg = msg;

	//����Ϣ�Ŀ��ַ�����ʽ��ŵ�������
	this->Wmsg = string2wstring(msg);
}


//Ⱥ�б����
void CQ_getGroupList_charp(char *fun)
{
	map<long long, std::string> a = CQ::getGroupList();
	string str;

	for (auto Group : a)
	{
		str += "[1\\n]";
		str += to_string(Group.first);

		str += "[2\\n]";
		str += Group.second;
	}
	strcpy(fun, str.c_str());
}

EVE_Menu(__menu)
{
	//Ⱥ�б����
	char dll_GroupList[10240];
	CQ_getGroupList_charp(dll_GroupList);


	//�޸�����ǰд������
	if (!Write_ini())
	{
		MessageBox(NULL, TEXT("�����쳣 �ļ�����ʧ��\n����ϵQQȺ��839067703 ����"), TEXT("�����쳣"), MB_OK);
	}

	//�򿪴���
	typedef void(*Pwinfun)(const char* Path, const char* fun);
	HINSTANCE hDLL;
	Pwinfun win;
	hDLL = LoadLibrary(L"GroupMonitorWin.dll"); //���ض�̬���ӿ�MyDll.dll�ļ���
	if (hDLL == NULL) {
		MessageBox(NULL, TEXT("�����ʧ�ܣ���������û�и��ƽ���dll\n�����⻶ӭ��QQȺ��839067703 ����"), TEXT("����"), MB_OK);
		logger.Info("�����ʧ��");
		return 0;
	}
	win = (Pwinfun)GetProcAddress(hDLL, "winfun");

	win(AppFile.c_str(), dll_GroupList);

	FreeLibrary(hDLL);

	//�޸����ú����¶�ȡ����
	if (!Read_ini())
	{
		MessageBox(NULL, TEXT("�����쳣 �ļ���ȡʧ��\n����ϵQQȺ��839067703 ����"), TEXT("�����쳣"), MB_OK);
	}
	else
	{
		for (long long root : cfg_main.root)
		{
			CQ::sendPrivateMsg(root, "��Ⱥ�ؼ��ʼ�ء���������Ч");
		}
	}

	return 0;
}