#include <stdlib.h>
#include <stdio.h>

#include <linux/input.h>

void usage ( int argc, char *argv[] )
{
    printf("Usage:\n\t%s key\n\nvalid keys are:\n\tlshift\t- Left Shift key\n" , argv[0]);

    exit(EXIT_FAILURE);
}

int main ( int argc, char *argv[], char *env[] )
{
    if ( argc != 2 )    usage(argc, argv);

    int key;

    if ( strcmp(argv[1], "lshift") == 0 )       key = KEY_LEFTSHIFT;
    else if ( strcmp(argv[1], "rshift") == 0 )  key = KEY_RIGHTSHIFT;
    else if ( strcmp(argv[1], "lalt") == 0 )    key = KEY_LEFTALT;
    else if ( strcmp(argv[1], "ralt") == 0 )    key = KEY_RIGHTALT;
    else if ( strcmp(argv[1], "lctrl") == 0 )   key = KEY_LEFTCTRL;
    else if ( strcmp(argv[1], "rctrl") == 0 )   key = KEY_RIGHTCTRL;


    FILE *kbd = fopen("/dev/input/by-path/platform-i8042-serio-0-event-kbd", "r");

    char key_map[KEY_MAX/8 + 1];    //  Create a byte array the size of the number of keys

    memset(key_map, 0, sizeof(key_map));    //  Initate the array to zero's
    ioctl(fileno(kbd), EVIOCGKEY(sizeof(key_map)), key_map);    //  Fill the keymap with the current keyboard state

    int keyb = key_map[key/8];  //  The key we want (and the seven others arround it)
    int mask = 1 << (key % 8);  //  Put a one in the same column as out key state will be in;
    
    printf("key = %d, KEY_MAX = %d\n", KEY_A, KEY_MAX);
    int i;
    if(!(keyb & mask)){
	printf("LSHIFT is pressed");
    }
    else{
	printf("LSHIFT is not pressed");
    }
	
    return !(keyb & mask);  //  Returns true if pressed otherwise false

}

