#include <CQEVE_PrivateMsg.h>
#include <CQcode.h>
#include <CQLogger.h>
#include <CQAPI_EX.h>

#include "fun.h"

#include <string>
#include <time.h>
#include <map>

using namespace std;
using namespace CQ;

//请加上static,表示这个logger只有本cpp有效
static Logger logger("群关键词监控");


long long itime = 0;
map<int64_t, Private> insQQ;

EVE_PrivateMsg(SendMsg) {

	insQQ[fromQQ].fromQQ=fromQQ;
	insQQ[fromQQ].funthing(msg);


	return 0;
}

EVE_GroupMsg(GroupMsg)
{
	OperateMsg a(fromQQ, fromGroup, msgId, msg);

	a.funthing();

	return 0;
}
