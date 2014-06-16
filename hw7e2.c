#include <stdio.h>
#include <stdlib.h>

#define CH_PRIMARY 1
#define CH_SECONDARY 2
#define MS_PER_SEC 1000
#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))

typedef struct note_t
{
	float freq;
	float dur;
} note_t;

struct note_event_t
{
	float freq;
	unsigned int time;
	int channel;
	struct note_event_t * next;
};

typedef struct note_event_t note_event_t;

note_event_t * CreateTimingMap (const note_t *C1, const note_t *C2, int buff_size)
{
	int C1count = 0, C2count = 0;

	int C1end = 0, C2end = 0;

	float C1NextEvent = 0, C2NextEvent = 0;

	note_event_t *node = malloc(sizeof(note_event_t));		

	note_event_t *TimingMap = node;
	
	while(1)
	{
		printf("before if flag C1 count%d freq %f\n", C1count, C1->freq);
//		printf("C1NextEvent: %f C2NextEvent: %f\n", C1NextEvent, C2NextEvent);
		if ((C1NextEvent <= C2NextEvent || C2end) && !C1end)
		{
//			printf("passed C1 if");
			if (C1->freq == 0 && C1->dur == 0)
			{
				printf("Inner loop C1 count %d freq %f\n", C1count, C1->freq);
				C1++;
				C1count++;
				SkippedNode = 1;
			}
			else
			{
				printf("after if flagC1 count%d\n freq %f", C1count, C1->freq);
				node->freq = C1->freq;
				node->channel = CH_PRIMARY;
				node->time = C1NextEvent;

				C1NextEvent += C1->dur * MS_PER_SEC;
				C1++;
				C1count++;
			}
		}
		else if ((C2NextEvent < C1NextEvent || C1end) && !C2end)
		{
//			printf("passed C2 if");
			if(C2->freq == 0 && C2->dur == 0)
			{
				printf("inner loop C2\n");
				C2++;
				C2count++;
				SkippedNode = 1;
			}
			else
			{
				node->freq = C2->freq;
				node->channel = CH_SECONDARY;
				node->time = C2NextEvent;
			
				C2NextEvent += C2->dur * MS_PER_SEC;
				C2++;
				C2count++;
			}
		}
		else printf("Did not enter either loop.\n");

//		printf("Check");
		printf("Current freq: %f time: %d channel: %s\n", node->freq, node->time, (node->channel == CH_PRIMARY)?"Primary":"Secondary");

		if(C1count >= buff_size && !C1end)
		{
			C1end = 1;
//			printf("C1end\n");
		}

		if(C2count >= buff_size && !C2end)
		{
			C2end = 1;
//			printf("C2end\n");
		}

		if (C1end && C2end)	
		{

//			printf("Returning...\n");

//			note_event_t *p = TimingMap;

//			while (p->next != NULL)
//			{
//				printf("Freq: %f", p->freq);
//				p = p->next;
//			} 
	

			return TimingMap;
		}


		if (!SkippedNode)
		{
//			printf("New loop update\n");
//			printf("Before: node: %d node->next: %d\n", (int)node, (int)(node->next));

			node->next = malloc(sizeof(note_event_t));
		
//			printf("Step 1: node: %d node->next: %d\n", (int)node, (int)(node->next));
		
			node = node->next;

//			printf("Step 2: node: %d node->next: %d\n", (int)node, (int)(node->next));
	
			node->next = NULL;

//			printf("After: node: %d node->next: %d\n", (int)node, (int)node->next);
		} else SkippedNode = 0;
	}

	return NULL;
}


int main(void)
{
	note_t C1[5];
	note_t C2[5];

	C1[0].freq = 440;
	C1[0].dur = 1;
	C2[0].freq = 440;
	C2[0].dur = 2.1;
	C1[1].freq = 500;
	C1[1].dur = 1;
	C2[1].freq = 440;
	C2[1].dur = 1.4;
	C1[2].freq = 800;
	C1[2].dur = 5;
	C2[2].freq = 445;
	C2[2].dur = 0.1;
	C1[3].freq = 770;
	C1[3].dur = 8;
	C2[3].freq = 100;
	C2[3].dur = 20;
	C1[4].freq = 0;
	C1[4].dur = 0;
	C2[4].freq = 12;
	C2[4].dur = 1;
	note_event_t *TimingMap;
	TimingMap = CreateTimingMap(C1, C2, 5);	

	note_event_t *p = TimingMap;

	while(p != NULL)
	{
		printf("Freq: %f Time: %d Channel: %s\n", p->freq, p->time, (p->channel == CH_PRIMARY)?"Primary":"Secondary");
		p = p->next;
	}
} 
