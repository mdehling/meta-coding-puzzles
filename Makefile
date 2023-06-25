SRCS= \
cafeteria.c \
director-of-photography-1.c \
director-of-photography-2.c \
rotary-lock-1.c \
rotary-lock-2.c \
stack-stabilization-1.c \
stack-stabilization-2.c \
uniform-integers.c

PRGS= $(SRCS:.c=)


all: $(PRGS)

.c:
	gcc $< -o $@

clean:
	rm -f $(PRGS)
