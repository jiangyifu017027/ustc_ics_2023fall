int REMOVE(int, int);
int PUT(int, int);

int main()
{
    int n; // the value of n in x3100
    REMOVE(n, 0);
    return 0;
}
int REMOVE(int n, int state)
{
    /**
     * Arguments:
     *   1. The range of rings to be removed (1 to n rings).
     *   2. The current state of all rings.
     *
     * Returns:
     *   The state of all rings after the specified operations are performed.
     *
     * Notes:
     *   - The state of all rings should be stored at a specific memory location.
     *   - Ensure that the location is appropriate for storing the state information.
     */

    if (n == 0)
        return state; // the state remains
    if (n == 1)
    {
        // change the state of the first ring
        // return all rings' current state
    }
    // REMOVE the first n-2 rings
    // REMOVE the n-th ring
    // PUT the first n-2 rings
    // RMOVE the first n-1 rings

    // return all rings' current state
}
int PUT(int n, int state)
{
    /**
     * Arguments:
     *   1. The range of rings to be put (1 to n rings).
     *   2. The current state of all rings.
     *
     * Returns:
     *   The state of all rings after the specified operations are performed.
     *
     * Notes:
     *   - You just need to inverse REMOVE
     */
}
