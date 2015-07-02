  //6SIMP2.CPP
  //�䲽��Simpson���ػ��ַ�
  #include  <iostream>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  simp2
  {
  private: 
		   double  a, b, eps, integ;
  public:
	       simp2 (double aa,double bb,double es)  
			                        //˳���ṩa,b,epsֵ�Ĺ��캯��
		   { a = aa; b = bb; eps = es; }
		   void integration ();    //ִ�б䲽��Simpson���ػ��ַ�
		   double simp1(double);  //һά���ֺ���
           void output ();         //�������ֵ���ļ�����ʾ
		   double func (double,double);   //���㱻������ֵ
		   double lower(double);          //�����ڲ��������ֵy0
		   double upper(double);          //�����ڲ��������ֵy1
  };

  void simp2::integration ()  //ִ�б䲽��Simpson���ػ��ַ�
  { 
      int n,j;
      double h,d,s1,s2,t1,x,t2,g,s,s0,ep;
      n=1; h=0.5*(b-a);
      d=fabs((b-a)*1.0e-06);
      s1=simp1(a); s2=simp1(b);
      t1=h*(s1+s2);
      s0=1.0e+35; ep=1.0+eps;
      while (((ep>=eps)&&(fabs(h)>d))||(n<16))
      { 
		  x=a-h; t2=0.5*t1;
          for (j=1;j<=n;j++)
          { 
			  x=x+2.0*h;
              g=simp1(x);
              t2=t2+h*g;
          }
          s=(4.0*t2-t1)/3.0;
          ep=fabs(s-s0)/(1.0+fabs(s));
          n=n+n; s0=s; t1=t2; h=h*0.5;
      }
      integ = s;
  }

  double simp2::simp1(double x)   //һά���ֺ���
  { 
	  int n,i;
      double y0,y1,h,d,t1,yy,t2,g,ep,g0;
      n=1;
	  y0 = lower(x);
	  y1 = upper(x);
      h=0.5*(y1-y0);
      d=fabs(h*2.0e-06);
      t1=h*(func (x,y0)+func (x,y1));
      ep=1.0+eps; g0=1.0e+35;
      while (((ep>=eps)&&(fabs(h)>d))||(n<16))
      { 
		  yy=y0-h;
          t2=0.5*t1;
          for (i=1;i<=n;i++)
          { 
			  yy=yy+2.0*h;
              t2=t2+h*func(x,yy);
          }
          g=(4.0*t2-t1)/3.0;
          ep=fabs(g-g0)/(1.0+fabs(g));
          n=n+n; g0=g; t1=t2; h=0.5*h;
      }
      return(g);
  }

  void simp2::output ()       //�������ֵ���ļ�����ʾ
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

  double simp2::func (double x, double y)    //���㱻������ֵ
  {  return exp(x*x+y*y); }

  double simp2::lower(double x)   //�����ڲ��������ֵy0
  {  return (-sqrt(1-x*x));  }

  double simp2::upper(double x)   //�����ڲ��������ֵy1
  {  return (sqrt(1-x*x));  }

  void main ()      //������
  {
	  simp2  solution (0.0, 1.0, 0.0001); 
	                        //��������˳���ṩa,b,eps��ֵ
	  solution.integration ();    //ִ�б䲽��Simpson���ػ��ַ�
	  solution.output ();    //�������ֵ���ļ�����ʾ
  }
