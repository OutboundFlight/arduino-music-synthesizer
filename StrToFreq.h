#ifndef STR_TO_FREQ
#define STR_TO_FREQ

#define NORMAL_OCTAVE_C '4'
#define BEATS_PER_MINUTE 60
#define SECONDS_PER_MINUTE 60

float note_table[14];
float tempo = 200;

int StrToFreq(const char *, note_t*);

#endif

