//---------------------------------------------------------------------------

#ifndef SMSH
#define SMSH

#include <Sockets.hpp>
#include "Dialogs.hpp"


//---------------------------------------------------------------------------
//! Класс по отправке и передаче сообщений между толщиномером и дефектоскопом по TCP/IP
class SMS
{
public:
	//! Конструктор по умолчанию
	SMS(TComponent *Owner);
	//! Деструктор
	~SMS();
	//! Сетевое имя сервера при передаче
	String ServerName;
	//! Номер порта (должен быть одинаковым у обоих компьютеров)
	int Port;
	//! Длина трубы в зонах
	int length_in_zones;
	//! @brief Послать посылку любого вида по сети
	//! @param sendString Строка, которую нужно отправить
	void SendAnyMessage(String sendString);
	//! @brief Отправить данные по толщине в конкретной зоне
	//! @param zone Номер зоны
	//! @param thickness Минимальная толщина в этой зоне
	bool SendZoneThickness(int zone , double thickness);
	//! Запросить длину трубы в зонах
	int AskTubeLength();
	//! Тест канала связи TCP/IP
	bool TestConnection();
	//! Стартануть сервер передачи
	void StartServer();

private:
	//! Объект для работы с сервером
	TTcpServer *Server;
	//! Объект для работы с клиентом
	TTcpClient *Client;
	//! Массив полученных данных клиентом
	wchar_t client_rec[50];
	//!	Массив полученных данных сервером
	wchar_t server_rec[50];
	//!	Длина полученных данных сервером
	int server_rec_length;
	//! Длина полученных данных клиентом
	int client_rec_length;
	//! Длина посылки (?) смысл не особо понятен
	int sen_length;
	//! Событие - клиент получил данные
	TEvent *client_get;

	//! Составить ответ сервера на полученную посылку
	String CreateServerRespond();
	//! Событие: сервер усппешно принял данные, генерируем ответ
	void __fastcall ServerAccept(TObject *Sender, TCustomIpClient *ClientSocket);
	//! Событие: сервер создан
	void __fastcall ServerCreateHandle(TObject *Sender);
	//! Событие: сервер уничтожен
	void __fastcall ServerDestroyHandle(TObject *Sender);
	//!	Событие: сервер отправил данные на клиент
	void __fastcall ServerSend(TObject *Sender, PChar Buf, int &DataLen);
	//!	Событие: сервер принял данные от клиента
	void __fastcall ServerReceive(TObject *Sender, PChar Buf, int &DataLen);

	//! Событие: клиент отправил данные
	void __fastcall ClientSend(TObject *Sender, PChar Buf, int &DataLen);
	//! Событие: клиент принял данные
	void __fastcall ClientReceive(TObject *Sender, PChar Buf, int &DataLen);
	//! Событие: клиент подключился к серверу
	void __fastcall ClientConnect(TObject *Sender);
	//! Событие: клиент отключился от сервера
	void __fastcall ClientDisconnect(TObject *Sender);
	//! Событие: ошибка при передаче в клиенте
	void __fastcall ClientError(TObject *Sender, int SocketError);

};
//---------------------------------------------------------------------------
extern SMS* sms;
//---------------------------------------------------------------------------
#endif
