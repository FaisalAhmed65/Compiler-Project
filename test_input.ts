// test_input.ts  –  Sample TypeScript input for the TS2C compiler
// Covers: variable declarations, arithmetic, if/else, while, for,
//         functions, console.log, boolean literals, null

// Variable declarations with type annotations
let x: number = 10;
let y: number = 20;
const PI: number = 3;

// Boolean
let flag: boolean = true;
let done: boolean = false;

// Null
let ptr: number = null;

// Arithmetic expression
let sum: number = x + y;
let product: number = x * y;
let diff: number = y - x;
let quotient: number = y / x;

// console.log output
console.log(sum);
console.log("Hello from TS2C");

// If / else
if (x < y) {
    console.log("x is smaller");
} else {
    console.log("y is smaller or equal");
}

// Nested if / else if
if (x == 10) {
    console.log("x is ten");
} else {
    console.log("x is not ten");
}

// While loop
let i: number = 0;
while (i < 5) {
    console.log(i);
    i = i + 1;
}

// For loop
let total: number = 0;
for (let j: number = 0; j < 10; j = j + 1) {
    total = total + j;
}
console.log(total);

// Function declaration
function add(a: number, b: number): number {
    return a + b;
}

function isPositive(n: number): boolean {
    if (n > 0) {
        return true;
    } else {
        return false;
    }
}

// Function calls
let result: number = add(3, 4);
console.log(result);

let positive: boolean = isPositive(5);
console.log(positive);

// Logical operators
let bothTrue: boolean = flag && positive;
let eitherTrue: boolean = done || flag;
let notDone: boolean = !done;

console.log(bothTrue);
console.log(eitherTrue);
console.log(notDone);
