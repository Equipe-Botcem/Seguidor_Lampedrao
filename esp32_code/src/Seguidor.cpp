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
    SerialBT.begin("Lampedrao");
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
	String VBc_str = "", VBr_str = "", KI_str = "", KP_str = "", KD_str = "", K_str = "", Nf_str = "", lixo_str = "";
	Serial.println(command);
	int pos_inicial = 4;
  int pos = command.indexOf(',', 2);
	for (int i = pos_inicial; i < pos; i++){
    KP_str += command[i];
  }
	
  pos_inicial = pos + 3;
	pos = command.indexOf(',', pos + 1);
	for (int i = pos_inicial; i < pos; i++){
    KI_str += command[i];
  }
		
  pos_inicial = pos + 3;
	pos = command.indexOf(',', pos + 1);
	for (int i = pos_inicial; i < pos; i++){
    KD_str += command[i];
  }
	
  pos_inicial = pos + 3;
	pos = command.indexOf(',', pos + 1);
	for (int i = pos_inicial; i < pos; i++){
    K_str += command[i];
  }

  pos_inicial = pos + 3;
	pos = command.indexOf(',', pos + 1);
	for (int i = pos_inicial; i < pos; i++){
    VBc_str += command[i];
  }
		
  pos_inicial = pos + 3;
  pos = command.indexOf(',', pos + 1);
	for (int i = pos_inicial; i < pos; i++){
    VBr_str += command[i];
  }

	pos_inicial = pos + 3;
  pos = command.indexOf(',', pos + 1);
	for (int i = pos_inicial; i < pos; i++){
    Nf_str += command[i];
  }
	
  pos_inicial = pos + 3;
  pos = command.indexOf(',', pos + 1);
	for (int i = pos_inicial; i < pos; i++){
    lixo_str += command[i];
  }
		
	// Configura os parâmetros do controlador  
	Nf = Nf_str.toInt();
	Vbr = VBr_str.toInt();
  driver.setVB(Vbr);
	Vbc = VBc_str.toInt();
	k = K_str.toInt()/100;
	controlador.setKp(KP_str.toDouble() / 100);
	controlador.setKd(KD_str.toDouble() / 100);
	controlador.setKi(KI_str.toDouble() / 100000);


	// Bluetooth check

	SerialBT.print("KP:");
	SerialBT.print(KP_str.toDouble() / 100);
	SerialBT.print(" ");

	SerialBT.print("KI:");
	SerialBT.print(KI_str.toDouble() / 100000, 3);
	SerialBT.print(" ");

	SerialBT.print("KD:");
	SerialBT.print(KD_str.toDouble() / 100);
	SerialBT.print(" ");

  SerialBT.print("K:");
	SerialBT.print(K_str.toDouble()/100);
	SerialBT.print(" ");

	SerialBT.print("VBr:");
	SerialBT.print(Vbr);
	SerialBT.print(" ");

  SerialBT.print("VBc:");
	SerialBT.print(Vbc);
	SerialBT.print(" ");

	SerialBT.print("Nf:");
	SerialBT.print(Nf);

}

//----------------------- Other Functions -----------------------//
void Seguidor::calibration()
{	
	unsigned long tempo;	
	isCalibrado = true;

	tempo = millis();

	while(millis() - tempo < 300){
		driver.setMotors(-80, -80);

		sensor_linha.calibration_max();

		sensor_esq.find_max();
		sensor_dir.find_max();
	}
	driver.Break();
	delay(500);
	tempo = millis();

	while(millis() - tempo < 300){
		driver.setMotors(80, 80);

		sensor_linha.calibration_max();

		sensor_esq.find_max();
		sensor_dir.find_max();
	}
	driver.Break();

  // Caso a calibração tenha sido bem sucedida
  if (sensor_linha.CheckCalibration()){
    if(sensor_esq.GetMax() != 0 and sensor_dir.GetMax() != 0){
      PiscaLed(5);
      
    }
  }
}

void Seguidor::controle(){	
	// Taxa de amostragem 
	if(millis() - execTime >= samplingTime){
		execTime = millis();

		erro = sensor_linha.getAngle();

		// Cálculo do redutor de velocidade translacional
    //if(abs(erro) > out){
      //trans = abs(erro)*k;
    //} else{
      //trans = 0;
    //}

		rot = controlador.calcPID(erro);
		driver.Set_speedRot(rot - trans);
	}
	
}

bool Seguidor::IsOut(){
  if(abs(erro) > out) return true;

  return false;
}

void Seguidor::mapeamento(){
	if(CheckLateralEsq()){
		fitas++;
		if(fitas >= Nf and bost == false){
			driver.setVB(Vbc);
			bost = true;
			SerialBT.println("Bost"); 
		} 
	}
	
}

void Seguidor::Run()
{
	start = true;
	end = false;
	startTime = millis();
	controlador.resetConditions();
	fitas = 0;
	bost = false;

	if(isCalibrado == false){
		sensor_linha.calibation_manual();

		sensor_esq.Cmax = 1220;
		sensor_dir.Cmax = 800;
	}
		
}

void Seguidor::Stop(){
	SerialBT.print("Seguidor parando...");
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

void Seguidor::stopRoutine(){
	// Para o seguidor no final da pista 
	if(millis() - startTime > 35000){
		if (CheckLateralDir() and end == false){
			SerialBT.print("Fitas: ");
  		SerialBT.println(fitas);
			end = true;
			stopTime = millis();
		}else if (millis() - stopTime > 300 and end == true) Stop();
	}
}

bool Seguidor::CheckLateralDir(){
	if(sensor_dir.Read_histerese() == HIGH and gate_sensor_dir == false) {
    gate_sensor_dir = true;
		checking_encruzilhada_dir = true;
		encruzilhada_timer_dir = millis();

  }else if(millis() - encruzilhada_timer_dir < debouce_sensor){
    if(sensor_esq.Read_histerese() == HIGH){
			LigaLed();
      checking_encruzilhada_dir = false;
      return false;
    }

  }else if(checking_encruzilhada_dir == true){
    checking_encruzilhada_dir = false;
		digitalWrite(led_dir_pin, HIGH);
		is_led_on = true;
		ledTimer = millis();
    return true;
  }

	if(millis() - encruzilhada_timer_dir > debouce_sensor) gate_sensor_dir = false;

	return false;
}		

bool Seguidor::CheckLateralEsq(){
	if(sensor_esq.Read_histerese() == HIGH and gate_sensor_esq == false) {
    gate_sensor_esq = true;
		checking_encruzilhada_esq = true;
		encruzilhada_timer_esq = millis();

  }else if(millis() - encruzilhada_timer_esq < debouce_sensor){
    if(sensor_dir.Read_histerese() == HIGH){
			LigaLed();
      checking_encruzilhada_esq = false;
      return false;
    }

  }else if(checking_encruzilhada_esq == true){
    checking_encruzilhada_esq = false;
    digitalWrite(led_esq_pin, HIGH);
		is_led_on = true;
		ledTimer = millis();
    return true;
  }

	if(millis() - encruzilhada_timer_esq > debouce_sensor) gate_sensor_esq = false;

	return false;
}
	
bool Seguidor::isStart(){
	return start;
}

void Seguidor::CheckLed(){
	if(is_led_on == true and millis() - ledTimer > debouce_sensor){
		DesligaLed();
	}
}

void Seguidor::LigaLed(){
	// Acende os leds
	digitalWrite(led_esq_pin, HIGH);
	digitalWrite(led_dir_pin, HIGH);

	ledTimer = millis();
	is_led_on = true;
}

void Seguidor::DesligaLed(){
  is_led_on = false;
  // Desliga os leds
  digitalWrite(led_esq_pin, LOW);
  digitalWrite(led_dir_pin, LOW);
}

void Seguidor::PiscaLed(int num_piscadas){
  for(unsigned i = 0; i < num_piscadas; i++){
    LigaLed();
    delay(200);
    DesligaLed();
    delay(200);
  }
}

void Seguidor::teste(){

	sensor_linha.calibation_manual();
	// if(sensor_linha.CheckBuraco()) LigaLed();
	//sensor_linha.calibation_manual();
	//LigaLed();
	Serial.println(sensor_linha.getAngle());
	//sensor_linha.testeLeitura(sensor_linha.CALIB);
 // TesteSensoresLat();
	// controlador.teste(sensor_linha.getAngle());
	// driver.teste();
  delay(1);
}

void Seguidor::TesteSensoresLat() {

  // Calibração dos sensores laterais 
	sensor_esq.Cmax = 1440;
	sensor_dir.Cmax = 815;

  Serial.print("Sensor Dir: ");
	Serial.print(sensor_dir.Read_histerese());
	Serial.print("   ");
	Serial.print("Sensor Esq: ");
	Serial.println(sensor_esq.Read_histerese());
}



