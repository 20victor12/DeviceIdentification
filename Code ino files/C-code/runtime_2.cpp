#include<stdio.h>
#include<conio.h>


/*********Preprocesser directives********/

#define arraysize 200
#define zero 514

#define previousdevice "d1"
#define Numberofdevice 5
#define c1tolerance 10

/*********Structures****************/
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
};

struct Runtime
{
  int previousSteadyState;
  int JumpMagnitude;
  int ShapeMagnitude;
  int ShapeIndex;
  float AvgSteadyState;
  int SettlingTime;
  int IndexofC1;
};

/********Structure Instances**********/
struct TemplateLibrary device[10];
struct Runtime D;

/********Variables***********/
int c2_jump;
int count = 0;
int count1 = 0;
int flag=0;
int trialcount =0;
int N=0;
//int zero=514;
int offset = 0;
char text1[200];
int currentValue[200];
int arraySize = 200;
int currentPeakIndex;
float currentPeak;
int peaks[arraysize];
int Min_JumpMagnitude;
int IndexofC1;
int Jump_Difference;
int NumberOfDevices = 5 ;
int True_flag = 0;
int prevValue = 0;
int current_Value = 0;
int n=0;
int f1=1;
int cl=1;
int initialpeaks[arraysize];
int identifydevice[5];



/*****Function Prototypes*****/
void setValues();
void Find_Min_JumpMagnitude();
int Get_Ipeak();
void C1_Jump(int i);
void C1_Identification();
void findShapeMagnitude();
void displayVal();
void Deviceidentification_C2();
int absolute(int one, int two);
float absolute(float one, float two);
void findSettlingTime();
void findAverage();
void comparison();
int Read_Value();
int FileWritting();


FILE *input;
FILE *runtime;
FILE *ptr_file;            // File Name instance of the file opening and closing


int main()
{
  clrscr();

  int i;
  int k;


      ptr_file = fopen("input.txt", "r");
      printf("\ninput.txt");
      runtime = fopen("runtime.txt","a");
      printf("\nruntim.txt\n");
  vik:
  k=Read_Value();
  printf("readval\n");
if(k==0)
    printf("\nError\n");
  /*for (int i = 0; i < arraysize; ++i)
  {
    printf("initialpeaks \n");
    printf("%d\n",initialpeaks[i] );
  }*/
	setValues();
	Find_Min_JumpMagnitude();
	prevValue = initialpeaks[0];
	current_Value = initialpeaks[1];
	for (i = 1; i < arraysize; ++i)
	{
    //printf("entered first loop\n");
		current_Value = initialpeaks[i];
		D.JumpMagnitude = current_Value - prevValue;
		if(D.JumpMagnitude>=Min_JumpMagnitude)
    {

     // printf("enterd jump if\n");
      //printf("\nEnetring jump at:%d \n",i );
			D.previousSteadyState = prevValue;
      C1_Jump(i);
	    //label: initial:
      //printf("previousSteadyState: %d\n",D.previousSteadyState);
      //break;

    }
		prevValue = current_Value;

	}
	if(k==5 && count1<21)
	{

	printf("reached goto\n");count1++;

	goto vik;
	}

	getch();


	fclose(runtime);
	fclose(ptr_file);

	return 0;
}

void setValues()
{
  device[0].JumpMagnitude = 5;
  device[0].ShapeMagnitude= 6;
  device[0].ShapeIndex = 2;
  device[0].AvgSteadyState = 5.08;
  device[0].ToleranceC3 = 1;

  device[1].JumpMagnitude = 19;
  device[1].ShapeMagnitude= 9;
  device[1].ShapeIndex = 1;
  device[1].AvgSteadyState = 7.80;
  device[1].ToleranceC3 = 4;

  device[2].JumpMagnitude = 46;
  device[2].ShapeMagnitude= 13;
  device[2].ShapeIndex = 1;
  device[2].AvgSteadyState = 12.24;
  device[2].ToleranceC3 = 3.00;

  device[3].JumpMagnitude = 41;
  device[3].ShapeMagnitude= 379;
  device[3].ShapeIndex = 6;
  device[3].AvgSteadyState = 36.54;
  device[3].ToleranceC3 = 5;

  device[4].JumpMagnitude = 8;
  device[4].ShapeMagnitude= 110;
  device[4].ShapeIndex = 3;
  device[4].AvgSteadyState = 109.48;
  device[4].ToleranceC3 = 5;

	/*device[0].JumpMagnitude = 8;
   device[1].JumpMagnitude = 10;
   device[2].JumpMagnitude = 330;
   device[3].JumpMagnitude = 106;
   device[0].ShapeMagnitude= 8;
   device[1].ShapeMagnitude= 8;
   device[2].ShapeMagnitude= 367;
   device[3].ShapeMagnitude= 106;
   device[0].ShapeIndex=2;
   device[1].ShapeIndex=1;
   device[2].ShapeIndex=4;
   device[3].ShapeIndex=1;
   device[0].AvgSteadyState = 7.71;
   device[1].AvgSteadyState = 6.42;
   device[2].AvgSteadyState = 34.10;
   device[3].AvgSteadyState = 105.78;
   device[0].ToleranceC3 = 1;
   device[1].ToleranceC3 = 5;
   device[2].ToleranceC3 = 4;
   device[3].ToleranceC3 = 4; */
   for(int i = 0; i<NumberOfDevices ; i++)
   {
    device[i].flag1 = 0;
    device[i].flag2 = 0;
    device[i].flag3 = 0;
   }
}

void Find_Min_JumpMagnitude()
{
	int MinimumValue = device[0].JumpMagnitude;
	for(int i=0; i<5; i++)
		if(device[i].JumpMagnitude<MinimumValue)
			MinimumValue = device[i].JumpMagnitude;

	Min_JumpMagnitude = MinimumValue;
}

void C1_Jump(int j)
{       N=j;
	/*D.previousSteadyState = prevValue;
	prevValue = current_Value; //initialpeaks[j];*/
	for (int i = j-1, k=0; i < arraysize; ++i, k++)
	{
		peaks[k] = initialpeaks[i];
	}
  C1_Identification();
  D.IndexofC1 = 1;
  findShapeMagnitude();
  if(flag==0)
  displayVal();


}

void C1_Identification()
{
  for (int i = 0; i < NumberOfDevices; ++i)
  {
    if((D.JumpMagnitude <= ( device[i].JumpMagnitude + c1tolerance) ) && ((D.JumpMagnitude >= (device[i].JumpMagnitude - c1tolerance))))
   {
     device[i].flag1 = 1;
   }
  }

}

void findShapeMagnitude()
{
  int localindex=0;
  int i;
  for (i = 1; i < arraysize-1; ++i)
  {
    /* code */
    peaks[i]=peaks[i+1];
  }

 // printf("Shape\n");
  for (i = 0; i < arraysize-1; ++i)
  {

    if (peaks[i] < peaks[i+1])
    {
      //printf("%d\n",peaks[i] );
      continue;
    }
    else
    {
      D.ShapeIndex = i;
      c2_jump = i;
      //printf("previousSteadyState in ShapeMagnitude:%d\n",D.previousSteadyState);
      D.ShapeMagnitude = (peaks[i]-D.previousSteadyState);
      break;
    }
  }
  /*printf("\nPrevious steady state:%d",D.previousSteadyState);
  printf("\nshape index:%d", D.ShapeIndex);
  printf("\nshape magnitude%d", D.ShapeMagnitude);*/
  Deviceidentification_C2();
}

void Deviceidentification_C2()
{
  int min=1000;
  int minindex;
  int difference;

  for (int i = 0; i < NumberOfDevices; ++i)
  {
    difference = absolute(D.ShapeMagnitude, device[i].ShapeMagnitude);
   // printf("difference: %d\n",difference);
    if (difference<min)
    {
      min = difference;
      minindex = i;
    }

  }
 // printf("minindex%d\n",minindex);
  device[minindex].flag2 = 1;
  findSettlingTime();
}

int absolute(int one, int two)
{
  if (one >= two)
  {
    return (one-two);
  }
  else
  {
    return (two-one);
  }
}

void findSettlingTime()
{
  //printf("Entered settling time\n");
  int localindex = D.ShapeIndex ;
  //printf(" settling localindex:%d\n", localindex);
  int i;
  int count =0;
  int Settlingflag=0;
  //printf("SEttling time values:\n");
  for (i = localindex; i < arraysize-1; ++i)
  {
    //printf("%d\n",peaks[i] );
    if ((peaks[i]<peaks[i+1]) && count<5)
    {
      //printf("peaks:%d\n",peaks[i] );
      count++;
      continue;
    }
    if ((peaks[i]<peaks[i+1] && count ==5))
    {
      D.SettlingTime = i - localindex;
      break;
    }

  }
  findAverage();
}

void findAverage()
{
  //printf("entering find average\n");
  int count =0;
  int average = 0;// To find average of steady state
  int localindex = D.SettlingTime;
  //printf("average localindex:%d\n",peaks[localindex] );
  //printf("%d\n",D.previousSteadyState );
  /*for (int i = 0; i < arraysize; ++i)
  {
    if(peaks[i]==0)
     {
      offset = i+1;
      break;
     }
  }*/
     /*for (int i = localindex; i < arraysize; ++i)
     {
       printf("%d\n",peaks[i] );
     }*/
  //printf("test%d\n",arraysize-offset);
  //printf("index%d\n",localindex );
       flag=0;
  for ( int i = localindex; i < arraysize-offset; i++ )
  {
    //printf("%d\n",peaks[i] );
    if(peaks[i]==0)
    {
      count++;
      continue;
    }
    if((absolute(peaks[i+1],peaks[i]))<=Min_JumpMagnitude)
      average = average + (peaks[i]- D.previousSteadyState);
    else{
      /*printf("break\n");
      printf("%d\n",peaks[i-1] );
      printf("%d\n",peaks[i] );
      printf("%d\n",peaks[i+1] );*/
      break;
    }
    if(average<=0)
    {
      //printf("alert%d,%d\n",i,peaks[i] );
      flag = 1;
      break;
      //goto label;
    }
  }
  //printf("average :%d\n",average );
  if (flag==0)
  {
    D.AvgSteadyState = average / ( (arraysize-offset) - localindex - count );
  //printf("denominatro:%d\n",(arraysize-localindex) );
  //printf("Average Steady State : %f\n",D.AvgSteadyState );
  //printf("average%d\n",average );
  comparison();
  }

}

void comparison()
{
  //printf("entereing comparison\n");
  for (int i = 0; i < 4; ++i)
  {
    //printf("Difference c3:%f\n", absolute(device[i].AvgSteadyState, D.AvgSteadyState ));
    if ( (absolute(device[i].AvgSteadyState, D.AvgSteadyState )) <= (device[i].ToleranceC3) )
      {
	//printf("Difference c3:%f\n", absolute(device[i].AvgSteadyState, D.AvgSteadyState ));
	device[i].flag3 = 1;
      }
  }

}

float absolute(float one, float two)
{
  if (one >= two)
  {
    return (one-two);
  }
  else
  {
    return (two-one);
  }
}







void displayVal()
{
  /*for (int i = 0; i < arraySize-29; ++i)
  {
    //printf("peak values\n");
  printf("%d\n",peaks[i] );
  }*/
  int sum[5]={0,0,0,0,0};
  int max=0;
  int dindex=0;
  int j;
{
  c_count3 = 10;
  for ( j = 0; j < NumberOfDevices; ++j)
  {
    sum[j] = sum[j]+ device[j].flag1+ device[j].flag2;
    if(sum[j]>max)
    {
      max=sum[j];
      dindex = j;

    }
    //sum[j]=0;

  }
  identifydevice[dindex] = 1;
  for (int i = 0; i < NumberOfDevices; ++i)
  {
    if(sum[i]==max)
    {
      if (device[i].flag2==1)
      {
	identifydevice[i]=1;


      }
      else
      {
	identifydevice[i]=3;
      }
    }
  }
  int fl=0;

//printf("\nIdentified Device:" );
//fprintf(runtime,"\nIdentified Device:" );
  for (i = 0; i < NumberOfDevices; ++i)
  {

    if(identifydevice[i]==1)
      {
      //printf("d%d,",i+1 );
      //fprintf(runtime,"d%d,",i+1 );

      }


  else if ( identifydevice[i]==3)
    {
      for (int i = 0; i < NumberOfDevices; ++i)
      {
	if (device[i].flag2==1)
	{
	  fl=1;
	  identifydevice[i] = 1;
	}

      }
      if(fl==0)
      //printf("or d%d ",i+1);
      {//fprintf(runtime,"or d%d ",i+1);
      }
    }
  }
  //while
  //fprintf(runtime,"\n");
  for (i = 0; i < NumberOfDevices; ++i)
  {
    if(identifydevice[i]==1 || identifydevice[i]==3)
   {}   //fprintf(runtime,"Device %d: C3 %d  \n",i+1 ,device[i].flag3);
  }
  //printf("\n");
  //printf("\n");
  for (i = 0; i < NumberOfDevices; ++i)
  {
      if(identifydevice[i]==1 )
    { // printf("Device %d:C3 %d  \n",i+1 ,device[i].flag3);
       iddevice = i;
      printf("\n Device %d : %d %d ,N-j: %d \n",i+1 ,device[i].flag1,device[i].flag2,c2_jump-jump);
       //fprintf(runtime,"id = %d\n",identifydevice[i]);

       //fprintf(runtime,"\nMod : %d ",mod);
      fprintf(runtime,"\n Device %d : %d %d ,N-j: %d \n",i+1 ,device[i].flag1,device[i].flag2,c2_jump-jump);
     }
  }
  //fprintf(runtime,"\n");
printf("\n");
  }//while

  for (int i = 0; i < NumberOfDevices; ++i)
  {
    device[i].flag1 = 0;
    device[i].flag2 = 0;
    device[i].flag3 = 0;

  }
  count++;

}













void displayVal()
{
  /*for (int i = 0; i < arraySize-29; ++i)
  {
    //printf("peak values\n");
  printf("%d\n",peaks[i] );
  }*/
  int sum[5]={0,0,0,0,0};
  int max=0;
  int dindex=0;
  int j;
  for ( j = 0; j < NumberOfDevices; ++j)
  {
    sum[j] = sum[j]+ device[j].flag1+ device[j].flag2+ device[j].flag3;
    if(sum[j]>max)
    {
      max=sum[j];
      dindex = j;

    }
    //sum[j]=0;

  }
  identifydevice[dindex] = 1;
  for (int i = 0; i < NumberOfDevices; ++i)
  {
    if(sum[i]==max)
    {
      if (device[i].flag2==1)
      {
	identifydevice[i]=1;


      }
      else
      {
	identifydevice[i]=3;
      }
    }
  }


  printf("\nIdentified Device:" );
 for (i = 0; i < NumberOfDevices; ++i)
  {

    if(identifydevice[i]==1)
      {printf("d%d,",i+1 );

  }
//    else if (identifydevice[i]==3)
  //  {
    //  printf("or d%d ",i+1);
    //}
  }
  printf("\n");
  for (i = 0; i < NumberOfDevices; ++i)
  {
      if(identifydevice[i]==1 || identifydevice[i]==3)
    {  printf("Device %d:%d %d %d  \n",i+1 ,device[i].flag1 ,device[i].flag2 ,device[i].flag3);
      fprintf(runtime,"\nDevice %d : %d %d %d, %d",i+1 ,device[i].flag1 ,device[i].flag2 ,device[i].flag3,arraysize-N);
     }
  }
  printf("\n");
  /*/////////////////////////////
  /printf("\nDevice %d\n",count+1);
  for (i = 0; i < NumberOfDevices; ++i)
  {
    printf("Device %d:%d %d %d  \n",i+1 ,device[i].flag1 ,device[i].flag2 ,device[i].flag3);
    //text1="Device"+(i+1);
  }
/*/  /////////////////////////////

  for (i = 0; i < NumberOfDevices; ++i)
  {
    device[i].flag1 = 0;
    device[i].flag2 = 0;
    device[i].flag3 = 0;
  }
  count++;

}

int Read_Value()    // functions returns 1 if file openeing was sucessful and 0 for error
    {

	    if (ptr_file == NULL)      // If file not available ot the file could not be opened
		return 0;       //  Returns one if the file could not be opened

	    for(int i=0; i<(arraysize+2) ; i++)
	      {

		fscanf(ptr_file, "%d\n", &initialpeaks[i]);
		//printf("%d\n",initialpeaks[i] );

	      if(initialpeaks[i-1]==3 )
	      {
	      printf("reached return5 \n%d\n",count1);
	      return 5;
	      }
    }


	    return 1 ;
}

