#include "Seguidor.h"

//----------------------- Construtores -----------------------//

Seguidor::Seguidor()
{
}

//----------------------- Configs e inits -----------------------//

void Seguidor::Config_motor_esq(unsigned char *pins)
{
	motor_esq = Motor_drive(pins[0], pins[1], pins[2], pins[3]);
	#if defined(DIAG_MODE)
		motor_esq.set_name("MOTOR ESQ");
	#endif
}

void Seguidor::Config_motor_dir(unsigned char *pins)
{
	motor_dir = Motor_drive(pins[0], pins[1], pins[2], pins[3]);
	#if defined(DIAG_MODE)
		motor_dir.set_name("MOTOR DIR");
	#endif
}

void Seguidor::Config_encoder_esq(unsigned char pin_interrupt)
{
	encoder_esq = Encoder(pin_interrupt);
}

void Seguidor::Config_encoder_dir(unsigned char pin_interrupt)
{
	encoder_dir = Encoder(pin_interrupt);
}

void Seguidor::Config_sensor_linha(unsigned char *pins)
{
	for(unsigned i = 0; i < 8; i++){
		sensor_linha[i] = Sensor(pins[i]);
	}

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

	Config_motor_esq(pins_motor_drive_esq);
	Config_motor_dir(pins_motor_drive_dir);
	Config_encoder_esq(pin_encoder_esq);
	Config_encoder_dir(pin_encoder_dir);
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
	encoder_esq.Init();
	encoder_dir.Init();

	for(unsigned i = 0; i < 8; i++){
		sensor_linha[i].Init();
	}
	
	sensor_esq.Init();
	sensor_dir.Init();

	// Parametros default
	Set_parametros(0.0225,0, 100, 5);
}

//----------------------- Sets -----------------------//


void Seguidor::Set_VB(int vb){
	VB = vb;
}

void Seguidor::Set_VM(int vmin){
	VM = vmin;
}

void Seguidor::Set_parametros(double kp, double kd, double vb, int vmin)
{
	Set_VB(vb);
	Set_VM(vmin);
	controlador.setControlador(0.0225, 0, 0);
}

// reescrever para tirar sobrecarga de tarefas
void Seguidor::set_handler()
{
	String VB = "", K_str = "", KP_str = "", KD_str = "", VM_str = "", lixo_str = "";
	int pos = command.indexOf(',', 2);
	Serial.println(command);
	for (int i = 4; i < pos; i++)
		VB += command[i];

	int pos2 = command.indexOf(',', pos + 1);
	for (int i = pos + 3; i < pos2; i++)
		K_str += command[i];

	pos = command.indexOf(',', pos2 + 1);
	for (int i = pos2 + 3; i < pos; i++)
		KP_str += command[i];

	pos2 = command.indexOf(',', pos + 1);
	for (int i = pos + 3; i < pos2; i++)
		KD_str += command[i];

	pos = command.indexOf(',', pos2 + 1);
	for (int i = pos2 + 3; i < pos; i++)
		VM_str += command[i];

	pos2 = command.indexOf(',', pos + 1);
	for (int i = pos + 3; i < pos2; i++)
		lixo_str += command[i];

	
	// Configura osf parâmetros do controlador  
	Set_VB(VB.toInt());
	controlador.setKp(KP_str.toDouble() / 1000);
	controlador.setKd(KD_str.toDouble() / 1000);
	Set_VM(VM_str.toInt());


	// Bluetooth check

	SerialBT.print("VB:");
	SerialBT.print(VB);
	SerialBT.print(" ");

	SerialBT.print("K:");
	SerialBT.print(K_str);
	SerialBT.print(" ");

	SerialBT.print("KP:");
	SerialBT.print(KP_str);
	SerialBT.print(" ");

	SerialBT.print("KD:");
	SerialBT.print(KD_str);
	SerialBT.print(" ");

	SerialBT.print("VMIN:");
	SerialBT.print(VM);
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

double Seguidor::calc_erro()
{
	double erro = 0;
	int Leituras[8];
	
	for(unsigned i = 0; i < 8; i++){
		Leituras[i] = sensor_linha[i].Read_Calibrado();
	}

	for (unsigned int i = 0; i < 8; i++){
		erro += Leituras[i] * pesos[i];
	}

	return erro;
}

void Seguidor::calibration()
{	
	unsigned long tempo;

	tempo = millis();

	while(millis() - tempo < 300){

		Enable_motors_drives();

		motor_dir.Set_speed(-80);
		motor_esq.Set_speed(-80);

		for(unsigned i = 0; i < 8; i++) sensor_linha[i].find_max();

		sensor_esq.find_max();
		sensor_dir.find_max();
	}
	Disable_motors_drives();

	tempo = millis();
	while(millis() - tempo < 300){
		Enable_motors_drives();

		motor_dir.Set_speed(80);
		motor_esq.Set_speed(80);

		for(unsigned i = 0; i < 8; i++) sensor_linha[i].find_min();

		sensor_esq.find_min();
		sensor_dir.find_min();
	}

	Disable_motors_drives();


}

void Seguidor::controle()
{
	int rot = controlador.calcRot(calc_erro());

	motor_dir.Set_speed(VB + rot);
	motor_esq.Set_speed(VB - rot);
	
}

void Seguidor::Run()
{
	Enable_motors_drives();
	start_condition = true;

}

void Seguidor::Stop(){
	Disable_motors_drives();
	start_condition = false;
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

	if(sensor_dir.Read_sensor() >= (RESOLUTION - 100) and sensor_esq.Read_sensor() <= 100){
		return true;
	}

	return false;
}		
	
void Seguidor::testeSensores(){

	Serial.print("S1: ");
	Serial.print(sensor_linha[0].Read_Calibrado());
	Serial.print("  S2: ");
	Serial.print(sensor_linha[1].Read_Calibrado());
	Serial.print("  S3: ");
	Serial.print(sensor_linha[2].Read_Calibrado());
	Serial.print("  S4: ");
	Serial.print(sensor_linha[3].Read_Calibrado());
	Serial.print("  S5: ");
	Serial.print(sensor_linha[4].Read_Calibrado());
	Serial.print("  S6: ");
	Serial.print(sensor_linha[5].Read_Calibrado());
	Serial.print("  S7: ");
	Serial.print(sensor_linha[6].Read_Calibrado());
	Serial.print("  S8: ");
	Serial.println(sensor_linha[7].Read_Calibrado());

	
}

void Seguidor::testeMotores(){
	int rot = controlador.calcRot(calc_erro());
	Serial.print("Speed motor dir: ");
	Serial.println(VB + rot);

	Serial.print("Speed motor esq: ");
	Serial.println(VB - rot);

}