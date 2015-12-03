#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include "locker.h"

class http_conn
{
public:
    static const int FILENAME_LEN = 200;
	//读缓冲区大小
    static const int READ_BUFFER_SIZE = 2048;
    //写缓冲区大小
	static const int WRITE_BUFFER_SIZE = 1024;
	/*HTTP请求方法，目前暂时支持GET*/
    enum METHOD { GET = 0, POST, HEAD, PUT, DELETE, TRACE, OPTIONS, CONNECT, PATCH };
    //三种状态：读取请求行、读取头部、读取主体内容
	enum CHECK_STATE { CHECK_STATE_REQUESTLINE = 0, CHECK_STATE_HEADER, CHECK_STATE_CONTENT };
    /*服务器处理HTTP可能的结果*/
	enum HTTP_CODE { NO_REQUEST, GET_REQUEST, BAD_REQUEST, NO_RESOURCE, FORBIDDEN_REQUEST, FILE_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION };
    /*行的读取状态*/
	enum LINE_STATUS { LINE_OK = 0, LINE_BAD, LINE_OPEN };

public:
    http_conn(){}
    ~http_conn(){}

public:
    void init( int sockfd, const sockaddr_in& addr );  /*初始化新接受的连接*/
    void close_conn( bool real_close = true );         /*关闭连接*/
    void process();                                    /*处理客户请求*/
    bool read();                                       /*非阻塞读操作*/
    bool write();                                      /*非阻塞写操作*/

private:
    void init();                                       /*初始化连接*/
    HTTP_CODE process_read();                          /*解析HTTP请求*/
    bool process_write( HTTP_CODE ret );               /*填充HTTP应答*/

	/*下面这一组函数被process_read调用来分析HTTP请求*/
    HTTP_CODE parse_request_line( char* text );
    HTTP_CODE parse_headers( char* text );
    HTTP_CODE parse_content( char* text );
    HTTP_CODE do_request();
    char* get_line() { return m_read_buf + m_start_line; }
    LINE_STATUS parse_line();

	/*下面这一组函数被process_write调用来填充HTTP应答*/
    void unmap();
    bool add_response( const char* format, ... );
    bool add_content( const char* content );
    bool add_status_line( int status, const char* title );
    bool add_headers( int content_length );
    bool add_content_length( int content_length );
    bool add_linger();
    bool add_blank_line();

public:
    static int m_epollfd;      /*所有类的对象的socket上的事件都被注册到同一个epoll内核事件表中，因此需要将epoll文件描述符设置为静态*/
    static int m_user_count;   /*统计用户数量，需要设置为静态*/

private:
    int m_sockfd;             /*连接的socket*/
    sockaddr_in m_address;    /*对方的地址*/

    char m_read_buf[ READ_BUFFER_SIZE ];          /*读缓冲区*/
    int m_read_idx;                               /*指向buffer中客户数据尾部的下一个字节*/
    int m_checked_idx;                            /*指向buffer中当前正在分析的字节*/
    int m_start_line;                             /*当前正在解析行的起始位置*/
    char m_write_buf[ WRITE_BUFFER_SIZE ];        /*写缓冲区*/
    int m_write_idx;                              /*写缓冲区中带发送的字节数*/

    CHECK_STATE m_check_state;                    /*主状态机当前的状态*/
    METHOD m_method;                              /*请求方法*/

    char m_real_file[ FILENAME_LEN ];/*客户请求目标文件的完整路径，相当于doc_root+m_url*/
    char* m_url;                     /*客户请求的目标文件名*/
    char* m_version;                 /*htpp协议版本号*/
    char* m_host;                    /*主机*/
    int m_content_length;            /*主体消息长度*/
    bool m_linger;                   /*是否保持连接*/

    char* m_file_address;            /*客户请求的目标文件被mmap到内存中的位置*/
    struct stat m_file_stat;         /*目标的文件状态*/
	/*下面两个成员用来进行分散读写*/
    struct iovec m_iv[2];
    int m_iv_count;
};

#endif
