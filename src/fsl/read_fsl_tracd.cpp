#include "libhashfile.h"
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <time.h>
#include "../chunking/Chunk.h"

#define MAXLINE 4096


static void print_chunk_hash(uint64_t chunk_count,const uint8_t *hash,
                            int hash_size_in_bytes)
{
	int j;
	printf("Chunk %06ld:", chunk_count);
	printf("%.2hhx", hash[0]);
	for (j = 1; j < hash_size_in_bytes; j++)
		printf(":%.2hhx", hash[j]);
	printf("\n");
    
}


void* read_fsl_trace(void *argv,char* path){

	char buf[MAXLINE];
	struct hashfile_hadle *handle;
	const struct chunk_info *ci;
	uint64_t chunk_count;
	time_t scan_start_time;
	int ret;

	handle = hashfile_open(path);
	if(!handle){
		fprintf(stderr,"Error open hash file :%d!",errno);
		exit(1);
	}

	/* Print some information about the hash file */
	scan_start_time = hashfile_start_time(handle);
	printf("Collected at [%s] on %s",
			hashfile_sysid(handle),
			ctime(&scan_start_time));

	ret = hashfile_chunking_method_str(handle,buf,MAXLINE);
	if(ret < 0 ){
		fprintf(stderr,"Unrecognized chunking method: %d!", errno);
		exit(1);
	}
	printf("Chunking method: %s", buf);

	ret = hashfile_hashing_method_str(handle,buf,MAXLINE);
	if(ret<0){
		fprintf(stderr,"Unreconized hashing method: %d", errno);
		exit(1);
	}
	printf("Hashing method: %s", buf);

	while (1)
	{
		ret = hashfile_next_file(handle);
		if(ret < 0){
			fprintf(stderr,
					"Can't get next file from a hashfile:%d",errno);
			exit(1);
		}

		if(ret==0) 
			break;

		Chunk* c = new Chunk(strlen(hashfile_curfile_path(handle)+1));
		strcpy(c->m_data,hashfile_curfile_path(handle));

		SET_CHUNK(c,CHUNK_FILE_START);

		// push the chunk to queue

		/* Go over the chunks in the current file*/
		chunk_count = 0;
        while (true)
		{
			ci = hashfile_next_chunk(handle);

			if(!ci){
				break;
			}

			chunk_count++;

			c = new Chunk(0);

			c->m_size = ci->size;
			/*
			 * Need some padding.
			 */
			memset(c->m_fp,0,sizeof(fingerprint));
			memcpy(c->m_fp,ci->hash,hashfile_hash_size/8);
			
			// push file
		}
		

		
	}
	
	
}
                            
