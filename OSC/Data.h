#pragma once
#include <myo.hpp>
#include <iostream>
#include <fstream>
using namespace myo;
class Data : public myo::DeviceListener
{
public:
	Data();
	~Data();
	void onAccelerometerData(Myo *myo, uint64_t timestamp, const Vector3< float > &accel);
	void print();
	void onGyroscopeData(Myo *myo, uint64_t timestamp, const Vector3< float > &gyro);
	void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat);
	void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose);
	void CalculEuler();
	void Save();
	void Connect();
	Pose current;
	void run(unsigned int ms);
	void getAccelerometer(float *acc);
	void getGyroscope(float *gyr);
	void getPosition(float *pos);
	void getEuler(float *eul);
	std::string getGesture();
private:
	Vector3<float> acceleration;
	Vector3<float> gyroscope;
	float euler[3];
	Quaternion<float> position;

	Hub hub;
	Myo *myMyo;

};

