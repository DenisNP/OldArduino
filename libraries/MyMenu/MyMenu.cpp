/* ===============================================
|	//MyMenu library for LCD with 2+ lines of text
|   	
|   //Usage:
|   
|   #define NUM_ITEMS 7 //total number of items in menu include submenu-headers
|	#define NUM_ROWS 4 //total number of rows in your lcd, must be greater than or equal to 2
|
|	MItm = items[NUM_ITEMS] = {
|		MItm("MAIN MENU title",0,0), //main header, always 0,0
|		MItm("Some item",1), //second parameter is submenu-index, which will be opened by this item
|		MItm("Second item",2),
|		MItm("FIRST SUBMENU",1,0), //sub-menu header (two input parameters), 1 is submenu index, any unique integer for submenus, 0 is previous-menu index for right hierarchy
|		MItm("Submenu item one",3),
|		MItm("Sub item two",4),
|		MItm("BACK",0) //optional BACK item
|	};
|
|	LiquidCrystal lcd(...); // your lcd initialization
|	String cursor = "-"; // the menu cursor, by default is ">"
|	Menu menu(items,NUM_ITEMS,&lcd,menuCallback,NUM_ROWS,cursor); //notice &lcd instead of lcd, we will work with pointer
|	menu.goMain(); // shows main menu on display
|
|	//Then you can call anywhere you want (on button press, for example)
|	menu.goDown(); // to list current menu down, this will move cursor from current item to next, but wont select it
|	menu.goUp(); // to list up
|	menu.goNext(); // to go to submenu or run callback (i.e "select item")
|	menu.goBack(); // to go to previous menu
|	menu.goLast(); // to return to last opened submenu
|	menu.goSub(int sub_idx); // to go to submenu with index <sub_idx>
|	menu.goItem(int item_idx); // to go to submenu contains item with index (in "items" array) <item_idx> and move cursor to this item
|
|	//also you need to declare callback, it will be run if there is no submenu with current index
|	//e.g.
|	//If current item is MItm("Title",5) and you call goNext(), then:
|	//1. If you have Submenu-header with index 5 (MItm("SUBMENU",5,0)) it will be opened
|	//2. If you have not, callBack will be run width "5" as input parameter 
|	
|	void menuCallback(int idx){
|		Serial.println(idx);
|	}
|
| ===============================================
| Made by Denis Peshekhonov, 2013 
| ===============================================
*/

#include <LiquidCrystal.h>
#include <MyMenu.h>
#include <math.h> 

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

String name;
int first;
int second;
int sub_id;
int back_id;
String sub_title;

MItm::MItm(String _name, int _first, int _second){
	sub_id = -1;
	back_id = -1;
  	name = _name;
  	first = _first;
  	second = _second;
}

void MItm::addPars(int _sub_id, int _back_id, String _sub_title){
	sub_id = _sub_id;
	back_id = _back_id;
	sub_title = _sub_title;
}

Menu::Menu(MItm *_items, int _num_items, LiquidCrystal* _lcd, void (*_callback)(int), int _num_rows, String _cursor){
	items = _items;
	num_rows = _num_rows;
    lcd = _lcd;
	fullsize = _num_items;
	callback = _callback;

	cur_sub = 1;
	cur_sub_index = 0;
	cur_item = -1;
	cur_pos = 0;
	cursor = _cursor;
	
	main_title = items[0].name;
		
	int next_sub_id = -1;
	int next_back_id = -1;
	String next_sub_title = "";
		
	for(int i=0; i<fullsize; i++){
		if(items[i].second > -1){
			next_sub_id = items[i].first;
			next_back_id = items[i].second;
			next_sub_title = items[i].name;
		}else{
			items[i].addPars(next_sub_id,next_back_id,next_sub_title);
		}
	}
	
	//goMain();
}

void Menu::goMain(){
	cur_item = -1;
	goNext();
}

void Menu::goDown(){
	cur_item++;
	cur_sub_index++;
	
	bool from_beginning = cur_item >= fullsize;
	
	if(cur_item < fullsize){
		if(items[cur_item].sub_id != cur_sub){
			from_beginning = true;
		}
	}
	
	if(from_beginning){
		cur_item = cur_item - cur_sub_index;
		cur_sub_index = 0;
	}
	
	drawSub();
}

void Menu::goUp(){
	cur_item--;
	cur_sub_index--;
	
	bool from_end = cur_item < 0;
	
	if(cur_item >= 0){
		if(items[cur_item].sub_id != cur_sub){
			from_end = true;
		}
	}
	
	if(from_end){
		int cur_item_start = cur_item+1;
		cur_sub_index = 0;
		
		for(int i=cur_item_start; i<fullsize; i++){
			if(items[i].sub_id == cur_sub){
				cur_sub_index++;
			}
		}
		
		cur_item = cur_item + cur_sub_index;
		cur_sub_index--;
	}
	
	drawSub();
}

void Menu::goNext(){

	if(cur_item != -1){
		int cur_item2 = findFirst(items[cur_item]);
		if(cur_item2 == -1){
			(*callback)(items[cur_item].first);
		}else{
			cur_item = cur_item2;
			MItm citm = items[cur_item];
			title = citm.sub_title;
			cur_sub = citm.sub_id;
			
			cur_sub_index = 0;
			drawSub();
		}
	}else{
		title = main_title;
		cur_sub = 0;
		cur_item = 1;
		
		cur_sub_index = 0;
		drawSub();
	}
}

void Menu::goSub(int sub_idx){
	for(int i=0; i<fullsize; i++){
		if(items[i].second > -1 && items[i].first == sub_idx){
			cur_sub = sub_idx;
			cur_item = i+1;
			cur_sub_index = 0;
			title = items[i].name;
			
			drawSub();
			break;
		}
	}
}

void Menu::goItem(int item_idx){
	MItm citm = items[item_idx];
	if(citm.second > -1){
		goSub(citm.first);
	}else{
		for(int i=0; i<fullsize; i++){
			if(items[i].second > -1 && items[i].first == citm.sub_id){
				cur_sub = citm.sub_id;
				cur_item = item_idx;
				cur_sub_index = item_idx-i-1;
				title = items[i].name;
				
				drawSub();
				break;
			}
		}
	}
}

void Menu::goLast(){
	goItem(cur_item);
}

void Menu::goBack(){
	goSub(items[cur_item].back_id);
}

void Menu::drawSub(){
	int start_item = -1;
	int page = floor(cur_sub_index/(num_rows-1));
	int line = 1;
	
	(*lcd).clear();
	(*lcd).setCursor(0,0);
	(*lcd).print(title+":");
	
	for(int i=0; i<fullsize; i++){
		MItm citem = items[i];
		
		if(citem.sub_id == cur_sub){
			if(start_item == -1){
				start_item = i;
			}

			int idx = (i - start_item);
			bool begin_draw = idx >= page*(num_rows-1);
			
			if(i + num_rows - 1 >= fullsize){
				begin_draw = true;
			}else{
				if(items[i + num_rows - 1].sub_id != cur_sub){
					begin_draw = true;
				}
			}
			
			if(begin_draw && line < num_rows){
				(*lcd).setCursor(0,line);
				if(i == cur_item){
					(*lcd).print(cursor+citem.name);
				}else{
					(*lcd).print(String("                    ").substring(1,cursor.length()+1)+citem.name);
				}
				line++;
			}
		}
	}
}

int Menu::findFirst(MItm citm){
	for(int i=0; i<fullsize; i++){
		MItm sitm = items[i];
		if(sitm.sub_id == citm.first){
			return i;
		}
	}
	
	return -1;
}