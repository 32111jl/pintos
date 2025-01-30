#include <stdio.h>
#include <string.h>
#include "devices/input.h"
#include "lib/kernel/console.h"
#include "threads/kmonitor.h"

static void read_line(char *buffer, size_t size);
static void trim_spaces(char *buffer);

void
interactive_kernel(void) {
  while (true) {
    printf("CS318> ");

    char buffer[128];
    read_line(buffer, sizeof buffer);
    trim_spaces(buffer);

    if (!strcmp(buffer, "whoami")) {
      printf("Jonathan Liu\n");
    } else if (!strcmp(buffer, "exit")) {
      break;
    } else {
      printf("invalid command\n");
    }
  }
}

static void
read_line(char * buffer, size_t size) {
  size_t i = 0;
  while (i < size - 1) {
    uint8_t c = input_getc();
    
    // new line
    if (c == '\n' || c == '\r') {
      putchar('\n');
      buffer[i] = '\0';
      break;
    }
    
    // handle backspaces
    else if (c == '\b' || c == 127) {
      if (i > 0) {
        i--; // go back one
        putchar('\b');
        putchar(' ');
        putchar('\b'); // overwrite and move back
      }
    }

    // printable chars
    else if (c >= 32 && c <= 126) {
      putchar(c);
      buffer[i] = c;
      i++;
    }
  }

  if (i == size - 1) buffer[i] = '\0';
}

static void
trim_spaces(char * buffer) {

  // remove leading spaces
  char * start = buffer;
  while (*start == ' ' || *start == '\t') start++;

  // remove trailing spaces
  char * end = start + strlen(start);
  while (end > start && (end[-1] == ' ' || end[-1] == '\t')) end--;

  size_t len = end - start;
  memmove(buffer, start, len);
  buffer[len] = '\0';
}