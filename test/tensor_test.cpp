#define TENSOR_TEST

#include "tensor.h"

#define TEST(x) std::cout << #x << std::endl; x; std::cout << std::endl;
#define SPACE std::cout << std::endl;

using sl::tensor;

int main(void)
{
	TEST(tensor<int> t1)
	TEST(tensor<int> t2({3, 3}))
	TEST(tensor<int> t3(t2))
	TEST(tensor<int> t4(std::move(t1)))
	TEST(tensor<int> t5 = t4)
	TEST(tensor<int> t6 = std::move(t2))

	TEST(t2 = t3 = t4)

	TEST(t2 == t3);
	TEST(t2 != t5);
	
	TEST(t3 += 1)
	TEST(t3 -= 1)
	TEST(t3 *= 1)
	TEST(t3 /= 1)

	TEST(t3 = t4 + 1)
	TEST(t3 = t4 - 1)
	TEST(t3 = t4 * 1)
	TEST(t3 = t4 / 1)

	TEST(t3 = 1 + t4)
	TEST(t3 = 1 - t4)
	TEST(t3 = 1 * t4)
	TEST(t3 = 1 / t4)

	TEST(t3 += t4);
	TEST(t3 -= t4);
	TEST(t3 *= t4);
	TEST(t3 /= t4);

	return 0;
}
