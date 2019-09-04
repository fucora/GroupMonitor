#include <iostream>
#include "Configini.h"


void ErrorInf()
{
	MessageBox(NULL, TEXT("非法调用"), TEXT("错误"), MB_OK);
}


//写入父类

//ini配置项写入--字符串
bool Config_Write_father::write_ini_str(const char* lpAppName, const char* lpKeyName, const char* lpFileName, string conf_value, bool replace = true)
{
	for (auto it = conf_value.begin(); it != conf_value.end();)
	{
		if (*it == '\r')
		{
			auto del = it;
			it = conf_value.erase(del);
		}
		else
		{
			it++;
		}
	}

	//转换\n
	if (replace)
		write_replace_line(conf_value);

	int err = WritePrivateProfileStringA(lpAppName, lpKeyName, conf_value.c_str(), (path + lpFileName).c_str());

	if (err)
		return true;
	else
		return false;


}

#ifdef MFC

//ini配置项写入--CString
bool Config_Write_father::write_ini_CString(const char* lpAppName, const char* lpKeyName, const char* lpFileName, CString conf_value)
{
	CStringA temp_A(conf_value);

	string temp_str(temp_A.GetBuffer());

	write_ini_str(lpAppName, lpKeyName, lpFileName, temp_str);
	return true;
}

#endif
//ini配置项写入--int
bool Config_Write_father::write_ini_int(const char* lpAppName, const char* lpKeyName, const char* lpFileName, int conf_value)
{
	string cfg = to_string(conf_value);

	return write_ini_str(lpAppName, lpKeyName, lpFileName, cfg, false);
}

//列表写入 string
bool Config_Write_father::write_ini_SubStr_string(const char* lpAppName, const char* lpKeyName, const char* lpFileName, vector<string> word)
{
	string temp_Word = "";
	for (auto KeyWord : word)
	{
		//保证每项前都有分隔符
		if (!temp_Word.empty())
		{
			temp_Word += "[\\n]";
		}

		temp_Word += KeyWord;
	}
	bool err = write_ini_str(lpAppName, lpKeyName, lpFileName, temp_Word);

	if (!err)
		return false;
	else
		return true;
}

//列表写入 WKEYWORD
bool Config_Write_father::write_ini_SubStr_WKEYWORD(const char* lpAppName, const char* lpKeyName, const char* lpFileName, vector<WKEYWORD> word)
{
	string temp_Word = "";
	for (auto KeyWord : word)
	{
		//保证每项前都有分隔符
		if (!temp_Word.empty())
		{
			temp_Word += "[\\n]";
		}

		temp_Word += KeyWord.Word;
	}
	bool err = write_ini_str(lpAppName, lpKeyName, lpFileName, temp_Word);

	if (!err)
		return false;
	else
		return true;
}

//列表写入 longlong
bool Config_Write_father::write_ini_SubStr_longlong(const char* lpAppName, const char* lpKeyName, const char* lpFileName, vector<long long> QQList)
{
	string temp_longlong = "";

	for (long long longlong_qq : QQList)
	{
		if (!temp_longlong.empty())
		{
			temp_longlong += "[\\n]";
		}
		temp_longlong += to_string(longlong_qq);
	}
	bool err = write_ini_str(lpAppName, lpKeyName, lpFileName, temp_longlong);

	if (!err)
		return false;
	else
		return true;
}

//字符串替换 \n转[\\n]
void Config_Write_father::write_replace_line(string &str)
{
	while (1)
	{
		if (str.empty())
			break;

		if (str.find("\n") == string::npos)
			break;

		str.replace(str.find("\n"), strlen("\n"), "[\\n]");

	}
}


//读取父类

//全部的单独的配置项
bool Config_Read_father::read_Alone()
{
	//读取默认监控群

	Read_DefGroupList();





	//读取配置QQ
	vector<long long> temp_name_qq;
	int err = read_ini_SubStr_longlong("main", "GroupList", "Group.ini", temp_name_qq);
	if (!err)
	{
		return false;
	}

	//遍历单个QQ
	for (long long qq_id : temp_name_qq)
	{
		string qq_str = to_string(qq_id).c_str();
		//读取单个配置名
		vector<string> temp_name_cfg;
		read_ini_SubStr_str(qq_str.c_str(), "Config", "Group.ini", temp_name_cfg);

		//遍历单个配置名
		read_AloneOne(qq_id, temp_name_cfg);

	}

	return true;
}

bool Config_Read_father::Read_DefGroupList()
{
	ErrorInf();
	return false;
}

bool Config_Read_father::read_AloneOne(long long qq_id, vector<string> &temp_name_cfg)
{
	ErrorInf();
	return false;
}

//string 转 wstring
wstring Config_Read_father::string2wstring(string str)
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

/*
	const char* lpAppName, 欲在其中查找条目的节名称。如设为NULL，就在lpReturnedString缓冲区内装载这个ini文件所有节的名字。
	const char* lpKeyName, 欲获取的项名或条目名。如设为NULL，就在lpReturnedString缓冲区内装载指定小节所有项的列表
	const char* lpFileName, ini文件的名字。如没有指定一个完整路径名，windows就在Windows目录中查找文件
	string &conf_value, 接收读取的字符串
	*/
	//ini配置项读取--字符串
bool Config_Read_father::read_ini_str(const char* lpAppName, const char* lpKeyName, const char* lpFileName, string &conf_value, bool replace = true)
{
	int nSize = 1024;
	while (1)
	{
		char *ch = new char[nSize];
		int err = GetPrivateProfileStringA(lpAppName, lpKeyName, "NULL", ch, nSize, (path + lpFileName).c_str());

		//如果返回值报错就重新调整字符串大小 重新进入while
		if (err == nSize - 1)
		{
			delete[] ch;
			nSize += 1024;
		}
		//如果读取失败就直接 结束函数
		else if (!strcmp(ch, "NULL"))
		{

			return false;
		}
		else
		{
			conf_value = ch;

			if (replace)
				read_replace_line(conf_value);//回行转换
			return true;
		}

		//如果分配内存太多就自动返回读取失败
		if (nSize > 50000)
		{
			return false;
		}
	}
}

#ifdef MFC

//ini读取配置项 CString
bool Config_Read_father::read_ini_CString(const char* lpAppName, const char* lpKeyName, const char* lpFileName, CString &conf_value)
{
	string temp_str;
	int err = read_ini_str(lpAppName, lpKeyName, lpFileName, temp_str);
	if (!err)
	{
		return false;
	}

	CString temp_value(temp_str.c_str());
	conf_value = temp_value;

	return true;
}

#endif

//解析以[\\n]分割的字符串
bool Config_Read_father::read_ini_SubStr_str(const char* lpAppName, const char* lpKeyName, const char* lpFileName, vector<string> &value)
{
	//读取配置所有字符
	string conf_value;
	vector<string> temp_value_vector;
	int err = read_ini_str(lpAppName, lpKeyName, lpFileName, conf_value, false);
	if (!err)
		return false;

	//解析字符
	size_t start_i = 0;
	size_t end_i = 0;
	while (1)
	{
		end_i = conf_value.find("[\\n]", start_i);

		if (end_i == string::npos)
		{
			string buf = conf_value.substr(start_i);
			if (!buf.empty())
				temp_value_vector.push_back(buf);

			break;
		}


		string buf = conf_value.substr(start_i, end_i - start_i);
		if (!buf.empty())
			temp_value_vector.push_back(buf);


		//改变开始位置 去掉前边的[\\n]
		start_i = end_i + strlen("[\\n]");
	}

	//删除重复项
	set<string> temp_value_set(temp_value_vector.begin(), temp_value_vector.end());
	value.assign(temp_value_set.begin(), temp_value_set.end());

	return true;
}

//解析以[\\n]分割的longlong
bool Config_Read_father::read_ini_SubStr_longlong(const char* lpAppName, const char* lpKeyName, const char* lpFileName, vector<long long> &value)
{
	vector<string> temp_ValueStr;
	int err = read_ini_SubStr_str(lpAppName, lpKeyName, lpFileName, temp_ValueStr);
	if (!err)
	{
		return false;
	}

	for (string qq : temp_ValueStr)
	{
		value.push_back(atoll(qq.c_str()));
	}
	return true;
}

//ini配置项读取--int---1
int Config_Read_father::read_ini_int(const char* lpAppName, const char*lpKeyName, const char* lpFileName, int &conf_value)
{
	conf_value = GetPrivateProfileIntA(lpAppName, lpKeyName, conf_value, (path + lpFileName).c_str());
	return conf_value;
}

//ini配置项读取--int---2
int Config_Read_father::read_ini_int(const char* lpAppName, const char*lpKeyName, const char* lpFileName)
{
	int conf_value = GetPrivateProfileIntA(lpAppName, lpKeyName, 0, (path + lpFileName).c_str());
	return conf_value;
}

//字符串替换 [\\n]转\n
void Config_Read_father::read_replace_line(string &str)
{
	while (1)
	{
		if (str.empty())
			break;


		if (str.find("[\\n]") == string::npos)
			break;

#ifdef MFC

		str.replace(str.find("[\\n]"), strlen("[\\n]"), "\r\n");
#else
		str.replace(str.find("[\\n]"), strlen("[\\n]"), "\n");
#endif
	}
}


//ini写入封装--插件

//构造函数
Config_Write_dll::Config_Write_dll(map<long long, CFG_ALONE_DLL_ALL> new_value_dll, string new_path)
{
	this->path = new_path;

	//拷贝map
	for (auto new_map : new_value_dll)
	{

		for (auto new_value : new_map.second.Alone)
		{

			value[new_map.first].Alone[new_value.first] = new_value.second;

		}

		value[new_map.first].GroupWarn = new_map.second.GroupWarn;
		value[new_map.first].KeyWordWhite = new_map.second.KeyWordWhite;
		value[new_map.first].Revoke = new_map.second.Revoke;
		value[new_map.first].Streng = new_map.second.Streng;
		value[new_map.first].WKeyWord = new_map.second.WKeyWord;
		value[new_map.first].REKeyWord = new_map.second.REKeyWord;
		value[new_map.first].Switch = new_map.second.Switch;
		value[new_map.first].Way = new_map.second.Way;
		value[new_map.first].WayTime = new_map.second.WayTime;
		value[new_map.first].WhiteList = new_map.second.WhiteList;
		value[new_map.first].WordKeyWarn = new_map.second.WordKeyWarn;
		value[new_map.first].GroupList = new_map.second.GroupList;
	}

	//默认配置项

}

//全部的单独的配置项 插件写入到文件
bool Config_Write_dll::write_Alone()
{
	//写入默认监控群列表
	write_ini_SubStr_longlong("0", "GroupList", "Group.ini", value[0].GroupList);


	//写入QQ

	/*********
	构造全部配置名格式

	如：
	[main]
	qq=1164442003[\n]1164442004

	************/
	string qq_str = "";
	for (auto qq : value)
	{
		//保证添加前一个有分隔符
		if (!qq_str.empty())
		{
			qq_str += "[\\n]";
		}

		//配置名全部累加
		qq_str += to_string(qq.first);


		//默认配置项
		string lpAppName = to_string(qq.first).c_str();

		//关键词
		write_ini_SubStr_WKEYWORD(lpAppName.c_str(), "KeyWord", "Group.ini", qq.second.WKeyWord);

		//正则表达式关键词
		write_ini_SubStr_string(lpAppName.c_str(), "REKeyWord", "Group.ini", qq.second.REKeyWord);


		//白名单关键词
		write_ini_SubStr_WKEYWORD(lpAppName.c_str(), "KeyWordWhite", "Group.ini", qq.second.KeyWordWhite);


		//自定义触发提醒
		write_ini_str(lpAppName.c_str(), "WordKeyWarn", "Group.ini", qq.second.WordKeyWarn);


		//白名单
		write_ini_SubStr_longlong(lpAppName.c_str(), "WhiteList", "Group.ini", qq.second.WhiteList);


		//开关
		write_ini_int(lpAppName.c_str(), "Switch", "Group.ini", qq.second.Switch);

		//处理方式
		write_ini_int(lpAppName.c_str(), "Way", "Group.ini", qq.second.Way);

		//处理禁言时间
		write_ini_int(lpAppName.c_str(), "WayTime", "Group.ini", qq.second.WayTime);

		//撤回
		write_ini_int(lpAppName.c_str(), "Revoke", "Group.ini", qq.second.Revoke);

		//强力检测
		write_ini_int(lpAppName.c_str(), "Streng", "Group.ini", qq.second.Streng);

		//触发提醒
		write_ini_int(lpAppName.c_str(), "GroupWarn", "Group.ini", qq.second.GroupWarn);


		//其他配置项

		/*********
		构造全部配置名格式

		如：
		[1164442003]
		config=c1[\n]c2

		************/
		string AloneCfg_str = "";
		for (auto conf : qq.second.Alone)
		{
			//保证添加前一个有分隔符
			if (!AloneCfg_str.empty())
			{
				AloneCfg_str += "[\\n]";
			}

			string qq_child = to_string(qq.first) + "_" + conf.first;
			//配置名全部累加
			AloneCfg_str += conf.first;

			//写入单个配置项
			write_ini_int(qq_child.c_str(), "Switch", "Group.ini", conf.second.Switch);
			write_ini_int(qq_child.c_str(), "Way", "Group.ini", conf.second.Way);
			write_ini_int(qq_child.c_str(), "WayTime", "Group.ini", conf.second.WayTime);
			write_ini_int(qq_child.c_str(), "Revoke", "Group.ini", conf.second.Revoke);
			write_ini_int(qq_child.c_str(), "Streng", "Group.ini", conf.second.Streng);

			//写入关键词
			write_ini_SubStr_WKEYWORD(qq_child.c_str(), "KeyWord", "Group.ini", conf.second.WKeyWord);

			//写入正则表达式关键词
			write_ini_SubStr_string(qq_child.c_str(), "REKeyWord", "Group.ini", conf.second.REKeyWord);

		}
		/*********
		写入全部配置名

		如：
		[1164442003]
		config=c1[\n]c2

		************/
		write_ini_str(lpAppName.c_str(), "Config", "Group.ini", AloneCfg_str.c_str());

	}
	/*********
	写入全部配置名

	如：
	[main]
	GroupList=1164442003[\n]1164442004

	************/
	write_ini_str("main", "GroupList", "Group.ini", qq_str.c_str());

	return true;
}

#ifdef MFC

//ini写入封装--界面

//构造函数
Config_Write_win::Config_Write_win(map<long long, CFG_ALONE_WIN_ALL> new_value, string new_path)
{
	this->path = new_path;

	//拷贝map
	for (auto new_map : new_value)
	{

		for (auto new_value : new_map.second.Alone)
		{

			value[new_map.first].Alone[new_value.first] = new_value.second;

		}

		value[new_map.first].GroupWarn = new_map.second.GroupWarn;
		value[new_map.first].KeyWordWhite = new_map.second.KeyWordWhite;
		value[new_map.first].Revoke = new_map.second.Revoke;
		value[new_map.first].Streng = new_map.second.Streng;
		value[new_map.first].KeyWord = new_map.second.KeyWord;
		value[new_map.first].Switch = new_map.second.Switch;
		value[new_map.first].Way = new_map.second.Way;
		value[new_map.first].WayTime = new_map.second.WayTime;
		value[new_map.first].WhiteList = new_map.second.WhiteList;
		value[new_map.first].WordKeyWarn = new_map.second.WordKeyWarn;
		value[new_map.first].GroupList = new_map.second.GroupList;
		value[new_map.first].REKeyWord = new_map.second.REKeyWord;
	}

	//默认配置项

}

//全部的单独的配置项 界面写入到文件
bool Config_Write_win::write_Alone()
{
	//写入默认监控群列表
	write_ini_SubStr_longlong("0", "GroupList", "Group.ini", value[0].GroupList);

	//写入QQ
	/*********
	构造全部配置名格式

	如：
	[main]
	qq=1164442003[\n]1164442004

	************/
	string qq_str = "";
	for (auto qq : value)
	{
		//保证添加前一个有分隔符
		if (!qq_str.empty())
		{
			qq_str += "[\\n]";
		}

		//配置名
		string lpAppName = to_string(qq.first);

		//配置名全部累加
		qq_str += lpAppName;


		//默认配置项

		//关键词
		CString temp_KeyWord = qq.second.KeyWord;
		write_ini_CString(lpAppName.c_str(), "KeyWord", "Group.ini", temp_KeyWord);

		//正则表达式关键词
		CString temp_REKeyWord = qq.second.REKeyWord;
		write_ini_CString(lpAppName.c_str(), "REKeyWord", "Group.ini", temp_REKeyWord);

		//白名单关键词
		CString temp_KeyWordWhite = qq.second.KeyWordWhite;
		write_ini_CString(lpAppName.c_str(), "KeyWordWhite", "Group.ini", temp_KeyWordWhite);


		//自定义触发提醒
		CString temp_WordKeyWarn = qq.second.WordKeyWarn;
		write_ini_CString(lpAppName.c_str(), "WordKeyWarn", "Group.ini", temp_WordKeyWarn);


		//白名单
		CString temp_WhiteList = qq.second.WhiteList;
		write_ini_CString(lpAppName.c_str(), "WhiteList", "Group.ini", temp_WhiteList);

		//开关
		write_ini_int(lpAppName.c_str(), "Switch", "Group.ini", qq.second.Switch);

		//处理方式
		write_ini_int(lpAppName.c_str(), "Way", "Group.ini", qq.second.Way);

		//处理禁言时间
		write_ini_int(lpAppName.c_str(), "WayTime", "Group.ini", qq.second.WayTime);

		//撤回
		write_ini_int(lpAppName.c_str(), "Revoke", "Group.ini", qq.second.Revoke);

		//强力检测
		write_ini_int(lpAppName.c_str(), "Streng", "Group.ini", qq.second.Streng);

		//触发提醒
		write_ini_int(lpAppName.c_str(), "GroupWarn", "Group.ini", qq.second.GroupWarn);

		//其他单独配置项
		/*********
		构造全部配置名格式

		如：
		[1164442003]
		config=c1[\n]c2

		************/
		string AloneCfg_str = "";
		for (auto conf : qq.second.Alone)
		{
			//保证添加前一个有分隔符
			if (!AloneCfg_str.empty())
			{
				AloneCfg_str += "[\\n]";
			}


			string qq_child = to_string(qq.first) + "_" + conf.first;


			//配置名全部累加
			AloneCfg_str += conf.first;

			//写入单个配置项
			write_ini_int(qq_child.c_str(), "Switch", "Group.ini", conf.second.Switch);
			write_ini_int(qq_child.c_str(), "Way", "Group.ini", conf.second.Way);
			write_ini_int(qq_child.c_str(), "WayTime", "Group.ini", conf.second.WayTime);
			write_ini_int(qq_child.c_str(), "Revoke", "Group.ini", conf.second.Revoke);
			write_ini_int(qq_child.c_str(), "Streng", "Group.ini", conf.second.Streng);
			write_ini_int(qq_child.c_str(), "GroupWarn", "Group.ini", conf.second.GroupWarn);

			//写入关键词
			CString buf_KeyWord = conf.second.KeyWord;
			write_ini_CString(qq_child.c_str(), "KeyWord", "Group.ini", buf_KeyWord);

			//正则表达式写入关键词
			CString buf_REKeyWord = conf.second.REKeyWord;
			write_ini_CString(qq_child.c_str(), "REKeyWord", "Group.ini", buf_REKeyWord);

			//自定义触发提醒
			CString buf_WordKeyWarn = conf.second.WordKeyWarn;
			write_ini_CString(qq_child.c_str(), "WordKeyWarn", "Group.ini", buf_WordKeyWarn);



			/*********
			写入全部配置名

			如：
			[1164442003]
			config=c1[\n]c2

			************/
			write_ini_str(lpAppName.c_str(), "Config", "Group.ini", AloneCfg_str.c_str());
		}

		/*********
		写入全部配置名

		如：
		[main]
		qq=1164442003[\n]1164442004

		************/
		write_ini_str("main", "GroupList", "Group.ini", qq_str.c_str());
	}
	return true;
}


#endif

//ini读取封装--插件

//构造函数
Config_Read_dll::Config_Read_dll(string path)
{
	this->path = path;
}

//读取单个配置名 插件
bool Config_Read_dll::read_AloneOne(long long qq_id, vector<string> &temp_name_cfg)
{
	//读取单独的默认配置项
	string qq_str = to_string(qq_id).c_str();
	//关键词
	vector<string> temp_KeyWord;
	read_ini_SubStr_str(qq_str.c_str(), "KeyWord", "Group.ini", temp_KeyWord);
	for (string keyword : temp_KeyWord)
	{
		WKEYWORD WkeyWord;
		WkeyWord.Word = keyword;
		WkeyWord.WWord = string2wstring(keyword);//存放宽字节字符串
		value[qq_id].WKeyWord.push_back(WkeyWord);
	}

	//正则表达式关键词
	vector<string> temp_REKeyWord;
	read_ini_SubStr_str(qq_str.c_str(), "REKeyWord", "Group.ini", temp_REKeyWord);
	value[qq_id].REKeyWord = temp_REKeyWord;

	//白名单关键词
	temp_KeyWord.clear();
	read_ini_SubStr_str(qq_str.c_str(), "KeyWordWhite", "Group.ini", temp_KeyWord);
	for (string keyword : temp_KeyWord)
	{
		WKEYWORD WkeyWord;
		WkeyWord.Word = keyword;
		WkeyWord.WWord = string2wstring(keyword);//存放宽字节字符串
		value[qq_id].KeyWordWhite.push_back(WkeyWord);
	}

	//自定义触发提醒
	string temp_WordKeyWarn;
	read_ini_str(qq_str.c_str(), "WordKeyWarn", "Group.ini", temp_WordKeyWarn);
	value[qq_id].WordKeyWarn = temp_WordKeyWarn;


	//白名单
	vector<long long> temp_WhiteList;
	read_ini_SubStr_longlong(qq_str.c_str(), "WhiteList", "Group.ini", temp_WhiteList);
	value[qq_id].WhiteList = temp_WhiteList;


	//开关
	int temp_Switch = 0;
	read_ini_int(qq_str.c_str(), "Switch", "Group.ini", temp_Switch);
	value[qq_id].Switch = temp_Switch;


	//处理方式
	int temp_Way = 0;
	read_ini_int(qq_str.c_str(), "Way", "Group.ini", temp_Way);
	value[qq_id].Way = temp_Way;

	//处理禁言时间
	int temp_WayTime = 0;
	read_ini_int(qq_str.c_str(), "WayTime", "Group.ini", temp_WayTime);
	value[qq_id].WayTime = temp_WayTime;

	//撤回
	int temp_Revoke = 0;
	read_ini_int(qq_str.c_str(), "Revoke", "Group.ini", temp_Revoke);
	value[qq_id].Revoke = temp_Revoke;

	//强力检测
	int temp_Streng = 0;
	read_ini_int(qq_str.c_str(), "Streng", "Group.ini", temp_Streng);
	value[qq_id].Streng = temp_Streng;

	//触发提醒
	int temp_GroupWarn = 0;
	read_ini_int(qq_str.c_str(), "GroupWarn", "Group.ini", temp_GroupWarn);
	value[qq_id].GroupWarn = temp_GroupWarn;


	//其他单独配置项
	for (string sub_name : temp_name_cfg)
	{
		CFG_ALONE_DLL temp_cfg;

		string qq_child = to_string(qq_id) + "_" + sub_name;
		//关键词
		vector<string> temp_KeyWord;
		read_ini_SubStr_str(qq_child.c_str(), "KeyWord", "Group.ini", temp_KeyWord);
		for (string keyword : temp_KeyWord)
		{
			WKEYWORD WkeyWord;
			WkeyWord.Word = keyword;
			WkeyWord.WWord = string2wstring(keyword);//存放宽字节字符串
			temp_cfg.WKeyWord.push_back(WkeyWord);
		}

		//正则表达式关键词
		vector<string> temp_REKeyWord;
		read_ini_SubStr_str(qq_child.c_str(), "REKeyWord", "Group.ini", temp_REKeyWord);
		temp_cfg.REKeyWord = temp_REKeyWord;

		//自定义触发提醒
		string temp_WordKeyWarn;
		read_ini_str(qq_child.c_str(), "WordKeyWarn", "Group.ini", temp_WordKeyWarn);
		temp_cfg.WordKeyWarn = temp_WordKeyWarn;


		//开关
		int temp_Switch = 0;
		read_ini_int(qq_child.c_str(), "Switch", "Group.ini", temp_Switch);
		temp_cfg.Switch = temp_Switch;


		//处理方式
		int temp_Way = 0;
		read_ini_int(qq_child.c_str(), "Way", "Group.ini", temp_Way);
		temp_cfg.Way = temp_Way;

		//处理禁言时间
		int temp_WayTime = 0;
		read_ini_int(qq_child.c_str(), "WayTime", "Group.ini", temp_WayTime);
		temp_cfg.WayTime = temp_WayTime;

		//撤回
		int temp_Revoke = 0;
		read_ini_int(qq_child.c_str(), "Revoke", "Group.ini", temp_Revoke);
		temp_cfg.Revoke = temp_Revoke;

		//强力检测
		int temp_Streng = 0;
		read_ini_int(qq_child.c_str(), "Streng", "Group.ini", temp_Streng);
		temp_cfg.Streng = temp_Streng;

		//触发提醒
		int temp_GroupWarn = 0;
		read_ini_int(qq_child.c_str(), "GroupWarn", "Group.ini", temp_GroupWarn);
		temp_cfg.GroupWarn = temp_GroupWarn;

		//存放到容器中
		value[qq_id].Alone[sub_name] = temp_cfg;
	}

	return true;
}

//读取默认群列表
bool Config_Read_dll::Read_DefGroupList()
{
	read_ini_SubStr_longlong("0", "GroupList", "Group.ini", value[0].GroupList);

	return false;
}


#ifdef MFC

//ini读取封装--界面

//构造函数
Config_Read_win::Config_Read_win(string path)
{
	this->path = path;
}

//读取单个配置名 界面
bool Config_Read_win::read_AloneOne(long long qq_id, vector<string> &temp_name_cfg)
{

	string qq_str = to_string(qq_id).c_str();
	//默认配置项

	//关键词
	CString temp_KeyWord = "";
	read_ini_CString(qq_str.c_str(), "KeyWord", "Group.ini", temp_KeyWord);
	value[qq_id].KeyWord = temp_KeyWord;

	//正则表达式关键词
	CString temp_REKeyWord = "";
	read_ini_CString(qq_str.c_str(), "REKeyWord", "Group.ini", temp_REKeyWord);
	value[qq_id].REKeyWord = temp_REKeyWord;

	//白名单关键词
	CString KeyWordWhite = "";
	read_ini_CString(qq_str.c_str(), "KeyWordWhite", "Group.ini", KeyWordWhite);
	value[qq_id].KeyWordWhite = KeyWordWhite;


	//自定义触发提醒
	CString temp_WordKeyWarn;
	read_ini_CString(qq_str.c_str(), "WordKeyWarn", "Group.ini", temp_WordKeyWarn);
	value[qq_id].WordKeyWarn = temp_WordKeyWarn;


	//白名单
	CString temp_WhiteList;
	read_ini_CString(qq_str.c_str(), "WhiteList", "Group.ini", temp_WhiteList);
	value[qq_id].WhiteList = temp_WhiteList;


	//开关
	int temp_Switch = 0;
	read_ini_int(qq_str.c_str(), "Switch", "Group.ini", temp_Switch);
	value[qq_id].Switch = temp_Switch;


	//处理方式
	int temp_Way = 0;
	read_ini_int(qq_str.c_str(), "Way", "Group.ini", temp_Way);
	value[qq_id].Way = temp_Way;

	//处理禁言时间
	int temp_WayTime = 0;
	read_ini_int(qq_str.c_str(), "WayTime", "Group.ini", temp_WayTime);
	value[qq_id].WayTime = temp_WayTime;

	//撤回
	int temp_Revoke = 0;
	read_ini_int(qq_str.c_str(), "Revoke", "Group.ini", temp_Revoke);
	value[qq_id].Revoke = temp_Revoke;

	//强力检测
	int temp_Streng = 0;
	read_ini_int(qq_str.c_str(), "Streng", "Group.ini", temp_Streng);
	value[qq_id].Streng = temp_Streng;

	//触发提醒
	int temp_GroupWarn = 0;
	read_ini_int(qq_str.c_str(), "GroupWarn", "Group.ini", temp_GroupWarn);
	value[qq_id].GroupWarn = temp_GroupWarn;


	//其他单独配置项
	for (string sub_name : temp_name_cfg)
	{
		CFG_ALONE_WIN temp_cfg;

		string qq_child = to_string(qq_id) + "_" + sub_name;

		//关键词
		CString temp_KeyWord = "";
		read_ini_CString(qq_child.c_str(), "KeyWord", "Group.ini", temp_KeyWord);
		temp_cfg.KeyWord = temp_KeyWord;

		//正则表达式关键词
		CString temp_REKeyWord = "";
		read_ini_CString(qq_child.c_str(), "REKeyWord", "Group.ini", temp_REKeyWord);
		temp_cfg.REKeyWord = temp_REKeyWord;

		//自定义触发提醒
		CString temp_WordKeyWarn;
		read_ini_CString(qq_child.c_str(), "WordKeyWarn", "Group.ini", temp_WordKeyWarn);
		temp_cfg.WordKeyWarn = temp_WordKeyWarn;

		//开关
		int temp_Switch = 0;
		read_ini_int(qq_child.c_str(), "Switch", "Group.ini", temp_Switch);
		temp_cfg.Switch = temp_Switch;

		//处理方式
		int temp_Way = 0;
		read_ini_int(qq_child.c_str(), "Way", "Group.ini", temp_Way);
		temp_cfg.Way = temp_Way;

		//处理禁言时间
		int temp_WayTime = 0;
		read_ini_int(qq_child.c_str(), "WayTime", "Group.ini", temp_WayTime);
		temp_cfg.WayTime = temp_WayTime;

		//撤回
		int temp_Revoke = 0;
		read_ini_int(qq_child.c_str(), "Revoke", "Group.ini", temp_Revoke);
		temp_cfg.Revoke = temp_Revoke;

		//强力检测
		int temp_Streng = 0;
		read_ini_int(qq_child.c_str(), "Streng", "Group.ini", temp_Streng);
		temp_cfg.Streng = temp_Streng;

		//触发提醒
		int temp_GroupWarn = 0;
		read_ini_int(qq_child.c_str(), "GroupWarn", "Group.ini", temp_GroupWarn);
		temp_cfg.GroupWarn = temp_GroupWarn;


		//存放到容器中
		value[qq_id].Alone[sub_name] = temp_cfg;

	}

	return true;
}

//读取默认群列表
bool Config_Read_win::Read_DefGroupList()
{
	read_ini_SubStr_longlong("0", "GroupList", "Group.ini", value[0].GroupList);

	return false;
}

#endif










//读取主要配置--插件
Config_Read_main_dll::Config_Read_main_dll(string path)
{
	this->path = path;
}

bool Config_Read_main_dll::read_main()
{
	//私聊消息转发给主人
	value.MsgRelay = read_ini_int("main", "MsgRelay", "Main.ini");

	//读取前缀
	read_ini_str("main", "Prefix", "Main.ini", value.Prefix);

	//主人QQ
	vector<long long> temp_root;

	bool err = read_ini_SubStr_longlong("main", "root", "Main.ini", temp_root);

	if (!err)
	{
		return false;
	}


	value.root = temp_root;

	return true;
}


#ifdef MFC

//读取主要配置 界面

Config_Read_main_win::Config_Read_main_win(string path)
{
	this->path = path;
}

bool Config_Read_main_win::read_main()
{
	//私聊消息转发给主人
	value.MsgRelay = read_ini_int("main", "MsgRelay", "Main.ini");

	//读取前缀
	read_ini_CString("main", "Prefix", "Main.ini", value.Prefix);

	//主人QQ
	CString temp_root;

	bool err = read_ini_CString("main", "root", "Main.ini", temp_root);

	if (!err)
		return false;

	value.root = temp_root;
	return true;
}

#endif

//写入主要配置 插件
Config_Write_main_dll::Config_Write_main_dll(CFG_MAIN_DLL value, string path)
{
	this->path = path;
	this->value = value;
}

bool Config_Write_main_dll::write_main()
{
	//私聊消息转发给主人
	write_ini_int("main", "MsgRelay", "Main.ini", value.MsgRelay);

	//指令前缀
	write_ini_str("main", "Prefix", "Main.ini", value.Prefix);

	//主人QQ
	write_ini_SubStr_longlong("main", "root", "Main.ini", value.root);

	return true;
}


#ifdef MFC

//写入主要配置 界面
Config_Write_main_win::Config_Write_main_win(CFG_MAIN_WIN value, string path)
{
	this->path = path;
	this->value = value;
}

bool Config_Write_main_win::write_main()
{
	//私聊消息转发给主人
	write_ini_int("main", "MsgRelay", "Main.ini", value.MsgRelay);

	//指令前缀
	write_ini_CString("main", "Prefix", "Main.ini", value.Prefix);

	//主人QQ
	write_ini_CString("main", "root", "Main.ini", value.root);

	return true;
}

#endif


#ifdef MFC

map<long long, CFG_ALONE_WIN_ALL> cfg_alone;
CFG_MAIN_WIN cfg_main;

#endif
//
//int main()
//{
//	Config_Read_win a("./test/");
//
//	bool a1 = a.read_Alone();
//
//	map<long long, CFG_ALONE_WIN_ALL> a2(a.value);
//
//
//	cout << "名字" << "0" << endl;
//	cout << "处理方法" << a2[0].Way << endl;
//	cout << "禁言时间" << a2[0].WayTime << endl;
//	cout << "强力检测" << a2[0].Streng << endl;
//	cout << "撤回" << a2[0].Revoke << endl;
//	cout << endl;
//
//	for (auto b : a2[0].Alone)
//	{
//		cout << "名字" << b.first << endl;
//		cout << "处理方法" << b.second.Way << endl;
//		cout << "禁言时间" << b.second.WayTime << endl;
//		cout << "强力检测" << b.second.Streng << endl;
//		cout << "撤回" << b.second.Revoke << endl;
//		cout << endl;
//
//		cout << "关键词"<< b.second.KeyWord.GetBuffer() << endl;
//		/*for (WKEYWORD b2 : b.second.WKeyWord)
//		{
//			cout << b2.Word << endl;
//		}*/
//		cout << endl;
//
//		cout << endl;
//	}
//
//	Config_Write_win a3(a2, "./test2/");
//
//	a3.write_Alone();
//
//	return 0;
//}

