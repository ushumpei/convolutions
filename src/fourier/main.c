#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>

#define MAX_SIGNALS_SIZE 1000
#define MAX_STRINGS 32

void get_double_data(const char *filename, double *data, int *datasize);

int main(int argc, char *argv){
  const char* signal_file = "../data/input.csv";
  double *signals;
  fftw_complex *f_signals;
  int signal_length;

  const char* filter_file = "../data/filter.csv";
  double *filter;
  fftw_complex *f_filter;
  int filter_length;

  double *products;
  fftw_complex *f_products;

  int i;
  signals   = (double *) malloc(sizeof(double) * MAX_SIGNALS_SIZE);
  filter    = (double *) malloc(sizeof(double) * MAX_SIGNALS_SIZE);
  products  = (double *) malloc(sizeof(double) * MAX_SIGNALS_SIZE);

  // ファイルから読み込む
  get_double_data(filter_file, filter,  &filter_length);
  get_double_data(signal_file, signals, &signal_length);

  // 積をとるためにFilterをゼロ埋めする
  for(i = filter_length; i < signal_length; i++) {
    filter[i] = 0;
  }

  // フーリエ変換実行
  f_signals = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * signal_length);
  f_filter  = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * signal_length);
  fftw_plan signals_plan = fftw_plan_dft_r2c_1d(signal_length, signals, f_signals, FFTW_ESTIMATE);
  fftw_plan filter_plan  = fftw_plan_dft_r2c_1d(filter_length, filter,  f_filter,  FFTW_ESTIMATE);
  fftw_execute(signals_plan);
  fftw_execute(filter_plan);

  // 複素数として積をとる
  f_products = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * signal_length);
  for(i = 0; i < signal_length; i++) {
    f_products[i][0] = f_signals[i][0] * f_filter[i][0] - f_signals[i][1] * f_filter[i][1];
    f_products[i][1] = f_signals[i][1] * f_filter[i][0] + f_signals[i][0] * f_filter[i][1];
  }

  // 逆フーリエ変換を実行
  fftw_plan products_plan = fftw_plan_dft_c2r_1d(signal_length, f_products, products, FFTW_ESTIMATE);
  fftw_execute(products_plan);

  // 定数を除去する
  for(i = 0; i < signal_length; i++) {
    products[i] = products[i] / signal_length;
  }

  // 出力する
  printf("\nSIGNALS:\n");
  for(i = 0; i < signal_length; i++) {
    printf("%d:\t%f\n", i, signals[i]);
  }
  printf("\nFILTER:\n");
  for(i = 0; i < signal_length; i++) {
    printf("%d:\t%f\n", i, filter[i]);
  }
  printf("\nOUTPUT:\n");
  for(i = 0; i < signal_length; i++) {
    printf("%d:\t%f\n", i, products[i]);
  }

  fftw_destroy_plan(signals_plan);
  fftw_destroy_plan(filter_plan);
  fftw_destroy_plan(products_plan);

}

void get_double_data(const char *filename, double *data, int *datasize) {
  FILE *fp;
  char line[MAX_STRINGS];

  if ((fp = fopen(filename, "r")) == NULL) {
    printf("ファイル「%s」のオープンに失敗しました。プログラムを終了します。\n", filename);
    exit(1);
  }

  /* データ処理 */
  *datasize = 0;
  while (fgets(line, MAX_STRINGS, fp) != NULL) {
    data[*datasize] = atof(line);
    (*datasize)++;
  }

  /* ファイル・クローズ */
  fclose(fp);
}

