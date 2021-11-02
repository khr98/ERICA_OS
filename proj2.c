/*
 * Copyright 2021. Heekuck Oh, all rights reserved
 * 이 프로그램은 한양대학교 ERICA 소프트웨어학부 재학생을 위한 교육용으로 제작되었습니다.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct //thread에 값을 넘겨주기 위한 구조체
{
    int row; //가로 위치
    int column; //세로 위치
    int k; //서브그리드 번호:vaild[2][k]에 쓰임
} location_t;

/*
 * 기본 스도쿠 퍼즐
 */
int sudoku[9][9] = {{6, 3, 9, 8, 4, 1, 2, 7, 5}, {7, 2, 4, 9, 5, 3, 1, 6, 8}, {1, 8, 5, 7, 2, 6, 3, 9, 4}, {2, 5, 6, 1, 3, 7, 4, 8, 9}, {4, 9, 1, 5, 8, 2, 6, 3, 7}, {8, 7, 3, 4, 6, 9, 5, 2, 1}, {5, 4, 2, 3, 9, 8, 7, 1, 6}, {3, 1, 8, 6, 7, 5, 9, 4, 2}, {9, 6, 7, 2, 1, 4, 8, 5, 3}};

/*
 * valid[0][0], valid[0][1], ..., valid[0][8]: 각 행이 올바르면 1, 아니면 0
 * valid[1][0], valid[1][1], ..., valid[1][8]: 각 열이 올바르면 1, 아니면 0
 * valid[2][0], valid[2][1], ..., valid[2][8]: 각 3x3 그리드가 올바르면 1, 아니면 0
 */
int valid[3][9];

/*
 * 스도쿠 퍼즐의 각 행이 올바른지 검사한다.
 * 행 번호는 0부터 시작하며, i번 행이 올바르면 valid[0][i]에 1을 기록한다.
 */
void *check_rows(void *arg)
{
    // 여기를 완성하세요
    int i, j, result;

    for (i = 0; i < 9; i++)
    {
        int arr[9] = {0}; //1~9까지 잘 들어있는지 확인하기 위한 배열.
        result = 0; //1~9까지 잘 들어있는지 확인하기 위한 변수.
        for (j = 0; j < 9; j++)
        {
            int num = sudoku[i][j]; //num에 현재 위치의 스도쿠의 값 저장.
            if (arr[num - 1] == 0)  //현재 스도쿠의 값의 배열 자리가 비어있다면 값 넣기.
                arr[num - 1] = num;
            else 
            {
                valid[0][i] = 0; //현재 스도쿠의 값의 배열자리가 비어있지않다(중복되는숫자라는말), vaild[0][i]에 0넣기
                result = 1;  //숫자가 중복되어 들어있다는 말.
                break; //다음 라인 탐색.
            }
        }
        if (result == 0) //숫자가 중복되지않고 1~9까지 잘 들어가있다는말.vaild[0][i]에 1 넣기.
            valid[0][i] = 1;
    }
    pthread_exit(0); //thread 종료.
}

/*
 * 스도쿠 퍼즐의 각 열이 올바른지 검사한다.
 * 열 번호는 0부터 시작하며, j번 열이 올바르면 valid[1][j]에 1을 기록한다.
 */
void *check_columns(void *arg)
{
    // 여기를 완성하세요
    int i, j, result;

    for (j = 0; j < 9; j++)
    {
        int arr[9] = {0}; //1~9까지 잘 들어있는지 확인하기 위한 배열.
        result = 0; //1~9까지 잘 들어있는지 확인하기 위한 변수.
        for (i = 0; i < 9; i++)
        {
            int num = sudoku[i][j]; //num에 현재 위치의 스도쿠의 값 저장.
            if (arr[num - 1] == 0) //현재 스도쿠의 값의 배열 자리가 비어있다면 값 넣기.
                arr[num - 1] = num;
            else
            {
                valid[1][j] = 0; //현재 스도쿠의 값의 배열자리가 비어있지않다(중복되는숫자라는말), vaild[1][j]에 0넣기
                result = 1; //숫자가 중복되어 들어있다는 말.
                break; //다음 라인 탐색.
            }
        }
        if (result == 0) //숫자가 중복되지않고 1~9까지 잘 들어가있다는말.vaild[0][i]에 1 넣기.
            valid[1][j] = 1;
    }
    pthread_exit(0); //thread 종료.
}

/*
 * 스도쿠 퍼즐의 각 3x3 서브그리드가 올바른지 검사한다.
 * 3x3 서브그리드 번호는 0부터 시작하며, 왼쪽에서 오른쪽으로, 위에서 아래로 증가한다.
 * k번 서브그리드가 올바르면 valid[2][k]에 1을 기록한다.
 */
void *check_subgrid(void *arg)
{
    // 여기를 완성하세요
    int i, j;
    int arr[9] = {0}; //1~9까지 잘 들어있는지 확인하기 위한 배열.

    location_t *pos = (location_t *)arg; //서브그리드의 위치와 번호를 매개변수로 받아옴.
    int row = pos->row; //서브그리드 가로 위치.
    int col = pos->column; //서브그리드 세로 위치.
    int k = pos->k; //서브그리드 번호

    for (i = row; i < row + 3; i++)
    {
        for (j = col; j < col + 3; j++)
        {
            int num = sudoku[i][j]; //num에 현재 위치의 스도쿠의 값 저장.
            if (arr[num - 1] == 0) //현재 스도쿠의 값의 배열 자리가 비어있다면 값 넣기.
                arr[num - 1] = num;
            else
            {
                valid[2][k] = 0; //중복되는숫자가 있다, vaild[2][k]에 0넣기
                pthread_exit(0); //thread 종료.
            }
        }
    }
    valid[2][k] = 1; //중복되는 숫자없이 잘 들어있다. vaild[2][k]에 1 넣기
    pthread_exit(0); //thread 종료.
}

/*
 * 스도쿠 퍼즐이 올바르게 구성되어 있는지 11개의 스레드를 생성하여 검증한다.
 * 한 스레드는 각 행이 올바른지 검사하고, 다른 한 스레드는 각 열이 올바른지 검사한다.
 * 9개의 3x3 서브그리드에 대한 검증은 9개의 스레드를 생성하여 동시에 검사한다.
 */
void check_sudoku(void)
{
    int i, j;
    pthread_t p_thread[11]; //11개의 스레드를 생성하기 위함.
    /*
     * 검증하기 전에 먼저 스도쿠 퍼즐의 값을 출력한다.
     */
    for (i = 0; i < 9; ++i)
    {
        for (j = 0; j < 9; ++j)
            printf("%2d", sudoku[i][j]);
        printf("\n");
    }
    printf("---\n");
    /*
     * 스레드를 생성하여 각 행을 검사하는 check_rows() 함수를 실행한다.
     */
    // 여기를 완성하세요
    pthread_create(&p_thread[0], NULL, check_rows, NULL);
    /*
     * 스레드를 생성하여 각 열을 검사하는 check_columns() 함수를 실행한다.
     */
    // 여기를 완성하세요
    pthread_create(&p_thread[1], NULL, check_columns, NULL);
    /*
     * 9개의 스레드를 생성하여 각 3x3 서브그리드를 검사하는 check_subgrid() 함수를 실행한다.
     * 3x3 서브그리드의 위치를 식별할 수 있는 값을 함수의 인자로 넘긴다.
     */
    // 여기를 완성하세요
    location_t *one = (location_t *)malloc(sizeof(location_t)); //첫번째 서브그리드의 위치와 번호를 인자로 넘겨줌.
    one->row = 0;
    one->column = 0;
    one->k = 0;
    location_t *two = (location_t *)malloc(sizeof(location_t)); //두번째 서브그리드의 위치와 번호를 인자로 넘겨줌.
    two->row = 0;
    two->column = 3;
    two->k = 1;
    location_t *three = (location_t *)malloc(sizeof(location_t)); //세번째 서브그리드의 위치와 번호를 인자로 넘겨줌.
    three->row = 0;
    three->column = 6;
    three->k = 2;
    location_t *four = (location_t *)malloc(sizeof(location_t)); //네번째 서브그리드의 위치와 번호를 인자로 넘겨줌.
    four->row = 3;
    four->column = 0;
    four->k = 3; 
    location_t *five = (location_t *)malloc(sizeof(location_t)); //다섯번째 서브그리드의 위치와 번호를 인자로 넘겨줌.
    five->row = 3;
    five->column = 3;
    five->k = 4;
    location_t *six = (location_t *)malloc(sizeof(location_t)); //여섯번째 서브그리드의 위치와 번호를 인자로 넘겨줌.
    six->row = 3;
    six->column = 6;
    six->k = 5;
    location_t *seven = (location_t *)malloc(sizeof(location_t)); //일곱번째 서브그리드의 위치와 번호를 인자로 넘겨줌.
    seven->row = 6;
    seven->column = 0;
    seven->k = 6;
    location_t *eight = (location_t *)malloc(sizeof(location_t)); //여덟번째 서브그리드의 위치와 번호를 인자로 넘겨줌.
    eight->row = 6;
    eight->column = 3;
    eight->k = 7;
    location_t *nine = (location_t *)malloc(sizeof(location_t)); //아홉번째 서브그리드의 위치와 번호를 인자로 넘겨줌.
    nine->row = 6;
    nine->column = 6;
    nine->k = 8;

    pthread_create(&p_thread[2], NULL, check_subgrid, (void *)one);
    pthread_create(&p_thread[3], NULL, check_subgrid, (void *)two);
    pthread_create(&p_thread[4], NULL, check_subgrid, (void *)three);
    pthread_create(&p_thread[5], NULL, check_subgrid, (void *)four);
    pthread_create(&p_thread[6], NULL, check_subgrid, (void *)five);
    pthread_create(&p_thread[7], NULL, check_subgrid, (void *)six);
    pthread_create(&p_thread[8], NULL, check_subgrid, (void *)seven);
    pthread_create(&p_thread[9], NULL, check_subgrid, (void *)eight);
    pthread_create(&p_thread[10], NULL, check_subgrid, (void *)nine);

    /* 11개의 스레드가 종료할 때까지 기다린다.
     */
    // 여기를 완성하세요
    pthread_join(p_thread[0], NULL);
    pthread_join(p_thread[1], NULL);
    pthread_join(p_thread[2], NULL);
    pthread_join(p_thread[3], NULL);
    pthread_join(p_thread[4], NULL);
    pthread_join(p_thread[5], NULL);
    pthread_join(p_thread[6], NULL);
    pthread_join(p_thread[7], NULL);
    pthread_join(p_thread[8], NULL);
    pthread_join(p_thread[9], NULL);
    pthread_join(p_thread[10], NULL);
    /*
     * 각 행에 대한 검증 결과를 출력한다.
     */
    printf("ROWS: ");
    for (i = 0; i < 9; ++i)
        printf(valid[0][i] == 1 ? "(%d,YES)" : "(%d,NO)", i);
    printf("\n");
    /*
     * 각 열에 대한 검증 결과를 출력한다.
     */
    printf("COLS: ");
    for (i = 0; i < 9; ++i)
        printf(valid[1][i] == 1 ? "(%d,YES)" : "(%d,NO)", i);
    printf("\n");
    /*
     * 각 3x3 서브그리드에 대한 검증 결과를 출력한다.
     */
    printf("GRID: ");
    for (i = 0; i < 9; ++i)
        printf(valid[2][i] == 1 ? "(%d,YES)" : "(%d,NO)", i);
    printf("\n---\n");
}

/*
 * 스도쿠 퍼즐의 값을 3x3 서브그리드 내에서 무작위로 섞는 함수이다.
 */
void *shuffle_sudoku(void *arg)
{
    int i, tmp;
    int grid;
    int row1, row2;
    int col1, col2;

    srand(time(NULL));
    for (i = 0; i < 100; ++i)
    {
        /*
         * 0부터 8번 사이의 서브그리드 하나를 무작위로 선택한다.
         */
        grid = rand() % 9;
        /*
         * 해당 서브그리드의 좌측 상단 행열 좌표를 계산한다.
         */
        row1 = row2 = (grid / 3) * 3;
        col1 = col2 = (grid % 3) * 3;
        /*
         * 해당 서브그리드 내에 있는 임의의 두 위치를 무작위로 선택한다.
         */
        row1 += rand() % 3;
        col1 += rand() % 3;
        row2 += rand() % 3;
        col2 += rand() % 3;
        /*
         * 홀수 서브그리드이면 두 위치에 무작위 수로 채우고,
         */
        if (grid & 1)
        {
            sudoku[row1][col1] = rand() % 8 + 1;
            sudoku[row2][col2] = rand() % 8 + 1;
        }
        /*
         * 짝수 서브그리드이면 두 위치에 있는 값을 맞바꾼다.
         */
        else
        {
            tmp = sudoku[row1][col1];
            sudoku[row1][col1] = sudoku[row2][col2];
            sudoku[row2][col2] = tmp;
        }
    }
    pthread_exit(NULL);
}

/*
 * 메인 함수는 위에서 작성한 함수가 올바르게 동작하는지 검사하기 위한 것으로 수정하면 안 된다.
 */
int main(void)
{
    int tmp;
    pthread_t tid;

    /*
     * 기본 스도쿠 퍼즐을 출력하고 검증한다.
     */
    check_sudoku();
    /*
     * 기본 퍼즐에서 값 두개를 맞바꾸고 검증해본다.
     */
    tmp = sudoku[5][3];
    sudoku[5][3] = sudoku[6][2];
    sudoku[6][2] = tmp;
    check_sudoku();
    /*
     * 기본 스도쿠 퍼즐로 다시 바꾼 다음, shuffle_sudoku 스레드를 생성하여 퍼즐을 섞는다.
     */
    tmp = sudoku[5][3];
    sudoku[5][3] = sudoku[6][2];
    sudoku[6][2] = tmp;
    if (pthread_create(&tid, NULL, shuffle_sudoku, NULL) != 0)
    {
        fprintf(stderr, "pthread_create error: shuffle_sudoku\n");
        exit(-1);
    }
    /*
     * 무작위로 섞는 중인 스도쿠 퍼즐을 검증해본다.
     */
    check_sudoku();
    /*
     * shuffle_sudoku 스레드가 종료될 때까지 기다란다.
     */
    pthread_join(tid, NULL);
    /*
     * shuffle_sudoku 스레드 종료 후 다시 한 번 스도쿠 퍼즐을 검증해본다.
     */
    check_sudoku();
    exit(0);
}
