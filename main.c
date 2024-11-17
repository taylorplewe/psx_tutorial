#include <sys/types.h>
#include <stdio.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>
#define OTLEN 8
#define BOX_SIZE 64
#define SCRN_WIDTH 320
#define SCRN_HEIGHT 240

DISPENV disp[2];
DRAWENV draw[2];
int db;
u_long ot[2][OTLEN];

char primbuff[2][32768];
char* nextprim;

int boxX, boxY;
int boxHspeed, boxVspeed;

void init() {
	ResetGraph(0);

	SetDefDispEnv(&disp[0], 0, 0, SCRN_WIDTH, SCRN_HEIGHT);
	SetDefDispEnv(&disp[1], 0, SCRN_HEIGHT, SCRN_WIDTH, SCRN_HEIGHT);
	SetDefDrawEnv(&draw[0], 0, SCRN_HEIGHT, SCRN_WIDTH, SCRN_HEIGHT);
	SetDefDrawEnv(&draw[1], 0, 0, SCRN_WIDTH, SCRN_HEIGHT);

	setRGB0(&draw[0], 50, 200, 50);
	setRGB0(&draw[1], 50, 200, 50);
	draw[0].isbg = 1;
	draw[1].isbg = 1;

	PutDispEnv(&disp[0]);
	SetDispMask(1);
	PutDrawEnv(&draw[0]);

	FntLoad(960, 0);
	FntOpen(0, 8, 320, 224, 0, 100);

	nextprim = primbuff[0];

	db = 0;
}

void display() {
	DrawSync(0);
	VSync(0);

	
	PutDispEnv(&disp[db]);
	PutDrawEnv(&draw[db]);

	DrawOTag(ot[db]+(OTLEN-1));

	db = !db;
	nextprim = primbuff[db];
}

void updateBox() {
	boxX += boxHspeed;
	boxY += boxVspeed;

	if (boxX + BOX_SIZE > SCRN_WIDTH) {
		boxX = SCRN_WIDTH - BOX_SIZE;
		boxHspeed *= -1;
	} else if (boxX < 0) {
		boxX = 0;
		boxHspeed *= -1;
	}
	if (boxY + BOX_SIZE > SCRN_HEIGHT) {
		boxY = SCRN_HEIGHT - BOX_SIZE;
		boxVspeed *= -1;
	} else if (boxY < 0) {
		boxY = 0;
		boxVspeed *= -1;
	}
}

int main() {
	init();

	TILE* tile;
	boxX = 32;
	boxY = 32;
	boxHspeed = 1;
	boxVspeed = 1;

	while (1) {
		FntPrint(0, "Hello California");
		FntFlush(-1);

		updateBox();

		ClearOTagR(ot[db], OTLEN);
		tile = (TILE*)nextprim;
		setTile(tile);
		setXY0(tile, boxX, boxY);
		setWH(tile, BOX_SIZE, BOX_SIZE);
		setRGB0(tile, 255, 255, 0);
		nextprim += sizeof(TILE);
		addPrim(ot[db], tile);

		display();
	}

	return 0;
}