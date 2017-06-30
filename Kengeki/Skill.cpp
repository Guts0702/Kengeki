#include "Header.h"

void ZSkill()
{
	g_Zangeki.x += 12.f;
}
void ZSkill2()
{
	g_Zangeki2.x += 12.f;
}
void XSkill()
{
	int Xcount = 0;

	for (int i = 0; i < 100; i++) {
		Xcount++;
		if (g_Player.x >= 720.f)
		{
			XskillFlag = false;
			break;
		}
		if (Xcount >= 5) {
			g_Kirinuke.x += 2.f;
			g_Player.x += 2.f;
		}
	}
}