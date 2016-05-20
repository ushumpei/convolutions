#include <stdio.h>

int MAX_FILTER_SIZE = 10;
int MAX_DATA_SIZE = 100;

void convolute(float *filter, int filter_size, int *input_data, int input_data_size, int *output_data);

int main(void) {
  int i, ret, input_data_size = 0, filter_size = 0;

  FILE *input_file;
  FILE *filter_file;
  char *input_file_name   = "input.csv";
  char *filter_file_name  = "filter.csv";

  float filter_data[MAX_FILTER_SIZE];
  int input_data[MAX_DATA_SIZE];
  int output_data[MAX_DATA_SIZE];

  // INPUT DATAの読み込み
  input_file = fopen( input_file_name, "r");
  if( input_file == NULL) {
    printf("%sファイルが開けません\n", input_file_name);
    return 1;
  }
  while( (ret = fscanf( input_file, "%d", input_data+input_data_size)) != EOF
      && input_data_size < MAX_DATA_SIZE) input_data_size++;
  fclose(input_file);

  // FILTERの読み込み
  filter_file = fopen( filter_file_name, "r");
  if( filter_file == NULL) {
    printf("%sファイルが開けません\n", filter_file_name);
    return 1;
  }
  while( (ret = fscanf( filter_file, "%f", filter_data+filter_size)) != EOF
      && filter_size < MAX_FILTER_SIZE) filter_size++;
  fclose(filter_file);

  // 畳み込みの実行
  convolute(filter_data, filter_size, input_data, input_data_size, output_data);

  // 表示
  printf("\nFILTER:\n");
  for(i = 0; i < filter_size; i++) {
    printf("%f ", filter_data[i]);
  }
  printf("\nINPUT:\n");
  for(i = 0; i < input_data_size; i++) {
    printf("%d ", input_data[i]);
  }
  printf("\nOUTPUT:\n");
  for(i = 0; i < input_data_size - filter_size + 1; i++) {
    printf("%d ", output_data[i]);
  }
  printf("\nEND:\n");
}

void convolute(float *filter, int filter_size, int *input_data, int input_data_size, int *output_data) {
  int i, j;
  float tmp;
  // loop from 0 to (DATA_SIZE - FILTER_SIZE + 1)
  for(i = 0; i < input_data_size - filter_size + 1; i++) {
    tmp = 0;
    for(j = 0; j < filter_size; j++) {
      tmp += *(filter+j) * *(input_data+i+j);
    }
    *(output_data+i) = tmp;
  }
}
