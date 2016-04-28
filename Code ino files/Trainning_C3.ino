//#define arraySize 200;
int Peaks[200];
int startingIndex; // Passed from vicky
int index; // Device index
int n; // The number of sample taken for trainning

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


void findSettlingTime () // Find settling time
{
  int count = 0; // Skipping the first occurance of a positive rise 
  int flag = 0; // To break the cycle
  for ( int i = device[i].ShapeIndex; i < (n - 1); i++ )
  {
    if (( Peaks[i] <= Peaks[i+1] ) && ( flag == 0 ))
     { 
       count = 1; // First positive rise
     }
     if ((( Peaks[i] <= Peaks[i+1] ) && ( count == 1 )) && (flag == 0 )) // Second rise
     {
       device[index].SettlingTime = i;
       flag = 1;
     }
  }
}


void findTolerance ()
{
  int toleranceArray[10]; // To store the discreet values
  int count = 0; // Number of steady state values
  int flag = 0;// To check if the value has occured before
  int temp; //For swapping
  float average = 0;// average for tolerance
  
  for ( int i = 0; i < 10; i++ )
  {
    toleranceArray[i] = 0;
  }
  
  for ( int i = device[index].SettlingTime; i < n; i ++ )// To store only unique values
  {
    flag = 0;
    for ( int j = 0; j < 10; j++ )
    {
      if ( Peaks[i] == toleranceArray[j] )// The value is already there in the toleranceArray
      {
        flag = 1;
      }
    }
    if ( flag == 0 )// The vlue is not in the toleranceArray
    {
      toleranceArray[count] = Peaks[i];//Add value to toleranceArray
      count = count + 1;//Increase last index in tolerance array
    }
  }
  
  for ( int i = 0; i < count; i++ )// Sort the toleranceArray using bubble sort
  {
    for ( int j = 0; j < count-i-1; j++ )
    {
      if ( toleranceArray[i] > toleranceArray[j] )
      {
        temp = toleranceArray [i];
        toleranceArray[i] = toleranceArray[j];
        toleranceArray[j] = temp;
      }
    }
  }
  
  for ( int i = 0; i < count; i++ ) // Find average
  {
    average = average + toleranceArray[i];
  }
  average = average / count;
  device[index].ToleranceC3 = ( abs ( toleranceArray[0] - toleranceArray[count] ) / average ); //Tolerance
}

void findAverage ()
{
  float average = 0;// To find average of steady state
  for ( int i = device[index].SettlingTime; i < n; i++ )
  {
    average = average + Peaks[i];
  }
  device[index].AvgSteadyState = average / ( n - device[index].SettlingTime ); // To find steady state average from settling time to end
}

void setup()
{
}

void loop()
{
}

