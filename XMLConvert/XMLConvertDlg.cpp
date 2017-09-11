
// XMLConvertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XMLConvert.h"
#include "XMLConvertDlg.h"
#include "afxdialogex.h"
#include <stdexcept>
#include <iostream>  

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CXMLConvertDlg dialog



CXMLConvertDlg::CXMLConvertDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CXMLConvertDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXMLConvertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, ControlInputDir);
	DDX_Control(pDX, IDC_EDIT2, ControlOutPutDir);
	DDX_Control(pDX, IDC_EDIT3, ControlConvertConfig);
}

BEGIN_MESSAGE_MAP(CXMLConvertDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CXMLConvertDlg::OnBnClickedOk)
	//ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CXMLConvertDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CXMLConvertDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CXMLConvertDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CXMLConvertDlg message handlers

BOOL CXMLConvertDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	//this->DragAcceptFiles(TRUE);
	//ChangeWindowMessageFilterEx(this->GetSafeHwnd(), WM_DROPFILES, MSGFLT_ALLOW, NULL);
	//ChangeWindowMessageFilterEx(this->GetSafeHwnd(), 0x0049, MSGFLT_ALLOW, NULL);//��䲻�꣬����û������

	m_szInputDir = new char[MAX_PATH];
	m_szOutPutDir = new char[MAX_PATH];
	m_szConvertConfigPath = new char[MAX_PATH];

	m_vecArrayPoint.clear();

	// ���س��������ļ�
	OnLoadSystemConfig();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXMLConvertDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (WM_DROPFILES == lParam)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXMLConvertDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXMLConvertDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CXMLConvertDlg::OnBnClickedOk()
{
	// ����ת�������ļ�
	OnLoadConvertConfig();
	// ִ��XMLת��
	DoXMLConvert();
	//CDialogEx::OnOK();

	// �ͷ��ڴ�
	FreeMemory();
}
void CXMLConvertDlg::DoXMLConvert()
{
	// �ж�����Ŀ¼�Ƿ����
	CString strInputDir(m_szInputDir);
	BOOL bRet = CheckFolderExist(strInputDir);
	if (FALSE == bRet)
	{
		CString strError1(_T("����Ŀ¼�����ڣ�"));
		MessageBox(strError1 + strInputDir, _T("����"));
		exit(0);
	}

	// �������Ŀ¼
	SYSTEMTIME st;
	CString strTime;
	GetLocalTime(&st);
	strTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	CString strOutPutDir(m_szOutPutDir);
	CString strRealOutPutDir = strOutPutDir + strTime;

	INT nLimit = 100;
	while ((PathIsDirectory(strRealOutPutDir)) && nLimit > 0)
	{
		strRealOutPutDir = strRealOutPutDir + _T("P");
		nLimit--;
	}

	if (!CreateDirectory(strRealOutPutDir, NULL))
	{
		CString strError1(_T("�������Ŀ¼ʧ�ܣ�"));
		MessageBox(strError1 + strRealOutPutDir, _T("����"));
		exit(0);
	}

	// ��������·���µ������ļ�
	WIN32_FIND_DATA  wfd;
	bool rValue = FALSE;
	CString strFind = strInputDir + _T("\\*");
	HANDLE hFind = FindFirstFile(strFind, &wfd);
	bRet = TRUE;
	while (INVALID_HANDLE_VALUE != hFind && TRUE == bRet)
	{
		CString strFileName = (wfd.cFileName);
		CString strExt = strFileName.Right(3).MakeLower();
		if (strExt == _T("xml"))
		{
			// ��xml�ļ�ִ��ת��
			DoOneXMLConvert(strInputDir + _T("\\") + strFileName, strFileName, strRealOutPutDir);
		}
		bRet = FindNextFile(hFind, &wfd);
	}
	FindClose(hFind);

	CString strMsg(_T("ת���ɹ������Ŀ¼Ϊ��"));
	MessageBox(strMsg + +strRealOutPutDir, _T("�ɹ�"));
}

void CXMLConvertDlg::DoOneXMLConvert(const CString& strPath, const CString& strFileName, const CString& strRealOutPutDir)
{
	// �ж����Ŀ¼�Ƿ����
	CString strOutPutDir(m_szOutPutDir);
	BOOL bRet = CheckFolderExist(strOutPutDir);
	if (FALSE == bRet)
	{
		CString strError1(_T("���Ŀ¼�����ڣ�"));
		MessageBox(strError1 + strOutPutDir, _T("����"));
		exit(0);
	}

	//// ���������ļ�����Ŀ¼
	//CString strNewPath = strOutPutDir + _T("\\") + strFileName;
	//BOOL bCopy = CopyFile(strPath, strNewPath, FALSE);
	//if (FALSE == bCopy)
	//{
	//	CString strError1(_T("�����ļ�ʧ�ܣ�"));
	//	MessageBox(strError1 + strNewPath, _T("����"));
	//	return;
	//}

	int nlen = WideCharToMultiByte(CP_ACP, 0, strPath, -1, NULL, 0, NULL, NULL);
	char *pTemp = new char[nlen + 1];
	WideCharToMultiByte(CP_ACP, 0, strPath, -1, pTemp, nlen, NULL, NULL);

	try
	{
		// ����
		rapidxml::file<> fConfig(pTemp);
		rapidxml::xml_document<> docSource;
		docSource.parse<0>(fConfig.data());

		// ���ڵ�
		rapidxml::xml_node<>* pRoot = docSource.first_node();

		if (NULL == pRoot)
		{
			CString strError(_T("xml���ڵ����:"));
			MessageBox(strError + strPath, _T("����"));
		}

		// ���������ӽڵ� ��������
		INT nCount = 0;
		for (rapidxml::xml_node<>* pSubNodeForRead = pRoot->first_node("Image");
			pSubNodeForRead; pSubNodeForRead = pSubNodeForRead->next_sibling("Image"))
		{
			nCount = nCount + 1;
			CString strCount;
			strCount.Format(_T("%d"), nCount);
			CString strError1 = strPath + _T("�е�") + strCount + _T("���ڵ�");
			rapidxml::xml_attribute<> *attr = pSubNodeForRead->first_attribute("Name");
			if (NULL == attr)
			{
				CString strError2(_T(":�Ҳ�������[Name]��"));
				MessageBox(strError1 + strError2, _T("����"));
				continue;
			}
			CString strName(attr->value());
			StructConvertConfigNode sConfig; 
			BOOL bRet = m_MapConvertConfig.Lookup(strName, sConfig);

			strError1 = strError1 + strName;

			if (TRUE == bRet)
			{
				// ��ԭʼ����
				attr = pSubNodeForRead->first_attribute("XPos");
				if (NULL == attr)
				{
					CString strError2(_T(":�Ҳ�������[XPos]��"));
					MessageBox(strError1 + strError2, _T("����"));
					continue;
				}
				INT nXPos = atoi(attr->value());

				attr = pSubNodeForRead->first_attribute("YPos");
				if (NULL == attr)
				{
					CString strError2(_T(":�Ҳ�������[YPos]��"));
					MessageBox(strError1 + strError2, _T("����"));
					continue;
				}
				INT nYPos = atoi(attr->value());

				attr = pSubNodeForRead->first_attribute("Width");
				if (NULL == attr)
				{
					CString strError2(_T(":�Ҳ�������[Width]��"));
					MessageBox(strError1 + strError2, _T("����"));
					continue;
				}
				INT nWidth = atoi(attr->value());

				attr = pSubNodeForRead->first_attribute("Height");
				if (NULL == attr)
				{
					CString strError2(_T(":�Ҳ�������[Height]��"));
					MessageBox(strError1 + strError2, _T("����"));
					continue;
				}
				INT nHeight = atoi(attr->value());

				// ת�� �Ź���ͼ
				if (CONVERT_TYPE_TO_8 == sConfig.m_Type || CONVERT_TYPE_TO_9 == sConfig.m_Type)
				{
					// ������Ч���ж�
					if (sConfig.m_XOffset1 <= 0 || sConfig.m_YOffset1 <= 0)
					{
						CString strError2(_T(":sConfig.m_XOffset1 <= 0 || sConfig.m_YOffset1 <= 0"));
						MessageBox(strError1 + strError2, _T("����"));
						continue;
					}

					if (sConfig.m_XOffset2 >= nWidth || sConfig.m_YOffset2 >= nHeight)
					{
						CString strError2(_T(":sConfig.m_XOffset2 >= nWidth || sConfig.m_YOffset2 >= nHeight"));
						MessageBox(strError1 + strError2, _T("����"));
						continue;
					}

					if (sConfig.m_XOffset1 >= sConfig.m_XOffset2 || sConfig.m_YOffset1 >= sConfig.m_YOffset2)
					{
						CString strError2(_T(":sConfig.m_XOffset1 >= sConfig.m_XOffset2 || sConfig.m_YOffset1 >= sConfig.m_YOffset2"));
						MessageBox(strError1 + strError2, _T("����"));
						continue;
					}

					// �Ҳ�
					CString strNewName = strName + _T("_R");
					AddNewNode(docSource, pRoot, pSubNodeForRead, strNewName,
						nXPos + sConfig.m_XOffset2, nYPos + sConfig.m_YOffset1, 
						nWidth - sConfig.m_XOffset2, sConfig.m_YOffset2 - sConfig.m_YOffset1);
					nCount = nCount - 1;

					// ���
					strNewName = strName + _T("_L");
					AddNewNode(docSource, pRoot, pSubNodeForRead, strNewName,
						nXPos , nYPos + sConfig.m_YOffset1,
						sConfig.m_XOffset1, sConfig.m_YOffset2 - sConfig.m_YOffset1);
					nCount = nCount - 1;

					// �м�
					if (CONVERT_TYPE_TO_9 == sConfig.m_Type)
					{
						strNewName = strName + _T("_M");
						AddNewNode(docSource, pRoot, pSubNodeForRead, strNewName,
							nXPos + sConfig.m_XOffset1, nYPos + sConfig.m_YOffset1,
							sConfig.m_XOffset2 - sConfig.m_XOffset1, sConfig.m_YOffset2 - sConfig.m_YOffset1);
						nCount = nCount - 1;
					}

					// �ײ�
					strNewName = strName + _T("_B");
					AddNewNode(docSource, pRoot, pSubNodeForRead, strNewName,
						nXPos + sConfig.m_XOffset1, nYPos + sConfig.m_YOffset2,
						sConfig.m_XOffset2 - sConfig.m_XOffset1, nHeight - sConfig.m_YOffset2);
					nCount = nCount - 1;

					// ����
					strNewName = strName + _T("_T");
					AddNewNode(docSource, pRoot, pSubNodeForRead, strNewName,
						nXPos + sConfig.m_XOffset1, nYPos ,
						sConfig.m_XOffset2 - sConfig.m_XOffset1, sConfig.m_YOffset1);
					nCount = nCount - 1;

					// ����
					strNewName = strName + _T("_RB");
					AddNewNode(docSource, pRoot, pSubNodeForRead, strNewName,
						nXPos + sConfig.m_XOffset2, nYPos + sConfig.m_YOffset2,
						nWidth - sConfig.m_XOffset2, nHeight - sConfig.m_YOffset2);
					nCount = nCount - 1;

					// ����
					strNewName = strName + _T("_LB");
					AddNewNode(docSource, pRoot, pSubNodeForRead, strNewName,
						nXPos, nYPos + sConfig.m_YOffset2,
						sConfig.m_XOffset1, nHeight - sConfig.m_YOffset2);
					nCount = nCount - 1;

					// ����
					strNewName = strName + _T("_RT");
					AddNewNode(docSource, pRoot, pSubNodeForRead, strNewName,
						nXPos + sConfig.m_XOffset2, nYPos,
						nWidth - sConfig.m_XOffset2, sConfig.m_YOffset1);
					nCount = nCount - 1;

					// ����
					strNewName = strName + _T("_LT");
					AddNewNode(docSource, pRoot, pSubNodeForRead, strNewName,
						nXPos, nYPos,
						sConfig.m_XOffset1, sConfig.m_YOffset1);
					nCount = nCount - 1;
				}

				// ת�� ��ֱ�и��3��
				if (CONVERT_TYPE_TO_3_VERTICAL == sConfig.m_Type)
				{
					// ������Ч���ж�
					if (sConfig.m_XOffset1 <= 0)
					{
						CString strError2(_T(":sConfig.m_XOffset1 <= 0"));
						MessageBox(strError1 + strError2, _T("����"));
						continue;
					}

					if (sConfig.m_XOffset2 >= nWidth)
					{
						CString strError2(_T(":sConfig.m_XOffset2 >= nWidth"));
						MessageBox(strError1 + strError2, _T("����"));
						continue;
					}

					if (sConfig.m_XOffset1 >= sConfig.m_XOffset2)
					{
						CString strError2(_T(":sConfig.m_XOffset1 >= sConfig.m_XOffset2"));
						MessageBox(strError1 + strError2, _T("����"));
						continue;
					}

					// �Ҳ�
					CString strNewName = strName + _T("_R");
					AddNewNode(docSource, pRoot, pSubNodeForRead, strNewName,
						nXPos + sConfig.m_XOffset2, nYPos,
						nWidth - sConfig.m_XOffset2, nHeight);
					nCount = nCount - 1;

					// �м�
					strNewName = strName + _T("_M");
					AddNewNode(docSource, pRoot, pSubNodeForRead, strNewName,
						nXPos + sConfig.m_XOffset1, nYPos,
						sConfig.m_XOffset2 - sConfig.m_XOffset1, nHeight);
					nCount = nCount - 1;

					// ���
					strNewName = strName + _T("_L");
					AddNewNode(docSource, pRoot, pSubNodeForRead, strNewName,
						nXPos, nYPos ,
						sConfig.m_XOffset1, nHeight);
					nCount = nCount - 1;
				}

				// ת�� ˮƽ�и��3��
				if (CONVERT_TYPE_TO_3_HORIZONTAL == sConfig.m_Type)
				{
					// ������Ч���ж�
					if (sConfig.m_YOffset1 <= 0)
					{
						CString strError2(_T(":sConfig.m_YOffset1 <= 0"));
						MessageBox(strError1 + strError2, _T("����"));
						continue;
					}

					if (sConfig.m_YOffset2 >= nHeight)
					{
						CString strError2(_T(":sConfig.m_YOffset2 >= nHeight"));
						MessageBox(strError1 + strError2, _T("����"));
						continue;
					}

					if (sConfig.m_YOffset1 >= sConfig.m_YOffset2)
					{
						CString strError2(_T(":sConfig.m_YOffset1 >= sConfig.m_YOffset2"));
						MessageBox(strError1 + strError2, _T("����"));
						continue;
					}

					// �ײ�
					CString strNewName = strName + _T("_B");
					AddNewNode(docSource, pRoot, pSubNodeForRead, strNewName,
						nXPos , nYPos + sConfig.m_YOffset2,
						nWidth, nHeight - sConfig.m_YOffset2);
					nCount = nCount - 1;

					// �м�
					strNewName = strName + _T("_M");
					AddNewNode(docSource, pRoot, pSubNodeForRead, strNewName,
						nXPos, nYPos + sConfig.m_YOffset1,
						nWidth, sConfig.m_YOffset2 - sConfig.m_YOffset1);
					nCount = nCount - 1;

					// ����
					strNewName = strName + _T("_T");
					AddNewNode(docSource, pRoot, pSubNodeForRead, strNewName,
						nXPos, nYPos,
						nWidth, sConfig.m_YOffset1);
					nCount = nCount - 1;
				}
			}
		}

		rapidxml::xml_node<>* rot = docSource.allocate_node(rapidxml::node_pi,
			docSource.allocate_string("xml version='1.0'"));
		docSource.prepend_node(rot);

		// ���xml
		CString strNewPath = strRealOutPutDir + _T("\\") + strFileName;
		std::ofstream out(strNewPath);
		out << docSource;

		//char buffer[4096];
		//char *end = rapidxml::print(buffer, docSource, 0);
		//*end = 0;

		//ofstream ofs("testxxx.xml");
		//ofs.write(buffer, strlen(buffer));
		//ofs.close();
	}
	catch (runtime_error e)
	{
		CString strError1(_T("����xml����"));
		CString strError2(e.what());
		MessageBox(strError1 + + strError2, _T("����"));
		return;
	}
}

void CXMLConvertDlg::AddNewNode(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* pNodeMother, rapidxml::xml_node<>* pNodeInsert,
	const CString& strName, INT nValue1, INT nValue2, INT nValue3, INT nValue4)
{
	// �����µĽڵ�
	rapidxml::xml_node<>* pNode = NULL;
	pNode = doc.allocate_node(rapidxml::node_element, "Image");

	rapidxml::xml_node<>* pRealInsertNode = pNodeInsert->next_sibling();
	if (NULL == pRealInsertNode)
	{
		pNodeMother->append_node(pNode);
	}
	else
	{
		pNodeMother->insert_node(pRealInsertNode, pNode);
	}

	int nlen = WideCharToMultiByte(CP_ACP, 0, strName, -1, NULL, 0, NULL, NULL);
	char *pTemp = new char[nlen + 1];
	WideCharToMultiByte(CP_ACP, 0, strName, -1, pTemp, nlen, NULL, NULL);

	rapidxml::xml_attribute<>* pAttr = doc.allocate_attribute("Name", pTemp);
	pNode->append_attribute(pAttr);

	char* pBuf1 = new char[MAX_PATH];
	::sprintf_s(pBuf1, MAX_PATH, "%d", nValue1);
	pAttr = doc.allocate_attribute("XPos", pBuf1);
	pNode->append_attribute(pAttr);

	char* pBuf2 = new char[MAX_PATH];
	::sprintf_s(pBuf2, MAX_PATH, "%d", nValue2);
	pAttr = doc.allocate_attribute("YPos", pBuf2);
	pNode->append_attribute(pAttr);

	char* pBuf3 = new char[MAX_PATH];
	::sprintf_s(pBuf3, MAX_PATH, "%d", nValue3);
	pAttr = doc.allocate_attribute("Width", pBuf3);
	pNode->append_attribute(pAttr);

	char* pBuf4 = new char[MAX_PATH];
	::sprintf_s(pBuf4, MAX_PATH, "%d", nValue4);
	pAttr = doc.allocate_attribute("Height", pBuf4);
	pNode->append_attribute(pAttr);

	m_vecArrayPoint.push_back(pBuf1);
	m_vecArrayPoint.push_back(pBuf2);
	m_vecArrayPoint.push_back(pBuf3);
	m_vecArrayPoint.push_back(pBuf4);

	//char buffer[4096];
	//char *end = rapidxml::print(buffer, doc, 0);
	//*end = 0;
	//ofstream ofs("testxxx.xml");
	//ofs.write(buffer, strlen(buffer));
	//ofs.close();

}

void CXMLConvertDlg::OnLoadSystemConfig()
{
	BOOL bRet = SystemConfigOp(OP_TYPE_READ, "InputPath", m_szInputDir);
	if (FALSE == bRet)
	{
		exit(0);
	}
	CString strInputDir(m_szInputDir);
	ControlInputDir.SetWindowTextW(strInputDir);

	bRet = SystemConfigOp(OP_TYPE_READ, "OutputPath", m_szOutPutDir);
	if (FALSE == bRet)
	{
		exit(0);
	}
	CString strOutPutDir(m_szOutPutDir);
	ControlOutPutDir.SetWindowTextW(strOutPutDir);

	bRet = SystemConfigOp(OP_TYPE_READ, "ConvertConfig", m_szConvertConfigPath);
	if (FALSE == bRet)
	{
		exit(0);
	}
	CString strConvertConfig(m_szConvertConfigPath);
	ControlConvertConfig.SetWindowTextW(strConvertConfig);
}

BOOL CXMLConvertDlg::SystemConfigOp(const OpType& eOpType, const char* szNodeName, char* szNodeValue)
{
	try
	{
		// ����
		rapidxml::file<> fConfig("./SystemConfig.xml");
		rapidxml::xml_document<> docConfig;
		docConfig.parse<0>(fConfig.data());

		// ���ڵ�
		rapidxml::xml_node<>* pRoot = docConfig.first_node();

		if (NULL == pRoot)
		{
			CString strError(_T("��Чϵͳ�����ļ�����xml��"));
			MessageBox(strError, _T("����"));
			return FALSE;
		}

		// ��ȡҪ��д�Ľڵ�
		rapidxml::xml_node<>* pNodeToOp = pRoot->first_node(szNodeName);

		if (NULL == pNodeToOp)
		{
			CString strError1(_T("ϵͳ�����ļ������Ҳ����ڵ㣺"));
			CString strError2(szNodeName);
			MessageBox(strError1 + strError2, _T("����"));
			return FALSE;
		}

		if (OP_TYPE_READ == eOpType)
		{
			strncpy_s(szNodeValue, MAX_PATH, pNodeToOp->value(), MAX_PATH);
			//szNodeValue = pNodeToOp->value();
		}

		if (OP_TYPE_WRITE == eOpType)
		{
			pNodeToOp->value(szNodeValue);
		}

	}
	catch (runtime_error e)
	{
		CString strError1(_T("ϵͳ�����ļ�����"));
		CString strError2(e.what());
		MessageBox(strError1 + strError2, _T("����"));
		return FALSE;
	}

	return TRUE;
}

void CXMLConvertDlg::OnLoadConvertConfig()
{
	// ��ȡҪ��д�Ľڵ�
	LoadOneConvertNode("ConvertTo8", CONVERT_TYPE_TO_8);
	LoadOneConvertNode("ConvertTo9", CONVERT_TYPE_TO_9);
	LoadOneConvertNode("ConvertTo3_VERTICAL", CONVERT_TYPE_TO_3_VERTICAL);
	LoadOneConvertNode("ConvertTo3_HORIZONTAL", CONVERT_TYPE_TO_3_HORIZONTAL);

	//POSITION pos = m_MapConvertConfig.GetStartPosition();
	//while (pos)
	//{
	//	CString strName;
	//	StructConvertConfigNode sStructConvertConfigNode;
	//	m_MapConvertConfig.GetNextAssoc(pos, strName, sStructConvertConfigNode);
	//}
}

void CXMLConvertDlg::LoadOneConvertNode(const char* szNodeName, ConvertType eType)
{
	try
	{
		// ����
		rapidxml::file<> fConfig(m_szConvertConfigPath);
		rapidxml::xml_document<> docConfig;
		docConfig.parse<0>(fConfig.data());

		// ���ڵ�
		rapidxml::xml_node<>* pRoot = docConfig.first_node();

		if (NULL == pRoot)
		{
			CString strError(_T("��Чת�������ļ�����xml��"));
			MessageBox(strError, _T("����"));
			exit(0);
		}

		// ��ȡҪ��д�Ľڵ�
		rapidxml::xml_node<>* pNodeForRead = pRoot->first_node(szNodeName);
		if (NULL == pNodeForRead)
		{
			CString strError1(_T("ϵͳ�����ļ������Ҳ������ýڵ㣺"));
			CString strError2(szNodeName);
			MessageBox(strError1 + strError2, _T("����"));
			exit(0);
		}

		// ���������ӽڵ� ��������
		for (rapidxml::xml_node<>* pSubNodeForRead = pNodeForRead->first_node("Image");
			pSubNodeForRead; pSubNodeForRead = pSubNodeForRead->next_sibling("Image"))
		{
			rapidxml::xml_attribute<> *attr = pSubNodeForRead->first_attribute("Name");
			if (NULL == attr)
			{
				CString strError1(szNodeName);
				CString strError2(_T("�����ýڵ�����Ҳ�������Name��"));
				MessageBox(strError1 + strError2, _T("����"));
				exit(0);
			}
			CString strName(attr->value());
			StructConvertConfigNode sStructConvertConfigNode;
			sStructConvertConfigNode.m_Type = eType;

			if (CONVERT_TYPE_TO_8 == eType || CONVERT_TYPE_TO_9 == eType || CONVERT_TYPE_TO_3_VERTICAL == eType)
			{
				rapidxml::xml_attribute<> *attr = pSubNodeForRead->first_attribute("XOffset1");
				if (NULL == attr)
				{
					CString strError1(_T("���ýڵ����ô���XOffset1��"));
					MessageBox(strError1 + strName, _T("����"));
					exit(0);
				}
				sStructConvertConfigNode.m_XOffset1 = atoi(attr->value());

				attr = pSubNodeForRead->first_attribute("XOffset2");
				if (NULL == attr)
				{
					CString strError1(_T("���ýڵ����ô���XOffset2��"));
					MessageBox(strError1 + strName, _T("����"));
					exit(0);
				}
				sStructConvertConfigNode.m_XOffset2 = atoi(attr->value());
			}

			if (CONVERT_TYPE_TO_8 == eType || CONVERT_TYPE_TO_9 == eType || CONVERT_TYPE_TO_3_HORIZONTAL == eType)
			{
				rapidxml::xml_attribute<> *attr = pSubNodeForRead->first_attribute("YOffset1");
				if (NULL == attr)
				{
					CString strError1(_T("���ýڵ����ô���YOffset1��"));
					MessageBox(strError1 + strName, _T("����"));
					exit(0);
				}
				sStructConvertConfigNode.m_YOffset1 = atoi(attr->value());

				attr = pSubNodeForRead->first_attribute("YOffset2");
				if (NULL == attr)
				{
					CString strError1(_T("���ýڵ����ô���YOffset2��"));
					MessageBox(strError1 + strName, _T("����"));
					exit(0);
				}
				sStructConvertConfigNode.m_YOffset2 = atoi(attr->value());
			}

			//map insert
			m_MapConvertConfig.SetAt(strName, sStructConvertConfigNode);
		}
	}
	catch (runtime_error e)
	{
		CString strError1(_T("Xmlת�������ļ�����"));
		CString strError2(e.what());
		MessageBox(strError1 + strError2, _T("����"));
		exit(0);
	}
}

void CXMLConvertDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	TCHAR szFileName[MAX_PATH + 1] = { 0 };//����ק���ļ��ľ���·��
	UINT nFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0); //����ק���ļ��ĸ���
	for (UINT i = 0; i<nFiles; i++)
	{
		DragQueryFile(hDropInfo, i, szFileName, MAX_PATH);//��ק���ļ���
		MessageBox(szFileName);
	}
	// TODO: Add your message handler code here and/or call default
	MessageBox(_T("����"), _T("����"));

	CDialogEx::OnDropFiles(hDropInfo);
}

BOOL CXMLConvertDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_DROPFILES == pMsg->message)
	{
		//OnDropFiles((HDROP)pMsg->hwnd);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CXMLConvertDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}

void CXMLConvertDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
}

void CXMLConvertDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
}

BOOL CXMLConvertDlg::CheckFolderExist(const CString& strPath)
{
	WIN32_FIND_DATA  wfd;
	bool rValue = FALSE;
	HANDLE hFind = FindFirstFile(strPath, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = TRUE;
	}
	FindClose(hFind);
	return rValue;
}

void CXMLConvertDlg::FreeMemory()
{
	for (auto iter = m_vecArrayPoint.cbegin(); iter != m_vecArrayPoint.cend(); iter++)
	{
		delete[] * iter;
	}
	m_vecArrayPoint.clear();
}