// constants
#define MAX_BUF_SIZE 200
#define MAX_CMD_LEN 50
#define MAX_ENV_VARS 2
#define DEFAULT_PATH "/bin/:/usr/bin/"
#define DEFAULT_PS1 "sbush"
#define SCREEN_HEIGHT 60

// environment variables
extern char *path;
extern char *ps1;

// function prototypes
void setenv(char *args[]);
void getenv(char *args[]);
void cd(char *args[]);
void runcmd(const char* cmd, char *const args[]);
void clear(void);
