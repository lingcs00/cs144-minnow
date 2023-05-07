#include "reassembler.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring, Writer& output )
{
  if ( is_last_substring ) {
    end_byte_id = first_index + data.size();
  }
  size_t free_internal_bytes = output.available_capacity();
  cache_vec.resize( free_internal_bytes, byte_node { 0, 0 } );

  // 计算接受数据的区间[target_idx,target_idx + free_internel_bytes]
  // 实际数据的区间[first_index,first_index + data.size]
  // 区间取交集-->result [a,b]
  // printf( "insert [%ld,%ld]  cache[%ld ,%ld] \n",
  //         first_index,
  //         first_index + data.size(),
  //         target_byte_id,
  //         target_byte_id + free_internal_bytes );

  int n = data.size();
  for ( int i = 0; i < n; i++ ) {
    size_t t = first_index + i;
    if ( t >= target_byte_id && t < ( target_byte_id + free_internal_bytes ) ) {
      // 写入cache中
      // printf( "write cache idx = %ld \n", t - target_byte_id );
      cache_vec[t - target_byte_id] = { data[i], 1 };
    }
  }

  string str_out;
  for ( size_t i = 0; i < free_internal_bytes; i++ ) {
    if ( cache_vec[i].flag && ( target_byte_id + i ) <= end_byte_id ) {
      str_out.push_back( cache_vec[i].ch );
      cache_vec[i].flag = false;
    } else {
      break;
    }
  }
  output.push( str_out );
  // 调整vec的内容
  size_t outsize = str_out.size();
  target_byte_id += outsize;
  rotate( cache_vec.begin(), cache_vec.begin() + outsize, cache_vec.end() );
  if ( target_byte_id == end_byte_id ) {
    output.close();
  }
}

uint64_t Reassembler::bytes_pending() const
{
  uint64_t cache_bytes = 0;
  for ( auto e : cache_vec ) {
    cache_bytes += ( e.flag );
  }
  return cache_bytes;
}