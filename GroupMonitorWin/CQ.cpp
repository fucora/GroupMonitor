#include "CQ.h"

map<long long, std::string> CQ_getGroupList;

void CQ_getGroupList_fun(const char *fun)
{
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
}