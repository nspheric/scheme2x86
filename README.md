### x86 compiler
this a compiler for a small subset of a scheme like language. it generates runnable x86 assembly for some expressions.

it can be improved in several ways, namely:
- a lexer and parser similar to the one in the dragon book, second edition.
- the anf needs to be more comprehensive for a scheme language. I didnt get right in the codebase.
- I can use the visitor patten to transform ASTs/DAGs
- I can use a DAG of blocks, which are also DAGs, for the backend
- the way i tested the code should have been different, namely, for each ast node test its corresponding x86 code instead of testing chunk of code.
- And i can do optimizations
this compiler was a nice project to give me a feeling of how to write a compiler in c++.

### running the code
```bash
$ make main
$ make test
```
