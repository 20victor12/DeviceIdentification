#define arraysize 200

/******Variables to read the current values from 20 milli second samples and get the I peaks for each 20 milliseconds.*******/

int count = 0;
int currentValue[200];      
int arraySize = 200;         
int currentPeakIndex;
float currentPeak;
int peaks[arraysize];
int Min_JumpMagnitude;
int IndexofC1;
//float ToleranceC3;
int Jump_Difference;
int NumberOfDevices = 4 ; // variable to store number of Devices

/******* C1's variable needed for identification *******/

int True_flag = 0;
int prevValue = 0;
int current_Value = 0;

/******* C2' Variables *****/
int n=0; 
int f1=1;
int cl=1;

/******* Structure For Learning********/

struct TemplateLibrary
{
  int JumpMagnitude ;
  int ShapeMagnitude;
  int ShapeIndex;
  int ToleranceC2;
  float AvgSteadyState;
  float SettlingTime;
  float ToleranceC3;
  int flag1;
  int flag2;
  int flag3;
} ;

/******* Structure ForRuntime*********/

struct Runtime
{
  int previousSteadyState;
  int JumpMagnitude;
  int ShapeMagnitude;
  int ShapeIndex;
  float AvgSteadyState;
  int SettlingTime;
  int IndexofC1;
} ;

/****** Object Declaration for the above Structures******/

TemplateLibrary device[10];
Runtime D;

void findShapeMagnitude();
void findSettlingTime();
/******* Function to find the current peak *******/

int findCurrentPeak ( int currentArray[200] )
{
  int currentPeakIndex = 0;
  for ( int i = 0; i < arraySize; i++ )
  {
    if ( currentArray[i] > currentArray[currentPeakIndex] )
    {
      currentPeakIndex = i;
    }
  }
  return currentPeakIndex;
}


/****Function which initially sets the values for the jump magnitude based on the trails of readings*****/

void setValues()
{
   device[0].JumpMagnitude = 522 - 514;
   device[1].JumpMagnitude = 521 - 514;
   device[2].JumpMagnitude = 873 - 514;
   device[3].JumpMagnitude = 617 - 514;
   device[0].ShapeMagnitude=522;
   device[1].ShapeMagnitude=521;
   device[2].ShapeMagnitude=875;
   device[3].ShapeMagnitude=619;
   device[0].ShapeIndex=2;
   device[1].ShapeIndex=1;
   device[2].ShapeIndex=4;
   device[3].ShapeIndex=1;
   device[0].AvgSteadyState = 521.3506;
   device[1].AvgSteadyState = 520.7272;
   device[2].AvgSteadyState = 548.2884;
   device[3].AvgSteadyState = 617;
   device[0].ToleranceC3 = 0.5;
   device[1].ToleranceC3 = 2;
   device[2].ToleranceC3 = 2;
   device[3].ToleranceC3 = 1;
   for(int i = 0; i<NumberOfDevices ; i++)
   {
    device[i].flag1 = 0;
    device[i].flag2 = 0;
    device[i].flag3 = 0;
   }
}


/***** Function to find the minimum of the jump magnitude among the available devices*****/

void Find_Min_JumpMagnitude()
{
 int MinimumValue = device[0].JumpMagnitude;

 for(int i = 0; i<3 ; i++)
 {
  if(device[i].JumpMagnitude<MinimumValue)
  {
    MinimumValue = device[i].JumpMagnitude;
  }
}

Min_JumpMagnitude = MinimumValue; 

/*Serial.println("min:");
Serial.println(Min_JumpMagnitude);
*/

}

/****** Function to get the Ipeak among the 20 Milliseconds reading******/

int Get_Ipeak()                                              
{
   for ( int i = 0; i < 200; i++ )
   {
     currentValue[i] = analogRead(A1);
   }
   currentPeakIndex = findCurrentPeak ( currentValue );
   return(currentValue[currentPeakIndex]);
}



/******* Function of C1 to find the jump value ******/

void C1_Jump()
  {  
      D.previousSteadyState = prevValue;
      prevValue = current_Value;
      current_Value = Get_Ipeak();                           //If true finding the first true value
      D.JumpMagnitude=current_Value -  D.previousSteadyState;   //Finding the Magnitude of the first array value

      if(D.JumpMagnitude > Min_JumpMagnitude)
     { 
       peaks[0] = D.previousSteadyState;                  //Initialising the zeroth , abrupt and first array value
       peaks[1] = prevValue;                              //Taking random value into the main array
       peaks[2] = current_Value;                             //Taking my n+1 th term as my threshold
       
       for(int j=3 ; j<arraysize ; j++)
       {
        peaks[j]=Get_Ipeak();                                        //Getting all the array values till n=200
       }

   C1_Identification();
  
   D.IndexofC1 = 1;   // index
  findShapeMagnitude();
  displayVal();
  }
  }
  


/***** Function to identify which device is on through the C1 jump value and reference table comparison ******/

void C1_Identification()
{

  for(int i=0; i<NumberOfDevices ;i++)
  {
   if((D.JumpMagnitude <= ( device[i].JumpMagnitude + 28) ) && ((D.JumpMagnitude >= (device[i].JumpMagnitude - 28))))
   {
     device[i].flag1 = 1;
   }
    else
   {
     device[i].flag1 = 0;
   }
  }
}

/***** Function to display which device is on based on the flag values set******/

void displayVal()
{
  Serial.println(D.previousSteadyState);
  Serial.println(prevValue);
  Serial.println(current_Value);
  Serial.println(D.JumpMagnitude);
  
 for(int i=0; i<NumberOfDevices; i++)
 {
   Serial.print("Device ");
   Serial.print(i+1);
   Serial.print(':');
   Serial.print(device[i].flag1);
   Serial.print(' ');
   Serial.print(device[i].flag2);
   Serial.print(' ');
   Serial.println(device[i].flag3);
  /*if(device[i].flag == 1)
 {
   Serial.println("Device: ");
   Serial.print(i);
   Serial.print("   is ON");
   Serial.println();
  } 
   
   else if(device[i].flag == 0)
 {
   Serial.println("Device: ");
   Serial.print(i);
   Serial.print("   is OFF");
   Serial.println();
  }*/ 
 }
 for(int i = 0; i<NumberOfDevices ; i++)  // again setting the flags to zero after identifing the device and printing
 {
  device[i].flag1 = 0;
  device[i].flag2 = 0;
  device[i].flag3 = 0;
 }
 Serial.println(D.previousSteadyState);
  
}
/*void displayC2()
{
  Serial.println(D.ShapeIndex);

  Serial.println(D.ShapeMagnitude);
  
 for(int i=0; i<4; i++)
 {
  if(device[i].flag == 1)
 {
   Serial.println("Device: ");
   Serial.print(i);
   Serial.print("   is ON");
   Serial.println();
  } 
   
   else if(device[i].flag == 0)
 {
   Serial.println("Device: ");
   Serial.print(i);
   Serial.print("   is OFF");
   Serial.println();
  } 
 }
 for(int i = 0; i<4 ; i++)  // again setting the flags to zero after identifing the device and printing
 {
  device[i].flag = 1;
 }
}*/


 
void findShapeMagnitude() {
  //int indexlocal=D.IndexofC1-1;  // local variable for indicating the point of implementation of C2
  int localindex=0;
  int i;
  int newMagnitude=D.JumpMagnitude; //local variable for succesive magnitude comparision
 
  for(i=1;i<arraySize-1;i++)
    peaks[i]=peaks[i+1];
  /*for(i=indexlocal; i<arraySize-1; i++)
  {
    if(peaks[i]<peaks[i+1])
      continue;
     else if(f1==1)
        {
          f1=0;
          for(int k=i;k<arraySize-1;k++)
            peaks[k]=peaks[k+1];
            break;
        }
  }*/
     for(i=0; i<arraySize-1; i++)
  {
    if(peaks[i]<peaks[i+1])
      continue;
      else
       {
       D.ShapeIndex=i+1;
       D.ShapeMagnitude=peaks[i];
       break;
       
       }
       
        }
        Deviceidentification_C2();
        Serial.print("ShapeMagnitude:");
        Serial.println(D.ShapeMagnitude);
        Serial.print("ShapeIndex:");
        Serial.println(D.ShapeIndex);
        
}


void Deviceidentification_C2()
  {
    for (int i=0;i<NumberOfDevices; i++)
    { 
      if( (abs(D.ShapeIndex-device[i].ShapeIndex)<=3) && (abs(D.ShapeMagnitude-device[i].ShapeMagnitude)<=2))
      { 
         device[i].flag2=1;
        //continue;
        //displayC2();
      }
     else
      {
        device[i].flag2=0;
      }
    
    }
    findSettlingTime();
   //displayC2(); 
  }
  /********** Runtime identification using C3 *****************/

void findSettlingTime () // Find settling time
{
  int localindex = D.ShapeIndex - 1;
  int i;
  int count = 0; // Skipping the first occurance of a positive rise 
  int Settlingflag = 0; // To break the cycle
  Serial.print("Starting value");
  Serial.println(peaks[localindex]);
  for ( i = localindex; i < (arraySize - 1); i++ )
  {
    if (( peaks[i] <= peaks[i+1] ) && ( count == 0 ))
     { 
       count = 1; // First positive rise
       continue;
     }
     if (( peaks[i] <= peaks[i+1] ) && ( count == 1 )) // Second rise
     {
       D.SettlingTime = i - localindex;
       break;
     }
  }
  Serial.print("Settling value");
  Serial.println(peaks[i]);
  Serial.print("SEttling Time");
  Serial.println(D.SettlingTime);
  findAverage();
}


void findAverage ()
{
  float average = 0;// To find average of steady state
  int localindex = D.SettlingTime;
  for ( int i = localindex; i < arraySize; i++ )
  {
    average = average + (peaks[i]); // - D.previousSteadyState );
  }
  D.AvgSteadyState = average / ( arraySize - localindex ); // To find steady state average from settling time to end
  Serial.print("Average");
  Serial.println(D.AvgSteadyState);
  comparison();
}

void comparison ()
{
  for ( int i = 0; i < 4; i++ )
  {
      Serial.print("Absolute value");
      Serial.println(abs (device[i].AvgSteadyState - D.AvgSteadyState ));
      Serial.print("Tolerance");
      Serial.println(device[i].ToleranceC3);
      if ( (abs (device[i].AvgSteadyState - D.AvgSteadyState )) <= (device[i].ToleranceC3) )
      {
        device[i].flag3 = 1;
      }
      /*else
      {
        device[i].flag3=0;
      }*/
    
  }
}

/*************End of C3 *********************/
void setup() {
Serial.begin(9600);
analogReference('EXTERNAL');
pinMode(A1,INPUT);
setValues();                           // this function is called only once from the setup to initialize the values
Find_Min_JumpMagnitude();            
}


                                  

void loop()
{ 
// during the initial condition both my prevvalue and current value are read as my analog vales to avoid false triggering
  while(count == 0)
  {     prevValue = Get_Ipeak(); 
  count =1;
  }
  
  current_Value = Get_Ipeak();
  D.JumpMagnitude = current_Value - prevValue;     // calculating the difference i.e. the jump values

   if(D.JumpMagnitude > 0 && (current_Value > 514) )
   { 
    C1_Jump();
   }
  prevValue = current_Value;
  
  
  

}

