  //11STR_LIST.CPP
  //�ṹ���Ĳ���������
  #include  <iostream>
  #include  <iomanip>
  #include  <fstream>
  #include  <cmath>
  #include  <string>
//  #define  SORT_KEY  name       //���������ùؼ��ֳ�Ա
//  #define  SERCH_KEY  name       //��������ùؼ��ֳ�Ա
  #define  SORT_KEY  age       //���������ùؼ��ֳ�Ա
  #define  SERCH_KEY  age       //��������ùؼ��ֳ�Ա
  using namespace std;
  template <typename T, typename KT>
  class  str_list
  {
  private: 
	       int n;    //������
		   T  *L;    //�ṹ��
  public:
	       str_list(int nn)
		   {  n = nn;   L = new T[n];  }
	       void  input ();      //���ļ�����ṹ��Ԫ��
		   void  heap ();       //�ṹ��������
		   void  sift(int,int);  //�������еĵ���
		   void ibsh (KT);        //�ṹ���Էֲ���
		   void serch (KT);       //�ṹ��˳�����
           void  output ();     //����ṹ���е�Ԫ�ص��ļ�����ʾ
		   void  PRINT(int,ofstream&); //����ṹ���е�һ��Ԫ�ص��ļ�����ʾ
		   ~str_list ()
		   {  delete [] L;  }
  };

  template <typename T, typename KT>
  void str_list<T,KT>::input ()     //���ļ�����ṹ��Ԫ��
  {
	  int  i;
	  char str1[20];
	  cout <<"\n�����ļ���:  ";
	  cin >>str1;
	  ifstream  fin (str1);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str1 <<endl; exit(1); }
      for (i=0; i<n; i++)   //����ṹ��Ԫ�صĸ���Ա
	  {
		  fin >>L[i].num;
		  fin >>L[i].name;
		  fin >>L[i].sex;
		  fin >>L[i].age;
		  fin >>L[i].score;
	  }
	  fin.close ();
  }

  template <typename T, typename KT>  
  void  str_list<T,KT>::heap ()       //�ṹ��������
  {
	  int i,mm;
      T t;
      mm=n/2;
      for (i=mm-1; i>=0; i--)  sift (i,n-1);
      for (i=n-1; i>=1; i--)
      { 
		  t=L[0]; L[0]=L[i]; L[i]=t;
          sift (0,i-1);
      }
      return;
  }

  template <typename T, typename KT>
  void str_list<T,KT>::sift (int i, int n)  //�������еĵ���
  { 
	  int j;
      T t;
      t=L[i]; j=2*(i+1)-1;
      while (j<=n)
      { if ((j<n)&&(L[j].SORT_KEY<L[j+1].SORT_KEY)) j=j+1;
        if (t.SORT_KEY<L[j].SORT_KEY)  
		{ L[i]=L[j]; i=j; j=2*(i+1)-1; }
        else j=n+1;
      }
      L[i]=t;
      return;
  }

  template <typename T, typename KT>
  void str_list<T,KT>::output () //����ṹ���е�Ԫ�ص��ļ�����ʾ
  {
	  int k;
	  char str2[20];
	  cout <<"\n����ļ���:  ";
	  cin >>str2;
	  ofstream fout (str2);
	  if (!fout)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
	  for (k=0; k<n; k++)  PRINT (k, fout); //����ṹ����һ��Ԫ��
	  fout.close ();
  }

  template <typename T, typename KT>
  void str_list<T,KT>::ibsh (KT x)       //�ṹ���Էֲ���
  {
	  char str2[20];
	  int i,j,k;
      i=1; j=n;
      while (i<=j)
      { 
		  k=(i+j)/2;
          if (L[k-1].SERCH_KEY == x)
		  {
			  cout <<"\n����ļ���:  ";
	          cin >>str2;
	          ofstream fout (str2);
	          if (!fout)
			  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
			  j = k - 1;
			  while ((j<n)&&(L[j].SERCH_KEY == x))
			  {
                  PRINT (j, fout);      //����ṹ����һ��Ԫ��
				  j = j + 1;
			  }
	          fout.close ();
			  return;
		  }
          if (L[k-1].SERCH_KEY > x) j=k-1;
          else i=k+1;
      }
      cout <<"�ýṹ��û�������¼��" <<endl;
  }

  template <typename T, typename KT>
  void str_list<T,KT>::serch (KT x)       //�ṹ��˳�����
  {
	  int k;
	  char str2[20];
	  cout <<"\n����ļ���:  ";
	  cin >>str2;
	  ofstream fout (str2);
	  if (!fout)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
	  for (k=0; k<n; k++)
	  {
		  if (L[k].SERCH_KEY == x) PRINT (k, fout);
	  }
	  fout.close ();	  
  }

  template <typename T, typename KT>
  void  str_list<T,KT>::PRINT (int k, ofstream& fout) 
                         //����ṹ���е�һ��Ԫ�ص��ļ�����ʾ
  {
	  fout <<setw(5) <<L[k].num;     //������Ѿ��򿪵��ļ�
	  fout <<setw(10) <<L[k].name;
	  fout <<setw(4) <<L[k].sex;
	  fout <<setw(5) <<L[k].age;
	  fout <<setw(8) <<L[k].score <<endl;
	  cout <<setw(5) <<L[k].num;     //��ʾ
	  cout <<setw(10) <<L[k].name;
	  cout <<setw(4) <<L[k].sex;
	  cout <<setw(5) <<L[k].age;
	  cout <<setw(8) <<L[k].score <<endl;
	  return;
  }

  struct Student
  {
	  int   num;
	  string  name;
	  char  sex;
	  int   age;
	  double  score;
  };
  void main ()      //������
  {
//	  str_list<struct Student, string>  p(10);       //��������
//	  p.input ();
//	  p.serch ("Mao");
//	  p.heap ();
//	  p.output ();
//	  p.ibsh ("Zhao");
	  str_list<struct Student, int>  p(10);       //��������
	  p.input ();
	  p.serch (18);
	  p.heap ();
	  p.output ();
	  p.ibsh (19);
  }

