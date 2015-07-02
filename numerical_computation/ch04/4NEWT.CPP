  //4NEWT.CPP
  //Newton���󷽳�ʵ��
  #include  <iostream>
  #include  <fstream>
  #include  <cmath>
  using namespace std;
  class  newt
  {
  private: 
           int Max;
		   double  x, eps;
		   double  y, dy;
  public:
	       newt(double x0, double es,int m)//�ṩ���ĳ�ֵ������Ҫ��������������
		   { x = x0; eps = es; Max = m; }
		   void newt_root ();      //ִ��Newton��
           void output ();         //��������ļ�����ʾ
		   void func (double);   //���㷽����˺���f(x)�ĺ���ֵ�뵼��ֵ
  };

  void newt::newt_root ()     //ִ��Newton��
  { 
	  int k;
      double d,p,x0,x1;
      k=0; x0=x;
      func (x0);
      d=eps+1.0;
      while ((d>=eps)&&(k!=Max))
	  {	
		  if (fabs(dy)+1.0==1.0)
		  { 
			  cout <<"\nf(x)�ĵ���Ϊ0��" <<endl;
			  x = x0;
			  return;
		  }
          x1=x0-y/dy;
          func (x1);
          d=fabs(x1-x0); p=fabs(y);
          if (p>d) d=p;
          x0=x1; k = k + 1;
	  }
	  if (k == Max)
	  	  cout <<"\n������������" <<endl;
      x=x1;
  }

  void newt::output ()       //��������ļ�����ʾ
  {
	  char str2[20];
	  cout <<"\n����ļ���:  ";
	  cin >>str2;
	  ofstream fout (str2);
	  if (!fout)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
	  fout <<endl <<x <<endl;
	  cout <<endl <<"x = " <<x <<endl;
	  fout.close ();
  }

  void newt::func(double x)    //���㷽����˺���f(x)�ĺ���ֵ�뵼��ֵ
  {
	  y = x*x*(x - 1.0) - 1.0;
	  dy = 3.0*x*x - 2.0*x;
  }

  void main ()      //������
  {
	  newt  root(1.5,0.000001,60); 
	                //�������󣬲������ĳ�ֵ������Ҫ��������������
	  root.newt_root ();    //ִ��Newton��
	  root.output ();     //��������ļ�����ʾ
  }

//x = 1.46557