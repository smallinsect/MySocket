
// MyClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MyClient.h"
#include "MyClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SOCKET WM_USER+100//自定义消息

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


// CMyClientDlg 对话框



CMyClientDlg::CMyClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CLOSE, &CMyClientDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_SEND, &CMyClientDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_CONNECT, &CMyClientDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_DISCON, &CMyClientDlg::OnBnClickedDiscon)
	ON_MESSAGE(WM_SOCKET, &CMyClientDlg::OnSocket)
END_MESSAGE_MAP()


// CMyClientDlg 消息处理程序

BOOL CMyClientDlg::OnInitDialog()
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

	GetDlgItem(IDC_CLOSE)->EnableWindow(false);
	GetDlgItem(IDC_SEND)->EnableWindow(false);
	GetDlgItem(IDC_DISCON)->EnableWindow(false);

	// 创建套接字
	s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyClientDlg::OnPaint()
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
HCURSOR CMyClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyClientDlg::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMyClientDlg::OnBnClickedSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str1;
	GetDlgItem(IDC_EDIT_SEND)->GetWindowText(str1);
	send(s, (const char*)str1.GetBuffer(1), str1.GetLength(), 0);
}


void CMyClientDlg::OnBnClickedConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str1, str2;
	GetDlgItem(IDC_EDIT_IP)->GetWindowText(str1);
	GetDlgItem(IDC_EDIT_PORT)->GetWindowText(str2);
	if (str1 == "" || str2 == "") {
		MessageBox(TEXT("服务器地址和端口不能为NULL"));
		return;
	}
	int port = atoi((const char *)str2.GetBuffer(1));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	GetDlgItem(IDC_EDIT_MSG)->SetWindowText(TEXT("正在连接服务器...\r\n"));
	if (::connect(s, (PSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR) {
		int n = ::WSAGetLastError();
		GetDlgItem(IDC_EDIT_MSG)->GetWindowText(str1);
		str1 += "服务器连接失败...请重新连接...\r\n";
		GetDlgItem(IDC_EDIT_MSG)->SetWindowText(str1);
	}
	else {
		// 将IDC_EDIT_MSG控件设为读取socket消息。
		::WSAAsyncSelect(s, this->m_hWnd, WM_SOCKET, FD_READ);
		int n = ::WSAGetLastError();
		GetDlgItem(IDC_EDIT_MSG)->GetWindowText(str1);
		str1 += "服务器连接成功...\r\n";
		GetDlgItem(IDC_EDIT_MSG)->SetWindowText(str1);
	}
}


void CMyClientDlg::OnBnClickedDiscon()
{
	// TODO: 在此添加控件通知处理程序代码
}


afx_msg LRESULT CMyClientDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	if (lParam == FD_READ) {
		char cs[1024] = "";
		recv(s, cs, 1024, 0);
		CString str;
		GetDlgItem(IDC_EDIT_MSG)->GetWindowText(str);
		str += "\r\n服务器说：";
		str += (LPTSTR)cs; 
		GetDlgItem(IDC_EDIT_MSG)->SetWindowText(str);
	}
	return 0;
}
