// ---------------------------------------------------------------------------
#ifndef SolenoidH
#define SolenoidH
#include <System.hpp>
// ! Класс работы с соленоидами
class Solenoid
{
public:
	// ! @brief Конструктор с параметрами
	// ! @param _lc Указатель на L-Card, с которого считываем значения
	// ! @param TypeOfControl Тип контроля - PP или PR
	Solenoid(String TypeOfControl);
	// ! Получить значение тока соленоида
	double getAmperage();
	// ! Получить значение напряжения соленоида
	double getVoltage();
	// ! Рассчитать значение температуры соленоида
	double getResist();
	// ! Проверяет, не перегрелся ли соленоид
	bool OkResist();
	bool OkU(void);
	AnsiString GetUIR(void);

private:
	// ! Коэффициент сопротивления
	double Devider;
	// ! Канал L-Card, на который подключен вывод тока
	int chAmperage;
	// ! Канал L-Card, на который подключен вывод напряжения
	int chVoltage;
	// ! Критический уровень температуры, выше которого считаем перегрев
	float AlarmLevel;
	float AlarmULevel;
	float DifULevel;
};
extern Solenoid *CrossSolenoid;
extern Solenoid *LinearSolenoid;
#endif
