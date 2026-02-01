#include "../exercise.h"
#include <map>

template<class k, class v>
bool key_exists(std::map<k, v> const &map, k const &key) {
    return map.find(key) != map.end();
}

template<class k, class v>
void set(std::map<k, v> &map, k key, v value) {
    map[key] = value;
}

int main(int argc, char **argv) {
    using namespace std::string_literals;
    std::map<std::string, std::string> secrets;
    
    set(secrets, "hello"s, "world"s);
    ASSERT(key_exists(secrets, "hello"s), "\"hello\" shoud be in `secrets`");
    
    return 0;
}