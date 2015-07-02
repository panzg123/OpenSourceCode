  //3GSDL.CPP
  //Gauss-Seidel������
  #include  <iostream>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  gsdl
  {
  private: 
           int n;
		   double  **a, *b, *x, eps;
  public:
	       gsdl (int nn)
		   {
			   int i;
			   n = nn;
	           a = new double*[n];   //��̬�����ڴ�ռ�
	           for (i=0; i<n; i++) a[i] = new double[n];
	           b = new double[n];
	           x = new double[n];
		   }
	       void input ();    //���ļ�����ϵ������A����������B�Լ�eps
		   void a_gsdl ();    //ִ��Gauss-Seidel������
           void output ();   //���������ļ�����ʾ
		   ~gsdl ()
		   {
			   int i;
			   for (i=0; i<n; i++) { delete [] a[i]; }
			   delete [] a;
			   delete [] b, x;
		   }
  };

  void gsdl::input ()      //���ļ�����ϵ������A����������B�Լ�eps
  {
	  int  i, j;
	  char str1[20];
	  cout <<"\n�����ļ���:  ";
	  cin >>str1;
	  ifstream  fin (str1);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str1 <<endl; exit(1); }
	  for (i=0; i<n; i++)                       //�������A
		  for (j=0; j<n; j++)  fin >>a[i][j];
	  for (i=0; i<n; i++)  fin >>b[i];          //���볣������B
	  fin >>eps;                              //����eps
	  fin.close ();
  }

  void gsdl::a_gsdl ()       //ִ��Gauss-Seidel������
  { 
	  int i,j;
      double p,t,s,q;
      for (i=0; i<=n-1; i++)
      { 
		  p=0.0; x[i]=0.0;
          for (j=0; j<=n-1; j++)
            if (i!=j)   p=p+fabs(a[i][j]);
          if (p>=fabs(a[i][i]))
          {
			  cout <<"\n������ʧ�ܣ�" <<endl;
			  return;
		  }
      }
      p=eps+1.0;
      while (p>=eps)
      { 
		  p=0.0;
          for (i=0; i<=n-1; i++)
          { 
			  t=x[i]; s=0.0;
              for (j=0; j<=n-1; j++)
              if (j!=i) s=s+a[i][j]*x[j];
              x[i]=(b[i]-s)/a[i][i];
              q=fabs(x[i]-t)/(1.0+fabs(x[i]));
              if (q>p) p=q;
          }
      }
  }

  void gsdl::output ()       //���������ļ�����ʾ
  {
	  int  i;
	  char str2[20];
	  cout <<"\n����ļ���:  ";
	  cin >>str2;
	  ofstream fout (str2);
	  if (!fout)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
	  fout <<endl;  cout <<endl;
	  for (i=0; i<n; i++)
	  {
		  fout <<x[i] <<"   ";
		  cout <<x[i] <<"   ";
	  }
	  fout <<endl;  cout <<endl;
	  fout.close ();
  }

  void main ()      //������
  {
	  gsdl  c(4);
	  c.input ();         //���ļ�����ϵ������A����������B�Լ�eps
	  c.a_gsdl ();        //ִ��Gauss-Seidel������
	  c.output ();        //���������ļ�����ʾ
  }
