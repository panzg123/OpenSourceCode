  //4QRRT.CPP
  //��QR�������������ȫ����
  #include  <iostream>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  qrrt
  {
  private: 
           int n, Max;
		   double  *a, *xr, *xi, **q, eps;
  public:
	       qrrt (int nn)
		   {
			   int i;
			   n = nn;
	           a = new double[n+1];  //��̬�����ڴ�ռ�
	           xr = new double[n];
	           xi = new double[n];
	           q = new double*[n];
	           for (i=0; i<n; i++) q[i] = new double[n];
		   }
	       void input ();       //���ļ��������ʽ����n+1��ϵ��
		   void qr_rt ();       //��QR���������������ȫ����
		   void qr ();         //QR����
		   void output ();     //���ȫ�������ļ�����ʾ
		   ~qrrt ()             //�ͷ��ڴ�ռ�
		   {
			   int i;
			   for (i=0; i<n; i++) { delete [] q[i]; }
			   delete [] q;
			   delete [] a, xr, xi;
		   }
  };

  void qrrt::input ()      //���ļ��������ʽ����n+1��ϵ��
  {
	  int  i;
	  char str1[20];
	  cout <<"\n�����ļ���:  ";
	  cin >>str1;
	  ifstream  fin (str1);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str1 <<endl; exit(1); }
	  for (i=n; i>=0; i--)  fin >>a[i];          //�������ʽϵ��
	  fin.close ();
	  Max = 100;  eps = 0.000001;
  }

  void qrrt::qr_rt ()        //��QR���������������ȫ����
  { 
	  int i,j;
	  for (i=0; i<n; i++)
		  for (j=0; j<n; j++)  q[i][j] = 0.0;
      for (j=0; j<=n-1; j++)
          q[0][j]=-a[n-j-1]/a[n];
      for (i=0; i<=n-2; i++) q[i+1][i]=1.0;
      qr ();
  }
 
  void  qrrt::qr ()         //QR����
  { 
	  int m,it,i,j,k,l;
      double b,c,w,g,xy,p,qq,r,x,s,e,f,z,y;
      it=0; m=n;
      while (m!=0)
      { 
		  l=m-1;
          while ((l>0)&&(fabs(q[l][l-1])>eps*
	            (fabs(q[l-1][l-1])+fabs(q[l][l])))) l=l-1;
          if (l==m-1)
          { 
			  xr[m-1]=q[m-1][m-1]; xi[m-1]=0.0;
              m=m-1; it=0;
          }
          else if (l==m-2)
          { 
			  b=-(q[m-1][m-1]+q[m-2][m-2]);
              c=q[m-1][m-1]*q[m-2][m-2]-q[m-1][m-2]*q[m-2][m-1];
              w=b*b-4.0*c;
              y=sqrt(fabs(w));
              if (w>0.0)
              { 
				  xy=1.0;
                  if (b<0.0) xy=-1.0;
                  xr[m-1]=(-b-xy*y)/2.0;
                  xr[m-2]=c/xr[m-1];
                  xi[m-1]=0.0; xi[m-2]=0.0;
              }
              else
              { 
				  xr[m-1]=-b/2.0; xr[m-2]=xr[m-1];
                  xi[m-1]=y/2.0; xi[m-2]=-xi[m-1];
              }
              m=m-2; it=0;
          }
          else
          { 
			  if (it>=Max)
              { 
				  cout <<"\n������ʧ�ܣ�" <<endl;
                  return;
              }
              it=it+1;
              for (j=l+2; j<=m-1; j++)  q[j][j-2]=0.0;
              for (j=l+3; j<=m-1; j++)  q[j][j-3]=0.0;
              for (k=l; k<=m-2; k++)
              { 
				  if (k!=l)
                  { 
					  p=q[k][k-1]; qq=q[k+1][k-1];
                      r=0.0;
                      if (k!=m-2)  r=q[k+2][k-1];
                  }
                  else
                  { 
					  x=q[m-1][m-1]+q[m-2][m-2];
                      y=q[m-2][m-2]*q[m-1][m-1]-q[m-2][m-1]*q[m-1][m-2];
                      p=q[l][l]*(q[l][l]-x)+q[l][l+1]*q[l+1][l]+y;
                      qq=q[l+1][l]*(q[l][l]+q[l+1][l+1]-x);
                      r=q[l+1][l]*q[l+2][l+1];
                  }
                  if ((fabs(p)+fabs(qq)+fabs(r))!=0.0)
                  { 
					  xy=1.0;
                      if (p<0.0) xy=-1.0;
                      s=xy*sqrt(p*p+qq*qq+r*r);
                      if (k!=l) q[k][k-1]=-s;
                      e=-qq/s; f=-r/s; x=-p/s;
                      y=-x-f*r/(p+s);
                      g=e*r/(p+s);
                      z=-x-e*qq/(p+s);
                      for (j=k; j<=m-1; j++)
                      { 
                          p=x*q[k][j]+e*q[k+1][j];
                          qq=e*q[k][j]+y*q[k+1][j];
                          r=f*q[k][j]+g*q[k+1][j];
                          if (k!=m-2)
                          {
                              p=p+f*q[k+2][j];
                              qq=qq+g*q[k+2][j];
                              r=r+z*q[k+2][j]; q[k+2][j]=r;
                          }
                          q[k+1][j]=qq; q[k][j]=p;
                      }
                      j=k+3;
                      if (j>=m-1) j=m-1;
                      for (i=l; i<=j; i++)
                      { 
                          p=x*q[i][k]+e*q[i][k+1];
                          qq=e*q[i][k]+y*q[i][k+1];
                          r=f*q[i][k]+g*q[i][k+1];
                          if (k!=m-2)
                          { 
                              p=p+f*q[i][k+2];
                              qq=qq+g*q[i][k+2];
                              r=r+z*q[i][k+2]; q[i][k+2]=r;
                          }
                          q[i][k+1]=qq; q[i][k]=p;
                      }
                  }
              }
          }
      }
  }

  void qrrt::output ()  //���ȫ�������ļ�����ʾ
  {
	  int  i;
	  char str2[20];
	  cout <<"\n����ļ���:  ";
	  cin >>str2;
	  ofstream fout (str2);
	  if (!fout)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
	  for (i=0; i<n; i++)
	  {
		  fout <<xr[i] <<"    " <<xi[i] <<endl;
		  cout <<xr[i] <<"   +j  " <<xi[i] <<endl;
	  }
	  fout.close ();
  }

  void main ()      //������
  {
	  qrrt  root(6); 
	  root.input ();             //���ļ��������ʽ����n+1��ϵ��
	  root.qr_rt();               //��QR���������������ȫ����
	  root.output ();           //���ȫ�������ļ�����ʾ
  }

// 4.33376    +j  0
//-1.40246    +j  0
// 1.18397    +j -0.936099
// 1.18397    +j  0.936099
//-0.149622   +j -1.19251
//-0.149622   +j  1.19251