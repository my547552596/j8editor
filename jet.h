char cBuffer8[8], cBuffer16[16], cBuffer32[32], cBuffer64[64], cBuffer512[512];

BOOL toBeUtf8WithoutBom(char *cString);
char *toGetFileName(char *cPath);
int toGetIndexOfString(char *str1, char *str2);
char *toGetStringFromInteger(int iNum);
char *toTrimAll(char *cString);
char *toTrimLeft(char *cString);
char *toTrimRight(char *cString);