/*
 * SerialClass.cpp
 */
 
 // Autor: Rafael Del Lama


#define ARDUINO_WAIT_TIME 2000

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include "SerialClass.h"

/** Construtor padr�o
*/
Serial::Serial(){
	
}
/** Construtor
*Recebe o nome da porta e tenta abrir uma conex�o.
*/
Serial::Serial(const char *portName) 
{
	//Status conex�o
    this->connected = false;
this->dcbPortaSerialparams = {0};
	this->timeouts = {0};
	this->PortaSerial = CreateFile(portName,
	GENERIC_READ|GENERIC_WRITE,     //para leitura e escrita
	  	0,							//outra abertura n�o ser� permitida
	   	NULL,						//atributo de seguranca(null)padr�o
	    OPEN_EXISTING, 				//cria��o ou abertura
		FILE_ATTRIBUTE_NORMAL, 		//ouverlapped
		NULL );
	
this->dcbPortaSerialparams.DCBlength = sizeof(this->dcbPortaSerialparams); 
	if (GetCommState(this->PortaSerial, &this->dcbPortaSerialparams) == 0)
    {
        fprintf(stderr, "Error de estado de dispositivo\n");
        CloseHandle(this->PortaSerial);        
    }
     
    this->dcbPortaSerialparams.BaudRate = CBR_9600;		   //velocidade
    this->dcbPortaSerialparams.ByteSize = 8;			   //tamanho do dado
    this->dcbPortaSerialparams.StopBits = ONESTOPBIT;	   //bits de parada
    this->dcbPortaSerialparams.Parity =  NOPARITY;		   //paridde
    if(SetCommState(this->PortaSerial, &this->dcbPortaSerialparams) == 0)
    {
        fprintf(stderr, "Parametro de dispositivo errado\n");
        CloseHandle(this->PortaSerial);        
    }
    else
    	this->connected = true;
    this->timeouts.ReadIntervalTimeout = 50;
    this->timeouts.ReadTotalTimeoutConstant = 50;
    this->timeouts.ReadTotalTimeoutMultiplier = 10;
    this->timeouts.WriteTotalTimeoutConstant = 50;
    this->timeouts.WriteTotalTimeoutMultiplier = 10;
    if(SetCommTimeouts(this->PortaSerial, &this->timeouts) == 0)
    {
        fprintf(stderr, "timeout configurado errado\n");
        CloseHandle(this->PortaSerial);
    }
	Sleep(2000); //Sleep de 2s para esperar o arduino estar pronto para receber msg  
}//Serial

/** Destrutor
* Se tiver uma conex�o estabelecida, fecha a conex�o
*/
Serial::~Serial()
{
	Close();
}// ~Serial

/* Fechar a serial
*
*/
void Serial::Close()
{
	fprintf(stderr, "\nFechando porta serial...");
	if(this->connected)
    {
        //We're no longer connected
        this->connected = false;
        //Close the serial handler
     	if (CloseHandle(this->PortaSerial) == 0)
    	{
        	fprintf(stderr, "Error\n");        
    	}
    	fprintf(stderr, "OK\n");
    }
}//Close

/** 
* Le dados atrav�s da conex�o
*/
void Serial::ReadData(char *bytes)
{
	bool boolean = true;
	int i = 0;
	
	while (PortaSerial != INVALID_HANDLE_VALUE && boolean){
		DWORD bytes_lidos;
	    if (!ReadFile(this->PortaSerial,		//identificador da porta serial
					this->bytes_para_receber,	//buffer, local tempor�rio onde os dados lidos ser�o armazenados 
					1,					        //tamanho do buffer
					&bytes_lidos,		        //ponteiro onde armazenar� a quantidade exata de bytes lidos
					NULL))				        //ponteiro para overlapped
		{
			fprintf(stderr, "Erro\n");
			CloseHandle(this->PortaSerial);		
		} 
		else 
		{
			if (bytes_lidos > 0){
				if(bytes_para_receber[0] == '\n')
					boolean = false;
			
				bytes[i] =  bytes_para_receber[0];
				i++;
			}
		}
	}
}//ReadData

/** 
* Envia dados atrav�s da conex�o
* buffer cont�m os dados a serem enviador
*/
void Serial::WriteData(const char *buffer)
{
	int m;
	m = strlen(buffer);
		
	for (int i = 0 ; i != m ; i++)
	{
	    bytes_pra_envio[i] = buffer[i];
	}
   int tamanho = m;

	DWORD bytes_written, total_bytes_written = 0;

    if(!WriteFile(this->PortaSerial,		//identificador da porta serial
				  this->bytes_pra_envio,	//buffer onde ser� depositado para serem enviados a porta serial
				  tamanho,					//tamanho do buffer de dados
				  &bytes_written,	        //ponteiro onde armazena a quantidade exata de bytes escritos
				  NULL))				    //ponteiro para overlapped
    {
        fprintf(stderr, "Error\n");
        CloseHandle(this->PortaSerial);        
    }   
   
}
/** 
* Retorna o status da conex�o
*/
bool Serial::IsConnected()
{
    return this->connected;
}

float Serial::calcFinessArduino(float *indiv, int tam) 
{
	char convertido[8];
    for(int i = 0; i < tam; i++){
		sprintf(convertido, "%.6f", indiv[i]); // converte float para char[]
		WriteData(convertido);
	}
    
   	char bytes[20];
	ReadData(bytes);

	return atof(bytes);	
}


