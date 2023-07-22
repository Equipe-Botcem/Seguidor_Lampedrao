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

	Config_led_esq(led_esq_pin);
	Config_led_dir(led_dir_pin);
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
}

void Seguidor::set_handler()
{
	String VBc_str = "", VBr_str = "", KI_str = "", KP_str = "", KD_str = "", K_str = "", lixo_str = "";
	int pos = command.indexOf(',', 2);
	Serial.println(command);
	for (int i = 4; i < pos; i++)
		KP_str += command[i];

	int pos2 = command.indexOf(',', pos + 1);
	for (int i = pos + 3; i < pos2; i++)
		KI_str += command[i];

	pos = command.indexOf(',', pos2 + 1);
	for (int i = pos2 + 3; i < pos; i++)
		KD_str += command[i];

	pos2 = command.indexOf(',', pos + 1);
	for (int i = pos + 3; i < pos2; i++)
		K_str += command[i];

	pos = command.indexOf(',', pos2 + 1);
	for (int i = pos2 + 3; i < pos; i++)
		VBc_str += command[i];

  pos2 = command.indexOf(',', pos + 1);
	for (int i = pos + 3; i < pos2; i++)
		VBr_str += command[i];

  pos = command.indexOf(',', pos2 + 1);
	for (int i = pos2 + 3; i < pos; i++)
		lixo_str += command[i];

  
	
	// Configura os parâmetros do controlador  
	Vbr = VBr_str.toInt();
  	Vbc = VBc_str.toInt();
  	k = K_str.toInt();
	controlador.setKp(KP_str.toDouble() / 100);
	controlador.setKd(KD_str.toDouble() / 100);
	controlador.setKi(KI_str.toDouble() / 100);


	// Bluetooth check

	SerialBT.print("KP:");
	SerialBT.print(KP_str.toDouble() / 100);
	SerialBT.print(" ");

	SerialBT.print("KI:");
	SerialBT.print(KI_str.toDouble() / 100);
	SerialBT.print(" ");

	SerialBT.print("KD:");
	SerialBT.print(KD_str.toDouble() / 100);
	SerialBT.print(" ");

  SerialBT.print("K:");
	SerialBT.print(K_str.toDouble() / 100);
	SerialBT.print(" ");

	SerialBT.print("VBr:");
	SerialBT.print(Vbr);
	SerialBT.print(" ");

  SerialBT.print("VBc:");
	SerialBT.print(Vbc);
	SerialBT.print(" ");

}

//----------------------- Other Functions -----------------------//
// TODO: Checar necessidade da calibração mínima
void Seguidor::calibration()
{	
	unsigned long tempo;	
	isCalibrado = true;

	tempo = millis();

	while(millis() - tempo < 300){
		SerialBT.println("Para tras");
		driver.setMotors(-80, -80);

		sensor_linha.calibration_max();

		sensor_esq.find_max();
		sensor_dir.find_max();
	}
	driver.Break();
	delay(500);
	tempo = millis();

	while(millis() - tempo < 300){
		LigaLed();
		SerialBT.println("Para frente");
		driver.setMotors(80, 80);

		sensor_linha.calibration_min();

		sensor_esq.find_min();
		sensor_dir.find_min();
	}
	driver.Break();
}

// TODO: Implementar controle do translacional
void Seguidor::controle(){	
	// Taxa de amostragem 
	if(millis() - execTime >= samplingTime){
		execTime = millis();

		float erro = sensor_linha.getAngle();

		int rot = controlador.calcPID(erro);

		driver.Set_speedRot(rot);
	}
	
}

// TODO: Implementar e testar mapeamento
void Seguidor::mapeamento(){
	if(CheckLateralEsq()){
		if(millis() - latEsqTime > 3000){
			latEsqTime = millis();

			// Configura a velodidade base a depender da pista
			if(isReta){
				// Entrou em curva
				SerialBT.println("Curva");
				driver.setVB(Vbc);
				isReta = false;
			}else{
				// Entrou em reta
				SerialBT.println("Reta");
				driver.setVB(Vbr);
				isReta = true;
			}
		}	
	}
	
}

void Seguidor::Run()
{
	start = true;
	end = false;
	startTime = millis();
	controlador.resetConditions();

	if(isCalibrado == false){
		sensor_linha.calibation_manual();

		sensor_esq.Cmax = 1220;
		sensor_esq.Cmin = 0;
		sensor_dir.Cmax = 800;
		sensor_dir.Cmin = 0;
	}
		
}

void Seguidor::Stop(){
  SerialBT.println("Parado");
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
	case 'B':
		bateryCheck();
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

void Seguidor::stopRoutine(){
	// Para o seguidor no final da pista 
	if(millis() - startTime > 3000){
		if (CheckLateralDir() and end == false){
			end = true;
			stopTime = millis();
		}else if (millis() - stopTime > 300 and end == true) Stop();
	}
}

//TODO Refatorar função
bool Seguidor::CheckLateralDir(){
	if(sensor_dir.Read_histerese() == HIGH and checking_encruzilhada_dir == false and gate_sensor == false) {
    gate_sensor = true;
		checking_encruzilhada_dir = true;
		encruzilhada_timer = millis();

  }else if(millis() - encruzilhada_timer < 50){
    if(sensor_esq.Read_histerese() == HIGH){
      checking_encruzilhada_dir = false;
      return false;
    }

  }else if(checking_encruzilhada_dir == true){
    checking_encruzilhada_dir = false;
    LigaLed();
    return true;
  }

  if(millis() - encruzilhada_timer > 200)  gate_sensor = false;

	return false;
}		

// TODO Refatorar função
bool Seguidor::CheckLateralEsq(){

	if(sensor_esq.Read_sensor() >= RESOLUTION*0.5 and sensor_dir.Read_sensor() <= RESOLUTION*0.1) return true;
	
	return false;
}
	
bool Seguidor::isStart(){
	return start;
}

/*
* @brief Checa o nível da bateria
* @return 0 - descarregada, 1 - carregada
*/
void Seguidor::bateryCheck(){

	// TODO: Checar valor da bateria no ADC em 6.5V
	float volt = analogRead(bateria);
	float bat_level = ((3.3/4095)*volt);
	float y = 142.857*(bat_level)-(142.857*2.3);
	
	if(y <= 0){
		SerialBT.println("Bateria Descarregada");
	}else{
		SerialBT.println("Bateria Carregada");
	}

	SerialBT.println(bat_level);
	SerialBT.print(y);
	SerialBT.println("%");
}

void Seguidor::CheckLed(){
	if(is_led_on == true and millis() - ledTimer > 500){
		is_led_on = false;
		// Desliga os leds
		digitalWrite(23, LOW);
		digitalWrite(12, LOW);
	}
}

void Seguidor::LigaLed(){
	// Acende os leds
	digitalWrite(23, HIGH);
	digitalWrite(12, HIGH);

	ledTimer = millis();
	is_led_on = true;
}

void Seguidor::teste(){
	
	//sensor_linha.testeLeitura(sensor_linha.CALIB);
  //TesteSensoresLat();
	//controlador.teste(sensor_linha.getAngle());
	//driver.teste();
	//delay(100);
}

void Seguidor::TesteSensoresLat() {

  // Calibração dos sensores laterais 
	sensor_esq.Cmax = 1440;
	sensor_esq.Cmin = 0;
	sensor_dir.Cmax = 815;
	sensor_dir.Cmin = 0;

  Serial.print("Sensor Dir: ");
	Serial.print(sensor_dir.Read_Calibrado());
	Serial.print("   ");
	Serial.print("Sensor Esq: ");
	Serial.println(sensor_esq.Read_Calibrado());
}





