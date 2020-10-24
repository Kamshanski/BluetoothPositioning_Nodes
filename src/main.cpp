// #include "Arduino.h"
// #include <BLEDevice.h>
// #include <BLEUtils.h>
// #include <BLEServer.h>
// #include <utilities.h>

// // See the following for generating UUIDs:
// // https://www.uuidgenerator.net/

// #define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
// #define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// BLEAdvertisementData* advData;
// int i = 0;
// std::string manufData = std::string("  dadadada");
// void setup()
// {
//   Serial.begin(115200);
//   Serial.println("Starting BLE work!");

//   BLEDevice::init("Long name works now");
//   BLEServer *pServer = BLEDevice::createServer();
//   // BLEService *pService = pServer->createService(SERVICE_UUID);
//   // BLECharacteristic *pCharacteristic = pService->createCharacteristic(
//   //     CHARACTERISTIC_UUID,
//   //     BLECharacteristic::PROPERTY_READ |
//   //         BLECharacteristic::PROPERTY_WRITE);

//   // pCharacteristic->setValue("Hello World says Neil");
//   // pService->start();
//   // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
//   BLEAdvertising *adv = BLEDevice::getAdvertising();
//   adv->addServiceUUID(SERVICE_UUID);
//   adv->setScanResponse(true);
//   adv->setMinPreferred(0x06); // functions that help with iPhone connections issue
//   adv->setMinPreferred(0x12);
//   advData = new BLEAdvertisementData();
//   advData.setManufacturerData("  potata");
//   adv->setAdvertisementData(advData);
//   BLEDevice::startAdvertising();
//   Serial.println("Characteristic defined! Now you can read it in your phone!");
// }

// void loop()
// {
//   BLEAdvertising *adv = BLEDevice::getAdvertising();
//   adv->stop();
//   delete advData;
//   advData = new BLEAdvertisementData();
//   manufData.append("q");
//   bool ok = advData->setManufacturerData(manufData);
//   if (!ok) {
//     manufData = std::string("  dadadada");
//     advData = new BLEAdvertisementData();
//     manufData.append("qqqq");
//     ok = advData->setManufacturerData(manufData);
//   }
//   adv->setAdvertisementData(*advData);
//   BLEDevice::startAdvertising();
//   // put your main code here, to run repeatedly:
//   delay(2000);
//   i++;
// }
// // #include <EEPROM.h>
// // #include "Wire.h"
// // #include <BLEDevice.h>
// // #include <BLEUtils.h>
// // #include <BLEServer.h>
// // #include <BLE2902.h>
// // #include <set>
// // #include "ArduinoJson.h"

// // #define pr(msg) (Serial.print(msg))
// // #define prl(msg) (Serial.println(msg))

// // const int BAUD_RATE = 115200;

// // struct TimePoint {
// // 	int hours, mins;
// // 	TimePoint(int h, int m) : hours(h), mins(m) {}
// // };

// // inline bool operator<(const TimePoint& lhs, const TimePoint& rhs)
// // {
// // 	if (lhs.hours < rhs.hours) {
// // 		return 1;
// // 	}
// // 	else if ((lhs.hours == rhs.hours) || (lhs.mins < rhs.mins)) {
// // 		return 1;
// // 	}
// // 	return 0;
// // }

// // class Schedule {
// // private:
// // 	//0b00000001 = 0b-Mon-Tue-Wed-Thu-Fri-Sat-Sun-AVALIABLE
// // 	bool avaliable = 0;
// // 	std::set<int>* days = NULL;
// // 	std::set<TimePoint*>* times = NULL;
// // 	bool triggered = 0;
// // public:
// // 	static const String ID;
// // 	static const String DAYS;
// // 	static const String TIMES;
// // 	static const char TYPE = 'P';
// // 	const int _id;
// // 	Schedule(int id) : _id(id) {}
// // 	void setDays(std::set<int>* days) {
// // 		this->days = days;
// // 	}
// // 	void setTimes(std::set<TimePoint*>* t) {
// // 		times = t;
// // 		//times.insert(new TimePoint(hours, mins));
// // 	}
// // 	std::set<int> getDays() {
// // 		return *days;
// // 	}
// // 	std::set<TimePoint*> getTimes() {
// // 		return *times;
// // 	}
// // 	int isTime(int day, int hours, int mins) {
// // 		if (days != NULL && times != NULL) {
// // 			if (days->size() != 0 && times->size() != 0) {
// // 				for (int d : *days) {
// // 					if (d == day) {
// // 						for (TimePoint* tp : *times) {
// // 							if (tp->hours == hours && tp->mins == mins) {
// // 								return 1;
// // 							}
// // 						}
// // 						return 0;
// // 					}
// // 				}
// // 			}
// // 		}
// // 		return 0;
// // 	}
// // 	void setAvaliable(bool avaliable) {
// // 		this->avaliable = avaliable;
// // 	}
// // 	bool isAvaliable() {
// // 		return avaliable;
// // 	}
// // 	bool isTreggered() {
// // 		return triggered;
// // 	}
// // 	void setTriggered(bool triggered) {
// // 		this->triggered = triggered;
// // 	}

// // 	// �� ������������ ����
// // 	static std::string toJson(Schedule* src) {
// // 		return "ds";
// // 	}

// // 	static Schedule* fromJson(DynamicJsonDocument* doc) {
// // 		int id = (*doc)[Schedule::ID].as<int>();
// // 		Schedule* schedule = new Schedule(id);

// // 		std::set<int>* days = new std::set<int>();
// // 		JsonArray arrayDays = (*doc)[Schedule::DAYS].as<JsonArray>();
// // 		if (0 < arrayDays.size() && arrayDays.size() <= 7) {
// // 			for (JsonVariant v : arrayDays) {
// // 				days->insert(v.as<int>());
// // 			}
// // 		}
// // 		else {
// // 			return NULL;
// // 		}
// // 		schedule->setDays(days);

// // 		std::set<TimePoint*>* times = new std::set<TimePoint*>();
// // 		JsonArray arrayTimes = (*doc)[Schedule::TIMES].as<JsonArray>();
// // 		if (0 < arrayTimes.size() && arrayTimes.size() <= 7) {
// // 			for (JsonVariant v : arrayTimes) {
// // 				JsonArray hm = v.as<JsonArray>();
// // 				if (hm.size() == 2) {
// // 					times->insert(new TimePoint(hm[0].as<int>(), hm[1].as<int>()));
// // 				}
// // 			}
// // 		}
// // 		else {
// // 			return NULL;
// // 		}
// // 		schedule->setTimes(times);
// // 		schedule->setAvaliable(1);

// // 		return schedule;
// // 	}

// // 	void printPill() {
// // 		if (isAvaliable()) {
// // 			pr("Pill from Memory ");
// // 			pr(_id);
// // 			pr(": days: ");
// // 			for (int day : *days) {
// // 				pr(day);
// // 				pr(" ");
// // 			}
// // 			pr(", times:");
// // 			for (TimePoint* tp : *times) {
// // 				pr("[");
// // 				pr(tp->hours);
// // 				pr(", ");
// // 				pr(tp->mins);
// // 				pr("], ");
// // 			}
// // 			prl(".");
// // 		}
// // 		else {
// // 			pr("Pill ");
// // 			pr(_id);
// // 			pr(" is NOT avaliable");
// // 		}

// // 	}
// // };
// // const String Schedule::ID = "id";
// // const String Schedule::DAYS = "days";
// // const String Schedule::TIMES = "times";

// // namespace Battery {
// // 	static const int MAX_ADC = 4095;
// // 	static const int MIN_ADC = 2900;
// // 	static const int MIN_CHARGE = 15;	// 15% is a minimum
// // 	static const int PIN = 34;
// // 	int voltage() {
// // 		return analogRead(PIN);
// // 	}
// // 	// � %-��
// // 	int getCharge() {
// // 		int bitsOfAdc = voltage();
// // 		if (bitsOfAdc < MIN_ADC) {
// // 			return 0;
// // 		} else {
// // 			return ((bitsOfAdc - MIN_ADC) * 100) / (MAX_ADC - MIN_ADC);
// // 		}
// // 	}

// // 	inline bool isLowBattery(int charge) {
// // 		return charge < MIN_CHARGE;
// // 	}

// // 	void init() {
// // 		pinMode(PIN, INPUT);
// // 	}
// // }

// // namespace Vibro {
// // 	// GPIO3 - RX0
// // 	static const int PIN = 3;
// // 	// States
// // 	static const byte INIT = -1;  // Special num to identify init state
// // 	static const byte FIRST_PILL = 0;
// // 	static const byte SECOND_PILL = 1;
// // 	static const byte THIRD_PILL = 2;
// // 	static const byte FORTH_PILL = 3;
// // 	static const byte NO_TABLETS = 4;
// // 	static const byte LOW_BATTERY = 5;
// // 	static const byte SLEEP = 6;
// // 	// State vars
// // 	static byte state = SLEEP; // must be set out of namespace
// // 	static byte previousState = INIT;
// // 	// Duration of all states
// // 	static const unsigned long DURATION = 5000UL;
// // 	// Time points
// // 	static unsigned long currentMillis = 0UL;
// // 	static unsigned long previousMillis = 0UL;
// // 	unsigned long startMillis = 0UL;
// // 	bool isVibrating = 0;
// // 	// 7 - num of Modes, 8 - num of numbers in Mode
// // 	static const float MODE[7][8] = {
// // 	{0.2f, 0.8f},                       // FIRST_PILL
// // 	{0.15f, 0.1f, 0.15f, 0.6f},                 // SECOND_PILL
// // 	{0.1f, 0.07f, 0.1f, 0.07f, 0.1f, 0.56f},          // THIRD_PILL
// // 	{0.08f, 0.04f, 0.08f, 0.04f, 0.08f, 0.04f, 0.08f, 0.56f}, // FORTH_PILL
// // 	{0.3f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f},     // NO_TABLETS
// // 	{0.13f, 0.07f, 0.13f, 0.07f, 0.13f, 0.07f, 0.3f, 0.1f},   // LOW_BATTERY
// // 	{0.0f}                            // SLEEP
// // 	};

// // 	static const byte MODE_SIZE[7] = {
// // 	  2,
// // 	  4,
// // 	  6,
// // 	  8,
// // 	  8,
// // 	  8,
// // 	  1
// // 	};

// // 	void dwrite(byte voltage) {
// // 		digitalWrite(PIN, voltage);
// // 	}

// // 	void init() {
// // 		pinMode(PIN, OUTPUT);
// // 		dwrite(LOW);  // init. state is OFF
// // 		state = previousState = SLEEP;
// // 	}

// // 	void run(unsigned long duration, byte size, const float gaps[]) {
// // 		static unsigned long after;  // Time when next part of state should be started
// // 		static unsigned int i;  // Iterator
// // 		currentMillis = millis();
// // 		// Enter a new state or Start new cycle of previous one
// // 		if (state != previousState || i == size) {
// // 			i = 0;
// // 			after = gaps[i] * duration;
// // 			previousMillis = currentMillis;
// // 			previousState = state;
// // 			dwrite(state != SLEEP ? HIGH : LOW);
// // 		}
// // 		// Fast skip if SLEEP
// // 		if (state == SLEEP) {
// // 			after = 0;
// // 			i = 0;
// // 			return;
// // 		}
// // 		unsigned long dt = currentMillis - previousMillis;
// // 		if (dt > after) {
// // 			i++;
// // 			if (i == size) {  // End of cycle
// // 				return;
// // 			}
// // 			after += gaps[i] * duration;
// // 			dwrite(i % 2 == 0 ? HIGH : LOW);
// // 		}
// // 	}

// // 	void go() {
// // 		currentMillis = millis();
// // 		if (state != SLEEP) {
// // 			if (isVibrating) {
// // 				if (millis() - startMillis > 10 * 1000) {
// // 					isVibrating = 0;
// // 					state = SLEEP;
// // 				}
// // 			}
// // 		}
// // 		run(DURATION, MODE_SIZE[state], MODE[state]);
// // 	}

// //   void startNewState() {
// //       startMillis = millis();
// //       isVibrating = 1;
// //     }

// // 	void notifyPill(byte id) {
// // 		switch (id) {
// // 		case 0:
// // 			state = FIRST_PILL;
// // 			break;
// // 		case 1:
// // 			state = SECOND_PILL;
// // 			break;
// // 		case 2:
// // 			state = THIRD_PILL;
// // 			break;
// // 		case 3:
// // 			state = FORTH_PILL;
// // 			break;
// // 		}
// // 		startNewState();
// // 	}

// // 	void notifyLowBattery() {
// // 		state = LOW_BATTERY;
// // 		startNewState();
// // 	}

// // }

// // namespace Clock {
// // 	static const char TYPE = 'C';
// // 	static const String SECONDS = "s";
// // 	static const String MINUTES = "mi";
// // 	static const String HOURS = "h";
// // 	static const String DAY_OF_WEEK = "dw";
// // 	static const String DAY_OF_MONTH = "dm";
// // 	static const String MONTH = "mo";
// // 	static const String YEAR = "y";
// // 	static const int I2C_RTC = 0x68;
// // 	// �������� to �������-����������
// // 	byte decToBcd(byte val)
// // 	{
// // 		return((val / 10 * 16) + (val % 10));
// // 	}
// // 	byte bcdToDec(byte val)
// // 	{
// // 		return((val / 16 * 10) + (val % 16));
// // 	}
// // 	byte amDoW(byte rusDoW) {
// // 		return rusDoW == 7 ? 1 : rusDoW + 1;
// // 	}
// // 	byte rusDoW(byte amDoW) {
// // 		return amDoW == 1 ? 7 : amDoW - 1;
// // 	}
// // 	void setTime(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
// // 	{
// // 		// sets time and date data to DS3231
// // 		Wire.beginTransmission(I2C_RTC);
// // 		Wire.write(0); // set next input to start at the seconds register
// // 		Wire.write(decToBcd(second)); // set seconds
// // 		Wire.write(decToBcd(minute)); // set minutes
// // 		Wire.write(decToBcd(hour)); // set hours
// // 		Wire.write(decToBcd(amDoW(dayOfWeek + 1))); // set day of week (1=Sunday, 7=Saturday)
// // 		Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
// // 		Wire.write(decToBcd(month)); // set month
// // 		Wire.write(decToBcd(year)); // set year (0 to 99)
// // 		Wire.endTransmission();
// // 	}
// // 	void readTime(byte* second, byte* minute, byte* hour, byte* dayOfWeek, byte* dayOfMonth, byte* month, byte* year)
// // 	{
// // 		Wire.beginTransmission(I2C_RTC);
// // 		Wire.write(0);  // set DS3231 register pointer to 00h

// // 		Wire.requestFrom(I2C_RTC, 7);
// // 		// request seven bytes of data from DS3231 starting from register 00h
// // 		*second = bcdToDec(Wire.read() & 0x7f);
// // 		*minute = bcdToDec(Wire.read());
// // 		*hour = bcdToDec(Wire.read() & 0x3f);
// // 		*dayOfWeek = rusDoW(bcdToDec(Wire.read())) - 1;  // -1 to make Mon=0...
// // 		*dayOfMonth = bcdToDec(Wire.read());
// // 		*month = bcdToDec(Wire.read());
// // 		*year = bcdToDec(Wire.read());
// // 		Wire.endTransmission();
// // 	}
// // 	void init() {
// // 		// INITIAL DATE
// // 		prl("Init time: 23.26.30 Cp 26 ��� 2019");
// // 		Wire.begin();
// // 		setTime(30, 26, 23, 4, 26, 12, 19);
// // 	}
// // 	void printlnTime() {
// // 		byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
// // 		Clock::readTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
// // 		pr(hour);
// // 		pr(":");
// // 		pr(minute);
// // 		pr(":");

// // 		if (second < 10) {
// // 			prl("0");
// // 		}
// // 		pr(second);
// // 		pr(" ");
// // 		pr(dayOfMonth);
// // 		pr(".");

// // 		pr(month);
// // 		pr(".");

// // 		pr(year);
// // 		pr(" ");
// // 		pr("Day of week: ");
// // 		switch (dayOfWeek) {
// // 		case 1:
// // 			prl("Sunday");
// // 			break;
// // 		case 2:
// // 			prl("Monday");
// // 			break;
// // 		case 3:
// // 			prl("Tuesday");
// // 			break;
// // 		case 4:
// // 			prl("Wednesday");
// // 			break;
// // 		case 5:
// // 			prl("Thursday");
// // 			break;
// // 		case 6:
// // 			prl("Friday");
// // 			break;
// // 		case 7:
// // 			prl("Saturday");
// // 			break;
// // 		default:
// // 			prl("ERROR!!!");
// // 			prl("CHECK IF RTC IS CONNECTED");
// // 		}
// // 	}
// // }

// // namespace Memory {
// // 	static const int STOP_HOUR_MIN = 0b10111111;
// // 	static const int MEMORY_SIZE = 65;
// // 	static const int VALID_BYTES_SIZE = 5;
// // 	static const int VALID_BYTES[VALID_BYTES_SIZE] = { 19, 18, 17, 16, 14 };

// // 	void clearMemory() {
// // 		for (int i = 0; i < MEMORY_SIZE; i++) {
// // 			EEPROM.write(i, 0);
// // 		}
// // 		for (int i = 0; i < VALID_BYTES_SIZE; i++) {
// // 			EEPROM.write(16 * i, VALID_BYTES[i]);
// // 		}
// // 		EEPROM.commit();
// // 	}

// // 	void checkMemory() {
// // 		for (size_t i = 0, adr = 0; i < VALID_BYTES_SIZE; i++, adr = adr + 16) {
// // 			int d = EEPROM.read(adr);
// // 			pr(i); pr(": "); pr(d); pr(" - "); prl();
// // 			if (d != VALID_BYTES[i]) {
// // 				prl("Clearing Memory");
// // 				clearMemory();
// // 				return;
// // 			}
// // 		}
// // 		prl("Memory is OK");
// // 	}

// // 	void init() {
// // 		bool result = EEPROM.begin(MEMORY_SIZE);
// // 		pr("Memory init: ");
// // 		prl(result);
// // 		if (!result) {
// // 			return;
// // 		}
// // 		checkMemory();
// // 	}

// // 	void writeDoW(int pillNum, int isAvaliable, std::set<int> days) {
// // 		int daysBits = 1 * isAvaliable;
// // 		if (days.size() > 0) {
// // 			for (int day : days) {
// // 				daysBits = daysBits | (0b10000000 >> day);
// // 			}
// // 		}
// // 		EEPROM.write(1 + 16 * pillNum, daysBits);
// // 	}

// // 	void writeTimes(int pillNum, std::set<TimePoint*> times) {
// // 		int timesLength = times.size();
// // 		int timeNum = 0;
// // 		for (TimePoint* tp : times) {
// // 			EEPROM.write(2 + 16 * pillNum + 2 * timeNum, tp->hours);
// // 			EEPROM.write(3 + 16 * pillNum + 2 * timeNum, tp->mins);
// // 			timeNum++;
// // 		}
// // 		// ���� ���� �� ��� 7 ���� ������, �������� ���� ���������
// // 		if (timesLength < 7) {
// // 			EEPROM.write(2 + 16 * pillNum + 2 * timesLength, STOP_HOUR_MIN);
// // 			EEPROM.write(3 + 16 * pillNum + 2 * timesLength, STOP_HOUR_MIN);
// // 		}

// // 	}

// // 	void writePill(Schedule* pill) {
// // 		int pillNum = pill->_id;

// // 		writeDoW(pillNum, pill->isAvaliable(), pill->getDays());

// // 		writeTimes(pillNum, pill->getTimes());

// // 		EEPROM.commit();  // ����� �� ����� �� ���
// // 	}

// // 	int readDoW(int pillNum) {
// // 		//0b00000001 = 0b-Mon-Tue-Wed-Thu-Fri-Sat-Sun-AVALIABLE
// // 		return EEPROM.read(1 + 16 * pillNum);
// // 	}

// // 	TimePoint* readTimes(int pillNum, int timeNum) {
// // 		int hours = 0;
// // 		int mins = 0;
// // 		hours = EEPROM.read(2 + 16 * pillNum + 2 * timeNum);
// // 		mins = EEPROM.read(3 + 16 * pillNum + 2 * timeNum);
// // 		TimePoint* tp = new TimePoint(hours, mins);
// // 		return tp;
// // 	}

// // 	Schedule* readPill(int id) {
// // 		Schedule* newSchedule = new Schedule(id);
// // 		std::set<int>* days = new std::set<int>();
// // 		std::set<TimePoint*>* times = new std::set<TimePoint*>();

// // 		int daysBits = readDoW(id);
// // 		for (int i = 0; i < 7; i++) {
// // 			if (daysBits & (0b10000000 >> i)) {
// // 				days->insert(i);
// // 			}
// // 		}
// // 		newSchedule->setDays(days);
// // 		newSchedule->setAvaliable(daysBits & 0b00000001);

// // 		for (int timeNum = 0; timeNum < 7; timeNum++) {
// // 			TimePoint* tp = readTimes(id, timeNum);
// // 			if (tp->hours == STOP_HOUR_MIN || tp->hours == STOP_HOUR_MIN) {
// // 				break;
// // 			}
// // 			times->insert(tp);
// // 		}
// // 		newSchedule->setTimes(times);

// // 		return newSchedule;
// // 	}

// // 	void deletePill(int pillNum) {
// // 		for (size_t i = 0; i < 15; i++) {
// // 			EEPROM.write(1 + pillNum * 16 + i, 0);
// // 		}
// // 	}
// // }

// // namespace Controller {
// // 	Schedule* pills[4];
// // 	unsigned long prevLoopTime = 0UL;
// // 	unsigned long batteryStartMillis = 0UL;

// // 	void checkSchedules() {
// // 		byte day, hours, mins, elze;
// // 		Clock::readTime(&elze, &mins, &hours, &day, &elze, &elze, &elze);
// // 		for (Schedule* pill : pills) {
// // 			if (pill != NULL) {
// // 				if (pill->isAvaliable()) {
// // 					if (pill->isTime(day, hours, mins)) {
// // 						if (!pill->isTreggered()) {
// // 							pill->setTriggered(1);
// // 							Vibro::notifyPill(pill->_id);
// // 							pr("It's ");
// // 							pr(day);
// // 							pr(" ");
// // 							pr(hours);
// // 							pr(" ");
// // 							prl(mins);
// // 						}
// // 					} else {
// // 						pill->setTriggered(0);
// // 					}
// // 				}
// // 			}
// // 		}
// // 	}

// // 	void setTime(DynamicJsonDocument* doc) {
// // 		int sec, min, hour, dow, dom, month, year;
// // 		sec = (*doc)[Clock::SECONDS].as<int>();
// // 		min = (*doc)[Clock::MINUTES].as<int>();
// // 		hour = (*doc)[Clock::HOURS].as<int>();
// // 		dow = (*doc)[Clock::DAY_OF_WEEK].as<int>();
// // 		dom = (*doc)[Clock::DAY_OF_MONTH].as<int>();
// // 		month = (*doc)[Clock::MONTH].as<int>();
// // 		year = (*doc)[Clock::YEAR].as<int>();
// // 		Clock::setTime(sec, min, hour, dow, dom, month, year);
// // 	}

// // 	void setNewPill(DynamicJsonDocument* doc) {
// // 		Schedule* pill = Schedule::fromJson(doc);
// // 		if (pill != NULL) {
// // 			if (0 <= pill->_id && pill->_id < 4) {
// // 				pills[pill->_id] = pill;
// // 				Memory::writePill(pill);
// // 			}
// // 			for (Schedule* pill : pills) {
// // 				pill->printPill();
// // 			}
// // 		}
// // 	}

// // 	void processJson(std::string src) {
// // 		DynamicJsonDocument doc(1024);
// // 		deserializeJson(doc, src);
// // 		String aim = doc["type"].as<String>();
// // 		if (aim[0] == Schedule::TYPE) {
// // 			prl("New Pill was received");
// // 			setNewPill(&doc);
// // 		}
// // 		else if (aim[0] == Clock::TYPE) {
// // 			prl("Clock time update was received");
// // 			setTime(&doc);
// // 		}
// // 		else {
// // 			pr("Aim is unclear: ");
// // 			prl(aim);
// // 		}
// // 	}

// // 	void checkBattery() {
// // 		// int charge = Battery::getCharge(); // � %-��
// // 		// if (Battery::isLowBattery(charge) && !lowBatteryNotified) {
// // 		// 	lowBatteryNotified = 1;
// // 		// 	batteryStartMillis = millis();
// // 		// 	Vibro::notifyLowBattery();
// // 		// } else if (lowBatteryNotified && (millis() - batteryStartTime > 65*1000)) {
// // 		// 	lowBatteryNotified = 0;
// // 		// }
// // 		// pr("Charge: ");
// // 		// pr(charge);
// // 		// prl("%");
// // 	}

// // 	void wait() {
// // 		Vibro::go();
// // 	}

// // 	void loop() {
// // 		if (millis() - prevLoopTime > 2 * 1000) {
// // 			prevLoopTime = millis();
// // 			checkSchedules();
// // 			checkBattery();
// // 			Clock::printlnTime();
// // 		}
// // 		wait();
// // 	}

// // 	void init() {
// // 		// Load all pills
// // 		for (int i = 0; i < 4; i++) {
// // 			pills[i] = Memory::readPill(i);
// // 			pills[i]->printPill();
// // 		}
// // 	}
// // }

// // namespace BLE {
// // 	static const String TYPE = "type";

// // 	static const char* PILL_SERVICE_UUID = "C6FBDD3C-7123-4C9E-86AB-005F1A7EDA01";
// // 	static const int BATTARY_SERVICE_UUID = 0x180F;
// // 	static const char* RX_CHARACTERISTIC_UUID = "B88E098B-E464-4B54-B827-79EB2B150A9F";
// // 	static const char* TX_CHARACTERISTIC_UUID = "D769FACF-A4DA-47BA-9253-65359EE480FB";

// // 	BLECharacteristic* rxCh;
// // 	BLECharacteristic* txCh;
// // 	bool deviceConnected = false;

// // 	class BraceletServerCallbacks : public BLEServerCallbacks {
// // 		void onConnect(BLEServer* pServer) {
// // 			BLE::deviceConnected = true;
// // 			prl("Device connected");
// // 		}
// // 		void onDisconnect(BLEServer* pServer) {
// // 			BLE::deviceConnected = false;
// // 			prl("Device DISconnected");
// // 		}
// // 	};

// // 	class RxCharacteristicCallbacks : public BLECharacteristicCallbacks {
// // 		void onWrite(BLECharacteristic* pCharacteristic) {
// // 			std::string rxValue = pCharacteristic->getValue();
// // 			if (rxValue.length() > 0) {
// // 				pr("Received value: ");  //  "���������� ��������: "
// // 				for (int i = 0; i < rxValue.length(); i++) {
// // 					pr(rxValue[i]);
// // 				}
// // 				prl("");
// // 				Controller::processJson(rxValue);
// // 				prl("changes are applied");
// // 			}
// // 		}
// // 	};

// // 	class TxCharacteristicCallbacks : public BLECharacteristicCallbacks {
// // 		void onRead(BLECharacteristic* pCharacteristic) {
// // 			pr("txCharacteristic was read");
// // 		}
// // 	};

// // 	void setRx(const char* msg) {
// // 		rxCh->setValue(msg);
// // 		rxCh->notify();
// // 		//pCharacteristic->indicate();
// // 	}

// // 	std::string getTx() {
// // 		return txCh->getValue();
// // 	}

// // 	void init() {
// // 		BLEDevice::init("PillReminder");
// // 		BLEServer* server = BLEDevice::createServer();
// // 		server->setCallbacks(new BraceletServerCallbacks());

// // 		BLEService* braceletService = server->createService(PILL_SERVICE_UUID);

// // 		// Characteristic to Send data to App
// // 		txCh = braceletService->createCharacteristic(
// // 			TX_CHARACTERISTIC_UUID,
// // 			BLECharacteristic::PROPERTY_NOTIFY |
// // 			BLECharacteristic::PROPERTY_READ);
// // 		txCh->setCallbacks(new TxCharacteristicCallbacks());

// // 		// Characteristic to Reveive data from App
// // 		rxCh = braceletService->createCharacteristic(
// // 			RX_CHARACTERISTIC_UUID,
// // 			BLECharacteristic::PROPERTY_WRITE);
// // 		rxCh->setCallbacks(new RxCharacteristicCallbacks());

// // 		braceletService->start();
// // 		server->getAdvertising()->start();

// // 		prl("Waiting a client connection to connect with...");

// // 		//Set a message to rxCharacteristic
// // 		BLE::setRx("This device will conquer the world)");
// // 	}
// // }

// // void setup() {
// // 	Serial.begin(BAUD_RATE);
// // 	Memory::init();
// // 	Vibro::init();
// // 	Clock::init();
// // 	BLE::init();
// // 	Battery::init();
// // 	Controller::init();
// // }

// // //long tf = 0;
// // //int v = 7;
// // void loop() {
// // 	while (true) {
// // 		//prl("New cycle :" + Vibro::state);

// // 		Controller::loop();
// // 	}
// // }

// // #include <Arduino.h>
// // #include <EEPROM.h>
// // #include "Wire.h"
// // #include <BLEDevice.h>
// // #include <BLEUtils.h>
// // #include <BLEServer.h>
// // #include <BLE2902.h>
// // #include <set>
// // #include <utilities.h>

// // class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
// //   void onResult(BLEAdvertisedDevice advertisedDevice) {
// //     // проверяем, совпадает ли название
// //     // BLE-сервера, рассылающего оповещения:
// //     Serial.print("Device found: ");
// //     Serial.print(advertisedDevice.getName().c_str());
// //     Serial.print(". Service data: ");
// //     Serial.println(advertisedDevice.getServiceData().c_str());
// //   }
// // };

// // void setup() {
// // 	Serial.begin(BAUD_RATE);
// //   Serial.println("Application started");
// //   BLEDevice::init("scanner");
// //   BLEScan* pBLEScan = BLEDevice::getScan();
// //   pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
// //   pBLEScan->setActiveScan(true);
// //   pBLEScan->start(30);
// // }

// // void loop() {
// // 	while (true) {

// // 	}
// // }
