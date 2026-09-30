#include "bench.hpp" // -Itests
#include <cstdio>
struct Order
{
    int id;
    double px;
    Order(int i, double p) : id(i), px(p) {}
    ~Order() { printf("~Order %d\n", id); }
};

int main()
{
    auto up = std::make_unique<Order>(3, 100.0);
    auto sp = std::make_shared<Order>(4, 100.0);

    double uq = ns_per_op([&]{ Order* r = up.get(); doNotOptimize(r); }, 50'000'000);
    double shd = ns_per_op([&]{ Order* r = sp.get(); doNotOptimize(r); }, 50'000'000); // deref
    double shc = ns_per_op([&]{ auto c = sp; doNotOptimize(c.get()); }, 50'000'000); // COPY
    printf("unique.get=%.2f ns  shared.get=%.2f ns  shared copy=%.2f ns\n", uq, shd, shc);
    return 0;
}