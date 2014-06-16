

note_event_t * CreateMonoTimingMap (const note_t *Chan, int buff_size)
{
	int Chancount = 0;			//stores the number of notes from our primary channel that we have added to the timing map
	float ChanNextEvent = 0;		//float timing of the next event; although it will be converted to int during storage in time

	note_event_t *node = malloc(sizeof(note_event_t));	//initialize the linked list
	node->next = NULL;

	note_event_t *TimingMap = node;				//TimingMap is the head of the list that we will eventually return

	while(1)
	{
		if (Chan->dur == 0)				//if a note has no duration it's a dummy note
		{
			Chan++;					//skip it
			Chancount++;

			if (Chancount >= buff_size)		//if we've gotten to the end of the list, just end it with that node empty
			{
				node->freq = 0;
				node->time = ChanNextEvent;
				node->channel = CH_PRIMARY;
				node->next = NULL;
				return TimingMap;
			}
		}
		else
		{
			node->freq = Chan->freq;			//if the duration in current Chan isn't empty, transfer freq, 
			node->channel = CH_PRIMARY;			//set channel, and store the current effective time in time 
			node->time = ChanNextEvent;

			ChanNextEvent += Chan->dur * MS_PER_SEC;	//add the duration of the note to ChanNextEvent so we know when the
			Chan++;						//note should finish and the next note being
			Chancount++;


			if (Chancount >= buff_size)			//return if we've traversed the entire buffer
				return TimingMap;
			else
			{
				node->next = malloc(sizeof(note_event_t));	//otherwise init a new node and move to it
				node = node->next;
				node->next = NULL;
			}
		}
	} //end while
	return NULL;
}

note_event_t * CreateStereoTimingMap (const note_t *C1, const note_t *C2, int buff_size)
{
	int C1count = 0, C2count = 0;		//number of notes from C1 and C2 we've added
	int C1end = 0, C2end = 0;		//flag if we've reached the end of C1 or C2 based on the counts
	int SkippedNode = 0;			//flag if we ate this index and don't need to init a new one 

	float C1NextEvent = 0, C2NextEvent = 0;	//timing for each channel - we'll always try to play the channel that's not used unless
						// we've reached the end of the other channel 

	note_event_t *node = malloc(sizeof(note_event_t));	//initialize a new node
	node->next = NULL;	
	note_event_t *TimingMap = node;				//TimingMap is the head of the list which we will eventually return
	
	while(1)
	{
//		printf("C1NextEvent: %f C2NextEvent: %f\n", C1NextEvent, C2NextEvent);
		if ((C1NextEvent <= C2NextEvent || C2end) && !C1end)		//if C1's next event is earlier than C2's, or C2 has ended,
										//and C1 has not ended
		{
//			printf("passed C1 if");
			if (C1->dur == 0)		//if it has zero duration it's a dummy note, skip it and set SkippedNode flag
			{
				C1++;
				C1count++;
				SkippedNode = 1;
			}
			else
			{
				node->freq = C1->freq;			//otherwise, store the note values in the map
				node->channel = CH_PRIMARY;
				node->time = C1NextEvent;		//the note starts at the next event time for C1

				C1NextEvent += C1->dur * MS_PER_SEC;	//add the length of the note to the next event time tracker
				C1++;					//move to the new note
				C1count++;
			}
		}
		else if ((C2NextEvent < C1NextEvent || C1end) && !C2end)	//but if C2 comes first, or C1 has ended, and C2 hasn't
		{
//			printf("passed C2 if");
			if(C2->dur == 0)					//check if it's a dummy note
			{
				C2++;						//if it is, skip it and set SkippedNode
				C2count++;
				SkippedNode = 1;
			}
			else
			{
				node->freq = C2->freq;				//otherwise, store note values in the map
				node->channel = CH_SECONDARY;			//set the channel
				node->time = C2NextEvent;			//set the time as the next event for C2
			
				C2NextEvent += C2->dur * MS_PER_SEC;		//add the length of the note to the channel time
				C2++;						//move to the next note
				C2count++;
			}
		}
//		else printf("Did not enter either loop.\n");

//		printf("Check");
//		printf("Current freq: %f time: %d channel: %s\n", node->freq, node->time, (node->channel == CH_PRIMARY)?"Primary":"Secondary");

		if(C1count >= buff_size && !C1end)	// if we're at the last note for C1, set the flag
		{
			C1end = 1;
//			printf("C1end\n");
		}

		if(C2count >= buff_size && !C2end)	//same with C2
		{
			C2end = 1;
//			printf("C2end\n");
		}

		if (C1end && C2end)	
		{
			if (SkippedNode)			//in case the last note we processed was a skipped note we'll have some
			{					//extra memory lying around. Set the freq to zero and the time to the last
				node->freq = 0;			//time stamp; Arduino will mostly ignore this.
				node->time = (C2NextEvent > C1NextEvent)?C2NextEvent:C1NextEvent;
				node->channel = CH_PRIMARY;
				node->next = NULL;
			}	
			return TimingMap;			//return the map
		}


		if (!SkippedNode)			//if we didn't skip a note this loop, initialize a new node
		{
			node->next = malloc(sizeof(note_event_t));		
			node = node->next;
			node->next = NULL;
		} else SkippedNode = 0;			//if we did skip a note, skip all that initialization and reset the flag.
	}	//end while

	return NULL;
}


//dummy main for testing purposes
/*
int main(void)
{
	note_t C1[5];
	note_t C2[5];

	C1[0].freq = 440;
	C1[0].dur = 1;
	C2[0].freq = 440;
	C2[0].dur = 2.1;
	C1[1].freq = 0;
	C1[1].dur = 0;
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
	TimingMap = CreateStereoTimingMap(C1, C2, 5);	

	note_event_t *p = TimingMap;

	while(p != NULL)
	{
		printf("Freq: %f Time: %d Channel: %s\n", p->freq, p->time, (p->channel == CH_PRIMARY)?"Primary":"Secondary");
		p = p->next;
	}
} 
*/

