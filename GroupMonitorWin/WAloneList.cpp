// WAloneList.cpp: 实现文件
//

#include "stdafx.h"
#include "GroupMonitorWin.h"
#include "WAloneList.h"
#include "afxdialogex.h"

#include "WAllConfig.h"

// WAloneList 对话框

IMPLEMENT_DYNAMIC(WAloneList, CDialogEx)

WAloneList::WAloneList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGALONELIST, pParent)
{
}


WAloneList::~WAloneList()
{
}

void WAloneList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(WAloneList, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &WAloneList::OnNMDblclkList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &WAloneList::OnLvnItemchangedList1)
	//	ON_NOTIFY(LVN_ODSTATECHANGED, IDC_LIST1, &WAloneList::OnLvnOdstatechangedList1)
END_MESSAGE_MAP()


// WAloneList 消息处理程序


BOOL WAloneList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	DWORD dwStyle = m_list.GetExtendedStyle();

	//选中某行使整行高亮（只适用于报表风格的listctrl）
	//dwStyle |= LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与报表风格的listctrl）
	//m_list.SetExtendedStyle(dwStyle); //设置扩展风格

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_list.InsertColumn(0, TEXT("群名"), LVCFMT_LEFT, 170);
	m_list.InsertColumn(1, TEXT("群号码"), LVCFMT_LEFT, 100);


	//数据初始化

	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void WAloneList::InitData()
{

	map<long long, std::string> &GroupList = CQ_getGroupList;

	m_list.DeleteAllItems();
	//先放置已经开启的群
	int m = 0;
	for (auto alone_all : cfg_alone)
	{
		if (alone_all.second.Switch)
		{

			CString GroupId_str;
			GroupId_str.Format(TEXT("%lld"), alone_all.first);

			CString Name_str(GroupList[alone_all.first].c_str());

			//存放群名称
			m_list.InsertItem(m, Name_str);

			//存放群号码
			m_list.SetItemText(m, 1, GroupId_str);

			//存放是否启动
			m_list.SetCheck(m, alone_all.second.Switch);

			m++;
		}
	}

	//放置其他群
	//m = 0;
	for (auto it : GroupList)
	{
		//排除已经开启的群
		if (cfg_alone[it.first].Switch)
		{

			//MessageBox(CString(to_string(it.first).c_str()));  //for test
			continue;
		}

		CString strItem(it.second.c_str());
		m_list.InsertItem(m, strItem);

		CString strText = _T("");
		strText.Format(_T("%lld"), it.first);
		m_list.SetItemText(m, 1, strText);

		m_list.SetCheck(m, cfg_alone[it.first].Switch);
		m++;

	}
}


void WAloneList::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取群号码
	CString str;
	str = m_list.GetItemText(pNMItemActivate->iItem, 1);

	//将群号码转成longlong
	CStringA str2(str);
	long long GroupId = atoll(str2.GetBuffer());

	//获取群名称
	CString GroupName;
	GroupName = m_list.GetItemText(pNMItemActivate->iItem, 0);

	//创建窗口
	WAllConfig *p = new WAllConfig(GroupId, GroupName);

	//窗口标题获取
	CString WindowsText("单独设置 ");
	WindowsText.Append(GroupName);

	//打开窗口
	p->Create(IDD_DIALOGALLCFG);
	p->SetWindowTextW(WindowsText);
	p->ShowWindow(SW_SHOW);

	//编辑后自动勾选
	m_list.SetCheck(pNMItemActivate->iItem, TRUE);

	*pResult = 0;
}


//项目内容更改后
void WAloneList::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		//获取群号码
		CString str;
		str = m_list.GetItemText(i, 1);

		//将群号码转成string
		CStringA str2(str);

		long long GroupId = atoll(str2.GetBuffer());

		cfg_alone[GroupId].Switch = m_list.GetCheck(i);

	}

	*pResult = 0;
}


//void WAloneList::OnLvnOdstatechangedList1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLVODSTATECHANGE pStateChanged = reinterpret_cast<LPNMLVODSTATECHANGE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//
//	MessageBox(TEXT("TEST2"));
//	*pResult = 0;
//}
