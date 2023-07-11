#include "Seguidor.h"
#include "Arduino.h"
#include "SimpleKalmanFilter.h"

// ! Precisa ser otimizado a velocidade das medições (provavelmente 0.1)
SimpleKalmanFilter angleKalmanFilter(1, 1, 0.01);

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

void Seguidor::Config_motor_esq(unsigned char *pins)
{
	motor_esq = Motor_drive(pins[0], pins[1], pins[2], pins[3]);
}

void Seguidor::Config_motor_dir(unsigned char *pins)
{
	motor_dir = Motor_drive(pins[0], pins[1], pins[2], pins[3]);
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
	Config_motor_esq(pins_motor_drive_esq);
	Config_motor_dir(pins_motor_drive_dir);
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

	motor_dir.Init();
	motor_esq.Init();
	sensor_linha.Init();
	sensor_esq.Init();
	sensor_dir.Init();

	// Parametros default
	Set_parametros(2, 0.05, 0.05, 100, 5);
}

//----------------------- Sets -----------------------//


void Seguidor::Set_VB(int vb){
	VB = vb;
}

void Seguidor::Set_parametros(float kp, float kd, float ki, float vb, int vmin)
{
	Set_VB(vb);
	controlador.setControlador(kp, kd, ki);
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
	Set_VB(VB.toInt());
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

void Seguidor::Enable_motors_drives()
{
	motor_esq.Enable_drive();
	motor_dir.Enable_drive();
}

void Seguidor::Disable_motors_drives()
{
	motor_esq.Disable_drive();
	motor_dir.Disable_drive();
}

void Seguidor::calibration()
{	
	unsigned long tempo;

	tempo = millis();

	while(millis() - tempo < 300){

		Enable_motors_drives();

		motor_dir.Set_speed(-80);
		motor_esq.Set_speed(-80);

		sensor_linha.calibration_max();

		sensor_esq.find_max();
		sensor_dir.find_max();
	}
	Disable_motors_drives();

	tempo = millis();
	while(millis() - tempo < 300){
		Enable_motors_drives();

		motor_dir.Set_speed(80);
		motor_esq.Set_speed(80);

		sensor_linha.calibration_min();

		sensor_esq.find_min();
		sensor_dir.find_min();
	}

	Disable_motors_drives();


}

void Seguidor::controle(){	
	// Taxa de amostragem 
	if (!samplingTime) samplingTime = millis();
	else if(millis() - samplingTime >= controlador.getAmostragem()){
		samplingTime = 0;

		int rot = controlador.calcPID(angleKalmanFilter.updateEstimate(sensor_linha.getAngle()));

		// Atua nos motores conforme a pista 
		mapeamento(rot);
	}
	
}

void Seguidor::mapeamento(int rot){
	motor_dir.Set_speed(VB + rot);
	motor_esq.Set_speed(VB - rot);
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
	Enable_motors_drives();
	start= true;
	stopTime = millis();
	bool fimPista = false;
}

void Seguidor::Stop(){
	Disable_motors_drives();
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
	
void Seguidor::teste(){

	// delay(1);
	// //controle();

	// Serial.print("Angle:");
	// Serial.println(getAngle());
	// Serial.print(",");
	// Serial.print("Kalman_filter:");
	// Serial.println(angleKalmanFilter.updateEstimate(getAngle()));
	

	// Serial.print("S2: ");
	// Serial.print(sensor_linha[0].Read_histerese());
	// Serial.print("  S3: ");
	// Serial.print(sensor_linha[1].Read_histerese());
	// Serial.print("  S4: ");
	// Serial.print(sensor_linha[2].Read_histerese());
	// Serial.print("  S5: ");
	// Serial.print(sensor_linha[3].Read_histerese());
	// Serial.print("  S6: ");
	// Serial.print(sensor_linha[4].Read_histerese());
	// Serial.print("  S7: ");
	// Serial.print(sensor_linha[5].Read_histerese());
	// Serial.print("  S8: ");
	// Serial.print(sensor_linha[6].Read_histerese());
	// Serial.print("  S9: ");
	// Serial.println(sensor_linha[7].Read_histerese());
	 


	// Serial.print("SE: ");
	// Serial.println(sensor_esq.Read_sensor());
	// Serial.print("SD: ");
	// Serial.println(sensor_dir.Read_sensor());

	
	// Serial.print("S2: ");
	// Serial.print(sensor_linha[0].Read_Calibrado());
	// Serial.print("  S3: ");
	// Serial.print(sensor_linha[1].Read_Calibrado());
	// Serial.print("  S4: ");
	// Serial.print(sensor_linha[2].Read_Calibrado());
	// Serial.print("  S5: ");
	// Serial.print(sensor_linha[3].Read_Calibrado());
	// Serial.print("  S6: ");
	// Serial.print(sensor_linha[4].Read_Calibrado());
	// Serial.print("  S7: ");
	// Serial.print(sensor_linha[5].Read_Calibrado());
	// Serial.print("  S8: ");
	// Serial.print(sensor_linha[6].Read_Calibrado());
	// Serial.print("  S9: ");
	// Serial.println(sensor_linha[7].Read_Calibrado());

	// Serial.print("S2: ");
	// Serial.print(sensor_linha[0].Read_sensor());
	// Serial.print("  S3: ");
	// Serial.print(sensor_linha[1].Read_sensor());
	// Serial.print("  S4: ");
	// Serial.print(sensor_linha[2].Read_sensor());
	// Serial.print("  S5: ");
	// Serial.print(sensor_linha[3].Read_sensor());
	// Serial.print("  S6: ");
	// Serial.print(sensor_linha[4].Read_sensor());
	// Serial.print("  S7: ");
	// Serial.print(sensor_linha[5].Read_sensor());
	// Serial.print("  S8: ");
	// Serial.print(sensor_linha[6].Read_sensor());
	// Serial.print("  S9: ");
	// Serial.println(sensor_linha[7].Read_sensor());
	 
	
}

bool Seguidor::isEnd(){
	return end;
}

bool Seguidor::isStart(){
	return start;
}

