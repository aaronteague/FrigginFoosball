#include "MyCurve.h"

MyCurve::MyCurve(Curve::InterpolationType curveType)
{
	_curve = NULL;
	numOfPoints = 0;
	totalTime = 0;
	runningTime = 0;
	numOfValues = 0;
	MyCurve::curveType = curveType;
}

MyCurve::~MyCurve(){
	if (_curve != NULL)
		_curve->release();
	//for (unsigned int i = 0; i < points.size(); ++i){
	//	if (numOfValues > 1)
	//		delete[] points[i];
	//	else
	//		delete points[i];
	//}

}

void MyCurve::addPoint(float keyTime, float v1)
{
	numOfPoints++;
	numOfValues = 1;
	float *temp = new float;
	*temp = v1;
	points.push_back(temp);

	totalTime += keyTime;
	times.push_back(totalTime);
}
void MyCurve::addPoint(float keyTime, float v1, float v2)
{
	numOfPoints++;
	numOfValues = 2;
	float *temp = new float[numOfValues];
	temp[0] = v1;
	temp[1] = v2;
	points.push_back(temp);

	totalTime += keyTime;
	times.push_back(totalTime);
}
void MyCurve::addPoint(float keyTime, float v1, float v2, float v3)
{
	numOfPoints++;
	numOfValues = 3;
	float *temp = new float[numOfValues];
	temp[0] = v1;
	temp[1] = v2;
	temp[2] = v3;
	points.push_back(temp);
	times.push_back(keyTime);
	totalTime += keyTime;
}
void MyCurve::addPoint(float keyTime, float v1, float v2, float v3, float v4)
{
	numOfPoints++;
	numOfValues = 4;
	float *temp = new float[numOfValues];
	temp[0] = v1;
	temp[1] = v2;
	temp[2] = v3;
	temp[3] = v4;
	points.push_back(temp);

	totalTime += keyTime;
	times.push_back(totalTime);
}

void MyCurve::evaluate(const float &time, float &v1)
{
	if (_curve == NULL){
		_curve = Curve::create(numOfPoints, numOfValues);

		for (unsigned int i = 0; i < points.size(); ++i)
			_curve->setPoint(i, times[i] / totalTime, points[i], curveType);
	}

	float* destination = new float;
	_curve->evaluate(runningTime / totalTime, destination);
	v1 = *destination;
	delete destination;
	runningTime += time;
}
void MyCurve::evaluate(const float &time, float &v1, float &v2)
{
	if (_curve == NULL){
		_curve = Curve::create(numOfPoints, numOfValues);
		for (unsigned int i = 0; i < points.size(); ++i)
			_curve->setPoint(i, times[i] / totalTime, points[i], curveType);
	}

	float* destination = new float[numOfValues];
	_curve->evaluate(runningTime / totalTime, destination);
	v1 = destination[0];
	v2 = destination[1];
	delete[] destination;
	runningTime += time;
}
void MyCurve::evaluate(const float &time, float &v1, float &v2, float &v3)
{
	//float *testArray = new float[3];
	//testArray[0] = 7;
	//testArray[1] = 8;
	//testArray[2] = 3;

	if (_curve == NULL){
		_curve = Curve::create(numOfPoints, numOfValues);
		for (unsigned int i = 0; i < points.size(); ++i){
			_curve->setPoint(i, times[i] / totalTime, points[i], curveType);
		//	float *test = points[i];
			//delete[] test;
		}
	}

	float* destination = new float[numOfValues];
	_curve->evaluate(runningTime / totalTime, destination);
	v1 = destination[0];
	v2 = destination[1];
	v3 = destination[2];
	delete[] destination;
	runningTime += time;


}
void MyCurve::evaluate(const float &time, float &v1, float &v2, float &v3, float &v4)
{
	if (_curve == NULL){
		_curve = Curve::create(numOfPoints, numOfValues);
		for (unsigned int i = 0; i < points.size(); ++i)
			_curve->setPoint(i, times[i] / totalTime, points[i], curveType);
	}

	float* destination = new float[numOfValues];
	_curve->evaluate(runningTime / totalTime, destination);
	v1 = destination[0];
	v2 = destination[1];
	v3 = destination[2];
	v4 = destination[3];
	delete[] destination;
	runningTime += time;
}

bool MyCurve::isComplete()
{
	return runningTime > totalTime;
}