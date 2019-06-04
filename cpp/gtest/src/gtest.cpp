#include <gtest/gtest.h>

extern int add(int a, int b);

TEST(testCase, testNormal) {
	EXPECT_EQ(add(2,3), 5);
}

TEST(testCase, testSmallThan0) {
	EXPECT_EQ(add(-2,3), 1);
}

TEST(testCase, testBig) {
	EXPECT_EQ(add(1000000, 1000000), 2000000);
}

int main(int argc, char **argv) {

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
