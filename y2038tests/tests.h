void tests_init(void);
void test_begin(const char *description);
void test_failure(int print_errno, const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));
void test_success(void);
void tests_report(void);
void test_run(void (*function)(void));
