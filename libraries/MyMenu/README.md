MyMenu
======

Menu library for LCDs with 2+ lines of text

Usage:

	#define NUM_ITEMS 7 //total number of items in menu include submenu-headers
	#define NUM_ROWS 4 //total number of rows in your lcd, must be greater than or equal to 2

	MItm = items[NUM_ITEMS] = {
		MItm("MAIN MENU title",0,0), //main header, always 0,0
		MItm("Some item",1), //second parameter is submenu-index, which will be opened by this item
		MItm("Second item",2),
		MItm("FIRST SUBMENU",1,0), //sub-menu header (two input parameters), 1 is submenu index, any unique integer for submenus, 0 is previous-menu index for right hierarchy
		MItm("Submenu item one",3),
		MItm("Sub item two",4),
		MItm("BACK",0) //optional BACK item
	};

	LiquidCrystal lcd(...); // your lcd initialization
	String cursor = "-"; // the menu cursor, by default is ">"
	Menu menu(items,NUM_ITEMS,&lcd,menuCallback,NUM_ROWS,cursor); //notice &lcd instead of lcd, we will work with pointer
	menu.goMain(); // shows main menu on display

	//Then you can call anywhere you want (on button press, for example)
	menu.goDown(); // to list current menu down, this will move cursor from current item to next, but wont select it
	menu.goUp(); // to list up
	menu.goNext(); // to go to submenu or run callback (i.e "select item")
	menu.goBack(); // to go to previous menu
	menu.goLast(); // to return to last opened submenu
	menu.goSub(int sub_idx); // to go to submenu with index <sub_idx>
	menu.goItem(int item_idx); // to go to submenu contains item with index (in "items" array) <item_idx> and move cursor to this item
	
	//also you need to declare callback, it will be run if there is no submenu with current index
	//e.g.
	//If current item is MItm("Title",5) and you call goNext(), then:
	//1. If you have Submenu-header with index 5 (MItm("SUBMENU",5,0)) it will be opened
	//2. If you have not, callBack will be run width "5" as input parameter 

	void menuCallback(int idx){
		Serial.println(idx);
	}
