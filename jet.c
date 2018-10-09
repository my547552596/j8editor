#include "j8editor.h"

char *toGetFileName(char *cPath) {
	_splitpath(cPath, NULL, NULL, cBuffer64, cBuffer16);

	if(strlen(cBuffer16)) {
		strcat(cBuffer64, ".");
		strcat(cBuffer64, cBuffer16);
	}

	return cBuffer64;
}

int toGetIndexOfString(char *str1, char *str2) {
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

char *toGetStringFromInteger(int iNum) {
	itoa(iNum, cBuffer16, 10);
	return cBuffer16;
}