#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  size_t len = 0;
  va_list ap;
  va_start(ap, fmt);
  while (*fmt) {
    if (*fmt == '%') {
      fmt++;
      switch (*fmt) {
        case 'd': {
          int val = va_arg(ap, int);
          size_t tmp_len = 0;
          int tmp_val = val;
          if (val == 0) {
            out[len++] = '0';
          } else {
            if (val < 0) {
              out[len++] = '-';
              val = -val;
            }
            while (tmp_val != 0) {
              tmp_len++;
              tmp_val /= 10;
            }
            len += tmp_len;
            for (int i = 0; i < tmp_len; i++) {
              out[len - i - 1] = (char)((val % 10) + '0');
              val /= 10;
            }
          }
          break;
        }
        case 's': {
          char *val = va_arg(ap, char*);
          while (*val) {
            out[len++] = *val++;
          }
          break;
        }
        default:
          break;
      }
    } else {
      out[len++] = *fmt;
    }
    fmt++;
  }
  out[len] = '\0';
  va_end(ap);
  return len;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
