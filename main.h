#ifndef MAIN_DEF
#define MAIN_DEF
#define _BSD_SOURCE
#define DEBUG_PRINT 1
#define FILE_TYPE_MONO 1
#define FILE_TYPE_STEREO 2

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <endian.h>
#include <time.h>

#include "NoteEvent.h"
#include "StrToFreq.h"
#include "utility.h"
#include "FileIO.h"
#include "UserInterface.h"
#include "arduino-serial/arduino-serial-lib.h"

#include "NoteEvent.c"
#include "StrToFreq.c"
#include "utility.c"
#include "FileIO.c"
#include "UserInterface.c"
#include "arduino-serial/arduino-serial-lib.c"

#endif

