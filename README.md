# Simple Interpreter Project

<br>

## Overview
A custom programming language interpreter written in C++

<br>


## Features
<br>


### **Variable DECLARATION and ASSIGNMENT:**

Variables can be declared with the `let` - keyword.

```js
let x = 0;
x = 1;
```

Constant variables can be declared with the `const` - keyword.<br>
Constant variables cannot be reassigned.


```js
const pi = 3.1415;
```

<br>


### **Data TYPES and VALUES :**

The type of the variable is determined automatically and can therefore change during runtime, unless it`s a constant variable.

```js
let x = "Hello World";        # String-type #
x = 0;                        # Integer-type #
```

Variables can take integer values, float values, string values, boolean values ​​or null values.

```js
let x = 0;                    # Integer value #
let y = 0.0;                  # Float value #
let z = "Hello World!";       # String value #
let q = true;                 # Boolean value #
let w = null;                 # Null value #
```

<br>


### **BINARY operations :**

Binary operations such as addition, subtraction, multiplication, division, and modulo are supported.<br>
The arithmetic follows the usual arithmetic rules. Pemdas. <br>
Expressions in parentheses are treated first.

```js
let x = 5*(2-(-1));            # 15 #
let y = 15 % 2;                # 1 #
let x = 2^(2+(5%2));           # 8 #
```

<br>




### **String concatenation and type coercion for operations between different types :**
String concatenation is supported using the + operator.

```js
let x =  "Hello " + "World!"         # "Hello World!" #
```
Meaningful binary operations between different types are also possible.
```js
const x = "There are ";
let y = 3;
let z = x + y + " apples"             # "There are 3 apples" #

let isItTrue = true;
z = "That is " + isItTrue;            # "That is true" #
```
Multiplication operation between strings and numbers allows you concatenating a string multiple times.

```js
let x = "Hello" * 3;               # "HelloHelloHello" #
let y = "Hey" * 5.5;               # "HeyHeyHeyHeyHey" #
```


<br>


### **PRINT :**
Print something to the terminal window using the `print` - function.

```js
print("Hello World!");                   # output: "Hello World!" #
```

Variables can also be printed without any problems.

```js
const x = "Hello World!";
print(x);                                # output: "Hello World!" #
```
Or print both.
```js
const x = 2025;
print("It`s" + x);                        # output: "It´s 2025" #
```
You can also print expressions.
```js
print("It`s" + (2024 + (3%2)) );          # output: "It´s 2025" #
```


<br>


### **IF - Statements :**

IF-Statements allow you to execute conditional statements. <br>
If the given condition in the parentheses is true, the corresponding block is executed.
```js
let x = 5;
let y = 2;

if( x > y ){
  print("x is bigger than y");
}

if( x < y ){
  print("x is smaller than y");
}

if( x == y ){
  print("x is as big as y");
}
```

With the ELSE-Statements, a code block can be executed if the condition is not met.

```js
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
They consist of three parts: initialization, condition, and update, which are all defined in the loop header.<br>
The loop will be executed as long as the given condition stays true.

```js
for(let i = 1; i <= 10; i = i + 1;){
  print(i + "");
}
```
Whether the condition is true is checked each time the code block is executed.


<br>


### **WHILE-Loops :**
WHILE-Loops allow you to repeatedly execute a block of code as long as a specified condition is true.<br>
The condition is checked before each iteration, if it evaluates to false, the loop ends immediately.

```js
let run = true;
let i = 0;

while(run == true){
  print("" + i);
  i = i + 1;
  if(i == 10){
    run = false;
  }
}
```


<br>


### **DO-WHILE-Loops :**
DO-WHILE-Loops are similar to WHILE-loops, but the condition is checked after the code block is executed.<br>
This guarantees that the loop body runs at least once, even if the condition is initially false.

```js
let i = 0;

do{
  print("" + i);
  i = i + 1;
}while(i < 10);
```


<br>


## **BREAK & CONTINUE :**

The `break` and `continue` keywords are used to control the flow inside loops.<br>

The `break` keywords immediately terminates the nearest enclosing loop, skipping any remaining iterations and continuing execution after the loop.

```js
for(let i = 0; i < 10; i = i + 1;){
  if(i == 5){
    break;      # Stop completely when i is 5 #
  }
}
```
The `continue` keywords skips the current iteration and jumps directly to the next loop check, leaving the loop running.

```js
for(let i = 0; i < 10; i = i + 1;){
  if(i == 5){
    continue;   # Skip current iteration and continues with next #
  }
  print(" " + i);
}               # output : 0 1 2 3 4 6 7 8 9 #
```



<br>


### **User-INPUT :**

By using the `in` - function you can get user inputs.

```js
let user_name = "";
in(user_name, s);
print("Hello " + user_name)
```
The `in` - function receives a variable and an input type as arguments.<br>
The input type specifies what kind of value the variable should store. <br>
Numbers are specified with `n`, strings with `s`, and Booleans with `b`.


<br>


### **FUNCTIONS :**

Functions allow you to define reusable code blocks that can be called with different parameters. <br>
A function is defined using the `function` keyword, followed by the function name, parameters in parentheses, and the code block in curly braces.<br>
Functions are called using the `call` keyword and can return values using the `return` expression, helping to structure code while avoiding repetition.


```js
function add(x, y){
  print("You are adding" + x + " and " + y);
  return x + y; 
}

let sum = call add(1, 2);

print("The sum of 1 and 2 is: " + sum);
```


<br>


### **COMMENTS :**
Comments are used to add explanatory text to your code. <br>
Comments are ignored by the interpreter and do not affect the execution of the code. <br>
Comments are writen between `##`.

```js
let i = 0; #This is a comment#

i = i #This is also a comment#  + 1;
```

<br>


### **Static one-dimensional ARRAYS :**
Arrays are used to store multiple values of the same type in a single variable.<br>
Arrays are defined using the `array` keyword, optionally followed by a size declaration using angle brackets < >, and then assigned with values in square brackets [ ].<br>
The type of an array is automatically inferred from its first element. All other elements must match this type.<br>


```js
array <5> numbers = [1, 2, 3, 4, 5];
array <> greetings = ["Hello", "Hi", "Hey"];
```
If a size is specified, and fewer values are provided, the remaining entries will automatically be filled with default values.<br>

```js
let size = 3;

array <size> numbers = [1];        # [1, 0, 0] #
array <size> words = ["Hello"];    # ["Hello", ""] #
array <size> bools = [true];       # [true, false, false] #
```
Accessing and modifying array elements is supported via index notation, where the first element has an index of 0.<br>

```js
array <5> numbers = [1, 2, 3, 4, 6];
numbers[4] = 5;
print(numbers[4]);
```
Likewise, assigning a value from one array to another is only allowed if both arrays have the same type.<br>

```js
array <5> numbers = [1, 2, 3, 4, 5];
array <5> numbers2 = [0, 2, 3, 4, 5];
numbers2[0] = numbers[0];
```

<br>


## Planned Features

- Support for copying arrays
- Support for multi-dimensional arrays
- Support for objects
- Nativ Functions


<br>


## Usage

#### **Windows :**

1. Download the caelum.exe executable from the src folder
2. cd to the folder containing the caelum.exe executable
3. Run the interpreter with the following command: <br>

`./caelum.exe filepath/to/script`
<br>

#### **Linux & Mac :**

1. Clone or download the repository to your local machine
2. Compile the interpreter in the src folder with the following command (g++ is required) : <br>

` g++ main.cpp -static -o caelum `
1. cd to the folder containing the caelum executable
2. Run the interpreter with the following command: <br>

`./caelum filepath/to/script`

<br>


## Example Code

```js
function exampleFunction(arr){

  const y = "Your age is : ";
  let x = 16;
  x = x + 1;
  print( y + x );


  let isHuman = false;
  print("Are you a human? (true/false): ");
  in(isHuman, b);

  if(isHuman == true){
    print("Hello Human!");
  }else{
    print("You are not a human!");
  }

  let w = x + 1;

  if( w > x ){
    print(w + " is bigger than " + x);
  }else{
    print(w + " is smaller than " + x);
  }


  print("You got : "); #Print something#

  for (let i = 0; i < 10; i = i+1;){
    if(i == 5){
      continue;
    }
    print("" + arr[i] );
  }

  return "hey";

}

array <10> numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];

let greeting = call exampleFunction(numbers);

print("");
print(greeting);

```
<br>