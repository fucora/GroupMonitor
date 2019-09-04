#pragma once


// WAllConfig 对话框

class WAllConfig : public CDialogEx
{
	DECLARE_DYNAMIC(WAllConfig)

public:
	WAllConfig(long long GroupId = 0, CString GroupName = NULL, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WAllConfig();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGALLCFG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

public:
	long long GroupId;
	CString GroupName;
	void InitData();

	afx_msg void OnBnClickedButton2();
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton4();
};
