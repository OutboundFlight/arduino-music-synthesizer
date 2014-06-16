#ifndef FILEIO_DEF
#define FILEIO_DEF

int PromptUser(char * DefaultFileName, char * ChosenFileName);
int CheckFile(char * FileToCheck, int * Lines, int * Comments);
int ReadFile(char * FileToCheck, note_t C1[], note_t C2[], int *MonoStereo);
int RemoveNewLine(char * StrToFix, int StrLen);

#endif

