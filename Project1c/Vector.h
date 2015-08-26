// Vector.h -- header file for Vector data structure project
/*Feb 26th,22:52 2015*/
/*EID:zh3499*/

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cstdint>
#include <stdexcept>
#include <utility>
//Utility gives std::rel_ops which will fill in relational
//iterator operations so long as you provide the
//operators discussed in class.  In any case, ensure that
//all operations listed in this website are legal for your
//iterators:
//http://www.cplusplus.com/reference/iterator/RandomAccessIterator/

using namespace std::rel_ops;
using std::begin;
using std::end;
namespace epl{

class invalid_iterator {
	public:
	enum SeverityLevel {SEVERE,MODERATE,MILD,WARNING};
	SeverityLevel level;

	invalid_iterator(SeverityLevel level = SEVERE){ this->level = level; }
	virtual const char* what() const {
    switch(level){
      case WARNING:   return "Warning"; // not used in Spring 2015
      case MILD:      return "Mild";
      case MODERATE:  return "Moderate";
      case SEVERE:    return "Severe";
      default:        return "ERROR"; // should not be used
    }
	}
};



template <typename T>
class vector {
private:
	int capacity;
	int length;
	T* elem;
	int head;//number of elements in head
	int tail;//number of elements in tail
	int version;
	//version ==0 : normal
	//version ==1 : severe
	//version ==2 : moderate, reallocate, new assignment
	//version ==3 : mild

	void destroy(void) {
		for (int i = 0; i < tail; i++) {
			elem[i].~T();
		}
		for (int i = 0; i < head; i++) {
			elem[capacity - 1 - i].~T();
		}
		operator delete(elem);
	}

	void copy(const vector<T>& that) {
		length = that.length; tail = that.tail; head = that.head; capacity = that.capacity; version = that.version;
		if (that.elem) {
			elem = reinterpret_cast<T*>(::operator new(sizeof(T)*capacity));
			for (int i = 0; i < tail; i++) new (&elem[i]) T{ that.elem[i] };
			for (int i = 0; i < head; i++) new (&elem[capacity - 1 - i]) T{ that.elem[capacity - 1 - i] };
		}
		else {
			elem = nullptr;
		}
	}

public:

	class iterator;

	class const_iterator;
	/*constructor*/
	vector(void) :capacity(8), length(0) {
		//elem = (T*)::operator new(sizeof(T)*8);
		elem = reinterpret_cast<T*>(::operator new(sizeof(T) * 8));
		head = 0;
		tail = 0;
		version = 0;
	};
	/*copy constructor*/
	vector(const vector<T>& other) {
		copy(other);
	};

	template<typename inputiterator>
	vector(inputiterator b, inputiterator e) {

		typename std::iterator_traits<inputiterator>::iterator_category x{};

		vector_iterator_ini(b, e, x);
		//vector_iterator_ini(b, e, typename iterator_traits<inputiterator>::iterator_category{});
	}

	template<typename inputiterator>		//only random access
	void vector_iterator_ini(inputiterator b, inputiterator e, std::random_access_iterator_tag) {
		length = e - b;
		if (length == 0) {
			capacity = 8;version = 0;
			head = 0; tail = 0;
			elem = reinterpret_cast<T*>(::operator new(sizeof(T) * 8));
			//elem = (T*) new(8 * sizeof(T));
		}
		else {
			capacity = length; version = 0;
			head = 0; tail = length;
			elem = reinterpret_cast<T*>(::operator new(sizeof(T) * capacity));
			//elem = (T*) new (capacity * sizeof(T));
			for (int i = 0; i < length; i++) {
				new (elem + i) T{ b[i] };
			}
		}
	};

	template<typename inputiterator>		//all others
	void vector_iterator_ini(inputiterator b, inputiterator e, std::input_iterator_tag) {

		if (b == e) {
			capacity = 8; version = 0; length = 0;
			head = 0; tail = 0;
			elem = reinterpret_cast<T*>(::operator new(sizeof(T) * 8));
			//elem = (T*) new(8 * sizeof(T));
		}
		else {
			//int le = 0;
			capacity = 30; version = 0; length = 0; length = 0;
			head = 0; tail = 0;
			int i = 0;
			elem = reinterpret_cast<T*>(::operator new(sizeof(T) * capacity));
				while(b!=e){

				new (elem + i) T{b};
				++b; i++; tail++; length++;
			}
		}
	};


	/*copy assignment*/
	vector<T>& operator=(const vector<T>& that) {
		if (this != &that) {

			destroy();
			copy(that);
			this->version = 2;
		}
		return *this;

	};

	/* explict constructor*/
	explicit vector(uint64_t n) {
		if (n > 0) {
			capacity = n;
			tail = n;
			head = 0;
			elem = reinterpret_cast<T*>(::operator new(sizeof(T)*capacity));
			for (int i = 0; i < capacity; i++) {
				new (&elem[i]) T{};
			}
			length = n;
		}
		else {
			capacity = 8; elem = (T*)::operator new(8 * sizeof(T)); length = 0;
		}
		version = 0;
	};

	vector(std::initializer_list<T> li) {
		length = li.size(); version = 0;
		if (length == 0) {
			capacity = 8; elem = (T*)::operator new(8 * sizeof(T)); tail = 0; head = 0;
		}
		else
		{
			capacity = length; elem = (T*)::operator new(8 * sizeof(T)); tail = length; head = 0;
			auto p = li.begin();
			for (int i = 0; i < length; i++) {
				new (elem + i) T((T&&)p[i]);
			}
		}
	}

	~vector(void) {/*destructor function*/

		destroy();
	};
	uint64_t size(void)const {
		/* const here means that this function won't change any member variable*/
		return length;
	};
	T& operator[](uint64_t k) {
		if (k >= length) { throw std::out_of_range{ "index out of range" }; }
		else {
			if (k >= head)
				return *(elem + k - head);
			else return *(elem + capacity - head + k);
		}
	};
	const T& operator[](uint64_t k) const {/*what's the differenece between this and above function*/
		const T* tmp1 = elem + k - head;
		const T* tmp2 = elem + capacity - head + k;
		if (k >= length) { throw std::out_of_range{ "index out of range" }; }
		else if (k >= head) return *tmp1;
		if (k < head)
		{
			return *tmp2;
		}
	}
	void push_back(const T& co) {

		if (length == capacity) {
			version = 2;

			T *p = (T*)::operator new(2 * capacity*sizeof(T));
			new (&p[tail]) T(co);
			//capacity = 2 * capacity;
			for (int i = 0; i < tail; i++) {
				new (&p[i]) T((T&&)elem[i]);

				//elem[i].~T();
			}
			for (int i = 0; i < head; i++) {
				new (&p[2 * capacity - head + i]) T((T&&)elem[capacity - head + i]);
				//elem[capacity-head+i].~T();
			}
			capacity = 2 * capacity;
			operator delete(elem);
			elem = p;
			length += 1;
			tail += 1;
		}
		else {
			version = 3;
			length += 1;
			tail += 1;
			//std::cout<<"still pushback by copy"<<std::endl;

			new (&elem[tail - 1]) T(co);
			//std::cout<<"problem is later"<<std::endl;

		}
		//std::cout<<elem[tail-1];
		return;
	}
	void push_front(const T& co) {
		if (length == capacity) {
			version = 2;
			T *p = (T*)::operator new(2 * capacity*sizeof(T));
			new (&p[2 * capacity - head - 1]) T(co);
			for (int i = 0; i < tail; i++) {
				new (&p[i]) T((T&&)elem[i]);
			}
			for (int i = 0; i < head; i++) {
				new (&p[2 * capacity - head + i]) T((T&&)elem[capacity - head + i]);
			}
			//destroy();
			capacity = 2 * capacity;
			elem = p;
			length += 1;
			head += 1;
			//new (&elem[capacity-head]) T(co);
		}
		else {
			length += 1;
			head += 1;
			version = 3;
			new (&elem[capacity - head]) T(co);
		}
		return;
	}
	void pop_back(void) {
		if (length == 0) { throw std::out_of_range{ "no element in the vector!" }; }

		if (tail > 0) {
			elem[tail - 1].~T();
			tail -= 1;
			length -= 1;
			version = 3;
		}
		else if (length == 1) {
			elem[capacity - 1].~T();
			length -= 1;
			head -= 1;
			version = 3;
		}
		else {
			for (int i = 0; i < length / 2; i++) {
				elem[tail] = T((T&&)elem[capacity - head / 2]);
				elem[capacity - head / 2] = T((T&&)elem[capacity - head]);
				elem[capacity - head].~T();
				head -= 1;
				tail += 1;
				version = 3;
			}
			version = 3;
			elem[tail - 1].~T();
			tail -= 1;
			length -= 1;
			// elem[length-1].~T();
		}
	}
	void pop_front(void) {
		if (length == 0) { throw std::out_of_range{ "no element in the vector!" }; }
		if (tail == capacity) {
			version = 3;
			elem[0].~T();
			tail = 0;
			head = capacity - 1;
			length -= 1;
			return;
		}
		if (head == 0) {
			version = 3;
			for (int i = 0; i < length / 2; i++) {
				elem[capacity - length / 2 + i] = T((T&&)elem[i]);
				elem[i] = T((T&&)elem[i + length / 2]);
				elem[i + length / 2].~T();
			}
		}
		version =3;
		elem[capacity - head].~T();
		head -= 1;
		length -= 1;
	}
	//move constructor
	vector(vector<T>&& that) :capacity(0), length(0), elem(nullptr), head(0), tail(0) {
		length = that.length; capacity = that.capacity; head = that.head; tail = that.tail;
		elem = that.elem;
		that.length = 0; that.capacity = 0; that.head = 0; that.tail = 0;
		that.elem = nullptr;
		that.version = 3;

	}
	// move assignment
	vector<T>& operator=(vector<T>&& that) {
		if (this != &that) {
			length = that.length; capacity = that.capacity; head = that.head; tail = that.tail;
			elem = that.elem;
			that.length = 0; that.capacity = 0; that.head = 0; that.tail = 0;
			that.elem = nullptr;
			that.version = 2;	//the vector that has been moved
			this->version = 2;
		}
		return *this;
	}
	void push_back(T&& that) {
		if (length == capacity) {
			version = 2;
			T *p = (T*)::operator new(2 * capacity*sizeof(T));
			new (&p[tail]) T((T&&)that);//why??
			//capacity = 2 * capacity;
			for (int i = 0; i < tail; i++) {
				new (&p[i]) T((T&&)elem[i]);
			}
			for (int i = 0; i < head; i++) {
				new (&p[2 * capacity - head + i]) T((T&&)elem[capacity - head + i]);
			}
			capacity = 2 * capacity;
			operator delete(elem);
			elem = p; length += 1;
			tail += 1;
		}
		else {
			version = 3;
			length += 1;
			tail += 1;
			new (&elem[tail - 1]) T((T&&)that);
		}
		return;
	}
	int prversion(void) {
		return version;
	}
	void push_front(T&& that) {
		if (length == capacity) {
			version = 2;
			T *p = (T*)::operator new(2 * capacity*sizeof(T));
			new (&p[2 * capacity - head - 1]) T((T&&)that);	//edit here
			for (int i = 0; i < tail; i++) { new (&p[i]) T((T&&)elem[i]); }
			for (int i = 0; i < head; i++) {
				new (&p[2 * capacity - head + i]) T((T&&)elem[capacity - head + i]);
			}
			capacity = 2 * capacity;
			elem = p; length += 1;
			head += 1;
		}
		else {
			version = 3;
			length += 1;
			head += 1;
			new (&elem[capacity - head]) T((T&&)that);
		}
		return;
	}

		iterator begin(void){
			return iterator(this,  0);
		};

		iterator end(void) {
			return iterator(this,  length);
		};

		const_iterator begin(void) const{
			return const_iterator(this, 0);
		};

		const_iterator end(void) const{
			return const_iterator(this, length);
			//return const_iterator(this, 0);
		};

		template <typename... Args>
		void emplace_back(Args&&... args) {
			if (length == capacity) {
				version = 2;
				T* p = (T*) operator new (capacity * 2 * sizeof(T));
				new (&p[tail]) T(std::forward<Args>(args)...);

				for (int i = 0; i < tail; i++) {
					new (&p[i]) T((T&&)elem[i]);
				}
				for (int i = 0; i < head; i++) {
					new (&p[2 * capacity - head + i]) T((T&&)elem[capacity - head + i]);
				}
				capacity = 2 * capacity;
				operator delete(elem);
				elem = p; length += 1;
				tail += 1;
			}
			else {
				version = 3;
				length += 1;
				tail += 1;
				//std::cout<<"still pushback by copy"<<std::endl;

				new (&elem[tail - 1]) T(std::forward<Args>(args)...);
				//std::cout<<"problem is later"<<std::endl;

			}

		};



};

template<typename T>
class vector<T>::const_iterator {
public:
		using value_type = T;
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = int;
		using pointer = T*;
		using reference = T&;// ????

protected:
		int iterator_version;
		vector<T>* buffer;//point to the vector, which is the start address of vector
		//T* position = nullptr;
		int index;
		int valid;//stand for valid or not
		T* front;
		T* back;
		T* top;
		T* end;

	public:
		const_iterator(void) {};
		const_iterator(const vector<T>* p,int addr) {
			//position =addr;
			buffer = (vector<T>*) p;
			index = addr;
			if ((addr >= 0) && (addr < buffer->length)) valid = 1;
			else valid = 0;
			front = p->elem;
			back = p->elem+p->capacity;
			end = front + p->tail;
			top = back - p->head;
			iterator_version = p->version;
		}

		const_iterator(const const_iterator& that) {
			that.judge();
				this->iterator_version = that.iterator_version;
				this->valid = that.valid;
				//this->position = that.position;
				this->index = that.index;
				this->front = that.front;
				this->back = that.back;
				this->top = that.top;
				this->end = that.end;
				this->buffer = that.buffer;

			//return (*this);
		}
		/*void pr(void) {
			printf("index=%d\n", index);
		}*/
		T& operator *(void) const {
			//printf("vector version is %d\n", buffer->version);
			judge();
			return buffer->operator[](index);
		}

		const_iterator& operator ++(void) {//prefix ++
			judge();
			index++;
			return *this;
		}
		const_iterator operator++(int) {//postfix ++
			const_iterator t{ *this };
			judge();
			// copy myself
			index++; // increment myself (pre-increment)
			return t; // return the copy of my old value
		}

		const_iterator operator =(const const_iterator& that) {
			that.judge();
			if (*this != that) {
			this->iterator_version = that.iterator_version;
			//this->position = that.position;
			buffer = that.buffer;
			index = that.index;
			valid = that.valid;
			this->front = that.front;
			this->back = that.back;
			this->top = that.top;
			this->end = that.end;
		}
			return (*this);
		}

		bool operator==(const const_iterator& that) const {
			judge();
			that.judge();
			bool t;
			t = (this->iterator_version == that.iterator_version) && (this->buffer == that.buffer) && (this->front == that.front) && (this->back == that.back) && (this->top == that.top);
			t = t&&(this->end == that.end)&&(this->index==that.index);
			//printf("t=%d\n", t);
			return t;
		}
		bool operator!=(const const_iterator& that) const {
			return !(*this == that);
		}
		const_iterator& operator --(void) {//prefix --
			judge();
			index--;
			return *this;
		}
		const_iterator operator--(int) {//postfix --
			const_iterator t{ *this }; // copy myself
			//this->operator--(); //
			judge();
			index--;
			return t; // return the copy of my old value
		}
		bool operator< (const const_iterator& that) const{
			judge();
			that.judge();
			if ((this->back == that.back) && (this->end == that.end) && (this->front == that.front) && (this->top == that.top)) {

				return (index < that.index);
			}
			else return false;
		}
		bool operator>(const const_iterator& that) {
			judge();
			if ((this->back == that.back) && (this->end == that.end) && (this->front == that.front) && (this->top == that.top)) {
				/*if ((that.position>this->top) && (this->position < this->end))return true;
				else if ((this->position>that.position) && (that.position>this->top)) return true;
				else if ((this->position>that.position) && (this->position<this->end)) return true;
				else return false;*/
				return (index>that.index);
			}
			else return false;
		}
		bool operator<=(const const_iterator& that) {
			//if ((this->back == that.back) && (this->end == that.end) && (this->front == that.front) && (this->top == that.top) && (this->position == that.position)) return true;
			if (*this == that) return true;
			else return (*this < that);

		}
		bool operator>=(const const_iterator& that) {
			//if ((this->back == that.back) && (this->end == that.end) && (this->front == that.front) && (this->top == that.top) && (this->position == that.position)) return true;
			if (*this == that) return true;
			else return (*this > that);

		}
		const_iterator operator+=(int n) {
			judge();
			index += n;
			return *this;
		}
		const_iterator operator-=(int n) {
			judge();
			index -= n;
			return *this;
		}
		const_iterator operator+(int n) {
			judge();

			return const_iterator{ buffer,index+n };
		}
		const_iterator operator-(int n) {
			judge();
			return const_iterator{ buffer,index - n };
			//return (*this + (-n));
		}
		int operator-(const_iterator that) {
			judge();
			that.judge();
			if ((this->back == that.back) && (this->end == that.end) && (this->front == that.front) && (this->top == that.top)) {
				return index - that.index;
			}
			else return 0;
		}
		T& operator[](int n) {
			/*iterator t;
			t = *this + n;
			return *t;
		*/judge();
			return buffer->operator[](index+n);
		};
		/*T& operator *(void) {
			printf("vector version is %d\n", buffer->version);
			judge();
			return buffer->operator[](index);
		};*/

		void judge() {//how to receive the changed version of vector
			if (this->iterator_version != buffer->version) {
				if ((index < 0) || (index >= buffer->length)) {
					if(this->valid==1)throw invalid_iterator{ invalid_iterator::SEVERE };//out of range
					else throw invalid_iterator{ invalid_iterator::MILD };
				}
				//if (buffer->version == 1) throw(invalid_iterator::SeverityLevel::SEVERE);
				else if (buffer->version == 2) throw invalid_iterator{ invalid_iterator::MODERATE };
				else if (buffer->version == 3) throw invalid_iterator{ invalid_iterator::MILD };

			}
		} //throw exception
		void judge() const{//how to receive the changed version of vector
			if (this->iterator_version != buffer->version) {
				if ((index < 0) || (index >= buffer->length)) {
						if (this->valid == 1)throw invalid_iterator{ invalid_iterator::SEVERE };//out of range
						else throw invalid_iterator{ invalid_iterator::MILD };
				}
																											 //if (buffer->version == 1) throw(invalid_iterator::SeverityLevel::SEVERE);
				else if (buffer->version == 2) throw invalid_iterator{ invalid_iterator::MODERATE };
				else if (buffer->version == 3) throw invalid_iterator{ invalid_iterator::MILD };

			}
			//throw invalid_iterator:
		} //throw exception
	};

	template<typename T>
	class vector<T>::iterator :public vector<T>::const_iterator {
	public:
		iterator(void) :const_iterator() {};
		iterator(vector<T>* p, int addr) {
			//position =addr;
            const_iterator::buffer = p;
			if ((addr >= 0) && (addr < const_iterator::buffer->length)) const_iterator::valid = 1;
			else const_iterator::valid = 0;
            const_iterator::index = addr;
            const_iterator::front = p->elem;
            const_iterator::back = p->elem + p->capacity;
            const_iterator::end = const_iterator::front + p->tail;
            const_iterator::top = const_iterator::back - p->head;
            const_iterator::iterator_version = p->version;

		}
		iterator(const iterator& that):const_iterator(that) {
			/*
			this->iterator_version = that.iterator_version;
			//this->position = that.position;
			this->index = that.index;
			this->front = that.front;
			this->back = that.back;
			this->top = that.top;
			this->end = that.end;
			buffer = that.buffer;
			*/
			//return (*this);
		}
		iterator& operator ++(void) {
            const_iterator::judge();
            const_iterator::index++;
			return *this;
		}
		iterator operator++(int) {//postfix ++
            const_iterator::judge();
			iterator t{ *this }; // copy myself
            const_iterator::index++; // increment myself (pre-increment)
			return t; // return the copy of my old value
		}
		iterator operator =(const iterator& that) {
			if (*this != that) {
				this->const_iterator::judge();
				that.const_iterator::judge();
				this->const_iterator::iterator_version = that.const_iterator::iterator_version;
				//this->position = that.position;
                const_iterator::buffer = that.const_iterator::buffer;
                const_iterator::index = that.const_iterator::index;
				this->const_iterator::valid = that.const_iterator::valid;
				this->const_iterator::front = that.const_iterator::front;
				this->const_iterator::back = that.const_iterator::back;
				this->const_iterator::top = that.const_iterator::top;
				this->const_iterator::end = that.const_iterator::end;
			}
			return (*this);
		}
		iterator& operator --(void) {
            const_iterator::judge();
            const_iterator::index--;
			return *this;
		}
		iterator operator--(int) {//postfix --
            const_iterator::judge();
			const_iterator t{ *this };
            const_iterator::index--;
			return t; // return the copy of my old value
		}
		iterator operator+(int n) {
            const_iterator::judge();
			return iterator{ const_iterator::buffer,const_iterator::index + n };
		}
		iterator operator-(int n) {
            const_iterator::judge();
			return iterator{ const_iterator::buffer,const_iterator::index - n };
		}
		iterator operator+=(int n) {
            const_iterator::judge();
            const_iterator::index += n;
			return *this;
		}
		iterator operator-=(int n) {
            const_iterator::judge();
            const_iterator::index -= n;
			return *this;
		}
	};

	}// namespace epl

#endif /* _Vector_h */
