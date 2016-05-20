#include <stdio.h>
#include <stdlib.h>

#define MAX_SIGNALS_SIZE 1000
#define MAX_STRINGS 32

void convolute(double *filter, int filter_length, double *signals, int signal_length, double *products);
void get_double_data(const char *filename, double *data, int *datasize);

int main(void) {
  const char *signal_file   = "../data/input.csv";
  double *signals;
  int signal_length;

  const char *filter_file  = "../data/filter.csv";
  double *filter;
  int filter_length;

  double *products;
  int i;

  // ファイルから読み込む
  get_double_data(filter_file, filter,  &filter_length);
  get_double_data(signal_file, signals, &signal_length);

  // 畳み込みの実行
  convolute(filter, filter_length, signals, signal_length, products);

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
  for(i = 0; i < signal_length - filter_length; i++) {
    printf("%d:\t%f\n", i, products[i]);
  }
}

void convolute(double *filter, int filter_length, double *signals, int signal_length, double *products) {
  int i, j;
  float tmp;
  for(i = 0; i < signal_length - filter_length + 1; i++) {
    tmp = 0;
    for(j = 0; j < filter_length; j++) {
      tmp += filter[j] * signals[i+j];
    }
    products[i] = tmp;
  }
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
