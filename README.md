### x86 compiler
this initial compiler version is based on chapers 2, 4, 5 in [Dr. Siek's book](https://github.com/IUCompilerCourse/Essentials-of-Compilation).

but i think i will just lower the core of scheme to x86.

I want to apply design patterns and improved my object oriented skills and
to make as good of a compiler backend as i can in an object oriented language.

Mainly, i will look at the llvm codebase and refer to the dragon book (2nd) edition.

The thing is that my compilers so far just use trees instead of DAGs. and
the x86 instructions ast is just a list of instructions. i almost got this
right but not quite.

the low level ir does consist of list of instructions but these are
broken into blocks.

according to the dragon book second edition, the following statements are true.

first, dags are more efficient since you dont reallocate existing nodes.

and secondly, dags are more useful for optimizations.

and theres a proper way to do this. theres a proper way to do intruction selection
and the low level ir needs to be a dag of dags (blocks).

need to apply all this to my scheme compiler in haskell as well. 

### running the code
```bash
$ make main
$ make test
```
