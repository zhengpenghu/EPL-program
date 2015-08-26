// Vector.h -- header file for Vector data structure project
/*Feb 12th, 2015*/
/*EID:zh3499*/

#pragma once
#ifndef _Vector_h
#define _Vector_h

namespace epl{

template <typename T>
class vector {
private:
	int capacity;
		int length;
		T* elem;
		int head;
		int tail;
		
		void destroy(void) {
			for (int i=0;i<tail;i++){
				elem[i].~T();
			}
			for (int i=0;i<head;i++){
						elem[capacity-1-i].~T();
					}
			operator delete(elem);}
		
		void copy(const vector<T>& that){
			length=that.length;tail=that.tail;head=that.head;capacity=that.capacity;
			if(that.elem){
				elem = reinterpret_cast<T*>(::operator new(sizeof(T)*capacity));
							for(int i=0;i<tail;i++) new (&elem[i]) T{that.elem[i]};							
							for(int i=0;i<head;i++) new (&elem[capacity-1-i]) T{that.elem[capacity-1-i]};								
			}
			else{
				elem=nullptr;
			}			
		}

	public:
		/*constructor*/
		vector(void)
			:capacity(8),length(0){
			//elem = (T*)::operator new(sizeof(T)*8);
			elem = reinterpret_cast<T*>(::operator new(sizeof(T)*8));
			head = 0;
			tail = 0;
		};
		/*copy constructor*/
		vector(const vector<T>& other){			
				copy(other);
			};
		/*copy assignment*/
		/* error here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
		vector<T>& operator=(const vector<T>& that){
				if(this!=&that){
					destroy();
					copy(that);
				}
				return *this;
			
		}
		
		/* explict constructor*/
		explicit vector(uint64_t n){
			if(n>0){
			capacity=n;
			tail=n;
			head=0;
			elem = reinterpret_cast<T*>(::operator new(sizeof(T)*capacity));
			for(int i=0;i<capacity;i++){
						new (&elem[i]) T{};						
					}
			length=n;
			}
			else {
			capacity=8;elem=(T*)::operator new(8*sizeof(T));length=0;
			}
		};
		
		~vector(void){/*destructor function*/
			
			destroy();
		};
		
		 uint64_t size(void)const{
	/* const here means that this function won't change any member variable*/
			return length;
		};
		 
		 
		 T& operator[](uint64_t k){
			 if(k>=length){throw std::out_of_range{"index out of range"};}
			 else{
				 if(k>=head)
				 return *(elem+k-head);
				 else return *(elem+capacity-head+k);
			 }
		 };
		 const T& operator[](uint64_t k) const{/*what's the differenece between this and above function*/
			 const T* tmp1 = elem+k-head;
			 const T* tmp2 = elem+capacity-head+k;
			 if(k>=length){throw std::out_of_range{"index out of range"};}
			 else if(k>=head) return *tmp1;
			 if(k<head)
			 {return *tmp2;}
		 }
		 void push_back(const T& co){
			 if(length==capacity){
				 T *p = (T*)::operator new(2*capacity*sizeof(T));
				 new (&p[capacity]) T(co);
				 capacity=2*capacity;
				 for(int i=0;i<tail;i++){
					 new (&p[i]) T((T&&) elem[i]);
					 
					 //elem[i].~T();
				 }
				 for(int i=0;i<head;i++){
				 	new (&p[2*capacity-head+i]) T((T&&) elem[capacity-head+i]);
				 	//elem[capacity-head+i].~T();
				 			 }
				 capacity=2*capacity;
				 operator delete(elem);
				 elem=p;
				 length+=1;
				 tail+=1;
			 }			 
			 else{
				 length+=1;
				 			 tail+=1;
				 			 //std::cout<<"still pushback by copy"<<std::endl;
				 			 
				 			 new (&elem[tail-1]) T(co);
				 			 //std::cout<<"problem is later"<<std::endl;
				 
			 }
			 //std::cout<<elem[tail-1];
			 return;
		 }
		
		
		 void push_front(const T& co){
		 		 if(length==capacity){		 			
		 			 	 	 T *p = (T*)::operator new(2*capacity*sizeof(T));
								new (&p[2*capacity-head-1]) T(co);							 
		 			 	 	 for(int i=0;i<tail;i++){
		 			 	 			new (&p[i]) T((T&&) elem[i]);
		 			 	 				 }
		 			 	 	for(int i=0;i<head;i++){		 			 	 				 				
		 			 	 		new (&p[2*capacity-head+i]) T((T&&) elem[capacity-head+i]);
		 			 	 				 			 }
		 		 			 //destroy();
		 			 	 capacity=2*capacity;
		 		 			 elem=p;	
		 		 			length+=1;
		 		 			head+=1;
		 		 			//new (&elem[capacity-head]) T(co);		 		 			
		 		 		 }
		 		 else {
		 			 	 	 length+=1;
		 			 		 head+=1;
		 			 		 new (&elem[capacity-head]) T(co);
		 		 }		 	
		 		 return;
		 	 }
		 
		 void pop_back(void){
			 if(length==0) {throw std::out_of_range{"no element in the vector!"};}
			 
			 if(tail>0){
				 elem[tail-1].~T();
			 tail-=1;
			 length-=1;}
			 else if(length==1){
				 elem[capacity-1].~T();
				 length-=1;
				 head-=1;
			 }
			 else{	 
				 for(int i=0;i<length/2;i++){
					elem[tail]=T((T&&) elem[capacity-head/2]); 
					elem[capacity-head/2]=T((T&&) elem[capacity-head]);
					elem[capacity-head].~T();
					head-=1;
					tail+=1;
				 }
				 elem[tail-1].~T();
				 tail-=1;
				 length-=1;
			// elem[length-1].~T();
			 }
		 }
		 
		 void pop_front(void){
			 if(length==0) {throw std::out_of_range{"no element in the vector!"};}
			 if(tail==capacity){
				 
				 elem[0].~T();
				 tail=0;
				 head=capacity-1;
				 length-=1;
				 return;
			 }
			 if(head==0){
				 for(int i=0;i<length/2;i++){
					 elem[capacity-length/2+i]=T((T&&) elem[i]);
					 elem[i]=T((T&&) elem[i+length/2]);
					 elem[i+length/2].~T();
				 }				 
			 }
			 elem[capacity-head].~T();
			 head-=1;
			 length-=1;			 
		 }
		 //move constructor
		vector(vector<T>&& that):capacity(0),length(0),elem(nullptr),head(0),tail(0)
		{
			
			length=that.length;capacity=that.capacity;head=that.head;tail=that.tail;
			
			elem=that.elem;
			
			that.length=0;that.capacity=0;that.head=0;that.tail=0;
			that.elem=nullptr;
		}
		
		
		// move assignment
		/* this need to be changed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
		vector<T>& operator=(vector<T>&& that){
			if(this!=&that){
			length=that.length;capacity=that.capacity;head=that.head;tail=that.tail;
			elem=that.elem;
			//std::cout<<"move assignment"<std::endl;
			//std::cout<<"move assignment"<<std::endl;
			that.length=0;that.capacity=0;that.head=0;that.tail=0;
			that.elem=nullptr;
			}
			return *this;//is that right??
		}
		
		void push_back(T&& that){/* there is error here*/
			//std::cout<<"pushback_move"<<std::endl;
			if(length==capacity){
							 T *p = (T*)::operator new(2*capacity*sizeof(T));
							 new (&p[capacity]) T((T&&) that);
							 capacity=2*capacity;
							 for(int i=0;i<tail;i++){
								 new (&p[i]) T((T&&) elem[i]);
								
								 //elem[i].~T();
							 }
							 for(int i=0;i<head;i++){
								 new (&p[2*capacity-head+i]) T((T&&) elem[capacity-head+i]);
								
							 	//elem[capacity-head+i].~T();
							 			 }
							 capacity=2*capacity;
							 operator delete(elem);
							 elem=p;length+=1;
						 tail+=1;	 
						 }
						 else{
						 length+=1;
						 tail+=1;
						 new (&elem[tail-1]) T((T&&) that);
						 
						 }
						 
						 
						 //std::cout<<elem[tail-1];
						 return;
		}
		void push_front(T&& that){
			if(length==capacity){		 			
					 			 	 	 T *p = (T*)::operator new(2*capacity*sizeof(T));
											new (&p[2*capacity-head-1]) T((T&&) that);	//edit here									 
					 			 for(int i=0;i<tail;i++){new (&p[i]) T((T&&) elem[i]);}
					 			 	 	for(int i=0;i<head;i++){		 			 	 				 				
					 			 	 		new (&p[2*capacity-head+i]) T((T&&) elem[capacity-head+i]);
					 			 	 				 			 }
					 		 			 //destroy();
					 			 	 capacity=2*capacity;
					 		 			 elem=p;length+=1;
					 		 			head+=1;
					 		 			//new (&elem[capacity-head]) T((T&&) that);		 		 			
					 		 		 }
					 		 else {
					 			 	 	 length+=1;
					 			 		 head+=1;
					 			 		 new (&elem[capacity-head]) T((T&&) that);
					 		 }		 	
					 		 return;
		}
	};

	}// namespace epl

#endif /* _Vector_h */
