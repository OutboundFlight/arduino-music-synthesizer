


byte inData[9];     // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
boolean songComplete = false;  // whether received full song info

int led = 4;
int count = 0; // Keeps track of how many bytes of data the Arduino has recieved.


// Variables to hold song notes and keep track of total count
// Using fixed length array for now, will use dynamic later
float FreqArray[70];
float DurArray[70];
int ArrayCounter = 0;

// Used to convert 4 bytes of unsigned int into floating point
union UDATA
{
    float fValue;
    byte iValue[4];

};

// Create to UDATA structures, one for freq and one for duration
UDATA uFreq, uDur;

void setup()
{
    // initialize serial:
    Serial.begin(9600);
    // initialize the digital pin as an output.
    pinMode(led, OUTPUT);
    Serial.println("Setup Function complete");
}

void loop()
{
    // If stringComplete is true, that means that SerialEvent has received
    // a NULL character, so now we need to read that data and do something
    // with it.
    // Data received is going to be a 9 byte, null terminated char array.
    // The first 4 bytes are the frequency (in little-endian order)
    // The next 4 bytes is the duration (in little endian order)
    // The last char is NULL
    if (stringComplete)
    {
        Serial.println(" ");  // This print is for character debugging
        uFreq.iValue[0]= inData[0];
        uFreq.iValue[1]= inData[1];
        uFreq.iValue[2]= inData[2];
        uFreq.iValue[3]= inData[3];
        
        uDur.iValue[0]= inData[4];
        uDur.iValue[1]= inData[5];
        uDur.iValue[2]= inData[6];
        uDur.iValue[3]= inData[7];
        
        // Store current values in next position in arrays
        FreqArray[ArrayCounter] = uFreq.fValue;
        DurArray[ArrayCounter] = uDur.fValue;
        ArrayCounter++;
        
        // clear the string:
        for (int i = 0; i < 8; i++)
            inData[i] = 0;

        count = 0;
        stringComplete = false;

        if (isnan(uFreq.fValue) && isnan(uDur.fValue))
        {
          Serial.print("Bytes received: ");
          for(int i = 0; i < 4; i++)
              Serial.print(uFreq.iValue[i]);
          for(int i = 0; i < 4; i++)
              Serial.print(uDur.iValue[i]);
          Serial.println();
            songComplete = true;    
            Serial.flush();
        }          
    } // end stringComplete loop
    
    if (songComplete)
    {
      // Now that song is complete, we'll play the song
      Serial.println("The song has been received!");
      for (int i = 0; i < ArrayCounter; i++)
      {
         tone(8,FreqArray[i],DurArray[i]*1000);
         delay(DurArray[i]*1000);
         //Serial.print("Freq: ");
         //Serial.print(FreqArray[i]);
         //Serial.print("Dur: ");
         //Serial.println(DurArray[i]);
         //tone(8,0,0.05);
         //delay(50);
      }
      songComplete = false;
      stringComplete = false;
      noTone(8);
    } // end songComplete loop
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent()
{
    byte inChar = 0;
    while (Serial.available())
    {
        // get the new byte:
        inChar = (byte)Serial.read();
        inData[count] = inChar;
        count++;
        
        // Code prints incoming data in HEX format. 
        // Uses stringComplete section ABOVE to print newline
        // To make readin easier. 
        // DOESNT PRINT THE CORRECT CHARACTERS when you have tone enabled
        // DISABLE the playing of tones for it to work. 
        //Serial.print(inData[count], HEX);
        //Serial.print(" ");
        
        // if the incoming character is 8 bytes long, set a flag
        // so the main loop can do something about it:
        if ((count > 8) && (inChar == '\0'))
        {
            stringComplete = true;
            break;
        }

    }
}





