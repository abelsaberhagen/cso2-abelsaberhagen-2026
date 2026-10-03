#ifndef FORK_H
#define FORK_H

void writeoutput(const char *command, const char *out_path, const char *err_path);
void parallelwriteoutput(int count, const char **argv_base, const char *out_path);

#endif