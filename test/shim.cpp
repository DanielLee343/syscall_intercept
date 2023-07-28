#include <string.h>

inline string to_string(int __val){ return __gnu_cxx::__to_xstring<string>(&std::vsnprintf, 4 * sizeof(int),
                 "%d", __val); }