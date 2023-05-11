#include "Seguidor.h"

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

	Config_led_esq(led_esq);
	Config_led_esq(led_dir);
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
	Set_parametros(0.0055,0.0125, 0, 100, 5);
}

//----------------------- Sets -----------------------//


void Seguidor::Set_VB(int vb){
	VB = vb;
}

void Seguidor::Set_VM(int vmin){
	VM = vmin;
}

void Seguidor::Set_parametros(float kp, float kd, float ki, float vb, int vmin)
{
	Set_VB(vb);
	Set_VM(vmin);
	controlador.setControlador(kp, kd, ki);
}

void Seguidor::set_handler()
{
	String VB = "", KI_str = "", KP_str = "", KD_str = "", VM_str = "", lixo_str = "";
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
		VM_str += command[i];

	pos2 = command.indexOf(',', pos + 1);
	for (int i = pos + 3; i < pos2; i++)
		lixo_str += command[i];

	
	// Configura osf parâmetros do controlador  
	Set_VB(VB.toInt());
	controlador.setKp(KP_str.toDouble() / 10000);
	controlador.setKd(KD_str.toDouble() / 10000);
	controlador.setKi(KI_str.toDouble() / 10000);
	Set_VM(VM_str.toInt());


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

float Seguidor::calc_erro()
{
	double erro = 0;
	int Leituras[8];
	
	for(unsigned i = 0; i < 8; i++){
		Leituras[i] = sensor_linha[i].Read_Calibrado();
	}

	for (unsigned int i = 0; i < 8; i++){
		erro += Leituras[i] * pesos[i];
	}

	if(abs(erro) >= 20475){
		if (erro > 0) erro = 20475;
		else erro = -20475;
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
	// Taxa de amostragem 
	//if (!tempo_corrido)
	//{
		//tempo_corrido = millis();

	//}else if(millis() - tempo_corrido >= controlador.getAmostragem()){
		//tempo_corrido = 0;
		float erro = calc_erro();

		// // Checa se saiu da linha 
		// if (outside and (millis() - tempo_corrido >= 50)){
		// 	returnToLine(erro);
		// 	return;
		// } 

		int rot = controlador.calcPID(erro);
		//int trans = controlador.calcTrans(erro);

		// Serial.print("Motor Dir = ");
		// Serial.println(VB + rot);

		// Serial.print("Motor Esq = ");
		// Serial.println(VB - rot);

		if(abs(erro) > 6000){
			motor_dir.Set_speed(0.7*VB + rot);
			motor_esq.Set_speed(0.7*VB - rot);
		}else{

			motor_dir.Set_speed(VB + rot);
			motor_esq.Set_speed(VB - rot);

			// motor_dir.Set_speed(VB*1.5 + rot);
			// motor_esq.Set_speed(VB*1.5 - rot);
		}

	//}
	
}

//! Não funciona
void Seguidor::returnToLine(float erro){
	bool sentido;
	if(!controlador.getLastDir()){
		SerialBT.println("Girar esquerda");
		motor_dir.Set_speed(100);
		motor_esq.Set_speed(0);
	}else{
		// direita
		SerialBT.println("Girar direita");
		motor_dir.Set_speed(0);
		motor_esq.Set_speed(100);
	}

	return;
}

void Seguidor::Run()
{
	Enable_motors_drives();
	start= true;
	tempo_corrido = millis();
	bool fimPista = false;
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

	if(sensor_dir.Read_sensor() >= 2000 and sensor_esq.Read_sensor() <= 100){
		return true;
	}

	return false;
}		
	
void Seguidor::testeSensores(){

	Serial.print("SE: ");
	Serial.println(sensor_esq.Read_sensor());
	Serial.print("SD: ");
	Serial.println(sensor_dir.Read_sensor());

	
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
	 
	
}

void Seguidor::testeMotores(){
	calc_erro();
}

void Seguidor::habiliteiStop(){
	end = true;
	tempo_stop = millis();
}

bool Seguidor::isEnd(){
	return end;
}

bool Seguidor::isStar(){
	return start;
}
