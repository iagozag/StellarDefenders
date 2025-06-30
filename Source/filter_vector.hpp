
#include <vector>
#include <functional>

template<typename T>
void filter(std::vector<T> &v, const std::function<bool (const T &)> &predicate) {
    size_t read_idx = 0, write_idx = 0;
    while(read_idx < v.size()) {
        while(read_idx < v.size() && !predicate(v[read_idx])) {
            read_idx++;
        }
        
        if(read_idx < v.size()) {
            if(write_idx != read_idx) {
                v[write_idx] = std::move(v[read_idx]);
            }
            write_idx++;
            read_idx++;
        }
    }

    v.erase(v.begin() + write_idx, v.end());
}
