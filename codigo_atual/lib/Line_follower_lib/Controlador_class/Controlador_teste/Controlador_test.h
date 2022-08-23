#include "Controlador_class/Controlador.h"

unsigned char pins_motores[5] = {0, 1, 2, 3, 4};

// só configura o motor "esquerdo"
void Controlador_teste_config();

//! pode dar erro devido a não ter as outras peças
void Controlador_teste_init();

void Controlador_teste_enable_motors_drives();

void Controlador_teste_disable_motors_drives();

void Controlador_teste_set_direction_forward();

void Controlador_teste_set_direction_reverse();

/**
 * @brief liga o motor "esquerdo" por 5 segundos então desliga
 *
 */
void Controlador_teste_command_1();