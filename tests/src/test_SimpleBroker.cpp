#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "usecase/simple_broker.hpp"

using namespace ::testing;


TEST(TestSimpleBroker, test1) {
    using key_t = int;
    using broker_t = hlb::SimpleBroker<key_t, bool(int)>;

    broker_t broker;

    try {
        broker(0, 10);
        FAIL();
    } catch (const std::runtime_error& error) {
        SUCCEED();
    }
}

TEST(TestSimpleBroker, test2) {
    using key_t = int;
    using broker_t = hlb::SimpleBroker<key_t, bool(int)>;
    using executor_mock_t = MockFunction<bool(int)>;

    executor_mock_t executor;
    broker_t broker;
    broker.add_executor(1, 0, executor.AsStdFunction());

    try {
        broker(0, 10);
        FAIL();
    } catch (const std::runtime_error& error) {
        SUCCEED();
    }
}

TEST(TestSimpleBroker, test3) {
    using key_t = int;
    using broker_t = hlb::SimpleBroker<key_t, bool(int)>;
    using executor_mock_t = MockFunction<bool(int)>;

    executor_mock_t executor;
    EXPECT_CALL(executor, Call(10)).WillOnce(Return(true));
    broker_t broker;
    broker.add_executor(0, 0, executor.AsStdFunction());
    ASSERT_THAT(broker(0, 10), IsTrue());
}

TEST(TestSimpleBroker, test4) {
    using key_t = int;
    using broker_t = hlb::SimpleBroker<key_t, bool(int)>;
    using executor_mock_t = MockFunction<bool(int)>;

    executor_mock_t executor1;
    executor_mock_t executor2;
    EXPECT_CALL(executor1, Call(10)).WillOnce(Return(true));
    EXPECT_CALL(executor2, Call(20)).WillOnce(Return(false));
    broker_t broker;
    broker.add_executor(0, 0, executor1.AsStdFunction());
    broker.add_executor(1, 1, executor2.AsStdFunction());
    ASSERT_THAT(broker(0, 10), IsTrue());
    ASSERT_THAT(broker(1, 20), IsFalse());
}

TEST(TestSimpleBroker, test5) {
    using key1_t = int;
    using key2_t = std::string;
    using broker1_t = hlb::SimpleBroker<key1_t, bool(const key2_t&, int)>;
    using broker2_t = hlb::SimpleBroker<key2_t, bool(int)>;
    using executor_mock_t = MockFunction<bool(int)>;

    executor_mock_t executor;
    EXPECT_CALL(executor, Call(10)).WillOnce(Return(true));
    broker1_t broker1;
    broker2_t broker2;
    broker2.add_executor("test", 0, executor.AsStdFunction());
    broker1.add_executor(0, 0, broker2);

    ASSERT_THAT(broker1(0, "test", 10), IsTrue());
}