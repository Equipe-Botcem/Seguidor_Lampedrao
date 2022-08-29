#include "include.h"



class Encoder
{
private:
	static unsigned char pin_a;
	static int contador_a;

public:
	Encoder();
	Encoder(unsigned char a);
	~Encoder();

	void Init();
	static void Rotina();

	int Get_contador();
	void Reset();
};
