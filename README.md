# Logical Inference

Final project for CS3 by Kevin Fang.

A logical inference machine that runs on Aristotelian logic. Run `make`, and then run `logical` for a logical inference system. Make sure that you use the exact spelling when making assertions and queries. The system is case sensitive and will check for contradictions before asserting.

Read more: https://plato.stanford.edu/entries/aristotle-logic/

## Examples
```
The classical example:
> ASSERT ALL Socrates are Human
> ASSERT ALL Human are Animal
> QUERY ALL Socrates are Animal
True
---
> ASSERT ALL Apple are Fruit
> ASSERT ALL Banana are Fruit
> QUERY ALL Apple are Banana
False
> ASSERT NO Apple are Banana
False (system doesn't know enough info)
> QUERY SOME Apple are Banana
False
---
> ASSERT SOME Humans are Mammals
> QUERY SOME Mammals are Humans
True
---
Example of a contradiction
> ASSERT ALL Socrates are Human
> ASSERT ALL Human are Animal
> ASSERT NO Animal are Socrates
Presents a contradiction.
> ASSERT NOTSOME Socrates are Human
Presents a contradiction.
```
