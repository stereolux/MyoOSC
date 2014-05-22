// Nom : Data.cpp
// Utilisation : définit le point d'entrée pour l'application console.
// Date de création : 17/05/2014
// Auteur : Thomas BLAVETTE, Sofiane BOURAHLA, Guillaume BOULIN, Matthieu MIGEON
// et Mathieu VAISSIERES
//
// Application modifié par des étudiants de Polytech Nantes en partenariat
// avec Stéréolux. Il s'agit d'envoyer les différentes données par le Myo
// via le protocole OSC.

#include "stdafx.h"
#include "Data.h"
#include <myo.hpp>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;
using namespace myo;

#define M_PI       3.14159265358979323846

Data::Data()
{
}


Data::~Data()
{
}

void Data::onAccelerometerData(Myo *myo, uint64_t timestamp, const Vector3< float > &accel)//Appeller par le sdk pour actualiser les valeurs d'accélérations
{
	acceleration = accel;
}


void Data::onGyroscopeData(Myo *myo, uint64_t timestamp, const Vector3< float > &gyro)//Appeller par le sdk pour actualiser les valeurs de gyroscope
{
	gyroscope = gyro;
}

void Data::onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)//Appeller par le sdk pour actualiser les valeurs de position
{
	position = quat;
}

void Data::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)//Appeller par le sdk pour actualiser le mouvement effectués par l'utilisateur
{
	current = pose;
}

void Data::CalculEuler()//Calculer les angles d'euler
{
        using std::atan2;
        using std::asin;
        using std::sqrt;

        // Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
        euler[0] = atan2(2.0f * (position.w() * position.x() + position.y() * position.z()),
                           1.0f - 2.0f * (position.x() * position.x() + position.y() * position.y()));
		euler[1] = asin(2.0f * (position.w() * position.y() - position.z() * position.x()));
		euler[2] = atan2(2.0f * (position.z() * position.z() + position.x() * position.y()),
			1.0f - 2.0f * (position.y() * position.y() + position.z() * position.z()));

        // Convert the floating point angles in radians to a scale from 0 to 255.

        euler[0] = static_cast<int>((euler[0] + (float)M_PI)/(M_PI * 2.0f) * 1024);
        euler[1] = static_cast<int>((euler[1] + (float)M_PI/2.0f)/M_PI * 1024);
        euler[2] = static_cast<int>((euler[2] + (float)M_PI)/(M_PI * 2.0f) * 1024);
}


void Data::Connect()//Procédure de connexion au myo
{

	try{
		myMyo = hub.waitForAnyMyo(10000);//Attendre une connexion à un myo
		if (myMyo == NULL)
		{
			std::cout << "Probleme de connection au Myo" << std::endl;
			getchar();
			exit(1);
		}
		else {
			std::cout << "Connected to " << myMyo->macAddressAsString() << std::endl;//Affiche l'adresse Mac du myo 
			myMyo->vibrate(myo::Myo::VibrationMedium);//Vibration pour signaler à l'utilisateur que la connexion a réussi
		}
	}
	catch (const std::exception e)
	{
		std::cout << "error" << std::endl;
	}

	hub.addListener(this);
}

void Data::run(unsigned int ms)//Actualiser les valeurs du myo toutes les n ms
{
	hub.run(ms);
}

void Data::getAccelerometer(float *acc)//Transformer les vectors 3d en tableau de float
{
	acc[0] = acceleration.x();
	acc[1] = acceleration.y();
	acc[2] = acceleration.z();
}

void Data::getGyroscope(float *gyr)//Transformer les vectors 3d en tableau de float
{
	gyr[0] = gyroscope.x();
	gyr[1] = gyroscope.y();
	gyr[2] = gyroscope.z();
}

void Data::getPosition(float *pos)//Transformer les vectors 3d en tableau de float
{
	pos[0] = position.x();
	pos[1] = position.y();
	pos[2] = position.z();
	pos[3] = position.w();
}

void Data::getEuler(float *eul)//Transformer les vectors 3d en tableau de float
{
	eul[0] = euler[0];
	eul[1] = euler[1];
	eul[2] = euler[2];
}

string Data::getGesture()//Retouner la chaine de caractère contenant la gesture
{
	return current.toString();
}
