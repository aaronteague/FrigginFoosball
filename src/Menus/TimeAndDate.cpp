#include "TimeAndDate.h"
#include "../FSM.h"
#include <time.h>

TimeAndDate::TimeAndDate(const Vector2 &screenSize)
	:Menu(screenSize)
{
	const int MENU_WIDTH = 200;
	const int MENU_HEIGHT = 50;
	const std::string MENU_TEX = "res/Menu/Time.png";

	imageClockRec.set(0, 0, 2048, 610);

	stateMachine = new FSM(this);
	// stateMachine->SetCu
	idleTimer = 0;
	onScreenClockRec.set
		( screenSize.x / 2 - MENU_WIDTH / 2 * scale.x
		, 0
		, MENU_WIDTH * scale.x
		, MENU_HEIGHT * scale.x);

	timeRec.set(onScreenClockRec.x + 5 * scale.x
		, 0
		, onScreenClockRec.width / 2
		, onScreenClockRec.height);
	monthRec.set(onScreenClockRec.x + onScreenClockRec.width / 2
		, 0
		, onScreenClockRec.width / 2
		, onScreenClockRec.height / 2);
	dayRec.set(onScreenClockRec.x + onScreenClockRec.width / 2
		, onScreenClockRec.height / 2
		, onScreenClockRec.width / 2
		, onScreenClockRec.height / 2);
	textureF = MENU_TEX;


//	time_t curTime = time(0);
//	currentTime = *localtime(&curTime);

	daysSinceLastPlayed = -1;

	monthChoices.reserve(12);
	monthChoices.push_back("JAN");
	monthChoices.push_back("FEB");
	monthChoices.push_back("MAR");
	monthChoices.push_back("APR");
	monthChoices.push_back("MAY");
	monthChoices.push_back("JUN");
	monthChoices.push_back("JUL");
	monthChoices.push_back("AUG");
	monthChoices.push_back("SEP");
	monthChoices.push_back("OCT");
	monthChoices.push_back("NOV");
	monthChoices.push_back("DEC");

	setupHolidays();

	// until we load up the actual first time played, this will be default
	
	time_t curTime = time(0);
	currentTime = firstTimePlayed = *localtime(&curTime);
}

luabridge::LuaRef TimeAndDate::save()
{
	luabridge::LuaRef ftp = luabridge::newTable(L);
	ftp["year"] = firstTimePlayed.tm_year;
	ftp["month"] = firstTimePlayed.tm_mon;
	ftp["day"] = firstTimePlayed.tm_mday;
	ftp["hour"] = firstTimePlayed.tm_hour;
	ftp["minute"] = firstTimePlayed.tm_min;
	ftp["second"] = firstTimePlayed.tm_sec;

	time_t curTime = time(0);
	lastTimePlayed = *localtime(&curTime);

	luabridge::LuaRef ltp = luabridge::newTable(L);
	ltp["year"] = lastTimePlayed.tm_year;
	ltp["month"] = lastTimePlayed.tm_mon;
	ltp["day"] = lastTimePlayed.tm_mday;
	ltp["hour"] = lastTimePlayed.tm_hour;
	ltp["minute"] = lastTimePlayed.tm_min;
	ltp["second"] = lastTimePlayed.tm_sec;

	luabridge::LuaRef time = luabridge::newTable(L);
	time["firstTimePlayed"] = ftp;
	time["lastTimePlayed"] = ltp;

	return time;
}
void TimeAndDate::load(luabridge::LuaRef &saveTable)
{
	time_t curTime = time(0);

	currentTime = *localtime(&curTime);


	luabridge::LuaRef ftp = saveTable["firstTimePlayed"];
	firstTimePlayed.tm_year = ftp["year"];
	firstTimePlayed.tm_mon = ftp["month"];
	firstTimePlayed.tm_mday = ftp["day"];
	firstTimePlayed.tm_hour = ftp["hour"];
	firstTimePlayed.tm_min = ftp["minute"];
	firstTimePlayed.tm_sec = ftp["second"];

	luabridge::LuaRef ltp = saveTable["firstTimePlayed"];
	lastTimePlayed.tm_year = ltp["year"];
	lastTimePlayed.tm_mon = ltp["month"];
	lastTimePlayed.tm_mday = ltp["day"];
	lastTimePlayed.tm_hour = ltp["hour"];
	lastTimePlayed.tm_min = ltp["minute"];
	lastTimePlayed.tm_sec = ltp["second"];

	const int SECONDS_IN_A_DAY = 86400;

	double seconds = difftime(curTime, mktime(&lastTimePlayed));
	int days = seconds / SECONDS_IN_A_DAY;

	if (lastTimePlayed.tm_hour > currentTime.tm_hour)
		days++;
	daysSinceLastPlayed = days;
}
int TimeAndDate::getDaysSinceLastPlayed()
{
	return daysSinceLastPlayed;
}

void TimeAndDate::Update(const float& elapsedTime)
{
	idleTimer += elapsedTime;
	// stateMachine->Update(elapsedTime);
	time_t curTime = time(0);
	currentTime = *localtime(&curTime);
	timeString.clear();
	int hour = currentTime.tm_hour;
	if (hour > 12)
		hour -= 12;
	timeString = to_string(hour);
	timeString += ":";
	int minute = currentTime.tm_min;
	if (minute < 10)
		timeString += "0";
	timeString += to_string(minute);

	currentMonthChoice = monthChoices[currentTime.tm_mon];
	currentDayStr = to_string(currentTime.tm_mday);
}
void TimeAndDate::Render()
{
	if (idleTimer > TIME_TIL_SHOW){
		_menuBatch->start();
		_menuBatch->draw(onScreenClockRec, imageClockRec);
		_menuBatch->finish();
		_font->start();
		_font->drawText
			(timeString.c_str(), timeRec
			, Vector4(1, 1, 1, 1)
			, _font->getSize() * scale.x
			, gameplay::Font::ALIGN_HCENTER, false, false);
		_font->drawText(currentMonthChoice.c_str(), monthRec, Vector4(1, 1, 1, 1), _font->getSize() *0.5 * scale.x
			, gameplay::Font::ALIGN_HCENTER, false, false);
		_font->drawText(currentDayStr.c_str(), dayRec, Vector4::one(), _font->getSize() * 0.5 * scale.x
			, gameplay::Font::ALIGN_HCENTER, false, false);
		_font->finish();
	}
}

void TimeAndDate::prepSelf()
{
	sendToLists(textureF, "none", "none", this);
}
void TimeAndDate::addToGame(Node* node, Texture* tex, Scene* _scene)
{
	_menuBatch = SpriteBatch::create(tex);
	_font = Font::create("res/Menu/menu_font.gpb");
}

bool TimeAndDate::touchPress(Vector2 position)
{
	return false;
}
void TimeAndDate::touchMove(Vector2 position)
{

}
void TimeAndDate::touchRelease(Vector2 position)
{

}

int TimeAndDate::getSeason(){
	switch (currentTime.tm_mon){
	case 11:
	case 0:
	case 1:
		return WINTER;
	case 2:
	case 3:
	case 4:
		return SPRING;
	case 5:
	case 6:
	case 7:
		return SUMMER;
	case 8:
	case 9:
	case 10:
		return AUTUMN;
	default:
		return -1;
	}
}

int TimeAndDate::getHour()
{
	return currentTime.tm_hour;
}

std::string TimeAndDate::getHoliday()
{
	std::string holiday = "none";

	for (unsigned int i = 0; i < holidayList.size(); ++i)
	if (holidayList[i].month == currentTime.tm_mon
		&& holidayList[i].day == currentTime.tm_mday){
		holiday = holidayList[i].name;
		break;
	}
	
	return holiday;
}

void TimeAndDate::setupHolidays()
{
	const int NUM_OF_HOLIDAYS = 8;
	holidayList.reserve(NUM_OF_HOLIDAYS);

	holidayList.push_back(Holiday("Thanksgiving", NOVEMBER, 26));
	holidayList.push_back(Holiday("Christmas Eve", DECEMBER, 24));
	holidayList.push_back(Holiday("Christmas", DECEMBER, 25));
	holidayList.push_back(Holiday("New Years Eve", DECEMBER, 31));
	holidayList.push_back(Holiday("New Years", JANUARY, 1));
	holidayList.push_back(Holiday("Valentines Day", FEBUARY, 14));
	holidayList.push_back(Holiday("Halloween", OCTOBER, 31));
	holidayList.push_back(Holiday("Easter", APRIL, 6));
}