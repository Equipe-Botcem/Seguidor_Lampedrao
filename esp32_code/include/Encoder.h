#ifndef _ENCODER_
#define _ENCODER_

class Encoder
{
private:
	unsigned char pin_a;
	static volatile int contador_a;
	static volatile int contador_b;

public:
	Encoder();
	Encoder(unsigned char a);
	~Encoder();

	void Init_dir();
	void Init_esq();

	static void Rotina_dir();
	static void Rotina_esq();

	int Get_contador_dir();
	int Get_contador_esq();

	void Reset_dir();
	void Reset_esq();
};

#endif