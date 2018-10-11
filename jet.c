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

BOOL toBeUtf8WithoutBom(char *cString) {
    int iBegin = 0, iEnd = strlen(cString);

    while(iBegin < iEnd) {
        int iStep = 0;

        if((cString[iBegin] & 0x80) == 0x00) {
            iStep = 1;
        } else if((cString[iBegin] & 0xE0) == 0xC0) {
            if(iBegin + 1 >= iEnd) {
                return FALSE;
            }
            if((cString[iBegin + 1] & 0xC0) != 0x80) {
                return FALSE;
            }
            iStep = 2;
        } else if((cString[iBegin] & 0xF0) == 0xE0) {
            if(iBegin + 2 >= iEnd) {
                return FALSE;
            }
            if((cString[iBegin + 1] & 0xC0) != 0x80) {
                return FALSE;
            }
            if((cString[iBegin + 2] & 0xC0) != 0x80) {
                return FALSE;
            }
            iStep = 3;
        } else {
            return FALSE;
        }

        iBegin += iStep;
    }

    if(iBegin == iEnd) {
        return TRUE;
    }

    return FALSE;
}

char *toTrimAll(char *cString) {
    char *cTemp = toTrimLeft(cString);
    return toTrimRight(cTemp);
}

char *toTrimLeft(char *cString) {
    while(isspace(*cString)) {
        cString++;
    }

    return cString;
}

char *toTrimRight(char *cString) {
    char *cEnd = cString + strlen(cString) - 1;
    while(cEnd > cString && isspace(*cEnd)) {
        *cEnd = '\0';
        cEnd--;
    }

    return cString;
}