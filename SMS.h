//---------------------------------------------------------------------------

#ifndef SMSH
#define SMSH

#include <Sockets.hpp>
#include "Dialogs.hpp"


//---------------------------------------------------------------------------
//! ����� �� �������� � �������� ��������� ����� ������������ � ������������� �� TCP/IP
class SMS
{
public:
	//! ����������� �� ���������
	SMS(TComponent *Owner);
	//! ����������
	~SMS();
	//! ������� ��� ������� ��� ��������
	String ServerName;
	//! ����� ����� (������ ���� ���������� � ����� �����������)
	int Port;
	//! ����� ����� � �����
	int length_in_zones;
	//! @brief ������� ������� ������ ���� �� ����
	//! @param sendString ������, ������� ����� ���������
	void SendAnyMessage(String sendString);
	//! @brief ��������� ������ �� ������� � ���������� ����
	//! @param zone ����� ����
	//! @param thickness ����������� ������� � ���� ����
	bool SendZoneThickness(int zone , double thickness);
	//! ��������� ����� ����� � �����
	int AskTubeLength();
	//! ���� ������ ����� TCP/IP
	bool TestConnection();
	//! ���������� ������ ��������
	void StartServer();

private:
	//! ������ ��� ������ � ��������
	TTcpServer *Server;
	//! ������ ��� ������ � ��������
	TTcpClient *Client;
	//! ������ ���������� ������ ��������
	wchar_t client_rec[50];
	//!	������ ���������� ������ ��������
	wchar_t server_rec[50];
	//!	����� ���������� ������ ��������
	int server_rec_length;
	//! ����� ���������� ������ ��������
	int client_rec_length;
	//! ����� ������� (?) ����� �� ����� �������
	int sen_length;
	//! ������� - ������ ������� ������
	TEvent *client_get;

	//! ��������� ����� ������� �� ���������� �������
	String CreateServerRespond();
	//! �������: ������ �������� ������ ������, ���������� �����
	void __fastcall ServerAccept(TObject *Sender, TCustomIpClient *ClientSocket);
	//! �������: ������ ������
	void __fastcall ServerCreateHandle(TObject *Sender);
	//! �������: ������ ���������
	void __fastcall ServerDestroyHandle(TObject *Sender);
	//!	�������: ������ �������� ������ �� ������
	void __fastcall ServerSend(TObject *Sender, PChar Buf, int &DataLen);
	//!	�������: ������ ������ ������ �� �������
	void __fastcall ServerReceive(TObject *Sender, PChar Buf, int &DataLen);

	//! �������: ������ �������� ������
	void __fastcall ClientSend(TObject *Sender, PChar Buf, int &DataLen);
	//! �������: ������ ������ ������
	void __fastcall ClientReceive(TObject *Sender, PChar Buf, int &DataLen);
	//! �������: ������ ����������� � �������
	void __fastcall ClientConnect(TObject *Sender);
	//! �������: ������ ���������� �� �������
	void __fastcall ClientDisconnect(TObject *Sender);
	//! �������: ������ ��� �������� � �������
	void __fastcall ClientError(TObject *Sender, int SocketError);

};
//---------------------------------------------------------------------------
extern SMS* sms;
//---------------------------------------------------------------------------
#endif
