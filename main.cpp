 // Autor: Rafael Del Lama
 
#include <iostream>
#include "SerialClass.h"
#include <string>
#include <stdlib.h>
#include<cmath>

#define TAM 16

using namespace std;

int main(int argc, char** argv) {

	
	Serial *arduino;
	arduino = new Serial("COM4");
	
	if(arduino->IsConnected()){
		cout << "Conectado!" << endl;
		
		float s2[TAM] = {-0.001, 0.0, 0.0, 0.003, -0.001, 0.001, 0.0, 0.0, -0.001, -0.003, 0.002, 0.003, -0.001, 0.001, -0.003, 0.002};
        while(true) {
			/*
			for(int i = 0; i < TAM; i++){
				s2[i] = (float)(rand())/(float)(RAND_MAX)*1.0;
			}
			*/
			float result = arduino->calcFinessArduino(s2, TAM);
			cout << "Fitness " << result << endl;
		}
	}	
	return 0;
}
