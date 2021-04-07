#ifndef FDT_RANDOM_VECTOR_H
#define FDT_RANDOM_VECTOR_H

#include <memory>
#include <initializer_list>
#include <utility>
#include <cmath>
#include <fdt/iterator.h>
#include <fdt/exception.h>

namespace fdt {
	template <typename T, class Allocator = std::allocator<T> >
	class vector {
	public:
		using value_type = T;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using const_pointer = const T*;
		using iterator = rand_iterator<T>;
		using const_iterator = const rand_iterator<T>;
		using reverse_iterator = reverse_rand_iterator<T>;
		using const_reverse_iterator = const reverse_rand_iterator<T>;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using allocator_type = Allocator;

		/**
		 * @brief Default constructor.
		 */
		constexpr vector() : data_(nullptr), size_(0), capacity_(0) {}

		/**
		 * @brief Pre-allocation constructor. Creates a vector of the given
		 * size with pre-allocated memory.
		 */
		constexpr vector(std::size_t n)
			: data_(allocator_traits::allocate(alloc_, n)), size_(n),
				capacity_(n)
		{
			for (size_t i = 0; i < size_; ++i)
				allocator_traits::construct(alloc_, data_ + i);
		}

		/**
		 * @brief Copy constructor. Copies the content of another vector
		 * preserving the data stored inside the container. The complexity of
		 * this constructor is linear in size of the vector.
		 */
		constexpr vector(const vector& other)
			: data_(allocator_traits::allocate(alloc_, other.capacity_)),
			size_(other.size_),
			capacity_(other.capacity_)
		{
			for (size_t i = 0; i < other.size(); ++i)
				allocator_traits::construct(
					alloc_, data_ + i, *(other.data_ + i)
				);
		}

		/**
		 * @brief Move constructor. Moves the pointer of another vector
		 * provided as rvalue-reference, avoiding unnecessary copies
		 * wherever the content of the "original" container must not
		 * be preserved. The complexity of this constructor is constant.
		 * 
		 */
		constexpr vector(vector&& other) noexcept
			: data_(std::move(other.data_)),
			size_(other.size_), capacity_(other.capacity_)
		{
			other.data_ = nullptr;
			other.size_ = 0;
			other.capacity_ = 0;
		}

		/**
		 * @brief Initializer list constructor. Takes an initializer
		 * list which will be used to initialize the content of the
		 * container, provided that the specified literal values are
		 * type-compliant with the template type parameter of the
		 * vector. The complexity of this constructor is linear in
		 * length of the initializer list.
		 */
		constexpr vector(std::initializer_list<T> init)
			: data_(allocator_traits::allocate(alloc_, init.size())),
			size_(init.size()), capacity_(init.size())
		{

			for (size_t i = 0; i < init.size(); ++i)
				allocator_traits::construct(alloc_, data_ + i,
					*(init.begin() + i));
		}

		/**
		 * @brief Assigns another vector to this instance copying
		 * the contents of the other container and preserving the
		 * original object. The complexity of this operator call
		 * is linear in the size of the vector.
		 * @param other vector to copy-assign.
		 * @return constexpr reference assigned vector. 
		 */
		constexpr vector& operator=(const vector& other)
		{
			if (&other == this) return *this;
			
			vector copy(other);
			copy.swap(*this);
			return *this;
		}

		/**
		 * @brief Assigns another vector to this instanche using move
		 * semantics. The pointer to the unerlying data structure is moved
		 * and the other object is reset to an empty state. The complexity
		 * of this operator call is constant.
		 * @param other reference to a temporary rvalue vector.
		 * @return constexpr reference 
		 */
		constexpr vector& operator=(vector&& other) noexcept
		{
			if (&other == this) return *this;
			
			vector copy(std::move(other));
			copy.swap(*this);
			return *this;
		}

		constexpr vector& operator=(std::initializer_list<T> init)
		{
			vector copy(init);
			copy.swap(*this);
			return *this; 
		}

		~vector()
		{
			allocator_traits::deallocate(alloc_, data_, capacity_);
		}

		constexpr allocator_type get_allocator() const noexcept
		{
			return alloc_;
		}

		/**
		 * @brief Returns a boolean after checking if the vector is
		 * empty. 
		 * @return true if vector is empty.
		 * @return false if vector is not empty.
		 */
		[[nodiscard]]
		constexpr bool empty() const noexcept { return size_ == 0 || capacity_ == 0; }

		/**
		 * @brief Returns the size of the vector (number of elements).
		 * @return constexpr size_type size of vector.
		 */
		constexpr size_type size() const { return size_; }
		
		/**
		 * @brief Returns the capacity of the vector, which represents
		 * the number of allocated slots able to store elements. Usually,
		 * memory locations past size() are not initialized or constructed.
		 * @return constexpr size_type capacity of the vector.
		 */
		constexpr size_type capacity() const { return capacity_; }

		/**
		 * @brief Returns the first element of the vector.
		 * @return constexpr reference to the first element of the vector.
		 */
		constexpr reference front() { return *data_; }
		constexpr const_reference front() const { return *data_; }

		/**
		 * @brief Returns the last element of the vector.
		 * @return constexpr reference to the last element of the vector.
		 */
		constexpr reference back() { return *(data_ + size_ - 1); }
		constexpr const_reference back() const { return *(data_ + size_ - 1); }

		/**
		 * @brief Returns a pointer to the underlying data structure
		 * containing all the elements of the vector.
		 * @return constexpr pointer to the underlying memory area.
		 */
		constexpr pointer data() { return data_; }
		constexpr const_pointer data() const { return data_; }

		/**
		 * @brief Returns an element with position i using bounds-checked
		 * array access. An exception of type array_out_of_range() is thrown
		 * if i is out of bounds.
		 * @param i position.
		 * @return constexpr reference to the element.
		 */
		constexpr reference at(size_t i)
		{
			if (i >= size_) throw array_out_of_range();
			return *(data_ + i);
		}
		constexpr const_reference at(size_t i) const
		{
			if (i >= size_) throw array_out_of_range();
			return *(data_ + i);
		}

		/**
		 * @brief Returns an element with position i.
		 * @param i position.
		 * @return constexpr reference to the element.
		 */
		constexpr reference operator[](size_t i) noexcept
		{
			return *(data_ + i);
		}
		constexpr const_reference operator[](size_t i) const noexcept
		{
			return *(data_ + i);
		}

		/**
		 * @brief Returns an iterator to the beginning of the vector.
		 * @return constexpr iterator value to the start of the vector.
		 */
		constexpr iterator begin() { return iterator(data_); }
		constexpr const_iterator begin() const noexcept
		{
			return iterator(data_);
		}

		/**
		 * @brief Returns a constant iterator to the beginning of the vector.
		 * @return constexpr const_iterator value to the start of the vector.
		 */
		constexpr const_iterator cbegin() const noexcept
		{
			return iterator(data_);
		}

		/**
		 * @brief Returns an iterator to the end of the vector.
		 * @return constexpr iterator value to the end of the vector.
		 */
		constexpr iterator end() { return iterator(data_ + size_); }

		constexpr const_iterator end() const noexcept
		{
			return iterator(data_ + size_);
		}

		/**
		 * @brief Returns a constant iterator to the end of the vector.
		 * @return constexpr const_iterator value to the end of the vector.
		 */
		constexpr const_iterator cend() const noexcept
		{
			return iterator(data_ + size_);
		}

		/**
		 * @brief Returns a reverse iterator to the beginning of the vector.
		 * @return constexpr reverse_iterator value to the start of the vector.
		 */
		constexpr reverse_iterator rbegin() { return reverse_iterator(data_ + size_ - 1); }
		constexpr const_reverse_iterator rbegin() const noexcept
		{
			return reverse_iterator(data_ + size_ - 1);
		}

		/**
		 * @brief Returns a constant reverse iterator to the beginning of the
		 * vector.
		 * @return constexpr const_reverse_iterator value to the start of the
		 * vector.
		 */
		constexpr const_reverse_iterator crbegin() const noexcept
		{
			return reverse_iterator(data_ + size_ - 1);
		}

		/**
		 * @brief Returns a reverse iterator to the end of the vector.
		 * @return constexpr reverse_iterator value to the end of the vector.
		 */
		constexpr reverse_iterator rend() { return reverse_iterator(data_); }
		constexpr const_reverse_iterator rend() const noexcept
		{
			return reverse_iterator(data_);
		}

		/**
		 * @brief Returns a constant reverse iterator to the end of the vector.
		 * @return constexpr const_reverse_iterator value to the end of the
		 * vector.
		 */
		constexpr const_reverse_iterator crend() const noexcept
		{
			return reverse_iterator(data_);
		}

		/**
		 * @brief Preallocates n elements in order to speed up insertions
		 * using push_back and emplace_back functions. The complexity
		 * of this function call is, at most, linear in size of n.
		 * @param n number of slots to preallocate.
		 */
		constexpr void reserve(size_t n)
		{
			if (n <= capacity_) return;
			auto* a = data_;
			a = allocator_traits::allocate(alloc_, n);

			for (size_t i = 0; i < size_; ++i)
				allocator_traits::construct(alloc_, a + i, *(data_ + i));

			// de-allocating old buffer
			allocator_traits::deallocate(alloc_, data_, capacity_);
			capacity_ = n;
			data_ = a;
		}

		/**
		 * @brief Resizes the vector to the given size. If the provided value
		 * is larger than the vector's current size, a reallocation happens
		 * automatically. If the provided value is smaller than the current
		 * size, size is set and elements past its index are discarded.
		 * @param n new size of the vector.
		 */
		constexpr void resize(size_t n)
		{
			if (n <= size_) {
				size_ = n;
				return;
			}

			reserve(alloc_size(n));
			size_ = n;
		}

		/**
		 * @brief Resizes the vector in order to perfectly fit the number of
		 * elements inside the container. It is not recommended to call
		 * shrink_to_fit() if a large number of insertions is planned in the
		 * future, because it will perform relocations, decreasing the
		 * container's efficiency.
		 */
		constexpr void shrink_to_fit()
		{
			if (size_ == capacity_) return;
			auto* a = data_;

			a = allocator_traits::allocate(alloc_, size_);
			std::copy(data_, data_ + size_, a);

			allocator_traits::deallocate(alloc_, data_, capacity_);
			capacity_ = size_;
			data_ = a;
		}

		/**
		 * @brief Adds an element of the given value to the end of the vector.
		 * @param value element to add to the end of the vector.
		 */
		constexpr void push_back(const T& value)
		{
			if (size_ + 1 < capacity_) {
				size_++;
				allocator_traits::construct(alloc_, data_ + size_ - 1, value);
			} else {
				reserve(alloc_size(capacity_ + 1));
				size_++;
				allocator_traits::construct(alloc_, data_ + size_ - 1, value);
			}
		}

		/**
		 * @brief Constructs a new element at the end of the vector.
		 * @tparam Args variadic template parameter of constructor arguments.
		 * @param args variadic constructor arguments.
		 * @return constexpr reference to the constructed element.
		 */
		template <typename... Args>
		constexpr reference emplace_back(Args&&... args)
		{
			if (size_ + 1 < capacity_) {
				size_++;
				allocator_traits::construct(alloc_, data_ + size_ - 1, 
					std::forward<Args>(args)...);
				return *(data_ + size_ - 1);
			} else {
				reserve(alloc_size(capacity_ + 1));
				size_++;
				allocator_traits::construct(alloc_, data_ + size_ - 1,
					std::forward<Args>(args)...);
				return *(data_ + size_ - 1);
			}
		}

		/**
		 * @brief Removes an element at the end of the vector by decreasing
		 * the vector's size. This operation has constant complexity.
		 */
		constexpr void pop_back()
		{
			if (size_ == 0) return;
			size_--;
		}

		/**
		 * @brief swaps the contents of another vector with the current instance of
		 * the vector
		 * @param other another vector instance to swap elements with.
		 */
		constexpr void swap(vector& other) noexcept
		{
			std::swap(other.data_, data_);
			std::swap(other.capacity_, capacity_);
			std::swap(other.size_, size_);
			std::swap(other.alloc_, alloc_);
		}

	private:
		using allocator_traits = std::allocator_traits<Allocator>;
		T* data_ = nullptr;
		size_type size_;
		size_type capacity_;
		allocator_type alloc_;

		constexpr static size_t alloc_size(size_t s)
		{
			if constexpr (sizeof(s) == 8) {
				--s;
				s |= s >> 1;
				s |= s >> 2;
				s |= s >> 4;
				s |= s >> 8;
				s |= s >> 16;
				s |= s >> 32;
				++s;
				return s;
			} else if constexpr (sizeof(s) == 4) {
				--s;
				s |= s >> 1;
				s |= s >> 2;
				s |= s >> 4;
				s |= s >> 8;
				s |= s >> 16;
				++s;
				return s;
			} else {
				return std::pow(2, ceil(log(s) / log(2)));
			}
		}
	};

	template <typename Ty>
	constexpr void swap(vector<Ty>& l, vector<Ty>& r)
	{
		l.swap(r);
	}

	/**
	 * @brief Lexicographically compares the values of two different vectors
	 * to check if they are equal.
	 * @tparam Ty deduced type of the container.
	 * @param l vector container.
	 * @param r vector container.
	 * @return true if the elements inside the vector are all equal.
	 * @return false if the elements inside the vector are different.
	 */
	template <typename Ty>
	constexpr bool operator==(const vector<Ty>& l, const vector<Ty>& r)
	{
		if (l.data() == nullptr || r.data() == nullptr) return false;
		if (l.size() != r.size()) return false;
		for (size_t i = 0; i < l.size(); ++i)
			if (l[i] != r[i]) return false;
		return true;
	}

	template <typename Ty>
	constexpr bool operator!=(const vector<Ty>& l, const vector<Ty>& r)
	{
		return !(l == r);
	}

	/**
	 * @brief Performs the summation of two vectors, provided that the type
	 * of the container is arithmetic-compliant or provides correctly
	 * overloaded operators.
	 * @tparam Ty deduced type of the container.
	 * @param l vector.
	 * @param r vector.
	 * @return constexpr auto new vector containing the summation result.
	 */
	template <typename Ty>
	constexpr auto operator+(const vector<Ty>& l, const vector<Ty>& r)
	{
		if (l.size() != r.size()) throw vector_size_mismatch();
		vector<Ty> sum;
		for (size_t i = 0; i < l.size(); ++i)
			sum.emplace_back(l[i] + r[i]);
		return sum;
	}

	/**
	 * @brief Performs the subtraction of two vectors, provided that the type
	 * of the container is arithmetic-compliant or provides correctly
	 * overloaded operators.
	 * @tparam Ty deduced type of the container.
	 * @param l vector.
	 * @param r vector.
	 * @return constexpr auto new vector containing the subtraction result.
	 */
	template <typename Ty>
	constexpr auto operator-(const vector<Ty>& l, const vector<Ty>& r)
	{
		if (l.size() != r.size()) throw vector_size_mismatch();
		vector<Ty> sum;
		for (size_t i = 0; i < l.size(); ++i)
			sum.emplace_back(l[i] - r[i]);
		return sum;
	}
}

#endif
