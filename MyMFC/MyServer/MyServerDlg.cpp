
// MyServerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MyServer.h"
#include "MyServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SOCKET WM_USER+100

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyServerDlg 对话框



CMyServerDlg::CMyServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, &CMyServerDlg::OnBnClickedSend)
	ON_MESSAGE(WM_SOCKET, &CMyServerDlg::OnSocket)
END_MESSAGE_MAP()


// CMyServerDlg 消息处理程序

BOOL CMyServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	WSADATA wd;
	::WSAStartup(MAKEWORD(2, 2), &wd);

	// 创建套接字
	s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) {
		n = ::WSAGetLastError();
	}
	// 初始化信息
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	//addr.sin_addr.s_addr = inet_addr("0.0.0.0"); 
	addr.sin_addr.s_addr = INADDR_ANY;
	// 绑定信息
	if (::bind(s, (PSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR) {
		n = ::WSAGetLastError();
	}
	// 监听服务器套接字
	if (::listen(s, 5) == SOCKET_ERROR) {
		n = ::WSAGetLastError();
	}
	// 将服务端的socket设为异步
	::WSAAsyncSelect(s, this->m_hWnd, WM_SOCKET, FD_ACCEPT | FD_READ);

	GetDlgItem(IDC_EDIT_MSG)->SetWindowText(TEXT("服务器开启成功"));

	n = 0;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMyServerDlg::OnBnClickedSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_EDIT_SEND, str);
	
	::send(s1, (const char *)str.GetBuffer(), str.GetLength(), 0);

	GetDlgItemText(IDC_EDIT_MSG, str);
	str += "发送成功...";
	SetDlgItemText(IDC_EDIT_MSG, str);
	AfxMessageBox(str);
}


afx_msg LRESULT CMyServerDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	CString str;
	char cs[1024] = "";
	switch (lParam) {
	case FD_ACCEPT:
	{
		int addrlen = sizeof(addr1);
		s1 = ::accept(s, (PSOCKADDR)&addr1, &addrlen);
		n = n + 1;
		str.Format(TEXT("有%d个客户端连接... %s:%d\r\n"), 
			n, ::inet_ntoa(addr1.sin_addr), ::ntohs(addr1.sin_port));
		GetDlgItem(IDC_EDIT_MSG)->SetWindowText(str);
		break;
	}
	case FD_READ:
	{
		::recv(s1, cs, 1024, 0);
		GetDlgItem(IDC_EDIT_MSG)->GetWindowText(str);
		str += cs;
		GetDlgItem(IDC_EDIT_MSG)->SetWindowText(str);
		break;
	}
	}
	return 0;
}
