#include "Driver.h"
#include "Arduino.h"

// Construtor padrão
Driver::Driver(){}

// Construtor que configura os pinos dos motores
Driver::Driver(unsigned char *pins_dir, unsigned char *pins_esq){
    motor_esq = Motor(pins_esq[0], pins_esq[1], pins_esq[2], pins_esq[3]); // Configura o motor esquerdo
	motor_dir = Motor(pins_dir[0], pins_dir[1], pins_dir[2], pins_dir[3]); // Configura o motor direito
}

// Função para inicializar os motores
void Driver::Init(){
    motor_dir.Init(); // Inicializa o motor direito
    motor_esq.Init(); // Inicializa o motor esquerdo
}

// Função para definir a velocidade base dos motores
void Driver::setVB(int vb){
    VB = vb;
}

// Função para ajustar a velocidade dos motores
void Driver::setMotors(int speed_esq, int speed_dir){
    motor_esq.Set_speed(speed_esq); // Ajusta a velocidade do motor esquerdo
    motor_dir.Set_speed(speed_dir); // Ajusta a velocidade do motor direito
}

// Função para habilitar os drivers dos motores
void Driver::Enable_motors_drives()
{
	motor_esq.Enable_drive(); // Habilita o driver do motor esquerdo
	motor_dir.Enable_drive(); // Habilita o driver do motor direito
}

// Função para desabilitar os drivers dos motores
void Driver::Disable_motors_drives()
{
	motor_esq.Disable_drive(); // Desabilita o driver do motor esquerdo
	motor_dir.Disable_drive(); // Desabilita o driver do motor direito
}

// Função para parar os motores
void Driver::Break(){
    motor_dir.Set_speed(0); // Define a velocidade do motor direito como 0
    motor_esq.Set_speed(0); // Define a velocidade do motor esquerdo como 0
}

// Função para ajustar a velocidade de rotação dos motores
void Driver::Set_speedRot(int rot)
{
	motor_dir.Set_speed(VB + rot); // Ajusta a velocidade do motor direito como a velocidade base mais a rotação
    motor_esq.Set_speed(VB - rot); // Ajusta a velocidade do motor esquerdo como a velocidade base menos a rotação
}

// Função de teste para os motores
void Driver::teste(){
    // Move os motores para frente
    setMotors(100, 100); // Define a velocidade de ambos os motores como 100
    delay(5000); // Espera 5 segundos

    // Move os motores para trás
    setMotors(-100,-100); // Define a velocidade de ambos os motores como -100
    delay(5000); // Espera 5 segundos

    // Para os motores
    Break(); // Chama a função para parar os motores
    delay(5000); // Espera 5 segundos
}
