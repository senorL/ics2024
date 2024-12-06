#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len = 0;
  size_t i = 0;
  while(s[i] != '\0') {
    i++;
    len++;
  }
  return len;
}

char *strcpy(char *dst, const char *src) {
  size_t i = 0;
  while(src[i] != '\0') {
    dst[i] = src[i];
    i++;
  }
  dst[i] = '\0';
  return dst;

}

char *strncpy(char *dst, const char *src, size_t n) {
  size_t i;
  for (i = 0; i < n && src[i] != '\0'; i++) 
    dst[i] = src[i];
  for ( ; i < n; i++)
    dst[i] = '\0';

  return dst;

}

char *strcat(char *dst, const char *src) {
  size_t i = 0;
  while(dst[i] != '\0')
    i++;
  size_t j = 0;
  while(src[j] != '\0') {
    dst[i + j] = src[j];
    j++;
  }
  dst[i + j] = '\0';
  return dst;
}

int strcmp(const char *s1, const char *s2) {
  size_t i;
  for (i = 0; (unsigned char)s1[i] != '\0'; i++) {
    if ((unsigned char)s2[i] == '\0') {
      return 1;
    }
    if ((unsigned char)s1[i] > (unsigned char)s2[i])
      return 1;
    else if ((unsigned char)s1[i] < (unsigned char)s2[i])
      return -1;
  }
  if ((unsigned char)s1[i] == '\0' && (unsigned char)s2[i] == '\0') {
    return 0;
  }
  else {
    return -1;
  }
}

int strncmp(const char *s1, const char *s2, size_t n) {
  size_t i;
  for (i = 0; i < n && (unsigned char)s1[i] != '\0' && (unsigned char)s2[i] != '\0'; i++) {
    if ((unsigned char)s1[i] > (unsigned char)s2[i])
      return 1;
    else if ((unsigned char)s1[i] < (unsigned char)s2[i])
      return -1;
  }
  if (i == n) {
    return 0;
  }
  if ((unsigned char)s1[i] == '\0' && (unsigned char)s2[i] == '\0')
    return 0;
  else if ((unsigned char)s1[i] == '\0' && (unsigned char)s2[i] != '\0')
    return -1;
  else 
    return 1; 
}

void *memset(void *s, int c, size_t n) {
  unsigned char *p = s;
  for (size_t i = 0; i < n; i++) {
    p[i] = (unsigned char)c;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  size_t i;
  unsigned char *d = (unsigned char *)dst;
  const unsigned char *s = (const unsigned char *)src;
  if (d < s) {
    for (i = 0; i < n; i++) {
      d[i] = s[i];
    }
  }
  else {
    for (i = n; i > 0; i--) {
      d[i - 1] = s[i - 1];
    }
  }
  return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  size_t i;
  unsigned char *o = (unsigned char *)out;
  const unsigned char *ii = (unsigned char *)in;
  for (i = 0; i < n; i++) {
    o[i] = ii[i];
  }
  return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  size_t i;
  const unsigned char *ss1 = (unsigned char *)s1;
  const unsigned char *ss2 = (unsigned char *)s2;
  for (i = 0; i < n; i++) {
    if (ss1[i] > ss2[i]) {
      return 1;
    }
    else if (ss1[i] < ss2[i]) {
      return -1;
    }
  }
  return 0;
}

#endif
