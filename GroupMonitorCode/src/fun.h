#include "Configini.h"

#include <vector>
#include <string>

//#include "../../GroupMonitorWin/CQ.h"
using namespace std;


/*私聊消息处理*/
class Private
{

public:

	//私聊消息处理
	void funthing(const char* msg);


	//消息转发
	void MsgRelayFun(const char* msg);

	//设置功能执行
	void funcute(const char* msg);

	//指令检测
	int instruct(const char* msg);

	int64_t fromQQ;

private:
	int x;
	int64_t ReplyGroup;
	int64_t ReplyQQ;
};


/*消息处理*/

class OperateMsg
{

public:

	OperateMsg(int64_t fromQQ, int64_t fromGroup, int64_t msgId, const char* msg);

	wstring string2wstring(string str);

	//单个容器内关键词对比
	bool KeyWordFun(vector<WKEYWORD> KeyWord, bool Streng, string *KeyWordWarn);

	//触发关键词操作
	void KeyWrodWarn(std::string& SendMsg_root, int Way, int WayTime, bool Revoke, bool GroupWarn, string &WordKeyWarn, string& KeyWord, string REWord = "");

	bool REKeyKordFun(vector<string> REKeyWord, string *REKeyWordWarn, string *REWord);

	//自定义触发关键词提醒
	void KeyWordWarnMsg(std::string& str, int Way, int WayTime);

	//单独处理函数
	bool AloneFun(string alone_name, CFG_ALONE_DLL alone);

	//内容处理功能
	void funthing();

private:
	int64_t fromQQ;
	int64_t fromGroup;
	string msg;
	int64_t msgId;

	wstring Wmsg;
};