#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>
#include <limits.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
    va_list v_args;
    va_start(v_args, fmt);
    char *start = out;

    while (*fmt != '\0') {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == '\0') break;
            switch (*fmt) {
                case '%': 
                    *out++ = '%';
                    break;
                case 'd': {
                    int num = va_arg(v_args, int);
                    unsigned int abs_num;
                    int is_negative = 0;
                    if (num < 0) {
                        is_negative = 1;
                        if (num == INT_MIN) {
                            abs_num = (unsigned int)INT_MAX + 1;
                        } else {
                            abs_num = (unsigned int)(-num);
                        }
                    } else {
                        abs_num = (unsigned int)num;
                    }
                    if (abs_num == 0) {
                        *out++ = '0';
                    } else {
                        char num_buf[12];
                        char *p = num_buf;
                        while (abs_num > 0) {
                            *p++ = '0' + (abs_num % 10);
                            abs_num /= 10;
                        }
                        if (is_negative) {
                            *p++ = '-';
                        }
                        while (p > num_buf) {
                            *out++ = *(--p);
                        }
                    }
                    break;
                }
                    
                case 's': {
                    const char *s = va_arg(v_args, const char*);
                    while (*s) {
                        *out++ = *s++;
                    }
                    break;
                }
            }
            fmt++;
        } else {
            *out++ = *fmt++;
        }
    }
    
    *out = '\0';
    va_end(v_args);

    return out - start;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
