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

	//如果选中自动回复单选框
	if (((CButton*)dlg->GetDlgItem(IDC_AUTOSEND_CHECK))->GetCheck())
	{
		//读取编辑框内容
		CString strAutoSendMsg;
		dlg->GetDlgItemText(IDC_AUTOSENDMSG_EDIT, strAutoSendMsg);
		//封包+组格式
		CString strName;
		dlg->GetDlgItemText(IDC_NAME_EDIT, strName);
		CString strMsg = strName + _T("[自动回复]: ") + strAutoSendMsg;

		char* szSendBuf = T2A(strMsg);
		dlg->m_client->Send(szSendBuf, SEND_MAX_BUF, 0);

		strShow = dlg->CatShowString(_T(""), strMsg);
		dlg->m_list.AddString(strShow);
	}

	dlg->UpdateData(FALSE);
	CAsyncSocket::OnReceive(nErrorCode);
}
