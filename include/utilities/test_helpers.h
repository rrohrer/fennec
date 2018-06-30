#ifndef test_helpers_h
#define test_helpers_h

#define RETURN_IF_FAILED(x)                                                    \
  {                                                                            \
    int ret = x;                                                               \
    if (ret != 0)                                                              \
      return ret;                                                              \
  }
#define FAIL_IF(condition, ...)                                                \
  if (condition) {                                                             \
    printf("%s: ", __func__);                                                  \
    printf(__VA_ARGS__);                                                       \
    return 1;                                                                  \
  }

#endif
