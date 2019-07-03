loadi  0 1     ! 257 line 0, R0 = fact(R1)   1-> r[0]
read   1       ! 45568 input R1 r[1] = 3
call   6       ! 41222 call, pc=3, sp=250;
load   0 33    ! 33 receive result of fact
write  0       ! 47104
halt            ! 49152
compri 1 1     ! 27393 line 6 3>1
jumpe  14      ! 37134 jump over the recursive call to fact if
jumpl  14      ! 35086 R1 is less than or equal 1
call   16      ! 41232 call mult (R0 = R0 * R1) pc=10, sp=244;
load   0 34    ! 34 receive result of mult
subi   1 1     ! 8961 decrement multiplier (R1) and multiply again
call   6       ! 41222 call fact
load   0 33    ! 33
store  0 33    ! 2337 line 14, return R0 (result of fact)
return          !43008
loadi  2 8     ! 1288 line 16, init R2 (counter)
loadi  3 0     ! 1792 init R3 (result of mult)
shr    1       ! 23040 line 18 (loop), shift right multiplier set CARRY
store  2 35    ! 3363 save counter
getstat 2      ! 29696 to find CARRY's value        
andi   2 1      !13569
compri 2 1      !27905
jumpe  25      ! 37145 if CARRY==1 add
jump   26      ! 33050 otherwise do nothing
add    3 0      !5632 
shl    0       ! 18432 make multiplicand ready for next add
load   2 35    ! 1059 restore counter
subi   2 1     ! 9473 decrement counter        
compri 2 0     ! 27904 if counter > 0 jump to loop
jumpg  18       !39186
store  3 34    ! 3874 return R3 (result of mult)
return          ! 43008
noop           ! line 33, fact return value
noop           ! line 34, mult return value
noop           ! line 35, mult counter
