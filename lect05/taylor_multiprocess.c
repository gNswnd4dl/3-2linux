#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

void taylor(int num_elements, int terms, double* x, double* result)
{
    for (int i = 0; i < num_elements; i++) {
        double value = x[i];
        double numer = x[i] * x[i] * x[i];
        double denom = 6.0; // 3!
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
    int N = 4;                  // 자식 프로세스 수
    int terms = 5;              // 테일러 급수 항 수
    double x_values[8] = {0.0, 0.524, 1.047, 0.134, 0.785, 1.571, 2.094, 2.618};
    int total_elements = 8;
    double results[8];

    int elements_per_child = total_elements / N;
    int fd[2 * N];

    for (int i = 0; i < N; i++) {
        pipe(fd + 2 * i);
        int pid = fork();

        if (pid < 0) {
            perror("fork error");
            exit(1);
        }

        if (pid == 0) { // 자식 프로세스
            close(fd[2 * i]); // 읽기용 닫기

            int start = i * elements_per_child;
            int end = (i == N - 1) ? total_elements : start + elements_per_child;
            int num_elements = end - start;

            double child_result[num_elements];
            taylor(num_elements, terms, &x_values[start], child_result);

            // 결과 파이프로 부모에게 전달
            write(fd[2 * i + 1], child_result, sizeof(double) * num_elements);

            close(fd[2 * i + 1]);
            exit(0); // 자식 종료
        } else {
            close(fd[2 * i + 1]); // 부모 쓰기용 닫기
        }
    }

    for (int i = 0; i < N; i++) {
        int start = i * elements_per_child;
        int end = (i == N - 1) ? total_elements : start + elements_per_child;
        int num_elements = end - start;

        read(fd[2 * i], &results[start], sizeof(double) * num_elements);
        close(fd[2 * i]);
    }

    for (int i = 0; i < N; i++)
        wait(NULL);

    for (int i = 0; i < total_elements; i++) {
        printf("sin(%.3f) by Taylor series = %.6f\n", x_values[i], results[i]);
        printf("sin(%.3f) = %.6f\n", x_values[i], sin(x_values[i]));
    }

    return 0;
}

