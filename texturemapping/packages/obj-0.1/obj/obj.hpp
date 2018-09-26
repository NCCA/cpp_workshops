#ifndef OBJ_OBJ_HPP_INCLUDED
#define OBJ_OBJ_HPP_INCLUDED

#include <cstddef>

#ifdef _WIN32
    #include <unordered_map>
    #include <memory>
    #include <tuple>
#else
    #include <tr1/unordered_map>
    #include <tr1/memory>
    #include <tr1/tuple>
#endif

namespace obj {

typedef std::size_t size_type;
typedef std::ptrdiff_t index_type;
typedef double float_type;
typedef std::tr1::tuple<index_type, index_type> index_2_tuple_type;
typedef std::tr1::tuple<index_type, index_type, index_type> index_3_tuple_type;

} // namespace obj

#endif // OBJ_OBJ_HPP_INCLUDED
