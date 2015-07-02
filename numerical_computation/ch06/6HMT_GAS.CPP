  //6HMT_GAS.CPP
  //Hermite-Gauss�����
  #include  <iostream>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  hmt_gas
  {
  private: 
		   double  integ;
  public:
		   void integration ();    //ִ��Hermite-Gauss�����
           void output ();         //�������ֵ���ļ�����ʾ
		   double func (double);   //���㱻������ֵ
  };

  void hmt_gas::integration ()     //ִ��Hermite-Gauss�����
  { 
	  int i;
      static double t[5]={-2.02018200,-0.95857190,
                           0.0,0.95857190,2.02018200};
      static double c[5]={1.181469599,0.9865791417,
               0.9453089237,0.9865791417,1.181469599};
      integ=0.0;
      for (i=0; i<=4; i++)
      { 
		  integ = integ + c[i]*func (t[i]); 
	  }
  }

  void hmt_gas::output ()       //�������ֵ���ļ�����ʾ
  {
	  char str2[20];
	  cout <<"\n����ļ���:  ";
	  cin >>str2;
	  ofstream fout (str2);
	  if (!fout)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
	  fout <<integ <<endl;  cout <<"����ֵ = " <<integ <<endl;
	  fout.close ();
  }

  double hmt_gas::func (double x)
  {  return x*x*exp(-x*x); }

  void main ()      //������
  {
	  hmt_gas  solution;     //��������
	  solution.integration ();    //ִ��Hermite-Gauss�����
	  solution.output ();    //�������ֵ���ļ�����ʾ
  }
