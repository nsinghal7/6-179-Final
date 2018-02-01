# 6-179-Final
Extended Game of life with custom organisms
Each square can have any amount of each of the nutrients you specify. If there is enough of a nutrient, the organism best suited for that nutrient will be created. The organism takes some amount of its primary nutrient to be created and to survive each round. It also produces some amount of its primary production nutrient and a small amount of other nutrients every round. If the organism cannot get enough nutrients, it dies and its resources are returned to the map.

The organism has several attributes:
1) Organism type: equivalent to species.
2) Primary consumption nutrient: this organism's defining nutrient which it uses to be created and survive. It can also drain this nutrient out of nearby squares (like roots).
3) Secondary consumption nutrient: each organism can acquire a secondary consumption nutrient if there is another common nutrient besides its primary in its square when it is created. The organism can consume its secondary nutrient when it cannot get enough of its primary nutrient; however, it cannot drain it from nearby squares, only its own. This nutrient is not assigned by the user, but rather is determined as the most common other nutrient on the square. It is not shared by all organisms of the same type
4) Primary production nutrient: this organism's defining produce. Each turn it creates a certain amount of this nutrient (along with small amounts of others) and distributes it evenly among its tile and all neighbors.
5) creation cost: the cost (in primary consumption nutrient) of creating the organism
6) consumption amount: the amount of primary or secondary consumption nutrient needed to survive
7) drain amount: the amount of primary consumption nutrient drained from each neighboring square
8) primary production amount: the amount of primary production nutrient created each round
9) other production amount: the amount of all other nutrients created each round (per nutrient)

Notes:
1) When one or many  organisms tries to drain a neighbor, if that neighbor doesn't have enough to supply them all, it splits its nutrients proportionally by the amount requested.
2) When any amount is split, any remainder after the division is left in the central square (or original square) since nutrient amounts are integers. This is to avoid losing nutrients due to round off and avoid arbitrarily favoring geometries
3) An organism tries to consume as much as it can of its primary nutrient before draining. Then it tries to consume again before switching to its secondary nutrient
4) When organisms die, 1/4 of their creation cost goes back to primary consumption nutrient (rounded down), all nutrients consumed on the round of death go to secondary nutrient (or primary if there is no secondary), and 3/4 of the creation cost goes back to primary production nutrient (plus remainder). These nutrients are placed in the dead organism's square.
5) All row and column numbers are 0 indexed


Input file format is the same as input to std::cin:
[number of types of nutrients]
[name of nutrient] ...
[number of types of organisms]
(
	[name of organism]
	[primary consumption nutrient name]
	[primary production nutrient name]
	[creation cost]
	[drain amount]
	[consumption amount]
	[primary production amount]
	[other nutrient production amount per turn]
) ...
[number map rows] [number map columns]
[number of map squares with initial nutrients]
(
	[row of square in question] [column of square in question]
	[number of types of nutrients on square]
	([Nutrient name] [amount of nutrient (int)]) ...
) ...

The rest of the operation is then run from the command line
