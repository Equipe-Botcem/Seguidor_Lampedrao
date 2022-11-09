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
	sensor_dir = Sensor(pin);
}

void Seguidor::Config_sensor_dir(unsigned char pin)
{
	sensor_esq = Sensor(pin);
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
	if (speed < 45)	speed = 45;
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
	int Leituras[8];
	// TODO testar coloeta de valor
	
	for(unsigned i = 0; i < 8; i++){
		Leituras[i] = sensor_linha[i].Read_sensor();
	}

	for (unsigned int i = 0; i < 8; i++){
		erro += Leituras[i] * pesos[i];
	}
	return erro;
}

void Seguidor::calibration()
{

	// Seguidor no preto
	int v_min_esq = sensor_esq.find_min();
	int v_min_dir = sensor_dir.find_min();

	Enable_motors_drives();
	Set_direction_forward();

	// Joga para o branco em relação a ponta do seguidor
	// TODO ajustar o tempo e velocidade
	Set_motor_esq_speed(100);
	Set_motor_dir_speed(100);
	delay(300);
	Disable_motors_drives();

	// Seguidor no branco
	int v_max_esq = sensor_esq.find_max();
	int v_max_dir = sensor_dir.find_max();

	// Acha val médio

	double v_med_esq = (v_max_esq - v_min_esq) / 2;
	double v_med_dir = (v_max_dir - v_min_dir) / 2;

	sensor_esq.setValorMed(v_med_esq);
	sensor_dir.setValorMed(v_med_dir);
}

void Seguidor::controle()
{
	if (millis() - last_control >= control_time)
	{
		last_control = millis();
		double erro = calc_erro();
		Set_motor_esq_speed(calc_translacional(erro) + calc_rotacional(erro));
		Set_motor_dir_speed(calc_translacional(erro) - calc_rotacional(erro));
	}
}

int Seguidor::calc_rotacional(double erro)
{

	double value = 0;
	// I = I + erro;
	//TODO unidade do last_control
	double D = (erro - erro_antigo)/last_control;
	//  erro_antigo = erro;

	value = (Kp * erro) + (Kd * D);
	// double valor = Kp * P + Ki * I + Kd * D;
	return value;
}

int Seguidor::calc_translacional(double erro)
{
	//TODO testar com diferentes velocidades 
	double value = (90 - K*erro);
	if(value <20) value = 20;
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

	Set_parametros(0.1,0.005,0);
}

void Seguidor::Set_parametros(double k, double kp, double kd)
{
	Set_K(k);
	Set_Kp(kp);
	Set_kd(kd);
}

void Seguidor::Auto_calibrate()
{
	calibration();
}

void Seguidor::Run()
{
	Enable_motors_drives();
	//u_c.controle();
	//u_c.Disable_motors_drives();
	start_condition = true;
}

void Seguidor::Stop(){
	Disable_motors_drives();
}

void Seguidor::Behavior(int bh)
{
	switch (bh)
	{
	case SET:
		//Set_parametros();
		break;
	case STOP:
		Stop();
		break;
	case RUN:
		Run();
		break;
	case CALIBRACAO:
		Auto_calibrate();
		break;
	default:
		break;
	}
}

void Seguidor::testeSensores(){
	Serial.print("SLE:");
	Serial.print(analogRead(39));
	Serial.print("  ");
	Serial.print("S0:");
	Serial.print(analogRead(36));
	Serial.print("  ");
	Serial.print("S1:");
	Serial.print(analogRead(13));
	Serial.print("  ");
	Serial.print("S2:");
	Serial.print(analogRead(14));
	Serial.print("  ");
	Serial.print("S3:");
	Serial.print(analogRead(35));
	Serial.print("  ");
	Serial.print("S4:");
	Serial.print(analogRead(32));
	Serial.print("  ");
	Serial.print("S5:");
	Serial.print(analogRead(33));
	Serial.print("  ");
	Serial.print("S6:");
	Serial.print(analogRead(25));
	Serial.print("  ");
	Serial.print("S7:");
	Serial.print(analogRead(26));
	Serial.print("  ");
	Serial.print("SLD:");
	Serial.print(analogRead(27));
	Serial.print("  ");
	Serial.println("  ");
	delay(300);
}