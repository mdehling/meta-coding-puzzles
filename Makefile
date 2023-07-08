SRCS= \
cafeteria.c \
director-of-photography-1.c \
director-of-photography-2.c \
hops.c \
kaitenzushi.c \
portals.c \
rabbit-hole-1.c \
rabbit-hole-2.c \
rotary-lock-1.c \
rotary-lock-2.c \
scoreboard-inference-1.c \
scoreboard-inference-2.c \
slippery-trip.c \
stack-stabilization-1.c \
stack-stabilization-2.c \
tunnel-time.c \
uniform-integers.c

PRGS= $(SRCS:.c=)


all: $(PRGS)

.c:
	gcc $< -o $@

clean:
	rm -f $(PRGS)
