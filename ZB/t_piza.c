#include <brender.h>
#include <limits.h>
#include "fwproto.h"
#include "zb.h"

#define add_carry(x, y) (x += y, (br_uint_32)x < (br_uint_32)y)

#define scan_inc(carry, wrap)                                                                                          \
    {                                                                                                                  \
        zb.pz.current += zb.pz.d_##carry;                                                                              \
        if (use_light)                                                                                                 \
            zb.pi.current += zb.pi.d_##carry;                                                                          \
        if (add_carry(zb.awsl.u_current, zb.awsl.du_##carry))                                                          \
        {                                                                                                              \
            zb.awsl.source_current += bpp;                                                                             \
            if (wrap)                                                                                                  \
                zb.awsl.u_int_current++;                                                                               \
        }                                                                                                              \
        zb.awsl.source_current += zb.awsl.dsource_##carry;                                                             \
        if (wrap)                                                                                                      \
            zb.awsl.u_int_current += zb.awsl.du_int_##carry;                                                           \
        if (add_carry(zb.awsl.v_current, zb.awsl.dv_##carry))                                                          \
            zb.awsl.source_current += zb.awsl.texture_stride;                                                          \
    }

br_size_t noffset;

static inline void __TriangleRenderZ2(br_boolean use_light, br_boolean use_bump, br_boolean use_transparency,
                                      br_uint_32 bpp)
{
    br_boolean is_forward;
    br_uint_16 *zptr;
    br_uint_8 *ptr, *src;
    br_uint_32 u, v, ecx, edx;

    while (zb.awsl.edge->count > 0)
    {
        zb.awsl.edge->count--;

        if (use_bump)
        {
            noffset = zb.bump_buffer - zb.texture_buffer;
        }

        is_forward = zb.awsl.start < zb.awsl.end;
        ptr = zb.awsl.start;
        u = (zb.awsl.u_current & 0xFFFF0000) | zb.awsl.u_int_current;
        v = zb.awsl.v_current;

        zb.pz.currentpix = zb.pz.current;

        if (use_light)
        {
            zb.pi.currentpix = zb.pi.current;
        }

        src = zb.awsl.source_current;
        zptr = (unsigned short *)zb.awsl.zstart;

        // pixel loop
        if (is_forward)
        {
            while (ptr < zb.awsl.end)
            {
                if (use_transparency && *src == 0)
                    goto failf;

                // 16 bit z test

                if (*zptr < BrFixedToInt(zb.pz.currentpix))
                    goto failf;

                if (use_light)
                {
                    *zptr = BrFixedToInt(zb.pz.currentpix);
                    zptr++;
                    for (int i = 0; i < bpp; i++)
                    {
                        *ptr = zb.shade_table[*src | ((BrFixedToInt(zb.pi.currentpix) & 0xFF) << 8)];
                    }
                    zb.pz.currentpix += zb.pz.grad_x;
                    ptr += bpp;
                    zb.pi.currentpix += zb.pi.grad_x;
                    goto contf;
                }
                else
                {
                    *zptr = BrFixedToInt(zb.pz.currentpix);
                    for (int i = 0; i < bpp; i++)
                    {
                        ptr[i] = src[bpp - i - 1];
                    }
                }

            failf:
                if (use_light)
                {
                    zptr++;
                    ptr += bpp;
                    ecx = zb.pz.currentpix;
                    edx = zb.pz.grad_x;
                    ecx += edx;
                    edx = zb.pi.currentpix;
                    zb.pz.currentpix = ecx;
                    ecx = zb.pi.grad_x;
                    edx += ecx;
                    zb.pi.currentpix = edx;
                }
                else
                {
                    ecx = zb.pz.currentpix;
                    zptr++;
                    edx = zb.pz.grad_x;
                    ecx += edx;
                    ptr += bpp;
                    zb.pz.currentpix = ecx;
                }

            contf:
                if (add_carry(u, zb.awsl.du))
                {
                    u++;
                    src += bpp;
                }

                src += zb.awsl.dsource;
                u += zb.awsl.du_int;

                if (add_carry(v, zb.awsl.dv))
                {
                    src += zb.awsl.texture_stride;
                }

                if ((u & 0x8000) == 0)
                {
                    u += zb.awsl.texture_width;
                    src += zb.awsl.texture_stride;
                }

                // Have we underrun?
                if (src < zb.awsl.texture_start)
                {
                    src += zb.awsl.texture_size;
                }
            }
        }
        else
        {
            while (ptr > zb.awsl.end)
            {
                zb.pz.currentpix -= zb.pz.grad_x;

                if (use_light)
                    zb.pi.currentpix -= zb.pi.grad_x;

                if (add_carry(u, zb.awsl.du))
                {
                    u++;
                    src += bpp;
                }

                src += zb.awsl.dsource;
                u += zb.awsl.du_int;

                if (add_carry(v, zb.awsl.dv))
                {
                    src += zb.awsl.texture_stride;
                }

                if ((u & 0x8000) == 0)
                {
                    u += zb.awsl.texture_width;
                    src += zb.awsl.texture_stride;
                }

            nowidthb:
                if (src < zb.awsl.texture_start) // Have we underrun?
                    src += zb.awsl.texture_size;

            nosizeb:
                zptr--;
                ptr -= bpp;

                if (use_transparency && *src == 0)
                    continue;

                // Skip if pixel failed z test
                if (*zptr < BrFixedToInt(zb.pz.currentpix))
                    continue;

                *zptr = BrFixedToInt(zb.pz.currentpix);
                if (use_light)
                {
                    *ptr = zb.shade_table[*src | ((BrFixedToInt(zb.pi.currentpix) & 0xFF) << 8)];
                }
                else
                {
                    for (int i = 0; i < bpp; i++)
                    {
                        ptr[i] = src[bpp - i - 1];
                    }
                }

                if (use_bump)
                {
                    edx = noffset;
                    edx = (edx & 0xFFFFFF00) | src[edx];
                    edx &= 0xFF;
                    ecx = *ptr;
                    ecx |= (br_uint_32)zb.lighting_table[edx] << 8; // Calculate light level from normal
                    ecx |= zb.shade_table[ecx];                     // Light texel
                    for (int i = 0; i < bpp; i++)
                    {
                        *ptr = (ecx & 0xFF);
                    }
                }
            }
        }

    next:
        // Per scan line updates
        br_uint_32 carry;

        carry = add_carry(zb.awsl.edge->f, zb.awsl.edge->d_f);
        zb.awsl.end += bpp * (zb.awsl.edge->d_i + carry);
        carry = add_carry(zb.main.f, zb.main.d_f);

        zptr = (unsigned short *)zb.awsl.zstart;
        zptr += zb.main.d_i + carry;
        zb.awsl.zstart = (br_fixed_ls *)zptr;
        zb.awsl.start += bpp * (zb.main.d_i + carry);

        if (carry)
        {
            scan_inc(carry, 1);
        }
        else
        {
            scan_inc(nocarry, 1);
        }

    cont:
        if (zb.awsl.u_int_current >= 0)
        {
            zb.awsl.u_int_current += zb.awsl.texture_width;
            zb.awsl.source_current += zb.awsl.texture_stride;
        }

    lab1:
        if (zb.awsl.source_current < zb.awsl.texture_start)
        {
            zb.awsl.source_current += zb.awsl.texture_size;
        }
    }
}

void BR_ASM_CALL TrapezoidRenderPIZ2TA()
{
    __TriangleRenderZ2(BR_FALSE, BR_FALSE, BR_TRUE, 1);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TA_RGB_555()
{
    __TriangleRenderZ2(BR_FALSE, BR_FALSE, BR_TRUE, 2);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TA_RGB_888()
{
    __TriangleRenderZ2(BR_FALSE, BR_FALSE, BR_TRUE, 3);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TAN()
{
    __TriangleRenderZ2(BR_FALSE, BR_TRUE, BR_TRUE, 1);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TAN_RGB_555()
{
    __TriangleRenderZ2(BR_FALSE, BR_TRUE, BR_TRUE, 2);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TAN_RGB_888()
{
    __TriangleRenderZ2(BR_FALSE, BR_TRUE, BR_TRUE, 3);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TIA()
{
    __TriangleRenderZ2(BR_TRUE, BR_FALSE, BR_TRUE, 1);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TIA_RGB_555()
{
    __TriangleRenderZ2(BR_TRUE, BR_FALSE, BR_TRUE, 2);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TIA_RGB_888()
{
    __TriangleRenderZ2(BR_TRUE, BR_FALSE, BR_TRUE, 3);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TIANT()
{
    __TriangleRenderZ2(BR_TRUE, BR_FALSE, BR_FALSE, 1);
}
