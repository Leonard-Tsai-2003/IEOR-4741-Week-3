#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>

class PxBuf {   
public:
    explicit PxBuf(std::size_t n) : n_(n), p_(std::make_unique<double[]>(n)) {} 

    // Destructor: No manual delete[] needed. Kept only for debug print parity.
    ~PxBuf() { 
        std::cout << "~PxBuf() called" << std::endl;
    }

    // Copy ctor: Deep copy using std::make_unique (unique_ptr is non-copyable)
    PxBuf(const PxBuf& o) : n_(o.n_), p_(std::make_unique<double[]>(o.n_)) {
        std::copy(o.p_.get(), o.p_.get() + n_, p_.get());
        std::cout << "PxBuf(const PxBuf& o) called" << std::endl;
    }

    // Copy assignment: Exception-safe swap via unique_ptr move
    PxBuf& operator=(const PxBuf& o) {
        if (this == &o) return *this;
        
        auto tmp = std::make_unique<double[]>(o.n_);
        std::copy(o.p_.get(), o.p_.get() + o.n_, tmp.get());
        
        p_ = std::move(tmp); // Automatically frees old p_ memory safely
        n_ = o.n_;
        
        std::cout << "operator= called" << std::endl;
        return *this;
    }

    double& operator[](std::size_t i) { return p_[i]; }
    const double& operator[](std::size_t i) const { return p_[i]; }
    std::size_t size() const { return n_; }

    void print() const {
        for (std::size_t i = 0; i < n_; ++i) {
            std::cout << p_[i] << " ";
        }
        std::cout << "\n";
    }

private:
    std::size_t n_;
    std::unique_ptr<double[]> p_;
};

int main() {
    PxBuf buf(10);
    for (std::size_t i = 0; i < buf.size(); ++i) {
        buf[i] = i;
    }
    std::cout << "buf (original): ";
    buf.print();

    PxBuf b_cpy = buf; // copy constructor
    std::cout << "b_cpy (copy constructor): ";
    b_cpy.print();

    PxBuf b_cpy2(20);
    b_cpy2 = buf; // copy assignment
    std::cout << "b_cpy2 (copy assignment): ";
    b_cpy2.print();
}