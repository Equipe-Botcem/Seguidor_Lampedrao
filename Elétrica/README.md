# Documentação do Projeto Eletrônico do Seguidor de Linha Alpha

## 1. Projeto Elétrico

O projeto elétrico tem seu funcionamento separado em três parte: potência (alimentação) e controle.

### 1.1. Potência

O circuito de potência é composto pelos seguintes elementos:

- Bateria: LiPo 7,4V 2S 300mAh;
- Conversor de tensão Step Up: XL6009;
- Regulador de Tensão 5V: LM7805;
- Regulador de Tensão 3.3V: LM1117;
- Capacitores de Filtro: 0.22uF, 0.1uF e 10uF.
- Diodo;
- Switchs.

### 1.2. Controle

O circuito de controle é composto pelos seguintes elementos:

- Shield ESP32;
- Drivers IC: TB9051FTG Single Brushed;
- Encoders;
- Sensores de Refletância: QRE1117;
- Resistores: 47Kohns e 47ohns.

## 2. Projeto da PCB

O projeto da PCB foi desenvolvido no software KiCAD.
As dimensões da placa são: 119mm X 151mm X 1.6mm (Comprimento X Largura X Altura).
A placa é composta por duas camadas de trilhas, inferior e superior.

## 3. Medições

### 3.1. Corrente Consumida

Realizando a medição do Seguidor de Linha por uma fonte de tensão, pode-se obter as seguintes medidas de consumo:

- Ligado: 300mA;
- Velocidade Baixa: 600mA;
- Velocidade Alta: 1.5A. 

## 4. Melhorias

- Desenvolvimento de uma placa totalmente integrada.
