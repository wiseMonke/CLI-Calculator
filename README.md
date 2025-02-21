# CLI Calculator 🧮

This project is a **high-precision command-line calculator** written entirely in C. It includes custom mathematical functions designed for **high accuracy** using advanced numerical methods.

---

## 🚀 Features

- 🧮 Supports basic and advanced mathematical operations  
- 📏 Implements **custom functions** for high-accuracy calculations  
- ⚡ Optimized using **Horner's method** for efficient polynomial evaluation  
- 📊 Uses **Chebyshev polynomials** and the **Remez algorithm** for function approximations  

---

## 🔧 Installation & Compilation

To compile and run the program, use the following commands:

```bash
# Compile the program
gcc -o calculator calc.c -lm

# Run the program
./calculator
```
##### **Note**: *Make sure gcc is installed*
---

## 🖥️ Example Usage
![Screenshot 2025-02-20 235737](https://github.com/user-attachments/assets/776658e8-090d-401c-aa69-6789dda31766)


## 📚 Algorithm Overview

This CLI calculator leverages advanced numerical methods for efficiency:

- **Horner's Method**: Reduces the number of multiplications in polynomial evaluation.

- **Chebyshev Polynomials**: Used for high-accuracy function approximations.

- **Remez Algorithm**: Finds the optimal polynomial approximation with minimal error.

These techniques ensure that calculations are performed quickly and accurately, making this calculator a powerful tool for numerical computations.

---

## 🎓 Personal Learning
Throughout this project, I gained deep insights into several advanced mathematical and computational techniques:

- [Abstract Syntax Trees (ASTs) ](https://en.wikipedia.org/wiki/Abstract_syntax_tree): Used to parse and structure mathematical expressions efficiently.
  
- [Chebyshev Polynomials](https://en.wikipedia.org/wiki/Chebyshev_polynomials): Essential for approximating functions with minimal error.
  
- [Minimax Approximation](https://en.wikipedia.org/wiki/Minimax): Learned how to minimize the maximum error in function approximations, *(very interesting compared to [Least Squares](https://en.wikipedia.org/wiki/Least_squares), which minimizes average error)*
  
- [Remez Algorithm](https://en.wikipedia.org/wiki/Remez_algorithm): Understood how to generate optimal polynomial approximations.
  
This project significantly improved my understanding of numerical analysis, function approximation, and performance optimizations in low-level programming.

It was very interesting to actually apply some of the math I learned in school and on my own. I learned about other approximations other than Taylor Series.

It also serves as a learning for parsing & lexing expressions, which will come in handy, as I plan to write my own compiler in the future.

---

### 🔮 Future Improvements

- 🌀 Adding support for complex numbers
- ➕ Expanding the set of custom mathematical functions
- 🚀 Further optimizations for faster execution
- ⚡ Vectorization using SIMD for increased performance
- 🔢 Exploring rational approximations (Pade Approximants) for better accuracy in transcendental functions

---
## 📜 License

This project is open-source and available under the MIT License.

