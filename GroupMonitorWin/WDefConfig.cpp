// WDefConfig.cpp: 实现文件
//

#include "stdafx.h"
#include "GroupMonitorWin.h"
#include "WDefConfig.h"
#include "afxdialogex.h"

#include "WAllWord.h"

// WDefConfig 对话框

IMPLEMENT_DYNAMIC(WDefConfig, CDialogEx)

WDefConfig::WDefConfig(long long GroupId, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGDEFCFG, pParent)
	, m_Way(0)
	, m_WayTime(0)
	, m_GroupWarn(TRUE)
	, m_Streng(FALSE)
	, m_Revoke(FALSE)
{
	this->GroupId = GroupId;
}


WDefConfig::~WDefConfig()
{
}

void WDefConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Way);
	DDX_Text(pDX, IDC_EDIT1, m_WayTime);
	DDX_Check(pDX, IDC_CHECK1, m_GroupWarn);
	DDX_Check(pDX, IDC_CHECK3, m_Streng);
	DDX_Check(pDX, IDC_CHECK4, m_Revoke);
}


BEGIN_MESSAGE_MAP(WDefConfig, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &WDefConfig::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &WDefConfig::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &WDefConfig::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &WDefConfig::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &WDefConfig::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &WDefConfig::OnBnClickedButton6)
END_MESSAGE_MAP()


// WDefConfig 消息处理程序

//默认关键词
void WDefConfig::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	//创建窗口
	WAllWord *p = new WAllWord(&cfg_alone[GroupId].KeyWord);

	//窗口标题获取
	CString WindowsText("默认关键词");

	//打开窗口
	p->Create(IDD_DIALOGALLWORD);
	p->SetWindowTextW(WindowsText);
	p->ShowWindow(SW_SHOW);
}

//白名单关键词
void WDefConfig::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	//创建窗口
	WAllWord *p = new WAllWord(&cfg_alone[GroupId].KeyWordWhite);

	//窗口标题获取
	CString WindowsText("白名单关键词");

	//打开窗口
	p->Create(IDD_DIALOGALLWORD);
	p->SetWindowTextW(WindowsText);
	p->ShowWindow(SW_SHOW);
}

//白名单QQ
void WDefConfig::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	//创建窗口
	WAllWord *p = new WAllWord(&cfg_alone[GroupId].WhiteList);

	//窗口标题获取
	CString WindowsText("白名单QQ");

	//打开窗口
	p->Create(IDD_DIALOGALLWORD);
	p->SetWindowTextW(WindowsText);
	p->ShowWindow(SW_SHOW);

}

//打开自定义触发提醒
void WDefConfig::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	//创建窗口
	WAllWord *p = new WAllWord(&cfg_alone[GroupId].WordKeyWarn);

	//窗口标题获取
	CString WindowsText("自定义触发关键词提醒");

	//打开窗口
	p->Create(IDD_DIALOGALLWORD);
	p->SetWindowTextW(WindowsText);
	p->ShowWindow(SW_SHOW);
}

//保存
void WDefConfig::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	//触发关键词提醒
	cfg_alone[GroupId].GroupWarn = m_GroupWarn;

	//撤回
	cfg_alone[GroupId].Revoke = m_Revoke;

	//强力检测
	cfg_alone[GroupId].Streng = m_Streng;

	//处理方法
	cfg_alone[GroupId].Way = m_Way;

	//禁言时间
	cfg_alone[GroupId].WayTime = m_WayTime;


	MessageBox(TEXT("保存成功"));
	CDialogEx::OnOK();
}


BOOL WDefConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//触发关键词提醒
	m_GroupWarn = cfg_alone[GroupId].GroupWarn;

	//撤回
	m_Revoke = cfg_alone[GroupId].Revoke;

	//强力检测
	m_Streng = cfg_alone[GroupId].Streng;

	//处理方法
	m_Way = cfg_alone[GroupId].Way;

	//禁言时间
	m_WayTime = cfg_alone[GroupId].WayTime;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//正则表达式关键词
void WDefConfig::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码

	//创建窗口
	WAllWord *p = new WAllWord(&cfg_alone[GroupId].REKeyWord);

	//窗口标题获取
	CString WindowsText("正则表达式关键词");

	//打开窗口
	p->Create(IDD_DIALOGALLWORD);
	p->SetWindowTextW(WindowsText);
	p->ShowWindow(SW_SHOW);
}
