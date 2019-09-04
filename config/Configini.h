#pragma once

#define MFC
#ifdef  MFC

#include <afx.h>

#else

#include <Windows.h>

#endif //  MFC


//数据结构
#include <string>
#include <map>
#include <Set>
#include <vector>

using namespace std;

//关键词带宽字节
struct WKEYWORD
{
	string Word;
	wstring WWord;
};

#ifdef  MFC

//单独的配置项 界面
struct CFG_ALONE_WIN
{
	bool Switch;//开关
	//string Name;//处理方式的名字


	int Way;//处理方法
	int WayTime;//禁言时间
	bool Revoke;//撤回
	bool Streng;//强力检测
	bool GroupWarn;//触发提醒

	CString KeyWord;//关键词字符串
	CString WordKeyWarn;//自定义触发关键词提醒
	CString REKeyWord;//正则表达式关键词
};

#endif

//单独的配置项 插件
struct CFG_ALONE_DLL
{
	bool Switch;//开关
	//string Name;//处理方式的名字

	int Way;//处理方法
	int WayTime;//禁言时间
	bool Revoke;//撤回
	bool Streng;//强力检测
	bool GroupWarn;//触发提醒

	vector<WKEYWORD> WKeyWord;//关键词带宽字节 方便插件使用
	vector<string> REKeyWord;//正则表达式关键词
	string WordKeyWarn;//自定义触发关键词提醒
};

struct CFG_ALONE_DLL_ALL
{
	map<string, CFG_ALONE_DLL> Alone;

	bool Switch;//群监控开关
	bool GroupWarn;//触发关键词提醒
	int Way;//处理方法
	int WayTime;//禁言时间
	bool Revoke;//撤回
	bool Streng;//强力检测

	vector<long long>GroupList;//默认监控群
	vector<long long> WhiteList;//白名单
	vector<WKEYWORD> WKeyWord;//关键词带宽字节 方便插件使用
	vector<WKEYWORD> KeyWordWhite;//关键词白名单
	string WordKeyWarn;//自定义触发关键词提醒
	vector<string> REKeyWord;//正则表达式关键词
};


#ifdef  MFC

struct CFG_ALONE_WIN_ALL
{
	map<string, CFG_ALONE_WIN> Alone;

	bool Switch;//群监控开关
	bool GroupWarn;//触发关键词提醒
	int Way;//处理方法
	int WayTime;//禁言时间
	bool Revoke;//撤回
	bool Streng;//强力检测

	vector<long long>GroupList;//默认监控群
	CString WhiteList;//白名单
	CString KeyWord;//关键词字符串
	CString KeyWordWhite;//关键词白名单
	CString WordKeyWarn;//自定义触发关键词提醒
	CString REKeyWord;//正则表达式关键词
};

#endif

struct CFG_MAIN_DLL
{
	bool MsgRelay;//消息转发给主人
	string Prefix;//指令前缀

	vector<long long> root;//主人QQ
};


#ifdef  MFC

struct CFG_MAIN_WIN
{
	bool MsgRelay;//消息转发给主人
	CString Prefix;//指令前缀

	CString root;//主人QQ
};

#endif

//写入配置项父类
class Config_Write_father
{

protected:
	//ini配置项写入--字符串
	bool write_ini_str(const char* lpAppName, const char* lpKeyName, const char* lpFileName, string conf_value, bool replace);

#ifdef MFC
	//ini配置项写入--CString
	bool write_ini_CString(const char* lpAppName, const char* lpKeyName, const char* lpFileName, CString conf_value);
#endif

	//ini配置项写入--int
	bool write_ini_int(const char* lpAppName, const char* lpKeyName, const char* lpFileName, int conf_value);

	//字符串替换 \n转[\\n]
	void write_replace_line(string &str);

	//写入列表 WKEYWORD
	bool write_ini_SubStr_WKEYWORD(const char* lpAppName, const char* lpKeyName, const char* lpFileName, vector<WKEYWORD> word);

	//写入列表 string
	bool write_ini_SubStr_string(const char* lpAppName, const char* lpKeyName, const char* lpFileName, vector<string> word);

	//写入列表 longlong
	bool write_ini_SubStr_longlong(const char* lpAppName, const char* lpKeyName, const char* lpFileName, vector<long long> QQList);

protected:
	string path;
};

//读取配置项 父类
class Config_Read_father
{

public:
	//全部的单独的配置项
	bool read_Alone();

private:

	virtual bool read_AloneOne(long long qq_id, vector<string> &temp_name_cfg);

	virtual bool Read_DefGroupList();

protected:
	//string 转 wstring
	wstring string2wstring(string str);

	/*
	const char* lpAppName, 欲在其中查找条目的节名称。如设为NULL，就在lpReturnedString缓冲区内装载这个ini文件所有节的名字。
	const char* lpKeyName, 欲获取的项名或条目名。如设为NULL，就在lpReturnedString缓冲区内装载指定小节所有项的列表
	const char* lpFileName, ini文件的名字。如没有指定一个完整路径名，windows就在Windows目录中查找文件
	string &conf_value, 接收读取的字符串
	*/
	//ini配置项读取--字符串
	bool read_ini_str(const char* lpAppName, const char* lpKeyName, const char* lpFileName, string &conf_value, bool replace);

#ifdef MFC
	//ini读取配置项 CString
	bool read_ini_CString(const char* lpAppName, const char* lpKeyName, const char* lpFileName, CString &conf_value);
#endif

	//解析以[\\n]分割的字符串
	bool read_ini_SubStr_str(const char* lpAppName, const char* lpKeyName, const char* lpFileName, vector<string> &value);

	//解析以[\\n]分割的longlong
	bool read_ini_SubStr_longlong(const char* lpAppName, const char* lpKeyName, const char* lpFileName, vector<long long> &value);

	//ini配置项读取--int---1
	int read_ini_int(const char* lpAppName, const char*lpKeyName, const char* lpFileName, int &conf_value);

	//ini配置项读取--int---2
	int read_ini_int(const char* lpAppName, const char*lpKeyName, const char* lpFileName);

	//字符串替换 [\\n]转\n
	void read_replace_line(string &str);

protected:
	string path;
};


//ini配置写入封装--插件
class Config_Write_dll :public Config_Write_father
{

public:
	//构造函数
	Config_Write_dll(map<long long, CFG_ALONE_DLL_ALL> new_value_dll, string new_path);

public:
	//全部的单独的配置项 插件写入到文件
	bool write_Alone();

public:
	map<long long, CFG_ALONE_DLL_ALL> value;
};


#ifdef  MFC

//ini配置写入封装--界面
class Config_Write_win :public Config_Write_father
{
public:
	//构造函数
	Config_Write_win(map<long long, CFG_ALONE_WIN_ALL> new_value, string new_path);


public:
	//全部的单独的配置项 界面写入到文件
	bool write_Alone();

public:
	map<long long, CFG_ALONE_WIN_ALL> value;

};

#endif

//ini配置读取封装---插件
class Config_Read_dll :public Config_Read_father
{

public:

	//构造函数
	Config_Read_dll(string path);

private:
	//读取单个配置名 插件
	virtual bool read_AloneOne(long long qq_id, vector<string> &temp_name_cfg);

	virtual bool Read_DefGroupList();

public:
	map<long long, CFG_ALONE_DLL_ALL> value;

};


#ifdef  MFC

//ini配置读取封装--界面
class Config_Read_win :public Config_Read_father
{

public:

	//构造函数
	Config_Read_win(string path);

private:
	//读取单个配置名 界面
	virtual bool read_AloneOne(long long qq_id, vector<string> &temp_name_cfg);

	virtual bool Read_DefGroupList();

public:
	map<long long, CFG_ALONE_WIN_ALL> value;

};


#endif

//读取主要配置
class Config_Read_main_dll :public Config_Read_father
{

public:
	Config_Read_main_dll(string path);

	virtual bool read_main();

public:
	CFG_MAIN_DLL value;
};


#ifdef  MFC

//读取主要配置
class Config_Read_main_win :public Config_Read_father
{

public:
	Config_Read_main_win(string path);

	virtual bool read_main();

public:
	CFG_MAIN_WIN value;
};

#endif

//写入主要配置
class Config_Write_main_dll :public Config_Write_father
{

public:
	Config_Write_main_dll(CFG_MAIN_DLL value, string path);

	virtual bool write_main();

public:
	CFG_MAIN_DLL value;
};


#ifdef  MFC

//写入主要配置
class Config_Write_main_win :public Config_Write_father
{

public:
	Config_Write_main_win(CFG_MAIN_WIN value, string path);

	virtual bool write_main();

public:
	CFG_MAIN_WIN value;

};

#endif


#ifdef  MFC

extern map<long long, CFG_ALONE_WIN_ALL> cfg_alone;
extern CFG_MAIN_WIN cfg_main;

#endif