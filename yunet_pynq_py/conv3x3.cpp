// conv3x3 first layer — ARM NEON.  (3→16)×640×640 stride 2, SAME padding.
// g++ -O2 -march=armv7-a -mfpu=neon -o conv3x3 conv3x3.cpp
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <arm_neon.h>

int main(int argc, char **argv) {
    if(argc<13) return 1;
    uint64_t in_pa=strtoull(argv[1],0,16), w_pa=strtoull(argv[2],0,16);
    uint64_t b_pa=strtoull(argv[3],0,16), out_pa=strtoull(argv[4],0,16);
    int H=atoi(argv[5]),W=atoi(argv[6]),C_in=atoi(argv[7]),C_out=atoi(argv[8]);
    int zi=atoi(argv[9]),zw=atoi(argv[10]),mq=atoi(argv[11]),sh=atoi(argv[12]),zo=atoi(argv[13]);

    int fd=open("/dev/mem",O_RDWR|O_SYNC);
    auto m=[fd](uint64_t pa,size_t sz)->volatile uint8_t*{
        return (volatile uint8_t*)mmap(0,sz,PROT_READ|PROT_WRITE,MAP_SHARED,fd,pa&~4095);
    };
    int8_t* in =(int8_t*)(m(in_pa, H*W*C_in)+(in_pa&4095));
    int8_t* w =(int8_t*)(m(w_pa, C_out*C_in*9)+(w_pa&4095));
    int32_t* b=(int32_t*)(m(b_pa, C_out*4)+(b_pa&4095));
    int8_t* out=(int8_t*)(m(out_pa, H/2*W/2*C_out)+(out_pa&4095));
    close(fd);

    int8x8_t vzi=vdup_n_s8(zi), vzw=vdup_n_s8(zw);
    int8_t pad = (int8_t)zi;  // pad with zp_in → (pad - zi = 0) so out-of-bounds contributes nothing
    int oH=H/2, oW=W/2;

    struct timespec t0,t1;
    clock_gettime(CLOCK_MONOTONIC,&t0);

    for(int oy=0;oy<oH;oy++) for(int ox=0;ox<oW;ox++) {
        int c=0;
        // NEON: 8 output channels at a time
        for(;c+8<=C_out;c+=8){
            int32x4_t accl=vdupq_n_s32(0),acch=vdupq_n_s32(0);
            for(int ky=0;ky<3;ky++) for(int kx=0;kx<3;kx++) {
                int iy=oy*2+ky-1, ix=ox*2+kx-1;
                for(int ci=0;ci<C_in;ci++){
                    int8_t iv=pad;
                    if(iy>=0&&iy<H&&ix>=0&&ix<W) iv=in[(iy*W+ix)*C_in+ci];
                    int8x8_t wv=vld1_s8(w+(ci*9+ky*3+kx)*C_out+c);  // (C_in,3,3,C_out)
                    int16x8_t ws=vsubl_s8(wv,vzw);
                    int16_t is=(int16_t)iv-(int16_t)zi;
                    accl=vmlal_n_s16(accl,vget_low_s16(ws),is);
                    acch=vmlal_n_s16(acch,vget_high_s16(ws),is);
                }
            }
            int32_t tl[4],th[4];
            vst1q_s32(tl,vaddq_s32(accl,vld1q_s32(b+c)));
            vst1q_s32(th,vaddq_s32(acch,vld1q_s32(b+c+4)));
            for(int k=0;k<4;k++){
                int q=((tl[k]*mq)>>sh)+zo; if(q<-128)q=-128; if(q>127)q=127;
                out[(oy*oW+ox)*C_out+c+k]=(int8_t)q;
                q=((th[k]*mq)>>sh)+zo; if(q<-128)q=-128; if(q>127)q=127;
                out[(oy*oW+ox)*C_out+c+4+k]=(int8_t)q;
            }
        }
        // scalar remainder
        for(;c<C_out;c++){
            int a=b[c];
            for(int ky=0;ky<3;ky++) for(int kx=0;kx<3;kx++) {
                int iy=oy*2+ky-1, ix=ox*2+kx-1;
                for(int ci=0;ci<C_in;ci++){
                    int iv=pad;
                    if(iy>=0&&iy<H&&ix>=0&&ix<W) iv=in[(iy*W+ix)*C_in+ci];
                    a+=((int)iv-zi)*((int)w[(ci*9+ky*3+kx)*C_out+c]-zw);
                }
            }
            int q=((a*mq)>>sh)+zo; if(q<-128)q=-128; if(q>127)q=127;
            out[(oy*oW+ox)*C_out+c]=(int8_t)q;
        }
    }

    clock_gettime(CLOCK_MONOTONIC,&t1);
    printf("%.6f",(t1.tv_sec-t0.tv_sec)+(t1.tv_nsec-t0.tv_nsec)*1e-9);
    return 0;
}
