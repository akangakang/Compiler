# Compiler

## Lab 1: Straight-line Program Interpreter

Implement a simple program analyzer and interpreter for the straight-line programming language. This exercise serves as an introduction to environments (symbol tables mapping variable-names to information about the variables); to abstract syntax (data structures representing the phrase structure of programs); to recursion over tree data structures, useful in many parts of a compiler; and to a functional style of programming without assignment statements.

It also serves as a “warm-up” exercise in C programming. Programmers experienced in other languages but new to C should be able to do this exercise, but will need supplementary material (such as textbooks) on C.

In this lab, you should modify the file myimpl.c. We have already given some hints for you in the comments.

Notice: Before you start this lab, you should carefully read the chapter 1 of the textbook.

##  Lab 2: Lexical Analysis

Use Lex to implement a lexical analyzer for the Tiger language. Appendix A describes, among other things, the lexical tokens of Tiger.

This chapter has left out some of the specifics of how the lexical analyzer should be initialized and how it should communicate with the rest of the compiler. You can learn this from the Lex manual, but the “skeleton” files in the lab directory will also help get you started.

Along with the tiger.lex file you should turn in documentation for the following point:

• how you handle comments;

• how you handle strings;

• error handling;

• end-of-file handling;

• other interesting features of your lexer.

Notice: Before you start this lab, you should carefully read the chapter 2 of the textbook, and you may need to get some help from the lex manual and the Tiger Language Reference Manual (Appendix A). 

## Lab 3: Parsing

Use Yacc to implement a parser for the Tiger language. Appendix A describes, among other things, the syntax of Tiger.

You should turn in the file tiger.y and read the file absyn.h.

Supporting files available in Lab 3 include:
  • Makefile The “Makefile”.

  • errormsg.[ch] The ErrorMessage structure, useful for producing errormessages with file names and line numbers.

  • tiger.lex You should use *your own* tiger.lex in lab 2 for subsequent labs.

  • tiger.y The skeleton of a file you must fill in.

You won’t need tokens.h anymore; instead, the header file for tokens is y.tab.h, which is produced automatically by Yacc from the token specification of your grammar.

Your grammar should have as few shift-reduce conflicts as possible, and no reduce-reduce conflicts.

Furthermore, your accompanying documentation should list each shift-reduce conflict (if any) and explain why it is not harmful.

Notice: Before you start this lab, you should carefully read the chapter 3 of the textbook

## Lab 4: Type Checking

Write a type-checking phase for your compiler, a module semant.c matching the following header file:

/* semant.h */

void SEM_transProg(A_exp exp);

that type-checks an abstract syntax tree and produces any appropriate error messages about mismatching types or undeclared identifiers.

Also provide the implementation of the Env module described in this chapter. You must use precisely the Absyn interface described in Figure 4.7, but you are free to follow or ignore any advice given in this chapter about the internal organization of the Semant module.
Notice: Before you start this lab, you should carefully read the chapter 5 of the textbook.

## Lab 5: Frames & Translation to Trees

Augment semant.c to allocate locations for local variables, and to keep track of the nesting level.

Try to keep all the machine-specific details in your machine-dependent Frame module (x86frame.c), not in Semant or Translate.

Design translate.h, implement translate.c, and rewrite the Semant structure to call upon Translate appropriately. The result of calling SEM_transProg should be a F_fragList.

Files related in Lab 5 include:
  • tiger.*   Use your own tiger.*

  • semant.c

  • frame.h

  • x86frame.c

  • translate.*

Notice: Before you start this lab, you should carefully read the chapter 6 and 7 of the textbook.

## Lab 6: Instruction Selection, Liveness Analysis, Register Allocation, and Put it all together!

In this lab, your goal is to make your compiler generate working code that runs on x86-64 platform.

The file runtime.c is a C-language file containing several external functions useful to your Tiger program. These are generally reached by externalCall from code generated by your compiler. You may modify this as necessary. Note: to avoid the size inconsistency between pointers and array elements, the size of an integer in this lab is 8 bytes, which is equal to the size of a pointer.

Write a module main.c that calls on all the other modules to produce an assembly language file prog.s for each input program prog.tig. This assembly language program should be assembled (producing prog.o) and linked with runtime.o to produce an executable file.

In your generated code, you should do what you can to avoid unnecessary stack accesses (push and pop), which means placing as many variables as possible in registers by escape analysis in register allocation phase. Otherwise, you will NOT get the full scores!

Notice: Before you start this lab, you should carefully read the chapter 8 to 12 of the textbook. 
