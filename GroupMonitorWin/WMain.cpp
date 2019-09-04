// WMain.cpp: 实现文件
//

#include "stdafx.h"
#include "GroupMonitorWin.h"
#include "WMain.h"
#include "afxdialogex.h"

#include "WAloneList.h"
#include "WAloneConfig.h"
#include "WAllConfig.h"
#include "WMainConfig.h"

#include <string>

#include "CQ.h"

// WMain 对话框

IMPLEMENT_DYNAMIC(WMain, CDialogEx)

WMain::WMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGMAIN, pParent)
{

}

WMain::~WMain()
{
}

void WMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(WMain, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &WMain::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &WMain::OnBnClickedButton2)
	//	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &WMain::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON4, &WMain::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &WMain::OnBnClickedButton3)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &WMain::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON5, &WMain::OnBnClickedButton5)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &WMain::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// WMain 消息处理程序


BOOL WMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//屏幕位置
	int x, y;
	x = GetSystemMetrics(SM_CXSCREEN); //屏幕宽度
	y = GetSystemMetrics(SM_CYSCREEN); //屏幕高度

	SetWindowPos(NULL, x / 3, y / 3, 0, 0, SWP_NOZORDER | SWP_NOSIZE);


	DWORD dwStyle = m_list.GetExtendedStyle();

	//选中某行使整行高亮（只适用于报表风格的listctrl）
	//dwStyle |= LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与报表风格的listctrl）
	//m_list.SetExtendedStyle(dwStyle); //设置扩展风格

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_list.InsertColumn(0, TEXT("群名"), LVCFMT_LEFT, 150);
	m_list.InsertColumn(1, TEXT("群号码"), LVCFMT_LEFT, 100);

	InitData();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void WMain::InitData()
{

	map<long long, std::string> &GroupList = CQ_getGroupList;


	//数据初始化
	m_list.DeleteAllItems();
	//先放置已经开启的群
	int m = 0;
	for (auto alone_all : cfg_alone[0].GroupList)
	{

		CString GroupId_str(to_string(alone_all).c_str());
		CString Name_str(GroupList[alone_all].c_str());

		//存放群名称
		m_list.InsertItem(m, Name_str);

		//存放群号码
		m_list.SetItemText(m, 1, GroupId_str);

		//存放是否启动
		m_list.SetCheck(m, TRUE);

		//删除已经添加过的元素
		GroupList.erase(alone_all);
		m++;
	}

	//放置其他群
	//m = 0;
	for (auto it : GroupList)
	{
		CString strItem(it.second.c_str());
		m_list.InsertItem(m, strItem);

		CString strText = _T("");
		strText.Format(_T("%lld"), it.first);
		m_list.SetItemText(m, 1, strText);

		m_list.SetCheck(m, FALSE);
		m++;

	}
}


//全选
void WMain::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		m_list.SetCheck(i, TRUE);
	}
}


//反选
void WMain::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		if (m_list.GetCheck(i) == FALSE)  //未被选中的
		{
			m_list.SetCheck(i, TRUE);
		}
		else  //选中的
		{
			m_list.SetCheck(i, FALSE);
		}
	}
}


//void WMain::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	CString str;
//
//	str.Format(TEXT("%d %d"), pNMItemActivate->iItem, pNMItemActivate->iSubItem);
//
//	MessageBox(str);
//
//
//	*pResult = 0;
//}


//单独设置
void WMain::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	WAloneList *p = new WAloneList;

	p->Create(IDD_DIALOGALONELIST);
	p->ShowWindow(SW_SHOW);
}


//全局设置
void WMain::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	WAllConfig *p = new WAllConfig;

	p->Create(IDD_DIALOGALLCFG);
	p->ShowWindow(SW_SHOW);

}

//双击
void WMain::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//MessageBox(TEXT("查看详细信息功能 正在努力开发中。。。"));

	*pResult = 0;
}

//主要设置
void WMain::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	WMainConfig *p = new WMainConfig;

	p->Create(IDD_DIALOGMAINCFG);
	p->ShowWindow(SW_SHOW);
}


//列表内容更改
void WMain::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//清空原有数据
	cfg_alone[0].GroupList.clear();
	
	//重新添加数据
	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		if (m_list.GetCheck(i) == TRUE)
		{

			cfg_alone[0].GroupList.push_back(atoll(CStringA(m_list.GetItemText(i, 1)).GetBuffer()));
		}
	}


	*pResult = 0;
}
