#include "Seguidor.h"
#include "Arduino.h"

// Construtor padrão
Seguidor::Seguidor() {}

// Função para configurar o LED esquerdo
void Seguidor::Config_led_esq(unsigned char pin)
{
	pinMode(pin, OUTPUT); // Define o pino do LED como saída
	digitalWrite(pin, LOW); // Inicialmente desliga o LED
}

// Função para configurar o LED direito
void Seguidor::Config_led_dir(unsigned char pin)
{
	pinMode(pin, OUTPUT); // Define o pino do LED como saída
	digitalWrite(pin, LOW); // Inicialmente desliga o LED
}

// Função para configurar os motores
void Seguidor::Config_motors(unsigned char *pins_dir, unsigned char *pins_esq)
{
	driver = Driver(pins_dir, pins_esq); // Inicializa o driver do motor com os pinos fornecidos
}

// Função para configurar o sensor de linha
void Seguidor::Config_sensor_linha(unsigned char *pins)
{	
	sensor_linha = Sensor_linha(pins); // Inicializa o sensor de linha com os pinos fornecidos
}

// Função para configurar o sensor esquerdo
void Seguidor::Config_sensor_esq(unsigned char pin)
{
	sensor_esq = Sensor(pin); // Inicializa o sensor esquerdo com o pino fornecido
}

// Função para configurar o sensor direito
void Seguidor::Config_sensor_dir(unsigned char pin)
{
	sensor_dir = Sensor(pin); // Inicializa o sensor direito com o pino fornecido
}

// Função para configurar todos os pinos
void Seguidor::Config_pins()
{
	Config_led_esq(led_esq_pin); // Configura o LED esquerdo
	Config_led_dir(led_dir_pin); // Configura o LED direito
	Config_motors(pins_motor_drive_dir, pins_motor_drive_esq); // Configura os motores
	Config_sensor_linha(pins_sensor_linha); // Configura o sensor de linha
	Config_sensor_esq(pin_sensor_esq); // Configura o sensor esquerdo
	Config_sensor_dir(pin_sensor_dir); // Configura o sensor direito
}

// Função para inicializar o Bluetooth
void Seguidor::initBluetooth()
{
    SerialBT.begin("ESP32"); // Inicia o Bluetooth com o nome "ESP32"
  	Serial.println("O dispositivo já pode ser pareado ou conectado!"); // Imprime uma mensagem indicando que o dispositivo Bluetooth está pronto para ser pareado ou conectado
}

// Função para inicializar todo o sistema
void Seguidor::Init()
{
	Config_pins(); // Configura todos os pinos

	driver.Init(); // Inicia o driver do motor
	sensor_linha.Init(); // Inicia o sensor de linha
	sensor_esq.Init(); // Inicia o sensor esquerdo
	sensor_dir.Init(); // Inicia o sensor direito

	controlador.setControlador(1, 0, 0.1); // Configura os parâmetros do controlador PID
	controlador.resetConditions(); // Reinicia as condições do controlador PID

	driver.Enable_motors_drives(); // Habilita os motores
}

// Função para manipular as configurações
void Seguidor::set_handler()
{
	// Variáveis para armazenar as configurações recebidas via Bluetooth
	String VBc_str = "", VBr_str = "", KI_str = "", KP_str = "", KD_str = "", K_str = "", lixo_str = "";
	Serial.println(command); // Imprime o comando recebido

	int pos_inicial = 4; // A posição inicial do valor de configuração na string do comando
	int pos = command.indexOf(',', 2); // A posição do próximo delimitador na string do comando

	// Loop para extrair o valor de configuração para KP
	for (int i = pos_inicial; i < pos; i++){
		KP_str += command[i];
	}
	
	pos_inicial = pos + 3; // Atualiza a posição inicial para o próximo valor de configuração
	pos = command.indexOf(',', pos + 1); // Atualiza a posição do delimitador para o próximo valor de configuração

	// Loop para extrair o valor de configuração para KI
	for (int i = pos_inicial; i < pos; i++){
		KI_str += command[i];
	}
		
	pos_inicial = pos + 3; // Atualiza a posição inicial para o próximo valor de configuração
	pos = command.indexOf(',', pos + 1); // Atualiza a posição do delimitador para o próximo valor de configuração

	// Loop para extrair o valor de configuração para KD
	for (int i = pos_inicial; i < pos; i++){
		KD_str += command[i];
	}
	
	pos_inicial = pos + 3; // Atualiza a posição inicial para o próximo valor de configuração
	pos = command.indexOf(',', pos + 1); // Atualiza a posição do delimitador para o próximo valor de configuração

	// Loop para extrair o valor de configuração para K
	for (int i = pos_inicial; i < pos; i++){
		K_str += command[i];
	}

	pos_inicial = pos + 3; // Atualiza a posição inicial para o próximo valor de configuração
	pos = command.indexOf(',', pos + 1); // Atualiza a posição do delimitador para o próximo valor de configuração

	// Loop para extrair o valor de configuração para VBc
	for (int i = pos_inicial; i < pos; i++){
		VBc_str += command[i];
	}
		
	pos_inicial = pos + 3; // Atualiza a posição inicial para o próximo valor de configuração
	pos = command.indexOf(',', pos + 1); // Atualiza a posição do delimitador para o próximo valor de configuração

	// Loop para extrair o valor de configuração para VBr
	for (int i = pos_inicial; i < pos; i++){
		VBr_str += command[i];
	}
	
	pos_inicial = pos + 3; // Atualiza a posição inicial para o próximo valor de configuração
	pos = command.indexOf(',', pos + 1); // Atualiza a posição do delimitador para o próximo valor de configuração

	// Loop para extrair o valor de configuração para lixo
	for (int i = pos_inicial; i < pos; i++){
		lixo_str += command[i];
	}
	
	// Configura os parâmetros do controlador PID com os valores de configuração extraídos
	Vbr = VBr_str.toInt();
	driver.setVB(Vbr);
	Vbc = VBc_str.toInt();
	k = K_str.toInt()/100;
	controlador.setKp(KP_str.toDouble() / 100);
	controlador.setKd(KD_str.toDouble() / 100);
	controlador.setKi(KI_str.toDouble() / 100000);

	// Imprime os parâmetros do controlador PID para o Bluetooth
	SerialBT.print("KP:");
	SerialBT.print(KP_str.toDouble() / 100);
	SerialBT.print(" ");

	SerialBT.print("KI:");
	SerialBT.print(KI_str.toDouble() / 100000, 5);
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
}

// Função para calibrar os sensores
void Seguidor::calibration()
{	
	unsigned long tempo;	
	isCalibrado = true;

	tempo = millis();

	// Loop para calibrar os sensores durante 300 ms enquanto os motores estão funcionando para trás
	while(millis() - tempo < 300){
		driver.setMotors(-80, -80); // Define a velocidade dos motores para -80

		sensor_linha.calibration_max(); // Calibra o sensor de linha

		sensor_esq.find_max(); // Encontra o valor máximo para o sensor esquerdo
		sensor_dir.find_max(); // Encontra o valor máximo para o sensor direito
	}
	driver.Break(); // Pára os motores
	delay(500);
	tempo = millis();

	// Loop para calibrar os sensores durante 300 ms enquanto os motores estão funcionando para a frente
	while(millis() - tempo < 300){
		driver.setMotors(80, 80); // Define a velocidade dos motores para 80

		sensor_linha.calibration_max(); // Calibra o sensor de linha

		sensor_esq.find_max(); // Encontra o valor máximo para o sensor esquerdo
		sensor_dir.find_max(); // Encontra o valor máximo para o sensor direito
	}
	driver.Break(); // Pára os motores

	// Se a calibração foi bem sucedida, pisca os LEDs 5 vezes
	if (sensor_linha.CheckCalibration()){
		if(sensor_esq.GetMax() != 0 and sensor_dir.GetMax() != 0){
			PiscaLed(5);
		}
	}
}

// Função para o controle PID
void Seguidor::controle()
{	
	// Verifica se passou o tempo de amostragem
	if(millis() - execTime >= samplingTime){
		execTime = millis();

		erro = sensor_linha.getAngle(); // Obtém o ângulo do sensor de linha

		// Se o erro absoluto é maior que "out", calcula a redução de velocidade translacional e liga o LED
		if(abs(erro) > out){
			trans = abs(erro)*k;
			LigaLed();
		} else{
			trans = 0;
		}

		rot = controlador.calcPID(erro); // Calcula a saída do controlador PID

		driver.Set_speedRot(rot - trans); // Define a velocidade de rotação do motor
	}
}

// Função para verificar se o robô saiu da linha
bool Seguidor::IsOut()
{
	if(abs(erro) > out) return true;

	return false;
}

// Função para mapeamento
void Seguidor::mapeamento()
{
	// Se o robô chegou a um cruzamento à esquerda
	if(CheckLateralEsq()){
		// Configura a velocidade base dependendo se está em uma reta ou curva
		if(isReta){
			// Entrou em uma curva
			SerialBT.println("Curva");
			driver.setVB(Vbc);
			isReta = false;
		}else{
			// Entrou em uma reta
			SerialBT.println("Reta");
			driver.setVB(Vbr);
			isReta = true;
		}
	}
}

// Função para iniciar a execução do seguidor
void Seguidor::Run()
{
	start = true;
	end = false;
	startTime = millis();
	controlador.resetConditions();

	if(isCalibrado == false){
		sensor_linha.calibation_manual();

		sensor_esq.Cmax = 1220;
		sensor_dir.Cmax = 800;
	}	
}

// Função para parar a execução do seguidor
void Seguidor::Stop()
{
	SerialBT.println("Parado"); // Imprime uma mensagem de "Parado"
	driver.Break(); // Pára os motores
	start = false;
}

// Função para verificar o comportamento do seguidor
void Seguidor::Behavior()
{
	comunica_serial(); // Comunica com o Bluetooth

	switch (command[0]) // Verifica o primeiro caractere do comando
	{
	case 'S': // Se o comando é "S"
		set_handler(); // Chama a função para manipular as configurações
		command = ""; // Limpa o comando
		break;
	case 'P': // Se o comando é "P"
		Stop(); // Chama a função para parar a execução do seguidor
		command = ""; // Limpa o comando
		break;
	case 'R': // Se o comando é "R"
		Run(); // Chama a função para iniciar a execução do seguidor
		command = ""; // Limpa o comando
		break;
	case 'C': // Se o comando é "C"
		calibration(); // Chama a função para calibrar os sensores
		command = ""; // Limpa o comando
		break;
	case 'B': // Se o comando é "B"
		bateryCheck(); // Chama a função para verificar o nível da bateria
		command = ""; // Limpa o comando
		break;	
	default: // Se o comando é qualquer outra coisa
		command = ""; // Limpa o comando
		break;
	}
}

// Função para comunicar com o Bluetooth
void Seguidor::comunica_serial()
{
	if(SerialBT.available()){ 
		command = SerialBT.readStringUntil(';'); // Lê a string até o delimitador ";" e armazena em "command"
	}
}

// Função para a rotina de parada
void Seguidor::stopRoutine()
{
	// Para o seguidor no final da pista
	if(millis() - startTime > 3000){
		if (CheckLateralDir() and end == false){
			end = true;
			stopTime = millis();
		}else if (millis() - stopTime > 300 and end == true) Stop();
	}
}

// Função para verificar se o robô chegou a um cruzamento à direita
bool Seguidor::CheckLateralDir()
{
	if(sensor_dir.Read_histerese() == HIGH and checking_encruzilhada_dir == false and gate_sensor == false) {
		gate_sensor = true;
		checking_encruzilhada_dir = true;
		encruzilhada_timer = millis();
	}else if(millis() - encruzilhada_timer < 160){
		if(sensor_esq.Read_histerese() == HIGH){
			checking_encruzilhada_dir = false;
			return false;
		}
	}else if(checking_encruzilhada_dir == true){
		checking_encruzilhada_dir = false;
		return true;
	}

	if(millis() - encruzilhada_timer > 180)  gate_sensor = false;

	return false;
}

// Função para verificar se o robô chegou a um cruzamento à esquerda
bool Seguidor::CheckLateralEsq()
{
	if(sensor_esq.Read_histerese() == HIGH and checking_encruzilhada_esq == false and gate_sensor_esq == false) {
		gate_sensor_esq = true;
		checking_encruzilhada_esq = true;
		encruzilhada_timer_esq = millis();
	}else if(millis() - encruzilhada_timer_esq < 100){
		if(sensor_dir.Read_histerese() == HIGH){
			checking_encruzilhada_esq = false;
			return false;
		}
	}else if(checking_encruzilhada_esq == true){
		checking_encruzilhada_esq = false;
		LigaLed();
		return true;
	}

	if(millis() - encruzilhada_timer_esq > 200)  gate_sensor_esq = false;

	return false;
}

// Função para verificar se o seguidor está começando
bool Seguidor::isStart()
{
	return start;
}

// Função para verificar o nível da bateria
void Seguidor::bateryCheck()
{
	// TODO: Verificar o valor da bateria no ADC em 6.5V
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

// Função para verificar o LED
void Seguidor::CheckLed()
{
	if(is_led_on == true and millis() - ledTimer > 300){
		DesligaLed();
	}
}

// Função para ligar o LED
void Seguidor::LigaLed()
{
	// Acende os LEDs
	digitalWrite(led_esq_pin, HIGH);
	digitalWrite(led_dir_pin, HIGH);

	ledTimer = millis();
	is_led_on = true;
}

// Função para desligar o LED
void Seguidor::DesligaLed()
{
	is_led_on = false;
	// Desliga os LEDs
	digitalWrite(led_esq_pin, LOW);
	digitalWrite(led_dir_pin, LOW);
}

// Função para piscar o LED um determinado número de vezes
void Seguidor::PiscaLed(int num_piscadas)
{
	for(unsigned i = 0; i < num_piscadas; i++){
		LigaLed();
		delay(200);
		DesligaLed();
		delay(200);
	}
}

// Função de teste
void Seguidor::teste()
{
	sensor_linha.testeLeitura(sensor_linha.RAW);
	delay(10);
}

// Função para testar os sensores laterais
void Seguidor::TesteSensoresLat()
{
	// Calibração dos sensores laterais
	sensor_esq.Cmax = 1440;
	sensor_dir.Cmax = 815;

	Serial.print("Sensor Dir: ");
	Serial.print(sensor_dir.Read_Calibrado());
	Serial.print("   ");
	Serial.print("Sensor Esq: ");
	Serial.println(sensor_esq.Read_Calibrado());
}
