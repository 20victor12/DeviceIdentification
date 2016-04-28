#define arraysize 200
int Peaks[arraysize];

struct TemplateLibrary {
  int JumpMagnitude;
  int IndexofC1;
  int ShapeMagnitude;
  int ShapeIndex;
  int ToleranceC2;
  float AvgSteadyState;
   float Tolerance;
   int SettlingTime;
   int flag;
};

TemplateLibrary device[10];

int a; //indexOfc1 c1's pass
int  b; //jumpmagnitude c1's pass
int c;//Shapemagnitude c1's pass
int d; //shapeIndex c1's pass
int i;
void findShapeMagnitude() {
  int index=a;  // local variable for indicating the point of implementation of C2
  int newMagnitude=b; //local variable for succesive magnitude comparision
  for( i=index+1;i<=arraysize;i++)
  {
    if (Peaks[i]>newMagnitude) //condition for finding maxima
    {
      newMagnitude=Peaks[i];
    }
    else
    {
      break;
    }
  }
  
  c=newMagnitude; // returning the peak
  d=i; //returning index of peak
}
  
 
 
