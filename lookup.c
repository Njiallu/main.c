#include <stddef.h>
#include <stdio.h>

typedef enum	e_type
{
	TYPE_1,
	TYPE_2,
	TYPE_3,
	TYPE_4,
	TYPE_MAX
}				t_type;

typedef struct	s1
{
	t_type		type;
	void		*data;
}				t_s1;

typedef struct	s2
{
	t_type		type;
	int			data;
}				t_s2;

typedef struct	s3
{
	t_type		type;
	void		*data;
	int			padding;
}				t_s3;

typedef union	u_data
{
	t_type		type;
	t_s1		s1;
	t_s2		s2;
	t_s3		s3;
}				t_data;

typedef void	(*t_cb_ptr)(t_data);
typedef t_data	(*t_data_ptr)(void*);

void			func1(t_data data)
{
	printf("Data as s1 [%d]: '%p'\n", data.type, data.s1.data);
}

void			func2(t_data data)
{
	printf("Data as s2 [%d]: '%d'\n", data.type, data.s2.data);
}

void			func3(t_data data)
{
	printf("Data as s3 [%d]: '%p' '%d'\n", data.type, data.s3.data, data.s3.padding);
}

t_data			generate_generic_data(void *data)
{
	return ((t_data){
		.s1 = (t_s1){
			.type = TYPE_1,
			.data = data,
		}
	});
}

t_data			generate_int_data(void *data)
{
	return ((t_data){
		.s2 = (t_s2){
			.type = TYPE_2,
			.data = (int)data,
		}
	});
}

t_data			generate_big_data(void *data)
{
	return ((t_data){
		.s3 = (t_s3){
			.type = TYPE_3,
			.data = data,
			.padding = (short)data,
		}
	});
}

void			func_select(t_type type, void *data)
{
	static t_cb_ptr		callback[TYPE_MAX] = {
		[TYPE_1] = &func1,
		[TYPE_2] = &func2,
		[TYPE_3] = &func3,
		[TYPE_4] = &func2,
	};
	static t_data_ptr	generate[TYPE_MAX] = {
		[TYPE_1] = &generate_generic_data,
		[TYPE_2] = &generate_int_data,
		[TYPE_3] = &generate_big_data,
		[TYPE_4] = &generate_generic_data,
	};

	callback[type](generate[type](data));
}

int				main(void)
{
	func_select(TYPE_4, (void*)23645237);
	func_select(TYPE_3, (void*)12);
	func_select(TYPE_2, (void*)42);
	func_select(TYPE_1, (void*)1254);
}
