/* ===============================================
|	MyMenu library for LCD with 2+ lines of text
|   	
|   Usage example is in *.cpp
|
|	Made by Denis Peshekhonov, 2013
| ===============================================
*/

#ifndef MYMENU_H
#define MYMENU_H

#include <LiquidCrystal.h>

class MItm
{
  public:
    MItm(String _name, int _first, int _second = -1);
	void addPars(int _sub_id, int _back_id, String _sub_title);
    String name;
    int first;
    int second;
	int sub_id;
	int back_id;
	String sub_title;
};

class Menu
{
  public:
    Menu(MItm *_items, int _num_items, LiquidCrystal* _lcd, void (*_callback)(int), int _num_rows = 4, String _cursor = ">");
	void goNext();
	void goBack();
  	void goMain();
	void goDown();
	void goUp();
	void goLast();
	void goSub(int sub_idx);
	void goItem(int item_idx);
  private:
	void drawSub();
	int findFirst(MItm citm);
	void (*callback)(int);

    MItm *items;
    LiquidCrystal *lcd;  
	int num_rows;
	int cur_sub;
	int cur_item;
	int cur_sub_index;
	
	int subsize;
	int fullsize;
	
	int cur_pos;
	String cursor;
	
	String title;
	String main_title;
};

#endif