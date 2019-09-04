#pragma once

#define MFC
#ifdef  MFC

#include <afx.h>

#else

#include <Windows.h>

#endif //  MFC


//���ݽṹ
#include <string>
#include <map>
#include <Set>
#include <vector>

using namespace std;

//�ؼ��ʴ����ֽ�
struct WKEYWORD
{
	string Word;
	wstring WWord;
};

#ifdef  MFC

//������������ ����
struct CFG_ALONE_WIN
{
	bool Switch;//����
	//string Name;//����ʽ������


	int Way;//������
	int WayTime;//����ʱ��
	bool Revoke;//����
	bool Streng;//ǿ�����
	bool GroupWarn;//��������

	CString KeyWord;//�ؼ����ַ���
	CString WordKeyWarn;//�Զ��崥���ؼ�������
	CString REKeyWord;//������ʽ�ؼ���
};

#endif

//������������ ���
struct CFG_ALONE_DLL
{
	bool Switch;//����
	//string Name;//����ʽ������

	int Way;//������
	int WayTime;//����ʱ��
	bool Revoke;//����
	bool Streng;//ǿ�����
	bool GroupWarn;//��������

	vector<WKEYWORD> WKeyWord;//�ؼ��ʴ����ֽ� ������ʹ��
	vector<string> REKeyWord;//������ʽ�ؼ���
	string WordKeyWarn;//�Զ��崥���ؼ�������
};

struct CFG_ALONE_DLL_ALL
{
	map<string, CFG_ALONE_DLL> Alone;

	bool Switch;//Ⱥ��ؿ���
	bool GroupWarn;//�����ؼ�������
	int Way;//������
	int WayTime;//����ʱ��
	bool Revoke;//����
	bool Streng;//ǿ�����

	vector<long long>GroupList;//Ĭ�ϼ��Ⱥ
	vector<long long> WhiteList;//������
	vector<WKEYWORD> WKeyWord;//�ؼ��ʴ����ֽ� ������ʹ��
	vector<WKEYWORD> KeyWordWhite;//�ؼ��ʰ�����
	string WordKeyWarn;//�Զ��崥���ؼ�������
	vector<string> REKeyWord;//������ʽ�ؼ���
};


#ifdef  MFC

struct CFG_ALONE_WIN_ALL
{
	map<string, CFG_ALONE_WIN> Alone;

	bool Switch;//Ⱥ��ؿ���
	bool GroupWarn;//�����ؼ�������
	int Way;//������
	int WayTime;//����ʱ��
	bool Revoke;//����
	bool Streng;//ǿ�����

	vector<long long>GroupList;//Ĭ�ϼ��Ⱥ
	CString WhiteList;//������
	CString KeyWord;//�ؼ����ַ���
	CString KeyWordWhite;//�ؼ��ʰ�����
	CString WordKeyWarn;//�Զ��崥���ؼ�������
	CString REKeyWord;//������ʽ�ؼ���
};

#endif

struct CFG_MAIN_DLL
{
	bool MsgRelay;//��Ϣת��������
	string Prefix;//ָ��ǰ׺

	vector<long long> root;//����QQ
};


#ifdef  MFC

struct CFG_MAIN_WIN
{
	bool MsgRelay;//��Ϣת��������
	CString Prefix;//ָ��ǰ׺

	CString root;//����QQ
};

#endif

//д���������
class Config_Write_father
{

protected:
	//ini������д��--�ַ���
	bool write_ini_str(const char* lpAppName, const char* lpKeyName, const char* lpFileName, string conf_value, bool replace);

#ifdef MFC
	//ini������д��--CString
	bool write_ini_CString(const char* lpAppName, const char* lpKeyName, const char* lpFileName, CString conf_value);
#endif

	//ini������д��--int
	bool write_ini_int(const char* lpAppName, const char* lpKeyName, const char* lpFileName, int conf_value);

	//�ַ����滻 \nת[\\n]
	void write_replace_line(string &str);

	//д���б� WKEYWORD
	bool write_ini_SubStr_WKEYWORD(const char* lpAppName, const char* lpKeyName, const char* lpFileName, vector<WKEYWORD> word);

	//д���б� string
	bool write_ini_SubStr_string(const char* lpAppName, const char* lpKeyName, const char* lpFileName, vector<string> word);

	//д���б� longlong
	bool write_ini_SubStr_longlong(const char* lpAppName, const char* lpKeyName, const char* lpFileName, vector<long long> QQList);

protected:
	string path;
};

//��ȡ������ ����
class Config_Read_father
{

public:
	//ȫ���ĵ�����������
	bool read_Alone();

private:

	virtual bool read_AloneOne(long long qq_id, vector<string> &temp_name_cfg);

	virtual bool Read_DefGroupList();

protected:
	//string ת wstring
	wstring string2wstring(string str);

	/*
	const char* lpAppName, �������в�����Ŀ�Ľ����ơ�����ΪNULL������lpReturnedString��������װ�����ini�ļ����нڵ����֡�
	const char* lpKeyName, ����ȡ����������Ŀ��������ΪNULL������lpReturnedString��������װ��ָ��С����������б�
	const char* lpFileName, ini�ļ������֡���û��ָ��һ������·������windows����WindowsĿ¼�в����ļ�
	string &conf_value, ���ն�ȡ���ַ���
	*/
	//ini�������ȡ--�ַ���
	bool read_ini_str(const char* lpAppName, const char* lpKeyName, const char* lpFileName, string &conf_value, bool replace);

#ifdef MFC
	//ini��ȡ������ CString
	bool read_ini_CString(const char* lpAppName, const char* lpKeyName, const char* lpFileName, CString &conf_value);
#endif

	//������[\\n]�ָ���ַ���
	bool read_ini_SubStr_str(const char* lpAppName, const char* lpKeyName, const char* lpFileName, vector<string> &value);

	//������[\\n]�ָ��longlong
	bool read_ini_SubStr_longlong(const char* lpAppName, const char* lpKeyName, const char* lpFileName, vector<long long> &value);

	//ini�������ȡ--int---1
	int read_ini_int(const char* lpAppName, const char*lpKeyName, const char* lpFileName, int &conf_value);

	//ini�������ȡ--int---2
	int read_ini_int(const char* lpAppName, const char*lpKeyName, const char* lpFileName);

	//�ַ����滻 [\\n]ת\n
	void read_replace_line(string &str);

protected:
	string path;
};


//ini����д���װ--���
class Config_Write_dll :public Config_Write_father
{

public:
	//���캯��
	Config_Write_dll(map<long long, CFG_ALONE_DLL_ALL> new_value_dll, string new_path);

public:
	//ȫ���ĵ����������� ���д�뵽�ļ�
	bool write_Alone();

public:
	map<long long, CFG_ALONE_DLL_ALL> value;
};


#ifdef  MFC

//ini����д���װ--����
class Config_Write_win :public Config_Write_father
{
public:
	//���캯��
	Config_Write_win(map<long long, CFG_ALONE_WIN_ALL> new_value, string new_path);


public:
	//ȫ���ĵ����������� ����д�뵽�ļ�
	bool write_Alone();

public:
	map<long long, CFG_ALONE_WIN_ALL> value;

};

#endif

//ini���ö�ȡ��װ---���
class Config_Read_dll :public Config_Read_father
{

public:

	//���캯��
	Config_Read_dll(string path);

private:
	//��ȡ���������� ���
	virtual bool read_AloneOne(long long qq_id, vector<string> &temp_name_cfg);

	virtual bool Read_DefGroupList();

public:
	map<long long, CFG_ALONE_DLL_ALL> value;

};


#ifdef  MFC

//ini���ö�ȡ��װ--����
class Config_Read_win :public Config_Read_father
{

public:

	//���캯��
	Config_Read_win(string path);

private:
	//��ȡ���������� ����
	virtual bool read_AloneOne(long long qq_id, vector<string> &temp_name_cfg);

	virtual bool Read_DefGroupList();

public:
	map<long long, CFG_ALONE_WIN_ALL> value;

};


#endif

//��ȡ��Ҫ����
class Config_Read_main_dll :public Config_Read_father
{

public:
	Config_Read_main_dll(string path);

	virtual bool read_main();

public:
	CFG_MAIN_DLL value;
};


#ifdef  MFC

//��ȡ��Ҫ����
class Config_Read_main_win :public Config_Read_father
{

public:
	Config_Read_main_win(string path);

	virtual bool read_main();

public:
	CFG_MAIN_WIN value;
};

#endif

//д����Ҫ����
class Config_Write_main_dll :public Config_Write_father
{

public:
	Config_Write_main_dll(CFG_MAIN_DLL value, string path);

	virtual bool write_main();

public:
	CFG_MAIN_DLL value;
};


#ifdef  MFC

//д����Ҫ����
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