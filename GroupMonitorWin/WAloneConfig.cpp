// WAloneConfig.cpp: 实现文件
//

#include "stdafx.h"
#include "GroupMonitorWin.h"
#include "WAloneConfig.h"
#include "afxdialogex.h"

#include "WAllWord.h"

// WAloneConfig 对话框

IMPLEMENT_DYNAMIC(WAloneConfig, CDialogEx)

WAloneConfig::WAloneConfig(long long GroupId, string name, bool add, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGALONECFG, pParent)
	, m_name(_T(""))
	, m_KeyWord(_T(""))
	, m_Way(FALSE)
	, m_WayTime(0)
	, m_Streng(FALSE)
	, m_Revoke(FALSE)
	, m_GroupWarn(TRUE)
{

	//检测是否为默认设置 如果GroupId为0则为默认设置

	this->GroupId = GroupId;
	this->add = add;
	this->name = name;
}

WAloneConfig::~WAloneConfig()
{
}

void WAloneConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_name);
	DDX_Text(pDX, IDC_EDIT1, m_KeyWord);
	DDX_Radio(pDX, IDC_RADIO1, m_Way);
	DDX_Text(pDX, IDC_EDIT2, m_WayTime);
	DDX_Check(pDX, IDC_CHECK1, m_Streng);
	DDX_Check(pDX, IDC_CHECK2, m_Revoke);
	DDX_Check(pDX, IDC_CHECK3, m_GroupWarn);
	DDX_Control(pDX, IDC_EDIT3, m_CEdit_name);
	DDX_Control(pDX, IDC_EDIT1, m_CEdit_KeyWord);
	DDX_Control(pDX, IDC_RADIO1, m_CButton_Way1);
	DDX_Control(pDX, IDC_EDIT2, m_CEdit_WayTime);
	DDX_Control(pDX, IDC_CHECK1, m_CButton_Streng);
	DDX_Control(pDX, IDC_CHECK2, m_CButton_Revoke);
	DDX_Control(pDX, IDC_CHECK3, m_CButton_GroupWarn);
	DDX_Control(pDX, IDC_BUTTON2, m_CButton_WordKeyWarn);
	DDX_Control(pDX, IDC_BUTTON3, m_CButton_Del);
	DDX_Control(pDX, IDC_BUTTON1, m_CButton_OK);
	DDX_Control(pDX, IDC_RADIO2, m_CButton_Way2);
	DDX_Control(pDX, IDC_RADIO3, m_CButton_Way3);
	DDX_Control(pDX, IDC_RADIO4, m_CButton_Way4);
	DDX_Control(pDX, IDC_BUTTON5, m_CButton_REKeyWord);

}


BEGIN_MESSAGE_MAP(WAloneConfig, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &WAloneConfig::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &WAloneConfig::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &WAloneConfig::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &WAloneConfig::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &WAloneConfig::OnBnClickedButton5)
END_MESSAGE_MAP()


void WAloneConfig::AllEnableWindow(bool t)
{
	m_CEdit_KeyWord.EnableWindow(t);
	m_CButton_Way1.EnableWindow(t);
	m_CButton_Way2.EnableWindow(t);
	m_CButton_Way3.EnableWindow(t);
	m_CButton_Way4.EnableWindow(t);
	m_CEdit_WayTime.EnableWindow(t);
	m_CButton_Streng.EnableWindow(t);
	m_CButton_Revoke.EnableWindow(t);
	m_CButton_GroupWarn.EnableWindow(t);
	m_CButton_WordKeyWarn.EnableWindow(t);
	m_CButton_Del.EnableWindow(t);
	m_CButton_OK.EnableWindow(t);
	m_CButton_REKeyWord.EnableWindow(t);
}

//保存
void WAloneConfig::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	string strname(CStringA(m_name).GetBuffer());

	auto &p = cfg_alone[GroupId].Alone[strname];

	//提醒内容开关
	p.GroupWarn = m_GroupWarn;

	//关键词
	p.KeyWord = m_KeyWord;

	//撤回
	p.Revoke = m_Revoke;

	//强力检测
	p.Streng = m_Streng;

	//处理方式
	p.Way = m_Way;

	//处理 禁言时间
	p.WayTime = m_WayTime;

	if (add)
		p.Switch = true;

	MessageBox(TEXT("保存成功"));

	CDialogEx::OnOK();
}


//自定义提醒内容
void WAloneConfig::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	//创建窗口
	WAllWord *p = new WAllWord(&cfg_alone[GroupId].Alone[string(CStringA(m_name).GetBuffer())].WordKeyWarn);

	//窗口标题获取
	CString WindowsText("自定义提醒内容");

	//打开窗口
	p->Create(IDD_DIALOGALLWORD);
	p->SetWindowTextW(WindowsText);
	p->ShowWindow(SW_SHOW);

	m_GroupWarn = TRUE;
	UpdateData(FALSE);
}


//初始化
BOOL WAloneConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//当点击空白处时
	if (name.empty())
	{
		add = true;

	}

	//获取配置项
	if (!add)
	{
		//如果不是添加 则禁用设置名字文本框
		m_CEdit_name.EnableWindow(FALSE);

		auto &p = cfg_alone[GroupId].Alone[name];

		//name
		m_name = CString(name.c_str());

		//触发提醒开关
		m_GroupWarn = p.GroupWarn;

		//撤回
		m_Revoke = p.Revoke;

		//强力检测
		m_Streng = p.Streng;

		//处理方法
		m_Way = p.Way;

		//禁言时间
		m_WayTime = p.WayTime;

		//关键词
		m_KeyWord = p.KeyWord;

		UpdateData(FALSE);
	}
	else
	{
		//如果是添加 在没有设置配置名前禁用所有项
		AllEnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//删除配置
void WAloneConfig::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	if (MessageBox(TEXT("确定删除此配置"), TEXT("删除配置"), MB_YESNO) == IDYES)
	{
		if (!add)
			cfg_alone[GroupId].Alone.erase(string(CStringA(m_name).GetBuffer()));
	}

	CDialogEx::OnOK();
}


//确定名称
void WAloneConfig::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (string(CStringA(m_name).GetBuffer()).empty())
	{
		MessageBox(TEXT("请先设置配置名"));
		return;
	}

	string strname(CStringA(m_name).GetBuffer());


	if (add)
	{
		//为添加模式后检测设置名称是否重复
		for (auto add_name : cfg_alone[GroupId].Alone)
		{
			if (add_name.first == strname)
			{
				MessageBox(TEXT("设置名称不能重复"));
				return;
			}
		}
	}

	//检测设置名称是否为空
	if (m_name.IsEmpty())
	{
		MessageBox(TEXT("设置名称不能为空"));
		return;
	}

	AllEnableWindow(TRUE);
	m_CEdit_name.EnableWindow(FALSE);
}

//正则表达式
void WAloneConfig::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	//创建窗口
	WAllWord *p = new WAllWord(&cfg_alone[GroupId].Alone[string(CStringA(m_name).GetBuffer())].REKeyWord);

	//窗口标题获取
	CString WindowsText("正则表达式关键词");

	//打开窗口
	p->Create(IDD_DIALOGALLWORD);
	p->SetWindowTextW(WindowsText);
	p->ShowWindow(SW_SHOW);
}
