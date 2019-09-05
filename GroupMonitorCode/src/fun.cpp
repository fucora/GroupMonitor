#include <CQEVE.h>
#include <CQAPI.h>
#include <CQAPI_EX.h>
#include <CQLogger.h>

//数据结构
#include <vector>
#include <map>
#include <string>

//系统
#include <Windows.h>
#include <direct.h>
#include <time.h>
#include <thread>
#include <regex>

#include "fun.h"

using namespace std;
static CQ::Logger logger("群关键词监控");

extern string replace_all_distinct(string& str, string old_value, string new_value);

string AppFile;
map<long long, CFG_ALONE_DLL_ALL> cfg_alone;
CFG_MAIN_DLL cfg_main;

////获取酷Q api函数
//void dll_cq_fun()
//{
//	dll_cq.getGroupList = CQ::getGroupList;
//}

//写入配置
bool Write_ini()
{
	//写入主要配置
	Config_Write_main_dll write_main(cfg_main, AppFile);
	if (!write_main.write_main())
	{
		logger.Info(("配置项写入失败" + AppFile).c_str());
		return false;
	}


	//写入全部单独配置
	Config_Write_dll write_ini(cfg_alone, AppFile);
	if (!write_ini.write_Alone())
	{
		logger.Info(("配置项写入失败" + AppFile).c_str());
		return false;
	}
	return true;
}

//读取配置
bool Read_ini()
{
	//读取主要配置
	Config_Read_main_dll read_main_ini(AppFile);
	if (!read_main_ini.read_main())
	{
		logger.Info(("配置项读取失败" + AppFile).c_str());
		return false;
	}

	cfg_main = read_main_ini.value;

	//读取全部单独配置
	Config_Read_dll read_ini(AppFile);
	if (!read_ini.read_Alone())
	{
		logger.Info(("配置项读取失败" + AppFile).c_str());
		return false;
	}

	//拷贝map
	for (auto new_map : read_ini.value)
	{
		cfg_alone[new_map.first] = new_map.second;
	}
	return true;
}

//写入并读取配置
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


/*私聊指令*/

//私聊消息处理
void Private::funthing(const char* msg)
{
	std::vector<int64_t>::iterator iter;
	iter = find(cfg_main.root.begin(), cfg_main.root.end(), fromQQ);

	if (iter == cfg_main.root.end())
	{
		//不是主人时将消息转发给主人
		MsgRelayFun(msg);
	}
	else
	{
		//为主人时进入指令函数
		instruct(msg);
	}
}

//消息转发
void Private::MsgRelayFun(const char* msg)
{
	if (cfg_main.MsgRelay)
	{
		std::string SendMsg;

		//获取QQ资料
		CQ::StrangerInfo QQInf = CQ::getStrangerInfo(fromQQ);

		//构造消息
		SendMsg = "来自" + QQInf.nick + "(" + std::to_string(fromQQ) + ")的私聊消息:\n\n";
		SendMsg += msg;
		SendMsg += "\n\n(回复请发送 回复" + std::to_string(fromQQ) + ")";

		//发送给主人
		for (long long root : cfg_main.root)
		{
			CQ::sendPrivateMsg(root, SendMsg);
		}
	}
}

//指令对话
void Private::funcute(const char* msg)
{
	switch (x)
	{
	case 5:
	{
		ReplyGroup = atoll(msg);
		CQ::sendPrivateMsg(fromQQ, "请发送回复内容");
		x = 6;
		break;
	}
	case 6:
	{
		char ch[100] = { '\0' };
		int t;
		t = CQ::sendGroupMsg(ReplyGroup, msg);
		if (t > 0)
			sprintf(ch, "发送给QQ群%lld成功", ReplyGroup);
		else
			sprintf(ch, "发送给QQ群%lld失败\n错误代码：%d", ReplyGroup, t);
		CQ::sendPrivateMsg(fromQQ, ch);
		ReplyGroup = 0;
		x = 0;
		break;
	}
	case 7:						//添加关键词
	{
		WKEYWORD Temp_add;
		Temp_add.Word = msg;
		cfg_alone[0].WKeyWord.push_back(Temp_add);
		ReInit();

		CQ::sendPrivateMsg(fromQQ, "添加默认关键词成功");

		x = 0;
		break;
	}
	case 8:						//删除关键词
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
			CQ::sendPrivateMsg(fromQQ, "删除默认关键词成功");
		else
			CQ::sendPrivateMsg(fromQQ, "删除默认关键词失败");

		x = 0;
		break;
	}
	case 11:					//添加白名单
	{
		cfg_alone[0].WhiteList.push_back(atoll(msg));
		ReInit();

		CQ::sendPrivateMsg(fromQQ, "添加默认白名单成功");

		x = 0;
		break;
	}
	case 12:					//删除白名单
	{
		bool del_bool = false;

		long long del_QQId = 0;
		del_QQId = atoll(msg);

		if (del_QQId == 0)
		{
			CQ::sendPrivateMsg(fromQQ, "格式有误");
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
			CQ::sendPrivateMsg(fromQQ, "删除默认白名单成功");
		else
			CQ::sendPrivateMsg(fromQQ, "删除默认白名单失败");

		x = 0;
		break;
	}
	case 13:					//添加监控群
	{
		cfg_alone[0].GroupList.push_back(atoll(msg));
		ReInit();

		CQ::sendPrivateMsg(fromQQ, "添加默认监控群成功");
		x = 0;
		break;
	}
	case 14:					//删除监控群
	{
		bool del_bool = false;

		long long del_GourpId = 0;
		del_GourpId = atoll(msg);

		if (del_GourpId == 0)
		{
			CQ::sendPrivateMsg(fromQQ, "格式有误");
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
			CQ::sendPrivateMsg(fromQQ, "删除默认监控群成功");
		else
			CQ::sendPrivateMsg(fromQQ, "删除默认监控群失败");
		x = 0;
		break;
	}
	case 15:
	{
		char buf[100] = { '\0' };
		int t;
		t = CQ::sendPrivateMsg(ReplyQQ, msg);
		if (t > 0)
			sprintf(buf, "发送给QQ%lld成功", ReplyQQ);
		else
			sprintf(buf, "发送给QQ%lld失败\n错误代码：%d", ReplyQQ, t);
		CQ::sendPrivateMsg(fromQQ, buf);
		ReplyQQ = 0;
		x = 0;
		break;
	}//回复QQ
	}

}

//指令检测
int Private::instruct(const char* msg)
{
	if (x != 0)
	{
		funcute(msg);
		return 0;
	}


	if (!strcmp(msg, "群监控菜单") || !strcmp(msg, "群监控功能") || !strcmp(msg, "查看群监控菜单") || !strcmp(msg, "查看群监控功能"))
	{
		std::string menu =
			"**设置类(自定义需加前缀)**\n"
			"[CQ:emoji,id=9999]添加关键词\n"
			"[CQ:emoji,id=9999]查看关键词\n"
			"[CQ:emoji,id=9999]删除关键词\n"
			"[CQ:emoji,id=127984]添加监控群\n"
			"[CQ:emoji,id=127984]删除监控群\n"
			"[CQ:emoji,id=127984]查看监控群\n"
			"[CQ:emoji,id=128203]添加白名单\n"
			"[CQ:emoji,id=128203]删除白名单\n"
			"[CQ:emoji,id=128203]查看白名单\n"
			"\n**其他类(自定义需加前缀)**\n"
			"[CQ:emoji,id=127380]回复群+QQ群号\n"
			"[CQ:emoji,id=127380]回复+QQ号\n"
			//"[CQ:emoji,id=127380]退出群+QQ群号\n"
			"请发送功能名称（不包括表情）";
		CQ::sendPrivateMsg(fromQQ, menu);
		x = 0;
	}

	else if (!std::string(cfg_main.Prefix + "查看关键词").compare(msg))
	{
		string SendMsg = "全局设置默认关键词\n";

		for (auto KeyWord : cfg_alone[0].WKeyWord)
			SendMsg += KeyWord.Word + "\n";

		CQ::sendPrivateMsg(fromQQ, SendMsg);
		x = 0;
	}

	else if (cfg_main.root.empty())
	{
		CQ::sendPrivateMsg(fromQQ, "暂无群监控主人，请先使用界面添加群监控主人");
		return -1;
	}

	else if (!std::string(cfg_main.Prefix + "添加白名单").compare(msg))
	{

		CQ::sendPrivateMsg(fromQQ, "请发送添加的白名单QQ");
		x = 11;
	}
	else if (!std::string(cfg_main.Prefix + "删除白名单").compare(msg))
	{
		std::string SendMsg = "全局默认白名单：\n";

		for (auto QQ : cfg_alone[0].WhiteList)
		{
			auto QQ_inf = CQ::getStrangerInfo(QQ);
			SendMsg += QQ_inf.nick + "(" + to_string(QQ) + ")\n";
		}

		CQ::sendPrivateMsg(fromQQ, SendMsg);
		CQ::sendPrivateMsg(fromQQ, "请发送要删除白名单号码");
		x = 12;
	}
	else if (!std::string(cfg_main.Prefix + "查看白名单").compare(msg))
	{
		std::string SendMsg = "全局默认白名单：\n";

		for (auto QQ : cfg_alone[0].WhiteList)
		{
			auto QQ_inf = CQ::getStrangerInfo(QQ);
			SendMsg += QQ_inf.nick + "(" + to_string(QQ) + ")\n";
		}

		CQ::sendPrivateMsg(fromQQ, SendMsg);

		x = 0;
	}
	else if (!std::string(cfg_main.Prefix + "添加监控群").compare(msg))
	{
		CQ::sendPrivateMsg(fromQQ, "请发送添加的群号码");
		x = 13;
	}
	else if (!std::string(cfg_main.Prefix + "删除监控群").compare(msg))
	{
		std::string SendMsg = "全局默认监控群：\n";
		auto temp_GourpList = CQ::getGroupList();

		for (auto GroupId : cfg_alone[0].GroupList)
		{

			SendMsg += temp_GourpList[GroupId] + "(" + to_string(GroupId) + ")\n";
		}

		CQ::sendPrivateMsg(fromQQ, SendMsg);
		CQ::sendPrivateMsg(fromQQ, "请发送要删除群号码");
		x = 14;
	}
	else if (!std::string(cfg_main.Prefix + "查看监控群").compare(msg))
	{
		std::string SendMsg = "全局默认监控群：\n";
		auto temp_GourpList = CQ::getGroupList();

		for (auto GroupId : cfg_alone[0].GroupList)
		{

			SendMsg += temp_GourpList[GroupId] + "(" + to_string(GroupId) + ")\n";
		}

		CQ::sendPrivateMsg(fromQQ, SendMsg);

		x = 0;
	}
	else if (!std::string(cfg_main.Prefix + "回复群").compare(msg))
	{
		CQ::sendPrivateMsg(fromQQ, "请发送要回复的QQ群号");
		x = 5;
	}
	else if (std::string(msg).find(cfg_main.Prefix + "回复群") != string::npos)
	{
		ReplyGroup = atoll(msg + std::string(cfg_main.Prefix + "回复群").length());

		if (ReplyGroup == 0)
		{
			sscanf(msg, "回复群＋%lld", ReplyGroup);
			if (ReplyGroup == 0)
			{
				CQ::sendPrivateMsg(fromQQ, "输入有误，请按照格式重新发送");
				return -1;
			}
		}
		CQ::sendPrivateMsg(fromQQ, "请发送回复内容");
		x = 6;
	}
	else if (!std::string(cfg_main.Prefix + "添加关键词").compare(msg))
	{
		CQ::sendPrivateMsg(fromQQ, "请发送添加的关键词");
		x = 7;
	}
	else if (!std::string(cfg_main.Prefix + "删除关键词").compare(msg))
	{
		std::string SendMsg = "全局默认关键词：\n";

		for (auto WKeyWord : cfg_alone[0].WKeyWord)
		{
			SendMsg += WKeyWord.Word + "\n";
		}

		CQ::sendPrivateMsg(fromQQ, SendMsg);

		CQ::sendPrivateMsg(fromQQ, "请发送准备删除的关键词");
		x = 8;
	}
	//else if (std::string(msg).find(cfg_main.Prefix + "退出群") != string::npos)
	//{
	//	int64_t GroupNum = atoll(msg + std::string(cfg_main.Prefix + "退出群").length());
	//	if (GroupNum != 0)
	//	{
	//		CQ::setGroupLeave(GroupNum);
	//		CQ::sendPrivateMsg(fromQQ, ("已退出群" + std::to_string(GroupNum)).c_str());
	//	}
	//	else
	//	{
	//		CQ::sendPrivateMsg(fromQQ, "输入有误，请发送 退出群+QQ群号");
	//	}
	//	x = 0;
	//}
	else if (std::string(msg).find(cfg_main.Prefix + "回复") != string::npos)
	{
		ReplyQQ = atoll(msg + std::string(cfg_main.Prefix + "回复").length());
		if (ReplyQQ != 0)
		{
			CQ::sendPrivateMsg(fromQQ, "请发送要回复的消息");
			x = 15;
		}
		else
		{
			CQ::sendPrivateMsg(fromQQ, "输入有误，请发送 回复+QQ号");
			x = 0;
		}

	}
	return x;
}


//string 转 wstring
wstring OperateMsg::string2wstring(string str)
{
	wstring result;
	//获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	//多字节编码转换成宽字节编码  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //添加字符串结尾  
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}

/*消息处理*/

//触发关键词操作
void OperateMsg::KeyWrodWarn(std::string& SendMsg_root, int Way, int WayTime, bool Revoke, bool GroupWarn, string &WordKeyWarn, string& KeyWord, string REWord)
{

	CQ::GroupMemberInfo QQInf = CQ::getGroupMemberInfo(fromGroup, fromQQ);

	std::string GroupAt;//群艾特


	SendMsg_root += "来自群" + std::to_string(fromGroup) + "中\n";
	SendMsg_root += "QQ号码:" + std::to_string(fromQQ) + "\n";
	SendMsg_root += "QQ名称:" + QQInf.昵称 + "\n";
	SendMsg_root += "QQ群名片:" + QQInf.名片 + "\n";
	SendMsg_root += "由于内容:\n\n";
	SendMsg_root += msg;
	SendMsg_root += "\n";

	if (!REWord.empty())
	{
		SendMsg_root += "\n正则表达式:";
		SendMsg_root += REWord;
	}

	SendMsg_root += "\n触发了关键词:";
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
			TimeStr = std::to_string(TimeDay) + "天";
		}

		if (TimeHour)
		{
			TimeStr += std::to_string(TimeHour) + "小时";
		}

		if (TimeMin)
		{
			TimeStr += std::to_string(TimeMin) + "分钟";
		}


		SendMsg_root += "处理方法：禁言" + TimeStr;
		SendMsg_root += "\n(回复请发送：回复群" + std::to_string(fromGroup) + ")";

		GroupAt = "[CQ:at,qq=" + std::to_string(fromQQ) + "]触发了关键词语，已被禁言" + TimeStr;

	}
	else if (Way == 2)
	{
		CQ::setGroupKick(fromGroup, fromQQ);


		SendMsg_root += "处理方法：踢出";
		SendMsg_root += "\n(回复请发送：回复群" + std::to_string(fromGroup) + ")";

		GroupAt = "[CQ:at,qq=" + std::to_string(fromQQ) + "]触发了关键词语，已被踢出本群";
	}
	else if (Way == 3)
	{
		CQ::setGroupKick(fromGroup, fromQQ, 1);


		SendMsg_root += "处理方法：踢出并拉黑";
		SendMsg_root += "\n(回复请发送：回复群" + std::to_string(fromGroup) + ")";

		GroupAt = "[CQ:at,qq=" + std::to_string(fromQQ) + "]触发了关键词语，已被踢出本群并拉黑";
	}
	else
	{
		SendMsg_root += "处理方法：未做处理";
		SendMsg_root += "\n(回复请发送：回复群" + std::to_string(fromGroup) + ")";

		GroupAt = "[CQ:at,qq=" + std::to_string(fromQQ) + "]触发了关键词语";
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

//自定义触发关键词提醒
void OperateMsg::KeyWordWarnMsg(std::string& str, int Way, int WayTime)
{
	//艾特
	str = replace_all_distinct(str, "{at}", "[CQ:at,qq=" + std::to_string(fromQQ) + "]");

	//获取当前时间
	SYSTEMTIME sys;
	GetLocalTime(&sys);

	char date[20];
	char time[20];
	char Week[20];

	sprintf(date, "%4d年%02d月%02d日", sys.wYear, sys.wMonth, sys.wDay);
	sprintf(time, "%02d:%02d:%02d.%03d", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	sprintf(Week, "星期%d", sys.wDayOfWeek);

	//日期
	str = replace_all_distinct(str, "{日期}", date);

	//时间
	str = replace_all_distinct(str, "{时间}", time);

	//星期
	str = replace_all_distinct(str, "{星期}", Week);

	//处理方式
	if (Way == 1)
	{
		std::string TimeStr = "禁言";

		int TimeDay = 0;
		int TimeHour = 0;
		int TimeMin = 0;

		TimeDay = WayTime / 1440;
		TimeHour = (WayTime - TimeDay * 1440) / 60;
		TimeMin = WayTime % 60;

		if (TimeDay)
		{
			TimeStr += std::to_string(TimeDay) + "天";
		}

		if (TimeHour)
		{
			TimeStr += std::to_string(TimeHour) + "小时";
		}

		if (TimeMin)
		{
			TimeStr += std::to_string(TimeMin) + "分钟";
		}


		str = replace_all_distinct(str, "{处理方式}", TimeStr);
	}
	else if (Way == 2)
	{
		str = replace_all_distinct(str, "{处理方式}", "踢出本群");
	}
	else if (Way == 3)
	{
		str = replace_all_distinct(str, "{处理方式}", "踢出并拉黑");
	}
	else
	{
		str = replace_all_distinct(str, "{处理方式}", "未做处理");
	}

}

//单独处理函数
bool OperateMsg::AloneFun(string alone_name, CFG_ALONE_DLL alone)
{
	////检测是否为白名单 如果为白名单将退出函数
	//if (find(alone.WhiteList.begin(), alone.WhiteList.end(), fromQQ) != alone.WhiteList.end())
	//{
	//	return;
	//}

	////如果关键词为白名单 将退出函数
	//if (KeyWordFun(alone.KeyWordWhite, false, NULL))
	//{
	//	return;
	//}


	string KeyWordWarn;

	//如果触发了关键词
	if (KeyWordFun(alone.WKeyWord, alone.Streng, &KeyWordWarn))
	{
		string SendMsg_root = "触发了单独设置中的“" + alone_name + "”\n";
		KeyWrodWarn(SendMsg_root, alone.Way, alone.WayTime, alone.Revoke, alone.GroupWarn, alone.WordKeyWarn, KeyWordWarn);

		return true;
	}

	string REWrod;
	//如果触发了正则表达式
	if (REKeyKordFun(alone.REKeyWord, &KeyWordWarn, &REWrod))
	{
		string SendMsg_root = "触发了单独设置中的“" + alone_name + "”正则表达式\n";
		KeyWrodWarn(SendMsg_root, alone.Way, alone.WayTime, alone.Revoke, alone.GroupWarn, alone.WordKeyWarn, KeyWordWarn, REWrod);

		return true;
	}

	return false;
}

//关键词对比  KeyWord关键词容器 Streng强力检测 KeyWordWarn记录触发关键词
bool OperateMsg::KeyWordFun(vector<WKEYWORD> KeyWord, bool Streng, string *KeyWordWarn)
{
	//判断关键词是否为空
	if (KeyWord.empty() == true)
	{
		return false;
	}

	//普通关键词检测

	for (auto KeyWord_one : KeyWord)
	{
		if (Wmsg.find(KeyWord_one.WWord) != std::wstring::npos)
		{

			if (KeyWordWarn != NULL)
				*KeyWordWarn = KeyWord_one.Word;

			return true;
		}

	}

	//强力检测
	if (Streng)
	{

		for (auto KeyWord_one : KeyWord)
		{
			//记录关键词字数与匹配数
			int temp_num = 0, temp_num_find = 0;


			int t_len = KeyWord_one.WWord.size();
			int g_len = Wmsg.size();


			//判断消息和关键词是否为空
			if (t_len == 0 || g_len == 0)
			{
				return false;
			}


			//开始强力检测   逐个字对比
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


			//触发强力检测后
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

//正则表达式
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
			SendMsg += "正则表达式崩溃\n";
			SendMsg += "表达式:";
			SendMsg += alone;
			SendMsg += "消息:\n\n";
			SendMsg += msg;

			for (long long root : cfg_main.root)
			{
				CQ::sendPrivateMsg(root, SendMsg);
			}
		}
	}
	return false;
}

//内容处理功能
void OperateMsg::funthing()
{
	//查找是否为默认监控群
	auto list_bool = find(cfg_alone[0].GroupList.begin(), cfg_alone[0].GroupList.end(), fromGroup);

	//如果找到看 为默认监控群
	if (list_bool != cfg_alone[0].GroupList.end())
	{
		//检测是否为白名单 如果为白名单将退出函数
		if (find(cfg_alone[0].WhiteList.begin(), cfg_alone[0].WhiteList.end(), fromQQ) != cfg_alone[0].WhiteList.end())
		{
			return;
		}

		//如果关键词为白名单 将退出函数
		if (KeyWordFun(cfg_alone[0].KeyWordWhite, false, NULL))
		{
			return;
		}


		string KeyWordWarn;

		//如果触发了关键词
		if (KeyWordFun(cfg_alone[0].WKeyWord, cfg_alone[0].Streng, &KeyWordWarn))
		{
			string SendMsg_root = "触发了全局设置中的默认设置关键词\n";
			KeyWrodWarn(SendMsg_root, cfg_alone[0].Way, cfg_alone[0].WayTime, cfg_alone[0].Revoke, cfg_alone[0].GroupWarn, cfg_alone[0].WordKeyWarn, KeyWordWarn);

			return;
		}

		string REWord;
		//如果触发了正则表达式关键词
		if (REKeyKordFun(cfg_alone[0].REKeyWord, &KeyWordWarn, &REWord))
		{
			string SendMsg_root = "触发了全局设置中的默认设置正则表达式\n";
			KeyWrodWarn(SendMsg_root, cfg_alone[0].Way, cfg_alone[0].WayTime, cfg_alone[0].Revoke, cfg_alone[0].GroupWarn, cfg_alone[0].WordKeyWarn, KeyWordWarn, REWord);

			return;
		}

		for (auto alone_map : cfg_alone[0].Alone)
		{
			if (AloneFun(alone_map.first, alone_map.second))
				return;
		}
	}

	//如果群单独开启执行
	if (cfg_alone[fromGroup].Switch)
	{

		//检测是否为白名单 如果为白名单将退出函数
		if (find(cfg_alone[fromGroup].WhiteList.begin(), cfg_alone[fromGroup].WhiteList.end(), fromQQ) != cfg_alone[fromGroup].WhiteList.end())
		{
			return;
		}

		//如果关键词为白名单 将退出函数
		if (KeyWordFun(cfg_alone[fromGroup].KeyWordWhite, false, NULL))
		{
			return;
		}


		string KeyWordWarn;

		//如果触发了关键词
		if (KeyWordFun(cfg_alone[fromGroup].WKeyWord, cfg_alone[fromGroup].Streng, &KeyWordWarn))
		{
			string SendMsg_root = "触发了单独设置中的默认设置关键词\n";
			KeyWrodWarn(SendMsg_root, cfg_alone[fromGroup].Way, cfg_alone[fromGroup].WayTime, cfg_alone[fromGroup].Revoke, cfg_alone[fromGroup].GroupWarn, cfg_alone[fromGroup].WordKeyWarn, KeyWordWarn);

			return;
		}

		string REWord;
		//如果触发了正则表达式关键词
		if (REKeyKordFun(cfg_alone[fromGroup].REKeyWord, &KeyWordWarn, &REWord))
		{
			string SendMsg_root = "触发了单独设置中的默认设置正则表达式\n";
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

	//将消息的宽字符串格式存放到对象中
	this->Wmsg = string2wstring(msg);
}


//群列表编码
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
	//群列表编码
	char dll_GroupList[10240];
	CQ_getGroupList_charp(dll_GroupList);


	//修改配置前写入配置
	if (!Write_ini())
	{
		MessageBox(NULL, TEXT("严重异常 文件存入失败\n请联系QQ群：839067703 求助"), TEXT("严重异常"), MB_OK);
	}

	//打开窗口
	typedef void(*Pwinfun)(const char* Path, const char* fun);
	HINSTANCE hDLL;
	Pwinfun win;
	hDLL = LoadLibrary(L"GroupMonitorWin.dll"); //加载动态链接库MyDll.dll文件；
	if (hDLL == NULL) {
		MessageBox(NULL, TEXT("界面打开失败，可能是你没有复制界面dll\n有问题欢迎到QQ群：839067703 求助"), TEXT("错误"), MB_OK);
		logger.Info("界面打开失败");
		return 0;
	}
	win = (Pwinfun)GetProcAddress(hDLL, "winfun");

	win(AppFile.c_str(), dll_GroupList);

	FreeLibrary(hDLL);

	//修改配置后重新读取配置
	if (!Read_ini())
	{
		MessageBox(NULL, TEXT("严重异常 文件读取失败\n请联系QQ群：839067703 求助"), TEXT("严重异常"), MB_OK);
	}
	else
	{
		for (long long root : cfg_main.root)
		{
			CQ::sendPrivateMsg(root, "《群关键词监控》配置已生效");
		}
	}

	return 0;
}