# 21726914
# Kayla Aky�z
# BBM234 Project 1
# empty line
.data # This indicated we are going to be declaring some data.
A: .word 0, 1, 80, 3, 160, 5, 240, 7, 320, 800 # Main array data that represents K�lyutmaz's encrypted data
size: .word 10 # Size of the array given above
num_students: .word 5 # Student number in current data
printText1: .asciiz "\nTestcase:\n---------\nnum_students = " # All of these are string data which will help me print out according output.
printText2: .asciiz "\ndatasize = " # All of these are string data which will help me print out according output.
printText3: .asciiz "\ndata[] = {" # All of these are string data which will help me print out according output.
printText4: .asciiz ", " # All of these are string data which will help me print out according output.
printText5: .asciiz "}\n" # All of these are string data which will help me print out according output.
printText6: .asciiz "\nExpected output for testcase:\n-----------------------------\ndata[] = {" # All of these are string data which will help me print out according output.
printText7: .asciiz "The average similarity score is: " # All of these are string data which will help me print out according output.
printText8: .asciiz "\nBonus: Cheaters IDs: " # All of these are string data which will help me print out according output.
printText9: .asciiz "\nThe cheaters are " # All of these are string data which will help me print out according output.
printText10: .asciiz " and " # All of these are string data which will help me print out according output.
printText11: .asciiz "\n" # All of these are string data which will help me print out according output.
printText12: .asciiz "Inek Saban" # All of these are string data which will help me print out according output.
printText13: .asciiz "G�d�k Necmi" # All of these are string data which will help me print out according output.
printText14: .asciiz "Damat Ferit" # All of these are string data which will help me print out according output.
printText15: .asciiz "Domdom Ali" # All of these are string data which will help me print out according output.
printText16: .asciiz "Tulum Hayri" # All of these are string data which will help me print out according output.
printText17: .asciiz "Hayta Ismail" # All of these are string data which will help me print out according output.
printText18: .asciiz "Unknown ID" # All of these are string data which will help me print out according output.
.text # Now we are starting code so we state that
.globl main # Main can be accessed from outside program (file)
	main: # We are starting main function
		la $s0, A # Load the address of A[0] to register s0
		lw $s1, size # Load value of size to s1
		lw $s2, num_students # Load value of num_students to s2
		jal initPrint # Jumping to initPrint, using jal so ra is set to here when we return ra we return here
		move $a0, $s0 # Inserting arguments for forLoopPart1, it takes address of main database because it will write over it
		move $a1, $s1 # Second argument is size, it takes because when iterating it is easier if size is known
		jal forLoopPart1 # Jumping to function, with jal again meaning setting ra so we can easly return here
		move $a0, $s0 # Inserting arguments for resultPrintPart1 function, it takes 4 arguments, this one is adress because it will use it printing changed result
		la $a1, printText6 # Sending address of printText6 to function, this is different than initPrint. This way that function only uses arguments
		la $a2, printText5 # Sending address of printText5 to function, this is different than initPrint. This way that function only uses arguments
		move $a3, $s1 # Size is also passed as argument because it will be used when iterating array
		jal resultPrintPart1 # Going to function with jal because we use ra to return back here
		move $a0, $s0 # Passing arguments to averageLoopPart2, two arguments passed, data and number n, this is according to assignment paper. That function only uses this arguments and is recursive
		move $a1, $s1 # number n aka size passed as second argument
		jal averageLoopPart2 # Going to averageLoopPart2 function, with jal, this is like function call in C, we will return with ra
		move $a0, $v1 # Return value of averageLoopPart2 is stored in v1, so we retrive it and immediatly retrive it to argument of next function. Next function is result printing function for part 2 so it will use this result
		la $a1, printText7 # printText7 also passed to resultPrintPart2 this is to ensure that function only uses arguments. I don't know if this is a must but it seems like more standard
		jal resultPrintPart2 # Going to resultprintPart2 with jal we will use ra to return back here 
		mul $s5, $s2, 4 # Now we are allocating enough space to store an array of size students. Later on inside other functions this array will be used to determine if students cheated by setting values to one
		add $sp, $sp, $s5 # s5 is storing size for (4*student number) our array, we add to stack pointer so according space is allocated. Why multiply by 4 because lw sw need it to be alligned with word (4)
		move $a1, $s1 # Passing arguments to getCheaters function, it takes 4 arguments, with a1 size of main data array is passed
		move $a2, $s2 # with a2 we are passing student numbers, it is used when doing calculations with upper triangular matrix
		move $a3, $s0 # Main array is passed as argument to getCheaters function.
		li $v0, 4 # Now here happens a print cheat I used a lot used in this project. The v0 set to 4 meaning we will print string (if passed to syscall) how ever at the end as you can see syscall is not called. No worries it will be called when time comes. If you look deeper you will see this is to ensure " and ", "," to output format. Also don't worry this values will not be edited until then
		la $a0, printText8 # This argument is passed down to function, it will direct be passed to syscall
		jal getCheaters # going to getCheaters function with jal, we will use ra when coming back here, ra points here
		jal printCheaters # there is no argument to set or return value to evaluate so we directly go to next function. Next function will use stack to get values and print accordingly
		sub $sp, $sp, $s5 # deallocating stack
		li $s5, 0 # setting these values 0 since it is used here but they are not protected, this way it looks cleaner
		li $a0, 0 # setting these values 0 since it is used here but they are not protected, this way it looks cleaner
		j done # going to done, not with jal cuz we don't need ra to return here, we just go, we won't return
# empty line
	initPrint: # init print function is here because it looks like it uses data so it looks better according this way
		addi $sp, $sp, -4 # make room on stack to save $ra. ra will be overwritten down below so we save it
		sw $ra, 0($sp) # saving ra
		li $v0, 4 # This line is systemcall number 4 indicates print string
		la $a0, printText1 # getting the string which will printed to a0, it is where systemcall number 4 reads to print
		syscall # syscall calls system with system call number in v0 and performs according function, see references section in report for system calls
		li $v0, 1 # This system call number 1 is to print words
		lw $a0, num_students # passing value to print to a0 this is where system call number 1 will look for value to print
		syscall # syscall calls system with system call number in v0 and performs according function, see references section in report for system calls
		li $v0, 4 # This line is systemcall number 4 indicates print string
		la $a0, printText2 # getting the string which will printed to a0, it is where systemcall number 4 reads to print
		syscall # syscall calls system with system call number in v0 and performs according function, see references section in report for system calls
		li $v0, 1 # This system call number 1 is to print words
		lw $a0, size # passing value to print to a0 this is where system call number 1 will look for value to print
		syscall # syscall calls system with system call number in v0 and performs according function, see references section in report for system calls
		li $v0, 4 # This line is systemcall number 4 indicates print string
		la $a0, printText3 # getting the string which will printed to a0, it is where systemcall number 4 reads to print. There is no systemcall below but don't worry, this is simply passed to next function which has systemcall accordingly. This is as well to print more beautiful output
		move $a1, $s0 # printArrayLoop function takes four arguments first one is main array data which it will print
		move $a2, $s1 # second one is the size of it so easier print will be done
		la $a3, printText4 # this is also passed it will be used when printing
		jal printArrayLoop # going to printArrayLoop with jal because ra will be pointing back here which we will use to return. printArrayLoop is enclosed as much so this represtents function call
		li $v0, 4 # This is reduntant since it is already 4 from previous print but just to be sure we set again
		la $a0, printText5 # getting the string which will printed to a0, it is where systemcall number 4 reads to print
		syscall # syscall calls system with system call number in v0 and performs according function, see references section in report for system calls
		lw $ra, 0($sp) # restore $ra
		addi $sp, $sp, 4 # deallocate stack space
		jr $ra # jumping back to stored ra which is where we left, thanks to jal operation this is stored
# empty line
	printCheaters: # printCheaters function which reads the stack (which was edited before, there are words of student number, they are 1 if they are cheated, so we check all of it and see if they are cheated with if else statements we print according result
		li $v0, 4 # This line is systemcall number 4 indicates print string
		la $a0, printText9 # Getting printText9 to print. Since this is used printCheaters is not like seperate function.
		li $t0, 0 # t0 is set to 0, we are using this to trace currently readen student number from stack
		pcLoopStart: # for loop we are reading stack, word by word, if there is a word at index i with value 1 that index id student cheated
			mul $t2, $t0, 4 # multiplying with 4, index is turned in to address
			add $t2, $sp, $t2 # adding to stack pointer to access that addreess
			lw $t3, 0($t2) # accessing address and loading word to t3
			beq $t3, 1, pcFound # if the value equals to 1 we jump to branch pcFound
			add $t0, $t0, 1 # going to next index, student id
			bge $t0, $s2, pcReturn # if index equals to student number, meaning we are at the end of our stack array we go to returning part of this function
			j pcLoopStart # jumping to looping
		pcReturn: # returning from this function
			li $t0, 0 # this temporary values are used and setted to 0 at the end just to be sure, there is no problems in future
			li $t2, 0 # this temporary values are used and setted to 0 at the end just to be sure, there is no problems in future
			li $t3, 0 # this temporary values are used and setted to 0 at the end just to be sure, there is no problems in future
			jr $ra # since we came here with jal we use this to go back to where we left
		pcFound: # Cheater is found, with if and else we print according output
			syscall # first the remaining print from above is printed
			beq $t0, 0, printInek # matching student id with according print
			beq $t0, 1, printGuduk # matching student id with according print
			beq $t0, 2, printDamat # matching student id with according print
			beq $t0, 3, printDomdom # matching student id with according print
			beq $t0, 4, printTulum # matching student id with according print
			beq $t0, 5, printHayta # matching student id with according print
			j printUnkown # matching student id with according print. If there is no matched id we will print Unknown. Since teacher gave until id 6 anything bigger is unknown
		printInek: # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			li $v0, 4 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			la $a0, printText12 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			syscall # Printing accordng student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			la $a0, printText10 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			add $t0, $t0, 1 # index increased by 1
			j pcLoopStart # and we continue loop
		printGuduk: # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			li $v0, 4 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			la $a0, printText13 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			syscall # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			la $a0, printText10 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			add $t0, $t0, 1 # index increased by 1
			j pcLoopStart # and we continue loop
		printDamat: # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			li $v0, 4 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			la $a0, printText14 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			syscall # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			la $a0, printText10 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			add $t0, $t0, 1 # index increased by 1
			j pcLoopStart # and we continue loop
		printDomdom: # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			li $v0, 4 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			la $a0, printText15 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			syscall # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			la $a0, printText10 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			add $t0, $t0, 1 # index increased by 1
			j pcLoopStart # and we continue loop
		printTulum: # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			li $v0, 4 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			la $a0, printText16 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			syscall # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			la $a0, printText10 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			add $t0, $t0, 1 # index increased by 1
			j pcLoopStart # and we continue loop
		printHayta: # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			li $v0, 4 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			la $a0, printText17 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			syscall # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			la $a0, printText10 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			add $t0, $t0, 1 # index increased by 1
			j pcLoopStart # and we continue loop
		printUnkown: # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			li $v0, 4 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			la $a0, printText18 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			syscall # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			la $a0, printText10 # Printing according student name, this is same and as I explaned lots of times above. systemcall number is set a0 is populated and syscall
			add $t0, $t0, 1 # index increased by 1
			j pcLoopStart # and we continue loop
	done: # end of main function
		li $v0, 10 # syscall 10 is for ending exit terminate execution
		syscall # called syscall 10
# empty line
printArrayLoop: # function for printing array, it iterates stored array data
	addiu $sp, $sp, -16 # make room on stack to save $ra and s0-s2
	sw $s0, 0($sp) # saving
	sw $s1, 4($sp) # saving
	sw $s2, 8($sp) # saving
	sw $ra, 12($sp) # saving
	move $s0, $a0 # reading arguments
	move $s1, $a1 # reading arguments
	move $s2, $a2 # reading arguments
	li $t3, 0 # we will use these so we set 0 just to be sure
	li $t4, 0 # we will use these so we set 0 just to be sure
	paLoopStart: # for loop we are iterating the function in
	syscall # why this is here; this is a smart movement to make sure commas are only in between numbers. At the beginnig it prints the print above. After every loop it prints a comma, at the end the comma in a0 wont be printed since this wont be called. This method used lots in this project to ensure output match
	add $t4, $t3, $t3 # multplying by 4 to get index to adress
	add $t4, $t4, $t4 # multplying by 4 to get index to adress
	add $t4, $t4, $s1 # adding to array to getting acutal adress
	li $v0, 1 # this is printing word systemcall number
	lw $a0, 0($t4) # reading data from adress
	syscall # syscalling to print
	li $v0, 4 # syscall 4 is for printing string
	move $a0, $a3 # comma is put, in the next iteration it will be printed or it wont if there is no next iteration
	addi $t3, $t3, 1 # next index 
	bne $s2,$t3, paLoopStart # if next index is not equal to max index, which stored in s2 which is array size, we loop
	li $t3, 0 # setting these 0 just to be sure, they are temporary anyways
	li $t4, 0 # setting these 0 just to be sure, they are temporary anyways
	li $a0, 0 # setting these 0 just to be sure, they are temporary anyways
	li $a1, 0 # setting these 0 just to be sure, they are temporary anyways
	li $a2, 0 # setting these 0 just to be sure, they are temporary anyways
	li $a3, 0 # setting these 0 just to be sure, they are temporary anyways
	lw $s0, 0($sp) # restoring values we stored in stack
	lw $s1, 4($sp) # restoring values we stored in stack
	lw $s2, 8($sp) # restoring values we stored in stack
	lw $ra, 12($sp) # restoring values we stored in stack the ra valu was stored now we can use it to ho back where we left from this function as return
	addiu $sp, $sp, 16 # deallocate stack space
	jr $ra # returning
# empty line
forLoopPart1: # for loop for part 1, this function has the for loop,
	addiu $sp, $sp, -16 # make room on stack to save $ra and stack
	sw $s0, 0($sp) # saving
	sw $s1, 4($sp) # saving
	sw $s2, 8($sp) # saving
	sw $ra, 12($sp) # saving ra we will use it to go back return
	move $s0, $a0 # reading arguments
	move $s1, $a1 # reading arguments
	li $t3, 0 # we will use these so we set 0 just to be sure
	li $t4, 0 # we will use these so we set 0 just to be sure
	li $t5, 0 # we will use these so we set 0 just to be sure
	li $t6, 0 # we will use these so we set 0 just to be sure
	flp1LoopStart: # the for loop
	add $t4, $t3, $t3 # multiplying by 4 to getting adress from i, this is done by adding and adding again
	add $t4, $t4, $t4 # multiplying by 4 to getting adress from i, this is done by adding and adding again
	add $t4, $t4, $s0 # adding result to array address to getting actual address
	lw $t5, 0($t4) # reading data from array to t5
	srl $t6, $t5, 1 # shift *original val* right by 1 this is done for checking if it is odd or not, if the value changes we know it is Odd
	sll $t6, $t6, 1 # shift *modified val* left by 1
	bne $t6, $t5, isOdd # Now we know if the value changed so it must be odd
	li $t6, 0 # t6 will be used again but it is not 0 we set it to 0
	j isEven # if not odd we jump to even
	isOdd: # if odd
		add $t6, $t5, $t5 # we multiply by 5
		add $t6, $t6, $t6 # it is done by adding it with itself and again and adding it to the result once more, like *4+1
		add $t6, $t6, $t5 # adding once more
		j doneDecryption # jumping to returning part of function
	isEven: # if even
		blt $t5, 8, lastDivisionStep # division done by subtracting until we get a value smaller then 8, then we check if it smaller than 4 if not it is .5 so we can add 1
		sub $t5, $t5, 8 # subtracting 8s
		addi $t6, $t6, 1 # increament to quotient
		j isEven # continuing the division loop
		lastDivisionStep: # at last we check if bigger then 4 it means .5 so we add 1 more. This gives us more precision
		blt $t5, 4, doneDecryption # checking if it is less then 4
		addi $t6, $t6, 1 # adding 1 more
		j doneDecryption # going to return part of this function
	doneDecryption: # we are writing the decrypted data back to main array
		sw $t6, 0($t4) # calculated data is written back
	addi $t3, $t3, 1 # Increament to loop counter
	bne $s1,$t3, flp1LoopStart # If ($t3 != $s1) Branch to loop, s1 is array size, t3 is current i
	li $t3, 0 # setting these 0 just to be sure, they are temporary anyways
	li $t4, 0 # setting these 0 just to be sure, they are temporary anyways
	li $t5, 0 # setting these 0 just to be sure, they are temporary anyways
	li $t6, 0 # setting these 0 just to be sure, they are temporary anyways
	li $a0, 0 # setting these 0 just to be sure, they are temporary anyways
	li $a1, 0 # setting these 0 just to be sure, they are temporary anyways
	lw $s0, 0($sp) # restoring values we stored in stack
	lw $s1, 4($sp) # restoring values we stored in stack
	lw $s2, 8($sp) # restoring values we stored in stack
	lw $ra, 12($sp) # restoring values we stored in stack the ra valu was stored now we can use it to ho back where we left from this function as return
	addiu $sp, $sp, 16 # deallocate stack space
	jr $ra # returning
# empty line
resultPrintPart1: # function to printing result of part 1
	addiu $sp, $sp, -16 # make room on stack to save $ra
	sw $s0, 0($sp) # saving
	sw $s1, 4($sp) # saving
	sw $s2, 8($sp) # saving
	sw $ra, 12($sp) # saving
	move $s0, $a0 # reading arguments
	move $s1, $a1 # reading arguments
	move $s2, $a2 # reading arguments
	move $s3, $a3 # reading arguments
	li $v0, 4 # syscall 4 is printing string, readen from v0
	move $a0, $s1 # setting up arguments to printArrayLoop function
	move $a1, $s0 # setting up arguments to printArrayLoop function
	move $a2, $s3 # setting up arguments to printArrayLoop function
	la $a3, printText4 # setting up arguments to printArrayLoop function
	jal printArrayLoop # going to printArrayLoop
	li $v0, 4  # syscall 4 is printing string, readen from v0
	la $a0, printText5 # printText5 is loaded to be passed to syscall
	syscall # syscalling to print
	li $a0, 0 # setting these 0 just to be sure, they are temporary anyways
	li $a1, 0 # setting these 0 just to be sure, they are temporary anyways
	li $a2, 0 # setting these 0 just to be sure, they are temporary anyways
	li $a3, 0 # setting these 0 just to be sure, they are temporary anyways
	lw $s0, 0($sp) # restoring values we stored in stack
	lw $s1, 4($sp) # restoring values we stored in stack
	lw $s2, 8($sp) # restoring values we stored in stack
	lw $ra, 12($sp) # restoring values we stored in stack the ra valu was stored now we can use it to ho back where we left from this function as return
	addiu $sp, $sp, 16 # deallocate stack space
	jr $ra # returning
# empty line
averageLoopPart2: # this is the recursive function used for part 2, it has two arguments everything as it is in C code in assignment paper
	addiu $sp, $sp, -16 # make room on stack to save $ra and s0-s2
	sw $s0, 0($sp) # saving
	sw $s1, 4($sp) # saving
	sw $s2, 8($sp) # saving
	sw $ra, 12($sp) # saving ra returning
	move $s0, $a0 # reading arguments first one is data
	move $s1, $a1 # reading arguments second is n
	bne $s1, 1, notEqual # if n is not 1 we jump
	lw $s2, 0($s0) # if n is 1 we return first argument of array
	j return # going to return of this function
	notEqual: # if not equal we will dive deep in to recursion here
		move $a0, $s0 # setting up arguments for recursive call data
		subi $a1, $s1, 1 # setting up arguments for recursive call n-1
		jal averageLoopPart2 # recursive function call
		li $t0, 0 # these are used and they are temporary so we set to 0 just to be sure because we will be use them right now
		li $t1, 0 # these are used and they are temporary so we set to 0 just to be sure because we will be use them right now
		li $t2, 0 # these are used and they are temporary so we set to 0 just to be sure because we will be use them right now
		move $t0, $v1 # return value of recursive function is returned with v1, it is readen here
		li $v1, 0 # setting return value v1 to 0 it will be overridden when we return from this function anyways
		move $t1, $s1 # s1 is n 
		subi $t1, $t1, 1 # we decrease n by 1 n-1
		multipLoop: # now we will loop n-1 times and add result of recursive call, in paper it says we are allowed to use mul but I had already coded this with thinking it isn't so it reaming anyways
			subi $t1, $t1, 1 # decreasing our loop
			add $t2, $t2, $t0 # adding
			bgtz $t1, multipLoop # if not 0 we loop
		subi $t1, $s1, 1 # again we get n-1 here cuz we will get data[n-1]
		add $t1, $t1, $t1 # converting the n-1 to address (word) value by multiplying with 4. Again mul is allowed but I had already coded this so it remains
		add $t1, $t1, $t1 # converting the n-1 to address (word) value by multiplying with 4. Again mul is allowed but I had already coded this so it remains
		add $t1, $t1, $s0 # adding to data array's pointer to getting actual value
		lw $t1, 0($t1) # reading the data
		add $s2, $t2, $t1 # adding it
		j return # going to return
	return: # returning part of recursive function
		sub $s2, $s2, $s1 # division is done again without using div. As I said before I coded this way thinking div mul is not allowed apperantly it is but this remains
		addi $v1, $v1, 1 # increament to quotient
		bge $s2, $s1, return # looping division
		li $t0, 0 # setting t0 to 0
		getHalfDividor: # for precision we get half of divider and comparing remainder to it. If remainder is greater it is .5 so we add 1 to quotient. However after I was testing I realise TA didn't use this precision so I removed
			subi $s1, $s1, 2 # diving the divider by 2
			addi $t0, $t0, 1 # result
			bgt $s1, 2, getHalfDividor # comparing
		blt $s2, $t0, moveOn # going to moveOn part
		# addi $v1, $v1, 1 This line is commented in order to match TA's output. This line rounds up .5 to integer up. TA didn't want precision.
		j moveOn # going to moveOn part
	moveOn: # last part of this recursive function we are finaly returning by setting values we used to back as it was
	li $t0, 0 # these are not protected values they are used within so we changed them just to be sure setting back to 0
	li $t1, 0 # these are not protected values they are used within so we changed them just to be sure setting back to 0
	li $t2, 0 # these are not protected values they are used within so we changed them just to be sure setting back to 0
	li $a0, 0 # these are not protected values they are used within so we changed them just to be sure setting back to 0
	li $a1, 0 # these are not protected values they are used within so we changed them just to be sure setting back to 0
	lw $s0, 0($sp) # these were protected values we stored in stack. We are getting them back
	lw $s1, 4($sp) # these were protected values we stored in stack. We are getting them back
	lw $s2, 8($sp) # these were protected values we stored in stack. We are getting them back
	lw $ra, 12($sp) # these were protected values we stored in stack. We are getting them back. ra will be used to jump back
	addiu $sp, $sp, 16 # deallocate stack space
	jr $ra # jumping back where we left, most probably upper scope of recursion
# empty line
resultPrintPart2: # Printing results of part 2
	addiu $sp, $sp, -16 # make room on stack to save $ra
	sw $s0, 0($sp) # saving
	sw $s1, 4($sp) # saving
	sw $s2, 8($sp) # saving
	sw $ra, 12($sp) # saving
	move $s0, $a0 # reading arguments
	move $s1, $a1 # reading arguments
	li $v0, 4 # syscall number 4 is for printing string
	move $a0, $s1 # getting the value we will print to a0
	syscall # syscall to print
	li $v0, 1 # syscall number 4 is for printing word
	move $a0, $s0 # getting the value we will print to a0
	syscall # syscall to print
	li $a0, 0 # setting these 0 just to be sure, they are temporary anyways
	li $a1, 0 # setting these 0 just to be sure, they are temporary anyways
	lw $s0, 0($sp) # restoring values we stored in stack
	lw $s1, 4($sp) # restoring values we stored in stack
	lw $s2, 8($sp) # restoring values we stored in stack
	lw $ra, 12($sp) # restoring values we stored in stack
	addiu $sp, $sp, 16 # deallocate stack space
	jr $ra # returning
# empty line
getCheaters: # Bonus part function, we traverse 1D array as if it is a upper triangular matrix and if greater then 80 we set the array in stack accordingly id to 1
	move $t0, $sp # storing current stack pointer, it points to an array we are using to storing 1-0 for ids 1 for cheater will be edited
	addiu $sp, $sp, -16 # make room on stack to save $ra
	sw $s0, 0($sp) # saving
	sw $s1, 4($sp) # saving
	sw $s2, 8($sp) # saving
	sw $ra, 12($sp) # saving
	move $s0, $a3 # reading arguments
	move $s1, $a1 # reading arguments
	move $s2, $a2 # reading arguments
	li $t1, 0 # we will use these so we set 0 just to be sure
	li $t2, 0 # we will use these so we set 0 just to be sure
	li $t3, 0 # we will use these so we set 0 just to be sure
	li $t4, 0 # we will use these so we set 0 just to be sure
	li $t5, 1 # we will use these so we set 1 because it is the row number starts from 1
	li $t6, 0 # we will use these so we set 0 just to be sure
	li $t7, 0 # we will use these so we set 0 just to be sure
	gcLoopStart: # row loop, this increases when row increases
	sub $t6, $s2, $t5 # row number is subtracted from student number getting length of current triangle step 
	gcInnerLooP: # inner loop is for columns
		add $t4, $t3, $t3 # converting index id to adress aka multiplying by 4, I use this way cuz I thought mul was not allowed in bonus but it might be allowed on part 3
		add $t4, $t4, $t4 # converting index id to adress aka multiplying by 4, I use this way cuz I thought mul was not allowed in bonus but it might be allowed on part 3
		add $t4, $t4, $s0 # adding to array to getting actual address
		lw $t1, 0($t4) # reading value
		blt $t1, 80, notCheater # if value is less than 80 they are not cheater so we skip
			syscall # printing the last call, this is used to ensuring and and comma is printed accordingly. No worries, if you trace you will see there is no problem
			move $s6, $t5 # row number equals to first cheater which is stored to s6
			sub $t7, $s2, $t6 # from student number we reduce column number, column number is right aligned but ids are left we are currently trying to convert this data
			addi $s7, $t7, 1 # adding 1 cuz getting index and it equals to our second cheater id so we store to s7
			subi $t2, $s6, 1 # getting 1 less of id
			mul $t2, $t2, 4 # multiplying by 4 to getting address
			add $t2, $t0, $t2 # adding address of previous stack pointer we stored. We are accessing to an array at stack we created before
			li $t7, 1 # t7 set to 1 it will be simply written to found cheaters id in array
			sw $t7, 0($t2) # storing word, value data, 1 to cheater in array
			subi $t2, $s7, 1 # getting 1 less of second id
			mul $t2, $t2, 4 # multiplying by 4 to getting address
			add $t2, $t0, $t2 # adding address of previous stack pointer we stored. We are accessing to an array at stack we created before
			li $t7, 1 # t7 set to 1 it will be simply written to found cheaters id in array
			sw $t7, 0($t2) # storing word, value data, 1 to cheater in array
			li $v0, 1 # for syscall word will be printed so syscall number 1 set to v0
			move $a0, $s6 # setting s6 id of first cheater to a0 to print
			syscall # syscall to print
			li $v0, 4 # for syscall string will be printed so syscall number 4 set to v0
			la $a0, printText10 # getting printText10 to printing
			syscall # syscall to print
			li $v0, 1 # for syscall word will be printed so syscall number 1 set to v0
			move $a0, $s7 # setting s7 id of second cheater to a0 to print
			syscall # syscall to print
			li $v0, 4 # for syscall string will be printed so syscall number 4 set to v0
			la $a0, printText4 # getting printText4 to printing
		notCheater: # if not cheater we just continue
		addi $t3, $t3, 1 # adding 1 to our current 1D array tracing index
		subi $t6, $t6, 1 # column id is reduced by 1, column index is right aligned, cuz upper triangular matrix rows are right aligned
		bgt $t6, 0, gcInnerLooP # if current collumn index is greater then 0 we continue the inner loop which mover right on row
	addi $t5, $t5, 1 # increament to row index counter, we are moving down in matrix
	bne $s1,$t3, gcLoopStart # comparing the current inner matrix index to size of the array, if it equals means we are at the end of 1D array, also meaning end of matrix too so we end the loop. If not equals we loop
	li $t0, 0 # setting these 0 just to be sure, they are temporary anyways
	li $t1, 0 # setting these 0 just to be sure, they are temporary anyways
	li $t2, 0 # setting these 0 just to be sure, they are temporary anyways
	li $t3, 0 # setting these 0 just to be sure, they are temporary anyways
	li $t4, 0 # setting these 0 just to be sure, they are temporary anyways
	li $t5, 0 # setting these 0 just to be sure, they are temporary anyways
	li $t6, 0 # setting these 0 just to be sure, they are temporary anyways
	li $t7, 0 # setting these 0 just to be sure, they are temporary anyways
	li $a0, 0 # setting these 0 just to be sure, they are temporary anyways
	li $a1, 0 # setting these 0 just to be sure, they are temporary anyways
	li $a2, 0 # setting these 0 just to be sure, they are temporary anyways
	li $a3, 0 # setting these 0 just to be sure, they are temporary anyways
	lw $s0, 0($sp) # restoring values we stored in stack
	lw $s1, 4($sp) # restoring values we stored in stack
	lw $s2, 8($sp) # restoring values we stored in stack
	lw $ra, 12($sp) # restoring values we stored in stack
	addiu $sp, $sp, 16 # deallocate stack space
	jr $ra # returning
