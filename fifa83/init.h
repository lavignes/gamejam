#ifndef INIT_H
#define INIT_H

#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/glfw.h>

#include <AL/alut.h>

void context_init();

void context_close();

#endif /* INIT_H */