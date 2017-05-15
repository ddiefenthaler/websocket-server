#ifndef WEBSOCKET_SERVER__INTERNAL_CIRCITER
#define WEBSOCKET_SERVER__INTERNAL_CIRCITER

#include <iterator>

namespace websocket {

namespace internal {

template <class Iter>
class circular_iterator {
private:
  using self_t = circular_iterator<Iter>;

public:
  using difference_type   = typename std::iterator_traits<Iter>::difference_type;
  using value_type        = typename std::iterator_traits<Iter>::value_type;
  using pointer           = typename std::iterator_traits<Iter>::pointer;
  using reference         = typename std::iterator_traits<Iter>::reference;
  using iterator_category = typename std::iterator_traits<Iter>::iterator_category;

public:
  circular_iterator() = default;
  circular_iterator(const self_t & other) = default;
  circular_iterator(self_t && other) = default;
  self_t & operator=(const self_t & other) = default;
  self_t & operator=(self_t && other) = default;
  
  circular_iterator(Iter first, Iter last)
  : _first(first)
  , _cur(first)
  , _last(last)
  , _dist(std::distance(first, last))
  {}
  
  reference operator*() {
    return *_cur;
  }
  
  reference operator[](difference_type n) {
    return *(*this + n);
  }
  
  self_t & operator++() {
    if(_cur == _last) {
      _cur = _first;
    }
    ++_cur;
    if(_cur == _last) {
      _cur = _first;
    }
    return *this;
  }
  
  self_t operator++(int) {
    self_t ret(*this);
    ++(*this);
    return ret;
  }
  
  self_t & operator--() {
    if(_cur == _first) {
      _cur = _last;
    }
    --_cur;
    return *this;
  }
  
  self_t operator--(int) {
    self_t ret(*this);
    --(*this);
    return ret;
  }
  
  self_t & operator+=(difference_type n) {
    n  = n % _dist;
    n +=     _dist;
    n  = n % _dist;
    difference_type dist_to_last = std::distance(_cur,_last);
    if(n < dist_to_last) {
      std::advance(_cur, n);
    } else {
      _cur = std::next(_first,n - dist_to_last);
    }
  }
  
  self_t operator+(difference_type n) {
    self_t ret(*this);
    ret += n;
    return ret;
  }
  
  self_t & operator-=(difference_type n) {
    (*this) += -n;
  }
  
  self_t operator-(difference_type n) {
    self_t ret(*this);
    ret += -n;
    return ret;
  }
  
  self_t operator-(const self_t & other) {
    return _cur - other._cur;
  }
  
  bool operator==(const self_t & other) {
    return (_first == other._first) && (_last == other._last) && (_cur == other._cur);
  }
  
  bool operator!=(const self_t & other) {
    return !(*this == other);
  }
  
  bool operator<(const self_t & other) {
    return _cur < other._cur;
  }
  
  bool operator>(const self_t & other) {
    return _cur > other._cur;
  }
  
  bool operator>=(const self_t & other) {
    return _cur >= other._cur;
  }
  
  bool operator<=(const self_t & other) {
    return _cur <= other._cur;
  }
  
  bool operator==(const Iter & it) {
    return _cur == it;
  }
  
  bool operator!=(const Iter & it) {
    return _cur != it;
  }
  
  bool operator<(const Iter & it) {
    return _cur < it;
  }
  
  bool operator>(const Iter & it) {
    return _cur > it;
  }
  
  bool operator>=(const Iter & it) {
    return _cur >= it;
  }
  
  bool operator<=(const Iter & it) {
    return _cur <= it;
  }

private:
  Iter            _first;
  Iter            _cur;
  Iter            _last;
  difference_type _dist = 0;

};

} // internal

template <class Iter>
internal::circular_iterator<Iter> make_circular_iterator(Iter first, Iter last) {
  return internal::circular_iterator<Iter>(first, last);
}

} // websocket

#endif
