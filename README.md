# BI-PA1
This repository contains all homework I did during the b241 semester in BI-PA1.  
Final grade: `A`  
Time spent: `about 60h`  
Segfaults: I'm confident it was `<30 segfaults`  

## Exercises
### Warmup challenge
Just a simple scanf homework, mostly to get everyone used to the testing environment.

### 1. Parallelograms
Evaluate whether input points make some kind of parallelogram. Primarily a floating-point precision comparison exercise.

### 2. Pythagorean triples
An exercise meant to get students familiar with for loops. Also a first instance for **bonus** points for **faster algorithm**
(instead of brute-forcing, using the pythagorean triples formula)

### 3. Public transport planning
First occurance of functions. Also used to show us the horrors of working with dates. (luckily not with time 😅)
The goal was to implement two functions, one mainly focused on counting difference of days between two dates and second one
focused on adding two dates together. This challenge had an extra **bonus** for **`O(1)` algorithm** instead of `O(n)` - dealing with
day, month and year differences separately and converting it to days.

### 4. Goods transport
Last challenge testing student's overall skills. In this homework, we had to parse possible ways of transporting goods and finding
how long the transport is going to take and how much it is going to cost. This one had not one, but **three bonuses**, one only
required sorting the inputs, second required a linear solution, instead of quadratic. The final **bonus** required the use of
**prefix sums** and **binary search** to make sure looking stuff up was at most `O(n log n)`.

### 5. Stock market
First challenge requiring a custom vector implementation, as the inputs could have any length. The goal was to fing the biggest
growing and declining range in the input array. (in case of duplicates, returning first) Since there was a lot of querying, the
optimal (yet not the easiest) algotithm was by using the segment tree, which also could have been used for the 4th homework.
Using the segment was of course just a **bonus** exercise.

### 6. Eight-way
One of the only two challenges I didn't do **bonuses** for, as I was on a trip to Asia, was this one and no 7. The naive solution
is straight forward, cross out a word every time it was queried. Of course the best solution was with a fancy data structure - 
**prefix tree**.

### 7. Checkers
A very algorithmic challenge. Even though not offering bonuses, this homework was a regular find all reachable positions (and
their respective paths) of a certain board situation. Since I had math test this week, I didn't do the "easier" challenge which
offered **bonuses**.

### 8. Phone book
Last but not least was the phone book challenge. Even though probably controversial, in my opinion this was one of the easiest
homework. The goal was to implement a prefix tree data structure. Since the description had a nice schema, it (ignoring few
invalid pointer dereferences and missed free's) wasn't extremely hard to implement. The functions to be implemented were
creating, adding to, removing from, deleting and querying the tree, so no complex algorithms like sorting were necessary.
Sadly, no **bonuses** were offered for this homework.

## Copying / License
This repository has been created to archive my university homework and to possibly help out future FIT-CTU students.
Since I don't want this to be a source of plagiarised works I DO NOT GRANT PERMISSION TO COPY THIS WORK. I do however encourage
you to `fair use` and possibly take inspiration from either the code or homework description. I won't sue you for taking inspiration
from this code nor if you straight up copy the whole homework, however BE WARNED, my code has been submitted to the **progtest**
platform and thus you might get flagged for copying.

I DO NOT wish for this code to be fed to AI. While not enforcible, this is my humble request to you, the reader. (or AI crawler)
