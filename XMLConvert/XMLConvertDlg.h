
// XMLConvertDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "rapidxml\rapidxml.hpp"
#include "rapidxml\rapidxml_print.hpp"
#include "rapidxml\rapidxml_utils.hpp"
#include <vector>

using namespace std;

enum OpType
{
	OP_TYPE_READ,
	OP_TYPE_WRITE,
};

enum ConvertType
{
	CONVERT_TYPE_NULL,
	// �Ź��и��8��
	CONVERT_TYPE_TO_8,
	// �Ź��и��9��
	CONVERT_TYPE_TO_9,
	// ��ֱ�и��3��
	CONVERT_TYPE_TO_3_VERTICAL,
	// ˮƽ�и��3��
	CONVERT_TYPE_TO_3_HORIZONTAL,
};

struct StructConvertConfigNode
{
	StructConvertConfigNode()
	{
		m_XOffset1	= 0;
		m_XOffset2	= 0;
		m_YOffset1	= 0;
		m_YOffset2	= 0;
		m_Type		= CONVERT_TYPE_NULL;
	}
public:
	INT m_XOffset1;
	INT m_XOffset2;
	INT m_YOffset1;
	INT m_YOffset2;
	ConvertType m_Type;
};


// CXMLConvertDlg dialog
class CXMLConvertDlg : public CDialogEx
{
// Construction
public:
	CXMLConvertDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_XMLCONVERT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

// use 
private:
	// ����
	CEdit ControlInputDir;
	CEdit ControlOutPutDir;
	CEdit ControlConvertConfig;
	CMap<CString, LPCTSTR, StructConvertConfigNode, StructConvertConfigNode> m_MapConvertConfig;
	char*  m_szInputDir;
	char*  m_szOutPutDir;
	char*  m_szConvertConfigPath;
	vector<char*> m_vecArrayPoint;
public:
	// ���س���������ļ�
	void OnLoadSystemConfig();
	// ϵͳ�����ļ���д
	BOOL SystemConfigOp(const OpType& eOpType, const char* szNodeName, char* szNodeValue);

	// ����ת�������ļ�
	void OnLoadConvertConfig();
	// ����һ��ת���ڵ�
	void LoadOneConvertNode(const char* szNodeName, ConvertType eType);

	// XMLת��
	void DoXMLConvert();
	void DoOneXMLConvert(const CString& strInputFilePath, const CString& strFileName, const CString& strRealOutPutDir);
	void AddNewNode(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* pNodeMother, rapidxml::xml_node<>* pNodeInsert,
		const CString& strName, INT nValue1,  INT nValue2, INT nValue3, INT nValue4);

	// ͨ�ú���
	BOOL CheckFolderExist(const CString& strPath);
	void FreeMemory();

	// event
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
