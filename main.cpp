#include <iostream>

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
		size_t n = 1;
		size_t dimension = size_ + sizeof(A);

		if (capacity_ < dimension) {
			while (pow(2, n) < dimension)
				n++;
			size_t new_capacity = pow(2, n);
			unsigned char* Bubble = new unsigned char[new_capacity];

			for (size_t i = 0; i < size_; ++i) {
				Bubble[i] = B_vector[i];
			}

			delete[] B_vector;
			B_vector = Bubble;
			capacity_ = new_capacity;
			size_ = dimension;
		}
	}

public:
	vector() : size_(0), capacity_(0), B_vector(nullptr) {}

	template <typename M>
	void push_back(M& a)
	{
		size_t started = size_;
		resize(a);
		size_t value = 0;

		if (sizeof(a) == 1) {
			B_vector[started] = a;
		}

		if (1 < sizeof(a)) {
			while (value < sizeof(a))
			{
				B_vector[started + value] = ((a >> ((value) * 8)) & 0xFF);
				value++;
			}
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
		return capacity_;
	}

	void print() {
		for (size_t i = 0; i < size_; i++)
			std::cout << (int)B_vector[i] << ' ';
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

		if (capacity_ < byteIndex) {
			while (pow(2, n) < byteIndex)
				n++;
			new_capacity = pow(2, n + 1);
			resize(a);
		}

		unsigned char* Bubble = new unsigned char[new_capacity];
		bool lastIndex = false;

		for (size_t i = 0; i < size_; i++) {
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
	}

	void erase(size_t index)
	{
		unsigned char* Bubble = new unsigned char[size_ - 1];

		for (size_t i = 0; i < size_ - 1; i++) {
			if (i < index) {
				Bubble[i] = B_vector[i];
			}
			else
				Bubble[i] = B_vector[i + 1];
		}
		delete[] B_vector;
		B_vector = Bubble;
		size_ = size_ - 1;
	}

	void erase_bool(size_t index)
	{
		size_t n = 1;
		size_t byteIndex = index / 8;
		size_t bitIndex = index % 8;
		unsigned char* Bubble = new unsigned char[size_];

		for (size_t i = 0; i < size_; i++) {
			bool lastIndex = false;

			if (i < size_ - 1)
				lastIndex = (B_vector[i + 1] & 1);

			if (i < byteIndex)
				Bubble[i] = B_vector[i];

			else if (i == byteIndex) {
				unsigned char mask_0 = 0xFF;
				unsigned char mask_1 = (mask_0 >> (8 - bitIndex) & B_vector[i]);
				unsigned char mask_2 = (mask_0 >> 1);
				for (size_t i = 0; i < bitIndex; i++)
					mask_2 = (mask_2 &= ~(1) << i);

				mask_2 = mask_2 & (B_vector[i] >> 1);
				mask_0 = mask_1 + mask_2;

				if (lastIndex == false)
					mask_0 &= ~(128);

				else if (lastIndex == true)
					mask_0 |= (128);

				Bubble[i] = mask_0;
			}

			else {
				unsigned char mask_0 = 0xFF;
				unsigned mask_1 = (mask_0 & (B_vector[i] >> 1));
				if (lastIndex == false)
					mask_1 &= ~(128);

				else if (lastIndex == true)
					mask_1 |= (128);

				Bubble[i] = mask_1;
			}

		}
		delete[] B_vector;
		B_vector = Bubble;
	}

};

int main()
{
	vector<bool> F;
	int a = 511;

	F.push_back(a);
	F.print();

	for (size_t i = 0; i < F.size() * 8; i++)
	{
		if (i % 8 == 0)
			std::cout << ' ';
		std::cout << F[i];
	}
	std::cout << std::endl;

	bool u = true;

	F.insert_bool(16, u);
	F.insert_bool(16, u);
	F.insert_bool(16, u);
	F.print();


	char X = 40;
	F.insert(8, X);
	F.print();
	for (size_t i = 0; i < F.size() * 8; i++)
	{
		if (i % 8 == 0)
			std::cout << ' ';
		std::cout << F[i];
	}
	std::cout << std::endl;


	F.erase_bool(9);
	F.erase_bool(9);
	F.erase_bool(7);


	for (size_t i = 0; i < F.size() * 8; i++)
	{
		if (i % 8 == 0)
			std::cout << ' ';
		std::cout << F[i];
	}
	std::cout << std::endl;
	F.print();
};
