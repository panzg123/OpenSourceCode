#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>
#include "locker.h"

template< typename T >
class threadpool
{
public:
    threadpool( int thread_number = 8, int max_requests = 10000 ); /*线程池的线程数目，请求队列的最大请求数*/
    ~threadpool();
    bool append( T* request );          /*向队列中添加请求*/

private:
    static void* worker( void* arg ); //工作线程函数，在pthread_create中调用，此函数必须为static
    void run();                       //线程主体

private:
    int m_thread_number;            //线程数目
    int m_max_requests;             //请求队列的上限
    pthread_t* m_threads;           //线程数组
    std::list< T* > m_workqueue;    //请求队列
    locker m_queuelocker;           //队列互斥锁
    sem m_queuestat;                //是否有任务需要处理
    bool m_stop;                    //是否结束
};
//构造函数，在其中创建一定数目的工作线程
template< typename T >
threadpool< T >::threadpool( int thread_number, int max_requests ) : 
        m_thread_number( thread_number ), m_max_requests( max_requests ), m_stop( false ), m_threads( NULL )
{
    if( ( thread_number <= 0 ) || ( max_requests <= 0 ) )
    {
        throw std::exception();
    }

    m_threads = new pthread_t[ m_thread_number ];
    if( ! m_threads )
    {
        throw std::exception();
    }
	//在此处创建thread_number数目的线程
    for ( int i = 0; i < thread_number; ++i )
    {
        printf( "create the %dth thread\n", i );
		//向pthread_create传递worker+this，通行做法
        if( pthread_create( m_threads + i, NULL, worker, this ) != 0 )
        {
            delete [] m_threads;
            throw std::exception();
        }
		//detach
        if( pthread_detach( m_threads[i] ) )
        {
            delete [] m_threads;
            throw std::exception();
        }
    }
}

template< typename T >
threadpool< T >::~threadpool()
{
    delete [] m_threads;
    m_stop = true;
}
//向请求队列中添加新任务
template< typename T >
bool threadpool< T >::append( T* request )
{
    m_queuelocker.lock();
    if ( m_workqueue.size() > m_max_requests )
    {
        m_queuelocker.unlock();
        return false;
    }
    m_workqueue.push_back( request );
    m_queuelocker.unlock();
    m_queuestat.post();
    return true;
}
/*工作线程函数，在pthread_create中调用*/
template< typename T >
void* threadpool< T >::worker( void* arg )
{
    threadpool* pool = ( threadpool* )arg;
    pool->run();
    return pool;
}
//从任务队列中取任务执行
template< typename T >
void threadpool< T >::run()
{
    while ( ! m_stop )
    {
		//此处，加锁可能影响性能
        m_queuestat.wait();
        m_queuelocker.lock();
        if ( m_workqueue.empty() )
        {
            m_queuelocker.unlock();
            continue;
        }
        T* request = m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();
        if ( ! request )
        {
            continue;
        }
		//这里要求我们的任务的主函数是process
        request->process();
    }
}

#endif
