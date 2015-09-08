// environment variables
extern char *path;
extern char *ps1;

void setenv(char *args[]);
void getenv(char *args[]);
void cd(char *args[]);
void runcmd(const char* cmd, char *const args[]);
