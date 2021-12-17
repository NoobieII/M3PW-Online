#include <stdio.h>
#include <Windows.h>

int main(){
	int n, n1, n2;
	INPUT_RECORD events[100];
	DWORD num_events;
	HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
	if(input_handle == INVALID_HANDLE_VALUE){
		printf("Could not get input handle.\n");
		return -1;
	}
	
	//read (without blocking) the console input buffer
	//PeekConsoleInput(input_handle, events, 100, &num_events);
	//FlushConsoleInputBuffer(input_handle);
	
	//get the state of the left shift
	for(n = 0; n < 100; ++n){
		n1 = (GetKeyState(VK_LSHIFT) & 0x80) >> 7;
		n2 = (GetKeyState(VK_RSHIFT) & 0x80) >> 7;
		printf("\rState of lshift = 0x%08x, rshift = 0x%08x", n1, n2);
		Sleep(100);
	}
	//get the state of the left shift
	for(n = 0; n < 100; ++n){
		n1 = (GetKeyState(VK_LMENU) & 0x80) >> 7;
		n2 = (GetKeyState(VK_RMENU) & 0x80) >> 7;
		printf("\rState of lmenu = 0x%08x, rmenu = 0x%08x", n1, n2);
		Sleep(100);
	}
	
	return 0;
}
