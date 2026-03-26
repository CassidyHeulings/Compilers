# Lab 3
## Crafting a Compiler
### 4.7
(a)  
\<Start>  
<E> $  
<T> plus <E> $  
<F> plus <E> $  
num plus <E> $  
num plus <T> plus <E> $  
num plus <T> times <F> plus <E> $  
num plus <F> times <F> plus <E> $  
num plus num times <F> plus <E> $  
num plus num times num plus <E> $  
num plus num times num plus <T> $  
num plus num times num plus <F> $  
num plus num times num plus num $  
(b)  
\<Start>  
<E> $  
<T> plus <E> $  
<T> plus <T> $  
<T> plus <T> times <F> $  
<T> plus <T> times num $  
<T> plus <F> times num $  
<T> plus num times num $  
<T> times <F> plus num times num $  
<T> times num plus num times num $  
<F> times num plus num times num $  
num times num plus num times num $  
(c)  
This grammar makes sure the order of operations is done correctly by putting plus and times in two different rules and adding () whenever we go from F to E in rules 6. Rule 6 allows for any expressions within () to be further down the tree so they are done first (using in order depth first traversal). If we did not have multiplication and addition in seperate rules (E becomes T which then becomes multiplication) then we would have addition and multiplication on the same level in the tree, making which every operation is next in the order go first since there would be no difference in depth. This would also allow addition to be done before multiplication.
### 5.2
(c)   
parseStart() {  
 addNode(root,start)  
 parseValue()   
 match($)  
 moveUp()  
}  
parseValue() {  
 addNode(branch,value)  
 if (checkExpected is num) {  
  match(num)  
 }  
 else {  
  match(lparen)  
  parseExpr()  
  match(rparen)  
  moveUp()  
 }  
}  
parseExpr() {  
 addNode(branch,expr)  
 if (checkExpected is plus) {  
  match(plus)  
  parseValue()  
  parseValue()  
 }  
 else {  
  match(prod)  
  parseValues()  
 }  
 moveUp()  
}  
parseValues() {  
 if (checkExpected is a Value) {  
  addNode(branch,values)  
  parseValue()  
  parseValues()  
 }  
 else {  
  // null  
 }  
 moveUp()  
}  
match(expected) {  
 x = checkExpected  
 if x addNode(leaf,x)  
 else error  
}  
## Dragon
## 4.2.1
My textbook says 5 -> S S + \ S S * \ a  
so I am assuming it says S -> S S + | S S * | a  
(a)  
\<S>  
\<S> \<S> *  
\<S> \<S> + \<S> *  
 a  \<S> + \<S> *  
 a   a  + \<S> *  
 a   a  +  a  *  
(b)  
\<S>  
\<S> \<S> *  
\<S>  a  *  
\<S> \<S> +  a  *  
\<S>  a  +  a  *  
 a   a  +  a  *  
(c)  
S  
~ S  
~ ~ S  
~ ~ ~ a  
~ ~ S  
~ ~ ~ a  
~ ~ +  
~ S  
~ ~ a  
~ *  
