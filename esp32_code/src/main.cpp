// Inclui o arquivo de cabeçalho da classe Seguidor
#include "Seguidor.h"

// Verifica se o Bluetooth está habilitado. 
// Se não estiver, emite um erro e solicita que seja habilitado.
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Cria um objeto da classe Seguidor.
Seguidor seguidor = Seguidor();

// A função setup() é chamada uma vez quando um sketch começa
void setup()
{
	// Inicia a comunicação serial a uma velocidade de 115200 bits por segundo
	Serial.begin(115200);
	// Chama a função Init() do objeto seguidor para inicializar os componentes do seguidor
	seguidor.Init();
	// Chama a função initBluetooth() do objeto seguidor para inicializar o Bluetooth
	seguidor.initBluetooth();
}

// A função loop() é chamada repetidamente e é o coração da maioria dos sketches do Arduino.
void loop(){
	
	// Comportamento principal do seguidor
	seguidor.Behavior();
	
	// Verifica o estado do LED
	seguidor.CheckLed();

	// Verifica se o seguidor foi iniciado
	if(seguidor.isStart()){
		// Se o seguidor foi iniciado, controle o seguidor
		seguidor.controle();

		// Verifica se o seguidor saiu da linha
		if(!seguidor.IsOut()){
			// Se o seguidor saiu da linha, para a rotina
			seguidor.stopRoutine();
		}

		// Mapeia o percurso
		seguidor.mapeamento();	
	}
}
