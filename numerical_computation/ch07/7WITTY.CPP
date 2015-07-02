  //7WITTY.CPP
  //Witty����
  #include  <iostream>
  #include  <iomanip>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  witty
  {
  private: 
	       int n, k;
		   double  t, h, *y, **z;
		   double  *d, *q, *a;
  public:
	       witty (int nn, int kk)
		   {
			   int i;
			   n = nn; k = kk;
               y = new double[n];       //��̬�����ڴ�
	           d = new double[n];
	           q = new double[n];
	           a = new double[n];
	           z = new double*[n];
	           for (i=0; i<n; i++) { z[i] = new double[k]; }
		   }
	       void  input ();         //���ļ��������ݳ�Աt,h
		                      //�Լ�n��δ֪��������ʼ��t���ĺ���ֵ
		   void solution ();    //ִ��Witty����
           void output ();        //���k�����ֵ���
		                          //��δ֪����ֵ���ļ�����ʾ
		   void func (double,double*,double*);   
		                         //����΢�ַ������и������Ҷ˺���ֵ
		   ~witty ()
		   { 
			   int i;
			   for (i=0; i<n; i++) { delete [] z[i]; }
			   delete [] z;
			   delete [] y, d, q, a;
		   }
  };

  void witty::input ()     //���ļ��������ݳ�Աt,h
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

  void witty::solution ()  //ִ��Witty����
  { 
	  int i,j;
      double x;
//      for (i=0; i<=n-1; i++) z[i*k]=y[i];
      func (t,y,d);
      for (j=0; j<k; j++)
      { 
		  for (i=0; i<=n-1; i++)
              q[i]=y[i]+h*d[i]/2.0;
          x=t+(j-0.5)*h;
          func (x,q,a);
          for (i=0; i<=n-1; i++)
          { 
			  d[i]=2.0*a[i]-d[i];
              z[i][j]=y[i]+h*a[i];
			  y[i] = z[i][j];
          }
      }
  }

  void witty::output ()   //���k�����ֵ���
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

  void witty::func (double t, double y[], double d[])
  {
	  d[0] = y[1];
	  d[1] = -y[0];
	  d[2] = -y[2];
  }

  void main ()      //������
  {
	  witty  s(3, 10);           //��������
	  s.input ();        //���ļ��������ݳ�Աt,h
		                 //�Լ�n��δ֪��������ʼ��t���ĺ���ֵ
	  s.solution ();    //ִ��Witty����
	  s.output (); //���k�����ֵ��ϵ�δ֪����ֵ���ļ�����ʾ
  }
