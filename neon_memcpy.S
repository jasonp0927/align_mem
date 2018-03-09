.global neonMemCopy

neonMemCopy:
push {r0-r7,lr}
loop:
pld 		[r1,#0xC0]
vldm 		r1!,{q0-q7}
vstm 		r0!,{q0-q7}
subs 		r2,r2,#0x80
bge 		loop
#mov		pc,lr
#bx          lr
pop {r0-r7,pc}
