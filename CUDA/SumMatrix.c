//OBS: Usei como base o código do Daniel Favero. Eu estava com dificuldade na estrutura e ele 
//me ajudou a entender melhor.
//Para diferenciar do dele, eu fiz uma soma de matrizes quadradas de N x N.

%%cu

#include <stdio.h>
#include <math.h>
#include <cuda_runtime.h>

void printMatrix(float *A, int rows, int columns) {
    int i;
    int j;

    for (i = 0; i < rows; i++) {
      for(j = 0; j < columns; j++) {
        printf("%f\t", A[i]);
      }
      printf("\n");
    }
    printf("\n");
}

__global__ void sumMatrix(float *A, float *B, float *C, int numElements) {
  int i;

  int row = blockIdx.y * blockDim.y + threadIdx.y;

  float sum = 0;
  if(row < numElements){
      for(i = 0; i < numElements; i++){
          sum = A[row * numElements + i] + B[row * numElements + i];
          C[row * numElements + i] = sum;
      }
  }
}

int main() {
    // Error check do CUDA
    cudaError_t err = cudaSuccess;

    // Criar duas matrizes quadradas de N x N
    int N = 4;
    int matrixSize = N * N;
    printf("[Soma de duas matrizes %d x %d]\n", N, N);

    size_t size = matrixSize * sizeof(float*);

    // Alocar matrizes
    float *h_A = (float *)malloc(size);
    float *h_B = (float *)malloc(size);
    float *h_C = (float *)malloc(size);

    // Verificar alocação
    if (h_A == NULL || h_B == NULL || h_C == NULL) {
        fprintf(stderr, "Ocorreu um erro ao alocar espaço para as matrizes!\n");
        exit(EXIT_FAILURE);
    }

    // Inicilaizar as matrizes do host
    int i;
    int j;
    for (i = 0; i < N; i++) {
        for(j = 0; j < N; j++){     
          h_A[i * N + j] = j;
          h_B[i * N + j] = j;
        }
    }

    // Alocar matriz de input A do device
    float *d_A = NULL;
    err = cudaMalloc((void **) &d_A, size);

    if (err != cudaSuccess) {
        fprintf(stderr, "Falha ao alocar a matriz A do device (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Alocar matriz de input B do device
    float *d_B = NULL;
    err = cudaMalloc((void **) &d_B, size);

    if (err != cudaSuccess) {
        fprintf(stderr, "Falha ao alocar a matriz B do device (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Alocar matriz de saída C do device
    float *d_C = NULL;
    err = cudaMalloc((void **) &d_C, size);

    if (err != cudaSuccess) {
        fprintf(stderr, "Falha ao alocar a matriz C do device (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Copiar os vetores host de input que estão na memória do host para a memória do device
    printf("Copiando vetor A do host para a memória do device\n");
    err = cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);

    if (err != cudaSuccess) {
        fprintf(stderr, "Falha ao copiar o vetor A do host para o device (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    printf("Copiando vetor B do host para a memória do device\n");
    err = cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    if (err != cudaSuccess) {
        fprintf(stderr, "Falha ao copiar o vetor B do host para o device (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    int threadsPerBlock = 16;
    int blocksPerGrid = (matrixSize + threadsPerBlock - 1) / threadsPerBlock;
    printf("kernel do CUDA rodando com %d blocos de %d threads\n", blocksPerGrid, threadsPerBlock);
    sumMatrix<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, N);
    err = cudaGetLastError(); 

    if (err != cudaSuccess) {
        fprintf(stderr, "Falha ao rodar o kernel sumMatrix (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Copiar os vetores do device que estão na memória do device para a memória do host
    printf("Copiando vetor C do device do CUDA para a memória do host\n");
    err = cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    if (err != cudaSuccess) {
        fprintf(stderr, "Falha ao copiar o vetor A do device para o host (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Verificar os resultados
    printf("\nA\n");
    printMatrix(h_A, N, N);
    // Verificar os resultados
    printf("\nB\n");
    printMatrix(h_B, N, N);
    printf("--------------------------------------------------------------------------------");
    printf("\nC = A + B\n");
    printMatrix(h_C, N, N);

    // Limpar a memória global
    err = cudaFree(d_A);

    if (err != cudaSuccess) {
        fprintf(stderr, "Falha ao liberar memória da matriz A (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    err = cudaFree(d_B);

    if (err != cudaSuccess) {
        fprintf(stderr, "Falha ao liberar memória da matriz B (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    err = cudaFree(d_C);

    if (err != cudaSuccess) {
        fprintf(stderr, "Falha ao liberar memória da matriz C (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }
    
    // Liberar memória do host
    free(h_A);
    free(h_B);
    free(h_C);

    // Resetar o device e sair
    err = cudaDeviceReset();

    if (err != cudaSuccess) {
        fprintf(stderr, "Falha ao desnicializar o device error=%s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    printf("Finalizado\n");
    return 0;
}