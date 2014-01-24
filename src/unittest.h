#ifndef UNIT_TEST_H_INCLUDE

#define UNIT_TEST_H_INCLUDE

#define TEST_ASSERT(e) do(if(!(e))throw "BUG";}while(0)

#define TEST_CASE(NAME)                             \
    extern void TESTCASE_##NAME();                  \
    namespace vl_unittest_executors                 \
    {                                               \
        class TESTCASE_RUNNER_##NAME                \
        {                                           \
        public:                                     \
            TESTCASE_RUNNER_##NAME()                \
            {                                       \
                TESTCASE_##NAME();                  \
            }                                       \
        } TESTCASE_RUNNER_##NAME##_INSTANCE;        \
    }                                               \
    void TESTCASE_##NAME()

#endif
