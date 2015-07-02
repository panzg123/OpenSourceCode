  //7GILL.CPP
  //�䲽��Gill����
  #include  <iostream>
  #include  <iomanip>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  gill
  {
  private: 
	       int n, k;
		   double  t, h, eps, *y, **z;
		   double  *d, *q, *u, *v, *g;
  public:
	       gill (int nn, int kk)
		   {
			   int i;
			   n = nn; k = kk;
               y = new double[n];      //��̬�����ڴ�
	           d = new double[n];
	           q = new double[n];
	           u = new double[n];
	           v = new double[n];
	           g = new double[n];
	           z = new double*[n];
	           for (i=0; i<n; i++) { z[i] = new double[k]; }
		   }
	       void  input ();         //���ļ��������ݳ�Աt,h,eps
		                      //�Լ�n��δ֪��������ʼ��t���ĺ���ֵ
		   void solution ();    //ִ�б䲽��Gill����
           void output ();        //���k�����ֵ���
		                          //��δ֪����ֵ���ļ�����ʾ
		   void func (double,double*,double*);   
		                         //����΢�ַ������и������Ҷ˺���ֵ
		   ~gill ()
		   { 
			   int i;
			   for (i=0; i<n; i++) { delete [] z[i]; }
			   delete [] z;
			   delete [] y, d, q, u, v, g;
		   }
  };

  void gill::input ()     //���ļ��������ݳ�Աt,h,eps
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
	  for (i=0; i<n; i++)  q[i] = 0.0;
	  fin.close ();
  }

  void gill::solution ()    //ִ�б䲽��Gill����
  { 
	  int i,j,kk,m,ii;
      double x,p,hh,r,s,t0,dt,qq,tt;
      static double a[4]={0.5,0.29289321881,
                        1.7071067812,0.166666667};
      static double b[4]={2.0,1.0,1.0,2.0};
      static double c[4],e[4]={0.5,0.5,1.0,1.0};
	  tt = t;
      for (kk=0; kk<k; kk++)
	  {
		  t = tt + kk*h;
          for (i=0; i<=2; i++) c[i]=a[i];
          c[3]=0.5;
          x=t; p=1.0+eps; hh=h; m=1;
          for (j=0; j<=n-1; j++) u[j]=y[j];
          while (p>=eps)
		  { 
			  for (j=0; j<=n-1; j++)
			  { v[j]=y[j]; y[j]=u[j]; g[j]=q[j]; }
              dt=h/m; t=x;
              for (i=0; i<=m-1; i++)
			  { 
				  func (t,y,d);
                  for (ii=0; ii<=3; ii++)
				  { 
					  for (j=0; j<=n-1; j++)  d[j]=d[j]*hh;
                      for (j=0; j<=n-1; j++)
					  { 
						  r=(a[ii]*(d[j]-b[ii]*g[j])+y[j])-y[j];
                          y[j]=y[j]+r;
                          s=g[j]+3.0*r;
                          g[j]=s-c[ii]*d[j];
					  }
                      t0=t+e[ii]*hh;
                      func(t0,y,d);
				  }
                  t=t+dt;
			  }
              p=0.0;
              for (j=0; j<=n-1; j++)
			  { 
				  qq=fabs(y[j]-v[j]);
                  if (qq>p) p=qq;
			  }
              hh=hh/2.0; m=m+m;
		  }
          for (j=0; j<=n-1; j++) 
		  {
		      z[j][kk] = y[j];
		      q[j]=g[j];
		  }
	  }
	  t = tt;
  }

  void gill::output ()   //���k�����ֵ���
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

  void gill::func (double t, double y[], double d[])
  {
	  d[0] = y[1];
	  d[1] = -y[0];
	  d[2] = -y[2];
  }

  void main ()      //������
  {
	  gill  s(3, 10);           //��������
	  s.input ();        //���ļ��������ݳ�Աt,h,eps
		                 //�Լ�n��δ֪��������ʼ��t���ĺ���ֵ
	  s.solution ();    //ִ�б䲽��Gill����
	  s.output (); //���k�����ֵ��ϵ�δ֪����ֵ���ļ�����ʾ
  }
