.import utils.s

.text
main:
    # This program will fill an array of size 10 with 0's

    # Allocate an array of size 10
    li a0 40   # 10 ints, 4 bytes each
    jal malloc # malloc is defined in utils.s
    mv t0 a0   # the pointer is returned in a0
    # Alternate Implementation 2 (For freeing the array)\
    # Save the original array pointer at t3
    # mv t3 a0   

    # Fill the array with 0's
    li t1 0  # t1 is the index
    li t2 10 # t2 is the size of the array

loop:
    # Store 0 at the current index
    sw x0 0(t0)
    # Increment the index
    addi t1 t1 1
    # Increment the pointer
    addi t0 t0 4
    # Check if we are done
    # If not, loop
    blt t1 t2 loop
    
    addi t0, t0, -40    # t0 now points back to the head of the array
    
    # Alternate Implementation 2 (For freeing the array)
    # Calculate the original address of the array
    # li t3, 4          # Element size (4 bytes)
    # mul t4, t1, t3    # Total offset
    # sub t0, t0, t4    # t0 now points back to the head of the array

    # Free the allocated memory
    mv a0, t0 # For Alternate Implemetation 1, mv a0, t3
    jal free   
    
    # Exit the program
    li a0 0
    jal exit
