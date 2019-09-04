#pragma once
#include <map>

using namespace std;
//
//struct DllFun
//{
//	map<long long, std::string>(*getGroupList)();
//};

void CQ_getGroupList_fun(const char *fun);

#ifdef MFC
extern map<long long, std::string> CQ_getGroupList;
#endif