#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <string.h>

void taylor(int num_elements, int terms, double* x, double* result)
{
    for (int i = 0; i < num_elements; i++) {
        double value = x[i];
        double numer = x[i] * x[i] * x[i];
        double denom = 6.0;
        int sign = -1;
        for (int j = 1; j <= terms; j++) {
            value += (double)sign * numer / denom;
            numer *= x[i] * x[i];
            denom *= (2.0 * (double)j + 2.0) * (2.0 * (double)j + 3.0);
            sign *= -1;
        }
        result[i] = value;
    }
}

int main() {
    int N = 2;          // 자식 프로세스 수
    int terms = 5;      // 테일러 급수 항 수
    int total = 4;      // 전체 데이터 수
    double x_values[4] = {0.0, M_PI/6, M_PI/3, M_PI/2};
    double results[4];

    int data = total / N;
    int fd[2 * N];
    char message[100], buffer[100];

    for (int i = 0; i < N; i++) {
        pipe(fd + 2 * i);
        int pid = fork();

        if (pid < 0) {
            exit(1);
        }

        if (pid == 0) { // 자식 프로세스
            close(fd[2 * i]); // 읽기용 닫기

            int start = i * data;
            int end = (i == N - 1) ? total : start + data;
            int num = end - start;

            double child_result[num];
            taylor(num, terms, &x_values[start], child_result);

            for (int k = 0; k < num; k++) {
                sprintf(message, "%.10lf", child_result[k]); 
                write(fd[2 * i + 1], message, strlen(message) + 1); 
            }

            close(fd[2 * i + 1]);
            exit(0);
        } else {
            close(fd[2 * i + 1]); // 부모는 쓰기용 닫기
        }
    }

    for (int i = 0; i < N; i++) {
        int start = i * data;
        int end = (i == N - 1) ? total : start + data;
        int num = end - start;

        for (int k = 0; k < num; k++) {
            read(fd[2 * i], buffer, sizeof(buffer));
            results[start + k] = atof(buffer);
        }
        close(fd[2 * i]);
    }

    for (int i = 0; i < N; i++)
        wait(NULL);

    for (int i = 0; i < total; i++) {
        printf("sin(%.3f) by Taylor series = %.6f\n", x_values[i], results[i]);
        printf("sin(%.3f) = %.6f\n", x_values[i], sin(x_values[i]));
    }

    return 0;
}

