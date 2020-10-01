main:
	gcc -g parse.c setenv.c process.c jobs.c history.c execute.c pinfo.c ls.c cd.c echo.c pwd.c formatpath.c prompt.c main.c -o leshell
