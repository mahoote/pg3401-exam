#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  // Check if the correct number of arguments were provided
  if (argc != 3) {
    printf("Usage: %s <input_file> <output_file>\n", argv[0]);
    return 1;
  }

  // Open the input file in read-only mode
  FILE *input_file = fopen(argv[1], "r");
  if (input_file == NULL) {
    printf("Error opening input file: %s\n", argv[1]);
    return 1;
  }

  // Open the output file in write-only mode
  FILE *output_file = fopen(argv[2], "w");
  if (output_file == NULL) {
    printf("Error opening output file: %s\n", argv[2]);
    return 1;
  }

  // Read one character at a time from the input file
  char ch;
  while ((ch = fgetc(input_file)) != EOF) {
    // Convert the character to its ASCII representation in hexadecimal
    char hex[3];
    snprintf(hex, 3, "%02X", ch);
    // Write the hexadecimal representation to the output file
    fputs(hex, output_file);
  }

  // Close the input and output files
  fclose(input_file);
  fclose(output_file);

  return 0;
}