# IEOR 4741 - Week 3: C++ Memory Management, RAII, and Smart Pointers

This repository contains the implementation and analysis for IEOR 4741 Week 3 assignments focusing on modern C++ dynamic memory management, the Rule of Three, Resource Acquisition Is Initialization (RAII), and smart pointer performance benchmarks (`std::unique_ptr` vs. `std::shared_ptr`).

---

## Table of Contents

- [Overview](#overview)
- [Repository Structure](#repository-structure)
- [Part 1: Rule of Three & Raw Buffer Management](#part-1-rule-of-three--raw-buffer-management)
- [Part 2: Refactoring with `std::unique_ptr`](#part-2-refactoring-with-stdunique_ptr)
- [Part 3: RAII Scope Guard (Timer)](#part-3-raii-scope-guard-timer)
- [Part 4: Smart Pointer Benchmark & Reference Count Overhead](#part-4-smart-pointer-benchmark--reference-count-overhead)

---

## Overview

1. **Part 1 (`hw3_raw.cpp`)**: Manages dynamic memory (`new[]`/`delete[]`) using explicit manual memory management adhering to the **Rule of Three** (destructor, copy constructor, and copy assignment operator).
2. **Part 2 (`hw3_unique_ptr.cpp`)**: Modernizes the raw buffer management using `std::unique_ptr` and analyzes what logic smart pointers remove.
3. **Part 3 (`hw3_RAII.cpp`)**: Implements an RAII execution timer guard that logs task durations inside its destructor upon exiting scope.
4. **Part 4 (`hw3_benchmark.cpp`)**: Benchmarks performance differences between `std::unique_ptr` and `std::shared_ptr` access/copy operations, explaining the atomic reference-counting overhead.

---

## Repository Structure

```
.
├── hw3_raw.cpp          # Part 1: Raw buffer management implementing Rule of Three
├── hw3_unique_ptr.cpp   # Part 2: Buffer management refactored with std::unique_ptr
├── hw3_RAII.cpp         # Part 3: RAII Timer guard implementation
└── hw3_benchmark.cpp    # Part 4: Smart pointer performance benchmark
```

---

## Part 1: Rule of Three & Raw Buffer Management

The class `PxBuf` manages a raw dynamically-allocated integer array (`new[]` / `delete[]`). To prevent memory leaks, dangling pointers, and double-free vulnerabilities, the class implements:
- **Destructor (`~PxBuf()`)**: Releases allocated buffer memory using `delete[]`.
- **Copy Constructor (`PxBuf(const PxBuf& o)`)**: Performs deep copying of the original buffer.
- **Copy Assignment Operator (`operator=`)**: Handles deep copying while guarding against self-assignment and properly clearing pre-existing memory allocations.

### Build & Run
```bash
clang++ -std=c++17 -O0 -g hw3_raw.cpp -o /tmp/mem && /tmp/mem
```

### Output
```text
buf (original): 0 1 2 3 4 5 6 7 8 9 
PxBuf(const PxBuf& o) called
b_cpy (copy constructor): 0 1 2 3 4 5 6 7 8 9 
operator= called
b_cpy2 (copy assignment): 0 1 2 3 4 5 6 7 8 9 
~PxBuf() called
~PxBuf() called
~PxBuf() called
```

---

## Part 2: Refactoring with `std::unique_ptr`

`hw3_unique_ptr.cpp` replaces raw memory handling with standard smart pointer ownership using `std::unique_ptr<double[]>`.

### What Smart Pointers Remove & Simplify
- **Manual Destruction**: Eliminates explicit `delete[]` calls inside destructors; memory is automatically released when `std::unique_ptr` leaves scope.
- **Memory Leak & Double-Free Prevention**: Compiler guarantees deterministic cleanup, eliminating dangling pointer access and double-deletion errors.
- **Boilerplate Reduction**: Replaces repetitive allocation/deallocation logic with clear scope-bound ownership semantics.
- **Exception Safety**: Automatically releases resources even if exceptions are thrown during execution.

### Build & Run
```bash
clang++ -std=c++17 -O0 -g hw3_unique_ptr.cpp -o /tmp/mem && /tmp/mem
```

### Output
```text
buf (original): 0 1 2 3 4 5 6 7 8 9 
PxBuf(const PxBuf& o) called
b_cpy (copy constructor): 0 1 2 3 4 5 6 7 8 9 
operator= called
b_cpy2 (copy assignment): 0 1 2 3 4 5 6 7 8 9 
~PxBuf() called
~PxBuf() called
~PxBuf() called
```

---

## Part 3: RAII Scope Guard (Timer)

In `hw3_RAII.cpp`, an RAII-compliant `Timer` class captures high-resolution timestamps upon instantiation and automatically outputs the total elapsed time in milliseconds within its destructor when exiting its lexical scope.

### Build & Run
```bash
clang++ -std=c++17 -O0 -g hw3_RAII.cpp -o /tmp/mem && /tmp/mem
```

### Output
```text
[Computation Task] Elapsed time: 5.22854 ms
[Sleep Task] Elapsed time: 55.0249 ms
```

---

## Part 4: Smart Pointer Benchmark & Reference Count Overhead

`hw3_benchmark.cpp` evaluates the performance overhead of pointer dereferencing (`.get()`) vs. pointer copying across `std::unique_ptr` and `std::shared_ptr`.

### Performance Analysis & Atomic Refcount Overhead
- **Raw Pointer Access (`.get()`)**: Both `unique.get()` (~0.41 ns) and `shared.get()` (~0.37 ns) exhibit near-identical raw pointer access costs with virtually zero abstraction penalty.
- **Shared Pointer Copy Overhead**: Copying a `std::shared_ptr` (~4.56 ns) is roughly **10x slower** than raw pointer access. This performance drop occurs because `std::shared_ptr` must increment its internal shared reference counter using thread-safe **atomic operations** (`std::atomic`). Atomic increments require hardware-level cache synchronization across CPU cores, adding significant clock cycle delays compared to non-atomic moves or direct pointer access.

### Build & Run
```bash
clang++ -std=c++17 -O2 -Itests hw3_benchmark.cpp -o /tmp/mem && /tmp/mem
```

### Output
```text
unique.get=0.41 ns  shared.get=0.37 ns  shared copy=4.56 ns
~Order 4
~Order 3
```