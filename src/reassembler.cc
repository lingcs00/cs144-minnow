#include "reassembler.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring, Writer& output )
{
  size_t max_cache_size = output.available_capacity();
  // 计算receiver windows[target_idx ,target_idx + max_cache_size]
  if ( !( ( first_index + data.size() ) >= target_byte_id
          && first_index <= ( target_byte_id + max_cache_size ) ) ) {
    // 丢弃包
    return;
  }
  // 如果当前的数据包不满足windows条件，先减成满足条件的
  // 清理后面的数据
  int windows_end_idx = target_byte_id + max_cache_size;
  int cur_end_idx = first_index + data.size();
  int overlay = cur_end_idx - windows_end_idx;
  if ( overlay > 0 ) {
    data = data.substr( 0, data.size() - overlay );
  }

  overlay = target_byte_id - first_index;
  if ( overlay > 0 ) {
    data = data.substr( overlay, data.size() - overlay );
    first_index += overlay;
  }

  // 已经缓存过了丢弃
  if ( cache_umap.count( first_index ) == 1 && cache_umap[first_index].data.size() > data.size() ) {
    return;
  }

  cache_umap[first_index] = { data, is_last_substring };

  // printf( "before combine : \n" );
  // for ( auto e : cache_umap ) {
  //   printf( "idx = %ld data = %s \n", e.first, e.second.data.c_str() );
  // }

  auto it = cache_umap.begin();
  // 合并不同的子区间
  while ( true ) {
    auto it_next = next( it );
    if ( it_next == cache_umap.end() )
      break;

    size_t idx = it->first;
    pack_data& pkg = it->second;

    overlay = idx + pkg.data.size() - it_next->first;
    if ( overlay >= 0 ) {
      pack_data& next_pkg = it_next->second;
      if ( (size_t)overlay <= next_pkg.data.size() ) {
        // 合并
        pkg.data += next_pkg.data.substr( overlay, next_pkg.data.size() - overlay );
        pkg.end |= next_pkg.end;
      }

      cache_umap.erase( it_next );
      continue;
    }

    it = it_next;
  }

  // printf( "after combine : \n" );
  // for ( auto e : cache_umap ) {
  //   printf( "idx = %ld data = %s \n", e.first, e.second.data.c_str() );
  // }

  // 输出
  if ( cache_umap.begin()->first == target_byte_id ) {
    pack_data& pkg = cache_umap.begin()->second;
    output.push( pkg.data );
    target_byte_id += pkg.data.size();
    if ( pkg.end ) {
      output.close();
    }
    cache_umap.erase( cache_umap.begin() );
  }
}

uint64_t Reassembler::bytes_pending() const
{
  uint64_t cache_bytes = 0;
  for ( auto e : cache_umap ) {
    cache_bytes += e.second.data.size();
  }
  return cache_bytes;
}