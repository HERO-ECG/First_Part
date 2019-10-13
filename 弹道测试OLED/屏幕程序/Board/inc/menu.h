#ifndef _MENU_H_
#define _MENU_H_

#include "stm32f10x.h"

#define MENUSIZE 10

#define FLASH_SAVE_ADDR 0X08007c00

typedef struct MENU {
	double *par;				//参数
	double add;
	char name[MENUSIZE];
	int num;
	
	struct MENU *up;			//上
	struct MENU *down;		//下

}Menu;

void GetName(const char *par_name, char *name);
void GetPar(const char *par_name, double *par,double add,Menu *menu );

void CreateMenu(const char *par_name, double *par, double add,Menu *menu);
void AddMenu(const char *par_name, double *par,double add,Menu *par_menu);

void PrintMenu(Menu *p,Menu *p_now,int y);
void PrintPar(Menu *p_now);
void PrintAllMenu(Menu *p_now,int serial);
//void DeleteMenu(Menu *head);

void Menu_AdjustPar(Menu *menu);
void Menu_ShowPar(void);

void ReadFlash(void);
void WriteFlash(void);
void Menu_Init(void);

void showSpeed(double *speed);












#endif
