#include <iostream>
#include <typeinfo>

template<typename T>
class vector
{
public:
	vector()
	{
		std::cout << "This is not a boolean vector" << std::endl;
	}
};

template <>
class vector<bool>
{
	unsigned char* B_vector;
	size_t capacity_;
	size_t size_;

	template<typename M>
	void resize(M& A)
	{
		size_t size = size_ / 8;
		if (size_ % 8 != 0)
			size++;

		if (typeid(A) != typeid(bool))
		{
			size_t n = 1;
			size_t dimension = size + sizeof(A);

			if (capacity_ < dimension) {
				while (pow(2, n) < dimension)
					n++;
				size_t new_capacity = pow(2, n);
				unsigned char* Bubble = new unsigned char[new_capacity];

				for (size_t i = 0; i < size; ++i) {
					Bubble[i] = B_vector[i];
				}
				delete[] B_vector;
				B_vector = Bubble;
				capacity_ = new_capacity;
			}
		}
		else {
			if (size_ % 8 == 0)
			{
				if (capacity_ < ((size_ / 8) + 1)) {
					size_t new_capacity = capacity_ + 1;
					unsigned char* Bubble = new unsigned char[new_capacity];

					for (size_t i = 0; i < size; ++i) {
						Bubble[i] = B_vector[i];
					}
					delete[] B_vector;
					B_vector = Bubble;
					capacity_ = new_capacity;
				}
			}
		}
	}

public:
	vector() : size_(0), capacity_(0), B_vector(nullptr) {}

	template <typename M>
	void push_back(M& a)
	{
		size_t byteIndex = size_ / 8;
		size_t bitIndex = size_ % 8;
		resize(a);

		if (typeid(a) != typeid(bool))
		{
			int num_bits = sizeof(a) * 8;
			for (int i = num_bits - 1; i >= 0; --i) {
				byteIndex = size_ / 8;
				bitIndex = size_ % 8;
				bool C = ((a >> i) & 1);
				push_back(C);
			}
		}
		else
		{
			if (a == false)
				B_vector[byteIndex] &= ~(1 << bitIndex);

			if (a == true)
				B_vector[byteIndex] |= (1 << bitIndex);
			size_++;

		}
	}

	bool operator[](size_t index) const {
		size_t bytePos = index / 8;
		size_t bitPos = index % 8;
		return (B_vector[bytePos] >> bitPos) & 1;
	}

	size_t size() {
		return size_;
	}

	size_t capacity() {
		return capacity_ * 8;
	}

	void print() {
		for (int index = 0; index < size_; index++)
		{
			if (index % 8 == 0)
				std::cout << ' ';

			int bytePos = index / 8;
			int bitPos = index % 8;
			std::cout << ((B_vector[bytePos] >> bitPos) & 1);
		}
		std::cout << std::endl;
	}

	template<typename M>
	void insert(size_t index, M& a)
	{
		bool broadcast;
		for (size_t i = 0; i < sizeof(a) * 8; i++)
		{
			broadcast = a & (1 << i);
			insert_bool(index, broadcast);
		}
	}

	void insert_bool(size_t index, bool a)
	{
		size_t n = 1;
		size_t byteIndex = index / 8;
		size_t bitIndex = index % 8;
		size_t new_capacity = capacity_;

		if (capacity_ <= byteIndex) {
			while (pow(2, n) < byteIndex)
				n++;
			new_capacity = pow(2, n + 1);
			resize(a);
		}

		size_t size = size_ / 8;
		if (size_ % 8)
			size++;

		unsigned char* Bubble = new unsigned char[new_capacity];
		bool lastIndex;

		for (size_t i = 0; i < size; i++) {
			if (i < byteIndex)
				Bubble[i] = B_vector[i];

			else if (i == byteIndex) {
				lastIndex = (B_vector[i] & 128);
				unsigned char mask_0 = 0xFF;
				unsigned char mask_1 = (mask_0 >> (bitIndex)) & (B_vector[i]);
				mask_1 = mask_1 << 1;

				if (a == false)
					mask_1 &= ~(1 << bitIndex);

				else if (a == true)
					mask_1 |= (1 << bitIndex);

				mask_0 = 0xFF;

				unsigned char mask_2 = (mask_0 >> (8 - bitIndex + 1)) & (B_vector[i]);
				mask_0 = mask_1 + mask_2;

				Bubble[i] = mask_0;
			}
			else {
				if (B_vector[i] == 0 && lastIndex == false) {
					Bubble[i] = 0;
				}

				else {
					unsigned char mask_0 = B_vector[i];
					mask_0 = mask_0 << 1;

					if (lastIndex == false)
						mask_0 &= ~(1);

					else if (lastIndex == true)
						mask_0 |= (1);

					lastIndex = (B_vector[i] & 128);
					Bubble[i] = mask_0;
				}
			}
		}
		delete[] B_vector;
		B_vector = Bubble;
		capacity_ = new_capacity;
		size_++;
	}

	void erase(size_t index)
	{
		size_t n = 1;
		size_t byteIndex = index / 8;
		size_t bitIndex = index % 8;

		size_t size = size_ / 8;
		if (size_ % 8)
			size++;

		bool last = false;
		unsigned char* Bubble = new unsigned char[size];

		for (size_t i = 0; i < size; i++)
		{
			if (i < byteIndex)
				Bubble[i] = B_vector[i];

			if (i == byteIndex)
			{
				if (i + 1 < size )
					last = B_vector[i + 1] & 1;

				unsigned char mask_1, mask_2;

				mask_1 = B_vector[i] & (0xFF >> (8 - bitIndex));
				mask_2 = (B_vector[i] & (0xFF << bitIndex + 1)) >> 1;

				unsigned char mask = mask_1 + mask_2;

				if (last == false)
					mask &= ~(1 << 7);

				else if (last == true)
					mask |= (1 << 7);

				Bubble[i] = mask;
			}

			else {
				unsigned char mask_0 = 0xFF;
				unsigned mask_1 = (mask_0 & (B_vector[i] >> 1));
				if (last == false)
					mask_1 &= ~(128);

				else if (last == true)
					mask_1 |= (128);

				Bubble[i] = mask_1;
			}

		}
	delete[] B_vector;
	B_vector = Bubble;
	size_--;
	}
};

int main()
{
	vector<bool> F;
	bool b = true;
	bool a = false;
	char c = 101;
	F.push_back(b);
	F.push_back(b);
	F.push_back(a);
	F.push_back(b);
	F.push_back(c);

	F.print();

	F.insert_bool(0, a);
	F.insert_bool(0, a);
	F.insert_bool(0, a);

	F.print();

	F.erase(0);
	F.erase(0);
	F.erase(0);

	F.print();
};
