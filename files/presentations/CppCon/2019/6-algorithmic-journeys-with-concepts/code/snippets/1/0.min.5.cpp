struct employee {
    std::string full_name;
    int64_t salary;
};

void usage() {
  employee e0{"Bjarne Stroustrup", 9999999ll};
  employee e1{"Alex Stepanov", 9999999ll};
  min(e0, e1, [](const auto& x, const auto& y) {
    return x.salary < y.salary;
  }).salary += 10000ll;
}
