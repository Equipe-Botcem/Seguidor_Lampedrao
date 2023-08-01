// Inclui o arquivo de cabeçalho da classe Sensor_linha e SimpleKalmanFilter
#include "Sensor_linha.h"
#include "SimpleKalmanFilter.h"

// Cria um objeto de filtro Kalman para filtrar o ângulo
SimpleKalmanFilter angleKalmanFilter(0.01, 0.01, 0.005);

// Construtor padrão da classe Sensor_linha
Sensor_linha::Sensor_linha(){}

// Construtor da classe Sensor_linha que aceita uma matriz de pinos como argumento
Sensor_linha::Sensor_linha(unsigned char* pins){
    for(unsigned i = 0; i < 8; i++){
		// Cria um novo objeto Sensor para cada pino e configura o ângulo correspondente
		sensores[i] = Sensor(pins[i]);
		sensores[i].setAngle(angulos[i]);
	}
	// Inicializa a última leitura como 0
    last_read = 0;
}

// Método para inicializar os sensores
void Sensor_linha::Init(){
    for(unsigned i = 0; i < 8; i++){
		// Inicializa cada sensor
		sensores[i].Init();
	}
}

// Método para obter o ângulo bruto
float Sensor_linha::getAngleRaw(){
    int j = 4;

	for(int i = 3; i >= 0; i--){
		// Lê cada sensor e se a leitura for verdadeira, retorna a média ponderada
		if(sensores[i].Read_histerese()){
            last_read = mediaPond(i);
            return last_read;  
        } 
		if(sensores[j].Read_histerese()){
            last_read = mediaPond(j);
            return last_read;
        } 
		j++;
	}

	// Se não for detectado nenhum ângulo, assume que saiu da pista
	if(last_read < 0) return -50;

	return 50;
}

// Método para obter o ângulo filtrado pelo filtro Kalman
float Sensor_linha::getAngle(){
	// Atualiza a estimativa do ângulo com o filtro Kalman
	return angleKalmanFilter.updateEstimate(getAngleRaw());
}

// Método para calcular a média ponderada dos sensores
float Sensor_linha::mediaPond(int pos){
    float num;
	float den;

	// Cálculos diferentes dependendo da posição do sensor
	if(pos == 0){
		num = sensores[pos].Read_CalibradoPonderado() + sensores[pos + 1].Read_CalibradoPonderado();
		den = sensores[pos].Read_Calibrado() + sensores[pos + 1].Read_Calibrado();
	}else if (pos == 7){
		num = sensores[pos].Read_CalibradoPonderado() + sensores[pos - 1].Read_CalibradoPonderado();
		den = sensores[pos].Read_Calibrado() + sensores[pos - 1].Read_Calibrado();
	}else{
		num = sensores[pos].Read_CalibradoPonderado() + sensores[pos - 1].Read_CalibradoPonderado() + sensores[pos + 1].Read_CalibradoPonderado();
		den = sensores[pos].Read_Calibrado() + sensores[pos - 1].Read_Calibrado() + sensores[pos + 1].Read_Calibrado();
	}
	// Retorna a média ponderada
	return num / den;
}

// Método para calibrar os valores máximos dos sensores
void Sensor_linha::calibration_max(){
	for(unsigned i = 0; i < 8; i++) sensores[i].find_max();
}

// Método para a calibração manual dos sensores
void Sensor_linha::calibation_manual(){
	for(unsigned i = 0; i < 8; i++){
		sensores[i].Cmax = c_max[i];
	}
}

// Método para testar a leitura dos sensores
void Sensor_linha::testeLeitura(ReadType tipo){

	switch (tipo){
	case RAW:
		// Imprime os valores brutos dos sensores
		for (int i = 0; i < 8; i++){
			Serial.print("S");
			Serial.print(String(i));
			Serial.print(": ");
			Serial.print(sensores[i].Read_sensor());
			Serial.print("       ");
    }
		Serial.println();
		break;
	case CALIB:
		// Imprime os valores calibrados dos sensores
		calibation_manual();
		for (int i = 0; i < 8; i++){
			Serial.print("S");
			Serial.print(String(i));
			Serial.print(": ");
			Serial.print(sensores[i].Read_Calibrado());
			Serial.print("       ");
		}
		Serial.println();
		break;
	case HIST:
		// Imprime os valores de histerese dos sensores
		calibation_manual();
		for (int i = 0; i < 8; i++){
			Serial.print("S");
			Serial.print(String(i));
			Serial.print(": ");
			Serial.print(sensores[i].Read_histerese());
			Serial.print("       ");
    }
		Serial.println();
		break;

		case KALMAN:
			// Imprime o ângulo bruto e o ângulo filtrado pelo filtro Kalman
			calibation_manual();
			Serial.print("Angle:");
			Serial.println(getAngleRaw());
			Serial.print(",");
			Serial.print("Kalman_filter:");
			Serial.println(getAngle());
		break;
	}
}

// Método para verificar se os sensores estão calibrados
bool Sensor_linha::CheckCalibration(){
	for(unsigned i = 0; i < 8; i++){
		if(sensores[i].GetMax() == 0) return false;
	}
	return true;
}
