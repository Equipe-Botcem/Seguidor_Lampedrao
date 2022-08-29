#include "motor_drive.h"



void Motor_drive_teste_init();

void Motor_drive_teste_enable();

void Motor_drive_teste_disable();

void Motor_drive_teste_set_forward();

void Motor_drive_teste_set_reverse();

void Motor_drive_teste_set_speed(int speed);

double Motor_drive_teste_get_current(int speed);

/**
 * @brief Liga o motor para frente por 5 segundos e então desliga (Disable)
 */
void Motor_drive_teste_command_1();

/**
 * @brief Liga o motor para frente por 5 segundos, inverte a direção e então desliga (Disable)
 * ! Pode acontecer de danificar a ponte H ao inverter diratamente
 */
void Motor_drive_teste_command_2();

/**
 * @brief Liga o motor para frente com velocidade zero e a cada 20 ms acelera em 1 até 255 e então desliga (Disable)
 */
void Motor_drive_teste_command_3();

/**
 * @brief Liga o motor para frente com velocidade zero e a cada 20 ms acelera em 1 até 255, mostra a corrente atual no drive a cada variação e então desliga (Disable)
 */
void Motor_drive_teste_command_4();