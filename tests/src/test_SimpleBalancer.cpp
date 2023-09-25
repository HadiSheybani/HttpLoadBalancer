#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "usecase/simple_balancer.hpp"

using namespace ::testing;

TEST(TestSimpleBalancer, test1) {
    using balancer_t = hlb::SimpleBalancer<bool(int)>;
    using executor_mock_t = MockFunction<bool(int)>;

    balancer_t balancer;
    try {
        balancer(10);
        FAIL();
    } catch (const std::runtime_error& error) {}
}

TEST(TestSimpleBalancer, test2) {
    using balancer_t = hlb::SimpleBalancer<bool(int)>;
    using executor_mock_t = MockFunction<bool(int)>;

    executor_mock_t executor1;
    balancer_t balancer;
    EXPECT_CALL(executor1, Call(10)).WillOnce(Return(true));
    balancer.add_executor(0, executor1.AsStdFunction());
    ASSERT_THAT(balancer(10), IsTrue());
}

TEST(TestSimpleBalancer, test3) {
    using balancer_t = hlb::SimpleBalancer<bool(int)>;
    using executor_mock_t = MockFunction<bool(int)>;

    executor_mock_t executor1;
    balancer_t balancer;
    EXPECT_CALL(executor1, Call(10)).WillOnce(Return(true));
    balancer.add_executor(0, executor1.AsStdFunction());
    balancer.add_executor(1, executor1.AsStdFunction());
    ASSERT_THAT(balancer(10), IsTrue());
    ASSERT_THAT(std::distance(balancer.executors.begin(), balancer.next_executor), Eq(1));
}

TEST(TestSimpleBalancer, test4) {
    using balancer_t = hlb::SimpleBalancer<bool(int)>;
    using executor_mock_t = MockFunction<bool(int)>;

    executor_mock_t executor1;
    executor_mock_t executor2;
    balancer_t balancer;
    EXPECT_CALL(executor1, Call(10)).WillOnce(Return(true));
    EXPECT_CALL(executor2, Call(20)).WillOnce(Return(false));
    balancer.add_executor(0, executor1.AsStdFunction());
    balancer.add_executor(1, executor2.AsStdFunction());
    ASSERT_THAT(balancer(10), IsTrue());
    ASSERT_THAT(balancer(20), IsFalse());
}

TEST(TestSimpleBalancer, test5) {
    using balancer_t = hlb::SimpleBalancer<bool(int)>;
    using executor_mock_t = MockFunction<bool(int)>;

    executor_mock_t executor1;
    executor_mock_t executor2;
    executor_mock_t executor3;
    balancer_t balancer1;
    balancer_t balancer2;
    EXPECT_CALL(executor1, Call(10)).WillOnce(Return(true));
    EXPECT_CALL(executor2, Call(20)).WillOnce(Return(false));
    EXPECT_CALL(executor3, Call(30)).WillOnce(Return(true));
    balancer1.add_executor(0, executor1.AsStdFunction());
    balancer1.add_executor(1, executor2.AsStdFunction());
    balancer2.add_executor(0, balancer1);
    balancer2.add_executor(1, executor3.AsStdFunction());
    ASSERT_THAT(balancer2(10), IsTrue());
    ASSERT_THAT(balancer2(30), IsTrue());
    ASSERT_THAT(balancer2(20), IsFalse());
}


TEST(TestSimpleBalancer, test6) {
    struct Data {
        int copy_counter{ 0 };
        int move_counter{ 0 };

        Data() = default;

        Data(const Data& rhs) {
            copy_counter = rhs.copy_counter + 1;
            move_counter = rhs.move_counter;
        }

        Data(Data&& rhs) {
            copy_counter = rhs.copy_counter;
            move_counter = rhs.move_counter + 1;
        }

        Data& operator=(const Data& rhs) {
            copy_counter = rhs.copy_counter + 1;
            move_counter = rhs.move_counter;
            return *this;
        }

        Data& operator=(Data&& rhs) {
            copy_counter = rhs.copy_counter;
            move_counter = rhs.move_counter + 1;
            return *this;
        }
    };

    using balancer_t = hlb::SimpleBalancer<bool(const Data&)>;
    using executor_mock_t = MockFunction<bool(const Data&)>;

    executor_mock_t executor;
    balancer_t balancer;

    int copy_counter{ 0 };
    int move_counter{ 0 };

    EXPECT_CALL(executor, Call(_)).WillOnce([&](const Data& data) {
        copy_counter = data.copy_counter;
        move_counter = data.move_counter;
        return true;
    });
    balancer.add_executor(0, executor.AsStdFunction());
    Data data;
    balancer(data);
    ASSERT_THAT(copy_counter, Eq(0));
}