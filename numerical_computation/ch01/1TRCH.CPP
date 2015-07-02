  //1TRCH.CPP
  //Toeplitz��������
  #include  <iostream>
  #include  <cmath>
  #include  <fstream>
  using namespace std;
  class  trch
  {
  private: 
           int n;
		   double  *t, *tt, **b;
  public:
	       trch (int  nn)
		   {
			   int  i;
			   n = nn;
	           t = new double[n];    //��̬�����ڴ�ռ�
	           tt = new double[n];
	           b = new double*[n];
	           for (i=0; i<n; i++)  b[i] = new double[n];
		   }
	       void input ();    //���ļ�����T����Ԫ��
		   void inv ();      //ִ������
           void output ();   //���������ļ�
		   ~trch ()              //�ͷ��ڴ�ռ�
		   {  delete [] t, tt; }
  };

  void trch::input ()      //���ļ�����T����Ԫ��
  {
	  int  i;
	  char str1[20];
	  cout <<"\n�����ļ���:  ";
	  cin >>str1;
	  ifstream  fin (str1);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str1 <<endl; exit(1); }
	  for (i=0; i<n; i++)  fin >>t[i];      //��������t
	  for (i=1; i<n; i++)  fin >>tt[i];     //��������tt
	  fin.close ();
  }

  void trch::inv ()           //ִ������
  { int i,j,k;
    double a,s,*c,*r,*p;
    c = new double[n];
    r = new double[n];
    p = new double[n];
    if (fabs(t[0])+1.0==1.0)
    {
		delete [] c, r, p;
        cout <<"\n����ʧ�ܣ�" <<endl;
		exit(1);
    }
    a=t[0]; c[0]=tt[1]/t[0]; r[0]=t[1]/t[0];
    for (k=0; k<=n-3; k++)
    { 
		s=0.0;
        for (j=1; j<=k+1; j++)  s=s+c[k+1-j]*tt[j];
        s=(s-tt[k+2])/a;
	    for (i=0; i<=k; i++)  p[i]=c[i]+s*r[k-i];
        c[k+1]=-s;
        s=0.0;
        for (j=1; j<=k+1; j++)  s=s+r[k+1-j]*t[j];
        s=(s-t[k+2])/a;
        for (i=0; i<=k; i++)
        {
			r[i]=r[i]+s*c[k-i];
            c[k-i]=p[k-i];
        }
        r[k+1]=-s;
	    a=0.0;
        for (j=1; j<=k+2; j++)  a=a+t[j]*c[j-1];
        a=t[0]-a;
        if (fabs(a)+1.0==1.0)
        {
			delete [] c, r, p;
            cout <<"\n����ʧ�ܣ�" <<endl;
			exit(1);
        }
    }
    b[0][0]=1.0/a;
    for (i=0; i<=n-2; i++)
    {
        b[0][i+1]=-r[i]/a; b[i+1][0]=-c[i]/a;
    }
    for (i=0; i<=n-2; i++)
    for (j=0; j<=n-2; j++)
    { 
        b[i+1][j+1]=b[i][j]-c[i]*b[0][j+1];
        b[i+1][j+1]=b[i+1][j+1]+c[n-j-2]*b[0][n-i-1];
    }
    delete [] c, r, p;
  }

  void trch::output ()       //����bд���ļ�
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
			  fout <<"    " <<b[i][j];
			  cout <<"    " <<b[i][j];
		  }
		  fout <<endl;  cout <<endl;
	  }
	  fout.close ();
  }

  void main ()      //������
  {
	  trch  b(6);
	  b.input ();         //���ļ��������T��Ԫ��
	  b.inv ();           //ִ����������
	  b.output ();        //���������ļ�
  }
