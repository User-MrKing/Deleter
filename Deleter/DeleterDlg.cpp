
// DeleterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Deleter.h"
#include "DeleterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//  设置数据
BOOL ACheck = TRUE, BCheck = TRUE;

//  线程函数
DWORD WINAPI GetHistory(LPVOID lpParameter);

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


// CDeleterDlg 对话框



CDeleterDlg::CDeleterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DELETER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDeleterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DELETE, v_btnDelete);
	DDX_Control(pDX, IDC_LIST_VIEW, v_lstView);
	DDX_Control(pDX, IDC_EDIT_COUNT, editCount);
}

BEGIN_MESSAGE_MAP(CDeleterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDeleterDlg::OnBnClickedButtonDelete)
	ON_UPDATE_COMMAND_UI(ID_MenuViewA, &CDeleterDlg::OnUpdateMenuViewA)
	ON_UPDATE_COMMAND_UI(ID_MenuViewB, &CDeleterDlg::OnUpdateMenuViewB)
	ON_COMMAND(ID_DevAbout, &CDeleterDlg::OnDevAbout)
END_MESSAGE_MAP()


// CDeleterDlg 消息处理程序

BOOL CDeleterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	v_lstView.SetExtendedStyle(v_lstView.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	v_lstView.InsertColumn(0, TEXT("编号"), 0, 40);
	v_lstView.InsertColumn(1, TEXT("类型"), 0, 70);
	v_lstView.InsertColumn(2, TEXT("路径"), 0, 490);

	v_lstView.DeleteAllItems();
	HANDLE hThread = CreateThread(NULL, 0, GetHistory, this, 0, NULL);

	CloseHandle(hThread);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDeleterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDeleterDlg::OnPaint()
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
HCURSOR CDeleterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

struct ROW
{
	CString num;
	CString path;
};

void CDeleterDlg::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
}

DWORD WINAPI GetHistory(LPVOID lpParameter)
{
	CDeleterDlg *dlg = (CDeleterDlg*)lpParameter;
	HKEY hKey = NULL;
	TCHAR *lpszSubKey = _T("S-1-5-21-454856390-3436349137-2182817502-1001\\SOFTWARE\\Microsoft\\VisualStudio\\14.0\\MRUItems\\{01235aad-8f1b-429f-9d02-61a0101ea275}\\Items");
	LONG lRet = RegOpenKeyEx(HKEY_USERS, lpszSubKey, 0, KEY_READ, &hKey);
	if (lRet == ERROR_SUCCESS) {
		int nFile = 0, nProject = 0, nSolution = 0;
		if (ACheck)
		{
			DWORD dwValueCount = 0, maxValueNameLen = 0, maxValueDataLen = 0;
			lRet = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &dwValueCount, &maxValueNameLen, &maxValueDataLen, NULL, NULL);
			if (lRet == ERROR_SUCCESS) {
				DWORD dwNameLen = maxValueNameLen + 1;
				TCHAR *pszName = new TCHAR[dwNameLen];

				DWORD dwType = 0;
				DWORD dwValueDataLen = maxValueDataLen + 1;
				BYTE *lpValueData = new BYTE[dwValueDataLen];

				for (DWORD dwIndex = 0; dwIndex < dwValueCount; ++dwIndex) {
					dwNameLen = maxValueNameLen + 1;
					ZeroMemory(pszName, dwNameLen);

					dwValueDataLen = maxValueDataLen + 1;
					ZeroMemory(lpValueData, dwValueDataLen);

					lRet = RegEnumValue(hKey, dwIndex, pszName, &dwNameLen, NULL, &dwType, lpValueData, &dwValueDataLen);

					CString strValueName;
					strValueName.Format(_T("%s"), pszName);

					CString strValueData;
					strValueData.Format(_T("%s"), lpValueData);

					strValueData = strValueData.Left(strValueData.Find(_T("|")));

					int t = dlg->v_lstView.GetItemCount();
					nFile++;
					dlg->v_lstView.InsertItem(t, strValueName);
					dlg->v_lstView.SetItemText(t, 1, _T("文件"));
					dlg->v_lstView.SetItemText(t, 2, strValueData);
				}
				delete[] pszName;
				delete[] lpValueData;
				RegCloseKey(hKey);
			}
		}
		if (BCheck)
		{
			HKEY hKey = NULL;
			TCHAR *lpszSubKey = _T("S-1-5-21-454856390-3436349137-2182817502-1001\\SOFTWARE\\Microsoft\\VisualStudio\\14.0\\MRUItems\\{a9c4a31f-f9cb-47a9-abc0-49ce82d0b3ac}\\Items");
			LONG lRet = RegOpenKeyEx(HKEY_USERS, lpszSubKey, 0, KEY_READ, &hKey);
			if (lRet == ERROR_SUCCESS) {
				DWORD dwValueCount = 0, maxValueNameLen = 0, maxValueDataLen = 0;
				lRet = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &dwValueCount, &maxValueNameLen, &maxValueDataLen, NULL, NULL);
				if (lRet == ERROR_SUCCESS) {
					DWORD dwNameLen = maxValueNameLen + 1;
					TCHAR *pszName = new TCHAR[dwNameLen];

					DWORD dwType = 0;
					DWORD dwValueDataLen = maxValueDataLen + 1;
					BYTE *lpValueData = new BYTE[dwValueDataLen];

					for (DWORD dwIndex = 0; dwIndex < dwValueCount; ++dwIndex) {
						dwNameLen = maxValueNameLen + 1;
						ZeroMemory(pszName, dwNameLen);

						dwValueDataLen = maxValueDataLen + 1;
						ZeroMemory(lpValueData, dwValueDataLen);

						lRet = RegEnumValue(hKey, dwIndex, pszName, &dwNameLen, NULL, &dwType, lpValueData, &dwValueDataLen);

						CString strValueName;
						strValueName.Format(_T("%s"), pszName);

						CString strValueData;
						strValueData.Format(_T("%s"), lpValueData);

						strValueData = strValueData.Left(strValueData.Find(_T("|")));

						int t = dlg->v_lstView.GetItemCount();
						CString type;
						if (strValueData.Find(_T(".sln")) != -1)
						{
							type = _T("解决方案");
							nSolution++;
						}
						else
						{
							type = _T("项目");
							nProject++;
						}
						dlg->v_lstView.InsertItem(t, strValueName);
						dlg->v_lstView.SetItemText(t, 1, type);
						dlg->v_lstView.SetItemText(t, 2, strValueData);
					}
					delete[] pszName;
					delete[] lpValueData;
					RegCloseKey(hKey);
				}
			}
		}
		CString txt;
		txt.Format(_T("文件数: %d   项目数: %d   解决方案数: %d"), nFile, nProject, nSolution);
		dlg->SetDlgItemText(IDC_EDIT_COUNT, txt);
	}
	return 0;
}



void CDeleterDlg::OnUpdateMenuViewA(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	ACheck = !ACheck;
	v_lstView.DeleteAllItems();

	HANDLE hThread = CreateThread(NULL, 0, GetHistory, this, 0, NULL);
	CloseHandle(hThread);

	//pCmdUI->SetCheck(ACheck);
	pCmdUI->SetRadio(ACheck);
}


void CDeleterDlg::OnUpdateMenuViewB(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	BCheck = !BCheck;
	v_lstView.DeleteAllItems();

	HANDLE hThread = CreateThread(NULL, 0, GetHistory, this, 0, NULL);
	CloseHandle(hThread);

	//pCmdUI->SetCheck(ACheck);
	CMenu* menu = GetMenu()->GetSubMenu(1);
	menu->CheckMenuItem(ID_MenuViewB, MF_UNCHECKED);
}


void CDeleterDlg::OnDevAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlg;
	dlg.DoModal();
}
