.globl factorial

.data
n: .word 8

.text
# Don't worry about understanding the code in main
# You'll learn more about function calls in lecture soon
main:
    la t0, n
    lw a0, 0(t0)
    jal ra, factorial

    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result

    addi a1, x0, '\n'
    addi a0, x0, 11
    ecall # Print newline

    addi a0, x0, 10
    ecall # Exit

# factorial takes one argument:
# a0 contains the number which we want to compute the factorial of
# The return value should be stored in a0
factorial:
    # Initialize result t0 = 1
    addi t0, x0, 1 
    # Copy n from a0 to t1
    mv t1, a0      

loop:
    # If t1 == 0, exit loop
    beqz t1, end_factorial 
    # t0 = t0 * t1
    mul t0, t0, t1     
    # t1 -= 1
    addi t1, t1, -1    
    # Jump back to the beginning of the loop
    j loop                  

end_factorial:
    # Move result back to a0
    addi a0, t0, 0          
    # This is how you return from a function. You'll learn more about this later.
    # This should be the last line in your program.
    jr ra
