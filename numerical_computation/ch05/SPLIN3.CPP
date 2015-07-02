  //5SPLIN3.CPP
  //第三种边界条件的三次样条函数插值、微商与积分
  #include  <iostream>
  #include  <iomanip>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class splin3
  {
  private: 
           int n, m;
		   double  *x, *y, *dy, *ddy, *t, *z, *dz, *ddz, integ;
		   double  *s;
  public:
	       splin3 (int nn, int mm)
		   {
			   n = nn;  m = mm;
               x = new double[n];   //动态分配内存
	           y = new double[n];
	           dy = new double[n];
	           ddy = new double[n];
	           s = new double[n];
	           t = new double[m];
	           z = new double[m];
	           dz = new double[m];
	           ddz = new double[m];
		   }
	       void input ();          //由文件读入n个数据点(x, y)
		                           //以及m个指定插值点t
		   void interp ();    //执行三次样条函数插值、微商与积分
           void output ();         //输出n个数据点及其一阶与二阶导数值
		      //m个插值点t处的函数值及其一阶与二阶导数值与积分值到文件并显示
		   ~splin3 ()
		   {  delete [] x, y, dy, ddy, t, z, dz, ddz, s;  }
  };

  void splin3::input ()      //由文件读入n个数据点(x, y)
		                     //以及m个指定插值点t
  {
	  int  k;
	  char str1[20];
	  cout <<"\n输入文件名:  ";
	  cin >>str1;
	  ifstream  fin (str1);
	  if (!fin)
	  { cout <<"\n不能打开这个文件 " <<str1 <<endl; exit(1); }
      for (k=0; k<n; k++)              //读入n个数据点
	  { fin >>x[k];  fin >>y[k]; }
      for (k=0; k<m; k++) fin >>t[k];    //读入m个插值点
	  fin.close ();
  }

  void splin3::interp ()     //执行三次样条函数插值、微商与积分
  { 
	  int i,j;
      double h0,y0,h1,y1,alpha,beta,u;
      h0=x[n-1]-x[n-2];
      y0=y[n-1]-y[n-2];
      dy[0]=0.0; ddy[0]=0.0; ddy[n-1]=0.0;
      s[0]=1.0; s[n-1]=1.0;
      for (j=1;j<=n-1;j++)
      { 
		  h1=h0; y1=y0;
          h0=x[j]-x[j-1];
          y0=y[j]-y[j-1];
          alpha=h1/(h1+h0);
          beta=3.0*((1.0-alpha)*y1/h1+alpha*y0/h0);
          if (j<n-1)
          { 
			  u=2.0+(1.0-alpha)*dy[j-1];
              dy[j]=-alpha/u;
              s[j]=(alpha-1.0)*s[j-1]/u;
              ddy[j]=(beta-(1.0-alpha)*ddy[j-1])/u;
          }
      }
      for (j=n-2;j>=1;j--)
      { 
		  s[j]=dy[j]*s[j+1]+s[j];
          ddy[j]=dy[j]*ddy[j+1]+ddy[j];
      }
      dy[n-2]=(beta-alpha*ddy[1]-(1.0-alpha)*ddy[n-2])/
            (alpha*s[1]+(1.0-alpha)*s[n-2]+2.0);
      for (j=2;j<=n-1;j++)
          dy[j-2]=s[j-1]*dy[n-2]+ddy[j-1];
      dy[n-1]=dy[0];
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
		  h0=t[j];
          while (h0>=x[n-1]) h0=h0-(x[n-1]-x[0]);
          while (h0<x[0]) h0=h0+(x[n-1]-x[0]);
          i=0;
          while (h0>x[i+1]) i=i+1;
          u=h0;
          h1=(x[i+1]-u)/s[i];
          h0=h1*h1;
          z[j]=(3.0*h0-2.0*h0*h1)*y[i];
          z[j]=z[j]+s[i]*(h0-h0*h1)*dy[i];
          dz[j]=6.0*(h0-h1)*y[i]/s[i];
          dz[j]=dz[j]+(3.0*h0-2.0*h1)*dy[i];
          ddz[j]=(6.0-12.0*h1)*y[i]/(s[i]*s[i]);
          ddz[j]=ddz[j]+(2.0-6.0*h1)*dy[i]/s[i];
          h1=(u-x[i])/s[i];
          h0=h1*h1;
          z[j]=z[j]+(3.0*h0-2.0*h0*h1)*y[i+1];
          z[j]=z[j]-s[i]*(h0-h0*h1)*dy[i+1];
          dz[j]=dz[j]-6.0*(h0-h1)*y[i+1]/s[i];
          dz[j]=dz[j]+(3.0*h0-2.0*h1)*dy[i+1];
          ddz[j]=ddz[j]+(6.0-12.0*h1)*y[i+1]/(s[i]*s[i]);
          ddz[j]=ddz[j]-(2.0-6.0*h1)*dy[i+1]/s[i];
      }
  }

  void splin3::output ()       //输出n个数据点及其一阶与二阶导数值
		//m个插值点t处的函数值及其一阶与二阶导数值与积分值到文件并显示
  {
	  int i;
	  char str2[20];
	  cout <<"\n输出文件名:  ";
	  cin >>str2;
	  ofstream fout (str2);
	  if (!fout)
	  { cout <<"\n不能打开这个文件 " <<str2 <<endl; exit(1); }
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

  void main ()      //主函数
  {
	  int i;
	  splin3  solution(37, 36); 
	  ofstream ft ("splin3.txt");
	  if (!ft)
	  { cout <<"\n不能打开文件 splin3.txt"  <<endl; exit(1); }
      for (i=0; i<=36; i++)
	  {
		  ft <<10*i*3.1415926/180 <<"    " 
			 <<sin (3.1415926*10*i/180) <<endl;
	  }
	  for (i=0; i<=35; i++)
		  ft <<(10*i+5)*3.1415926/180 <<"  ";
	  ft <<endl;
	  ft.close ();
	  solution.input ();           //由文件读入n个数据点(x, y)
		                           //以及m个指定插值点t
	  solution.interp ();    //执行三次样条函数插值、微商与积分
	  solution.output ();    //输出n个数据点及其一阶与二阶导数值
		//m个插值点t处的函数值及其一阶与二阶导数值与积分值到文件并显示
  }

