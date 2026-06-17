// SA pointwise — with DMA reset between calls.
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>

#define SA_BASE   0x40000000
#define CH_BASE   0x40001000
#define DMA_BASE  0x40400000

static void dma_send(volatile uint32_t *dma, uint64_t pa, uint32_t nb) {
    dma[0x18/4]=(uint32_t)(pa&0xFFFFFFFF); dma[0x1C/4]=(uint32_t)(pa>>32); dma[0x28/4]=nb;
    dma[0x00/4]=1;
    for(int i=0;i<2000000;i++){if(dma[0x04/4]&2)break;}
}
static void bypass(volatile uint32_t *ch, int on){ch[0x8/4]=on?1:0;}
static void a_load(volatile uint32_t *sa){sa[0x10/4]=1;}
static void c_load(volatile uint32_t *sa){sa[0x08/4]=1;}

int main(int argc, char **argv) {
    if(argc<9){fprintf(stderr,"Usage: sa_pw <A_pa> <C_pa> <B_pa> <O_pa> <nA> <nC> <nB> <nO>\n");return 1;}
    uint64_t a_pa=strtoull(argv[1],0,16),c_pa=strtoull(argv[2],0,16);
    uint64_t b_pa=strtoull(argv[3],0,16),o_pa=strtoull(argv[4],0,16);
    uint32_t nA=atoi(argv[5]),nC=atoi(argv[6]),nB=atoi(argv[7]),nO=atoi(argv[8]);

    int fd=open("/dev/mem",O_RDWR|O_SYNC);
    if(fd<0){perror("/dev/mem");return 1;}
    auto sa=(volatile uint32_t*)mmap(0,0x1000,PROT_READ|PROT_WRITE,MAP_SHARED,fd,SA_BASE);
    auto ch=(volatile uint32_t*)mmap(0,0x1000,PROT_READ|PROT_WRITE,MAP_SHARED,fd,CH_BASE);
    auto dma=(volatile uint32_t*)mmap(0,65536,PROT_READ|PROT_WRITE,MAP_SHARED,fd,DMA_BASE);
    if(sa==MAP_FAILED||ch==MAP_FAILED||dma==MAP_FAILED) return 1;
    close(fd);

    struct timespec t0,t1;
    clock_gettime(CLOCK_MONOTONIC,&t0);

    if(nA>0){bypass(ch,1);a_load(sa);dma_send(dma,a_pa,nA);bypass(ch,0);}
    if(nC>0){bypass(ch,1);c_load(sa);dma_send(dma,c_pa,nC);bypass(ch,0);}

    dma[0x48/4]=(uint32_t)(o_pa&0xFFFFFFFF);dma[0x4C/4]=(uint32_t)(o_pa>>32);dma[0x58/4]=nO;
    dma[0x18/4]=(uint32_t)(b_pa&0xFFFFFFFF);dma[0x1C/4]=(uint32_t)(b_pa>>32);dma[0x28/4]=nB;
    dma[0x30/4]=1;dma[0x00/4]=1;
    for(int i=0;i<2000000;i++){if(dma[0x34/4]&2)break;}

    clock_gettime(CLOCK_MONOTONIC,&t1);
    printf("%.6f",(t1.tv_sec-t0.tv_sec)+(t1.tv_nsec-t0.tv_nsec)*1e-9);
    munmap((void*)sa,0x1000);munmap((void*)ch,0x1000);munmap((void*)dma,65536);
    return 0;
}
