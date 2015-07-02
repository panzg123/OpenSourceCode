  //3CGAS.CPP
  //ȫѡ��ԪGauss��ȥ����⸴ϵ��������
  #include  <iostream>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  cgas
  {
  private: 
           int n;
		   double  **ar, **ai, *br, *bi;
  public:
	       cgas (int nn)
		   {
			   int i;
			   n = nn;
	           ar = new double*[n];   //��̬�����ڴ�ռ�
	           for (i=0; i<n; i++) ar[i] = new double[n];
	           ai = new double*[n];
	           for (i=0; i<n; i++) ai[i] = new double[n];
	           br = new double[n];
	           bi = new double[n];
		   }
	       void input ();    //���ļ�����ϵ������A�Լ���������B
		   void c_gauss ();  //ִ��Gauss��ȥ��
           void output ();   //���������ļ�����ʾ
		   ~cgas ()
		   {
			   int i;
			   for (i=0; i<n; i++) { delete [] ar[i]; }
			   delete [] ar;
			   for (i=0; i<n; i++) { delete [] ai[i]; }
			   delete [] ai;
			   delete [] br, bi;
		   }
  };

  void cgas::input ()      //���ļ�����ϵ������A�Լ���������B
  {
	  int  i, j;
	  char str1[20];
	  cout <<"\n�����ļ���:  ";
	  cin >>str1;
	  ifstream  fin (str1);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str1 <<endl; exit(1); }
	  for (i=0; i<n; i++)                       //�������A
		  for (j=0; j<n; j++)  fin >>ar[i][j];
	  for (i=0; i<n; i++)
		  for (j=0; j<n; j++)  fin >>ai[i][j];
	  for (i=0; i<n; i++)  fin >>br[i];           //���볣������B
	  for (i=0; i<n; i++)  fin >>bi[i];
	  fin.close ();
  }

  void cgas::c_gauss ()       //ִ��Gauss��ȥ��
  { 
	  int *js,k,i,j,is;
      double p,q,s,d;
      js = new int[n];
      for (k=0;k<=n-2;k++)
      { 
		  d=0.0;
          for (i=k;i<=n-1;i++)
          for (j=k;j<=n-1;j++)
          { 
              p=ar[i][j]*ar[i][j]+ai[i][j]*ai[i][j];
              if (p>d) {d=p;js[k]=j;is=i;}
          }
          if (d+1.0==1.0)
          { 
			  delete [] js;
			  cout <<"\nϵ���������죡�޽�. " <<endl;
              return;
          }
          if (is!=k)
          { 
			  for (j=k;j<=n-1;j++)
              { 
                  p=ar[k][j]; ar[k][j]=ar[is][j]; ar[is][j]=p;
                  p=ai[k][j]; ai[k][j]=ai[is][j]; ai[is][j]=p;
              }
              p=br[k]; br[k]=br[is]; br[is]=p;
              p=bi[k]; bi[k]=bi[is]; bi[is]=p;
          }
          if (js[k]!=k)
            for (i=0;i<=n-1;i++)
			{ 
                p=ar[i][k]; ar[i][k]=ar[i][js[k]]; ar[i][js[k]]=p;
                p=ai[i][k]; ai[i][k]=ai[i][js[k]]; ai[i][js[k]]=p;
            }
          for (j=k+1;j<=n-1;j++)
          { 
              p=ar[k][j]*ar[k][k]; q=-ai[k][j]*ai[k][k];
              s=(ar[k][k]-ai[k][k])*(ar[k][j]+ai[k][j]);
              ar[k][j]=(p-q)/d; ai[k][j]=(s-p-q)/d;
          }
          p=br[k]*ar[k][k]; q=-bi[k]*ai[k][k];
          s=(ar[k][k]-ai[k][k])*(br[k]+bi[k]);
          br[k]=(p-q)/d; bi[k]=(s-p-q)/d;
          for (i=k+1;i<=n-1;i++)
          { 
              for (j=k+1;j<=n-1;j++)
              { 
                  p=ar[i][k]*ar[k][j]; q=ai[i][k]*ai[k][j];
                  s=(ar[i][k]+ai[i][k])*(ar[k][j]+ai[k][j]);
                  ar[i][j]=ar[i][j]-p+q;
                  ai[i][j]=ai[i][j]-s+p+q;
              }
              p=ar[i][k]*br[k]; q=ai[i][k]*bi[k];
              s=(ar[i][k]+ai[i][k])*(br[k]+bi[k]);
              br[i]=br[i]-p+q; bi[i]=bi[i]-s+p+q;
          }
      }
      d=ar[n-1][n-1]*ar[n-1][n-1]+ai[n-1][n-1]*ai[n-1][n-1];
      if (d+1.0==1.0)
      { 
		  delete [] js;
		  cout <<"\nϵ���������죡�޽�. " <<endl;
          return;
      }
      p=ar[n-1][n-1]*br[n-1]; q=-ai[n-1][n-1]*bi[n-1];
      s=(ar[n-1][n-1]-ai[n-1][n-1])*(br[n-1]+bi[n-1]);
      br[n-1]=(p-q)/d; bi[n-1]=(s-p-q)/d;
      for (i=n-2;i>=0;i--)
      for (j=i+1;j<=n-1;j++)
      { 
          p=ar[i][j]*br[j]; q=ai[i][j]*bi[j];
          s=(ar[i][j]+ai[i][j])*(br[j]+bi[j]);
          br[i]=br[i]-p+q;
          bi[i]=bi[i]-s+p+q;
      }
      js[n-1]=n-1;
      for (k=n-1;k>=0;k--)
        if (js[k]!=k)
        { 
			p=br[k]; br[k]=br[js[k]]; br[js[k]]=p;
            p=bi[k]; bi[k]=bi[js[k]]; bi[js[k]]=p;
        }
      delete [] js;
      return;
  }

  void cgas::output ()       //���������ļ�����ʾ
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
		  fout <<br[i] <<"   ";
		  cout <<br[i] <<"   ";
	  }
	  fout <<endl;  cout <<endl;
	  for (i=0; i<n; i++)
	  {
		  fout <<bi[i] <<"   ";
		  cout <<bi[i] <<"   ";
	  }
	  fout <<endl;  cout <<endl;
	  fout.close ();
  }

  void main ()      //������
  {
	  cgas  c(4);
	  c.input ();         //���ļ�����ϵ������A�Լ���������B
	  c.c_gauss ();       //ִ��Gauss��ȥ��
	  c.output ();        //���������ļ�����ʾ
  }
