#ifndef UTILITY
#define UTILITY

union unote;
union usint;
union uusint;

void strtrim(char *target, const char *input, int nospace);

void float2byte(float fl, uint8_t b[4], int bigendian);
void uint2byte(unsigned int n, uint8_t b[4], int bigendian);
void usint2byte(unsigned short int n, uint8_t b[2], int bigendian);

#endif

