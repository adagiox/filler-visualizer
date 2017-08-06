#include "visualizer.h"

int setup_colors();

int setup_screen()
{
	initscr();
 	noecho();
 	curs_set(FALSE);
	keypad(stdscr, TRUE);
	clear();
	setup_colors();
	refresh();
	usleep(SECOND);
	return (1);
}

int cleanup_screen()
{
	clear();
	refresh();
	attroff(COLOR_PAIR(3));
	endwin();
	return (1);
}

int setup_colors()
{
	start_color();
	init_color(COLOR_RED, 500, 0, 0);
	init_color(COLOR_GREEN, 0, 500, 0);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_BLUE, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(7, COLOR_CYAN, COLOR_BLACK);
	return (1);
}

void sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		cleanup_screen();
		printf("ABORTING...\n");
		exit(1);
	}
}

int open_file(char *file)
{
	int fd;

	fd = open(file, O_RDONLY);
	return (fd);
}

void print_all_frames(t_frame_list *head)
{
	t_frame_list *p;
	p = head;
	while (p != NULL)
	{
		if (p->frame->grid == NULL)
			break ;
		printf("Frame number: %i\n", p->frame->frame_number);
		printf("Plateau %i %i:\n", p->frame->row, p->frame->col);
		for (int i = 0; i < p->frame->row + 1; i++)
			printf("%s\n", p->frame->grid[i]);
		p = p->next;
		printf("\n");
	}
}

t_frame_list *add_frame_list_node(t_frame_list *head, t_frame *frame)
{
	t_frame_list *node;
	t_frame_list *p;
	p = head;
	int frame_count;

	frame_count = 1;
	if (frame->grid == NULL)
		head->more = 0;
	if (p == NULL)
	{
		p = (t_frame_list *)malloc(sizeof(t_frame_list));
		p->next = NULL;
		p->prev = NULL;
		p->frame = frame;
		p->frame->frame_number = 0;
		p->more = 1;
		return (p);
	}
	while (p->next != NULL)
	{
		p = p->next;
		frame_count++;
	}
	if (p->next == NULL && p != NULL)
	{
		node = (t_frame_list *)malloc(sizeof(t_frame_list));
		node->next = NULL;
		node->prev = p;
		node->frame = frame;
		node->frame->frame_number = frame_count;
		p->next = node;
	}
	return (head);
}

t_frame *next_frame(int fd)
{
	t_frame *frame;
	char *line;
	int i;
	char **grid;

	frame = (t_frame *)malloc(sizeof(t_frame));
	frame->grid = NULL;
	while (get_next_line(fd, &line))
	{
		if (ft_strstr(line, "Plateau"))
		{
			i = 8;
			frame->row = ft_atoi(&line[i]);
			i += ft_count_digits(frame->row) + 1;
			frame->col = ft_atoi(&line[i]);
			grid = (char **)malloc(sizeof(char *) * (frame->row + 1));
			for (int i = 0; i < frame->row + 1; i++)
				grid[i] = (char *)malloc(sizeof(char) * frame->col);
			for (int i = 0; i < frame->row + 1; i++)
			{
				get_next_line(fd, &line);
				grid[i] = line;
			}
			frame->grid = grid;
			break ;
		}
		else if (ft_strstr(line, "fin"))
		{
			i = 10;
			frame->row = ft_atoi(&line[i]);
			get_next_line(fd, &line);
			frame->col = ft_atoi(&line[i]);
		}
	}
	return (frame);
}

// Returns 1 if a frame is added or -1 no more frames to read or error

int get_next_frame(t_frame_list **head, int fd)
{
	t_frame_list *ptr;
	*head = add_frame_list_node(*head, next_frame(fd));
	if ((*head)->more == 0)
		return (-1);
	return (1);
}

t_frame_list *parse_frames(int fd)
{
	t_frame_list *head;

	head = NULL;
	while (get_next_frame(&head, fd) == 1)
		;
	return (head);
}

int print_frame_scr(t_frame_list *node)
{
	if (node->frame->grid == NULL)
	{
		int row;
		int col;
		getyx(stdscr, row, col);
		move(row + 1, 0);
		printw("Final Score\n== O fin: %i\n== X fin: %i\n", node->frame->row, node->frame->col);
		printw("\nSpacebar to continue...");
		refresh();
		while (1)
		{
			int ch;
			nodelay(stdscr, TRUE);
			if ((ch = getch()) == ERR)
				continue;
			else if (ch == ' ')
				break;
			sleep(SECOND / 50);
		}
		return 1;
	}
	move(0, 0);
	printw("Frame number: %i", node->frame->frame_number);
	move(1, 0);
	printw("Plateau %i %i:", node->frame->row, node->frame->col);
	int r = 1;
	time_t t;
	srand((unsigned) time(&t));
	for (int i = 2; i < node->frame->row + 2; i++)
	{
		move(i, 0);
		char *str = node->frame->grid[i - 2];
		for (int j = 0; j < ft_strlen(str); j++)
		{
			if (str[j] == 'O' || str[j] == 'o')
			{
				//r = rand() % 7;
				attron(COLOR_PAIR(3));
				printw("%c", str[j]);
				attroff(COLOR_PAIR(3));
			}
			else if (str[j] == 'X' || str[j] == 'x')
			{
				//r = rand() % 7;
				attron(COLOR_PAIR(4));
				printw("%c", str[j]);
				attroff(COLOR_PAIR(4));
			}
			else if (str[j] == '.')
				printw(" ");
			else
				printw("%c", str[j]);
		}
		printw("\n");
	}
	return (1);
}

int play_frames(t_frame_list *head)
{
	t_frame_list *p;
	int sleepdiv = 10;

	p = head;
	while (p != NULL)
	{
		int ch;
		nodelay(stdscr,  TRUE);
		if ((ch = getch()) == ERR)
		{
			print_frame_scr(p);
			refresh();
			p = p->next;
			usleep(SECOND / sleepdiv);
		}
		else if (ch == KEY_DOWN)
		{
			if (sleepdiv > 1)
				sleepdiv--;
		}
		else if (ch == KEY_UP)
		{
			if (sleepdiv < 100)
				sleepdiv++;
		}
		else if (ch == ' ')
		{
			usleep(SECOND / 50);
			while (1)
			{
				if ((ch = getch()) == ERR)
					continue;
				else if (ch == ' ')
					break;
				else if (ch == KEY_LEFT)
				{
					if (p->prev != NULL)
						p = p->prev;
					print_frame_scr(p);
					refresh();
					usleep(SECOND / 50);
					continue;
				}
				else if (ch == KEY_RIGHT)
				{
					if (p->next->frame->grid != NULL)
						p = p->next;
					print_frame_scr(p);
					refresh();
					usleep(SECOND / 50);
					continue;
				}
			}
		}
	}
	usleep(SECOND / 50);
	return (1);
}

int main(int argc, char **argv)
{
	char *file;
	int fd;
	int num;
	signal(SIGINT, sigint_handler);

	if (argc < 2)
	{
		printf("Usage:\nPlease specify a valid filename as the second argument.\n");
		return (-1);
	}
	num = 1;
	while (num < argc)
	{
		setup_screen();
		file = argv[num];
		if ((fd = open_file(file)) < 0)
		{
			printf("Open failed!\n");
			return (-1);
		}
	
		// Read the file and parse into frames
		t_frame_list *frame_list;
		frame_list = parse_frames(fd);
		//print_all_frames(frame_list);

		// Start the visualizer
		if (frame_list != NULL)
			play_frames(frame_list);
		num++;
		cleanup_screen();
	}
	return (1);
}