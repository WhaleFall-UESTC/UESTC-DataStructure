#define OUTPUT_ALL      1
#define OUTPUT_MAX      2
#define OUTPUT_TOPN     4
#define OUTPUT_DEFAULT  OUTPUT_ALL

#define DEFAULT_FILE "src.txt"
#define DEFAULT_TOPN    5
#define DEFAULT_MIN_SUP 3

#define RAND(l, u) (rand() % (l - u + 1) + l)
#define MAX_LEN         54
#define MIN_LEN         10
#define MAX_INDEX       128
#define MIN_INDEX       64
#define MAX_GROUP_NUM   64
#define MIN_GROUP_NUM   28

#define BUF 512

int         read_file(database *db, const char *filename);
int         random_file(const char *filename);
