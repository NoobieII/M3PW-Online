#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h> //for usleep
#include <linux/input.h>

//credit to Youarefunny, from Stack Exchange, for providing this
//working code!

void usage ( int argc, char *argv[] )
{
    printf("Usage:\n\t%s key\n\nvalid keys are:\n\tlshift\t- Left Shift key\n" , argv[0]);

    exit(EXIT_FAILURE);
}

int main ( int argc, char *argv[], char *env[] )
{
    

    int key;
    
    printf("Enter what key you will check (see Linux input codes): ");
    scanf("%d", &key);
    //key = KEY_LEFTSHIFT;
    
    
    //give time to press the key to test
    usleep(1000000);
    
    //this file contains the state of the keyboard. Open it each time the keyboard
    //is read...
    FILE *kbd = fopen("/dev/input/by-path/platform-i8042-serio-0-event-kbd", "r");
    
    if(!kbd){
        printf("Could not open file for keyboard state. Open with root privileges.\n");
        return 0;
    }
    
    char key_map[KEY_MAX/8 + 1];    //  Create a byte array the size of the number of keys

    memset(key_map, 0, sizeof(key_map));    //  Initate the array to zero's
    ioctl(fileno(kbd), EVIOCGKEY(sizeof(key_map)), key_map);    //  Fill the keymap with the current keyboard state

    int keyb = key_map[key/8];  //  The key we want (and the seven others arround it)
    int mask = 1 << (key % 8);  //  Put a one in the same column as out key state will be in;
    
    
    if(keyb & mask){
	printf("Key %d is pressed\n", key);
    }
    else{
	printf("Key %d is not pressed\n", key);
    }
	
    return !(keyb & mask);  //  Returns true if pressed otherwise false

}

