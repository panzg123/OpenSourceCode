  //4RMTCL.CPP
  //Monte-Carlo���󷽳�һ��ʵ��
  #include  <iostream>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  rmtcl
  {
  private: 
           int m;
		   double  b, eps, x;
		   double  r;
  public:
	       rmtcl(int mm,double bb, double es, double x0)
		   { m=mm; b=bb; eps=es;  x=x0; }    //�ṩb,m,eps�Լ����ĳ�ֵx
		   void solution ();        //ִ��Monte-Carlo��
		   double rnd ();           //����һ�������
           void output ();         //��������ļ�����ʾ
		   double func (double);   //���㷽����˺���f(x)ֵ
  };

  void rmtcl::solution ()       //ִ��Monte-Carlo��
  { 
	  int k;
      double xx,a,y,x1,y1;
      a=b; k=1; r=1.0; xx=x; y=func (xx);
      while (a>=eps)
      { x1=rnd(); x1=-a+2.0*a*x1;
        x1=xx+x1; y1=func (x1);
        k=k+1;
        if (fabs(y1)>=fabs(y))
          { if (k>m) { k=1; a=a/2.0; }}
        else
          { k=1; xx=x1; y=y1;
            if (fabs(y)<eps)
              { x=xx; return; }
          }
      }
      x=xx; 
  }

  double rmtcl::rnd ()      //����һ�������
  { 
	  int m;
      double s,u,v,p;
      s=65536.0; u=2053.0; v=13849.0;
      m=(int)(r/s); r=r-m*s;
      r=u*r+v; m=(int)(r/s);
      r=r-m*s; p=r/s;
      return(p);
  }

  void rmtcl::output ()       //��������ļ�����ʾ
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

  double rmtcl::func (double x)    //���㷽����˺���f(x)ֵ
  {
	  double y;
	  y = exp(-x*x*x) - sin(x)/cos(x) + 800;
	  return y;
  }

  void main ()      //������
  {
	  rmtcl  root(10, 1.0, 0.00001, 0.5); //���������ṩm,b,eps,x0
	  root.solution ();    //ִ��Monte-Carlo��
	  root.output ();     //��������ļ�����ʾ
  }

//x = 1.56955