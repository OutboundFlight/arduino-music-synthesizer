#include "main.h"

int main(int argc, const char * argv[])
{

    int NumOfLines, NumOfComments, NumOfNotes;			//tracking variables from read files
    int MonoStereo = FILE_TYPE_MONO;				//default is Mono - files may contain STEREO flag
	char filename [100];

	if (argc == 1)
		PromptUser("Song_Twinkle.txt", filename);
	else strcpy (filename, argv[1]);

    NumOfLines = CheckFile(filename, &NumOfLines, &NumOfComments);
    note_t Channel1[NumOfLines];
    note_t Channel2[NumOfLines];

    int Error, i, j;


    if (NumOfLines < 0)
        printf("\n There was an error checking the file!");
    else
    {
        printf("\n The file has %d lines.", NumOfLines);
        printf("\n The file has %d comments.\n", NumOfComments);
    }

    // frequency, duration in seconds
    
    NumOfNotes = ReadFile(filename, Channel1, Channel2, &MonoStereo);	//ReadFile stores the actual notes in Channel1 and Channel2
    if (NumOfNotes == -1)
        printf("There was an error");

	note_event_t * TimingMap = NULL;							//initialize the head for TimingMap;

	if (MonoStereo == FILE_TYPE_MONO)
		TimingMap = CreateMonoTimingMap(Channel1, NumOfNotes);	//create either a mono or stereo timing map based on file flags
	else if (MonoStereo == FILE_TYPE_STEREO)
		TimingMap = CreateStereoTimingMap(Channel1, Channel2, NumOfNotes);

    // Now that we have the song information and have converted it to
    // notes and duration, will initialize the serial port and then
    // send the data to the Arduino.
    int fd;
    int baud = 9600;						//set up variables for USB connection
    char serialport[] = "/dev/ttyACM6";		//right now we don't have a better way to get a virtual USB than to hard-code the name
    uint8_t bytewise[4];
    uint8_t DebuggingOutput[4096];
    int DebuggingCounter = 0;

	note_event_t * p = TimingMap;

//	while (p != NULL)
//	{
//		printf("Freq: %f Time: %d Chan: %d \n", p->freq, p->time, p->channel);
//		p = p->next;
//	}


    fd = serialport_init(serialport, baud);		//initialize the serial port
    Error = serialport_flush(fd);
    if (Error == -1)
        printf("Error flushing port");

	while(p != NULL)
	{
		float2byte(p->freq, bytewise, 0);		//convert frequency to bytes

		printf("Writing: Freq %f Time %d %s\n", p->freq, p->time, (p->channel == CH_PRIMARY)?"Primary Channel":"Secondary Channel");

        for ( j = 0; j <= 3; j++)				//send each byte
        {
            Error = serialport_writebyte(fd, bytewise[j]);
            if (Error == -1)
                printf("Error writing byte");
            tcdrain(fd);
            // For Debugging, store all bytes in array

            #if DEBUG_PRINT
		    DebuggingOutput[DebuggingCounter] = bytewise[j];
            DebuggingCounter++;   
            #endif 
	    //
        }

		uint2byte(p->time, bytewise, 0);		//convert event time (uint) to bytes

        for (j = 0; j <= 3; j++)				//send each byte
        {
            Error = serialport_writebyte(fd, bytewise[j]);
            if (Error == -1)
                printf("Error writing byte");
            tcdrain(fd);
            // For Debugging, store all bytes in array
            #if DEBUG_PRINT
		    DebuggingOutput[DebuggingCounter] = bytewise[j];
            DebuggingCounter++;
		    #endif   
            //
        }

		usint2byte(p->channel, bytewise, 0);				//convert channel to bytes (unsigned short int)
	
		for (j = 0; j <= 1; j++)							//send each byte
		{

			Error = serialport_writebyte(fd, bytewise[j]);
			if (Error == -1)
				printf("Error writing byte");
			tcdrain(fd);
			//Store all bytes in debugging array
			#if DEBUG_PRINT
			DebuggingOutput[DebuggingCounter] = bytewise[j];
			DebuggingCounter++;
			#endif
		}

        Error = serialport_writebyte(fd, (uint8_t)'\0');		//send a null to pad the bytes
		if (Error == -1)
            printf("Error writing byte");
        tcdrain(fd);
        // For Debugging, store all bytes in array
	    #if DEBUG_PRINT
        DebuggingOutput[DebuggingCounter] = (uint8_t)'\0';
        DebuggingCounter++;   
        #endif
	    //

		p = p->next;

    }

	for (j = 0; j < 4; j++)								//send a word (ten bytes) full of 0xFF and then a null terminator
	{													//this is our "song complete" signal
		for (i = 0; i < 10; i++)
			Error = serialport_writebyte(fd, 0xff);
		Error = serialport_writebyte(fd, (uint8_t)'\0');
	}


    // Run tcdrain and then sleep for 2 ms to be sure all data has been sent
    tcdrain(fd);
    usleep(2000);

    Error = serialport_writebyte(fd, (uint8_t)'\n');
    // For Debugging, store all bytes in array
    #if DEBUG_PRINT
    DebuggingOutput[DebuggingCounter] = (uint8_t)'\n';
    DebuggingCounter++;   
    #endif
    //


    // Run tcdrain and then sleep for 2 ms to be sure all data has been sent
    tcdrain(fd);
    usleep(2000);

    // Flush serial port, then close
    Error = serialport_flush(fd);
    Error = serialport_close(fd);

    // For Debugging, iterate through each char and print hex to screen
    // Then compare sniffed serial data to see where it differs
    #if DEBUG_PRINT
    printf("\n\n\nBeginning to print sent bytes to screen: \n\n");
    int NewLineCtr = 0;
    for (int pos = 0; pos < DebuggingCounter; pos++)
    {
        printf("%2x ", DebuggingOutput[pos]);        
        NewLineCtr++;
        if (NewLineCtr == 11)
	{
	    printf("\n");
	    NewLineCtr = 0;  
	}
    }
    printf("\n\n\n");
    #endif



    return 1;
}
