#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 512

typedef struct opts_status {
  int e, i, v, c, l, n, h, s, f, o;
  char* pattern;
  size_t pattern_length;
  size_t pattern_size;
} OPTS_STATUS;

void resize_ar(char** line, int* size) {
  *size *= 2;
  char* new_line = realloc(*line, *size);
  if (new_line == NULL) {
    perror("Memory increase error");
    exit(EXIT_FAILURE);
  } else {
    *line = new_line;
  }
}

void pattern_adding(OPTS_STATUS* opts_status, const char* pattern) {
  if (opts_status->pattern == NULL) {
    opts_status->pattern_size = MAX_SIZE;
    opts_status->pattern = malloc(opts_status->pattern_size);
    if (opts_status->pattern == NULL) {
      perror("Memory allocation error");
      exit(EXIT_FAILURE);
    }
    opts_status->pattern[0] = '\0';
  }
  int n = strlen(pattern);
  if (opts_status->pattern_size <= (opts_status->pattern_length + n + 3))
    resize_ar(&opts_status->pattern, (int*)&opts_status->pattern_size);
  if (opts_status->pattern_length != 0) {
    opts_status->pattern[opts_status->pattern_length++] = '|';
  }
  opts_status->pattern_length += sprintf(
      opts_status->pattern + opts_status->pattern_length, "(%s)", pattern);
}

void regex_from_file(OPTS_STATUS* opts_status, const char* path) {
  FILE* file_reg = fopen(path, "r");
  if (file_reg == NULL) {
    if (!opts_status->s) {
      perror("Error opening file");
      exit(EXIT_FAILURE);
    }
  }
  char* line_reg = malloc(MAX_SIZE);
  if (line_reg == NULL) {
    perror("Memory allocation error");
    exit(EXIT_FAILURE);
  }
  line_reg[0] = '\0';
  int current_symbol;
  int n = 0;
  int size = MAX_SIZE;
  do {
    current_symbol = fgetc(file_reg);
    if (current_symbol != '\n' && current_symbol != EOF) {
      if (n >= size)
        resize_ar(&line_reg,
                  &size); /* Указатель на указатель нужен, когда функция должна
                             изменить сам указатель, а не только данные, на
                             которые он указывает. Иначе мы меняе локальную
                             копию указателя line, но оригинальный указатель в
                             вызывающей функции останется неизменным.*/
      line_reg[n++] = current_symbol;
    } else {
      line_reg[n] = '\0';
      pattern_adding(opts_status, line_reg);
      n = 0;
    }
  } while (current_symbol != EOF);
  fclose(file_reg);
  free(line_reg);
}

void opts_handler(int argc, char* argv[], OPTS_STATUS* opts_status) {
  int opt;
  while ((opt = getopt(argc, argv, "+e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'e':
        opts_status->e = 1;
        pattern_adding(opts_status, optarg);
        break;
      case 'i':
        opts_status->i = REG_ICASE;
        break;
      case 'v':
        opts_status->v = 1;
        break;
      case 'c':
        opts_status->c = 1;
        break;
      case 'l':
        opts_status->l = 1;
        break;
      case 'n':
        opts_status->n = 1;
        break;
      case 'h':
        opts_status->h = 1;
        break;
      case 's':
        opts_status->s = 1;
        break;
      case 'f':
        opts_status->f = 1;
        regex_from_file(opts_status, optarg);
        break;
      case 'o':
        opts_status->o = 1;
        break;
      case '?':
        printf("Unknown argument: %c\n", optopt);
        exit(1);
      default:
        break;
    }
  }
  if (opts_status->pattern_length == 0) {
    pattern_adding(opts_status, argv[optind++]);
  }
  if ((argc - optind) == 1) {
    opts_status->h = 1;
  }
}

void output_line(const char* line, int n) {
  for (int i = 0; i < n; i++) putchar(line[i]);
}

void output_path_line(const OPTS_STATUS* opts_status, const char* path,
                      int line_counter) {
  if (!opts_status->h) {
    printf("%s:", path);
  }
  if (opts_status->n) {
    printf("%d:", line_counter);
  }
}

void output_match(char* line, regex_t* regex, const OPTS_STATUS* opts_status,
                  const char* path, const int line_counter) {
  regmatch_t match;
  int result = 1;
  char* ptr = line;
  while ((result = regexec(regex, ptr, 1, &match, 0)) == 0) {
    output_path_line(opts_status, path, line_counter);
    for (int i = match.rm_so; i < match.rm_eo; i++) putchar(ptr[i]);
    printf("\n");
    ptr += match.rm_eo;
  }
}

void output_result(const OPTS_STATUS* opts_status, char* line, regex_t* regex,
                   const char* path, const int line_counter, const int n) {
  if (!opts_status->c && !opts_status->l) {
    if (opts_status->o) {
      output_match(line, regex, opts_status, path, line_counter);
    } else {
      output_path_line(opts_status, path, line_counter);
      output_line(line, n);
      printf("\n");
    }
  }
}

void file_handler(const char* path, const OPTS_STATUS* opts_status,
                  regex_t* regex) {
  FILE* file = fopen(path, "r");
  if (file == NULL) {
    if (!opts_status->s) perror("Error opening file");
    exit(1);
  }
  char* line = malloc(MAX_SIZE);
  if (line == NULL) {
    perror("Memory allocation error");
    exit(EXIT_FAILURE);
  }
  line[0] = '\0';
  int current_symbol;
  int n = 0;
  int size = MAX_SIZE;
  int line_counter = 0;
  int match_counter = 0;
  do {
    current_symbol = fgetc(file);
    if (current_symbol != '\n' && current_symbol != EOF) {
      if (n >= size) resize_ar(&line, &size);
      line[n++] = current_symbol;
    } else if (!(n == 0 && line_counter == 0)) {
      line_counter++;
      line[n] = '\0';
      int result = regexec(regex, line, 0, NULL, 0);
      if ((result == 0 && !opts_status->v) || (result != 0 && opts_status->v)) {
        output_result(opts_status, line, regex, path, line_counter, n);
        if (!opts_status->v) match_counter++;
      }
      n = 0;
    }
  } while (current_symbol != EOF);
  if (opts_status->c && !opts_status->l) {
    if (!opts_status->h) printf("%s:", path);
    printf("%d\n", match_counter);
  }
  if (opts_status->l && match_counter > 0) printf("%s\n", path);
  fclose(file);
  free(line);
}

void output(int argc, char* argv[], const OPTS_STATUS* opts_status) {
  regex_t regex;
  int regcomp_error =
      regcomp(&regex, opts_status->pattern, REG_EXTENDED | opts_status->i);
  if (regcomp_error) {
    perror("Error compile the regular expression.");
    exit(EXIT_FAILURE);
  }
  for (int i = optind; i < argc; i++) {
    file_handler(argv[i], opts_status, &regex);
  }
  regfree(&regex);
}

int main(int argc, char* argv[]) {
  OPTS_STATUS opts_status = {
      .pattern = NULL, .pattern_length = 0, .pattern_size = 0};
  opts_handler(argc, argv, &opts_status);
  output(argc, argv, &opts_status);

  free(opts_status.pattern);
  return 0;
}