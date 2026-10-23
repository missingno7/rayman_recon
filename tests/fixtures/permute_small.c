/* Standalone parser fixture: real tests also exercise pinned preprocessing. */
typedef short s16;
typedef int s32;
extern int helper(int);
#pragma aux helper parm [eax] value [eax] modify exact [eax]
int neighbour(void) { return 7; }
s16 pilot(s16 x, s16 y)
{
    s16 a;
    s16 b;
    a = x + y;
    b = x - y;
    if (a > 3) {
        b += a;
    } else {
        b -= a;
    }
    return b;
}
int tail(void) { return 9; }
