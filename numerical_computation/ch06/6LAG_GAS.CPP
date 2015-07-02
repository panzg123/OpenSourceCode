  //6LAG_GAS.CPP
  //Laguerre-Gauss�����
  #include  <iostream>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  lag_gas
  {
  private: 
		   double  integ;
  public:
		   void integration ();    //ִ��Laguerre-Gauss�����
           void output ();         //�������ֵ���ļ�����ʾ
		   double func (double);   //���㱻������ֵ
  };

  void lag_gas::integration ()   //ִ��Laguerre-Gauss�����
  { 
	  int i;
      static double t[5]={0.26355990,1.41340290,
                3.59642600,7.08580990,12.64080000};
      static double c[5]={0.6790941054,1.638487956,
                 2.769426772,4.315944000,7.104896230};
      integ = 0.0;
      for (i=0; i<=4; i++)
      { 
	      integ = integ + c[i]*func (t[i]); 
	  }
  }

  void lag_gas::output ()       //�������ֵ���ļ�����ʾ
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

  double lag_gas::func (double x)
  {  return x*exp(-x); }

  void main ()      //������
  {
	  lag_gas  solution;     //��������
	  solution.integration ();    //ִ��Laguerre-Gauss�����
	  solution.output ();    //�������ֵ���ļ�����ʾ
  }
