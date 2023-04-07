#include "pch.h"
#include "CMySocket.h"
#include "MFCChatClientDlg.h"


CMySocket::CMySocket()
{
}

CMySocket::~CMySocket()
{
}

//�ص�  ��д��Ҫ���������Լ���Ƶ�������;��������
void CMySocket::OnConnect(int nErrorCode)
{
	TRACE("####OnConnect %d", GetLastError());
	CMFCChatClientDlg* dlg = (CMFCChatClientDlg*)AfxGetApp()->GetMainWnd();

	CString str = dlg->CatShowString(_T(""), _T("����������ӳɹ�"));

	dlg->m_list.AddString(str);
	dlg->UpdateData();

	CAsyncSocket::OnConnect(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode)
{
	TRACE("####OnReceive");
	//�������ݵ�szRecvBuf
	TRACE("####CMySocket OnReceive");
	CMFCChatClientDlg* dlg = (CMFCChatClientDlg*)AfxGetApp()->GetMainWnd();
	char szRecvBuf[SEND_MAX_BUF] = { 0 };
	Receive(szRecvBuf, SEND_MAX_BUF, 0);
	TRACE("####CMySocket szRecvBuf=%s", szRecvBuf);

	//��ʾbuf
	USES_CONVERSION;
	CString strRecvMsg = A2W(szRecvBuf);
	CString strShow = dlg->CatShowString(_T("�����: "), strRecvMsg);
	dlg->m_list.AddString(strShow);

	//���ѡ���Զ��ظ���ѡ��
	if (((CButton*)dlg->GetDlgItem(IDC_AUTOSEND_CHECK))->GetCheck())
	{
		//��ȡ�༭������
		CString strAutoSendMsg;
		dlg->GetDlgItemText(IDC_AUTOSENDMSG_EDIT, strAutoSendMsg);
		//���+���ʽ
		CString strName;
		dlg->GetDlgItemText(IDC_NAME_EDIT, strName);
		CString strMsg = strName + _T("[�Զ��ظ�]: ") + strAutoSendMsg;

		char* szSendBuf = T2A(strMsg);
		dlg->m_client->Send(szSendBuf, SEND_MAX_BUF, 0);

		strShow = dlg->CatShowString(_T(""), strMsg);
		dlg->m_list.AddString(strShow);
	}

	dlg->UpdateData(FALSE);
	CAsyncSocket::OnReceive(nErrorCode);
}
