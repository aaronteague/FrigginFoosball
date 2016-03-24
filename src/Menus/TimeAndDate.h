#include "gameplay.h"
#include "../Menu.h"

using namespace gameplay;

struct Holiday{
	std::string name;
	int month;
	int day;
	Holiday(std::string name, int month, int day)
		:name(name), month(month), day(day){}
};

class TimeAndDate : public Menu{
public:
	enum Seasons{
		SPRING,
		SUMMER,
		AUTUMN,
		WINTER
	};
	enum Months{
		JANUARY,
		FEBUARY,
		MARCH,
		APRIL,
		MAY,
		JUNE,
		JULY,
		AUGUST,
		SEPTEMBER,
		OCTOBER,
		NOVEMBER,
		DECEMBER
	};
	TimeAndDate(const Vector2 &screenSize);

	luabridge::LuaRef save();
	void load(luabridge::LuaRef &saveTable);
	int getDaysSinceLastPlayed();
	int getSeason();
	int getHour();
	std::string getHoliday();

	virtual void Update(const float& elapsedTime);
	virtual void Render();

	virtual void prepSelf();
	virtual void addToGame(Node* node, Texture* tex, Scene* _scene);

	virtual bool touchPress(Vector2 position);
	virtual void touchMove(Vector2 position);
	virtual void touchRelease(Vector2 position);
private:
	std::vector<Holiday> holidayList;

	float idleTimer;
	struct tm currentTime;
	struct tm firstTimePlayed;
	struct tm lastTimePlayed;
	int daysSinceLastPlayed;
	std::vector<std::string> monthChoices;
	std::string currentMonthChoice;
	std::string currentDayStr;
	// int weather
	std::string timeString;
	gameplay::Rectangle onScreenClockRec;
	gameplay::Rectangle timeRec;
	gameplay::Rectangle monthRec;
	gameplay::Rectangle dayRec;
	Font* _font;
	gameplay::Rectangle imageClockRec;

	TimeAndDate();

	void setupHolidays();

	const float TIME_TIL_SHOW = 1000;
};