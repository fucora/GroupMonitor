// WMainConfig.cpp: 实现文件
//

#include "stdafx.h"
#include "GroupMonitorWin.h"
#include "WMainConfig.h"
#include "afxdialogex.h"


// WMainConfig 对话框

IMPLEMENT_DYNAMIC(WMainConfig, CDialogEx)

WMainConfig::WMainConfig(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGMAINCFG, pParent)
	, m_root(_T(""))
	, m_MsgRelay(FALSE)
	, m_Prefix(_T(""))
{

}

WMainConfig::~WMainConfig()
{
}

void WMainConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_root);
	DDX_Check(pDX, IDC_CHECK1, m_MsgRelay);
	DDX_Text(pDX, IDC_EDIT2, m_Prefix);
}


BEGIN_MESSAGE_MAP(WMainConfig, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &WMainConfig::OnBnClickedButton4)
END_MESSAGE_MAP()


// WMainConfig 消息处理程序


BOOL WMainConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_MsgRelay = cfg_main.MsgRelay;
	m_root = cfg_main.root;
	m_Prefix = cfg_main.Prefix;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void WMainConfig::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	cfg_main.MsgRelay = m_MsgRelay;
	cfg_main.root = m_root;
	cfg_main.Prefix = m_Prefix;

	MessageBox(TEXT("保存成功"));
	CDialogEx::OnOK();
}
