  //5SPLIN1.CPP
  //��һ�ֱ߽���������������������ֵ��΢�������
  #include  <iostream>
  #include  <iomanip>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class splin1 
  {
  private: 
           int n, m;
		   double  *x, *y, *dy, *ddy, *t, *z, *dz, *ddz, integ;
		   double  *s;
  public:
	       splin1 (int nn, int mm)
		   {
			   n = nn; m = mm;
               x = new double[n];    //��̬�����ڴ�
	           y = new double[n];
	           dy = new double[n];
	           ddy = new double[n];
	           s = new double[n];
	           t = new double[m];  
	           z = new double[m];
	           dz = new double[m];
	           ddz = new double[m];
		   }
	       void input ();          //���ļ�����n�����ݵ�(x, y)
		                           //�Լ����˵��ϵ�һ�׵���ֵ
		                           //�Լ�m��ָ����ֵ��t
		   void interp ();    //ִ����������������ֵ��΢�������
           void output ();         //���n�����ݵ㼰��һ������׵���ֵ
		      //m����ֵ��t���ĺ���ֵ����һ������׵���ֵ�����ֵ���ļ�����ʾ
		   ~splin1 ()
		   {  delete [] x, y, dy, ddy, t, z, dz, ddz, s;  }
  };

  void splin1::input ()      //���ļ�����n�����ݵ�(x, y)
		                     //�Լ����˵��ϵ�һ�׵���ֵ
		                     //�Լ�m��ָ����ֵ��t
  {
	  int  k;
	  char str1[20];
	  cout <<"\n�����ļ���:  ";
	  cin >>str1;
	  ifstream  fin (str1);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str1 <<endl; exit(1); }
      for (k=0; k<n; k++)              //����n�����ݵ�
	  { fin >>x[k];  fin >>y[k]; }
	  fin >>dy[0] >>dy[n-1];        //�������˵��ϵ�һ�׵���ֵ
      for (k=0; k<m; k++) fin >>t[k];    //����m����ֵ��
	  fin.close ();
  }

  void splin1::interp ()    //ִ����������������ֵ��΢�������
  { 
	  int i,j;
      double h0,h1,alpha,beta;
      s[0]=dy[0]; dy[0]=0.0;
      h0=x[1]-x[0];
      for (j=1;j<=n-2;j++)
      { 
		  h1=x[j+1]-x[j];
          alpha=h0/(h0+h1);
          beta=(1.0-alpha)*(y[j]-y[j-1])/h0;
          beta=3.0*(beta+alpha*(y[j+1]-y[j])/h1);
          dy[j]=-alpha/(2.0+(1.0-alpha)*dy[j-1]);
          s[j]=(beta-(1.0-alpha)*s[j-1]);
          s[j]=s[j]/(2.0+(1.0-alpha)*dy[j-1]);
          h0=h1;
      }
      for (j=n-2;j>=0;j--)
          dy[j]=dy[j]*dy[j+1]+s[j];
      for (j=0;j<=n-2;j++) s[j]=x[j+1]-x[j];
      for (j=0;j<=n-2;j++)
      { 
		  h1=s[j]*s[j];
          ddy[j]=6.0*(y[j+1]-y[j])/h1-2.0*(2.0*dy[j]+dy[j+1])/s[j];
      }
      h1=s[n-2]*s[n-2];
      ddy[n-1]=6.*(y[n-2]-y[n-1])/h1+2.*(2.*dy[n-1]+dy[n-2])/s[n-2];
      integ=0.0;
      for (i=0;i<=n-2;i++)
      { 
		  h1=0.5*s[i]*(y[i]+y[i+1]);
          h1=h1-s[i]*s[i]*s[i]*(ddy[i]+ddy[i+1])/24.0;
          integ=integ+h1;
      }
      for (j=0;j<=m-1;j++)
      { 
		  if (t[j]>=x[n-1]) i=n-2;
          else
          { 
			  i=0;
              while (t[j]>x[i+1]) i=i+1;
          }
          h1=(x[i+1]-t[j])/s[i];
          h0=h1*h1;
          z[j]=(3.0*h0-2.0*h0*h1)*y[i];
          z[j]=z[j]+s[i]*(h0-h0*h1)*dy[i];
          dz[j]=6.0*(h0-h1)*y[i]/s[i];
          dz[j]=dz[j]+(3.0*h0-2.0*h1)*dy[i];
          ddz[j]=(6.0-12.0*h1)*y[i]/(s[i]*s[i]);
          ddz[j]=ddz[j]+(2.0-6.0*h1)*dy[i]/s[i];
          h1=(t[j]-x[i])/s[i];
          h0=h1*h1;
          z[j]=z[j]+(3.0*h0-2.0*h0*h1)*y[i+1];
          z[j]=z[j]-s[i]*(h0-h0*h1)*dy[i+1];
          dz[j]=dz[j]-6.0*(h0-h1)*y[i+1]/s[i];
          dz[j]=dz[j]+(3.0*h0-2.0*h1)*dy[i+1];
          ddz[j]=ddz[j]+(6.0-12.0*h1)*y[i+1]/(s[i]*s[i]);
          ddz[j]=ddz[j]-(2.0-6.0*h1)*dy[i+1]/s[i];
      }
  }

  void splin1::output ()       //���n�����ݵ㼰��һ������׵���ֵ
		 //m����ֵ��t���ĺ���ֵ����һ������׵���ֵ�����ֵ���ļ�����ʾ
  {
	  int i;
	  char str2[20];
	  cout <<"\n����ļ���:  ";
	  cin >>str2;
	  ofstream fout (str2);
	  if (!fout)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
	  cout <<setw(16)<<"x[i]" <<setw(16)<<"y[i]" 
		   <<setw(16)<<"dy[i]" <<setw(16)<<"ddy[i]" <<endl;
	  for (i=0; i<n; i++)
	  {
		  fout <<setw(16)<<x[i]<<setw(16)<<y[i]
			   <<setw(16)<<dy[i]<<setw(16)<<ddy[i]<<endl;
		  cout <<setw(16)<<x[i]<<setw(16)<<y[i]
			   <<setw(16)<<dy[i]<<setw(16)<<ddy[i]<<endl;
	  }
	  fout <<endl;  cout <<endl;
	  cout <<setw(16)<<"t[i]" <<setw(16)<<"z[i]" 
		   <<setw(16)<<"dz[i]" <<setw(16)<<"ddz[i]" <<endl;
	  for (i=0; i<m; i++)
	  {
		  fout <<setw(16)<<t[i]<<setw(16)<<z[i]
			   <<setw(16)<<dz[i]<<setw(16)<<ddz[i]<<endl;
		  cout <<setw(16)<<t[i]<<setw(16)<<z[i]
			   <<setw(16)<<dz[i]<<setw(16)<<ddz[i]<<endl;
	  }
	  fout <<endl;  cout <<endl;
	  fout <<integ <<endl;  cout <<integ <<endl;
	  fout.close ();
  }

  void main ()      //������
  {
	  splin1  solution(12, 8); 
	  solution.input ();           //���ļ�����n�����ݵ�(x, y)
		                           //�Լ����˵��ϵ�һ�׵���ֵ
		                           //�Լ�m��ָ����ֵ��t
	  solution.interp ();    //ִ����������������ֵ��΢�������
	  solution.output ();    //���n�����ݵ㼰��һ������׵���ֵ
		//m����ֵ��t���ĺ���ֵ����һ������׵���ֵ�����ֵ���ļ�����ʾ
  }
