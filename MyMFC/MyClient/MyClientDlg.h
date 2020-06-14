
// MyClientDlg.h: 头文件
//

#pragma once


// CMyClientDlg 对话框
class CMyClientDlg : public CDialogEx
{
// 构造
public:
	CMyClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
private:
	SOCKET s;
	SOCKADDR_IN addr;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedDiscon();
protected:
	afx_msg LRESULT OnSocket(WPARAM wParam, LPARAM lParam);
};
