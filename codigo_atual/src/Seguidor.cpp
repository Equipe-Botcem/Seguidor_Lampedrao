#include "Seguidor.h"

Seguidor::Seguidor()
{
}

Seguidor::Seguidor(double K, double kp, double kd)
{
	K = K;
	Kp = kp;
	Kd = kd;
}

void Seguidor::Config_motor_esq(unsigned char *pins)
{
	motor_esq = Motor_drive(pins[0], pins[1], pins[2], pins[3]);
}

void Seguidor::Config_motor_dir(unsigned char *pins)
{
	motor_dir = Motor_drive(pins[0], pins[1], pins[2], pins[3]);
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

void Seguidor::Set_direction_forward()
{
	motor_esq.Set_motor_forward();
	motor_dir.Set_motor_forward();
}

void Seguidor::Set_direction_reverse()
{
	motor_esq.Set_motor_reverse();
	motor_dir.Set_motor_reverse();
}

int Seguidor::check_speed(int speed){
	if (speed > 255)	speed = 255;
	if (speed < VM)	speed = VM;
	return speed;
}

void Seguidor::Set_motor_esq_speed(int speed)
{
	motor_esq.Set_speed(check_speed(speed));
}

void Seguidor::Set_motor_dir_speed(int speed)
{
	motor_dir.Set_speed(check_speed(speed));
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

void Seguidor::calibration()
{	
	unsigned long tempo;

	tempo = millis();

	while(millis() - tempo < 500){

		Enable_motors_drives();
		Set_direction_reverse();

		Set_motor_esq_speed(80);
		Set_motor_dir_speed(80);

		for(unsigned i = 0; i < 8; i++) sensor_linha[i].find_max();

		sensor_esq.find_max();
		sensor_dir.find_max();
	}

	tempo = millis();

	Disable_motors_drives();

	while(millis() - tempo < 500){

		Enable_motors_drives();
		Set_direction_forward();

		Set_motor_esq_speed(80);
		Set_motor_dir_speed(80);

		for(unsigned i = 0; i < 8; i++) sensor_linha[i].find_min();

		sensor_esq.find_min();
		sensor_dir.find_min();
	}

	Disable_motors_drives();
	
}

void Seguidor::controle()
{
	erro = calc_erro();
	//int trans = calc_translacional(erro);
	int rot = calc_rotacional(erro);

	Set_motor_esq_speed(VB + rot);
	Set_motor_dir_speed(VB - rot);
	
}

int Seguidor::calc_rotacional(double erro)
{

	double value = 0;
	// I = I + erro;
	double D = (erro - erro_antigo);
	//  erro_antigo = erro;

	value = (Kp * erro) + (Kd * D);
	// double valor = Kp * P + Ki * I + Kd * D;
	return value;
}

int Seguidor::calc_translacional(double erro)
{
	//TODO testar com diferentes velocidades 
	double value = (VB - K*abs(erro));
	if(value <10) value = 10;
	return value;
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
	Set_parametros(0.09,0.1,0, 60, 0);
}

void Seguidor::Set_parametros(double k, double kp, double kd, double vb, int vmin)
{
	Set_K(k);
	Set_Kp(kp);
	Set_kd(kd);
	Set_VB(vb);
	Set_VM(vmin);
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

void Seguidor::testeSensores(){

	// Serial 
	Serial.print("SLE:");
	Serial.print(sensor_esq.Read_sensor());
	Serial.print("  ");
	Serial.print("S0:");
	Serial.print(sensor_linha[0].Read_sensor());
	Serial.print("  ");
	Serial.print("S1:");
	Serial.print(sensor_linha[1].Read_sensor());
	Serial.print("  ");
	Serial.print("S2:");
	Serial.print(sensor_linha[2].Read_sensor());
	Serial.print("  ");
	Serial.print("S3:");
	Serial.print(sensor_linha[3].Read_sensor());
	Serial.print("  ");
	Serial.print("S4:");
	Serial.print(sensor_linha[4].Read_sensor());
	Serial.print("  ");
	Serial.print("S5:");
	Serial.print(sensor_linha[5].Read_sensor());
	Serial.print("  ");
	Serial.print("S6:");
	Serial.print(sensor_linha[6].Read_sensor());
	Serial.print("  ");
	Serial.print("S7:");
	Serial.print(sensor_linha[7].Read_sensor());
	Serial.print("  ");
	Serial.print("SLD:");
	Serial.print(sensor_dir.Read_sensor());
	Serial.print("  ");
	Serial.println("  ");
	delay(1000);
}

void Seguidor::initBluetooth(){
    SerialBT.begin("ESP32");
  	Serial.println("O dispositivo já pode ser pareado ou conectado!");
}

void Seguidor::comunica_serial(){
	if(SerialBT.available()){ 
		command = SerialBT.readStringUntil(';');

	}
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
	Serial.println(VM_str);
	Set_VB(VB.toInt());
	Set_K(K_str.toDouble() / 1000);
	Set_Kp(KP_str.toDouble() / 1000);
	Set_kd(KD_str.toDouble() / 1000);
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

void Seguidor::Check_stop(){

	Serial.print("Sensor direito: ");
	Serial.println(sensor_dir.Read_sensor());

	Serial.print("Sensor esquerdo: ");
	Serial.println(sensor_esq.Read_sensor());

	if(sensor_dir.Read_sensor() >= 180 and sensor_esq.Read_sensor() <= 100){
		stop_condition = true;
	}
}		
	
void Seguidor::testeMotores(){
	Serial.print("Erro:");
	double erro = calc_erro();
	Serial.print(erro);
	Serial.print("     ");

	Serial.print("Erro rot:");
	double rot = calc_rotacional(erro);
	Serial.print(rot);
	Serial.print("     ");

	Serial.print("Motor esq:");
	Serial.print(100 + rot);
	Serial.print("     ");

	Serial.print("Motor dir:");
	Serial.print(100 - rot);
	Serial.print("     ");

	Serial.println();
	delay(500);
}