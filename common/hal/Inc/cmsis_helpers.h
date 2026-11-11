#define FILL_REG(REG, BITS, VAL)		\
{ 										\
	REG &= ~BITS;						\
	REG |=((VAL << BITS##_Pos) & BITS); \
}
