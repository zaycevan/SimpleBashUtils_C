#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct opts_status {
  int b, E, n, s, T, v;
} OPTS_STATUS;

static void opts_handler(int argc, char* argv[], OPTS_STATUS* opts_status) {
  struct option long_opts[] = {{"number-nonblank", 0, NULL, 'b'},
                               {"number", 0, NULL, 'n'},
                               {"squeeze-blank", 0, NULL, 's'},
                               {NULL, 0, NULL, 0}};

  int opt;
  while ((opt = getopt_long(argc, argv, "beEnstT", long_opts, NULL)) != -1) {
    switch (opt) {
      case 'b':
        opts_status->b = 1;
        break;
      case 'e':
        opts_status->E = 1;
        opts_status->v = 1;
        break;
      case 'E':
        opts_status->E = 1;
        break;
      case 'n':
        opts_status->n = 1;
        break;
      case 's':
        opts_status->s = 1;
        break;
      case 't':
        opts_status->T = 1;
        opts_status->v = 1;
        break;
      case 'T':
        opts_status->T = 1;
        break;
      case '?':
        printf("Unknown argument: %c\n", optopt);
        exit(1);
      default:
        break;
    }
  }
}

static void symbol_handler(int* current_symbol) {
  if (*current_symbol < 32 && *current_symbol != 9 && *current_symbol != 10 &&
      *current_symbol != 13) {
    putchar('^');
    *current_symbol = *current_symbol + 64;
  } else if (*current_symbol == 127) {
    putchar('^');
    *current_symbol = '?';
  } else if (*current_symbol > 127) {
    printf("M-");
    *current_symbol = *current_symbol - 128;
    if (*current_symbol < 32 && *current_symbol != 9 && *current_symbol != 10 &&
        *current_symbol != 13) {
      putchar('^');
      *current_symbol = *current_symbol + 64;
    } else if (*current_symbol == 127) {
      putchar('^');
      *current_symbol = '?';
    }
  }
}

static void file_handler(const char* argv[], const OPTS_STATUS* opts_status) {
  FILE* file = fopen(argv[optind], "r");
  if (file == NULL) {
    perror("Error opening file");
    exit(1);
  }

  int current_symbol;
  int prev_symbol = '\n';
  int line_counter = 0;
  int empty_line_counter = 0;

  while ((current_symbol = fgetc(file)) != EOF) {
    if (opts_status->s && current_symbol == '\n' && prev_symbol == '\n') {
      empty_line_counter++;
    } else
      empty_line_counter = 0;
    if ((opts_status->b || opts_status->n) && current_symbol != '\n' &&
        prev_symbol == '\n') {
      printf("%6d\t", ++line_counter);
    } else if (empty_line_counter <= 1 && opts_status->n &&
               current_symbol == '\n' && prev_symbol == '\n')
      printf("%6d\t", ++line_counter);

    if ((opts_status->E) && current_symbol == '\n' && empty_line_counter <= 1)
      printf("$");

    if (opts_status->T && current_symbol == '\t') {
      putchar('^');
      current_symbol = 'I';
    }

    if (opts_status->v) symbol_handler(&current_symbol);
    if (empty_line_counter <= 1) putchar(current_symbol);
    prev_symbol = current_symbol;
  }
  fclose(file);
}

int main(int argc, char* argv[]) {
  OPTS_STATUS opts_status = {0};
  opts_handler(argc, argv, &opts_status);
  if (opts_status.b) opts_status.n = 0;
  file_handler((const char**)argv, &opts_status);

  return 0;
}