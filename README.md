# NormalRelation

A C++20 library for normalizing database schemas using classical theory (3NF, BCNF, 4NF).

---
## Building and Running

To build and run the example code, follow these steps:
```bash
git clone https://github.com/StarFriedTree/NormalRelation.git
```
Using CMake:
```cmd
mkdir build
cd build
cmake ..
cmake --build .
```
Run the example:
```cmd
./NormalRelation.exe
```
___

## General Flow of Use

```cpp
// Create a Relation
Relation R("R");

// Create Attributes (optional to create separately, Relation stores copies)
Attribute A("A"), B("B"), C("C");

// Assign Attributes to Relation
R.addAttribute(A);
R.addAttribute(B);
R.addAttribute(C);

// Create Functional Dependencies (FDs)
	// Note: FDs (and MVDs) store pointers to Attributes
FD fd1;
fd1.addLeft(&A);
fd1.addRight(&B);

FD fd2;
fd2.addLeft(&A);
fd2.addLeft(&B);
fd2.addRight(&C);

//Create MVDs (optional, similar to FDs)
MVD mvd1;
mvd1.addLeft(&B);
mvd1.addRight(&C);

// Add FDs and MVDs to Relation
	// Note: A Relation only accepts Dependencies containing Attributes it itself contains.
	//		 Pointers to equivalent Attributes are allowed (converted to pointers to Relation's Attributes))
R.addFD(fd1);
R.addFD(fd2);
R.addMVD(mvd1);

// Display Relation
	// using << operator
std::cout << R << '\n';
	// using .display() method
R.display();

// Decompose Relation into 3NF, BCNF, or 4NF
	// Returns a vector of Relations (schemas)

auto schemas3NF = R.decompose3NF();
auto schemasBCNF = R.decomposeBCNF();
auto schemas4NF = R.decompose4NF();

// Display decomposed schemas
std::cout << "3NF Schemas:\n" << Util::printSet(schemas3NF, " ", " ", "") << '\n';
std::cout << "BCNF Schemas:\n" << Util::printSet(schemasBCNF, " ", " ", "") << '\n';
std::cout << "4NF Schemas:\n" << Util::printSet(schemas4NF, " ", " ", "") << '\n';
```

---

## Normalizer Algorithms (Pseudocode Overview)

### 3NF Decomposition
1.	Compute a minimal basis for FDs.
2.	For each FD, create a schema with the FD's attributes.
3.	Ensure at least one schema contains a candidate key.
4.	Remove redundant schemas (subsets of others).
5.	Project FDs onto each schema.
6.	Return the set of schemas.


### BCNF Decomposition
1.	While there exists a FD X --> Y violating BCNF (X is not a superkey): 
	
	a. Decompose R into: 
		
		- R₁ = X⁺ 
		- R₂ = R - (R₁ - X)
	b. Recursively apply BCNF to R₁ and R₂.
2.	If no violations, return R.


### 4NF Decomposition
1.	Compute a minimal basis for MVDs (singleton RHS, minimal LHS, no trivial/implied).
2.	While there exists a non-trivial MVD X -->> Y where X is not a superkey: 
	
	a. Decompose R into: 
		
		- R₁ = X U Y 
		- R₂ = R - (Y - X) 
	b. Recursively apply 4NF to R₁ and R₂.
3.	If no violations, return R.


---

## Future Developments

- **Lossless Decomposition & Anomaly Checks:**  
  Add utilities to verify that decompositions are lossless and free from update, insertion, and deletion anomalies.

- **Robustness Improvements:**  
  Make all functions more robust by removing assumptions about input state and validating inputs more thoroughly.

- **Operator Overloading:**  
  Overload more operators for sets and relation objects to simplify code and improve usability.

- **Algorithm Optimization:**  
  Optimize normalization algorithms for performance, including exploring multi-threaded implementations for large schemas.


---

## Contributions

Contributions, bug reports, and suggestions are welcome!