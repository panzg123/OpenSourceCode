  //7EULER1.CPP
  //�䲽��Euler����
  #include  <iostream>
  #include  <iomanip>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  euler1
  {
  private: 
	       int n, k;
		   double  t, h, eps, *y, **z;
		   double  *d, *a, *b, *c;
  public:
	       euler1 (int nn, int kk)
		   {
			   int i;
			   n = nn; k = kk;
               y = new double[n];     //��̬�����ڴ�
	           d = new double[n];
	           a = new double[n];
	           b = new double[n];
	           c = new double[n];
	           z = new double*[n];
	           for (i=0; i<n; i++) { z[i] = new double[k]; }
		   }
	       void  input ();         //���ļ��������ݳ�Աt,h,eps
		                      //�Լ�n��δ֪��������ʼ��t���ĺ���ֵ
		   void solution ();    //ִ�б䲽��Euler����
           void output ();   //���k�����ֵ���
		                     //��δ֪����ֵ���ļ�����ʾ
		   void func (double,double*,double*);   
		                         //����΢�ַ������и������Ҷ˺���ֵ
		   ~euler1 ()
		   { 
			   int i;
			   for (i=0; i<n; i++) { delete [] z[i]; }
			   delete [] z;
			   delete [] y, d, a, b, c;
		   }
  };

  void euler1::input ()     //���ļ��������ݳ�Աt,h,eps
		                      //�Լ�n��δ֪��������ʼ��t���ĺ���ֵ
  {
	  int  i;
	  char str1[20];
	  cout <<"\n�����ļ���:  ";
	  cin >>str1;
	  ifstream  fin (str1);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str1 <<endl; exit(1); }
      fin >>t >>h >>eps;        //����t,h,eps
      for (i=0; i<n; i++)  fin >>y[i];  
	                            //����n��δ֪��������ʼ��t���ĺ���ֵ	  
	  fin.close ();
  }

  void euler1::solution ()  //ִ�б䲽��Euler����
  { 
	  int i,j,m, kk;
      double hh,p,x,q, tt;
      tt = t;
	  for (kk=0; kk<k; kk++)
	  {
		  hh=h; m=1; p=1.0+eps;
          for (i=0; i<=n-1; i++) a[i]=y[i];
          while (p>=eps)
		  { 
		      for (i=0; i<=n-1; i++)
			  { b[i]=y[i]; y[i]=a[i]; }
              for (j=0; j<=m-1; j++)
			  { 
			      for (i=0; i<=n-1; i++) c[i]=y[i];
                  x=t+j*hh;
                  func (x,y,d);
                  for (i=0; i<=n-1; i++)
                      y[i]=c[i]+hh*d[i];
                  x=t+(j+1)*hh;
                  func (x,y,d);
                  for (i=0; i<=n-1; i++)
                      d[i]=c[i]+hh*d[i];
                  for (i=0; i<=n-1; i++)
                      y[i]=(y[i]+d[i])/2.0;
			  }
              p=0.0;
              for (i=0; i<=n-1; i++)
			  { 
			      q=fabs(y[i]-b[i]);
                  if (q>p) p=q;
			  }
              hh=hh/2.0; m=m+m;
		  }
          for (i=0; i<n; i++)  z[i][kk] = y[i];
	      t = t + h;
	  }
	  t = tt;
  }

  void euler1::output () //���k�����ֵ��ϵ�δ֪����ֵ���ļ�����ʾ
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

  void euler1::func (double t, double y[], double d[])
  {
	  d[0] = y[1];
	  d[1] = -y[0];
	  d[2] = -y[2];
  }

  void main ()      //������
  {
	  euler1  s(3, 10);           //��������
	  s.input ();        //���ļ��������ݳ�Աt,h,eps
		                 //�Լ�n��δ֪��������ʼ��t���ĺ���ֵ
	  s.solution ();    //ִ�б䲽��Euler����
	  s.output ();    //���k�����ֵ��ϵ�δ֪����ֵ���ļ�����ʾ
  }
