#define TIMEOUT 5000

int timeout = TIMEOUT;
time_t start = time( NULL );
time_t end = time( NULL );
while( 1 )
{
    printf( "the timeout is now %d mill-seconds\n", timeout );
    start = time( NULL );
    int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, timeout );
    if( ( number < 0 ) && ( errno != EINTR ) )
    {
        printf( "epoll failure\n" );
        break;
    }
	/*epoll_wait返回0，说明超时时间到，此时便可以处理定时任务，并重置定时时间*/
    if( number == 0 )
    {
        timeout = TIMEOUT;
        continue;
    }
	/*更新timeout，获得下次epoll_wait调用的超时参数*/
    end = time( NULL );
    timeout -= ( end - start ) * 1000;
    if( timeout <= 0 )
    {
        timeout = TIMEOUT;
    }

    // handle connections
}
