#include <sys/sbunix.h>
#include <sys/proc.h>

// buffer to copy things into from the kernel buffer
char termbuf[200];

int fwrite(int fd, char* buf, uint64_t n) {
  int i;
  if (fd == STDOUT) {
    for ( i=0; buf[ i ] != '\0'; i++ ) {
      if (buf[i] == '%' && buf[i+1] == 'd') {
        printf( buf, (int)n );
        break;
      } else if (buf[i] == '%' && buf[i+1] == 's') {
        printf( buf, (char*)n );
        break;
      } else if (buf[i] == '%' && buf[i+1] == 'c') {
        printf( buf, (char)n );
        break;
      }
    }
    if ( buf[ i ] == '\0' )
      printf( buf );
  }
  return 0;
}

int write(int fd, char *buf, int n) {
  int i;
  if (fd == STDOUT) {
    for (i = 0; i < n; i++) {
      putchar(buf[i]);
    }
  }
  return 0;
}

int hasnewline() {
  int i;
  for(i = 0; i < inputpos; i++) {
    if (inputqueue[i] == '\n')
      return 1;
  }
  return 0;
}

// returns the number of bytes read
int read(int fd, char* buf, int n) {
  int ret = 0, i;
  struct file *f;
  int bytes_to_read;
  inputpos = 0;

  if (fd == STDIN) {
    for(i = 0; i < 200; i++)
      termbuf[ i ] = 0;

    // wait it out, till the data is here or the user entered newline
    // cannot copying data because keyboard interrupts are interleaved with
    // this function and inputpos increments are not always by 1
    while (inputpos < n && !hasnewline()) {
    }

    // copying data into the buffer
    for (i = 0; i < n; i++) {
      if (inputqueue[i] == '\n')
	break;
      termbuf[ i ] = inputqueue[ i ];
      ret++;
    }

    for( i=0; i < n; i++ ) {
      *(buf + i) = termbuf[ i ];
    }
    *(buf+i) = '\0';
    
  }
  else {
    f = &current_proc->files[fd];

    if (f->curr_off == f->start_off + f->sz) {
      // eof
      bytes_to_read = 0;
    } else if (f->curr_off + n > f->start_off + f->sz) {
      // reading beyond eof
      bytes_to_read = f->start_off + f->sz - f->curr_off;
    } else {
      bytes_to_read = n;
    }

    strncpy(buf, f->curr_off, bytes_to_read);
    f->curr_off += bytes_to_read;

    ret = bytes_to_read;
  }
  return ret;
}
