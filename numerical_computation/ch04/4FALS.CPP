  //4FALS.CPP
  //��λ���󷽳̸�
  #include  <iostream>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  fals
  {
  private: 
           
		   double  x_low, x_high, x, eps;
  public:
	       fals(double a, double b, double es)//�ṩ�����������뾫��Ҫ��
		    { x_low = a;  x_high = b; eps = es; }
		   void false_root ();        //ִ����λ��
           void output ();         //��������ļ�����ʾ
		   double func (double);   //���㷽����˺���f(x)ֵ
  };

  void fals::false_root ()     //ִ����λ��
  {
	  int M;
	  double f0, f1, f;
	  M = 0;
	  f0 = func (x_low);  f1 = func (x_high);
	  if (f0*f1 > 0)
	  {
		  cout <<"\n���������ڿ����޸���" <<endl;
		  exit(1);
	  }
	  do
	  {
		  M = M + 1;
		  x = (x_low*f1 - x_high*f0)/(f1 - f0);
          f = func (x);
		  if (f*f0 < 0) { x_high = x; f1 = f; }
		  else  { x_low = x; f0 = f; }
	  } while (fabs(f) >= eps);
      cout <<"\n�����Ĵ���Ϊ��" <<M <<endl;
  }

  void fals::output ()       //��������ļ�����ʾ
  {
	  char str2[20];
	  cout <<"\n����ļ���:  ";
	  cin >>str2;
	  ofstream fout (str2);
	  if (!fout)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
	  fout <<endl <<x <<endl;
	  cout <<endl <<"x = " <<x <<endl;
	  fout.close ();
  }

  double fals::func(double x)    //���㷽����˺���f(x)ֵ
  {
	  double y;
	  y = x*x*x - 2*x*x + x - 2;
	  return y;
  }

  void main ()      //������
  {
	  fals  root(1.0, 3.0, 0.000001); 
	                    //�������󣬲����������������뾫��Ҫ��
	  root.false_root ();    //ִ����λ��
	  root.output ();     //��������ļ�����ʾ
  }

//x = 2
//����24��