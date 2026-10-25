#include "lowlevel_normal.h"
#include <string.h>
#include <limits.h>

#ifdef LOWLEVEL_NORMAL_TRACE
extern void port_normal_trace_write(const void *address, size_t size);
#define TRACE(p,n) port_normal_trace_write((p),(n))
#else
#define TRACE(p,n) ((void)0)
#endif
#define SET(v,x) do { (v)=(x); TRACE(&(v),sizeof(v)); } while (0)

/* ASM_SPEC sprite_clipping, flocon_clipping, Swap_To_Screen, Display_Bloc. */
static int32_t sprite_box[4]={8,312,0,200};
static uint32_t saved_sprite_width, saved_sprite_color, diff_sprite_width;
static uint32_t normal_strides[2]={320,320};
static int32_t weather_bounds[4];
static uint32_t tile_original_width;
/* The game owns Bloc_lim_*; tests supply them in their device shim. */
static int32_t deform_steps[8];
static uint8_t *deform_source;
static uint32_t deform_base_step, deform_inner_step, deform_row_step, deform_source_width, deform_denominator;
/* Original data has empty rows before the first reset, and 0x7fff span
 * displacements until reset replaces them. Initial edge fractions are 0xffff. */
#define EMPTY_ROW {255,255}
#define ROWS2 EMPTY_ROW,EMPTY_ROW
#define ROWS4 ROWS2,ROWS2
#define ROWS8 ROWS4,ROWS4
#define ROWS16 ROWS8,ROWS8
#define ROWS32 ROWS16,ROWS16
#define ROWS64 ROWS32,ROWS32
#define ROWS128 ROWS64,ROWS64
static uint8_t raster_rows[200][32]={ROWS128,ROWS64,ROWS8};
#undef EMPTY_ROW
#undef ROWS2
#undef ROWS4
#undef ROWS8
#undef ROWS16
#undef ROWS32
#undef ROWS64
#undef ROWS128
#define SPANS16 32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767,32767
static int32_t raster_span_displacements[80]={SPANS16,SPANS16,SPANS16,SPANS16,SPANS16};
#undef SPANS16
static uint8_t raster_edge_constant[4][2]={{0,0},{255,255},{255,255},{0,0}};
static uint16_t raster_edge_texture_fraction[4][2]={{65535,65535},{65535,65535},{65535,65535},{65535,65535}};
static uint16_t raster_edge_x_fraction[4][2]={{65535,65535},{65535,65535},{65535,65535},{65535,65535}};
static uint8_t *raster_destination, *raster_descriptor, *raster_texture, *raster_scanline;
static uint32_t raster_du, raster_neg_du, raster_dv, raster_neg_dv;
static uint32_t raster_first_y, raster_rows_left, raster_row_offset;

static int32_t signed32(uint32_t n)
{
    return n<=INT32_MAX ? (int32_t)n : -1-(int32_t)(UINT32_MAX-n);
}
static uint32_t read32(const uint8_t *p)
{
    return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
static uint32_t read16(const uint8_t *p) { return (uint32_t)p[0]|((uint32_t)p[1]<<8); }
static void write8(uint8_t *p, uint8_t v) { *p=v; TRACE(p,1); }
static void write16(uint8_t *p, uint32_t v)
{
    p[0]=(uint8_t)v; p[1]=(uint8_t)(v>>8); TRACE(p,2);
}
static void write32(uint8_t *p, uint32_t v)
{
    for (unsigned i=0;i<4;i++) p[i]=(uint8_t)(v>>(8*i));
    TRACE(p,4);
}
static void bytes_copy(uint8_t *d, const uint8_t *s, size_t n)
{
    for (size_t i=0;i<n;i++) write8(d+i,s[i]);
}
static void bytes_fill(uint8_t *d, uint8_t c, size_t n)
{
    for (size_t i=0;i<n;i++) write8(d+i,c);
}

void DisplayBufferModeNormal(uint8_t *source, uint8_t *dest) { bytes_copy(dest,source,64000); }
void N_CLRSCR(uint8_t *buffer) { bytes_fill(buffer,0,64000); }
void ClearDrawAndDisplayBufferNormal(uint8_t *draw, uint8_t *display)
{
    N_CLRSCR(draw); N_CLRSCR(display);
}
void DrawWldPointPlan2Normal(uint8_t *buffer, int32_t x, int32_t y)
{
    uint8_t *d=buffer+(ptrdiff_t)y*384+x;
    write16(d+1,0x0c00); write32(d+384,0x0c0e0a00); write32(d+768,0x000a0900); write16(d+1153,0);
}
void DrawBlackBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width)
{
    uint8_t *d=buffer+(ptrdiff_t)y*320+x;
#ifdef LOWLEVEL_NORMAL_MUTATE
    width--;
#endif
    for (int32_t row=0;row<height;row++) bytes_fill(d+(ptrdiff_t)row*320,0,(size_t)width);
}
void DrawBorderBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width, uint16_t colors)
{
    uint8_t *d=buffer+(ptrdiff_t)y*320+x;
    uint8_t light=(uint8_t)colors, dark=(uint8_t)(colors>>8);
    for (int32_t row=0;row<height;row++) write8(d+(ptrdiff_t)row*320,light);
    bytes_fill(d+(ptrdiff_t)(height-1)*320+1,dark,(size_t)(width-1));
    bytes_fill(d+(ptrdiff_t)height*320,dark,(size_t)width);
    bytes_fill(d,light,(size_t)width);
    for (int32_t row=1;row<height;row++) write8(d+(ptrdiff_t)row*320+width-1,dark);
    for (int32_t row=0;row<height;row++) write8(d+(ptrdiff_t)row*320+width,dark);
}
void DrawFondBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width, int32_t type)
{
    int back=type==2, step=back?-1:1;
    ptrdiff_t at=(ptrdiff_t)y*320+x;
    if (back) at+=(ptrdiff_t)(height-1)*320+width-1;
    for (int32_t row=0;row<height;row++) {
        for (int32_t col=0;col<width;col++) {
            uint8_t v=buffer[at+(back?-321:321)], out=(uint8_t)(v+(back?5:-5));
            uint8_t cross=(v^out)&224;
            if (cross) out=(uint8_t)(cross|(back?31:0));
            write8(buffer+at,out); at+=step;
        }
        at+=(ptrdiff_t)step*(320-width);
    }
}
void DisplayAnyPictureNormal(uint8_t *source, uint8_t *dest, int32_t sx, int32_t sy,
                             int32_t dx, int32_t dy, int32_t stride, int32_t width, int32_t height)
{
    for (int32_t y=0;y<height;y++)
        bytes_copy(dest+(ptrdiff_t)(dy+y)*320+dx, source+(ptrdiff_t)(sy+y)*stride+sx,(size_t)width);
}
void lowlevel_normal_set_strides(uint32_t source_pitch, uint32_t display_pitch)
{
    SET(normal_strides[0],source_pitch); SET(normal_strides[1],display_pitch);
}
void Swap_To_Screen(uint8_t *source, uint8_t *dest, int32_t width, int32_t height)
{
    size_t n=((uint32_t)width/4)*4;
    for (int32_t y=0;y<height;y++)
        bytes_copy(dest+(size_t)y*normal_strides[1],source+(size_t)y*normal_strides[0],n);
    port_vga_display_changed();
}
void clear_borders_Normal(uint8_t *dest, int32_t height, int32_t right)
{
    for (int32_t y=0;y<(height/2)*2;y++) {
        write32(dest+(ptrdiff_t)y*320,0); write32(dest+(ptrdiff_t)y*320+right,0);
    }
}
void Copy_Plan0Diff_To_Buf(uint8_t *source, uint8_t *dest, int32_t width, int32_t height, int32_t source_pitch)
{
    if (width<=0 || height<=0) return;
    size_t n=((uint32_t)width+7)/8*8;
    for (int32_t y=0;y<height;y++) bytes_copy(dest+(ptrdiff_t)y*320,source+(ptrdiff_t)y*source_pitch,n);
}
void asm_04d55(uint8_t *source, uint8_t *dest, int32_t width, int32_t height, int32_t source_pitch)
{
    if (width<=0 || height<=0) return;
    size_t n=((uint32_t)width+7)/8;
    for (int32_t by=0;by<height;by+=8)
        for (size_t bx=0;bx<n;bx++)
            for (int32_t y=0;y<8;y++)
                bytes_copy(dest+(ptrdiff_t)(by+y)*320+8*bx,source+(ptrdiff_t)(by+y)*source_pitch+8*bx,8);
}

/* ASM_SPEC Display_Bloc[_Clippe]: flags are MSB-first row pairs. */
static void tile_row(const uint8_t *source, uint8_t *dest, unsigned n, unsigned kind)
{
    if (kind&1) return;
    if (kind&2) bytes_copy(dest,source,n);
    else for (unsigned i=0;i<n;i++) write8(dest+i,(dest[i]&source[256+i])|source[i]);
}
void Display_Bloc(uint8_t *source, uint8_t *dest)
{
    uint32_t flags=read32(source+512);
    for (unsigned y=0;y<16;y++) tile_row(source+16*y,dest+(size_t)y*normal_strides[0],16,(flags>>(30-2*y))&3);
}
void Display_Bloc_Plein(uint8_t *source, uint8_t *dest)
{
    for (unsigned y=0;y<16;y++) bytes_copy(dest+320*y,source+16*y,16);
}
static void clipped_tile(uint8_t *source, uint8_t *dest, int32_t x, int32_t y, int masked)
{
    unsigned n=16,rows=16,skipx=0,skipy=0;
    uint32_t flags=masked?read32(source+512):0;
    SET(tile_original_width,16);
    if (x<Bloc_lim_W1) {
        int64_t distance=(int64_t)Bloc_lim_W1-x;
        if (distance>=16) return;
        skipx=(unsigned)distance&0xfffc; n=(16-skipx)/4*4;
    } else if ((int64_t)x+16>Bloc_lim_W2) {
        if (x>=Bloc_lim_W2) return;
        n=((unsigned)(Bloc_lim_W2-x)+3)/4*4;
    }
    if (y<Bloc_lim_H1) {
        if ((int64_t)y+16<=Bloc_lim_H1) return;
        skipy=(unsigned)(Bloc_lim_H1-y); rows-=skipy;
        flags<<=2*skipy;
    } else if ((int64_t)y+16>Bloc_lim_H2) {
        if (y>=Bloc_lim_H2) return;
        rows=(unsigned)(Bloc_lim_H2-y);
    }
    for (unsigned i=0;i<rows;i++) tile_row(source+16*(skipy+i)+skipx,
        dest+(size_t)(skipy+i)*normal_strides[0]+skipx,n,masked?(flags>>(30-2*i))&3:2);
}
void Display_Bloc_Clippe(uint8_t *source, uint8_t *dest, int32_t x, int32_t y) { clipped_tile(source,dest,x,y,1); }
void Display_Bloc_Plein_Clippe(uint8_t *source, uint8_t *dest, int32_t x, int32_t y) { clipped_tile(source,dest,x,y,0); }
void fplot_Normal(uint8_t *dest, int32_t x, int32_t y, int32_t color)
{
#ifdef LOWLEVEL_NORMAL_MUTATE
    color^=1;
#endif
    write8(dest+(ptrdiff_t)y*320+x,(uint8_t)color);
}
void flocon_clipping(int32_t top, int32_t bottom, int32_t left, int32_t right)
{
    SET(weather_bounds[0],top); SET(weather_bounds[1],bottom);
    SET(weather_bounds[2],left); SET(weather_bounds[3],right);
}
static void weather(uint8_t *dest, int32_t x, int32_t y, const char *pattern, unsigned width, unsigned height)
{
    if (x<weather_bounds[2] || (uint32_t)x>(uint32_t)weather_bounds[3] ||
        y<weather_bounds[0] || (uint32_t)y>(uint32_t)weather_bounds[1]) return;
    for (unsigned row=0;row<height;row++) for (unsigned col=0;col<width;col++) {
        char c=pattern[row*width+col];
        if (c!='.') write8(dest+((ptrdiff_t)y+row)*320+x+col,(uint8_t)(c-'0'));
    }
}
#define WEATHER(name,pattern,w,h) void name(uint8_t *d,int32_t x,int32_t y) { weather(d,x,y,pattern,w,h); }
WEATHER(draw_flocon1_Normal,"7",1,1)
WEATHER(draw_flocon2_Normal,"7867",2,2)
WEATHER(draw_flocon3_Normal,".6.866.6.",3,3)
WEATHER(draw_flocon4_Normal,"3",1,1)
WEATHER(draw_flocon5_Normal,"5",1,1)
WEATHER(draw_flocon6_Normal,"..66...7887.688886688886.7887...66..",6,6)
WEATHER(draw_flocon7_Normal,".66.68866886.66.",4,4)
WEATHER(draw_normal_5755,".55.57755775.55.",4,4)
WEATHER(draw_pluie5_Normal,".45.",2,2)
WEATHER(draw_pluie6_Normal,".4253236.",3,3)
WEATHER(draw_pluie7_Normal,"..32.324543.46..",4,4)

static int32_t clamp(int32_t v,int32_t upper) { return v<0?0:v>upper?upper:v; }
void sprite_clipping(int32_t xmin,int32_t xmax,int32_t ymin,int32_t ymax)
{
    SET(sprite_box[0],clamp(xmin,320)); SET(sprite_box[1],clamp(xmax,320));
#ifdef LOWLEVEL_NORMAL_MUTATE
    SET(sprite_box[0],sprite_box[0]^1);
#endif
    SET(sprite_box[2],clamp(ymin,200)); SET(sprite_box[3],clamp(ymax,200));
}
void default_sprite_clipping(void)
{
    SET(sprite_box[0],8); SET(sprite_box[1],312); SET(sprite_box[2],0); SET(sprite_box[3],200);
}
void get_sprite_clipping(int32_t *xmin,int32_t *xmax,int32_t *ymin,int32_t *ymax)
{
    SET(*xmin,sprite_box[0]); SET(*xmax,sprite_box[1]); SET(*ymin,sprite_box[2]); SET(*ymax,sprite_box[3]);
}
int32_t get_sprite_clipping_xmin(void) { return sprite_box[0]; }
int32_t get_sprite_clipping_xmax(void) { return sprite_box[1]; }
int32_t get_sprite_clipping_ymin(void) { return sprite_box[2]; }
int32_t get_sprite_clipping_ymax(void) { return sprite_box[3]; }

/* ASM_SPEC clip_sprite_on_screen: upper comparisons deliberately unsigned. */
static int clip_sprite(lowlevel_sprite_clip *s,int flip,int diff,uint8_t **mask)
{
    unsigned w=s->size&255,h=(s->size>>8)&255;
    uint32_t end=w;
    if (diff) SET(diff_sprite_width,w); else SET(saved_sprite_width,w);
    int cropped_left=s->x<sprite_box[0];
    if (cropped_left) {
        uint32_t distance=(uint32_t)sprite_box[0]-(uint32_t)s->x;
        if (!diff) s->x=signed32(distance);
        if (w<=distance) return 0;
        distance&=diff?0xfffc:0xffff;
        end=w-distance; s->size=(s->size&~255u)|((w-distance)&255);
        if (diff) {
            s->x=signed32((uint32_t)s->x+distance); s->source+=distance; *mask+=distance;
        } else {
            if (!flip) s->source+=distance;
            s->x=sprite_box[0];
        }
        w=s->size&255;
    }
    end=(diff?w:end)+(uint32_t)s->x;
    if (diff && cropped_left) s->size=(s->size&~255u)|((s->size&255)/4);
    else if (end>(uint32_t)sprite_box[1]) {
        if ((uint32_t)s->x>=(uint32_t)sprite_box[1]) return 0;
        end=(uint32_t)sprite_box[1]-(uint32_t)s->x;
        s->size=(s->size&~255u)|(end&255);
        if (flip) s->source+=saved_sprite_width-end;
        if (diff) s->size=(s->size&~255u)|(((s->size&255)+3)/4);
    } else if (diff) s->size=(s->size&~255u)|((s->size&255)/4);
    end=(s->size>>8)&255;
    if (s->y<sprite_box[2]) {
        end+=(uint32_t)s->y;
        if (signed32(end)<=sprite_box[2]) return 0;
        unsigned skip=((uint32_t)sprite_box[2]-(uint32_t)s->y)&65535;
        s->size=(s->size&~0xff00u)|(((h-skip)&255)<<8);
        size_t advance=(size_t)(diff?diff_sprite_width:saved_sprite_width)*skip;
        s->source+=advance; if (diff) *mask+=advance;
        s->y=sprite_box[2]; end=diff?0:(s->size>>8)&255;
    }
    end+=(uint32_t)s->y;
    if (end>(uint32_t)sprite_box[3]) {
        if ((uint32_t)s->y>=(uint32_t)sprite_box[3]) return 0;
        end=(uint32_t)sprite_box[3]-(uint32_t)s->y;
        s->size=(s->size&~0xff00u)|((end&255)<<8);
    }
    return 1;
}
int clip_sprite_on_screen(lowlevel_sprite_clip *s) { return clip_sprite(s,0,0,NULL); }
int clip_sprite_on_screen_flipped(lowlevel_sprite_clip *s) { return clip_sprite(s,1,0,NULL); }
static void sprite(int32_t x,int32_t color,int32_t y,uint32_t size,uint8_t *dest,uint8_t *source,
                   int flip,int raw,int colored,int noclip)
{
    lowlevel_sprite_clip s={x,y,size,source};
    if (colored) SET(saved_sprite_color,(uint32_t)color*8);
    if (noclip) SET(saved_sprite_width,size&255);
    else if (!clip_sprite(&s,flip,0,NULL) || !(s.size&255)) return;
    unsigned w=s.size&255,h=(s.size>>8)&255;
    for (unsigned row=0;row<h;row++) {
        uint8_t *d=dest+((ptrdiff_t)s.y+row)*320+s.x;
        const uint8_t *src=s.source+(size_t)row*saved_sprite_width;
        for (unsigned i=0;i<w;) {
            uint8_t value=src[i];
            if (raw) {
                if (value) write8(d+(flip?w-1-i:i),value);
                i++;
            } else if (value<160) {
                write8(d+(flip?w-1-i:i),value|(colored?(uint8_t)saved_sprite_color:0)); i++;
            } else i+=value-160;
        }
    }
}
#define SPRITE(name,flip,raw,color,noclip) \
void name(int32_t x,int32_t c,int32_t y,uint32_t size,uint8_t *d,uint8_t *s) { sprite(x,c,y,size,d,s,flip,raw,color,noclip); }
SPRITE(DrawSpriteNormalNoClip,0,0,0,1)
SPRITE(DrawSpriteNormal,0,0,0,0)
SPRITE(DrawSpriteNormal256,0,1,0,0)
SPRITE(DrawSpriteFlipNormalNoClip,1,0,0,1)
SPRITE(DrawSpriteFlipNormal,1,0,0,0)
SPRITE(DrawSpriteFlipNormal256,1,1,0,0)
SPRITE(DrawSpriteColorNormal,0,0,1,0)
SPRITE(DrawSpriteColorFlipNormal,1,0,1,0)
void DrawSpriteDiffNormal(int16_t x,int16_t y,uint16_t size,uint8_t *mask,uint8_t *dest,uint8_t *source)
{
    lowlevel_sprite_clip s={x,y,size,source};
    if (!clip_sprite(&s,0,1,&mask) || !(s.size&255)) return;
    unsigned n=s.size&255,h=(s.size>>8)&255;
    for (unsigned row=0;row<h;row++) for (unsigned i=0;i<n;i++) {
        const uint8_t *src=s.source+(size_t)row*diff_sprite_width+4*i;
        uint32_t image=read32(src);
        if (image) {
            uint8_t *d=dest+((ptrdiff_t)s.y+row)*320+s.x+4*i;
            write32(d,(read32(d)&read32(mask+(size_t)row*diff_sprite_width+4*i))|image);
        }
    }
}
void lowlevel_copy_dwords_direction(uint8_t *source,uint8_t *dest,uint32_t count,int direction)
{
    for (uint32_t i=0;i<count;i++) {
        ptrdiff_t off=(ptrdiff_t)i*4*(direction<0?-1:1);
        write32(dest+off,read32(source+off));
    }
}
void copy_dwords(uint32_t *source,uint32_t *dest,int32_t count)
{
    lowlevel_copy_dwords_direction((uint8_t *)source,(uint8_t *)dest,(uint32_t)count,1);
}

/* ASM_SPEC draw_sprite_deform, 1:04828: indices avoid a source-minus-one C pointer. */
void draw_sprite_deform(uint8_t *source,uint8_t *dest,deform_blit_t *g)
{
    int32_t p[13]; memcpy(p,g,sizeof p);
    int32_t A=p[0],B=p[1],C=p[2],D=p[3],E=p[4],F=p[5],G=p[6],H=p[7];
    uint32_t sw=(uint32_t)p[11],sh=(uint32_t)p[12];
    SET(deform_source,source);
    bytes_fill(dest,161,(size_t)(4*((p[8]>>2)+1))*p[9]);
    for (unsigned i=0;i<8;i++) SET(deform_steps[i],p[i]);
    uint32_t rowstep=(sh<<16)/(uint32_t)(G+H);
    SET(deform_denominator,(uint32_t)(G+H)); SET(deform_source_width,sw); SET(deform_row_step,rowstep);
    int descending=C>D;
    uint32_t limit=(uint32_t)(descending?C:D),goal=(uint32_t)(descending?H:G);
    uint32_t step=(sw<<16)/limit;
    SET(deform_inner_step,step); SET(deform_base_step,step);
    uint32_t outer_error=0,outer_count=0,sample_y=0,innerstep=step;
    ptrdiff_t row_dest=p[10]; size_t row_source=0;
    for (;;) {
        uint32_t error=0,sample_x=0; ptrdiff_t d=row_dest;
        for (uint32_t k=0;k<limit;k++) {
            uint8_t value=source[row_source+(sample_x>>16)];
            if (dest[d]==(descending?161:129)) write8(dest+d,value);
            sample_x+=innerstep; error+=(uint32_t)(descending?D:C);
            if (k+1<limit) {
                d+=descending?A:B;
                if (descending?error>(uint32_t)C:signed32(error)>0) { d+=descending?B:A; error-=(uint32_t)(descending?C:D); }
            }
        }
        outer_count++; outer_error+=(uint32_t)(descending?G:H);
        row_dest+=descending?F:E; sample_y+=rowstep;
        row_source=(size_t)sw*(sample_y>>16); SET(deform_inner_step,step); innerstep=step;
        if (outer_count>=goal) break;
        if (descending?signed32(outer_error)>0:signed32(outer_error)>G) {
            row_dest+=descending?E-F:F-E; outer_count--; outer_error-=(uint32_t)(G+H);
            innerstep=step-64; SET(deform_inner_step,innerstep);
        }
    }
}
void local_04a83(uint8_t *source,uint8_t *dest)
{
    for (unsigned y=0;y<200;y++) for (unsigned x=0;x<320;x++)
        write8(dest+(x&3)*16000+x/4+y*80,source[y*320+x]);
}

void effect_reset_geometry(void)
{
    for (unsigned i=0;i<80;i++) SET(raster_span_displacements[i],-320+4*(int32_t)i);
    for (unsigned y=0;y<200;y++) { write32(raster_rows[y],65535); write32(raster_rows[y]+4,0); }
}
static uint32_t replace_low(uint32_t v,uint32_t x) { return (v&0xffffff00u)|(x&255); }
static uint32_t replace_high(uint32_t v,uint32_t x) { return (v&0xffff00ffu)|((x&255)<<8); }
/* Floor signed Q16 without implementation-defined right shift. */
static int64_t floor_q16(int64_t v) { return v>=0?v/65536:-1-((-1-v)/65536); }
static void span_setup(const uint8_t *row,uint32_t width,uint32_t *a,uint32_t *d,uint32_t *b)
{
    uint32_t u0=read32(row+8),u1=read32(row+12),v0=read32(row+16),v1=read32(row+20);
    int64_t divisor=signed32(width<<16);
    uint32_t qv=(uint32_t)(((int64_t)signed32(v1-v0+1)*INT64_C(4294967296))/divisor);
    uint32_t qu=(uint32_t)(((int64_t)signed32(u1-u0+1)*INT64_C(4294967296))/divisor);
    *a=((qv&65535)<<16)|(qu>>16); *d=((qu&65535)<<16)|((qv>>16)&255); *b=((v0<<8)+u0)&65535;
}
/* ASM_SPEC draw_effect_segment, 1:00045: modular coupled UV accumulators. */
void draw_effect_segment(uint8_t *dest,uint8_t *descriptor)
{
    uint8_t *texture; int32_t verts[8]; uint8_t uv[8];
    memcpy(&texture,descriptor,sizeof texture);
    memcpy(verts,descriptor+sizeof texture,sizeof verts); memcpy(uv,descriptor+sizeof texture+sizeof verts,sizeof uv);
    uint32_t du=(uint8_t)(uv[6]-uv[0]),dv=(uint8_t)(uv[3]-uv[1]);
    SET(raster_destination,dest); SET(raster_descriptor,descriptor+sizeof texture); SET(raster_texture,texture);
    SET(raster_du,du); SET(raster_neg_du,0u-du); SET(raster_dv,dv); SET(raster_neg_dv,0u-dv);
    int32_t miny=verts[1],maxy=verts[1];
    for (unsigned i=1;i<4;i++) { if (verts[2*i+1]<miny) miny=verts[2*i+1]; if (verts[2*i+1]>maxy) maxy=verts[2*i+1]; }
    if (miny>=200 || maxy<0) return;
    int32_t first=miny<0?0:miny,last=maxy>199?199:maxy;
    SET(raster_first_y,(uint32_t)first); SET(raster_rows_left,(uint32_t)(last-first+1));
    const unsigned ends[4][3]={{0,3,0},{3,2,1},{2,1,0},{1,0,1}};
    for (unsigned edge=0;edge<4;edge++) {
        unsigned i=ends[edge][0],j=ends[edge][1],axis=ends[edge][2];
        int32_t xa=verts[2*j],ya=verts[2*j+1],xb=verts[2*i],yb=verts[2*i+1];
        int32_t ta=uv[2*j+axis],constant=uv[2*j+1-axis];
        int32_t delta=edge==0?-(int32_t)du:edge==1?-(int32_t)dv:edge==2?(int32_t)du:(int32_t)dv;
        if ((ya<0 && yb<0) || (ya>=200 && yb>=200)) continue;
        if (yb<ya) {
            int32_t t=xa;xa=xb;xb=t; t=ya;ya=yb;yb=t;
            ta=uv[2*i+axis]; delta=-delta;
        }
        int64_t count=(int64_t)yb-ya+1,dx=(int64_t)xb-xa;
        int64_t qx=(dx+(dx>=0?1:-1))*65536/count,qt=(int64_t)delta*65536/count;
        for (unsigned k=0;k<2;k++) SET(raster_edge_constant[edge][k],(uint8_t)constant);
        for (unsigned k=0;k<2;k++) SET(raster_edge_texture_fraction[edge][k],(uint16_t)qt);
        for (unsigned k=0;k<2;k++) SET(raster_edge_x_fraction[edge][k],(uint16_t)qx);
        for (int32_t y=ya<0?0:ya;y<=(yb>199?199:yb);y++) {
            int64_t n=(int64_t)y-ya;
            uint16_t biased=(uint16_t)floor_q16(((int64_t)xa+32000)*65536+qx*n);
            uint8_t t=(uint8_t)floor_q16((int64_t)ta*65536+qt*n),uu=axis==0?t:(uint8_t)constant,vv=axis==0?(uint8_t)constant:t;
            uint8_t *row=raster_rows[y];
            if (read16(row)>biased) { write16(row,biased); write8(row+8,uu); write8(row+16,vv); }
            if (read16(row+4)<biased) { write16(row+4,biased); write8(row+12,uu); write8(row+20,vv); }
        }
    }
    SET(raster_scanline,dest+(ptrdiff_t)first*320); SET(raster_row_offset,(uint32_t)first*32);
    for (int32_t y=first;y<=last;y++) {
        uint8_t *row=raster_rows[y]; uint32_t left=read32(row),right=read32(row+4);
        write16(row,65535); write16(row+4,0);
        if (left<32320 && right>=32000) {
            uint32_t width=right-left+1,a,d,b,fraction=0;
            span_setup(row,width,&a,&d,&b); int32_t x=(int32_t)left-32000;
            if (x<0) {
                uint32_t skip=(uint32_t)-x; width-=skip;
                uint64_t product=(uint64_t)a*skip,total=(uint64_t)b+(uint32_t)product;
                uint32_t carry=(uint32_t)(total>>32); b=(uint32_t)total;
                b=replace_high(b,(b>>8)+(uint8_t)(product>>32)+carry);
                product=(uint64_t)d*skip;
                b=replace_low(b,(b&255)+(uint8_t)(product>>32)); b=replace_high(b,(b>>8)+(uint8_t)product);
                fraction=(uint32_t)product; x=0;
            }
            if (width>(uint32_t)(320-x)) width=(uint32_t)(320-x);
            for (uint32_t col=0;col<width;col++) {
                write8(dest+(ptrdiff_t)y*320+x+col,texture[b&65535]);
                uint64_t total=(uint64_t)fraction+d; fraction=(uint32_t)total;
                total=(uint64_t)b+a+(total>>32); b=(uint32_t)total;
                b=replace_high(b,(b>>8)+(d&255)+(uint32_t)(total>>32));
            }
        }
        SET(raster_scanline,dest+(ptrdiff_t)(y+1)*320); SET(raster_row_offset,(uint32_t)(y+1)*32); SET(raster_rows_left,(uint32_t)(last-y));
    }
}

typedef struct state_region { void *address; size_t size; unsigned kind; } state_region;
#define REGION(v,k) { &(v),sizeof(v),k }
static state_region state_regions[]={
    REGION(sprite_box,1), REGION(saved_sprite_width,1), REGION(saved_sprite_color,1), REGION(diff_sprite_width,1),
    REGION(normal_strides,1), REGION(weather_bounds,1), REGION(tile_original_width,1), REGION(deform_steps,1),
    REGION(deform_source,2), REGION(deform_base_step,1), REGION(deform_inner_step,1), REGION(deform_row_step,1),
    REGION(deform_source_width,1), REGION(deform_denominator,1), REGION(raster_rows,0), REGION(raster_span_displacements,1),
#define EDGE(e) REGION(raster_edge_constant[e][0],0),REGION(raster_edge_constant[e][1],0), \
    REGION(raster_edge_texture_fraction[e][0],0),REGION(raster_edge_texture_fraction[e][1],0), \
    REGION(raster_edge_x_fraction[e][0],0),REGION(raster_edge_x_fraction[e][1],0)
    EDGE(0),EDGE(1),EDGE(2),EDGE(3),
    REGION(raster_destination,2),REGION(raster_descriptor,2),REGION(raster_texture,2),REGION(raster_du,1),REGION(raster_neg_du,1),
    REGION(raster_dv,1),REGION(raster_neg_dv,1),REGION(raster_first_y,1),REGION(raster_rows_left,1),
    REGION(raster_scanline,2),REGION(raster_row_offset,1)
};
void *lowlevel_normal_state_region(unsigned id,size_t *size,unsigned *kind)
{
    if (id>=sizeof(state_regions)/sizeof(state_regions[0])) return NULL;
    *size=state_regions[id].size; *kind=state_regions[id].kind; return state_regions[id].address;
}
unsigned lowlevel_normal_state_region_count(void) { return sizeof(state_regions)/sizeof(state_regions[0]); }
