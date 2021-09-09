#pragma once

#include <iostream>
#include <iterator>
#include <string>

namespace Structures {
    /*! \brief Pair structure
     */
    template <typename T1, typename T2>
    struct Pair {
	T1 first;
	T2 second;
        
        Pair() {
        }

        Pair(T1 a, T2 b) {
                this->first = a;
                this->second = b;
        }
	/*
	inline bool operator=(const Pair<T1, T2>& x) {
	    first = x.first;
	    second = x.second;
	}
	*/
    };

    template <typename T1, typename T2>
    inline bool operator==(const Pair<T1, T2>& x, const Pair<T1, T2>& y) {
	return x.first == y.first && x.second == y.second;
    }

    /*! \brief Templated Vector class
     */
    template <typename T>
    class Vector {
       private:
	T* data;
	size_t _capacity;
	size_t _length;

       public:
	explicit Vector();
	explicit Vector(const Vector<T>& other);
	~Vector();
	void push(T val);
	void insert(T data, size_t index);
	void set(T data, size_t index);
	const T get(size_t index);
	size_t length();
	size_t capacity();
	std::string print();

	T* begin();
	T* end();

	T const& operator[](size_t index) const;  // no assignment
	T& operator[](size_t index);                 // assignment

	inline Structures::Vector<T>& operator=(const Vector<T>& other) {
	    if (this != other) {
		delete[] this->data;
		this->_length = other.length();
		this->_capacity = other.capacity();
		this->data = other.data;
	    }
	    return *this;
	}
    };

    template <typename T>
    inline T* Structures::Vector<T>::begin() {
	return this->length() ? &this->data[0] : nullptr;
    }

    template <typename T>
    inline T* Structures::Vector<T>::end() {
	return this->length() ? &this->data[0] + this->length() : nullptr;
    }

    // Init constructor
    template <typename T>
    inline Structures::Vector<T>::Vector() {
	data = new T[10];
	_capacity = 10;
	_length = 0;
    }

    // Copy constructor
    template <typename T>
    inline Structures::Vector<T>::Vector(const Vector<T>& other) {
	this->_capacity = other._capacity;
	this->_length = other._length;
	this->data = new T[this->_capacity];
	std::copy(other.data, other.data + other.length, this->data);
    }

    template <typename T>
    inline Structures::Vector<T>::~Vector<T>() {
	delete[] data;
    }

    template <typename T>
    inline void Structures::Vector<T>::push(T val) {
	if (this->_length == this->_capacity) {
	    T* new_data = new T[this->_capacity * 2];
	    ::std::copy(this->begin(), this->end(), new_data);
	    delete[] this->data;
	    this->data = new_data;

	    this->_capacity = this->_capacity * 2;
	}
	this->data[this->_length] = val;
	++this->_length;
    }

    template <typename T>
    inline void Structures::Vector<T>::insert(T val, size_t index) {
	throw ::std::runtime_error("Not implemented");
    }

    template <typename T>
    inline void Structures::Vector<T>::set(T val, size_t index) {
	if (this->_length < index + 1) {
	    throw ::std::out_of_range("Set out of bounds");
	}

	this->data[index] = val;
    }

    template <typename T>
    inline const T Structures::Vector<T>::get(size_t index) {
	if (this->_length < index + 1) {
	    throw ::std::out_of_range("Get out of bounds");
	}

	return this->data[index];
    }

    template <typename T>
    inline size_t Structures::Vector<T>::length() {
	return _length;
    }

    template <typename T>
    inline size_t Structures::Vector<T>::capacity() {
	return _capacity;
    }

    template <typename T>
    inline ::std::string Structures::Vector<T>::print() {
	return "Placeholder 1 2 3\n";
    }

    /*
    template <typename T>
    inline ::std::ostream& operator<<(::std::ostream& out,
                                      Structures::Vector<T>& vector) {
        out << vector.print();
    }
    */
    template <typename T>
    inline T const& Structures::Vector<T>::operator[](size_t index) const {
        if (this->_length < index + 1) {
	    throw ::std::out_of_range("Get out of bounds");
	}

	return this->data[index];
    }

    template <typename T>
    inline T& Structures::Vector<T>::operator[](size_t index) {
	if (this->_length < index + 1) {
	    throw ::std::out_of_range("Get out of bounds");
	}

	return this->data[index];
    }

    /*! \brief Templated CSR Matrix
     *         based on Pair struct and Vector class
     */
    template <typename T>
    class CSRMatrix {
       private:
        // n rows, m columns
	size_t n;
	size_t m;
	Vector<Pair<size_t, T>>* rows;

       public:
	CSRMatrix(size_t n, size_t m);
	~CSRMatrix();

	std::string print();

	void set(int x, int y, T val);
	void push(int row, Pair<size_t, T> val);
	const T get(int row, int column);
	Vector<Pair<size_t, T>>& get_row(int row);
	const Vector<Pair<size_t, T>>& operator[](size_t index);
    };

    template <typename T>
    inline const Vector<Pair<size_t, T>>& Structures::CSRMatrix<T>::operator[](size_t index) {
            return rows[index];
    }

    template <typename T>
    inline Structures::CSRMatrix<T>::CSRMatrix(size_t n, size_t m) {
	this->n = n;
	this->m = m;
        
        // array of n pointers to Vector<T>
	this->rows = new Vector<Pair<size_t, T>>[n];
	//for (int i = 0; i < n; ++i) {
	//    this->rows[i] = new Vector<Pair<size_t, T>>();
	//}
    }

    template <typename T>
    inline Structures::CSRMatrix<T>::~CSRMatrix<T>() {
	delete[] this->rows;
    }

    template <typename T>
    inline void Structures::CSRMatrix<T>::set(int x, int y, T val) {}

    template <typename T>
    inline void Structures::CSRMatrix<T>::push(int row, Pair<size_t, T> val) {
	Vector<Pair<size_t, T>>& _row = rows[row];
	_row.push(val);
    }
}  // namespace Structures
