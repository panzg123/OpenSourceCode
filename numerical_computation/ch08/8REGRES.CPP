  //8REGRES.CPP
  //һԪ���Իع��������
  #include  <iostream>
  #include  <iomanip>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  regres
  {
  private: 
	       int n;
		   double  *x, *y, a, b;
		   double  q, s, p, umax, umin, u;
  public:
	       regres (int nn)
		   {
			   n = nn;
               x = new double[n];   //��̬�����ڴ�
	           y = new double[n];
		   }
	       void  input ();      //���ļ�����n���۲��(x, y)
		   void analysis ();    //ִ��һԪ���Իع����
           void output ();  //����ع�ϵ��a��b�Լ���ͳ�������ļ�����ʾ
		   ~regres ()
		   {   delete [] x, y; }
  };

  void regres::input ()     //���ļ�����n�Լ�n���۲��(x, y)
  {
	  int  i;
	  char str1[20];
	  cout <<"\n�����ļ���:  ";
	  cin >>str1;
	  ifstream  fin (str1);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str1 <<endl; exit(1); }
      for (i=0; i<n; i++)  fin >>x[i];  
      for (i=0; i<n; i++)  fin >>y[i];  
	  fin.close ();
  }

  void regres::analysis()   //ִ��һԪ���Իع����
  { 
	  int i;
      double xx,yy,e,f;
      xx=0.0; yy=0.0;
      for (i=0; i<=n-1; i++)
	  { xx=xx+x[i]/n; yy=yy+y[i]/n; }
      e=0.0; f=0.0;
      for (i=0; i<=n-1; i++)
      { 
		  q=x[i]-xx; e=e+q*q;
          f=f+q*(y[i]-yy);
      }
      a = f/e;  b = yy-a*xx;
      q=0.0; u=0.0; p=0.0;
      umax=0.0; umin=1.0e+30;
      for (i=0; i<=n-1; i++)
      { 
		  s=a*x[i]+b;
          q=q+(y[i]-s)*(y[i]-s);
          p=p+(s-yy)*(s-yy);
          e=fabs(y[i]-s);
          if (e>umax) umax=e;
          if (e<umin) umin=e;
          u=u+e/n;
      }
    s = sqrt(q/n);
  }

  void regres::output () //����ع�ϵ��a��b�Լ���ͳ�������ļ�����ʾ
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
	  cout <<"�ع�ƽ���� = " <<p <<endl;    fout <<p <<endl;
	  cout <<"���ƫ�� = " <<umax <<endl;   fout <<umax <<endl;
	  cout <<"��Сƫ�� = " <<umin <<endl;   fout <<umin <<endl;
	  cout <<"ƫ��ƽ��ֵ = " <<u <<endl;    fout <<u <<endl;
	  fout.close ();
  }

  void main ()      //������
  {
	  regres  s(11);           //��������
	  s.input ();        //���ļ�����n���۲��(x, y)
	  s.analysis ();     //ִ��һԪ���Իع����
	  s.output (); //����ع�ϵ��a��b�Լ���ͳ�������ļ�����ʾ
  }
