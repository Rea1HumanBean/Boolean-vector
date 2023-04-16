#include <iostream>

template<typename T>
class BoolVector
{
    bool* BitVector;
    size_t _size = 0;
    size_t _capacity = 1;

    void resize(T& a) {
        size_t dimension = _size + (sizeof(a) * 8);
        if (_capacity < dimension) {
            size_t n = 1;
            while (pow(2, n) < dimension){
                n++;
            }
            size_t new_capacity = pow(2, n) + 8;
            bool* Vector = new bool[new_capacity];
            for (size_t i = 0; i < _size; ++i) {
                Vector[i] = BitVector[i];
            }
            delete[] BitVector;
            BitVector = Vector;
            _capacity = new_capacity;
        }
    }

public:
    BoolVector():_size(0), _capacity(0), BitVector(nullptr) {}
   
    void push_back(T& a) {
        resize(a);
        for (size_t i = 0; i < (sizeof(a) * 8); i++) {
            bool b = a & (1 << i);
            BitVector[_size] = b;
            _size++;
        }
    }

    bool operator [] (size_t index) const {
        if (index < 0 or index >= size()) {
            throw "Index out of range";
        }
        return BitVector[index];
    }

    bool& operator[](int index) {
        if (index < 0 or index >= size()) {
            throw "Index out of range";
        }
        return BitVector[index];
    }

    bool* insert(size_t pos, T& a)
    {
        if (pos < 0 or pos >= size()) {
            throw "Index out of range";
        }
        size_t dimension = _size + (sizeof(a) * 8);
        bool* Vector = new bool[dimension];
        size_t j = 0;
        for (size_t i = 0; i < dimension; i++)
        {
            if (i < pos)
                Vector[i] = BitVector[i];
            else if (i >= pos and i <= (pos + (sizeof(a) * 8)))
            {
                Vector[i] = a & (1 << j);
                j++;
            }
            else
                Vector[i] = BitVector[i - j];
        }
        _size+= sizeof(a) * 8;
        delete[] BitVector;
        BitVector = Vector;
    }

    bool* erase(size_t pos)
    {
        if (pos < 0 or pos >= size()) {
            throw "Index out of range";
        }
        bool* Vector = new bool[size() - 1];
        for (size_t i = 0; i < (size() - 1); i++)
        {
            if (i < pos)
                Vector[i] = BitVector[i];
            else
                Vector[i] = BitVector[i + 1];
        }
        delete[] BitVector;
        BitVector = Vector;
        _size--;
    }

    size_t size() {
        return _size;
    }

    size_t capacity() {
        return _capacity;
    }
};

int main()
{
    char a = 'F';
    char b = 'b';
    BoolVector<char> A;
    A.push_back(a);
    for (auto i = 0; i < A.size(); i++)
        std::cout << A[i] << " ";
    std::cout << A.capacity();
    A.insert(1, b);
    std::cout << std::endl;
    for (auto i = 0; i < A.size(); i++)
        std::cout << A[i] << " ";
    A.erase(1);
    A.erase(0);
    std::cout << std::endl;
    for (auto i = 0; i < A.size(); i++)
        std::cout << A[i] << " ";
};
