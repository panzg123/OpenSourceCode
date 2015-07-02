  //11LLIST.CPP
  //˳����Ĳ���������
  #include  <iostream>
  #include  <iomanip>
  #include  <fstream>
  #include  <cmath>
  #include  <string>
  using namespace std;
  template <typename T>
  class  llist
  {
  private: 
	       int n;    //������
		   T  *L;    //˳���
  public:
	       llist(int nn)
		   {  n = nn;   L = new T[n];  }
	       void  input ();      //���ļ�����Ԫ��
		   void  bubble ();     //ð������
		   void  bub (T *, int);  //ִ��ð������
		   void  quick ();      //��������
           void  qck (T *, int);  //��������ݹ�
           void  split (T *, int, int*);  //��������ָ�
		   void  shell ();      //Shell����
		   void  heap ();       //������
		   void  sift (int,int); //�������еĵ���
		   int ibsh (T);        //�Էֲ���
           void  output ();     //���˳����е�Ԫ�ص��ļ�����ʾ
		   ~llist ()
		   {  delete [] L;  }
  };

  template <typename T>
  void llist<T>::input ()     //���ļ�����Ԫ��
  {
	  int  i;
	  char str1[20];
	  cout <<"\n�����ļ���:  ";
	  cin >>str1;
	  ifstream  fin (str1);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str1 <<endl; exit(1); }
      for (i=0; i<n; i++)  fin >>L[i];   //����Ԫ��
	  fin.close ();
  }

  template <typename T>
  void  llist<T>::bubble ()    //ð������
  { bub (L, n);  return;  }

  template <typename T>
  void  llist<T>::bub (T *p, int n)    //ִ��ð������
  {
	  int m,k,j,i;
      T d;
      k=0; m=n-1;
      while (k<m)
      { 
		  j=m-1; m=0;
          for (i=k; i<=j; i++)
              if (p[i]>p[i+1])
			  { d=p[i]; p[i]=p[i+1]; p[i+1]=d; m=i; }
          j=k+1; k=0;
          for (i=m; i>=j; i--)
              if (p[i-1]>p[i])
			  { d=p[i]; p[i]=p[i-1]; p[i-1]=d; k=i; }
      }
      return;
  }

  template <typename T>
  void  llist<T>::quick ()      //��������
  { qck(L, n);  return; }

  template <typename T>  
  void  llist<T>::qck (T *p, int n) //��������ݹ�
  {
	  int m,i0,*i;
      T *s;
      i=&i0;
      if (n>10)
      { 
		  split(p,n,i);
          m=i0;
          qck(p,m);
	      s=p+(i0+1);
          m=n-(i0+1);
          qck(s,m);
      }
      else bub(p,n);
      return;
  }

  template <typename T>
  void llist<T>::split (T *p, int n, int *m)  //��������ָ�
  { 
	  int i,j,k,l;
      T t;
      i=0; j=n-1;
      k=(i+j)/2;
      if ((p[i]>=p[j])&&(p[j]>=p[k])) l=j;
      else if ((p[i]>=p[k])&&(p[k]>=p[j])) l=k;
      else l=i;
      t=p[l]; p[l]=p[i];
      while (i!=j)
      { 
		  while ((i<j)&&(p[j]>=t)) j=j-1;
          if (i<j)
          { 
			  p[i]=p[j]; i=i+1;
              while ((i<j)&&(p[i]<=t)) i=i+1;
              if (i<j)
              { p[j]=p[i]; j=j-1; }
          }
      }
      p[i]=t; *m=i;
      return;
  }

  template <typename T>  
  void  llist<T>::shell ()     //Shell����
  {
	  int k,j,i;
      T t;
      k=n/2;
      while (k>0)
      { 
		  for (j=k; j<=n-1; j++)
          { 
			  t=L[j]; i=j-k;
              while ((i>=0)&&(L[i]>t))
              { L[i+k]=L[i]; i=i-k; }
              L[i+k]=t;
          }
          k=k/2;
      }
      return;
  }

  template <typename T>  
  void  llist<T>::heap ()       //������
  {
	  int i,mm;
      T t;
      mm=n/2;
      for (i=mm-1; i>=0; i--) sift(i,n-1);
      for (i=n-1; i>=1; i--)
      { 
		  t=L[0]; L[0]=L[i]; L[i]=t;
          sift(0,i-1);
      }
      return;
  }

  template <typename T>
  void llist<T>::sift(int i, int n)  //�������еĵ���
  { 
	  int j;
      T t;
      t=L[i]; j=2*(i+1)-1;
      while (j<=n)
      { if ((j<n)&&(L[j]<L[j+1])) j=j+1;
        if (t<L[j])  { L[i]=L[j]; i=j; j=2*(i+1)-1;}
        else j=n+1;
      }
      L[i]=t;
      return;
  }

  template <typename T>
  void llist<T>::output () //���˳����е�Ԫ�ص��ļ�����ʾ
  {
	  int i, k;
	  char str2[20];
	  cout <<"\n����ļ���:  ";
	  cin >>str2;
	  ofstream fout (str2);
	  if (!fout)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
	  k = 0;
	  while (k<n)
	  {
		  i=0;
		  while ((i < 8) && (k < n))
		  {
			  cout <<L[k] <<"    ";   fout <<L[k] <<"    ";
			  k = k + 1;   i = i + 1;
		  }
		  cout <<endl;   fout <<endl;
	  }
	  fout.close ();
  }

  template <typename T>
  int llist<T>::ibsh (T x)       //�Էֲ���
  {
	  int i,j,k;
      i=1; j=n;
      while (i<=j)
      { 
		  k=(i+j)/2;
          if (L[k-1] == x)  return k-1;
          if (L[k-1] > x) j=k-1;
          else i=k+1;
      }
      return (-1);
  }

  void main ()      //������
  {
	  llist<int>  p(12);       //��������
      llist<int>  p1(100);
	  llist<double>  p2(100);
	  llist<char>    p3(22);
	  llist<string>  p4(10);
	  p.input ();              //���������p123.txt
      p.output ();
      cout <<p.ibsh (19) <<endl;
	  cout <<p.ibsh (34) <<endl;
	  p1.input ();             //����100���������llist_i.txt
	  p2.input ();             //����100�����ʵ��list_r.txt
	  p3.input ();             //����22���ַ�llist_c.txt
	  p4.input ();             //����10���ַ���llist_h.txt
//ð������
	  p1.bubble ();   //��������
	  p1.output ();
	  p2.bubble ();   //ʵ������
	  p2.output ();
	  p3.bubble ();   //�ַ�����
	  p3.output ();
	  p4.bubble ();   //�ַ�������
	  p4.output ();
//��������
	  p1.quick ();     //��������
	  p1.output ();
	  p2.quick ();     //ʵ������
	  p2.output ();
	  p3.quick ();     //�ַ�����
	  p3.output ();
	  p4.quick ();     //�ַ�������
	  p4.output ();
//Shell����
	  p1.shell ();     //��������
	  p1.output ();
	  p2.shell ();     //ʵ������
	  p2.output ();
	  p3.shell ();     //�ַ�����
	  p3.output ();
	  p4.shell ();     //�ַ�������
	  p4.output ();
//������
	  p1.heap ();       //��������
	  p1.output ();
	  p2.heap ();       //ʵ������
	  p2.output ();
	  p3.heap ();       //�ַ�����
	  p3.output ();
	  p4.heap ();       //�ַ�������
	  p4.output ();
  }

