//Lithio
//November 07/15, 2021

//NPC... YEeah...

//This is not using any engine, for I just want to make a quick implementation
//that gives insight conceptually on how it will be put in the game.

#include <map>
#include <vector>

class Player{
	int x;
	int y;
	//npc's use this to keep track of the player's conversations
	int npc_data[1000];
};

class NPC{
	int id;
	int x;
	int y;
}

class NPCCondition{
	//default constructor
	NPCCondition();
	
	//define conditions. Multiple conditions can be given!
	void level(int min, int max);
	void item(int id, int min, int max);
	void hp(int min, int max);
	void mp(int min, int max);
	
	//0x01 for level, 0x02 for item, 0x04 for HP, 0x08 for MP,
	//0x10 for data value
	int type;
	
	int level_min;
	int level_max;
	
	int item_id;
	int item_min;
	int item_max;
	
	int hp_min;
	int hp_max;
	
	int mp_min;
	int mp_max;
	
	int data_id;
	int data_min;
	int data_max;
};

NPCCondition::NPCCondition():
type(0),
level_min(0),
level_max(0),
item_id(0),
item_min(0),
item_max(0),
hp_min(0),
hp_max(0),
mp_min(0),
mp_max(0),
data_id(0),
data_min(0),
data_max(0)
{
}

void NPCCondition::level(int min, int max){
	type |= 0x01;
	level_min = min;
	level_max = max;
}
	
void NPCCondition::item(int id, int min, int max){
	type |= 0x02;
	item_id = id;
	item_min = min;
	item_max = max;
}

void NPCCondition::hp(int min, int max){
	type_ |= 0x04;
	hp_min = min;
	hp_max = max;
}

void NPCCondition::mp(int min, int max){
	type |= 0x08;
	mp_min = min;
	mp_max = max;
}

class NPCOption{
	//id in the option list (unique, starting at 0)
	int id;
	
	//index for the next NPCMessage
	int next_message;
	
	//actual message string
	std::string str;
	
	//condition for this option to appear
	NPCCondition condition;
};

class NPCAction{
	NPCAction();
	
	void give_item(int id, int num);
	void change_hp(int n);
	void change_mp(int n);
	void add_exp(int n);
	void change_map(int n);
	void change_data(int id, int val);
	
	//0x01 for give_item, 0x02 for change_hp, 0x04 for change_mp, 0x08 for
	//add_exp, 0x10 for change_map, 0x20 for change_data
	int type
	
	//id and quantity of item
	int item_id;
	int item_num;
	int hp;
	int mp;
	int exp;
	int map_id;
	int data_id;
	int data_val;

class NPCMessage{
	int id;
	
	//0x00 for basic (button shown for NEXT dialogue)
	//0x01 for option list
	int type;
	
	//string to be displayed
	int next_message;
	std::string message;
	
	//alternate message id index to jump to, in case the player doesn't
	//meet the conditions defined in condition
	int alt_message;
	NPCCondition condition;
	NPCAction action;
	
	//optional options
	std::vector<NPCOption> options;
};

class NPCDialogue{
public:
	NPCDialogue(Player *p);
	
	//loads the NPC dialogue from file, but performs no evaluations
	//state_ will begin at 0.
	void load_file(const char *filename);
	
	//call this once before displaying the message. It will check NPC
	//conditions, actions and move to the appropriate message if needed.
	void evaluate_message();
	//shows the current message and available options(if available)
	void display_message();
	//advances the dialogue. If it is a basic dialogue box, option will be
	//ignored
	void enter_data(int option);
	
private:
	//state_ is the index of messages_ that we are at. Always start at 0
	int state_;
	Player *player_;
	std::map<int, NPCMessage> messages_;
};

