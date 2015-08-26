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
class vector{
	
};

} //namespace epl

#endif
