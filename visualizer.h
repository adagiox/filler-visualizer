#ifndef VISUALIZER_H
# define VISUALIZER_H
# include <curses.h>
# include <stdio.h>
# include <unistd.h>
# include "../libft/includes/libft.h"
# include "../libft/includes/ft_printf.h"

# define SECOND 1000000

/* 
**  frame to be displayed
**  includes the dimensions and both the output of the filler and our internal mapping
*/

typedef struct 			s_frame
{
	int 				row;
	int 				col;
	int					frame_number;
	char				**grid;
}						t_frame;

/* 
**  doubly linked list of frames
*/

typedef struct 			s_frame_list
{
	unsigned int		more : 1;
	t_frame 			*frame;
	struct s_frame_list	*next;
	struct s_frame_list	*prev;
}						t_frame_list;

int setup_screen();
int cleanup_screen();
int open_file(char *file);

#endif