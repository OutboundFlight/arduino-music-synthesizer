//  Version:        0.01 - Initial Version
//  Date/Time:      May 24, 2014    2:56pm
//  Release Notes:
//      None - Initial Version
//
//
/* *****************************************************************
 int ReadFile(char * FileToCheck, double * NFreq, double * NDur, int MonoOrStereo);

 ReadFile -


 A usage example would be:




 ***************************************************************** */

//MonoStereo is either FILE_TYPE_MONO for mono or FILE_TYPE_STEREO for stereo

int ReadFile(char * FileToCheck, note_t C1[], note_t C2[], int *MonoStereo)
{

    FILE *fp; // This is the file point to the file we are checking
    char str[256];  // Temporary storage string to perform the checking
    int Error;

    //   Open file with read access. If file is opened successfully,
    // continue with function. Otherwise, return 0.
    fp = fopen(FileToCheck, "r");
    if (fp==NULL)
        return 0;

	*MonoStereo = FILE_TYPE_MONO;

    int k = 0;
    while (fgets(str,256,fp) != NULL)
    {

		//tries to find a tag "TEMPO" in the file. If it does, it'll read the float given. If it doesn't, 
		//it'll use the default value (100) from StrToFreq.c If it finds multiple it will use the last one.
		char temp[256];
		strtrim(temp, str, 1);

		if (!strcmp(temp, ""))
			continue;

		if (strstr(str, "TEMPO")!= NULL)
		{
			sscanf(str, " TEMPO %f ", &tempo);		
			continue;
		}

		if (strstr(str, "STEREO")!=NULL)
		{
			*MonoStereo = FILE_TYPE_STEREO;
			continue;
		}
		else if (strstr(str, "MONO")!=NULL)
		{
			*MonoStereo = FILE_TYPE_MONO;		
			continue;
		}

        if (str[0]!='%')
        {
            // This removes characters that are not alpha/numberic/symbol


            RemoveNewLine(str, strlen(str));


            Error = StrToFreq(str, &C1[k]);
            if (Error == -1)
            {
                printf("\nThere was an error returning from StrToFreq!\n");
		
            }

			if (*MonoStereo == FILE_TYPE_STEREO)
			{
				Error = StrToFreq(strchr(str, ')') + 1, &C2[k]);	
				if (Error == -1)
				{
					printf("\nThere was an error returning from StrToFreq for the stereo channel!\n");
				}
			}
			k++;
		//	printf("In ReadFile: Freq %f Dur %f Chan 1\n", C1->freq, C1->dur);
		//	printf("In ReadFile: Freq %f Dur %f Chan 2\n", C2->freq, C2->dur);
		
        }
    }

	fclose(fp);

    return k;
}























//  Version:        0.02
//  Date/Time:      May 25, 2014    11:53am
//  Release Notes:
//      Added ByReference arguments Lines and Comments as a way to
// get more valuable information from this function and to make the
// ReadFile function easier.
//
/******************************************************************
 int CheckFile(char * FileToCheck, int * Lines, int * Comments);

 CheckFile - This function checks a song file for validity and then
 returns the number of lines contained in the file.
 It also passes the pointers to Lines, Comments and assigns the
 Total number of lines to Lines and the number of comments to
 Comments.

 A usage example would be:

 int NumOfLines, NumOfComments;
 char FileName[] = "Song_Twinkle.txt";

 NumOfLines = CheckFile(FileName, &NumOfLines, &NumOfComments);
 if (NumOfLines < 0)
 printf("\n There was an error checking the file!");
 else
 {
 printf("\n The file has %d lines.",NumOfLines);
 printf("\n The file has %d comments.\n",NumOfComments);
 }

 ******************************************************************/

int CheckFile(char * FileToCheck, int * Lines, int * Comments)
{

    FILE *fp; // This is the file point to the file we are checking
    char str[256];  // Temporary storage string to perform the checking

    //   Open file with read access. If file is opened successfully,
    // continue with function. Otherwise, return 0.
    fp = fopen(FileToCheck, "r");
    *Lines = -1; // File Unavailable to Open. If successful, this value will be replaced.
    if (fp==NULL)
        return -1;

    int k = 0; // k keeps track of the TOTAL number of lines in the file.
    int c = 0; // c keeps track of comment lines (start with %)
    while (fgets(str,256,fp) != NULL)
    {
        k++;
        if (strchr(str,'%')!= NULL)
			c++;	
        //  ***  Will add basic string checking functionality here ***

    }

    // Set calculated values in passed structure, then return k.
    *Lines = k;
    *Comments = c;
    return k;
}










/******************************************************************************

RemoveNewLine - This function removes newline characters and replaces with NULL




*******************************************************************************/
int RemoveNewLine(char * StrToFix, int StrLen)
{

    for (int i = 0; i < StrLen; i++)
    {
        // Originally removed NewLine characters exclusively, but modified it to
        // replace any character that's not a number, letter, or symbol with NULL
        if ((StrToFix[i] < 32) || (StrToFix[i] > 126))
            StrToFix[i] = 0;
    }

    return 0;
}
