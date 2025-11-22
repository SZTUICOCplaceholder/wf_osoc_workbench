#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len = 0;
  while(s[len] != '\0'){
      len++;
  }
  return len;
}

char *strcpy(char *dst, const char *src) {
  size_t i = 0;
  while(src[i] != '\0'){
    dst[i] = src[i];
    i++;
  }
  dst[i] = '\0';

  return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  size_t i = 0;
  for(; i < n && src[i] != '\0'; i++){
    dst[i] = src[i];
  }
  dst[i] = '\0';

  return dst;
}

char *strcat(char *dst, const char *src) {
  size_t i = 0; size_t j = 0;
  while(dst[i] != '\0'){
    i++;
  }
  while(src[j] != '\0'){
    dst[i+j] = src[j];
    j++;
  }
  dst[i+j] = '\0';

  return dst;
}

int strcmp(const char *s1, const char *s2) {
  const unsigned char *p1 = (const unsigned char *)s1;
  const unsigned char *p2 = (const unsigned char *)s2;

  while(*p1 == *p2){
    if(*p1 == '\0') return 0;
    p1++; p2++;
  }

  return *p1-*p2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  if(n == 0) return 0;

  const unsigned char *p1 = (const unsigned char *)s1;
  const unsigned char *p2 = (const unsigned char *)s2;

  while(n > 0){
    if(*p1 != *p2 || *p1 == '\0'){
      return *p1-*p2;
    }
    p1++; p2++;
    n--;
  }

  return 0;
}

void *memset(void *s, int c, size_t n) {
    if (n == 0 || s == NULL) {
        return s;
    }

    unsigned char *byte_ptr = (unsigned char *)s;
    unsigned char set_val = (unsigned char)c;

    while (n-- > 0) {
        *byte_ptr++ = set_val;
    }

    return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  if(n == 0) return 0;
  if(s1 == NULL || s2 == NULL) return 1;

  const unsigned char* p1 = (const unsigned char *)s1;
  const unsigned char* p2 = (const unsigned char *)s2;

  while(n-->0){
    if(*p1 != *p2) return *p1 - *p2;
    p1++; p2++;
  }

  return 0;
}

#endif
