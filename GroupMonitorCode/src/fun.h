#include "Configini.h"

#include <vector>
#include <string>

//#include "../../GroupMonitorWin/CQ.h"
using namespace std;


/*˽����Ϣ����*/
class Private
{

public:

	//˽����Ϣ����
	void funthing(const char* msg);


	//��Ϣת��
	void MsgRelayFun(const char* msg);

	//���ù���ִ��
	void funcute(const char* msg);

	//ָ����
	int instruct(const char* msg);

	int64_t fromQQ;

private:
	int x;
	int64_t ReplyGroup;
	int64_t ReplyQQ;
};


/*��Ϣ����*/

class OperateMsg
{

public:

	OperateMsg(int64_t fromQQ, int64_t fromGroup, int64_t msgId, const char* msg);

	wstring string2wstring(string str);

	//���������ڹؼ��ʶԱ�
	bool KeyWordFun(vector<WKEYWORD> KeyWord, bool Streng, string *KeyWordWarn);

	//�����ؼ��ʲ���
	void KeyWrodWarn(std::string& SendMsg_root, int Way, int WayTime, bool Revoke, bool GroupWarn, string &WordKeyWarn, string& KeyWord, string REWord = "");

	bool REKeyKordFun(vector<string> REKeyWord, string *REKeyWordWarn, string *REWord);

	//�Զ��崥���ؼ�������
	void KeyWordWarnMsg(std::string& str, int Way, int WayTime);

	//����������
	bool AloneFun(string alone_name, CFG_ALONE_DLL alone);

	//���ݴ�����
	void funthing();

private:
	int64_t fromQQ;
	int64_t fromGroup;
	string msg;
	int64_t msgId;

	wstring Wmsg;
};