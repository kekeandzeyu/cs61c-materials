.globl f # this allows other files to find the function f

# f takes in two arguments:
# a0 is the value we want to evaluate f at
# a1 is the address of the "output" array (read the lab spec for more information).
# The return value should be stored in a0
f:
    # Adjust the input value to get an index between 0 and 6
    addi t0, a0, 3     

    # Calculate the offset by multiplying the index by 4 (size of a word)
    slli t1, t0, 2        # t1 = t0 << 2

    # Calculate the address of the desired output
    add t2, a1, t1 

    # Load the value from the output array into a0
    lw a0, 0(t2)

    # This is how you return from a function. You'll learn more about this later.
    # This should be the last line in your program.
    jr ra
