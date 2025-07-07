# Simple Interpreter Project

<br>

## Overview
A custom programming language interpreter written in C++

<br>


## Features
<br>


### **Variable DECLARATION and ASSIGNMENT:**

Variables can be declared with the `let` - keyword.

```
let x = 0;
x = 1;
```

Constant variables can be declared with the `const` - keyword.<br>
Constant variables cannot be reassigned.


```
const pi = 3.1415;
```

<br>


### **Data TYPES and VALUES :**

The type of the variable is determined automatically and can therefore change during runtime, unless it`s a constant variable.

```
let x = "Hello World";        # String-type #
x = 0;                        # Integer-type #
```

Variables can take integer values, float values ,string values, boolean values ​​or null values.

```
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

```
let x = 5*(2+1);            # 15 #
let y = x % 2;              # 1 #
```

<br>




### **String concatenation and type coercion for operations between different types :**
String concatenation is supported using the + operator.

```
let x =  "Hello " + "World!"         # "Hello World!" #
```
Meaningful binary operations between different types are also possible.
```
const x = "There are ";
let y = 3;
let z = x + y + " apples"             # "There are 3 apples" #

let isItTrue = true;
z = "That is " + isItTrue;            # "That is true" #
```
<br>


### **PRINT :**
Print something to the terminal window using the `print` - function.

```
print("Hello World!");                   # output: "Hello World!" #
```

Variables can also be printed without any problems.

```
const x = "Hello World!";
print(x);                                # output: "Hello World!" #
```
Or print both.
```
const x = 2025;
print("It`s" + x);                        # output: "It´s 2025" #
```
You can also print expressions.
```
print("It`s" + (2024 + (3%2)) );          # output: "It´s 2025" #
```


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

if( x == y ){
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
They consist of three parts: initialization, condition, and update, which are all defined in the loop header.<br>
The loop will be executed as long as the given condition stays true.

```
for(let i = 1; i <= 10; i = i + 1;){
  print(i + "");
}
```
Whether the condition is true is checked each time the code block is executed.


<br>


### **WHILE-Loops :**
WHILE-Loops allow you to repeatedly execute a block of code as long as a specified condition is true.<br>
The condition is checked before each iteration, if it evaluates to false, the loop ends immediately.

```
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

### **User-INPUT :**

By using the `in` - function you can get user inputs.

```
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


```
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

```
let i = 0; #This is a comment#

i = i #This is also a comment#  + 1;
```

<br>


## Planned Features


- Support for objects
- Nativ Functions


<br>


## Example Code

```
function exampleFunction(){
  const y = "Your age is : ";
  let x = 16;
  x = x + 1;
  print( y + x );


  let isHuman = false;
  print("Are you a human? (true/false): ");
  in(isHuman, b);

  if(isHuman == true){
    print("Hello Human!);
  }else{
    print("You are not a human!");
  }


  let w = x + 1;

  if( w > x ){
    print(w + " is bigger than " + x);
  }else{
    print(w + " is smaller than " + x);
  }


  print("You got : "); #Print count of strawberries#

  for (let i = 0; i < 10; i = i+1;){
      if(i = 1){
          print("" + i + " strawberry");
      }else{
          print("" + i + " strawberries");
      }
  }
}

call exampleFunction();

```
<br>