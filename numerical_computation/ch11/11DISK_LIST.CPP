  //11DISK_LIST.CPP
  //�����ı��ļ��Ĳ���������
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
  template <typename T>
  class  disk_list
  {
  private: 
	       long int n;    //��¼����
  public:
	       disk_list(long int nn)  {  n = nn; }
		   void  quick ();       //��������
           void  disk(fstream&, long int, long int);//�ݹ��������		   
		   void  ibsh (char *);        //�Էֲ���
		   void  serch (char*);       //˳�����
           void  output ();       //��ʾ�ļ�����
		   void  PRINT(T);        //��ʾһ����¼
  };

  template <typename T>
  void  disk_list<T>::quick ()       //��������
  {
	  char str2[20];
	  cout <<"\n���������ļ���:  ";
	  cin >>str2;
	  fstream f (str2);
	  if (!f)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
      disk(f, 0, n-1);
  }

  template <typename T>
  void disk_list<T>::disk(fstream& f, long int k, long int m)
  { 
	  long int i,j,kk;
	  T w, u, t;
      i=k; j=m; kk=(long)(i+j)/2;
      f.seekg(kk*sizeof(T));
      f.read((char *)&w, sizeof(T));
      t = w;
      do
	  { 
		  f.seekg(i*sizeof(T));
		  f.read((char *)&w, sizeof(T));		
          while ((strcmp(w.SORT_KEY,t.SORT_KEY)<0)&&(i<=j))
		  { 
			  i=i+1; 
			  f.seekg(i*sizeof(T));
		      f.read((char *)&w, sizeof(T));
		  }
		  f.seekg(j*sizeof(T));
		  f.read((char *)&w, sizeof(T));
          while ((strcmp(w.SORT_KEY,t.SORT_KEY)>0)&&(i<=j))
		  { 
			  j=j-1; 
		      f.seekg(j*sizeof(T));
		      f.read((char *)&w, sizeof(T));
		  }
          if (i<=j)
		  {
		      f.seekg(i*sizeof(T));
		      f.read((char *)&w, sizeof(T));
		      f.seekg(j*sizeof(T));
		      f.read((char *)&u, sizeof(T));

		      f.seekg(i*sizeof(T));
		      f.write((char *)&u, sizeof(T));
		      f.seekg(j*sizeof(T));
		      f.write((char *)&w, sizeof(T));
			 
			  i=i+1; j=j-1;
		  }
	  }while (i<=j);
      if (k<j) disk(f,k,j);
      if (i<m) disk(f,i,m);
      return;
  }

  template <typename T>
  void disk_list<T>::output () //��ʾ�ļ�����
  {
	  int k;
	  T  x;
	  char str2[20];
	  cout <<"\n�����ļ���:  ";
	  cin >>str2;
	  ifstream fin (str2);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
	  for (k=0; k<n; k++)
	  {
		  fin.read((char *)&x, sizeof(x));
		  PRINT (x);
	  }
	  fin.close ();
  }

  template <typename T>
  void disk_list<T>::ibsh (char *x)       //�Էֲ���
  {
	  T w;
	  char str2[20];
	  int i,j,k;
	  cout <<"\n����Էֲ����ļ���:  ";
	  cin >>str2;
	  ifstream fin (str2);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }	  
	  i=1; j=n;
      while (i<=j)
      { 
		  k=(i+j)/2;
		  fin.seekg((k-1)*sizeof(T));
		  fin.read((char *)&w, sizeof(T));

          if (strcmp(w.SERCH_KEY,x) == 0)
		  {
			  j = k - 1;
			  while ((j<n)&&(strcmp(w.SERCH_KEY,x) == 0))
			  {
                  PRINT (w);      //���һ����¼
				  j = j + 1;
		          fin.read((char *)&w, sizeof(T));
			  }
	          fin.close ();
			  return;
		  }
          if (strcmp(w.SERCH_KEY,x) > 0) j=k-1;
          else i=k+1;
      }
      cout <<"�ô����ļ���û�������¼��" <<endl;
  }

  template <typename T>
  void disk_list<T>::serch (char *key)   //˳�����
  {
	  int k;
	  T  x;
	  char str2[20];
	  cout <<"\n����˳������ļ���:  ";
	  cin >>str2;
	  ifstream fin (str2);
	  if (!fin)
	  { cout <<"\n���ܴ�����ļ� " <<str2 <<endl; exit(1); }
	  for (k=0; k<n; k++)
	  {
		  fin.read((char *)&x, sizeof(x));
		  if (strcmp(x.SERCH_KEY,key) == 0)  PRINT (x);
	  }
	  fin.close ();
  }

  template <typename T>
  void  disk_list<T>::PRINT (T x)   //��ʾһ����¼
  {
	  cout <<x.num;
	  cout <<x.name;
	  cout <<x.sex;
	  cout <<x.age;
	  cout <<x.score <<endl;
  }

  struct Student
  {
	  char  num[6];
	  char  name[9];
	  char  sex[4];
	  char  age[5];
	  char  score[8];
  };
  void main ()      //������
  {
/*
	  int i;
	  struct Student x[10] ={ 
	  {"101  ", "Zhang   ", "M  ", "19  ", "095.6  "},
	  {"102  ", "Wang    ", "F  ", "18  ", "092.4  "},
	  {"103  ", "Zhao    ", "M  ", "19  ", "085.7  "},
	  {"104  ", "Li      ", "M  ", "20  ", "096.3  "},
	  {"105  ", "Gou     ", "M  ", "19  ", "090.2  "},
	  {"106  ", "Lin     ", "M  ", "18  ", "091.5  "},
	  {"107  ", "Ma      ", "F  ", "17  ", "098.7  "},
	  {"108  ", "Zhen    ", "M  ", "21  ", "090.1  "},
	  {"109  ", "Xu      ", "M  ", "19  ", "100.0  "},
	  {"110  ", "Mao     ", "F  ", "18  ", "094.9  "}};
	  
	          ofstream fout ("disk_list.dat");
	          if (!fout)
			  { cout <<"\n���ܴ�����ļ� " <<endl; exit(1); }
      for (i=0; i<10; i++)
		  fout.write((char *)&x[i], sizeof(struct Student));
*/

	  disk_list<struct Student>  p(10);       //��������
	  p.output ();              //��ʾ�����ļ�����
	  p.serch ("18  ");         //˳����ҳ�ԱageΪ"18  "�ļ�¼
	  p.quick ();               //�Դ����ļ�����Աage���п�������
	  p.output ();              //��ʾ�����Ĵ����ļ�����
	  p.ibsh ("19  ");          //�Էֲ��ҳ�ԱageΪ"19  "�ļ�¼
  }

