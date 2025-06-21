# Simple Interpreter Project

<br>

## Overview
A custom programming language interpreter written in C++

<br>


## Features
<br>


### **Variable declaration and assignment:**

Variables can be declared with the `let` - keyword.

```
let x = 0;
x = 1;
```

Constant variables can be declared with the `const` - keyword.<br>
Constant variables cannot be reassigned.


```
const pi = 31415/10000;
```

<br>


### **Data types and values :**

The type of the variable is determined automatically and can therefore change during runtime, unless it`s a constant variable.

```
let x = "Hello World";        // String-type
x = 0;                        // Integer-type
```

Variables can take integer values, string values, boolean values ​​or null values.

```
let x = 0;                    // Integer value
let y = "Hello World!";       // String value
let z = true;                 // Boolean value
let q = null;                 // Null value
```

<br>


### **Binary operations :**

Binary operations such as addition, subtraction, multiplication, division, and modulo are supported.<br>
The arithmetic follows the usual arithmetic rules. Pemdas. <br>
Expressions in parentheses are treated first.

```
let x = 5*(2+1);            // 15
let y = x % 2;              // 1
```

<br>




### **String concatenation and type coercion for operations between different types :**
String concatenation is supported using the + operator.

```
let x =  "Hello " + "World!"             // "Hello World!"
```
Meaningful binary operations between different types are also possible.
```
const x = "There are ";
let y = 3;
let z = x + y + " apples"             // "There are 3 apples"

let isItTrue = true;
z = "That is " + isItTrue;            // "That is true"
```
<br>


### **Print :**
Print something to the terminal window using the `print` - function.

```
print("Hello World!");                   // output: "Hello World!"
```

Variables can also be printed without any problems.

```
const x = "Hello World!";
print(x);                                // output: "Hello World!"
```
Or print both.
```
const x = 2025;
print("It`s" + x);                        // output: "It´s 2025"
```
You can also print expressions.
```
print("It`s" + (2024 + (3%2)) );          // output: "It´s 2025"
```


<br>


### **Floating-points :**

Floating-point numbers cannot yet be assigned directly as values.<br>
However, this can be easily circumvented.
<br>


For example, we want to represent the number 1.23.
```
let x= 123/100;
print(x);          // output: "1.23"
```
Support for floating point numbers will follow.


<br>


### **IF - Statements :**

IF-Statements allow you to execute conditional statements. <br>
If the given condition in the parentheses is true, the corresponding block is executed.
```
let x = 5;
let y = 2;

if( x > y ){
  print("x is bigger than y");
}

if( x < y ){
  print("x is smaller than y");
}

if( x = y ){
  print("x is as big as y");
}
```

With the ELSE-Statements, a code block can be executed if the condition is not met.

```
let x = 1;
let y = 10;

if( x > y ){
  print("x is bigger than y");
}else{
  print("x is smaller than y");
}
```

<br>


### **FOR-Loops :**
FOR-Loops allow you to execute a code block multiple times.<br>
The loop will be executed as long as the given condition stays true.

```
for(let i = 0; i < 10; i = i + 1;){
  print(i + "");
}
```
Whether the condition is true is checked each time the code block is executed.


<br>




## Planned Features
<br>

- Support for user inputs
- Support for floating-points
- Support for objects
- Nativ Functions
- User defined Functions
- User defined Comments

<br>

## Example Code

```
let y = "Your age is : ";
let x = 16;
x = x + 1;
print( y + x );

const isHuman = true;
print("Are you a human: "+ isHuman);

let w = x + 1;
if( w > x ){
  print(w + " is bigger than " + x);
}else{
  print(w + " is smaller than " + x);
}

print("You got : ");

for (let i = 0; i < 10; i = i+1;){
    if(i = 1){
        print("" + i + " strawberry");
    }else{
        print("" + i + " strawberries");
    }
}
```
