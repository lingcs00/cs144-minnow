#include <stdexcept>

#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity )
  : capacity_( capacity ),s_data_(), accumulate_pop( 0 ), accumulate_push( 0 ), flag_( FLAG_OPEN )
{
  // 申请capacity大小的内存
  this->s_data_.reserve( capacity );
}

void Writer::push( string data )
{
  // Your code here.
  const size_t max_push = min( available_capacity(), data.size() );
  this->s_data_ += data.substr( 0, max_push );
  this->accumulate_push += max_push;
}

void Writer::close()
{
  // Your code here.
  this->flag_ = FLAG_CLOSED;
}

void Writer::set_error()
{
  // Your code here.
  this->flag_ = FLAG_ERROR;
}

bool Writer::is_closed() const
{
  // Your code here.
  return this->flag_ == FLAG_CLOSED;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return ( capacity_ - this->s_data_.size() );
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return this->accumulate_push;
}

string_view Reader::peek() const
{
  // Your code here.
  return s_data_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return flag_ == FLAG_CLOSED && s_data_.empty();
}

bool Reader::has_error() const
{
  // Your code here.
  return flag_ == FLAG_ERROR;
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  const size_t pop_size = min( s_data_.size(), len );
  this->s_data_.erase( 0, pop_size );
  this->accumulate_pop += pop_size;

}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return s_data_.size();
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return this->accumulate_pop;
}
