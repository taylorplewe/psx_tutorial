#include <sys/types.h>
#include <stdio.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>

DISPENV disp[2];
DRAWENV draw[2];
int db;

void init() {
	ResetGraph(0);

	SetDefDispEnv(&disp[0], 0, 0, 320, 240);
	SetDefDispEnv(&disp[1], 0, 240, 320, 240);
	SetDefDrawEnv(&draw[0], 0, 240, 320, 240);
	SetDefDrawEnv(&draw[1], 0, 0, 320, 240);

	setRGB0(&draw[0], 50, 200, 50);
	setRGB0(&draw[1], 50, 200, 50);
	draw[0].isbg = 1;
	draw[1].isbg = 1;

	PutDispEnv(&disp[0]);
	SetDispMask(1);
	PutDrawEnv(&draw[0]);

	FntLoad(960, 0);
	FntOpen(0, 8, 320, 224, 0, 100);

	db = 0;
}

void display() {
	DrawSync(0);
	VSync(0);

	db = !db;
	
	PutDispEnv(&disp[db]);
	PutDrawEnv(&draw[db]);
}

int main() {
	init();

	while (1) {
		FntPrint(0, "Hello California");
		FntFlush(-1);

		display();
	}

	return 0;
}