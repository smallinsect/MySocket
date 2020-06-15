
// MyServerDlg.h: 头文件
//

#pragma once


// CMyServerDlg 对话框
class CMyServerDlg : public CDialogEx
{
// 构造
public:
	CMyServerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	SOCKET s;//服务器的套接字
	SOCKADDR_IN addr;//服务器信息

	SOCKET s1;//客户端的套接字
	SOCKADDR_IN addr1;//客户端信息

	int n;

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
	afx_msg void OnBnClickedSend();
protected:
	afx_msg LRESULT OnSocket(WPARAM wParam, LPARAM lParam);
};
