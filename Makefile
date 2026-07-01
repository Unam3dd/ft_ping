NAME = ft_ping
INCLUDE_DIR = inc
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDE_DIR)

vpath %.c src
vpath %.h inc

SRCS = src/init.c \
	   src/options.c \
	   src/help.c \
	   src/echo.c \
	   src/checksum.c \
	   src/display.c \
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
