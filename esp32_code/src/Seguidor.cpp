#include "Seguidor.h"

//----------------------- Construtores -----------------------//

Seguidor::Seguidor()
{
}

Seguidor::Seguidor(double K, double kp, double kd)
{
	K = K;
	Kp = kp;
	Kd = kd;
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
	Set_parametros(0,0.1,10, 70, 0);
}

//----------------------- Sets -----------------------//

void Seguidor::Set_K(double k)
{
	K = k;
}

void Seguidor::Set_Kp(double kp)
{
	Kp = kp;
}

void Seguidor::Set_kd(double kd)
{
	Kd = kd;
}

void Seguidor::Set_VB(int vb){
	VB = vb;
}

void Seguidor::Set_VM(int vmin){
	VM = vmin;
}

void Seguidor::Set_parametros(double k, double kp, double kd, double vb, int vmin)
{
	Set_K(k);
	Set_Kp(kp);
	Set_kd(kd);
	Set_VB(vb);
	Set_VM(vmin);
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
	Set_K(K_str.toDouble());
	Set_Kp(KP_str.toDouble() / 1000);
	Set_kd(KD_str.toDouble());
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
	uint16_t Leituras[8];
	
	for(unsigned i = 0; i < 8; i++){
		//uint16_t d = sensor_linha[i].Read_sensor();
		//Leituras[i] = sensor_linha[i].Read_Calibrado(d);
		Leituras[i] = sensor_linha[i].Read_sensor();
	}

	for (unsigned int i = 0; i < 8; i++){
		erro += Leituras[i] * pesos[i];
	}

	return erro;
}

//TODO testar
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
	calib = true;
}

void Seguidor::controle()
{
	erro = calc_erro();
	//int trans = calc_translacional(erro);
	int rot = calc_rotacional(erro);

	motor_dir.Set_speed(VB + rot);
	motor_esq.Set_speed(VB - rot);
	
}

int Seguidor::calc_rotacional(double erro)
{

	double value = 0;
	// I = I + erro;
	//double D = (erro - erro_antigo);
	//  erro_antigo = erro;

	value = (Kp * erro);
	// double valor = Kp * P + Ki * I + Kd * D;
	return value;
}

int Seguidor::calc_translacional(double erro)  
{	
	double value = (VB - K*abs(erro));
	if(value <10) value = 10;
	return value;
}

void Seguidor::Run()
{
	Enable_motors_drives();
	
	stop_condition = false;
	start_condition = true;
	time_stop = millis();
}

void Seguidor::Stop(){
	Disable_motors_drives();
	stop_condition = false;
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

void Seguidor::Check_stop(){

	if(sensor_dir.Read_sensor() >= 180 and sensor_esq.Read_sensor() <= 60){
		stop_condition = true;
	}
}		
	
