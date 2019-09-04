// WAllConfig.cpp: 实现文件
//

#include "stdafx.h"
#include "GroupMonitorWin.h"
#include "WAllConfig.h"
#include "afxdialogex.h"

#include "WDefConfig.h"
#include "WAloneConfig.h"


// WAllConfig 对话框

IMPLEMENT_DYNAMIC(WAllConfig, CDialogEx)

WAllConfig::WAllConfig(long long GroupId, CString GroupName, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGALLCFG, pParent)
{
	this->GroupId = GroupId;
	this->GroupName = GroupName;
}


WAllConfig::~WAllConfig()
{
}

void WAllConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(WAllConfig, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &WAllConfig::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &WAllConfig::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &WAllConfig::OnBnClickedButton3)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &WAllConfig::OnLvnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &WAllConfig::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON4, &WAllConfig::OnBnClickedButton4)
END_MESSAGE_MAP()


// WAllConfig 消息处理程序

//默认设置
void WAllConfig::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//创建窗口
	WDefConfig *p = new WDefConfig(GroupId);

	//窗口标题获取
	CString WindowsText("默认设置 ");
	WindowsText.Append(GroupName);

	//打开窗口
	p->Create(IDD_DIALOGDEFCFG);
	p->SetWindowTextW(WindowsText);
	p->ShowWindow(SW_SHOW);
}


//全选
void WAllConfig::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		m_list.SetCheck(i, TRUE);
	}
}

//初始化
BOOL WAllConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD dwStyle = m_list.GetExtendedStyle();

	//选中某行使整行高亮（只适用于报表风格的listctrl）
	//dwStyle |= LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与报表风格的listctrl）
	//m_list.SetExtendedStyle(dwStyle); //设置扩展风格

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_list.InsertColumn(0, TEXT("设置名称"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(1, TEXT("处理方法"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, TEXT("强力检测"), LVCFMT_LEFT, 70);


	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void WAllConfig::InitData()
{
	m_list.DeleteAllItems();
	//数据初始化
	int m = 0;
	for (auto alone_all : cfg_alone[GroupId].Alone)
	{
		if (alone_all.second.Switch)
		{
			CString strItem(alone_all.first.c_str());
			m_list.InsertItem(m, strItem);

			if (alone_all.second.Way == 0)
			{
				m_list.SetItemText(m, 1, CString("不做处理"));
			}
			else if (alone_all.second.Way == 1)
			{
				CString cstr;
				cstr.Format(TEXT("禁言%d分钟"), alone_all.second.WayTime);
				m_list.SetItemText(m, 1, cstr);
			}
			else if (alone_all.second.Way == 2)
			{
				m_list.SetItemText(m, 1, CString("踢出"));
			}
			else if (alone_all.second.Way == 3)
			{
				m_list.SetItemText(m, 1, CString("踢出并拉黑"));
			}

			if (alone_all.second.Streng)
			{
				m_list.SetItemText(m, 2, CString("是"));
			}
			

			//设置复选框
			m_list.SetCheck(m, true);
			m++;
		}

	}

	for (auto alone_all : cfg_alone[GroupId].Alone)
	{
		if (!alone_all.second.Switch)
		{
			CString strItem(alone_all.first.c_str());
			m_list.InsertItem(m, strItem);

			if (alone_all.second.Way == 0)
			{
				m_list.SetItemText(m, 1, CString("不做处理"));
			}
			else if (alone_all.second.Way == 1)
			{
				m_list.SetItemText(m, 1, CString("禁言"));
			}
			else if (alone_all.second.Way == 2)
			{
				m_list.SetItemText(m, 1, CString("踢出"));
			}
			else if (alone_all.second.Way == 4)
			{
				m_list.SetItemText(m, 1, CString("踢出并拉黑"));
			}

			if (alone_all.second.Streng)
			{
				m_list.SetItemText(m, 2, CString("是"));
			}

			m_list.SetCheck(m, false);
			m++;
		}
	
	}
}

//添加设置
void WAllConfig::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码WAloneConfig

	WAloneConfig p(GroupId, "", true);
	p.DoModal();

	//数据初始化
	InitData();
}

//表格内容已更改
void WAllConfig::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		//获取配置名	
		CString CStr_name;
		CStr_name = m_list.GetItemText(i, 0);

		//将群号码转成string
		CStringA str2(CStr_name);
		string str_name(str2.GetBuffer());

		cfg_alone[GroupId].Alone[str_name].Switch = m_list.GetCheck(i);

	}

	*pResult = 0;
}

//双击
void WAllConfig::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	WAloneConfig p(GroupId, CStringA(m_list.GetItemText(pNMItemActivate->iItem, 0)).GetBuffer(), false);
	p.DoModal();

	//数据初始化
	InitData();

	*pResult = 0;
}

//全不选
void WAllConfig::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		m_list.SetCheck(i, FALSE);
	}
}
