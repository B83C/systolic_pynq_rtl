// Depthwise 3×3 INT8 — ARM NEON for MAC, scalar for requant.
// g++ -O2 -march=armv7-a -mfpu=neon -o dw3x3 dw3x3.cpp
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <arm_neon.h>

int main(int argc, char **argv) {
    if (argc<13) return 1;
    uint64_t in_pa=strtoull(argv[1],0,16), w_pa=strtoull(argv[2],0,16);
    uint64_t b_pa=strtoull(argv[3],0,16), out_pa=strtoull(argv[4],0,16);
    int H=atoi(argv[5]),W=atoi(argv[6]),C=atoi(argv[7]);
    int zi=atoi(argv[8]),zw=atoi(argv[9]),mq=atoi(argv[10]),sh=atoi(argv[11]),zo=atoi(argv[12]);

    int fd=open("/dev/mem",O_RDWR|O_SYNC);
    auto m = [fd](uint64_t pa, size_t sz)->volatile uint8_t* {
        size_t off = pa & 4095;
        return (volatile uint8_t*)mmap(0, sz+off, PROT_READ|PROT_WRITE, MAP_SHARED, fd, pa-off) + off;
    };
    int8_t* in = (int8_t*)m(in_pa, H*W*C);
    int8_t* w  = (int8_t*)m(w_pa, C*9);
    int32_t* b = (int32_t*)m(b_pa, C*4);
    int8_t* out= (int8_t*)m(out_pa, H*W*C);
    close(fd);

    int stride = W * C;
    int8x8_t vzi = vdup_n_s8(zi), vzw = vdup_n_s8(zw);

    struct timespec t0,t1;
    clock_gettime(CLOCK_MONOTONIC,&t0);

    for (int oy=0; oy<H; oy++) {
        for (int ox=0; ox<W; ox++) {
            int c = 0;
            for (; c+8 <= C; c += 8) {
                int32x4_t acc_l = vdupq_n_s32(0), acc_h = vdupq_n_s32(0);
                for (int ky=0;ky<3;ky++) for (int kx=0;kx<3;kx++) {
                    int iy=oy+ky-1, ix=ox+kx-1;
                    if (iy<0||iy>=H||ix<0||ix>=W) continue;
                    int8x8_t iv = vld1_s8(in + iy*stride + ix*C + c);
                    int8x8_t wv = vld1_s8(w + (ky*3+kx)*C + c);  // (3,3,C) layout
                    int16x8_t is = vsubl_s8(iv, vzi);
                    int16x8_t ws = vsubl_s8(wv, vzw);
                    acc_l = vmlal_s16(acc_l, vget_low_s16(is), vget_low_s16(ws));
                    acc_h = vmlal_s16(acc_h, vget_high_s16(is), vget_high_s16(ws));
                }
                // Add bias + requant (scalar)
                int32_t tl[4], th[4];
                vst1q_s32(tl, vaddq_s32(acc_l, vld1q_s32(b+c)));
                vst1q_s32(th, vaddq_s32(acc_h, vld1q_s32(b+c+4)));
                for (int k=0;k<4;k++) {
                    int q=((tl[k]*mq)>>sh)+zo; if(q<-128)q=-128; if(q>127)q=127;
                    out[(oy*W+ox)*C+c+k]=(int8_t)q;
                    q=((th[k]*mq)>>sh)+zo; if(q<-128)q=-128; if(q>127)q=127;
                    out[(oy*W+ox)*C+c+4+k]=(int8_t)q;
                }
            }
            for (; c<C; c++) {
                int a=b[c];
                for (int ky=0;ky<3;ky++) for (int kx=0;kx<3;kx++) {
                    int iy=oy+ky-1, ix=ox+kx-1;
                    if (iy<0||iy>=H||ix<0||ix>=W) continue;
                    a+=((int)in[iy*stride+ix*C+c]-zi)*((int)w[(ky*3+kx)*C+c]-zw);
                }
                int q=((a*mq)>>sh)+zo; if(q<-128)q=-128; if(q>127)q=127;
                out[(oy*W+ox)*C+c]=(int8_t)q;
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC,&t1);
    printf("%.6f", (t1.tv_sec-t0.tv_sec)+(t1.tv_nsec-t0.tv_nsec)*1e-9);
    return 0;
}
