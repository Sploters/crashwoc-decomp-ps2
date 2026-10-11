/*
 * Unit: game/glass
 *
 * Functions:
 *   0x00238bf8 InitGlass
 *   0x00239190 GlassGivenMaterial
 *   0x00239540 ProcessGlass
 *   0x002396b8 DoGlass
 *   0x002398f0 InitSpecular
 *   0x00239960 InitEnviro
 *   0x002399d0 SpecularGivenMaterial
 *   0x00239b38 EnviroGivenMaterial
 *   0x00239ca8 InitFlickerPal
 *   0x00239cb0 FlickerPalGivenMaterial
 *   0x00239cb8 DrawGlassCreatures
 */


#ifndef NULL
#define NULL ((void *)0)
#endif
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;


void FlickerPalGivenMaterial(void) {

}





void InitFlickerPal(void) {

}



s32 DrawCreatures(s32 *, s32, s32, s32);
extern s32 Character[];
extern s32 D_0057FB1C[];
extern s32 GLASSPLAYER;
extern s32 Level;
extern s32 glass_draw;
extern s32 glass_phase;
extern f32 plr_invisibility_time;

void DrawGlassCreatures(s32 arg0) {
    if (Level != 0x17) {
        if (GLASSPLAYER != 0) {
            if (plr_invisibility_time < 5.0f) {
                glass_draw = arg0;
                glass_phase = 1;
                DrawCreatures(&Character[0], 1, 1, 0);
            }
        }
    } else {
        glass_draw = arg0;
        glass_phase = 1;
        DrawCreatures(&D_0057FB1C[0], 8, 1, 1);
    }
}



s32 NuMemFreeFn(void *, s32 *, s32);
s32 NuTexCreate(void *);
void *NuTexReadBitmap(s32 *);
extern s32 D_00620740[];
extern s32 D_00620750[];
extern s32 D_00631E88;

void InitEnviro(void)
{
  void *temp_a0;
  s32 new_var;
  void *temp_v0;
  D_00631E88 = 0;
  temp_v0 = NuTexReadBitmap(&D_00620750[0]);
  if (temp_v0 != ((void *) 0))
  {
    new_var = NuTexCreate(temp_v0);
    temp_a0 = *((void **) (((s8 *) temp_v0) + 0x10));
    D_00631E88 = new_var;
    if (temp_a0 != ((void *) 0))
    {
      NuMemFreeFn(temp_a0, &D_00620740[0], 0xA2);
    }
    NuMemFreeFn(temp_v0, &D_00620740[0], 0xA3);
  }
}



s32 NuMemFreeFn(void *, s32 *, s32);
s32 NuTexCreate(void *);
void *NuTexReadBitmap(s32 *);
extern s32 D_00620728[];
extern s32 D_00620740[];
extern s32 D_00631E84;

void InitSpecular(void)
{
  void *temp_a0;
  s32 new_var;
  void *temp_v0;
  D_00631E84 = 0;
  temp_v0 = NuTexReadBitmap(&D_00620728[0]);
  if (temp_v0 != ((void *) 0))
  {
    new_var = NuTexCreate(temp_v0);
    temp_a0 = *((void **) (((s8 *) temp_v0) + 0x10));
    D_00631E84 = new_var;
    if (temp_a0 != ((void *) 0))
    {
      NuMemFreeFn(temp_a0, &D_00620740[0], 0x94);
    }
    NuMemFreeFn(temp_v0, &D_00620740[0], 0x95);
  }
}
