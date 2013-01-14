#include <stdlib.h>
#include <GL/glew.h>
#include <glib.h>
#include "gfx.h"

#define gfx_is_valid_shader_type(shader) (shader >= 0 && shader < GFX_NUMBER_OF_SHADER_TYPES)

struct gfx_shader
{
	GLuint object;
	gfx_shader_type type;
};

struct gfx_program
{
	GLuint object;
	gfx_shader *vertex_shader;
	gfx_shader *fragment_shader;
	gfx_shader *geometry_shader;
};

static void print_shader_log(GLuint object)
{
	int length = 0;
	int written = 0;
	char *log = NULL;

	glGetShaderiv(object, GL_INFO_LOG_LENGTH, &length);

	if(length > 0)
	{
		log = (char*)malloc(length);
		glGetShaderInfoLog(object, length, &written, log);
		fprintf(stderr, "%s\n", log);
		free(log);
	}
}

static void print_program_log(GLuint object)
{
	int length = 0;
	int written = 0;
	char *log = NULL;

	glGetProgramiv(object, GL_INFO_LOG_LENGTH, &length);

	if(length > 0)
	{
		log = (char*)malloc(length);
		glGetProgramInfoLog(object, length, &written, log);
		fprintf(stderr, "%s\n", log);
		free(log);
	}
}


gfx_shader *gfx_shader_new(const gfx_shader_type type, const char *source)
{
	gfx_shader *shader = NULL;
	GLuint target;
	GLint status;

	if(!gfx_is_valid_shader_type(type))
		return NULL;

	shader = calloc(1, sizeof(struct gfx_shader));

	switch(type)
	{
	case GFX_VERTEX_SHADER:
		target = GL_VERTEX_SHADER;
		break;
	case GFX_FRAGMENT_SHADER:
		target = GL_FRAGMENT_SHADER;
		break;
	case GFX_GEOMETRY_SHADER:
		target = GL_GEOMETRY_SHADER;
		break;
	default:
		break;
	}

	shader->object = glCreateShader(target);
	shader->type = type;
	glShaderSource(shader->object, 1, &source, NULL);
	glCompileShader(shader->object);
	glGetShaderiv(shader->object, GL_COMPILE_STATUS, &status);

	if(!status)
	{
		print_shader_log(shader->object);
		glDeleteShader(shader->object);
		free(shader);
		return NULL;
	}

	return shader;
}

void gfx_shader_delete(gfx_shader **shader)
{
	gfx_shader* target;
	if(!shader || !*shader)
		return;

	target = *shader;

	glDeleteShader(target->object);
	free(target);

	*shader = NULL;
}

gfx_shader *gfx_shader_new_from_file(const gfx_shader_type type, const char *filename)
{
	char *source = NULL;
	gfx_shader *shader = NULL;

	g_file_get_contents(filename, &source, NULL, NULL);

	if(!source)
		return NULL;

	shader = gfx_shader_new(type, source);
	g_free(source);

	return shader;
}

gfx_program *gfx_program_new(gfx_shader *vertex_shader, gfx_shader *geometry_shader, gfx_shader *fragment_shader)
{
	gfx_program *program = NULL;
	GLint status;

	program = calloc(1, sizeof(struct gfx_program));
	program->object = glCreateProgram();

	if(vertex_shader && vertex_shader->type == GFX_VERTEX_SHADER)
	{
		glAttachShader(program->object, vertex_shader->object);
		program->vertex_shader = vertex_shader;
	}

	if(geometry_shader && geometry_shader->type == GFX_GEOMETRY_SHADER)
	{
		glAttachShader(program->object, geometry_shader->object);
		program->geometry_shader = geometry_shader;
	}

	if(fragment_shader && fragment_shader->type == GFX_FRAGMENT_SHADER)
	{
		glAttachShader(program->object, fragment_shader->object);
		program->geometry_shader = fragment_shader;
	}

	glLinkProgram(program->object);
	glGetProgramiv(program->object, GL_LINK_STATUS, &status);

	if(!status)
	{
		print_program_log(program->object);
		glDeleteProgram(program->object);
		free(program);
		return NULL;
	}

	return program;
}

void gfx_program_delete(gfx_program **program)
{
	gfx_program* target;

	if(!program || !*program)
		return;

	target = *program;

	gfx_shader_delete(&(target->vertex_shader));
	gfx_shader_delete(&(target->geometry_shader));
	gfx_shader_delete(&(target->fragment_shader));
	glDeleteProgram(target->object);
	free(target);

	*program = NULL;
}

void gfx_program_use(const gfx_program *program)
{
	if(!program)
		return;

	glUseProgram(program->object);
}

