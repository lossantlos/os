
#include <pcspkr.h>

#define char2int(x) (x+'a')

/*
	// C2 D2 E2 G2 E2 D2 C2 - LOTR
	tone("c4", 6);
	tone("d4", 5);
	tone("e4", 10);
	tone("g4", 9);
	tone("e4", 8);
	tone("d4", 6);
	tone("c4", 5);
	*/

void tone(const char c[2], int time)
{
	int f = 0;
	switch (c[0]) {
		case 'a':
			f = 27; //27.5
			break;
		case 'b':
			f = 31; //30.8677
			break;
		case 'c':
			f = 33; //32.7032
			break;
		case 'd':
			f = 37;//36.7081
			break;
		case 'e':
			f = 41; //41.2034
			break;
		case 'f':
			f = 44; //43.6535
			break;
		case 'g':
			f = 49; //48.9994
			break;
	}

	f *= char2int(c[1]);
	play_sound(f);
	pic_wait(time);
	nosound();
}
