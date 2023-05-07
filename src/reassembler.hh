#pragma once

#include "byte_stream.hh"
#include <algorithm>
#include <list>
#include <string>
#include <vector>

class Reassembler
{

  struct byte_node
  {
    char ch;
    bool flag; // 0 invalid 1 valid 2 last_string
  };

protected:
  size_t target_byte_id = 0; // 记录当前期望的数据包id
  std::vector<byte_node> cache_vec;
  size_t end_byte_id = -1;
  // std::list<sub_node> cache_list; // 存放没有reorder的数据包
  // size_t cache_bytes = 0;

public:
  /*
   * Insert a new substring to be reassembled into a ByteStream.
   *   `first_index`: the index of the first byte of the substring
   *   `data`: the substring itself
   *   `is_last_substring`: this substring represents the end of the stream
   *   `output`: a mutable reference to the Writer
   *
   * The Reassembler's job is to reassemble the indexed substrings (possibly out-of-order
   * and possibly overlapping) back into the original ByteStream. As soon as the Reassembler
   * learns the next byte in the stream, it should write it to the output.
   *
   * If the Reassembler learns about bytes that fit within the stream's available capacity
   * but can't yet be written (because earlier bytes remain unknown), it should store them
   * internally until the gaps are filled in.
   *
   * The Reassembler should discard any bytes that lie beyond the stream's available capacity
   * (i.e., bytes that couldn't be written even if earlier gaps get filled in).
   *
   * The Reassembler should close the stream after writing the last byte.
   */
  Reassembler() : target_byte_id( 0 ), cache_vec() {}
  ~Reassembler() {}
  void insert( uint64_t first_index, std::string data, bool is_last_substring, Writer& output );

  void assembler_cache_insert( uint64_t index, std::string& data, size_t free_cache );
  void assembler_cache_pop( size_t len );
  // How many bytes are stored in the Reassembler itself?
  uint64_t bytes_pending() const;
};
