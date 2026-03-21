NAME = ft_ping
INCLUDE_DIR = inc
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDE_DIR) -std=c18

vpath %.c src
vpath %.h inc

SRCS_ARGPARSE = src/argparse/parse.c \
				src/argparse/usage.c \
				src/argparse/getopt.c

SRCS = $(SRCS_ARGPARSE) \
	   src/main.c

OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

all: $(NAME)


$(NAME): $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR):
	mkdir -p $(sort $(addprefix $(OBJDIR)/, $(dir $(SRCS))))

$(OBJDIR)/%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
