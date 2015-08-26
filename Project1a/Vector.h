// Vector.h -- header file for Vector data structure project
/*zhengpeng hu*/
/*zh3499*/
/*23:20pm Feb 5th*/
#pragma once
#ifndef _Vector_h
#define _Vector_h

namespace epl{

template <typename T>/*T is the element type of vector*/
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

public:
	/*constructor*/
	vector(void)
		//:capacity(8),elem(new T[8]),length(0){
		:capacity(8),length(0){
		//elem = (T*)::operator new(sizeof(T)*8);
		elem = reinterpret_cast<T*>(::operator new(sizeof(T)*8));
		head = 0;
		tail = 0;
	};
	/*copy constructor*/
	vector(const vector<T>& other)
		
		:capacity(other.capacity),length(other.length),head(other.head),tail(other.tail){
		
		elem = reinterpret_cast<T*>(::operator new(sizeof(T)*capacity));
		for(int i=0;i<tail;i++) new (&elem[i]) T{other.elem[i]};
				for(int i=0;i<head;i++) new (&elem[capacity-1-i]) T{other.elem[capacity-1-i]};
				//};
		};
	/*copy assignment*/
	vector<T>& operator=(const vector<T>& a){
		
		//T *p=new T[a.capacity];
		T *p = reinterpret_cast<T*>(::operator new(sizeof(T)*a.capacity));
		for(int i=0;i<a.tail;i++){
					new (&p[i]) T{a.elem[i]};
					//p[i]=T(a.elem[i]);
				}
				for(int i=0;i<a.head;i++){
					new (&p[capacity-head+i]) T{a.elem[capacity-head+i]};
					//p[capacity-head+i]=T(a.elem[capacity-head+i]);
				}
		
		
		//for(int i=0;i<a.length;i++)p[i]=T(a.elem[i]);		//don't use = here
		destroy();//delete[] elem;
		
		elem=p;
		capacity=a.capacity;
		length=a.capacity;
		head=a.head;
		tail=a.tail;
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
				
		/*for(int i=0;i<capacity;i++){
					new (&elem[i]) T{};
				}*/
		
		//elem = new T[n];
		//elem=(T*)::operator new(n*sizeof(T));
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
			 capacity=2*capacity;
			 for(int i=0;i<tail;i++){
				 new (&p[i]) T(elem[i]);
				 elem[i].~T();
			 }
			 for(int i=0;i<head;i++){
			 	new (&p[2*capacity-head+i]) T(elem[capacity-head+i]);
			 	elem[capacity-head+i].~T();
			 			 }
			 capacity=2*capacity;
			 operator delete(elem);
			 elem=p;	 
		 }
		 length+=1;
		 tail+=1;
		 //elem[length-1]=co;/*can I directly assign co to elem[length-1],or use a copy assigment?*/
		 //or write in this way?  elem[length-1]=T(co);
		 new (&elem[tail-1]) T(co);
		 //std::cout<<elem[tail-1];
		 return;
	 }
	
	
	 void push_front(const T& co){
	 		 if(length==capacity){		 			
	 			 	 	 T *p = (T*)::operator new(2*capacity*sizeof(T));	 	
	 			 	 	 for(int i=0;i<tail;i++){
	 			 	 					 new (&p[i]) T(elem[i]);
	 			 	 					// elem[i].~T();
	 			 	 				 }
	 			 	 				 for(int i=0;i<head;i++){
	 			 	 				 				 //p[i]=elem[i];
	 			 	 				 	new (&p[2*capacity-head+i]) T(elem[capacity-head+i]);
	 			 	 				 	//elem[capacity-head+i].~T();
	 			 	 				 			 }
	 		 				 //p[i+1]=elem[i];
	 		 			 
	 		 			
	 		 			 destroy();
	 			 	 capacity=2*capacity;
	 		 			 elem=p;	
	 		 			 
	 		 			length+=1;
	 		 			head+=1;
	 		 			new (&elem[capacity-head]) T(co);
	 		 			//std::cout<<elem[capacity-head];
	 		 			
	 		 		 }
	 		 else {
	 			 	 	 length+=1;
	 			 		 head+=1;
	 			 		 new (&elem[capacity-head]) T(co);
	 		 }
	 		 		 
	 		 //elem[0]=T(co);
	 		 return;
	 	 }
	 
	 void pop_back(void){
		 if(length==0) {throw std::out_of_range{"no element in the vector!"};}
		 
		 //delete &elem[length-1];
		 
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
				elem[tail]=T(elem[capacity-head/2]); 
				elem[capacity-head/2]=elem[capacity-head];
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
				 elem[capacity-length/2+i]=T(elem[i]);
				 elem[i]=T(elem[i+length/2]);
				 elem[i+length/2].~T();
			 }
			 
		 }
		 elem[capacity-head].~T();
		 head-=1;
		 length-=1;
		 
	 }
	 
};



} //namespace epl

#endif /* _Vector_h */
