#include "pch.h"
#include "CMySocket.h"
#include "MFCChatClientDlg.h"


CMySocket::CMySocket()
{
}

CMySocket::~CMySocket()
{
}

//回调  重写：要满足我们自己设计的特殊用途：传参数
void CMySocket::OnConnect(int nErrorCode)
{
	TRACE("####OnConnect %d", GetLastError());
	CMFCChatClientDlg* dlg = (CMFCChatClientDlg*)AfxGetApp()->GetMainWnd();

	CString str = dlg->CatShowString(_T(""), _T("与服务器连接成功"));

	dlg->m_list.AddString(str);
	dlg->UpdateData();

	CAsyncSocket::OnConnect(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode)
{
	TRACE("####OnReceive");
	//接收数据到szRecvBuf
	TRACE("####CMySocket OnReceive");
	CMFCChatClientDlg* dlg = (CMFCChatClientDlg*)AfxGetApp()->GetMainWnd();
	char szRecvBuf[SEND_MAX_BUF] = { 0 };
	Receive(szRecvBuf, SEND_MAX_BUF, 0);
	TRACE("####CMySocket szRecvBuf=%s", szRecvBuf);

	//显示buf
	USES_CONVERSION;
	CString strRecvMsg = A2W(szRecvBuf);
	CString strShow = dlg->CatShowString(_T("服务端: "), strRecvMsg);
	
	dlg->m_list.AddString(strShow);
	dlg->UpdateData();

	CAsyncSocket::OnReceive(nErrorCode);
}
