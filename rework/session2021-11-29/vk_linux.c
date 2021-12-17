//Lithio (the developer's pseudonym)
//December 01, 2021

//Source file for Linux to map input event codes to virtual key codes

#include "vk_linux.h"

char linux_to_vk[256] = 
{
	0, VK_ESCAPE, '1', '2', '3', '4', '5', '6',//0x00~0x07
	'7', '8', '9', '0', VK_OEM_MINUS, VK_OEM_PLUS, VK_BACK, VK_TAB, //0x08~0x1f
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',//0x10~0x17
	'O', 'P', VK_OEM_4, VK_OEM_6, VK_RETURN, VK_CONTROL, 'A', 'S',//0x18~0x1f
	'D', 'F', 'G', 'H', 'J', 'K', 'L', VK_OEM_1,//0x20~0x27
	VK_OEM_7, VK_OEM_3, VK_SHIFT, VK_OEM_5, 'Z', 'X', 'C', 'V',//0x28~0x2f
	'B', 'N', 'M', VK_OEM_COMMA, VK_OEM_PERIOD, VK_OEM_2, VK_SHIFT, VK_MULTIPLY,//0x30~0x37
	VK_MENU, VK_SPACE, VK_CAPITAL, VK_F1, VK_F2, VK_F3, VK_F4, VK_F5,//0x38~0x3f
	VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_NUMLOCK, VK_SCROLL, VK_NUMPAD7,//0x40~0x47
	VK_NUMPAD8, VK_NUMPAD9, VK_SUBTRACT, VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6, VK_ADD, VK_NUMPAD6,//0x48~0x4f
	VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD0, VK_DECIMAL, 0, 0, 0, VK_F11,//0x50~0x57
	VK_F12, 0, 0, 0, 0, 0, 0, 0,//0x58~0x5f
	0, VK_CONTROL, VK_DIVIDE, VK_SNAPSHOT, VK_MENU, 0, VK_HOME, VK_UP,//0x60~0x67
	VK_PRIOR, VK_LEFT, VK_RIGHT, VK_END, VK_DOWN, VK_NEXT, VK_INSERT, VK_DELETE,//0x68~0x6f
	0, VK_VOLUME_MUTE, VK_VOLUME_DOWN, VK_VOLUME_UP, , , , ,//0x70~0x77
	, , , , , , , ,//0x78~0x7f
	, , , , , , , ,//0x80~0x87
	, , , , , , , ,//0x88~0x8f
	, , , , , , , ,
	, , , , , , , ,
}

int get_vk_from_linux_key(int key);
