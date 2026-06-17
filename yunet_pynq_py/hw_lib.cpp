// HW acceleration library — compiled as .so, called from Python via ctypes.
// g++ -O2 -march=armv7-a -mfpu=neon -shared -fPIC -o libhwynet.so hw_lib.cpp
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <arm_neon.h>

static int mem_fd = -1;

extern "C" void hw_init() {
    if (mem_fd < 0) mem_fd = open("/dev/mem", O_RDWR|O_SYNC);
}
extern "C" void hw_close() { if (mem_fd>=0) close(mem_fd); mem_fd=-1; }

// ── Helper: map physical address ──
static int8_t* map_pa(uint64_t pa, size_t sz) {
    uint64_t pg = pa & ~4095ULL;
    return (int8_t*)mmap(0, sz + (pa-pg), PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, pg)
           + (pa - pg);
}

// ── Depthwise 3x3 ──────────────────────────────────────────────────
extern "C" void hw_dw3x3(uint64_t in_pa, uint64_t w_pa, uint64_t b_pa, uint64_t out_pa,
                          int H, int W, int C, int zi, int zw, int mq, int sh, int zo) {
    int8_t* in  = map_pa(in_pa,  H*W*C);
    int8_t* w   = map_pa(w_pa,   C*9);
    int32_t* b  = (int32_t*)map_pa(b_pa, C*4);
    int8_t* out = map_pa(out_pa, H*W*C);
    int s = W*C;
    int8x8_t vzi=vdup_n_s8(zi), vzw=vdup_n_s8(zw);
    for(int oy=0;oy<H;oy++) for(int ox=0;ox<W;ox++) {
        int c=0;
        for(;c+8<=C;c+=8){
            int32x4_t al=vdupq_n_s32(0), ah=vdupq_n_s32(0);
            for(int ky=0;ky<3;ky++) for(int kx=0;kx<3;kx++){
                int iy=oy+ky-1,ix=ox+kx-1;
                if(iy<0||iy>=H||ix<0||ix>=W) continue;
                int8x8_t iv=vld1_s8(in+iy*s+ix*C+c);
                int8x8_t wv=vld1_s8(w+(ky*3+kx)*C+c);
                int16x8_t is=vsubl_s8(iv,vzi),ws=vsubl_s8(wv,vzw);
                al=vmlal_s16(al,vget_low_s16(is),vget_low_s16(ws));
                ah=vmlal_s16(ah,vget_high_s16(is),vget_high_s16(ws));
            }
            int32_t tl[4],th[4];
            vst1q_s32(tl,vaddq_s32(al,vld1q_s32(b+c)));
            vst1q_s32(th,vaddq_s32(ah,vld1q_s32(b+c+4)));
            for(int k=0;k<4;k++){
                int q=((tl[k]*mq)>>sh)+zo; if(q<-128)q=-128; if(q>127)q=127;
                out[(oy*W+ox)*C+c+k]=(int8_t)q;
                q=((th[k]*mq)>>sh)+zo; if(q<-128)q=-128; if(q>127)q=127;
                out[(oy*W+ox)*C+c+4+k]=(int8_t)q;
            }
        }
        for(;c<C;c++){
            int a=b[c];
            for(int ky=0;ky<3;ky++) for(int kx=0;kx<3;kx++){
                int iy=oy+ky-1,ix=ox+kx-1;
                if(iy<0||iy>=H||ix<0||ix>=W) continue;
                a+=((int)in[iy*s+ix*C+c]-zi)*((int)w[(ky*3+kx)*C+c]-zw);
            }
            int q=((a*mq)>>sh)+zo; if(q<-128)q=-128; if(q>127)q=127;
            out[(oy*W+ox)*C+c]=(int8_t)q;
        }
    }
    munmap(in,0); munmap(w,0); munmap((void*)b,0); munmap(out,0);
}

// ── Conv3x3 (first layer, 3→16, stride 2) ──────────────────────────
extern "C" void hw_conv3x3(uint64_t in_pa, uint64_t w_pa, uint64_t b_pa, uint64_t out_pa,
                           int H, int W, int Ci, int Co,
                           int zi, int zw, int mq, int sh, int zo) {
    int8_t* in =map_pa(in_pa, H*W*Ci);
    int8_t* w =map_pa(w_pa, Ci*9*Co);
    int32_t* b=(int32_t*)map_pa(b_pa, Co*4);
    int8_t* out=map_pa(out_pa, H/2*W/2*Co);
    int8_t pad=(int8_t)zi;
    int8x8_t vzi=vdup_n_s8(zi), vzw=vdup_n_s8(zw);
    int oH=H/2,oW=W/2;
    for(int oy=0;oy<oH;oy++) for(int ox=0;ox<oW;ox++){
        int c=0;
        for(;c+8<=Co;c+=8){
            int32x4_t al=vdupq_n_s32(0),ah=vdupq_n_s32(0);
            for(int ky=0;ky<3;ky++) for(int kx=0;kx<3;kx++){
                int iy=oy*2+ky-1,ix=ox*2+kx-1;
                for(int ci=0;ci<Ci;ci++){
                    int8_t iv=pad;
                    if(iy>=0&&iy<H&&ix>=0&&ix<W) iv=in[(iy*W+ix)*Ci+ci];
                    int8x8_t wv=vld1_s8(w+(ci*9+ky*3+kx)*Co+c);  // (Ci,3,3,Co)
                    int16x8_t ws=vsubl_s8(wv,vzw);
                    int16_t is=(int16_t)iv-(int16_t)zi;
                    al=vmlal_n_s16(al,vget_low_s16(ws),is);
                    ah=vmlal_n_s16(ah,vget_high_s16(ws),is);
                }
            }
            int32_t tl[4],th[4];
            vst1q_s32(tl,vaddq_s32(al,vld1q_s32(b+c)));
            vst1q_s32(th,vaddq_s32(ah,vld1q_s32(b+c+4)));
            for(int k=0;k<4;k++){
                int q=((tl[k]*mq)>>sh)+zo; if(q<-128)q=-128; if(q>127)q=127;
                out[(oy*oW+ox)*Co+c+k]=(int8_t)q;
                q=((th[k]*mq)>>sh)+zo; if(q<-128)q=-128; if(q>127)q=127;
                out[(oy*oW+ox)*Co+c+4+k]=(int8_t)q;
            }
        }
        for(;c<Co;c++){
            int a=b[c];
            for(int ky=0;ky<3;ky++) for(int kx=0;kx<3;kx++){
                int iy=oy*2+ky-1,ix=ox*2+kx-1;
                for(int ci=0;ci<Ci;ci++){
                    int iv=(int)pad;
                    if(iy>=0&&iy<H&&ix>=0&&ix<W) iv=in[(iy*W+ix)*Ci+ci];
                    a+=((int)iv-zi)*((int)w[(ci*9+ky*3+kx)*Co+c]-zw);
                }
            }
            int q=((a*mq)>>sh)+zo; if(q<-128)q=-128; if(q>127)q=127;
            out[(oy*oW+ox)*Co+c]=(int8_t)q;
        }
    }
    munmap(in,0); munmap(w,0); munmap((void*)b,0); munmap(out,0);
}
