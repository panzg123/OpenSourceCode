  //6LEG_GAS.CPP
  //Legendre_Gauss�����
  #include  <iostream>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  leg_gas 
  {
  private: 
		   double  a, b, eps, integ;
  public:
	       leg_gas (double aa, double bb, double es)
			                           //˳���ṩa,b,epsֵ�Ĺ��캯��
           { a = aa; b = bb; eps = es; }
		   void integration ();    //ִ��Legendre_Gauss�����
           void output ();         //�������ֵ���ļ�����ʾ
		   double func (double);   //���㱻������ֵ
  };

  void leg_gas::integration ()    //ִ��Legendre_Gauss�����
  { 
	  int m,i,j;
      double s,p,ep,h,aa,bb,w,x,g;
      static double t[5]={-0.9061798459,-0.5384693101,0.0,
                         0.5384693101,0.9061798459};
      static double c[5]={0.2369268851,0.4786286705,0.5688888889,
                        0.4786286705,0.2369268851};
      m=1;
      h=b-a; s=fabs(0.001*h);
      p=1.0e+35; ep=eps+1.0;
      while ((ep>=eps)&&(fabs(h)>s))
      { 
		  g=0.0;
          for (i=1;i<=m;i++)
          { 
			  aa=a+(i-1.0)*h; bb=a+i*h;
              w=0.0;
              for (j=0;j<=4;j++)
              { 
				  x=((bb-aa)*t[j]+(bb+aa))/2.0;
                  w = w + func(x)*c[j];
              }
              g=g+w;
          }
          g=g*h/2.0;
          ep=fabs(g-p)/(1.0+fabs(g));
          p=g; m=m+1; h=(b-a)/m;
      }
      integ = g;
  }

  void leg_gas::output ()       //�������ֵ���ļ�����ʾ
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

  double leg_gas::func (double x)
  {  return (x*x+sin(x)); }

  void main ()      //������
  {
	  leg_gas  solution(2.5, 8.4, 0.000001); //��������˳���ṩa, b, epsֵ
	  solution.integration ();    //ִ��Legendre_Gauss�����
	  solution.output ();    //�������ֵ���ļ�����ʾ
  }
