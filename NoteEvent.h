#ifndef NOTE_EVENT_H
#define NOTE_EVENT_H

#define CH_PRIMARY 1
#define CH_SECONDARY 2
#define MS_PER_SEC 1000

typedef struct note_t
{
	float freq;
	float dur;
}note_t;

typedef struct note_event_t 
{
	float freq;
	unsigned int time;
	short int channel;
	struct note_event_t *next;
} note_event_t;

note_event_t * CreateStereoTimingMap(const note_t * C1, const note_t * C2, int buff_size);

note_event_t * CreateMonoTimingMap(const note_t * Chan, int buff_size);


#endif
