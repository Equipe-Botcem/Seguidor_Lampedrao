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

	driver.Enable_motors_drives();


	int c_max[8];
	c_max[0] = 1300;
	c_max[1] = 1230; 
	c_max[2] = 1500;
	c_max[3] = 1460;
	c_max[4] = 980;
	c_max[5] = 1400;
	c_max[6] = 1500;
	c_max[7] = 1400;

	int c_min[8];
	c_min[0] = 0;
	c_min[1] = 0; 
	c_min[2] = 0;
	c_min[3] = 0;
	c_min[4] = 0;
	c_min[5] = 0;
	c_min[6] = 0;
	c_min[7] = 0;


	sensor_linha.calibation_manual(c_max, c_min);
	sensor_esq.Cmax = 1220;
	sensor_esq.Cmin = 0;
	sensor_dir.Cmax = 800;
	sensor_dir.Cmin = 0;
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

	
	// Configura os parâmetros do controlador  
	driver.setVB(VB.toInt());
	controlador.setKp(KP_str.toDouble() / 100);
	controlador.setKd(KD_str.toDouble() / 100);
	controlador.setKi(KI_str.toDouble() / 100);


	// Bluetooth check

	SerialBT.print("VB:");
	SerialBT.print(VB);
	SerialBT.print(" ");

	SerialBT.print("KI:");
	SerialBT.print(KI_str.toDouble() / 100);
	SerialBT.print(" ");

	SerialBT.print("KP:");
	SerialBT.print(KP_str.toDouble() / 100);
	SerialBT.print(" ");

	SerialBT.print("KD:");
	SerialBT.print(KD_str.toDouble() / 100);
	SerialBT.print(" ");

}

//----------------------- Other Functions -----------------------//
void Seguidor::calibration()
{	
	unsigned long tempo;

	tempo = millis();

	while(millis() - tempo < 300){
		driver.setMotors(-80, -80);

		sensor_linha.calibration_max();

		sensor_esq.find_max();
		sensor_dir.find_max();
	}
	driver.Break();

	tempo = millis();
	while(millis() - tempo < 300){
		driver.setMotors(80, 80);

		sensor_linha.calibration_min();

		sensor_esq.find_min();
		sensor_dir.find_min();
	}
	driver.Break();
}

void Seguidor::controle(){	
	// Taxa de amostragem 
	if(millis() - execTime >= samplingTime){
		execTime = millis();

		float erro = sensor_linha.getAngle();

		int rot = controlador.calcPID(erro);

		driver.Set_speedRot(rot);
	}
	
}

void Seguidor::mapeamento(){
	if(Check_latEsq()){
		if(millis() - latEsqTime > 3000){
			latEsqTime = millis();

			// Configura a velodidade base a depender da pista
			if(isReta){
				// Entrou em curva
				SerialBT.println("Curva");
				driver.setVB(60);
				isReta = false;
			}else{
				// Entrou em reta
				SerialBT.println("Reta");
				driver.setVB(100);
				isReta = true;
			}
		}	
	}
	
}

void Seguidor::stopRoutine(){
	// Para o seguidor no final da pista 
	if(millis() - startTime > 5000){
		if (Check_stop() and end == false){
			end = true;
			stopTime = millis();
		}else if (millis() - stopTime > 300 and end == true) Stop();
	}
}

void Seguidor::Run()
{
	start = true;
	end = false;
	stopTime = millis();
	controlador.resetConditions();
}

void Seguidor::Stop(){
	driver.Break();
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
	
bool Seguidor::isStart(){
	return start;
}

void Seguidor::teste(){
	
	//sensor_linha.testeLeitura(sensor_linha.CALIB);
	// Serial.print("Sensor Dir: ");
	// Serial.print(sensor_dir.Read_Calibrado());
	// Serial.print("   ");
	// Serial.print("Sensor Esq: ");
	// Serial.println(sensor_esq.Read_Calibrado());
	//controlador.teste(sensor_linha.getAngle());
	driver.setMotors(100, 100);
	

	delay(100);
}



