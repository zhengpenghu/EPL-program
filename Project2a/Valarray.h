// Valarray.h

/* Put your solution in this file, we expect to be able to use
 * your epl::valarray class by simply saying #include "Valarray.h"
 *
 * We will #include "Vector.h" to get the epl::vector<T> class 
 * before we #include "Valarray.h". You are encouraged to test
 * and develop your class using std::vector<T> as the base class
 * for your epl::valarray<T>
 * you are required to submit your project with epl::vector<T>
 * as the base class for your epl::valarray<T>
 */
//zh3499
#ifndef _Valarray_h
#define _Valarray_h
//#include "Vector.h"
//using std::vector; // during development and testing
// using epl::vector; // after submission
#include "Vector.h"
#include "utility"
#include "complex.h"
using epl::vector;

//template <typename T,typename R>
//class valarray;


template<typename T>
struct isScalar;

template<>struct isScalar<int> { using type = int;};
template<>struct isScalar<float>{using type = float;};
template<>struct isScalar<double>{using type = double;};

template <typename T>
struct rank;

template <> struct rank<int> { static constexpr int value = 1; };
template <> struct rank<float> { static constexpr int value = 2; };
template <> struct rank<double> { static constexpr int value = 3; };
template <typename T> struct rank<std::complex<T>> { static constexpr int value = rank<T>::value; };

template <int R>
struct stype;

template <> struct stype<1> { using type = int; };
template <> struct stype<2> { using type = float; };
template <> struct stype<3> { using type = double; };

template <typename T> struct is_complex : public std::false_type {};
template <typename T> struct is_complex<std::complex<T>> : public std::true_type {};

template <bool p, typename T> struct ctype;
template <typename T> struct ctype<true, T> { using type = std::complex<T>; };
template <typename T> struct ctype<false, T> { using type = T; };

template <typename T1, typename T2>
struct choose_type {
    static constexpr int t1_rank = rank<T1>::value;
    static constexpr int t2_rank = rank<T2>::value;
    static constexpr int max_rank = (t1_rank < t2_rank) ? t2_rank : t1_rank;
    
    using my_stype = typename stype<max_rank>::type;//decide the base type, but we still don't know if it's a complex or not
    
    static constexpr bool t1_comp = is_complex<T1>::value;
    static constexpr bool t2_comp = is_complex<T2>::value;
    static constexpr  bool my_comp = t1_comp || t2_comp;
    
    using type = typename ctype<my_comp, my_stype>::type;
};


template <typename T>//, typename R=vector<T> >
class valarray: public vector<T>{
//data member
    using value_type = T;
    
public:
    using vector<T>::vector;
    //constructor
    /*valarray(void):vector<T>(){
    
    }
    
    explicit valarray(int a):vector<T>(a){
    
    }*/
    //copy constructor
   // valarray(valarray<T>& a){
    
    //}
    
    template<typename R>
    valarray& operator=(const valarray<R>& other){
        int size=this->size();
        for (int i=0; i<size; i++) {
            (*this)[i]=(T)(other[i]);
        }
        return *this;
    }

    template<typename R,typename unused =typename isScalar<R>::type>
    valarray& operator=(const R& other){
        int size=this->size();
        for (int i=0; i<size; i++) {
            (*this)[i]=(T)other;
        }
        return *this;
    }
    
    /*valarray()
    ~valarray(void){
    
    }*/
    
    valarray<T> operator-(void){
        valarray<T> a(this->size());
        
        typename valarray<T>::iterator ai=a.begin();
        typename valarray<T>::iterator bi=this->begin();
        for(int i=0;i<this->size();i++){
            *ai=-(*bi);
            ai++;
        }
        return a;
    }
    
   
    
    };

//operator function for valarray

template<typename T,typename R>
valarray<typename choose_type<T,R>::type> operator+(const valarray<T>& first,const valarray<R>& other){
    
    int size=first.size()>other.size()?other.size():first.size();
    valarray<typename choose_type<T,R>::type> a(size);
    typename valarray<typename choose_type<T,R>::type>::iterator ai=a.begin();
    typename valarray<R>::const_iterator bi=other.begin();
    typename valarray<T>::const_iterator ci=first.begin();
    using same =typename choose_type<T,R>::type;
    for (int i=0; i<size; i++) {
        *ai=(same)*ci+(same)*bi;
        ai++;
        bi++;
        ci++;
        
    }
 
    return a;
};


//


template<typename T,typename R,typename unused =typename isScalar<R>::type>
valarray<typename choose_type<T,R>::type> operator+(const R& other, const valarray<T>& val){

    
    valarray<typename choose_type<T ,R>::type> a(val.size());
    using same =typename choose_type<T,R>::type;
    typename valarray<typename choose_type<T,R>::type>::iterator ai=a.begin();
    typename valarray<T>::const_iterator bi=val.begin();
    for(int i=0;i<val.size();i++){
        *ai=(same)other+(same)*bi;
        ai++;
    }
    return a;

};

template<typename T,typename R,typename unused =typename isScalar<R>::type>
valarray<typename choose_type<T,R>::type> operator+(const valarray<T>& val,const R& other){
   valarray<typename choose_type<T ,R>::type> a(val.size());
    using same =typename choose_type<T,R>::type;
    typename valarray<typename choose_type<T,R>::type>::iterator ai=a.begin();
    typename valarray<T>::const_iterator bi=val.begin();
    for(int i=0;i<val.size();i++){
        *ai=(same)*bi+(same)other;
        ai++;
    }
    return a;
    
};

//jian
template<typename T,typename R>
valarray<typename choose_type<T,R>::type> operator-(const valarray<T>& first,const valarray<R>& other){
    using same =typename choose_type<T,R>::type;
    int size=first.size()>other.size()?other.size():first.size();
    valarray<typename choose_type<T,R>::type> a(size);
    typename valarray<typename choose_type<T,R>::type>::iterator ai=a.begin();
    typename valarray<R>::const_iterator bi=other.begin();
    typename valarray<T>::const_iterator ci=first.begin();
    
    for (int i=0; i<size; i++) {
        *ai=(same)*ci-(same)*bi;
        ai++;
        bi++;
        ci++;
        
    }
    
    return a;
};


//


template<typename T,typename R,typename unused =typename isScalar<R>::type>
valarray<typename choose_type<T,R>::type> operator-(const R& other, const valarray<T>& val){
    
    
    valarray<typename choose_type<T ,R>::type> a(val.size());
    using same =typename choose_type<T,R>::type;
    typename valarray<typename choose_type<T,R>::type>::iterator ai=a.begin();
    typename valarray<T>::const_iterator bi=val.begin();
    for(int i=0;i<val.size();i++){
        *ai=(same)other-(same)*bi;
        ai++;
    }
    return a;
    
};

template<typename T,typename R,typename unused =typename isScalar<R>::type>
valarray<typename choose_type<T,R>::type> operator-(const valarray<T>& val,const R& other){
    valarray<typename choose_type<T ,R>::type> a(val.size());
    using same =typename choose_type<T,R>::type;
    typename valarray<typename choose_type<T,R>::type>::iterator ai=a.begin();
    typename valarray<T>::const_iterator bi=val.begin();
    for(int i=0;i<val.size();i++){
        *ai=(same)*bi-(same)other;
        ai++;
    }
    return a;
    
};



/*******************/
//cheng
template<typename T,typename R>
valarray<typename choose_type<T,R>::type> operator*(const valarray<T>& first,const valarray<R>& other){
    using same =typename choose_type<T,R>::type;
    int size=first.size()>other.size()?other.size():first.size();
    valarray<typename choose_type<T,R>::type> a(size);
    typename valarray<typename choose_type<T,R>::type>::iterator ai=a.begin();
    typename valarray<R>::const_iterator bi=other.begin();
    typename valarray<T>::const_iterator ci=first.begin();
    
    for (int i=0; i<size; i++) {
        *ai=(same)(*ci)*((same)(*bi));
        ai++;
        bi++;
        ci++;
        
    }
    
    return a;
};


//


template<typename T,typename R,typename unused =typename isScalar<R>::type>
valarray<typename choose_type<T,R>::type> operator*(const R& other, const valarray<T>& val){
    
    using same =typename choose_type<T,R>::type;
    valarray<typename choose_type<T ,R>::type> a(val.size());
    
    typename valarray<typename choose_type<T,R>::type>::iterator ai=a.begin();
    typename valarray<T>::const_iterator bi=val.begin();
    for(int i=0;i<val.size();i++){
        *ai=(same)other*((same)(*bi));
        ai++;
    }
    return a;
    
};

template<typename T,typename R,typename unused =typename isScalar<R>::type>
valarray<typename choose_type<T,R>::type> operator*(const valarray<T>& val,const R& other){
    valarray<typename choose_type<T ,R>::type> a(val.size());
    using same =typename choose_type<T,R>::type;
    typename valarray<typename choose_type<T,R>::type>::iterator ai=a.begin();
    typename valarray<T>::const_iterator bi=val.begin();
    for(int i=0;i<val.size();i++){
        *ai=(same)(*bi)*((same)other);
        ai++;
    }
    return a;
    
};
//chu
template<typename T,typename R>
valarray<typename choose_type<T,R>::type> operator/(const valarray<T>& first,const valarray<R>& other){
    using same =typename choose_type<T,R>::type;
    int size=first.size()>other.size()?other.size():first.size();
    valarray<typename choose_type<T,R>::type> a(size);
    typename valarray<typename choose_type<T,R>::type>::iterator ai=a.begin();
    typename valarray<R>::const_iterator bi=other.begin();
    typename valarray<T>::const_iterator ci=first.begin();
    
    for (int i=0; i<size; i++) {
        *ai=((same)*ci)/((same)(*bi));
        ai++;
        bi++;
        ci++;
        
    }
    
    return a;
};


//


template<typename T,typename R,typename unused =typename isScalar<R>::type>
valarray<typename choose_type<T,R>::type> operator/(const R& other, const valarray<T>& val){
    
    
    valarray<typename choose_type<T ,R>::type> a(val.size());
    using same =typename choose_type<T,R>::type;
    typename valarray<typename choose_type<T,R>::type>::iterator ai=a.begin();
    typename valarray<T>::const_iterator bi=val.begin();
    for(int i=0;i<val.size();i++){
        *ai=((same)other)/((same)(*bi));
        ai++;
    }
    return a;
    
};

template<typename T,typename R,typename unused =typename isScalar<R>::type>
valarray<typename choose_type<T,R>::type> operator/(const valarray<T>& val,const R& other){
    valarray<typename choose_type<T ,R>::type> a(val.size());
    using same =typename choose_type<T,R>::type;
    typename valarray<typename choose_type<T,R>::type>::iterator ai=a.begin();
    typename valarray<T>::const_iterator bi=val.begin();
    for(int i=0;i<val.size();i++){
        *ai=((same)(*bi))/((same)other);
        ai++;
    }
    return a;
    
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const valarray<T>& valarr){
    int size=valarr.size();
    for (int i=0; i<size; i++) {
        out<<valarr[i]<<std::endl;
    }
    return out;
    }



/***************************/


//template<typename T,typename R>

#endif /* _Valarray_h */

