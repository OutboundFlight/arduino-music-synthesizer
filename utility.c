//  Version:        0.01 - Initial Version
//  Date/Time:      May 24, 2014    4:02pm
//  Release Notes:
//      None - Initial Version
//
//
/* *****************************************************************

strtrim trims extra whitespace from a standard null-terminated
c-string "input" and stores it in "target".If nospace is zero, it trims
leading and trailing space only. If nospace is one, it scrubs all
whitespace from the string.

float2byte takes a floating point value and stores it bytewise in an
array of four fixed-width 8-bit unsigned ints (uint8_t). It is stored
big-endian.

****************************************************************** */

void strtrim(char *target, const char *input, int nospace)
{
    char *end	= (char *)input; 			//Pointer to the location of first \0 in the original input
    char *p		= NULL;						//Current working pointer
    char *q		= NULL;						//Extra

    for ( ; *end != '\0'; end++);

//	printf("%s ends at %d\n", input, (int)(end - input));

    for (p = (char *)input; p < end ; p++)			//check simple corner cases; this tests for both null string and all spaces
        if (!isspace(*p))							//that would otherwise be missed below
            break;


    if (p == end)									//if we can iterate all the way through the string and not find a character other than the
    {
        //null terminator it either means the string is already empty or it's just whitespace
        strcpy(target, "");
        return;
    }

    if(nospace)										//if you want to destroy all whitespace
    {
        for (p = (char *)input; p <= end; p++)
            if (!isspace(*p))						//only store the current location in target if it's not a whitespace character
                *target++ = *p;						//if it is non-whitespace also increment our pointer to target
        return;
    }

    else											//if you just want to eat leading and trailing whitespace
    {
        for(p = end; p > (char *)input; p--)		//loop from the end until you find a non-whitespace character
            if (!isspace(*p))						//p will have the memory location of the last non-whitespace character in input
                break;

        for(q = (char *)input; q < end; q++)		//loop from the beginning until you find a non-whitespace character
            if (!isspace(*q))						//q will have the memory location of the first non-whitespace character
                break;

        strncpy(target, q, (int)(p-q));					//copies text starting at q for p-q characters, aka until p
        target[(int)(p - q)] = '\0';					//adds a null terminator
        return;
    }
}

union ufloat
{
    float f;
    uint8_t byte[4];
};

union uusint
{
	unsigned short int i;
	uint8_t byte[2];
};

union uuint
{
	unsigned int i;
	uint8_t byte[4];
};


/* This function copies the unsigned eight-bit values of float fl into array. if bigendian is 1 it will return the values in bigendian order
(i.e. MSB in register 0, LSB in register 3); otherwise it will return the values in littleendian order (LSB in 0, MSB in 3).
*/
void float2byte(float fl, uint8_t b[], int bigendian)
{
    union ufloat temp;
    temp.f = fl;
    if(((__BYTE_ORDER == __LITTLE_ENDIAN) && !(bigendian)) || ((__BYTE_ORDER == __LITTLE_ENDIAN) && (bigendian)))
    {
        b[0] = temp.byte[0];
        b[1] = temp.byte[1];
        b[2] = temp.byte[2];
        b[3] = temp.byte[3];
    }
    else
    {
        b[0] = temp.byte[3];
        b[1] = temp.byte[2];
        b[2] = temp.byte[1];
        b[3] = temp.byte[0];
    }

}

void uint2byte(unsigned int n, uint8_t b[4], int bigendian)
{
    union uuint temp;
    temp.i = n;
    if(((__BYTE_ORDER == __LITTLE_ENDIAN) && !(bigendian)) || ((__BYTE_ORDER == __LITTLE_ENDIAN) && (bigendian)))
    {
        b[0] = temp.byte[0];
        b[1] = temp.byte[1];
        b[2] = temp.byte[2];
        b[3] = temp.byte[3];
    }
    else
    {
        b[0] = temp.byte[3];
        b[1] = temp.byte[2];
        b[2] = temp.byte[1];
        b[3] = temp.byte[0];
    }
}

void usint2byte(unsigned short int n, uint8_t b[2], int bigendian)
{
	union uusint temp;
    temp.i = n;
    if(((__BYTE_ORDER == __LITTLE_ENDIAN) && !(bigendian)) || ((__BYTE_ORDER == __LITTLE_ENDIAN) && (bigendian)))
    {
        b[0] = temp.byte[0];
        b[1] = temp.byte[1];
    }
    else
    {
        b[0] = temp.byte[1];
        b[1] = temp.byte[0];
    }
}
