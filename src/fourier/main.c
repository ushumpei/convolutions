/***************************/
/* FourierTransformation.c */
/***************************/

// 要素数Nの数列 a[m] をフーリエ変換で b[n] にする。添字は 0,...,N-1
// b[n] = sum[m=0,N-1]  a[m]*exp(-2*PI*I* (m/N) *n)

#include <stdio.h>
#include <complex.h> // complex.h は fftw3.h より先に include する
#include <fftw3.h>   // windows環境では #include "C:/path/to/fftw3.h"
                     // あるいは        #include "./相対パス/fftw3.h"

int main( void ){

  int N=4;

  // a,b は double _Complex 型のC99標準複素配列と実質的に同じ
  // double _Complex a[4] としても動くけど計算速度が低下する可能性あり
  fftw_complex *a, *b;
  a = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
  b = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

  // プランの生成
  // フーリエ逆変換つまり位相因子を exp(-k)ではなく exp(+k)とする場合は
  // FFTW_FORWARD の代わりに FFTW_BACKWARD とする
  fftw_plan plan;
  plan = fftw_plan_dft_1d( N, a, b, FFTW_FORWARD, FFTW_ESTIMATE );

  // フーリエ変換前の数列値を設定
  a[0] = 1.0 + 0.0*I;
  a[1] = 2.0 + 0.0*I;
  a[2] = 5.0 + 0.0*I;
  a[3] = 3.0 + 0.0*I;

  // フーリエ変換実行   b[n]に計算結果が入る
  fftw_execute(plan);

  // b[n]の値を表示
  int n;
  for( n=0; n<N; n++ ){
    printf("b_%d = %+lf %+lf*i\n", n, creal(b[n]), cimag(b[n]) );
  }

  // ここで a[m] の値を変えて再度 fftw_execute(plan) を実行すれば、
  // b[n] が再計算される。

  // 計算終了時、メモリ開放を忘れないように
  if(plan) fftw_destroy_plan(plan);
  fftw_free(a); fftw_free(b);

  return 0;
}



