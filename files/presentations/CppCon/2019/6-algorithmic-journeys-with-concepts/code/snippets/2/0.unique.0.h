template<typename It>
It unique(It first, It last) {
    if (first == last) { return last; }
    It result = first;
    ++first;
    while (first != last) {
        if (*result == *first) {
           ++first;
        } else {
            ++result;
            *result = *first;
            ++first;
        }
    }
    ++result;
    return result;
}


