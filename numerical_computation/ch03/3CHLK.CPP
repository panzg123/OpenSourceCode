  //3CHLK.CPP
  //��Cholesky�ֽⷨ���Գ�����������
  #include  <iostream>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  chlk
  {
  private: 
           int n, m;
		   double  **a, **d;
  public:
	       chlk (int nn, int mm)
		   {
			   int i;
			   n = nn;  m = mm;
	           a = new double*[n];  //��̬�����ڴ�ռ�
	           for (i=0; i<n; i++) a[i] = new double[n];
	           d = new double*[n];
	           for (i=0; i<n; i++) d[i] = new double[m];
		   }
	       void input ();    //���ļ�����ϵ������A�Լ�m�鳣������D
		   void chol ();      //ִ��Cholesky�ֽ�
           void output ();   //���������ļ�����ʾ
		   ~chlk ()
		   {
			   int i;
			   for (i=0; i<n; i++) { delete [] a[i]; }
			   delete [] a;
			   for (i=0; i<n; i++) { delete [] d[i]; }
			   delete [] d;
		   }
  };

  void chlk::input ()      //���ļ�����ϵ������A�Լ�m�鳣������D
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
	  for (i=0; i<m; i++)                       //����m�鳣������D
		  for (j=0; j<n; j++)  fin >>d[j][i];
	  fin.close ();
  }

  void chlk::chol ()       //ִ��Cholesky�ֽ�
  { 
	  int i,j,k;
      if ((a[0][0]+1.0==1.0)||(a[0][0]<0.0))
      { 
		  cout <<"\n������ʧ�ܣ��޽�. " <<endl;
		  return;
	  }
      a[0][0]=sqrt(a[0][0]);
      for (j=1; j<=n-1; j++)  a[0][j]=a[0][j]/a[0][0];
      for (i=1; i<=n-1; i++)
      { 
          for (j=1; j<=i; j++)
          { 
              a[i][i]=a[i][i]-a[j-1][i]*a[j-1][i];
          }
          if ((a[i][i]+1.0==1.0)||(a[i][i]<0.0))
		  { 
		      cout <<"\n������ʧ�ܣ��޽�. " <<endl;
		      return;
		  }
          a[i][i]=sqrt(a[i][i]);
          if (i!=(n-1))
          { 
			  for (j=i+1; j<=n-1; j++)
              { 
                  for (k=1; k<=i; k++)
                      a[i][j]=a[i][j]-a[k-1][i]*a[k-1][j];
                  a[i][j]=a[i][j]/a[i][i];
              }
          }
      }
      for (j=0; j<=m-1; j++)
      { 
		  d[0][j]=d[0][j]/a[0][0];
          for (i=1; i<=n-1; i++)
          { 
              for (k=1; k<=i; k++)
                  d[i][j]=d[i][j]-a[k-1][i]*d[k-1][j];
              d[i][j]=d[i][j]/a[i][i];
          }
      }
      for (j=0; j<=m-1; j++)
      { 
          d[n-1][j]=d[n-1][j]/a[n-1][n-1];
          for (k=n-1; k>=1; k--)
          { 
              for (i=k; i<=n-1; i++)
              { 
                  d[k-1][j]=d[k-1][j]-a[k-1][i]*d[i][j];
              }
              d[k-1][j]=d[k-1][j]/a[k-1][k-1];
          }
      }
  }

  void chlk::output ()       //���������ļ�����ʾ
  {
	  int  i, j;
	  char str2[20];
	  cout <<"\n����ļ���:  ";
	  cin >>str2;
	  ofstream fout (str2);
	  if (!fout)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
	  fout <<endl;  cout <<endl;
	  for (i=0; i<m; i++)
	  {
		  for (j=0; j<n; j++)
		  {
			  fout <<d[j][i] <<"   ";
		      cout <<d[j][i] <<"   ";
		  }
	      fout <<endl;  cout <<endl;
	  }
	  fout.close ();
  }

  void main ()      //������
  {
	  chlk  c(4, 2);
	  c.input ();         //���ļ�����ϵ������A�Լ�m�鳣������D
	  c.chol ();          //ִ��Cholesky�ֽ�
	  c.output ();        //���������ļ�����ʾ
  }
