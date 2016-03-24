#pragma once

#include "Gameplay.h"

using namespace gameplay;

class MyCurve{
public:
	MyCurve(Curve::InterpolationType curveType);
	~MyCurve();

	void addPoint(float keyTime, float v1);
	void addPoint(float keyTime, float v1, float v2);
	void addPoint(float keyTime, float v1, float v2, float v3);
	void addPoint(float keyTime, float v1, float v2, float v3, float v4);

	void evaluate(const float &time, float &v1);
	void evaluate(const float &time, float &v1, float &v2);
	void evaluate(const float &time, float &v1, float &v2, float &v3);
	void evaluate(const float &time, float &v1, float &v2, float &v3, float &v4);

	bool isComplete();
private:
	int numOfPoints;
	int numOfValues;
	float totalTime;
	float runningTime;
	Curve::InterpolationType curveType;
	Curve* _curve;

	std::vector<float*> points;
	std::vector<float> times;
};