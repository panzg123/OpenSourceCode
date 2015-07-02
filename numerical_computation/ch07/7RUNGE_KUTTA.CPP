  //7RUNGE_KUTTA.CPP
  //������Runge-Kutta��
  #include  <iostream>
  #include  <iomanip>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  runge_kutta
  {
  private: 
	       int n, k;
		   double  t, h, *y, **z;
		   double  *d, *b, *c;
  public:
	       runge_kutta (int nn, int kk)
		   {
			   int i;
			   n = nn; k = kk;
               y = new double[n];     //��̬�����ڴ�
	           d = new double[n];
	           b = new double[n];
	           c = new double[n];
	           z = new double*[n];
	           for (i=0; i<n; i++) { z[i] = new double[k]; }
		   }
	       void  input ();         //���ļ��������ݳ�Աt,h
		                      //�Լ�n��δ֪��������ʼ��t���ĺ���ֵ
		   void solution ();    //ִ�ж�����Runge-Kutta��
           void output ();        //���k�����ֵ���
		                          //��δ֪����ֵ���ļ�����ʾ
		   void func (double,double*,double*);   
		                         //����΢�ַ������и������Ҷ˺���ֵ
		   ~runge_kutta ()
		   { 
			   int i;
			   for (i=0; i<n; i++) { delete [] z[i]; }
			   delete [] z;
			   delete [] y, d, b, c;
		   }
  };

  void runge_kutta::input ()     //���ļ��������ݳ�Աt,h
		                      //�Լ�n��δ֪��������ʼ��t���ĺ���ֵ
  {
	  int  i;
	  char str1[20];
	  cout <<"\n�����ļ���:  ";
	  cin >>str1;
	  ifstream  fin (str1);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str1 <<endl; exit(1); }
      fin >>t >>h;        //����t,h
      for (i=0; i<n; i++)  fin >>y[i];  
	                            //����n��δ֪��������ʼ��t���ĺ���ֵ	  
	  fin.close ();
  }

  void runge_kutta::solution ()  //ִ�ж�����Runge-Kutta��
  { 
	  int i,j,kk;
      double a[4],tt;
      a[0]=h/2.0; a[1]=a[0];
      a[2]=h; a[3]=h;
	  tt = t;
      for (kk=0; kk<k; kk++)
      { 
		  func (tt,y,d);
          for (i=0; i<=n-1; i++)
		  { b[i]=y[i];  c[i] = y[i]; }
          for (j=0; j<=2; j++)
          { 
			  for (i=0; i<=n-1; i++)
              { 
				  y[i]=c[i]+a[j]*d[i];
                  b[i]=b[i]+a[j+1]*d[i]/3.0;
              }
              func (tt+a[j],y,d);
          }
          for (i=0; i<=n-1; i++)
		  {
              y[i]=b[i]+h*d[i]/6.0;
              z[i][kk]=y[i];
		  }
          tt=tt+h;
      }
  }

  void runge_kutta::output ()   //���k�����ֵ���
	                      //��δ֪����ֵ���ļ�����ʾ
  {
	  int i, j;
	  char str2[20];
	  cout <<"\n����ļ���:  ";
	  cin >>str2;
	  ofstream fout (str2);
	  if (!fout)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
      for (i=0; i<k; i++)
	  {
		  cout <<"t = " <<t+(i+1)*h <<endl;
		  for (j=0; j<n; j++)
		  {
			  fout <<z[j][i] <<"   ";
			  cout <<"y(" <<j <<")=" <<setw(12) <<z[j][i] <<"  ";
		  }
		  fout <<endl;  cout <<endl;
	  }
	  fout.close ();
  }

  void runge_kutta::func (double t, double y[], double d[])
  {
	  d[0] = y[1];
	  d[1] = -y[0];
	  d[2] = -y[2];
  }

  void main ()      //������
  {
	  runge_kutta  s(3, 10);           //��������
	  s.input ();        //���ļ��������ݳ�Աt,h
		                 //�Լ�n��δ֪��������ʼ��t���ĺ���ֵ
	  s.solution ();    //ִ�ж�����Runge-Kutta��
	  s.output (); //���k�����ֵ��ϵ�δ֪����ֵ���ļ�����ʾ
  }
