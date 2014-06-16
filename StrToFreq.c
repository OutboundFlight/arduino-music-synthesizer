/*
//  Version:        0.02 - First patch
//  Date/Time:      May 27, 2014    11:08pm
//  Release Notes:
//      0.02 - Fixed output bugs and added float compatibility for reading of duration
//
//
******************************************************************

This function interprets a single, reasonably well-formatted string containing a "note"
in the format NmO(D) where N is the note (A-G), m is an optional modulus (# or b), O
is the octave, and D is the duration in beats. The function calculates the appropriate freq
using a lookup table and simple modulation, stores it in the address passed as *freq, and
calcuates the duration of the note in seconds based on the tempo read from the original file.

****************************************************************** */

    float note_table[14] = {261.625565301, 277.182630977, 293.664767917, 311.126983722, 329.627556913, 349.228231433, 369.994422712, 391.995435982, 415.30469758, 440.00000000, 466.163761518, 493.883301256, 523.251130601, 0.00000000};

int StrToFreq (const char *line, note_t *Note)
{
    char temp;
    char note;			//note is the actual letter note w/o sharp or flat
    int halfstep;		//halfstep is the offset of the note in half steps from A, i.e. A is 0, A# is 1, B is 2, C is 3...
    int oct, modulus;	//oct is the octave, modulus is 1 for sharp, 0 for natural, -1 for flat
    int octshift = 0;   //octshift is set to a value other than zero only if the note is a Cb in which case we need to drop down an octave
    float beats = 0;	//beats is the value of the number of beats to hold the note for

	sscanf(line, " %c  %c ", &note, &temp);  

	if (temp == '#' || temp == 'b')
	{
		sscanf(line, " %c %c %d ( %f ) ", &note, &temp, &oct, &beats);
	//	printf("Note: %c Mod: %c Oct: %d Beats: %f\n", note, temp, oct, beats);
		switch (temp)
		{
			case '#':
				modulus = 1;
				break;
			case 'b':
				modulus = -1;
				break;
			otherwise:
				modulus = 0;
		}
	}
	else if (temp >= '2' && temp <= '5')
	{
		sscanf(line, " %c %d ( %f ) ", &note, &oct, &beats);
	//	printf("Note: %c Oct: %d Beats: %f\n", note, oct, beats);
		modulus = 0;
	}
	else return -1;

	if (oct < 2 || oct > 5)
		return -1;

	note = toupper(note);

   	//printf("modulus = %d\nnote = %c\n", modulus, note);

    switch (note)
    {
    case 'C':
        if (modulus == -1)
        {
            halfstep = 12 + modulus;
            octshift = -1;
        }
        else
            halfstep = 0 + modulus;
        break;
    case 'D':
        halfstep = 2 + modulus;
        break;
    case 'E':
        halfstep = 4 + modulus;
        break;
    case 'F':
        halfstep = 5 + modulus;
        break;
    case 'G':
        halfstep = 7 + modulus;
        break;
    case 'A':
        halfstep = 9 + modulus;
        break;
    case 'B':
        halfstep = 11 + modulus;
        break;
    case 'Z':
		halfstep = 13;
		break;
    }

//	printf("halfstep = %d", halfstep);

    oct += -4 + octshift;

//	printf("oct = %d", oct);

   Note->freq = note_table[halfstep] * pow(2, oct);					//the frequency value is the note value times 2 to the power of octave
                                                                    //so one octave above 4 will be the base frequency*2 and below, *1/2

//	printf("freq = %lf\n", *freq);

   Note->dur = beats * SECONDS_PER_MINUTE / tempo;

//	printf("Freq: %f Dur: %f\n", Note->freq, Note->dur);
    return 0;
}

//dummy main for testing


/*

int main(int argc, char *argv[])
{
	float freq, duration;
	int error;
//	char string[] = "A4(1)";

	error = StrToFreq(argv[1], &freq, &duration);
	printf("String value: %s\nError code: %d\nFrequency: %lf (Hz)\nDuration: %lf(s)\n", argv[1], error, freq, duration);

	return 0;
}

*/
