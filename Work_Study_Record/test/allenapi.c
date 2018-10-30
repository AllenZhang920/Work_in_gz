/**********************
*file：allenapi.c
*time:	2018-10-27
*author:allen
*Provide lots of apis 
*********************/
#include<stdio.h>
#include<time.h>

/*********************
*
*
*
*********************/
void test()
{
	printf("test api\n");
} 

void test_shared()
{
	printf("test share so\n");

}

void allen_get_gmtime()
{
	time_t t;
	struct tm *tm;	
	
	t = time(NULL); 
	tm = gmtime(&t);

	printf("now, the utc fime hour is %d\n",tm->tm_hour);
}

void allen_get_localtime()
{
	time_t t;
	struct tm *tm;	
	
	t = time(NULL); 
	tm = localtime(&t);

	printf("now is hour is %d\n",tm->tm_hour);
}

void allen_get_localfulltime()
{
	time_t t;
	struct tm *tm;	
	char *stime;
	t = time(NULL); 
	tm = localtime(&t);
	printf("now,the local time is  %d:%d\n",tm->tm_hour,tm->tm_min);
	stime = asctime(tm);
	printf("Now the time :%s\n",stime);
}
