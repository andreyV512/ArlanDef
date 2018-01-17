//---------------------------------------------------------------------------

#ifndef ComPortH
#define ComPortH

#include "Global.h"
#include "clustering.h"
#include "Result.h"
#include "Main.h"

#include "slycomm\CommPort.h"
#pragma link "CommPort"

//---------------------------------------------------------------------------
//! Обертка для COM порта, позволяющая легко отправлять заранее подготовленные посылки
typedef void (__closure *OnStatusDef)(AnsiString _msg);
class MyComPort
{
private:

// параметры передачи
	//! Имя порта (COM1, COM2 ...)
	String PortName;
	//! Бод рейт (скорость)
	int BaudRate;
	//! Биты данных
	int DataBits;
	//! Стоповые биты
	int StopBits;
	//! Четность
	int Parity;

// внутренние переменные
	//! Режим передачи (для разных проектов)
	int ASUMode;
	//! Длина посылки от АСУ (5 или 17)
	int length;
	//! Адрес абонента в сети (всегда const)
	int abonent;
	//! Функция обмена	(1, 2 или 5)
	int function;
	//! Принятые данные с кома
	unsigned char data[50];
	//! Длина принятых данных
	int data_size;
	//! Событие - данные получены
	TEvent* DataReceived;
	//! Элемент Слы Диман :)
	TCommPort *CommPort;

// внутренние функции
	//! Установить настройки COM порта
	void SetComPortSettings();
	//! Запрос во второму протоколу	(Пыть-Ях)
	AnsiString GetTubeNumber_2(bool* _transit);
	//! Проверяет присланные данные (только 2-ой протокол )
	bool CheckSentMessage();

// запись результатов по Елисеевски, жесть
	//! Конвертит результаты контроля по Елисеевской формуле
	unsigned char EliseevRes(int zone);
	//! Зона реза по шаблонированию
	int plant_cut;
//	Result pp2;
//	Result pr2;
//	ThicknessResult th2;
//	SummaryResult sum2;
	//! Костыли для извлечения результатов
//	TColor Good;
//	TColor Class2;
//	TColor Brack;

	//! Время ожидания номера трубы
	UINT TubeNumTimeout;
	//! Время ожидания результата
	UINT ResultTimeout;
	//! Время ожидания тестовой посылки
	UINT TestTimeout;
	//! Время ожидания послыки с группой прочности
	UINT SolidGroupTimeout;

	void Create();
	MyComPort();
	void Status(AnsiString _msg);
public:
	//! Конструктор нашего класса
	MyComPort(AnsiString _ini_section);
	//! Деструктор
	~MyComPort();

	//! Событие - данные приняты
	void __fastcall MyComPortDataReceived(TObject *Sender, DWORD Count);
	//! Расчет Контрольной суммы по 1 протоколу
	unsigned char countCRC16_1(unsigned char* buff, unsigned int size);
	//! Расчет контрольной суммы по 2 протоколу
	unsigned short countCRC16_2(unsigned char* bufData, unsigned int sizeData);
	//! Тест канала связи
	bool TestConnection();
	//! Запрос номера трубы (весь обмен)
	AnsiString GetTubeNumber(bool* _transit);
	//! @brief Посылка результата (весь обмен)
	//! @param res Общий результат (годно / брак)
	//!	@param c1,c2 Зоны реза 1, 2 (начало и конец трубы)
	//! @param len Длина трубы в зонах
	//! @param sgg Группа прочности (1 буква латиницей)
	bool SendResultToASU(short res, short c1, short c2, short len, SolidGroups::Group sgg);		// 1 - Стрежевой
//	bool SendResultToASU(Result &pp1, Result &pr1, ThicknessResult &th1, SummaryResult &sum1 , SolidGroups::Group sgg);	// 2 - Пыть-Ях
	//! @brief Посылка результата (весь обмен)
	//! @param solid_num Группа прочности, выраженная числом
	bool SendResultToASU(int solid_num);	// 2 - Пыть-Ях
	//! Принять группу прочности (весь обмен)
	SolidGroups::Tube GetSolidGroup();
	//! Зона реза по шаблонированию
	inline int GetPlantCut() { return plant_cut;}
	OnStatusDef OnStatus;
	inline bool IsOpened(void){return(CommPort->Open);}
	bool SendToProtocol();
};

//---------------------------------------------------------------------------
// глобальный экземпляр, управляющий цифровыми сигналами из любой точки программы.
extern MyComPort *MyCom;
//---------------------------------------------------------------------------

#endif
