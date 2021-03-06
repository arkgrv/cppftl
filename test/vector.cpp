#include "gtest/gtest.h"
#include <ftl/vector>

using namespace ftl;

TEST(vector, construct_default)
{
    vector<int> x;
    (void)x;
}

TEST(vector, construct_with_size)
{
    const auto size = 50;
    vector<int> x(size);

    ASSERT_EQ(size, x.size());
    ASSERT_EQ(size, x.capacity());
    (void)x;
}

TEST(vector, construct_with_ilist)
{
    vector<int> x{ 1, 2, 3, 4, 5 };
    ASSERT_EQ(5, x.size());
    ASSERT_EQ(5, x.capacity());
    ASSERT_EQ(1, x[0]);
    (void)x;
}

TEST(vector, empty)
{
    vector<int> x;
    ASSERT_TRUE(x.empty());
    ASSERT_EQ(0, x.capacity());
    (void)x;
}

TEST(vector, size)
{

    vector<int> x{ 1, 2, 3, 4 };
    ASSERT_EQ(4, x.size());
    ASSERT_EQ(4, x.capacity());
    (void)x;
}

TEST(vector, capacity)
{
    vector<int> x;
    ASSERT_EQ(0, x.capacity());
    x = vector<int>(50);
    ASSERT_EQ(x.size(), x.capacity());
    (void)x;
}

TEST(vector, front_back)
{
    const vector<int> x{ 1, 2, 3, 4 };
    ASSERT_EQ(1, x.front());
    ASSERT_EQ(4, x.back());
    (void)x;
}

TEST(vector, data)
{
    const vector<int> x{ 1, 2, 3, 4 };
    ASSERT_NE(nullptr, x.data());
    const auto x_data = x.data();
    ASSERT_EQ(x[0], x_data[0]);
    (void)x;
}

TEST(vector, at)
{
    const vector<int> x{ 1, 2, 3, 4, 5, 6 };
    ASSERT_THROW(x.at(6), array_out_of_range);
    ASSERT_NO_THROW(x.at(2));
    ASSERT_EQ(3, x.at(2));
    (void)x;
}

TEST(vector, begin_end)
{
    vector<int> x{ 1, 2, 3, 4, 5 };
    auto begin = x.begin();
    auto end = x.end();
    ASSERT_NE(begin, end);
    auto begin_val = *begin;
    --end;
    auto end_val = *end;
    ASSERT_EQ(begin_val, x.front());
    ASSERT_EQ(end_val, x.back());
    (void)x;
}

TEST(vector, iter_traversal)
{
    vector<int> x{ 1, 1, 1, 1, 1 };
    for (auto it = x.begin(); it != x.end(); ++it)
        ASSERT_EQ(1, *it);
    (void)x;
}

TEST(vector, push_back)
{
    vector<int> x;
    x.push_back(1);
    x.push_back(2);
    ASSERT_EQ(1, x.front());
    ASSERT_EQ(2, x.back());
    (void)x;
}

TEST(vector, emplace_back)
{
    vector<std::string> strs;
    strs.emplace_back("Hello");
    strs.emplace_back("World");
    ASSERT_EQ("Hello", strs[0]);
    ASSERT_EQ("World", strs[1]);
    (void)strs;
}

TEST(vector, resize)
{
    vector<std::string> strs({
        "Barbara",
        "Maurizio",
        "Andrea",
        "Simone"
    });
    strs.emplace_back("Elisabetta");
    ASSERT_EQ(5, strs.size());
    strs.resize(2);
    ASSERT_EQ(2, strs.size());
    ASSERT_EQ(8, strs.capacity());
    (void)strs;
}

TEST(vector, operator_equality)
{
    vector<std::string> strs {
        "Hello",
        "World"
    };
    auto strs1 = strs;
    ASSERT_EQ(strs, strs1);
    (void)strs;
    (void)strs1;
}

TEST(vector, operator_nequality)
{
    vector<std::string> strs {
        "Hello",
        "World"
    };
    auto strs1 = strs;
    strs1.push_back("World2");
    ASSERT_NE(strs, strs1);
    (void)strs;
    (void)strs1;
}

TEST(vector, reverse_iterator_traversal)
{
    vector<int> v { 1, 2, 3, 4 };
    vector<int> u { 4, 3, 2, 1 };

    size_t i = 0;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        ASSERT_EQ(*it, u[i]);
        ++i;
    }
    (void)v;
    (void)u;
}
