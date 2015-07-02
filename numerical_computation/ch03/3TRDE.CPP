  //3TRDE.CPP
  //������ԽǷ�����
  #include  <iostream>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  trde
  {
  private: 
           int n;
		   double *b, *d;
  public:
	       trde (int nn)
		   {
			   n = nn;
	           b = new double[3*n-2];    //��̬�����ڴ�ռ�
	           d = new double[n];
		   }
	       void input ();  //���ļ����������ԽǾ��������B�Լ���������D
		   void a_trde ();      //ִ��׷�Ϸ�
           void output ();     //���������ļ�����ʾ
		   ~trde ()
		   {
			   delete [] b;
			   delete [] d;
		   }
  };

  void trde::input ()      //���ļ����������ԽǾ��������B�Լ���������D
  {
	  int  i;
	  char str1[20];
	  cout <<"\n�����ļ���:  ";
	  cin >>str1;
	  ifstream  fin (str1);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str1 <<endl; exit(1); }
	  for (i=0; i<3*n-2; i++)  fin >>b[i];  //�������ԽǾ���A�еķ�0Ԫ��
	  for (i=0; i<n; i++)  fin >>d[i];       //���볣������d
	  fin.close ();
  }

  void trde::a_trde ()        //ִ��׷�Ϸ�
  { 
	  int k,j,m;
      double s;
      m= 3*n-2;
      for (k=0;k<=n-2;k++)
      { 
		  j=3*k; s=b[j];
          if (fabs(s)+1.0==1.0)
          { 
			  cout <<"\n������ʧ�ܣ��޽�. " <<endl;
			  return;
		  }
          b[j+1]=b[j+1]/s;
          d[k]=d[k]/s;
          b[j+3]=b[j+3]-b[j+2]*b[j+1];
          d[k+1]=d[k+1]-b[j+2]*d[k];
      }
      s=b[3*n-3];
      if (fabs(s)+1.0==1.0)
      { 
		  cout <<"\n������ʧ�ܣ��޽�. " <<endl;
		  return;
	  }
      d[n-1]=d[n-1]/s;
      for (k=n-2;k>=0;k--)
          d[k]=d[k]-b[3*k+1]*d[k+1];
  }

  void trde::output ()       //���������ļ�����ʾ
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
		  fout <<d[i] <<"   ";
		  cout <<d[i] <<"   ";
	  }
	  fout <<endl;  cout <<endl;
	  fout.close ();
  }

  void main ()      //������
  {
	  trde  c(5); 
	  c.input ();  //���ļ����������ԽǾ��������B�Լ���������D
	  c.a_trde ();        //ִ��׷�Ϸ�
	  c.output ();        //���������ļ�����ʾ
  }
