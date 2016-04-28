//#define arraySize 200;
int peaks[200];

int arraySize; // The number of sample taken for trainning

struct TemplateLibrary
{
  int JumpMagnitutde;
  int IndexofC1;
  int ShapeMagnitude;
  int ShapeIndex;
  int ToleranceC2;
  float AvgSteadyState;
  float ToleranceC3;
  int SettlingTime;
  int flag;
};

TemplateLibrary device[10];


struct Runtime
{
  int PreviousSteadyState;
  int JumpMagnitutde;
  int ShapeMagnitude;
  int ShapeIndex;
  float AvgSteadyState;
  int SettlingTime;
  int Indexofc1;
};

Runtime D;

void findSettlingTime () // Find settling time
{
  int localindex = ( D.Indexofc1) + D.ShapeIndex - 1;
  int count = 0; // Skipping the first occurance of a positive rise 
  int Settlingflag = 0; // To break the cycle
  for ( int i = localindex; i < (arraySize - 1); i++ )
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
}


void findAverage ()
{
  float average = 0;// To find average of steady state
  int localindex = D.SettlingTime + (( D.Indexofc1 ) + ( D.ShapeIndex ) - 1 );
  for ( int i = localindex; i < arraySize; i++ )
  {
    average = average + (peaks[i] - D.previousSteadyState );
  }
  D.AvgSteadyState = average / ( arraySize - localindex ); // To find steady state average from settling time to end
}

void comparison ()
{
  for ( int i = 0; i < 10; i++ )
  {
    if ( device[i].flag == 1 )
    {
      if ( abs (device[i].AvgSteadyState - D.AvgSteadyState ) > device[i].ToleranceC3 )
      {
        device[i].flag = 0;
      }
    }
  }
}

void setup()
{
}

void loop ()
{
}

