  //8LOG1.CPP
  //�����������س���
  #include  <iostream>
  #include  <iomanip>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  log1
  {
  private: 
	       int n;
		   double  *x, *y, t;
		   double  a, b, q, s, umax, umin, u;
  public:
	       log1 (int nn)
		   {
			   n = nn;
               x = new double[n];     //��̬�����ڴ�
	           y = new double[n];
		   }
	       void  input ();      //���ļ�����t�Լ�n�����ݵ�(x, y)
		   void solution ();    //ִ�а�����������
           void output ();  //�����Ϻ�������a,b�Լ���ͳ�������ļ�����ʾ
		   ~log1 ()
		   {   delete [] x, y; }
  };

  void log1::input ()     //���ļ�����t�Լ�n�����ݵ�(x, y)
  {
	  int  i;
	  char str1[20];
	  cout <<"\n�����ļ���:  ";
	  cin >>str1;
	  ifstream  fin (str1);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str1 <<endl; exit(1); }
      fin >>t;                //����t
      for (i=0; i<n; i++)  fin >>x[i];    //����n���Ա���ֵ
      for (i=0; i<n; i++)  fin >>y[i];    //����n�������ֵ
	  fin.close ();
  }

  void log1::solution ()  //ִ�а�����������
  { 
	  int i;
      double xx,yy,dx,dxy;
      xx=0.0; yy=0.0;
      for (i=0; i<=n-1; i++)
      { 
		  xx=xx+x[i]/n; 
          yy=yy+log(y[i])/log(t)/n;
      }
      dx=0.0; dxy=0.0;
      for (i=0; i<=n-1; i++)
      { 
		  q=x[i]-xx; dx=dx+q*q;
          dxy=dxy+q*(log(y[i])/log(t)-yy);
      }
      a=dxy/dx; b=yy-a*xx;
      b=b*log(t); b=exp(b);
      q=0.0; u=0.0; umax=0.0; umin=1.0e+30;
      for (i=0; i<=n-1; i++)
      { 
		  s=a*x[i]*log(t); s=b*exp(s);
          q=q+(y[i]-s)*(y[i]-s);
          dx=fabs(y[i]-s);
          if (dx>umax) umax=dx;
          if (dx<umin) umin=dx;
          u=u+dx/n;
      }
      s=sqrt(q/n);
  }

  void log1::output ()     //�����Ϻ�������a,b�Լ���ͳ�������ļ�����ʾ
  {
	  char str2[20];
	  cout <<"\n����ļ���:  ";
	  cin >>str2;
	  ofstream fout (str2);
	  if (!fout)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
      cout <<"a = " <<a <<"     " <<"b = " <<b <<endl;
	  fout <<a <<"     " <<b <<endl;
	  cout <<"ƫ��ƽ���� = " <<q <<endl;    fout <<q <<endl;
	  cout <<"ƽ����׼ƫ�� = " <<s <<endl;  fout <<s <<endl;
	  cout <<"���ƫ�� = " <<umax <<endl;   fout <<umax <<endl;
	  cout <<"��Сƫ�� = " <<umin <<endl;   fout <<umin <<endl;
	  cout <<"ƫ��ƽ��ֵ = " <<u <<endl;    fout <<u <<endl;
	  fout.close ();
  }

  void main ()      //������
  {
	  log1  s(12);           //��������
	  s.input ();        //���ļ�����t�Լ�n�����ݵ�(x, y)
	  s.solution ();     //ִ�а�����������
	  s.output (); //�����Ϻ�������a,b�Լ���ͳ�������ļ�����ʾ
  }
