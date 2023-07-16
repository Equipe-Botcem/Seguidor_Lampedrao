#include "Seguidor.h"
#include "Arduino.h"


//----------------------- Construtores -----------------------//

Seguidor::Seguidor()
{
}

//----------------------- Configs e inits -----------------------//


void Seguidor::Config_led_esq(unsigned char pin)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

void Seguidor::Config_led_dir(unsigned char pin)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

void Seguidor::Config_motors(unsigned char *pins_dir, unsigned char *pins_esq)
{
	driver = Driver(pins_dir, pins_esq);
}

void Seguidor::Config_sensor_linha(unsigned char *pins)
{	
	sensor_linha = Sensor_linha(pins);
}

void Seguidor::Config_sensor_esq(unsigned char pin)
{
	sensor_esq = Sensor(pin);
}

void Seguidor::Config_sensor_dir(unsigned char pin)
{
	sensor_dir = Sensor(pin);
}

void Seguidor::Config_pins()
{

	Config_led_esq(led_esq);
	Config_led_esq(led_dir);
	Config_motors(pins_motor_drive_dir, pins_motor_drive_esq);
	Config_sensor_linha(pins_sensor_linha);
	Config_sensor_esq(pin_sensor_esq);
	Config_sensor_dir(pin_sensor_dir);
}

void Seguidor::initBluetooth(){
    SerialBT.begin("ESP32");
  	Serial.println("O dispositivo já pode ser pareado ou conectado!");
}

void Seguidor::Init()
{
	Config_pins();

	driver.Init();
	sensor_linha.Init();
	sensor_esq.Init();
	sensor_dir.Init();

	controlador.setControlador(1, 0, 0.1);
	controlador.resetConditions();
}

void Seguidor::set_handler()
{
	String VB = "", KI_str = "", KP_str = "", KD_str = "", lixo_str = "";
	int pos = command.indexOf(',', 2);
	Serial.println(command);
	for (int i = 4; i < pos; i++)
		VB += command[i];

	int pos2 = command.indexOf(',', pos + 1);
	for (int i = pos + 3; i < pos2; i++)
		KI_str += command[i];

	pos = command.indexOf(',', pos2 + 1);
	for (int i = pos2 + 3; i < pos; i++)
		KP_str += command[i];

	pos2 = command.indexOf(',', pos + 1);
	for (int i = pos + 3; i < pos2; i++)
		KD_str += command[i];

	pos = command.indexOf(',', pos2 + 1);
	for (int i = pos2 + 3; i < pos; i++)
		lixo_str += command[i];

	
	// Configura osf parâmetros do controlador  
	driver.setVB_reta(VB.toInt());
	controlador.setKp(KP_str.toDouble() / 10000);
	controlador.setKd(KD_str.toDouble() / 10000);
	controlador.setKi(KI_str.toDouble() / 10000);


	// Bluetooth check

	SerialBT.print("VB:");
	SerialBT.print(VB);
	SerialBT.print(" ");

	SerialBT.print("KI:");
	SerialBT.print(KI_str);
	SerialBT.print(" ");

	SerialBT.print("KP:");
	SerialBT.print(KP_str);
	SerialBT.print(" ");

	SerialBT.print("KD:");
	SerialBT.print(KD_str);
	SerialBT.print(" ");

}

//----------------------- Other Functions -----------------------//
void Seguidor::calibration()
{	
	unsigned long tempo;

	tempo = millis();

	while(millis() - tempo < 300){

		driver.Enable_motors_drives();
		driver.Set_speedTrans(-80);

		sensor_linha.calibration_max();

		sensor_esq.find_max();
		sensor_dir.find_max();
	}
	driver.Disable_motors_drives();

	tempo = millis();
	while(millis() - tempo < 300){
		driver.Enable_motors_drives();

		driver.Set_speedTrans(80);

		sensor_linha.calibration_min();

		sensor_esq.find_min();
		sensor_dir.find_min();
	}

	driver.Disable_motors_drives();


}

void Seguidor::controle(){	
	// Taxa de amostragem 
	if (!samplingTime) samplingTime = millis();
	else if(millis() - samplingTime >= controlador.getAmostragem()){
		samplingTime = 0;

		float erro = sensor_linha.getAngle();

		int rot = controlador.calcPID(erro);

		// Atua nos motores conforme a pista 
		driver.Set_speedTrans(rot);
	}
	
}

void Seguidor::mapeamento(){
	if(Check_latEsq){
		if(millis() - latEsqTime > 3000){
			latEsqTime = millis();
			driver.setVB_reta(60);
		}	
	}
	
}

void Seguidor::stopRoutine(){
	// Para o seguidor no final da pista 
	if(millis() - startTime > 5000){
		if (Check_stop() and isEnd() == false){
			end = true;
			stopTime = millis();
		}
		else if (millis() - stopTime > 300) Stop();
	}
}

void Seguidor::Run()
{
	driver.Enable_motors_drives();
	start = true;
	stopTime = millis();
	bool fimPista = false;
	controlador.resetConditions();
}

void Seguidor::Stop(){
	driver.Disable_motors_drives();
	start = false;
}

void Seguidor::Behavior()
{
	comunica_serial();

	switch (command[0])
	{
	case 'S':
		set_handler();
		command = "";
		break;
	case 'P':
		Stop();
		command = "";
		break;
	case 'R':
		Run();
		command = "";
		break;
	case 'C':
		calibration();
		command = "";
		break;
	default:
		command = "";
		break;
	}
}

void Seguidor::comunica_serial(){
	if(SerialBT.available()){ 
		command = SerialBT.readStringUntil(';');
	}
}

bool Seguidor::Check_stop(){

	if(sensor_dir.Read_sensor() >= RESOLUTION*0.5 and sensor_esq.Read_sensor() <= RESOLUTION*0.1) return true;
	
	return false;
}		

bool Seguidor::Check_latEsq(){

	if(sensor_esq.Read_sensor() >= RESOLUTION*0.5 and sensor_dir.Read_sensor() <= RESOLUTION*0.1) return true;
	
	return false;
}
	
void Seguidor::teste(){
	//Serial.print(controlador.calcPID();
	// Serial.print("        ");
	//Serial.println(sensor_linha.getAngle());
	
	//controle();
	//sensor_linha.testeLeitura(sensor_linha.HIST);
	// float erro = sensor_linha.getAngle() - 2*rot_k1;

	// int rot = controlador.calcPID(erro);

	// rot_k1 = rot;

	// Serial.print(erro);
	// Serial.print("  ");
	// Serial.println(rot);

	delay(1);
}

bool Seguidor::isEnd(){
	return end;
}

bool Seguidor::isStart(){
	return start;
}

