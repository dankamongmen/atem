#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

static void
usage(const char **argv){
	fprintf(stderr,"usage: %s outdir\n",argv[0]);
}

// FIXME ought ensure that we don't hit a symbol twice, since we'll rewrite the
// file each time there's a gap :/
int main(int argc,const char **argv){
	char buf[BUFSIZ],ptoken[PATH_MAX],curfile[PATH_MAX];
	FILE *curout = NULL;

	if(argc != 2){
		usage(argv);
		return EXIT_FAILURE;
	}
	// FIXME require an -f argument to continue on EEXIST?
	if(mkdir(argv[1],0755) && errno != EEXIST){
		fprintf(stderr,"Couldn't mkdir(%s) (%s?)\n",argv[1],strerror(errno));
		return EXIT_FAILURE;
	}
	ptoken[0] = '\0';
	while(fgets(buf,sizeof(buf),stdin)){
		char *delim = strchr(buf,',');

		if(delim == NULL || delim == buf || delim - buf + 1 > sizeof(ptoken)){
			fprintf(stderr,"Format error on stdin: %s\n",buf);
			return EXIT_FAILURE;
		}
		if(strncmp(ptoken,buf,delim - buf) == 0 && ptoken[delim - buf] == '\0'){
			if(fprintf(curout,"%s",buf) <= 0){
				fprintf(stderr,"Error writing to %s: %s\n",curfile,strerror(errno));
				return EXIT_FAILURE;
			}
		}else{
			if(curout){
				if(fclose(curout)){
					fprintf(stderr,"Error closing %s: %s\n",curfile,strerror(errno));
					return EXIT_FAILURE;
				}
			}
			strncpy(ptoken,buf,delim - buf + 1);
			ptoken[delim - buf] = '\0';
			if(snprintf(curfile,sizeof(curfile),"%s/%s.csv",argv[1],ptoken) >= sizeof(curfile) ||
			  (curout = fopen(curfile,"w")) == NULL){
				fprintf(stderr,"Error opening %s: %s\n",curfile,strerror(errno));
				return EXIT_FAILURE;
			}
			if(fprintf(curout,"Ticker,Date,Open,High,Low,Close,Volume,Open Interest\n") < 0){
				fprintf(stderr,"Error writing to %s: %s\n",curfile,strerror(errno));
				return EXIT_FAILURE;
			}
		}
	}
	if(ferror(stdin)){
		fprintf(stderr,"Error reading from stdin (%s?)\n",strerror(errno));
		return EXIT_FAILURE;
	}
	if(curout){
		if(fclose(curout)){
			fprintf(stderr,"Error closing %s: %s\n",curfile,strerror(errno));
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
