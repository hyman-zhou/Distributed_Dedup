#include <stdlib.h>
#include <string.h>
#define CHUNK_UNIQUE (0x00)
#define CHUNK_DUPLICATE (0x01)

#define SET_CHUNK(c, f) (c->flag |= f)
#define UNSET_CHUNK(c, f) (c->flag &= ~f)
#define CHECK_CHUNK(c, f) (c->flag & f)

/* signal chunk */
#define CHUNK_FILE_START (0x0001)
#define CHUNK_FILE_END (0x0002)
#define CHUNK_SEGMENT_START (0x0004)
#define CHUNK_SEGMENT_END (0x0008)

typedef unsigned char fingerprint[20];
struct Chunk
{
    public:
    Chunk(int32_t size):m_size(size),m_flag(CHUNK_UNIQUE),m_data(nullptr){
        m_fp = new fingerprint;
        memset(&m_fp,0x00,sizeof(fingerprint));
        if(size > 0){
            m_data = malloc(size);
        }
    }
    ~Chunk(){
        if(m_data != nullptr){
            free(m_data);
        }
        m_data = nullptr;
        delete [] m_fp;
    }
    public:   
    int32_t m_size;
	int m_flag;
	// containerid id;
	fingerprint m_fp;
	unsigned char *m_data;
};
