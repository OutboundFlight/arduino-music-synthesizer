//  Version:        0.01 - Initial Version
//  Date/Time:      May 24, 2014    1:52pm
//  Release Notes:
//      None - Initial Version
//
//
/******************************************************************
 int PromptUser(char * DefaultFileName, char * ChosenFileName);

 PromptUser - This function prints a welcome message and
 then prompts the user to enter the name of a file to read. It will
 also display the default file, which is provided as an input to
 the function.
 A usage example would be:

 int Error;
 char FileName[256];
 Error = PromptUser("Song_Twinkle.txt", FileName);
 if (Error != 0)
 printf("%s is the chosen file!\n",FileName);
 else
 printf("The Function returned with error: %d",Error);

 ******************************************************************/

int PromptUser(char * DefaultFileName, char * ChosenFileName)
{

    //    Creating character array to store user input. Will assume
    // no input will be greater than 256 characters.
    char UserInputStr[256];

    // Print welcome screen and prompt the user for a file name.
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
           "**************************************************\n"
           "*          Welcome to the Music Player           *\n"
           "**************************************************\n"
           " Please enter the name of the file to play. The   \n"
           " default file is '%s'.                            \n"
           " FILENAME:  ", DefaultFileName);
    scanf("%s", UserInputStr);


    //   Do very brief string validation, then copy value to
    // ChosenFileName to be used in returning function.
    // Will most likely implement further input validation
    if (UserInputStr != NULL)
    {
        strcpy(ChosenFileName,UserInputStr);
        return 1;
    }
    else
    {
        return 0;
    }

}

