#pragma once


// WAloneConfig 对话框

class WAloneConfig : public CDialogEx
{
	DECLARE_DYNAMIC(WAloneConfig)

public:
	WAloneConfig(long long GroupId = 0, string name = "", bool add = false, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WAloneConfig();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGALONECFG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	long long GroupId;
	bool add;
	string name;
	void AllEnableWindow(bool t);

	CString m_name;
	CString m_KeyWord;
	BOOL m_Way;
	int m_WayTime;
	BOOL m_Streng;
	BOOL m_Revoke;
	afx_msg void OnBnClickedButton1();
	BOOL m_GroupWarn;
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton3();
	CEdit m_CEdit_name;
	afx_msg void OnBnClickedButton4();
	CEdit m_CEdit_KeyWord;
	CButton m_CButton_Way1;
	CEdit m_CEdit_WayTime;
	CButton m_CButton_Streng;
	CButton m_CButton_Revoke;
	CButton m_CButton_GroupWarn;
	CButton m_CButton_WordKeyWarn;
	CButton m_CButton_Del;
	CButton m_CButton_OK;
	CButton m_CButton_Way2;
	CButton m_CButton_Way3;
	CButton m_CButton_Way4;
	afx_msg void OnBnClickedButton5();
	CButton m_CButton_REKeyWord;
};
