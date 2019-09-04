//// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
////
#include "pch.h"
#include <map>
#include <string>
#include <Windows.h>
#include <iostream>

using namespace std;

string AppFile = "D:/用户文档/桌面/桌面整理/酷Q/data/app/cn.xiaoxiaoge.GroupMonitor/";


//map<long long, std::string> getGroupList()
//{
//	map<long long, std::string> a;
//	a[1164442003] = "xxxx";
//	a[1164442004] = "xxx";
//	a[1164442005] = "xxx";
//	a[1164442006] = "xxx";
//	return a;
//}

void CQ_getGroupList_fun(const char *fun)
{
	map<long long, std::string> CQ_getGroupList;
	char GroupId[20];
	char GroupName[100];
	char temp_str[10240];
	char temp_str2[10240];

	strcpy(temp_str2, fun);
	while (1)
	{
		char *p1 = strstr(temp_str2, "[1\\n]");

		if (p1 == NULL)
			break;
		strcpy(temp_str, p1 + strlen("[1\\n]"));
		strcpy(temp_str2, temp_str);

		char *p2 = strstr(temp_str, "[2\\n]");

		if (p2 == NULL)
			break;
		*p2 = 0;

		strcpy(GroupId, temp_str);

		p1 = strstr(temp_str2, "[2\\n]");

		if (p1 == NULL)
			break;
		strcpy(temp_str, p1 + strlen("[2\\n]"));
		strcpy(temp_str2, temp_str);

		p2 = strstr(temp_str, "[1\\n]");

		if (p2 != NULL)
			*p2 = 0;

		strcpy(GroupName, temp_str);

		CQ_getGroupList[atoll(GroupId)] = GroupName;
	}

	for (auto Group : CQ_getGroupList)
	{
		cout << "Group[" << Group.first << "] = " << Group.second << endl;
	}
}

void CQ_getGroupList_charp(char *fun)
{
	map<long long, std::string> a;
	a[1164442003] = "xxxx";
	a[1164442004] = "xxx";
	a[1164442005] = "xxx";
	a[1164442006] = "xxx";

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

int main()
{
	cout << "测试" << endl;

	char fun[10240];
	CQ_getGroupList_charp(fun);
	//CQ_getGroupList_fun(fun);


	/*DllFun a;
	a.getGroupList = getGroupList;*/
	typedef void(*Pwinfun)(const char* Path, const char* fun);
	HINSTANCE hDLL;
	Pwinfun win;
	hDLL = LoadLibrary(L"GroupMonitorWin.dll"); //加载动态链接库MyDll.dll文件；
	if (hDLL == NULL) {
		MessageBoxA(NULL, "界面打开失败，可能是你没有复制界面dll\n有问题欢迎到QQ群：839067703 求助", "错误", NULL);
		//logger.Info("界面打开失败");
		return -1;
	}
	win = (Pwinfun)GetProcAddress(hDLL, "winfun");

	win(AppFile.c_str(), fun);

	return 0;
}