#ifndef COMMANDS_H
#define COMMANDS_H

void initRepository();
void hashObject(int argc, char* argv[]);
void catFile(int argc, char* argv[]);
// Ensure these two are present:
void commit(int argc, char* argv[]);
void log(int argc, char* argv[]);
void add(int argc, char* argv[]);
void status(int argc, char* argv[]);

#endif