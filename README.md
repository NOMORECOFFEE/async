async
=====

 The framework for supporting a style of parallel programming
in which problems are solved by (recursively) splitting them into subtasks
that are solved in parallel, waiting for them to complete,
and then composing results.

 The fork/join framework provides a very straightforward and intuitive structure
to implement recursive and divide and conquer problems
that can be solved concurrently. You start with the big problem and break it down
until each smaller work unit can be solved directly.
