/*
 * Copyright 2021. Heekuck Oh, all rights reserved
 * 이 프로그램은 한양대학교 ERICA 소프트웨어학부 재학생을 위한 교육용으로 제작되었습니다.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 3
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KBLU "\x1B[34m"

int alive = 1;
int arg[3] = {1,2,3};
pthread_mutex_t mutex;

/*
 * 3개의 서로 다른 쓰레드는 서로 빨간색, 파란색, 초록색 글자를 출력한다.
 */

void *print(void *data) {
	while (alive) {
		pthread_mutex_lock(&mutex); // 이 코드 아래있는 지점부터 critical section이라는 말
		/*
		* 임계구역 시작: R, B, G
		*/
		if( *((int *)data) == 1){
			for(int i = 0 ; i < 5 ; i++){
				printf("%sRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n%s", KRED,KNRM);
			}
		}
		else if( *((int *)data) == 2){
			for(int i = 0 ; i < 5 ; i++){
				printf("%sBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\n%s", KBLU,KNRM);
			}			
		}
		else if( *((int *)data) == 3){
			for(int i = 0 ; i < 5 ; i++){
				printf("%sGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG\n%s", KGRN, KNRM);
			}	
		}
		/*
		* 임계구역 종료
		*/
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}


int main() {
	pthread_t thread_id[N];
	pthread_mutex_init(&mutex, NULL);
	struct timespec req;
	/*
	* 세 개의 자식 스레드를 생성한다.
	*/
	for (int i = 0; i < N; i++) {
		pthread_create(&thread_id[i], NULL, print, arg+i);
	}
	/*
	* 스레드가 실행될 동안 0.5 마이크로초 잠잔다.
	*/
	req.tv_sec = 0;
	req.tv_nsec = 5000000L;
	nanosleep(&req, NULL);
	alive = 0;
	/*
	* 자식 스레드가 종료될 때까지 기다린다.
	*/
	for (int i = 0; i < N; i++) {
		pthread_join(thread_id[i], NULL);
	}
	pthread_mutex_destroy(&mutex);
}

