  //1CHOL.CPP
  //Cholesky�ֽ���������ʽֵ
  #include  <iostream>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  chol
  {
  private: 
           int n;
		   double  **a, det;
  public:
	       chol (int nn)
		   {
			   int i;
			   n = nn;
	           a = new double*[n];  //��̬�����ڴ�ռ�
	           for (i=0; i<n; i++) a[i] = new double[n];
		   }
	       void input ();       //���ļ��������A��Ԫ��
		   void a_chol ();      //ִ��Cholesky�ֽ���������ʽֵ
           void output ();      //���L����������ʽֵ���ļ�����ʾ
		   ~chol ()
		   {
			   int i;
			   for (i=0; i<n; i++) { delete [] a[i]; }
			   delete [] a;
		   }
  };

  void chol::input ()      //���ļ��������A��Ԫ��
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
	  fin.close ();
  }

  void chol::a_chol ()           //ִ��Cholesky�ֽ���������ʽֵ
  { 
	  int i,j,k;
      double d;
      if ((a[0][0]+1.0==1.0)||(a[0][0]<0.0))
      { 
		  cout <<"\n�ֽ�ʧ�ܣ�" <<endl;
		  exit(1);
	  }
      a[0][0]=sqrt(a[0][0]);
      d=a[0][0];
      for (i=1; i<=n-1; i++)  a[i][0]=a[i][0]/a[0][0];
      for (j=1; j<=n-1; j++)
      { 
          for (k=0; k<=j-1; k++)
              a[j][j]=a[j][j]-a[j][k]*a[j][k];
          if ((a[j][j]+1.0==1.0)||(a[j][j]<0.0))
		  { 
		      cout <<"\n�ֽ�ʧ�ܣ�" <<endl;
		      exit(1);
		  }
          a[j][j]=sqrt(a[j][j]);
          d=d*a[j][j];
          for (i=j+1; i<=n-1; i++)
          { 
              for (k=0; k<=j-1; k++)
                 a[i][j]=a[i][j]-a[i][k]*a[j][k];
              a[i][j]=a[i][j]/a[j][j];
          }
      }
      det=d*d;
      for (i=0; i<=n-2; i++)
      for (j=i+1; j<=n-1; j++)  a[i][j]=0.0;
  }

  void chol::output ()       //���L����������ʽֵ���ļ�����ʾ
  {
	  int  i, j;
	  char str2[20];
	  cout <<"\n����ļ���:  ";
	  cin >>str2;
	  ofstream fout (str2);
	  if (!fout)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
	  for (i=0; i<n; i++)
	  {
		  for (j=0; j<n; j++)
		  {
			  fout <<"    " <<a[i][j];
			  cout <<"    " <<a[i][j];
		  }
		  fout <<endl;  cout <<endl;
	  }
	  fout <<det <<endl; cout <<"det = " <<det <<endl;
	  fout.close ();
  }

  void main ()      //������
  {
	  chol  c(4); 
	  c.input ();            //���ļ��������A��Ԫ��
	  c.a_chol ();           //Cholesky�ֽ���������ʽֵ
	  c.output ();        //���L����������ʽֵ���ļ�����ʾ
  }
