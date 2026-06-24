#ifndef TENSOR_H
#define TENSOR_H

#include <array>
#include <cmath>
#include <vector>
#include <memory>

#ifdef TENSOR_TEST

#include <iostream>

#define PRINT(x) do{ std::cout << x << std::endl; }while(0)

#define TEST_DEF_INIT "Default constructor"		// class()
#define TEST_ARG_INIT "Constructor using argument" 	// class(params)
#define TEST_COPY_INIT "Copy constructor"		// class(const class&)
#define TEST_MOVE_INIT "Move constructor"		// class(class&&)

#define TEST_COPY_ASSIGNMENT "Copy assignment"		// operator=(const class&)
#define TEST_MOVE_ASSIGNMENT "Move assignment"		// operator=(class&&)

#define TEST_ADD_ASSIGNMENT "Addition assignment"	// operator+=(const class&)
#define TEST_SUB_ASSIGNMENT "Subtraction assignment"	// operator-=(const class&)
#define TEST_MUL_ASSIGNMENT "Multiplication assignment"	// operator*=(const class&)
#define TEST_DIV_ASSIGNMENT "Division assignment"	// operator/=(consr class&)

#define TEST_ADD "Addition"				// operator+(class, const class&)
#define TEST_SUB "Subtraction"				// operator-(class, const class&)
#define TEST_MUL "Multiplication"			// operator*(class, const class&)
#define TEST_DIV "Division"				// operator/(class, const class&)

#define TEST_EQUAL "Equal"				// operator==(const class&)
#define TEST_NOT_EQUAL "Not equal"			// operator!=(const class&)

#endif 

namespace sl
{

template <typename T>
class tensor
{
	std::unique_ptr<T[]> data;
	std::unique_ptr<size_t[]> shape;
	size_t size;
	size_t rang;

	public:

	tensor(): data(nullptr), shape(nullptr), size(0), rang(0)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_DEF_INIT);
#endif
	}

	tensor(const std::vector<size_t>& shape)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_ARG_INIT);
#endif
		size_t rang = shape.size();
		this->size = 1;
		this->rang = rang;
		this->shape = std::make_unique<size_t[]>(this->rang);
		for(size_t i = 0; i < rang; i++)
		{
			this->shape[i] = shape[i];
			this->size *= shape[i];
		}

		this->data = std::make_unique<T[]>(this->size);
	}

	tensor(const tensor<T>& other)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_COPY_INIT);
#endif
		this->size = other.size;
		this->rang = other.rang;
		this->shape = std::make_unique<size_t[]>(this->rang);
		this->data = std::make_unique<T[]>(this->size);

		for(size_t i = 0; i < this->rang; i++)
		{
			this->shape[i] = other.shape[i];
		}
		
		for(size_t i = 0; i < this->size; i++)
		{
			this->data[i] = other.data[i];
		}
	}

	tensor(tensor<T>&& other)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_MOVE_INIT);
#endif
		this->size = other.size;
		this->rang = other.rang;
		this->shape = std::move(other.shape);
		this->data = std::move(other.data);
		other.size = 0;
		other.rang = 0;
	}

	tensor<T>& operator=(const tensor<T>& other)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_COPY_ASSIGNMENT);
#endif
		std::unique_ptr<size_t[]> temp_shape;

		if(this->rang != other.rang)
		{
			try
			{
				temp_shape = std::make_unique<size_t[]>(other.rang);
				this->rang = other.rang;
			
				for(size_t i = 0; i < this->rang; i++)
				{
					temp_shape[i] = other.shape[i];
				}

				this->shape = std::move(temp_shape);
			}
			catch(const std::exception&)
			{
				throw;
			}
		}

		std::unique_ptr<T[]> temp_data;

		if(this->size != other.size)
		{
			try
			{
				temp_data = std::make_unique<T[]>(other.size);
				this->size = other.size;

				for(size_t i = 0; i < this->size; i++)
				{
					temp_data[i] = other.data[i];
				}

				this->data = std::move(temp_data);
			}
			catch(const std::exception&)
			{
				throw;
			}
		}
		return *this;
	}

	tensor<T>& operator=(tensor<T>&& other)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_MOVE_ASSIGNMENT);
#endif
		this->size = other.size;
		this->rang = other.rang;
		this->data = std::move(other.data);
		this->shape = std::move(other.shape);
		other.size = 0;
		other.rang = 0;
		return *this;
	}

	tensor<T>& operator+=(const tensor<T>& other)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_ADD_ASSIGNMENT);
#endif
		if(!same_tensor(other))
		{
			throw;
		}
		for(size_t i = 0; i < this->size; i++)
		{
			this->data[i] += other.data[i];
		}
		return *this;
	}

	tensor<T>& operator-=(const tensor<T>& other)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_SUB_ASSIGNMENT);
#endif
		if(!same_tensor(other))
		{
			throw;
		}
		for(size_t i = 0; i < this->size; i++)
		{
			this->data[i] -= other.data[i];
		}
		return *this;
	}

	tensor<T>& operator*=(const tensor<T>& other)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_MUL_ASSIGNMENT);
#endif
		if(!same_tensor(other))
		{
			throw;
		}
		for(size_t i = 0; i < this->size; i++)
		{
			this->data[i] *= other.data[i];
		}
		return *this;
	}

	tensor<T>& operator/=(const tensor<T>& other)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_DIV_ASSIGNMENT);
#endif
		if(!same_tensor(other))
		{
			throw;
		}
		for(size_t i = 0; i < this->size; i++)
		{
			this->data[i] /= other.data[i];
		}
		return *this;
	}

	template <typename Constant>
	tensor<T>& operator+=(Constant x)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_ADD_ASSIGNMENT);
#endif
		for(size_t i = 0; i < this->size; i++)
		{
			this->data[i] += x;
		}
		return *this;
	}

	template <typename Constant>
	tensor<T>& operator-=(Constant x)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_SUB_ASSIGNMENT);
#endif
		for(size_t i = 0; i < this->size; i++)
		{
			this->data[i] -= x;
		}
		return *this;
	}

	template <typename Constant>
	tensor<T>& operator*=(Constant x)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_MUL_ASSIGNMENT);
#endif
		for(size_t i = 0; i < this->size; i++)
		{
			this->data[i] *= x;
		}
		return *this;
	}

	template <typename Constant>
	tensor<T>& operator/=(Constant x)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_DIV_ASSIGNMENT);
#endif
		for(size_t i = 0; i < this->size; i++)
		{
			this->data[i] /= x;
		}
		return *this;
	}

	T& operator[](size_t n)
	{
		return this->data[n];
	}

	template <typename Constant>
	friend tensor<T> operator+(tensor<T> t, Constant x)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_ADD);
#endif
		t += x;
		return std::move(t);
	}

	template <typename Constant>
	friend tensor<T> operator+(Constant x, tensor<T> t)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_ADD);
#endif
		t += x;
		return std::move(t);
	}

	template <typename Constant>
	friend tensor<T> operator-(tensor<T> t, Constant x)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_SUB);
#endif
		t -= x;
		return std::move(t);
	}

	template <typename Constant>
	friend tensor<T> operator-(Constant x, tensor<T> t)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_SUB);
#endif
		t -= x;
		return std::move(t);
	}

	template <typename Constant>
	friend tensor<T> operator*(tensor<T> t, Constant x)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_MUL);
#endif
		t *= x;
		return std::move(t);
	}

	template <typename Constant>
	friend tensor<T> operator*(Constant x, tensor<T> t)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_MUL);
#endif
		t *= x;
		return std::move(t);
	}

	template <typename Constant>
	friend tensor<T> operator/(tensor<T> t, Constant x)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_DIV);
#endif
		t /= x;
		return std::move(t);
	}

	template <typename Constant>
	friend tensor<T> operator/(Constant x, tensor<T> t)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_DIV);
#endif
		t /= x;
		return std::move(t);
	}

	bool operator==(const tensor<T>& other)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_EQUAL);
#endif
		if(this->size != other.size)
		{
			return false;
		}
		if(this->rang != other.rang)
		{
			return false;
		}
		for(size_t i = 0; i < this->rang; i++)
		{
			if(this->shape[i] != other.shape[i])
			{
				return false;
			}
		}
		for(size_t i = 0; i < this->size; i++)
		{
			if(std::abs(this->data[i] - other.data[i]) > 1e-9)
			{
				return false;
			}
		}
		return true;
	}

	bool operator!=(const tensor<T>& other)
	{
#ifdef TENSOR_TEST
		PRINT(TEST_NOT_EQUAL);
#endif
		return !(*this == other);
	}

	~tensor() = default;

	private:

	bool same_tensor(const tensor<T>& other)
	{
		if(this->rang != other.rang)
		{
			false;
		}
		for(size_t i = 0; i < this->rang; i++)
		{
			if(this->shape[i] != other.shape[i])
			{
				false;
			}
		}
		return true;
	}
};

} // namespace sl

#endif
