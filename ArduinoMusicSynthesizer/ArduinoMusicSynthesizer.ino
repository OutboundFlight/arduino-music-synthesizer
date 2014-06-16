/////////////////////////////////////////////////////////////
//        ***     ArduinoMusicSynthesizer   ***            //
//                                                         //
//        Authors:  Eric Bennett & Aaron Baker             //
//                                                         //
//                                                         //
//                                                         //
//                                                         //
/////////////////////////////////////////////////////////////

//   Create Structure Array. This array will be fixed in length. 
// When a note is played, the index will advance. Once that happens, 
// the prior array value at that index will be set to NULL. Once 
// the sketch has advanced through the entire array, it will put the
// index back to 0, and check the value for NULL. If NULL, it is done
// playing, if not NULL, it will cycle through until it reaches NULL. 
// Currently, will set the channel to 0, since that is a value that
// should not occur otherwise (Instead of testing for NULL)
typedef struct
{
   float freq; // Size 4 bytes
   unsigned long time; // size 4 bytes
   unsigned int channel; // size 2 bytes
}NoteStructList;
unsigned int ARRAY_SIZE = 150; // This is just for reference, not for actual declaration of Array
NoteStructList NoteArray[150]; //Main struct array to cycle through

//  This global variable stores the index of the next position in the array
// that contains note information to be played next. 
unsigned int PlaybackPosition = 0; 

//  This global variable stores the index of the next position in the array 
// to store the next received note information (in serialEvent())
unsigned int StorePosition = 0;

//  This global variable stores the total number of notes received
// Although the application will check struct values for NULL to indicate end,
// this may be used in circumstances where an actual numerical value is desired.
unsigned int TotalNotesReceived = 0;

// This global variable is used to store the 8-byte 'packets' that are received
// from the serialEvent and then converted to the float and int values to be 
// stored in the NoteArray.
byte inData[11];
unsigned int count = 0; //Used to keep track of how many of the 8 bytes have been received.

// Initial Buffer Packet Received. 
boolean ReceivedInitialBuffer = false;

// This keeps track on whether entire song has been received
boolean ReceivedEntireSong = false; 

// This stores the system time at t=0 in reference to initial playback
unsigned long TimeEqualsZero = 0;

// This is set to true when playback starts. This allows the sketch
// to store the system up-time in a variable as a reference
// time-point for song playback. It essentially sets t=0 at playback.
boolean PlaybackStarted = false;

// Boolean to indicate song is finished
boolean PlaybackComplete = false; 

// Used to convert 4 bytes into floating point
union uDATA4
{
    float FloatValue;
    byte ByteArray[4];

} UDATA4;

// Used to convert 4 bytes into unsigned long
union uDATA4L
{
    unsigned long LongValue;
    byte ByteArray[4];

} UDATA4L;

// Used to convert 2 bytes into unsigned int
union uDATA2
{
    unsigned int IntValue;
    byte ByteArray[2];
} UDATA2;

// Output pins to drive speaker circuit
unsigned int ChannelOnePin = 8;
unsigned int ChannelTwoPin = 7;

// Output pins to drive LEDs
unsigned int LED_Oct3 = 3;
unsigned int LED_Oct4 = 9;
unsigned int LED_Oct5 = 10;


/////////////////////////////////////////////////////////////
//             ***     setup()   ***
// This function runs only once prior to loop().           //
/////////////////////////////////////////////////////////////
void setup(void)
{
// Initialize Serial port
Serial.begin(9600);
Serial.println("Welcome to the Arduino Music Synthesizer!");

// Initialize PINs
pinMode(ChannelOnePin, OUTPUT);
pinMode(ChannelTwoPin, OUTPUT);
pinMode(LED_Oct3, OUTPUT);
pinMode(LED_Oct4, OUTPUT);
pinMode(LED_Oct5, OUTPUT);

}///END setup()//////////////////////////////////////////////






/////////////////////////////////////////////////////////////
//             ***     loop()   ***
// This function is the equivilient of main() in c.        //
/////////////////////////////////////////////////////////////
void loop(void)
{
  
  // This checks to see if playback should occur
  if (((ReceivedInitialBuffer)||(ReceivedEntireSong))&&(PlaybackComplete==false))
  {
    // If playback has not yet started, take care of some playback
    // initializations. 
    if (!PlaybackStarted)
    {
      TimeEqualsZero = millis();
      PlaybackStarted = true;
      Serial.println("Printing TimeEqualsZero");
      Serial.println(TimeEqualsZero);
    }
    
    // Checks to see if it is currently at, or after, the time specified
    // for the next note to be played. 
    if ((millis()-TimeEqualsZero) >= NoteArray[PlaybackPosition].time)
    {
       
       // Determine Channel, and play back accordingly
       if (NoteArray[PlaybackPosition].channel == 1)
       {
           // Put channel one playback code here 
           //Serial.println("Playback Channel One: ");
           //Serial.println(NoteArray[PlaybackPosition].freq);
           if(NoteArray[PlaybackPosition].freq == 0)
           {        
             noTone(ChannelOnePin);
             delay(10);
           }
           else
           { 
             noTone(ChannelOnePin);
             delay(20);
             tone(ChannelOnePin, NoteArray[PlaybackPosition].freq);
             
           }
       }
       if (NoteArray[PlaybackPosition].channel == 2)
       {
           // Put channel two playback code here 
           Serial.println("Playback Channel Two: ");
           Serial.println(NoteArray[PlaybackPosition].freq);
           if(NoteArray[PlaybackPosition].freq == 0)  
           {      
             //noTone(ChannelTwoPin);
             //delay(20);
           }
           else
           { 
             //noTone(ChannelTwoPin);
             //delay(20);
             //tone(ChannelTwoPin, NoteArray[PlaybackPosition].freq);
           }
       }
       delay(20);
       // Now that playback is being handled, going to light
       // up some LEDs!
       if ((NoteArray[PlaybackPosition].freq < 250) && (NoteArray[PlaybackPosition].freq > 0))
       {
           //analogWrite(LED_Oct3,255);
           //analogWrite(LED_Oct3,(int)NoteArray[PlaybackPosition].freq);
           //analogWrite(LED_Oct4,0);
           //analogWrite(LED_Oct5,0);
       } else if ((NoteArray[PlaybackPosition].freq >= 250) && (NoteArray[PlaybackPosition].freq <= 500))
       {
           //analogWrite(LED_Oct4,255);
           //analogWrite(LED_Oct4,(int)NoteArray[PlaybackPosition].freq % 255);
           //analogWrite(LED_Oct3,0);
           //analogWrite(LED_Oct5,0);
       } else if (NoteArray[PlaybackPosition].freq >= 500)
       {
           //analogWrite(LED_Oct5,255);
           //analogWrite(LED_Oct5,(int)NoteArray[PlaybackPosition].freq % 255);
           //analogWrite(LED_Oct3,0);
           //analogWrite(LED_Oct4,0);
       }
       
       
       // Set the current struct array value (will use channel as the test)
       //  to NULL, since played and increment playback position. 
       //  Reset to 0 if past 149
       NoteArray[PlaybackPosition].channel = 0;
       NoteArray[PlaybackPosition].freq = 0xFFFFFFFF;
       PlaybackPosition++;
       
       // Rolling over to first position
       if (PlaybackPosition > 149) 
       {
         PlaybackPosition = 0;
       }
       
       // Checking to see if next note's channel is set to zero, or 255. Both conditions
       // would indicate that the song is over and playback should be stopped.
       if ((NoteArray[PlaybackPosition].channel == 0) || (NoteArray[PlaybackPosition].channel == 255)||(isnan((double)NoteArray[PlaybackPosition].freq)))
       {
           PlaybackComplete = true;
           delay(1000);
           noTone(ChannelOnePin);
           Serial.println("Song playback complete!");
           //noTone(ChannelTwoPin);
           //analogWrite(LED_Oct3,0);
           //analogWrite(LED_Oct4,0);
           //analogWrite(LED_Oct5,0);
       }
    }
  }


}///END loop()///////////////////////////////////////////////



/////////////////////////////////////////////////////////////
//             ***     serialEvent()   ***
// This Event is responsible for recieving new serial data //
/////////////////////////////////////////////////////////////
void serialEvent()
{
    if (!ReceivedEntireSong){
    byte inChar = 0;
    while (Serial.available())
    {
        // get the new byte:
        inChar = (byte)Serial.read();
        inData[count] = inChar;
        count++;
        
        // if the incoming character is 8 bytes long, and it receives
        // a NULL char, process this incoming packet. 
        if ((count > 10) && (inChar == '\0'))
        {  
          //   float freq; // Size 4 bytes
           //  unsigned int time; // size 2 bytes
           //unsigned int channel; // size 2 bytes
          UDATA4.ByteArray[0] = inData[0];
          UDATA4.ByteArray[1] = inData[1];
          UDATA4.ByteArray[2] = inData[2];
          UDATA4.ByteArray[3] = inData[3];
          NoteArray[StorePosition].freq = UDATA4.FloatValue;
          
          UDATA4L.ByteArray[0]= inData[4];
          UDATA4L.ByteArray[1]= inData[5];
          UDATA4L.ByteArray[2]= inData[6];
          UDATA4L.ByteArray[3]= inData[7];
          NoteArray[StorePosition].time = UDATA4L.LongValue;
          
          UDATA2.ByteArray[0]= inData[8];
          UDATA2.ByteArray[1]= inData[9];
          NoteArray[StorePosition].channel = UDATA2.IntValue;
          
          // Print for debugging purposes
          Serial.println(NoteArray[StorePosition].freq);
          Serial.println(NoteArray[StorePosition].time);
          Serial.println(NoteArray[StorePosition].channel);
          
          // Advance the StorePosition counter
          if (isnan(NoteArray[StorePosition].freq))
          {
              ReceivedEntireSong = true;
              Serial.println("Received the entire Song!");
          } else
          {
              StorePosition++;
              if (StorePosition > 149)
              {
                StorePosition = 0; // Cycle back to index 0 for new values 
              }
              TotalNotesReceived++;
              if (TotalNotesReceived == 50)
              {
                ReceivedInitialBuffer = true;
                Serial.println("Received Buffer of 50 values.");
              }
          }
          // clear the string:
          for (int i = 0; i < 10; i++)
              inData[i] = 0;
          count = 0;  
        }//END IF
        if (ReceivedInitialBuffer)
        {
           break; // If already received buffer, break after each packet
        }  
    }//END WHILE
    }//END IF for !ReceivedEntireSong
}///END serialEvent//////////////////////////////////////////



