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
//TUE 23：30

#ifndef _Valarray_h
#define _Valarray_h
//#include "Vector.h"
//using std::vector; // during development and testing
// using epl::vector; // after submission
#include "Vector.h"
#include "utility"
#include "complex.h"
#include <climits>
#include<cmath>
using epl::vector;

//template <typename T,typename R>
//class valarray;
template<typename T>
struct scalar{
    using value_type =T;
    int size() const{return INT_MAX;};
    T elem;
    
public:
    scalar(const T& input):elem(input){};
    
    T operator[](const int k) const{
        return elem;
    }
};

template<typename T>
struct isScalar;

template<>struct isScalar<int> { using value_type = int;};
template<>struct isScalar<float>{using value_type = float;};
template<>struct isScalar<double>{using value_type = double;};
template<>struct isScalar<std::complex<float>>{using value_type = std::complex<float>;};
template<>struct isScalar<std::complex<double>>{using value_type = std::complex<double>;};
    

template <typename T>
struct rank;

template <> struct rank<int> { static constexpr int value = 1; };
template <> struct rank<float> { static constexpr int value = 2; };
template <> struct rank<double> { static constexpr int value = 3; };
template <typename T> struct rank<std::complex<T>> { static constexpr int value = rank<T>::value; };

template <int R>
struct stype;

template <bool P, typename T, typename F>
struct ifthen {
    using type = T;
};

template <typename T, typename F>
struct ifthen<false, T, F> {
    using type = F;
};

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

template<typename T>
struct squareroot{
    using return_type =typename ifthen<is_complex<T>::value,std::complex<double>,double>::type;
    return_type operator()(T x) const{
         return std::sqrt(x);
    }
};

// Ref is used for that if the object is vector, we store the reference, otherwise we copy the object(scalar,proxy)
template<typename T> struct ref {using type = T;};
template<typename T> struct ref<epl::vector<T>> {using type =const epl::vector<T>&;};
template<typename T>
using Ref = typename ref<T>::type;

template <typename L,typename R, typename OP>
class proxy{
public:
    Ref<L> left;
    Ref<R> right;
    OP opetor;
    using value_type = typename choose_type<typename L::value_type,typename R::value_type>::type;
    
    class const_iterator{
        proxy<L,R,OP> host;
        int index;
        using Same = const_iterator;
        
    public:
        const_iterator(const int size,const proxy<L,R,OP>& that):host(that),index(size){        }
        //copy assignment
        Same& operator=(const Same& that){
            index=that.index;
            host=that.host;
            return (*this);
        };
        
        Same& operator++(void){
            index+=1;
            return (*this);
        }
        Same operator++(int){
            Same t(*this);
            index++;
            //this->operator++();
            return t;
        }
        
        bool operator==(const Same& that){
            return index==that.index;
        }
        
        bool operator!=(const Same& that){
            return index!=that.index;
        }
        
        const value_type operator*(void) const{
            return host[index];
        }
    };
    // if L,R is a valarray or scalar, I should use a reference, if that's a proxy I should use a copy
    proxy( const L& l,const R& r,OP ops):left(l),right(r),opetor(ops){};
    int size() const{
        return left.size()<right.size()?left.size():right.size();
    }
    
     value_type operator[](int k) const {//choose type
        //proxy and proxy
         return opetor((value_type)left[k],(value_type)right[k]);
       };
    
    const_iterator begin(void) const{
        return const_iterator(0,*this);
    }
    
    const_iterator end(void) const{
        return const_iterator(size(),*this);
    }
};

template<typename T, typename OP>
class unaryproxy{
public:
    Ref<T> middle;
    OP opetor;
    
    using value_type = typename T::value_type;
    class const_iterator{
        unaryproxy<T,OP> host;
        int index;
        using Same = const_iterator;
        
    public:
        const_iterator(const int size,const unaryproxy<T,OP>& that):host(that),index(size){
        }
        //copy assignment
        Same& operator=(const Same& that){
            index=that.index;
            host=that.host;
            return (*this);
        };
        
        Same& operator++(void){
            index+=1;
            return (*this);
        }
        Same operator++(int){
            Same t(*this);
            this->operator++();
            return t;
        }
        
        bool operator==(const Same& that){
            return index==that.index;
        }
        
        bool operator!=(const Same& that){
            return index!=that.index;
        }
        
        const value_type operator*(void) const{
            return host[index];
        }
    };

    unaryproxy(Ref<T> m,int a,OP OPS):middle(m),opetor(OPS){};//& or not & is a question
    
    int size() const{   return middle.size();};
    value_type operator[](int k) const{ return opetor((value_type)middle[k]);}
    
    const_iterator begin(void) const{       return const_iterator(0,*this);}
    const_iterator end(void) const{       return const_iterator(size(),*this);}
};

template <class T>//, typename R=vector<T> >
class vec_wrap: public T{// T will be vector<int> or proxy<L,R>
    public:
    using value_type = typename T::value_type;
    using T::T;
    
    // vec_wrap(void):T(){
    vec_wrap(void):T(){
    };
    
//    vec_wrap(const T& x){
//    };
    
    template<typename R>
     vec_wrap(vec_wrap<R> const& that):T(that.size()){
        for (int k=0; k<that.size(); k++) {
            (*this)[k]=(value_type)that[k];
        }
    };
    template<typename R>
    vec_wrap<T>& operator=(const vec_wrap<R>& other){
        int size=(this->size())<other.size()?this->size():other.size();
        for (int i=0; i<size; i++) {
            (*this)[i]=(value_type)(other[i]);//here value_type is int, double float
        }
        return *this;
    }
    
    vec_wrap<unaryproxy<T,std::negate<typename T::value_type>>> operator-(void) const {
        std::negate<typename T::value_type> f;
        return vec_wrap<unaryproxy<T,std::negate<typename T::value_type>>>(*this,1,f);
    }

    template<typename R,typename unused =typename isScalar<R>::value_type>
    vec_wrap<T>& operator=(const R& other){
        int size=this->size();
        for (int i=0; i<size; i++) {
            (*this)[i]=(value_type)other;
        }
        return *this;
    }
    
    value_type sum(void){
        return accumulate(std::plus<value_type>{});
    }
    
    template<typename F>
    value_type accumulate(F fun){
        typename T::value_type t=(*this)[0];
        for (int i=1; i<this->size(); i++) {
            t=fun(t,(*this)[i]);
        }
        return t;
    }
    
    template<typename FUN>
    vec_wrap<unaryproxy<T,FUN>> apply(FUN f){
        return vec_wrap<unaryproxy<T,FUN>>(*this,1,f);//pass the function object
    };
    
    vec_wrap<unaryproxy<T,squareroot<typename T::value_type>>> sqrt(void){
        return apply(squareroot<typename T::value_type>{});
    };
};

template<typename T,typename unused = typename isScalar<T>::value_type>
using valarray = vec_wrap<vector<T>>;

/*******ostream***********/
template<typename T>
std::ostream& operator<<(std::ostream& out, const vec_wrap<T>& valarr){
    int size=valarr.size();
    for (int i=0; i<size; i++) {
        out<<valarr[i]<<std::endl;
    }
    return out;
};
/***************************/
template <bool, typename T> struct enableif;

template <typename T>struct enableif<true,T>{
    using type = T;
};
template <typename T>struct enableif<false,T>{};

template <typename X>
struct isvalid :public std::false_type{};

template <typename X>
struct isvalid<vec_wrap<X>>: public std::true_type{
    using ty = X;
};
template <> struct isvalid<int>: public std::true_type{
    using ty =scalar<int>;
};
template <> struct isvalid<double>: public std::true_type{
    using ty =scalar< double>;
};
template <> struct isvalid<float>: public std::true_type{
    using ty = scalar<float>;
};
template <> struct isvalid<std::complex<float>>: public std::true_type{
    using ty = scalar<std::complex<float>>;
};
template <> struct isvalid<std::complex<double>>: public std::true_type{
    using ty = scalar<std::complex<double>>;
};

//using this to get rid of the decoration and get the proxy or vector which is the content
template<typename T>
struct on_off_decoration{
    using value_type = scalar<T>;
};

template<typename T>
struct on_off_decoration<vec_wrap<T>>{
    using value_type = T;
};

template<typename T,typename F,typename op>
struct Bresult{
    static constexpr bool isok1 = isvalid<T>::value;
    static constexpr bool isok2 = isvalid<F>::value;
    using vec_w = vec_wrap< proxy<typename on_off_decoration<T>::value_type,typename on_off_decoration<F>::value_type,op>>;
    static constexpr bool  isok= isok1&&isok2;
    using type =typename enableif<isok, vec_w>::type;
};
template<typename T,typename F>
using optype =typename choose_type< typename T::value_type,typename F::value_type>::type;

//These are my four operator I think they works but I'm not so sure about that.if not,I wonder could you try the commented 12 oeprators? Thanks.
    template<typename T,typename F,typename used1 = typename isvalid<T>::ty,typename used2 = typename isvalid<F>::ty>
typename Bresult<T,F,std::plus<optype<used1,used2>>>::type operator+(const T& x,const F& y){
    std::plus<optype<used1,used2>> f;
    return (typename Bresult<T,F,std::plus<optype<used1,used2>>>::type){x,y,f};
}
//interesting bugs happen here when I try to cast x to vector
//x is a warpper here
template<typename T,typename F,typename used1 = typename isvalid<T>::ty,typename used2 = typename isvalid<F>::ty>
typename Bresult<T,F,std::minus<optype<used1,used2>>>::type operator-(const T& x,const F& y){
    std::minus<optype<used1,used2>> f;
    return (typename Bresult<T,F,std::minus<optype<used1,used2>>>::type){x,y,f};
}

template<typename T,typename F,typename used1 = typename isvalid<T>::ty,typename used2 = typename isvalid<F>::ty>
typename Bresult<T,F,std::multiplies<optype<used1,used2>>>::type operator*(const T& x,const F& y){
    std::multiplies<optype<used1,used2>> f;
    return (typename Bresult<T,F,std::multiplies<optype<used1,used2>>>::type){x,y,f};
}

template<typename T,typename F,typename used1 = typename isvalid<T>::ty,typename used2 = typename isvalid<F>::ty>
typename Bresult<T,F,std::divides<optype<used1,used2>>>::type operator/(const T& x,const F& y){
    std::divides<optype<used1,used2>> f;
    return (typename Bresult<T,F,std::divides<optype<used1,used2>>>::type){x,y,f};
}

/////////////////////////

//That's the 12 operator version,which works for all testcase
////add
//template<typename T,typename F>
//vec_wrap<proxy<T,F, std::plus<optype<T,F>> >> operator+(const vec_wrap<T>& x,const vec_wrap<F>& y){
//    std::plus<optype<T,F>> f;
//    return vec_wrap<proxy<T,F, std::plus<optype<T,F>> >>{x,y,f};
//};
//
//template<typename T,typename F,typename unused = typename isScalar<T>::value_type>
//vec_wrap< proxy<scalar<T>,F, std::plus<optype<scalar<T>,F>> > > operator+(const T& x,const vec_wrap<F>& y){
//    std::plus<optype<scalar<T>,F>> f;
//    return vec_wrap<proxy<scalar<T>,F, std::plus<optype<scalar<T>,F>>>>{scalar<T>{x},y,f};
//};
//
//template<typename T,typename F,typename unused = typename isScalar<F>::value_type>
//vec_wrap< proxy<T,scalar<F>, std::plus<optype<T,scalar<F>>>>> operator+(const vec_wrap<T>& x,const F& y){
//    std::plus<optype<T,scalar<F>>> f;
//    return vec_wrap<proxy<T,scalar<F>, std::plus<optype<T,scalar<F>>>>>{x,scalar<F>{y},f};
//};
//
////subtract
//template<typename T,typename F>
//vec_wrap<proxy<T,F, std::minus<optype<T,F>> >> operator-(const vec_wrap<T>& x,const vec_wrap<F>& y){
//    std::minus<optype<T,F>> f;
//    return vec_wrap<proxy<T,F, std::minus<optype<T,F>> >>{x,y,f};
//};
//
//template<typename T,typename F,typename unused = typename isScalar<T>::value_type>
//vec_wrap< proxy<scalar<T>,F, std::minus<optype<scalar<T>,F>>>> operator-(const T& x,const vec_wrap<F>& y){
//    std::minus<optype<scalar<T>,F>> f;
//    return vec_wrap<proxy<scalar<T>,F, std::minus<optype<scalar<T>,F>>>>{scalar<T>{x},y,f};
//};
//
//template<typename T,typename F,typename unused = typename isScalar<F>::value_type>
//vec_wrap< proxy<T,scalar<F>, std::minus<optype<T,scalar<F>>>>> operator-(const vec_wrap<T>& x,const F& y){
//    std::minus<optype<T,scalar<F>>> f;
//    return vec_wrap<proxy<T,scalar<F>, std::minus<optype<T,scalar<F>>>>>{x,scalar<F>{y},f};
//};
//
////multiply
//template<typename T,typename F>
//vec_wrap<proxy<T,F, std::multiplies<optype<T,F>> >> operator*(const vec_wrap<T>& x,const vec_wrap<F>& y){
//    std::multiplies<optype<T,F>> f;
//    return vec_wrap<proxy<T,F, std::multiplies<optype<T,F>> >>{x,y,f};
//};
//
//template<typename T,typename F,typename unused = typename isScalar<T>::value_type>
//vec_wrap< proxy<scalar<T>,F, std::multiplies<optype<scalar<T>,F>>>> operator*(const T& x,const vec_wrap<F>& y){
//    std::multiplies<optype< scalar<T>,F>> f;
//    return vec_wrap<proxy<scalar<T>,F, std::multiplies<optype< scalar<T>,F>>>>{scalar<T>{x},y,f};
//};
//
//template<typename T,typename F,typename unused = typename isScalar<F>::value_type>
//vec_wrap< proxy<T,scalar<F>, std::multiplies<optype<T,scalar<F>>>>> operator*(const vec_wrap<T>& x,const F& y){
//    std::multiplies<optype<T,scalar<F>>> f;
//    return vec_wrap<proxy<T,scalar<F>, std::multiplies<optype<T,scalar<F>>>>>{x,scalar<F>{y},f};
//};
//
////divide
//template<typename T,typename F>
//vec_wrap<proxy<T,F, std::divides<optype<T,F>> >> operator/(const vec_wrap<T>& x,const vec_wrap<F>& y){
//    std::divides<optype<T,F>> f;
//    return vec_wrap<proxy<T,F, std::divides<optype<T,F>> >>{x,y,f};
//};
//
//template<typename T,typename F,typename unused = typename isScalar<T>::value_type>
//vec_wrap< proxy<scalar<T>,F, std::divides<optype<scalar<T>,F>>>> operator/(const T& x,const vec_wrap<F>& y){
//    std::divides<optype<scalar<T>,F>> f;
//    return vec_wrap<proxy<scalar<T>,F, std::divides<optype<scalar<T>,F>>>>{scalar<T>{x},y,f};
//};
//
//template<typename T,typename F,typename unused = typename isScalar<F>::value_type>
//vec_wrap< proxy<T,scalar<F>, std::divides<optype<T,scalar<F>>>>> operator/(const vec_wrap<T>& x,const F& y){
//    std::divides<optype<T,scalar<F>>> f;
//    return vec_wrap<proxy<T,scalar<F>, std::divides<optype<T,scalar<F>>>>>{x,scalar<F>{y},f};
//};



#endif /* _Valarray_h */

