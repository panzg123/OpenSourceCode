  //6PART.CPP
  //���񵴺��������
  #include  <iostream>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  part 
  {
  private: 
	       int n, m;
		   double  a, b, *fa, *fb, integ_cos, integ_sin;
  public:
	       part (int nn)
		   {
			   n = nn;
               fa = new double[n+1]; //��̬�����ڴ�
	           fb = new double[n+1];
		   }
	       void input ();          //���ļ�����a, b�Լ���Ƶ��m
		                //f(x)�ڻ���������˵�x=a���ĺ���ֵ����׵���ֵ��
		                //f(x)�ڻ��������Ҷ˵�x=b���ĺ���ֵ����׵���ֵ
		   void integration ();    //ִ�зֲ����ַ�
           void output ();         //�������ֵ���ļ�����ʾ
		   ~part ()
		   { delete  fa, fb; }
  };

  void part::input ()          //���ļ�����a, b�Լ���Ƶ��m
		                //f(x)�ڻ���������˵�x=a���ĺ���ֵ����׵���ֵ��
		                //f(x)�ڻ��������Ҷ˵�x=b���ĺ���ֵ����׵���ֵ
  {
	  int  k;
	  char str1[20];
	  cout <<"\n�����ļ���:  ";
	  cin >>str1;
	  ifstream  fin (str1);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str1 <<endl; exit(1); }
	  fin >>a  >>b;                      //����a,b
      fin >>m;                          //����m
      for (k=0; k<=n; k++)  fin >>fa[k]; 
	               //����f(x)�ڻ���������˵�x=a���ĺ���ֵ����׵���ֵ
      for (k=0; k<=n; k++)  fin >>fb[k]; 
	               //����f(x)�ڻ��������Ҷ˵�x=b���ĺ���ֵ����׵���ֵ
	  fin.close ();
  }

  void part::integration ()         //ִ�зֲ����ַ�
  { 
	  int mm,k,j;
      double sa[4],sb[4],ca[4],cb[4],sma,smb,cma,cmb;
      sma=sin(m*a); smb=sin(m*b);
      cma=cos(m*a); cmb=cos(m*b);
      sa[0]=sma; sa[1]=cma; sa[2]=-sma; sa[3]=-cma;
      sb[0]=smb; sb[1]=cmb; sb[2]=-smb; sb[3]=-cmb;
      ca[0]=cma; ca[1]=-sma; ca[2]=-cma; ca[3]=sma;
      cb[0]=cmb; cb[1]=-smb; cb[2]=-cmb; cb[3]=smb;
      integ_cos = 0.0;  integ_sin = 0.0;
      mm=1;
      for (k=0;k<=n-1;k++)
      { 
		  j=k;
          while (j>=4) j=j-4;
          mm=mm*m;
          integ_cos = integ_cos+(fb[k]*sb[j]-fa[k]*sa[j])/(1.0*mm);
          integ_sin = integ_sin+(fb[k]*cb[j]-fa[k]*ca[j])/(1.0*mm);
      }
      integ_sin = -integ_sin;
  }

  void part::output ()       //�������ֵ���ļ�����ʾ
  {
	  char str2[20];
	  cout <<"\n����ļ���:  ";
	  cin >>str2;
	  ofstream fout (str2);
	  if (!fout)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
	  fout <<integ_cos <<endl;  fout <<integ_sin <<endl;
	  cout <<"integration_cos = " <<integ_cos <<endl;
	  cout <<"integration_sin = " <<integ_sin <<endl;
	  fout.close ();
  }

  void main ()      //������
  {
	  part  solution(3); 
	  solution.input ();         //���ļ�����a, b�Լ���Ƶ��m
		                //f(x)�ڻ���������˵�x=a���ĺ���ֵ����׵���ֵ��
		                //f(x)�ڻ��������Ҷ˵�x=b���ĺ���ֵ����׵���ֵ
	  solution.integration ();    //ִ�зֲ������
	  solution.output ();    //�������ֵ���ļ�����ʾ
  }
