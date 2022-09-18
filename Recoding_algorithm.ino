int i = 0;
const int c=256; //number_of_recorded_samples
const int d=44; //number_of_old_samples
const int n=1; //number_of_harmonics
int h=c/2; //half_of_recorded_samples

//read previous values
float signal_transmitted_read_ahead[d];

//read main data
float signal_transmitted[c+d+8];

//variables for throshold
int temp_transmitted[8];
int avg_old;
int avg_new;
int threshold = 8;

//FFT global variables
float a0[c+d+8];
float an_1[c+d+8];
//float bn_1[c+d+8];

void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop()
{
  Read_ahead();
  Threshold();
  FFT_a0();
  FFT_an_1();
  //FFT_bn_1();
  //print_data();
}

void Read_ahead()
{
  for(i=0;i<d;i++) //read_ahead
  {
    signal_transmitted_read_ahead[i]=analogRead(A0);
  }
  avg_old=0;
  avg_new=0;
  for(i=0;i<8;i++)
  {
    temp_transmitted[i]=analogRead(A0);
  }
  for(i=0;i<4;i++)
  {
    avg_old+=temp_transmitted[i];
  }
  for(i=5;i<8;i++)
  {
    avg_new += temp_transmitted[i];
  }
}

void Threshold()
{
  if(avg_new-avg_old>threshold) //start recording data
  {
    for(i=d+8;i<c+d+8;i++) //main recorded values
    {
      signal_transmitted[i]=analogRead(A0);
    }
    for(i=0;i<d;i++) //copy read ahead values
    {
      signal_transmitted[i]=signal_transmitted_read_ahead[i];
    }
    for(i=0;i<8;i++) //copy main values
    {
      signal_transmitted[i+d]=temp_transmitted[i];
    }
  }
}

void FFT_a0()
{
  float a0_sum=0;
  float sum_of_recroded_data;
  for(i=0;i<c+d+8;i++)
  {
    sum_of_recroded_data=+signal_transmitted[i];
  }
   a0_sum=(1/c)*sum_of_recroded_data;
   for(i=0;i<c+d+8;i++)
  {
    a0[i]=a0_sum;
  } 
}

void FFT_an_1()
{
  int j=0;
  float an_1_for_external_loop;
  for(i=0;i<n;i++)
  {
    calculate_internal_loop_for_an_1();
  }
  for(j=0;j<c+d+8;j++)
  {
   an_1[j]=an_1_for_external_loop*cos((n*(j+1)*3.14)/h);
  }
}
float calculate_internal_loop_for_an_1()
{
  int j=0;
  float an_1_half[c+d+8];
  float sum_an_1_half;
  float an_1_for_external_loop;
  for(j=0;j<c+d+8;j++)
  {
    an_1_half[j]=signal_transmitted[j]*cos((n*(j+1)*3.14)/h);
  }
  sum_an_1_half=0;
  for(j=0;j<c+d+8;j++)
  {
    sum_an_1_half+=an_1_half[j];
  }
  an_1_for_external_loop=(1/h)*sum_an_1_half;
  return an_1_for_external_loop;
}
