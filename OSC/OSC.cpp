// Nom : OSC.cpp
// Utilisation : définit le point d'entrée pour l'application console.
// Date de création : 17/05/2014
// Auteurs : Thomas BLAVETTE, Sofiane BOURAHLA, Guillaume BOULIN, Matthieu MIGEON
// et Mathieu VAISSIERES
//
// Application developper par des étudiants de Polytech Nantes en partenariat
// avec Stéréolux. Il s'agit d'envoyer les différentes données par le Myo
// via le protocole OSC.


#include "stdafx.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "Data.h"
#include <iostream>
#include <stdio.h>
#include <istream>

#define ADDRESS "127.0.0.1"
#define PORT 7000

#define X 0
#define Y 1
#define Z 2
#define W 3

#define OUTPUT_BUFFER_SIZE 2048

int _tmain(int argc, _TCHAR* argv[])
{
	try {
		/*Initialisation des variables*/
		Data myo;
		float accelerometer[3];
		float gyroscope[3];
		float euler[3];
		float position[4];
		std::string pose;
		std::string adresse;
		std::string test;
		int port = 0;

		char buffer[OUTPUT_BUFFER_SIZE];
		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
		/*Configuration du serveur OSC*/
		std::cout << "Adresse par defaut : 127.0.0.1" << std::endl;
		std::cout << "Nouvelle adresse [ laisser vide pour utiliser adresse par defaut ] : ";
		std::getline(std::cin, adresse);
		if (adresse.empty()) adresse = ADDRESS;

		std::cout << "Port par defaut : 7000" << std::endl;
		std::cout << "Nouveau port [ laisser vide pour utiliser adresse par defaut ] : ";
		std::getline(std::cin, test);
		port = atoi(test.c_str());
		if (port == 0) port = PORT;

		myo.Connect();//Connexion au myo
		try{
			UdpTransmitSocket transmitSocket(IpEndpointName(adresse.c_str(), port));
			std::cout << "Accelerometre vecteur : /myo/accelerometer" << std::endl;
			std::cout << "Gyroscope vecteur : /myo/gyroscope" << std::endl;
			std::cout << "Position vecteur : /myo/position" << std::endl;
			std::cout << "Euler vecteur : /myo/euler" << std::endl;
			std::cout << "Gesture string : /myo/gesture" << std::endl;
			while (1)
			{	//Récupération des valeurs du myo
				myo.run(5);
				myo.getAccelerometer(accelerometer);
				myo.getGyroscope(gyroscope);
				myo.getPosition(position);
				myo.CalculEuler();
				myo.getEuler(euler);
				pose = myo.getGesture();
				//Préparation du message OSC
				p << osc::BeginBundleImmediate
					<< osc::BeginMessage("/myo/accelerometer")
					<< accelerometer[X]
					<< accelerometer[Y]
					<< accelerometer[Z] << osc::EndMessage
					<< osc::BeginMessage("/myo/gyroscope")
					<< gyroscope[X]
					<< gyroscope[Y]
					<< gyroscope[Z] << osc::EndMessage
					<< osc::BeginMessage("/myo/position")
					<< position[X]
					<< position[Y]
					<< position[Z]
					<< position[W] << osc::EndMessage
					<< osc::BeginMessage("/myo/euler")
					<< euler[X]
					<< euler[Y]
					<< euler[Z] << osc::EndMessage
					<< osc::BeginMessage("/myo/gesture")
					<< pose.c_str() << osc::EndMessage
					<< osc::EndBundle;
				transmitSocket.Send(p.Data(), p.Size());//Transmission des données
				p.Clear(); //Effacer le buffer
			}
		}
		catch (std::runtime_error e)
		{
			std::cout << "Erreur de socket" << std::endl;//Erreur relative à l'ouverture de socket pour transmission OSC
			system("PAUSE");
			exit(0);
		}
	}
	catch (std::exception& e){
		std::cerr << e.what() << std::endl;//Erreur relative au myo
		std::cerr << "Press enter to continue.";
		std::cin.ignore();
	}



}


