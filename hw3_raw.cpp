#include <algorithm>
#include <cstddef>
#include <iostream>


class PxBuf {   
public:                                               // owns a raw new[]
    explicit PxBuf(std::size_t n) : n_(n), p_(new double[n]) {} 
    ~PxBuf() { 
        delete[] p_; 
        std::cout << "~PxBuf() called" << std::endl;
    }                                   // 1. destructor

    PxBuf(const PxBuf& o) : n_(o.n_), p_(new double[o.n_]) {     // 2. copy ctor
        std::copy(o.p_, o.p_ + n_, p_);                          //    DEEP copy
        std::cout << "PxBuf(const PxBuf& o) called" << std::endl;
    }
    PxBuf& operator=(const PxBuf& o) {                           // 3. copy assign
        if (this == &o) return *this;        // self-assignment must be safe
        double* tmp = new double[o.n_];      // allocate BEFORE you free anything,
        std::copy(o.p_, o.p_ + o.n_, tmp);   // so a throw leaves *this intact
        delete[] p_;
        p_ = tmp; n_ = o.n_;
        std::cout << "operator= called" << std::endl;
        return *this;
    }

    double& operator[](std::size_t i) { return p_[i]; }
    std::size_t size() const { return n_; }
    void print() const {
        for (std::size_t i = 0; i < n_; ++i) {
            std::cout << p_[i] << " ";
        }
        std::cout << "\n";
    }
private:
    std::size_t n_;
    double*     p_;
};

int main(){
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