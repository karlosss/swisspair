# Swisspair

A swiss pairing algorithm for (not only) Magic: The Gathering.

## Installation

This project depends on https://github.com/karlosss/gmpwrap and https://github.com/karlosss/Minimum-Cost-Perfect-Matching.

It also needs `gcc`, `make`, `cmake` and `gmp` to be installed in your system.

Then, the library is build with `cmake . && make`.

## Algorithm description

From the product point of view, the algorithm is doing the following:
- Players who already played each other will not play each other again
- A player who already had a bye will not get another bye
- Players will play opponents of as equal match points as possible
  - If this is not possible, the algorithm will prefer downpairs of players at lower standings to players at higher standings
- Optionally, the algorithm can prefer to pair the top 8 players against each other
  - This is important for draws into the top 8
 
## Implementation

Players are separated into pods where each pod holds all players with the same amount of points. Then, a graph is built, where vertices represent
players and edges between two players represent the fact that those two players could theoretically play against each other (they did not play against each other yet).
The edges have weights assigned: an edge between two players from within the same pod always has weight = 0, and edges between players from different pods have a cost,
describing how bad this downpair would be (so that we will likely not pair the best player in the tournament against the worst). Details of the cost computation are below.
Then, the pairings are the min-cost maximum matching of this graph.

### Cost computation

Let `C(x, y)` be the cost of pairing a player from the `x`th highest pod with a player from the `y`th highest pod (note that `C(x,y) = C(y,x)` for all `x,y`, so we will only consider `x <= y`).

Pairing players with equal number of points is the best case, therefore `C(x, x) = 0` for all `x`.

Now, imagine 4 pods. To ensure integrity of the tournament, we would rather downpair all players in all other pods with players from the lowest pod, rather than downpairing even a single player from the first pod with a player from the second pod. This brings the following
hierarchy of costs:

`C(3,4) < C(2,3) < C(2,4) < C(1,2) < C(1,3) < C(1,4)`

Taking into account that we rather apply a lower cost as many times as possible rather than applying a higher cost even once, we define the cost `C(x,y)` as follows: Let `l` be the last pod and `|p|` the number of players in pod `p`, 
then 
- `C(x,y) = max(|x+1|, |l|) * C(x+1, l) + max(|x+2|, |l|) * C(x+2, l) + ... + max(|l|, |l|) * C(l, l) + 1` in case `x+1=y`, and
- `C(x,y) = max(|x|, |y-1|) * C(x, y-1) + max(|x+1|, |l|) * C(x+1, l) + max(|x+2|, |l|) * C(x+2, l) + ... + max(|l|, |l|) * C(l, l) + 1` otherwise.

In words, the cost will be equal to the most expensive possible pairing of all players in lower pods (or also in the same pod when we are computing the cost of downpairs across multiple pods) plus one, so that we achieve the desired property.

This cost computation is sound, however the absolute value of the cost grows pretty quickly. That is why we need unlimited precision floating point numbers, as standard doubles would become inaccurate even for a pretty low number of players.

### Byes

If the number of players is odd, the algorithm creates a phantom player - bye. This player always resides alone in the lowest pod and can play anyone who did not have a bye yet.

### Power pairing

Each of the top 8 players are in their own pods, and the rest of the field is separated into pods as usual. The "field pods" follow the top 8 pods in the hierarchy.

## Acknowledgements

- **Jari Rentsch** ([@Tiray7](https://www.github.com/Tiray7)) for helping with the product specification
- **Dilson Lucas Pereira** ([@dilsonpereira](https://www.github.com/dilsonpereira)) for publishing an algorithm to solve the min-cost maximum matching problem and helping with using unlimited floats instead of doubles

## License

MIT, whatever it means. If you like this project, I would be happy for a star :)
