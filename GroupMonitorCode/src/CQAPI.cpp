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

//�����static,��ʾ���loggerֻ�б�cpp��Ч
static Logger logger("Ⱥ�ؼ��ʼ��");


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
