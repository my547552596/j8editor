#include "j8editor.h"

int toGetIndex(char *str1, char *str2) {
    char *pos = str1;
    int index = 0;

    if(pos = strstr(str1, str2)) {
        while(str1 != pos) {
            str1++;
            index++;
        }
    } else {
        index = -1;
    }

    return index;
}

char *toGetStrFromInt(int iNum) {
	itoa(iNum, cIntToChar, 10);
	return cIntToChar;
}