/*
 * Unit: game/game_obj
 *
 * Functions:
 *   0x001f9270 CylinderCuboidOverlapXZ
 *   0x001f9558 ObjectCylinderCollision
 *   0x001f99e0 GameObjectOverlap
 *   0x001f9bc0 PlayerCreatureCollisions
 *   0x001fa998 GetTopBot
 *   0x001fab20 CrateCollisions
 *   0x001fbef0 WumpaCollisions
 *   0x001fc100 MoveLoopXZ
 *   0x001fc2e0 GetSurfaceInfo
 *   0x001fc788 ObjectRotation
 *   0x001fce88 PickupItem
 *   0x001fd158 HitCreatures
 *   0x001fd2e0 WipeCreatures
 *   0x001fd4a8 KillGameObject
 *   0x001fe0a0 AddProjectile
 *   0x001feeb0 UpdateProjectiles
 *   0x00200050 DrawProjectiles
 *   0x002005f8 CreatureTopBelow
 *   0x002007b0 CreatureRayCast
 *   0x00200ab0 ClearGameObjects
 *   0x00200ad8 ResetGameObject
 *   0x00200b38 AddGameObject
 *   0x00200c38 RemoveGameObject
 *   0x00200ca8 GameObjectRadius
 *   0x00200d30 CylinderCylinderOverlapXZ
 *   0x00200d80 FlyGameObject
 *   0x00200df8 GetDieAnim
 *   0x00200e90 KillPlayer
 *   0x00200f38 KillItem
 *   0x00200f58 PickupCrystal
 *   0x00200fb0 PickupCrateGem
 *   0x00201008 PickupBonusGem
 *   0x00201068 PickupPower
 *   0x00201140 HitItems
 *   0x00201200 NewTopBot
 *   0x00201230 OldTopBot
 *   0x00201248 FindAnglesZX
 *   0x002012a0 ScaleFlatShadow
 *   0x00201320 ResetProjectiles
 *   0x002013d8 CountGameObjects
 *   0x00201418 PickupRelic
 */

#include "creature.h"

/* Level data record; only flags at +0x24 is touched here. */
struct ldata_s {
    u8 unk_0x00[0x24];       /* 0x00 (opaque) */
    unsigned short flags;    /* 0x24 */
};

extern struct ldata_s *LDATA;

extern f32 vtog_time;
extern f32 vtog_duration;
extern s32 vtog_blend;

void LoseMask(struct obj_s *obj);
s32 KillGameObject(struct obj_s *obj, s32 anim);

extern s32 level_part_2;
extern s32 temp_xzmomset;
extern s32 GAMEOBJECTCOUNT;
extern struct obj_s *pObj[];

/* Projectile record; obj header at 0x0, active flag at 0x1A9, stride 0x1B0. */
struct projectile_s {
    struct obj_s obj;    /* 0x000 */
    u8 unk_0x188[0x21];  /* 0x188 */
    s8 active;           /* 0x1A9 */
    u8 unk_0x1AA[0x6];   /* 0x1AA */
};

extern struct projectile_s Projectile[16];
extern s32 jonglow;
extern s32 jonglowsize;
extern s32 temp_cuboid_side;
extern f32 temp_cuboid_bounce_angle;
extern s32 VEHICLECONTROL;
extern CharacterData CData[191];
extern s32 gamesfx_effect_volume;
extern f32 NuTrigTable[];
extern struct nuvec_s v010;

extern f32 D_0062D7C0;
extern f32 D_0062D7C4;
extern f32 D_0062D7C8;
extern f32 D_0062D7CC;
extern f32 D_0062D7D0;
extern f32 D_0062D7D4;
extern f32 D_0062D7D8;

void NewTopBot(struct obj_s *obj);
s32 GameObjectOverlap(struct obj_s *a, struct obj_s *b, s32 find_angle);
void PickupItem(struct obj_s *obj);
s32 CylinderCuboidOverlapXZ(struct nuvec_s *pos, struct obj_s *obj,
                            struct nuvec_s *p2, f32 radius);
void AddGameDebris(s32 type, struct nuvec_s *pos);
void NuVecScale(struct nuvec_s *dst, struct nuvec_s *src, f32 s);
void NuVecSub(struct nuvec_s *d, struct nuvec_s *a, struct nuvec_s *b);
void GameSfx(s32 sfx, struct nuvec_s *pos);
s32 NuAtan2D(f32 x, f32 z);
f32 NuFsqrt(f32 x);
extern void *memset(void *s, s32 c, s32 n);
void NuVecRotateX(struct nuvec_s *dst, struct nuvec_s *src, s32 angle);
void NuVecRotateY(struct nuvec_s *dst, struct nuvec_s *src, s32 angle);
s32 GetDieAnim(struct obj_s *obj, s32 anim);
void ObjectToAtlas(struct obj_s *obj, struct creature_s *player);

struct plr_lives_s {
    short count;   /* 0x0 */
    short draw;    /* 0x2 */
    u8 frame;      /* 0x4 */
    u8 unk_0x5[2]; /* 0x5 */
    u8 type;       /* 0x7 */
};

extern u16 plr_items;
extern struct plr_lives_s plr_crystal;
extern struct plr_lives_s plr_crategem;
extern struct plr_lives_s plr_bonusgem;
extern f32 D_0062D904;
extern f32 D_0062D908;
extern f32 D_0062D90C;

void AddPanelDebris(f32 x, f32 y, s32 obj, f32 z, s32 flag);

extern s32 new_power;
extern u8 Game[];
extern u8 Cursor[];
extern s32 Level;
extern u16 temp_xrot;
extern u16 temp_zrot;
extern u16 temp_yrot;
s32 qrand(void);
extern s32 new_mode;
extern s32 new_level;
extern u64 LBIT;
extern s32 boss_dead;
void ClockOff(void);
void StartTimeTrial(struct nuvec_s *pos, s32 clock);
void NewMenu(void *cursor, s32 menu, s32 mode, s32 item);


s32 CylinderCuboidOverlapXZ(struct nuvec_s *pos, struct obj_s *obj,
                            struct nuvec_s *p2, f32 radius) {
    struct nuvec_s v;
    f32 minx;
    f32 maxx;
    f32 minz;
    f32 maxz;
    f32 x0;
    f32 x1;
    f32 z0;
    f32 z1;
    f32 cx;
    f32 cz;
    f32 dx;
    f32 dz;
    s32 corner;
    s32 ang;

    NuVecSub(&v, pos, p2);
    NuVecRotateY(&v, &v, -obj->hdg);
    minx = obj->min.x * obj->SCALE;
    x0 = minx - radius;
    if (v.x < x0) {
        return 0;
    }
    maxx = obj->max.x * obj->SCALE;
    x1 = maxx + radius;
    if (v.x > x1) {
        return 0;
    }
    minz = obj->min.z * obj->SCALE;
    z0 = minz - radius;
    if (v.z < z0) {
        return 0;
    }
    maxz = obj->max.z * obj->SCALE;
    z1 = maxz + radius;
    if (v.z > z1) {
        return 0;
    }

    corner = 0;
    if (v.x < minx) {
        if (v.z < minz) {
            corner = 10;
            cx = minx;
            cz = minz;
            temp_cuboid_side = (minz - v.z < minx - v.x) ? 0x20 : 0x40;
        } else if (v.z > maxz) {
            corner = 9;
            cx = minx;
            cz = maxz;
            temp_cuboid_side = (v.z - maxz < minx - v.x) ? 0x10 : 0x40;
        }
    } else if (v.x > maxx) {
        if (v.z < minz) {
            corner = 6;
            cx = maxx;
            cz = minz;
            temp_cuboid_side = (minz - v.z < v.x - maxx) ? 0x20 : 0x80;
        } else if (v.z > maxz) {
            corner = 5;
            cx = maxx;
            cz = maxz;
            temp_cuboid_side = (v.z - maxz < v.x - maxx) ? 0x10 : 0x80;
        }
    }

    if (corner != 0) {
        dx = v.x - cx;
        dz = v.z - cz;
        if (dx * dx + dz * dz > radius * radius) {
            return 0;
        }
        temp_cuboid_bounce_angle = NuAtan2D(dx, dz) + obj->hdg;
        return 1;
    }

    dx = v.x - x0;
    if (x1 - v.x > dx) {
        if (v.z - z0 < z1 - v.z) {
            ang = (v.z - z0 < dx) ? 0x8000 : 0xC000;
        } else {
            ang = (z1 - v.z < dx) ? 0 : 0xC000;
        }
    } else {
        if (v.z - z0 < z1 - v.z) {
            ang = (v.z - z0 < x1 - v.x) ? 0x8000 : 0x4000;
        } else {
            ang = (z1 - v.z < x1 - v.x) ? 0 : 0x4000;
        }
    }
    temp_cuboid_bounce_angle = ang + obj->hdg;
    if (ang == 0) {
        temp_cuboid_side = 0x10;
    } else if (ang == 0x4000) {
        temp_cuboid_side = 0x80;
    } else if (ang == 0x8000) {
        temp_cuboid_side = 0x20;
    } else {
        temp_cuboid_side = 0x40;
    }
    return 1;
}


s32 ObjectCylinderCollision(struct obj_s *obj, struct obj_s *cyl, f32 scale,
                            s32 flag) {
    f32 dx;
    f32 dz;
    f32 r;
    f32 dist;
    s32 side;
    s32 result;
    u16 a;
    f32 obj_top;
    f32 obj_bot;
    f32 cyl_top;
    f32 cyl_bot;

    dx = obj->pos.x - cyl->pos.x;
    dz = obj->pos.z - cyl->pos.z;
    r = obj->RADIUS + cyl->RADIUS;
    dist = dx * dx + dz * dz;
    if (flag != 0) {
        r *= scale;
    }
    if (dist > r * r) {
        if (flag == 0) {
            return 0;
        }
        a = NuAtan2D(dx, dz);
        obj->pos.x = cyl->pos.x + NuTrigTable[a] * r;
        obj->pos.z = cyl->pos.z + NuTrigTable[(u16)(a + 0x4000)] * r;
    }

    side = 0;
    obj_top = obj->top * obj->SCALE;
    if (obj->pos.y + obj_top < cyl->pos.y + cyl->bot * cyl->SCALE) {
        side = 1;
    } else {
        obj_bot = obj->bot * obj->SCALE;
        cyl_top = cyl->pos.y + cyl->top * cyl->SCALE;
        cyl_bot = cyl->pos.y + cyl->bot * cyl->SCALE;
        if (obj->pos.y + obj_bot > cyl_top) {
            side = 1;
        } else if (obj->oldpos.y + obj->bot * obj->old_SCALE >=
                   cyl->oldpos.y + cyl->top * cyl->old_SCALE - 0.012f) {
            obj->pos.y = cyl_top - obj_bot;
            obj->ground = 3;
            result = 2;
            obj->shadow = cyl->pos.y + cyl->top * cyl->SCALE;
            obj->got_shadow = 1;
            obj->surface_type = 0;
            obj->vSN = v010;
            obj->mom.y = cyl->mom.y;
            goto apply;
        } else if (obj->oldpos.y + obj->top * obj->old_SCALE <=
                   cyl->oldpos.y + cyl->bot * cyl->old_SCALE + 0.012f) {
            result = 4;
            obj->pos.y = cyl_bot - obj_top;
            obj->mom.y = cyl->mom.y - 0.012f;
            goto apply;
        }
    }

    if (side != 0) {
        f32 mid;
        f32 top;

        top = cyl->pos.y + cyl->top * cyl->SCALE;
        mid = obj->pos.y + (obj->bot + obj->top) * obj->SCALE * 0.5f;
        result = 0;
        if (top < mid) {
            if (obj->got_shadow == 0 || top > obj->shadow) {
                obj->shadow = top;
                obj->got_shadow = 1;
                obj->surface_type = 0;
                obj->vSN = v010;
            }
            result = 0;
        }
    } else {
        result = 1;
        a = NuAtan2D(obj->pos.x - cyl->pos.x, obj->pos.z - cyl->pos.z);
        obj->mom.x = NuTrigTable[a] * 0.06f;
        obj->mom.z = NuTrigTable[(u16)(a + 0x4000)] * 0.06f;
    }

apply:
    if (result & 6) {
        f32 new_x;
        f32 new_z;

        obj->vSN = v010;
        new_x = obj->pos.x + cyl->mom.x;
        obj->pos.x = new_x;
        new_z = obj->pos.z + cyl->mom.z;
        obj->pos.z = new_z;
        if (cyl->dyrot != 0) {
            if ((result & 2) || ((result & 4) && (cyl->flags & 0x200))) {
                f32 ox;
                f32 oz;
                f32 xoff;
                f32 zoff;

                obj->hdg += cyl->dyrot;
                dx = new_x - cyl->pos.x;
                dz = new_z - cyl->pos.z;
                a = NuAtan2D(dx, dz);
                dist = NuFsqrt(dx * dx + dz * dz);
                ox = NuTrigTable[a] * dist;
                oz = NuTrigTable[(u16)(a + 0x4000)] * dist;
                a += cyl->dyrot;
                xoff = NuTrigTable[a] * dist - ox;
                zoff = NuTrigTable[(u16)(a + 0x4000)] * dist - oz;
                obj->pos.x += xoff;
                obj->pos.z += zoff;
            }
        }
    }
    return result;
}


s32 GameObjectOverlap(struct obj_s *a, struct obj_s *b, s32 find_angle) {
    struct nuvec_s pa;
    struct nuvec_s pb;
    f32 dx;
    f32 dz;
    f32 r;

    if (a->flags & 0x2000) {
        if (a->draw_frame == 0 || a->pLOCATOR == 0 ||
            (a->model != 0 && a->model->pLOCATOR[0] == 0)) {
            return 0;
        }
    }
    if (b->flags & 0x2000) {
        if (b->draw_frame == 0 || b->pLOCATOR == 0 ||
            (b->model != 0 && b->model->pLOCATOR[0] == 0)) {
            return 0;
        }
    }
    if (a->flags & 0x2000) {
        pa.x = a->pLOCATOR->_30;
        pa.y = a->pLOCATOR->_31;
        pa.z = a->pLOCATOR->_32;
    } else {
        pa = a->pos;
    }
    if (b->flags & 0x2000) {
        pb.x = b->pLOCATOR->_30;
        pb.y = b->pLOCATOR->_31;
        pb.z = b->pLOCATOR->_32;
    } else {
        pb = b->pos;
    }
    if (pa.y + a->bot * a->SCALE > pb.y + b->top * b->SCALE) {
        return 0;
    }
    if (pb.y + b->bot * b->SCALE > pa.y + a->top * a->SCALE) {
        return 0;
    }
    dx = pb.x - pa.x;
    dz = pb.z - pa.z;
    r = a->RADIUS + b->RADIUS;
    if (dx * dx + dz * dz <= r * r) {
        if (find_angle) {
            temp_yrot = NuAtan2D(dx, dz);
        }
        return 1;
    }
    return 0;
}


void PlayerCreatureCollisions(struct obj_s *obj)
{
    s32 i;
    s32 maskflag;
    s32 mask_weak;
    struct obj_s *po;
    s32 lose;
    s32 killanim;

    if (level_part_2 != 0) {
        return;
    }
    temp_xzmomset = 0;
    if (obj->dead != 0) {
        return;
    }
    maskflag = 0;
    if (obj->mask != 0) {
        s32 act = obj->mask->active;
        if (act != 0) {
            if (LDATA->flags & 0xE00) {
                act = 0;
            }
            maskflag = act;
        }
    }
    lose = 0;
    mask_weak = maskflag < 3;
    for (i = 0; i < GAMEOBJECTCOUNT; i++) {
        struct nuvec_s cpos;
        struct nuvec_s dpos;
        s32 overlap;
        s32 is_cuboid;
        s32 side;
        s32 ctype;
        s32 dir;
        s32 hitside;
        f32 reach;
        f32 distsq;
        s32 ang;

        po = pObj[i];
        if (po == 0) {
            continue;
        }
        po->touch = 0;
        po->contact = 0;
        if (po == obj) {
            continue;
        }
        po->kill_contact = 0;
        if (po->dead != 0) {
            continue;
        }
        if (po->invisible != 0) {
            continue;
        }
        if (po->draw_frame == 0) {
            continue;
        }
        if ((po->flags & 0x4) == 0) {
            continue;
        }
        if (po->flags & 0x1000) {
            continue;
        }
        NewTopBot(obj);
        if (po->flags & 0x2000) {
            if (po->draw_frame == 0) {
                continue;
            }
            if (po->pLOCATOR != 0 && po->model->pLOCATOR[0] != 0) {
                cpos.x = po->pLOCATOR->_30;
                cpos.y = po->pLOCATOR->_31;
                cpos.z = po->pLOCATOR->_32;
            } else {
                cpos = po->pos;
            }
        } else {
            cpos = po->pos;
        }

        if (po->flags & 0x8000) {
            overlap = CylinderCuboidOverlapXZ(&obj->pos, po, &cpos, obj->RADIUS);
            is_cuboid = 1;
            side = temp_cuboid_side;
        } else {
            f32 dx = obj->pos.x - cpos.x;
            f32 dz = obj->pos.z - cpos.z;
            f32 r = obj->RADIUS + po->RADIUS;
            distsq = dx * dx + dz * dz;
            overlap = (r * r < distsq) ? 0 : 1;
            is_cuboid = 0;
            side = 8;
        }

        po->objbot = cpos.y + po->bot * po->SCALE;
        po->objtop = cpos.y + po->top * po->SCALE;

        if (!overlap) {
            f32 f1;
            if (po->attack & 0x2) {
                continue;
            }
            if ((obj->attack & 0x2) && (po->vulnerable & 0x2)) {
                goto vertical;
            }
            if (obj->character == 1 && (obj->attack & 0x10) &&
                (po->vulnerable & 0x10)) {
                goto vertical;
            }
            f1 = obj->mom.y;
        ycheck:
            if (!(f1 < 0.0f)) {
                continue;
            }
            if ((obj->attack & 0xC) == 0) {
                continue;
            }
            if ((po->vulnerable & 0xC) == 0) {
                continue;
            }
        vertical:
            if (obj->objtop < po->objbot) {
                continue;
            }
            if (po->objtop < obj->objbot) {
                continue;
            }
            if (obj->attack & 0x8) {
                reach = obj->radius * obj->SCALE + 0.5f;
            } else if (obj->attack & 0x4) {
                reach = obj->radius * obj->SCALE + 0.25f;
            } else {
                reach = obj->radius * obj->SCALE;
                if (obj->attack & 0x2) {
                    if (obj->character != 1) {
                        reach = reach + reach;
                    } else if (((struct creature_s *)obj->parent)->slide == 0) {
                        reach = reach + reach;
                    } else {
                        reach = reach * 3.0f;
                    }
                }
            }

            if (is_cuboid) {
                if (CylinderCuboidOverlapXZ(&obj->pos, po, &po->pos, reach) == 0) {
                    continue;
                }
                po->dead = (obj->attack & 0x2) ? 1 : 4;
                hitside = temp_cuboid_side;
            } else {
                f32 rr = reach + po->RADIUS;
                if (rr * rr < distsq) {
                    continue;
                }
                po->dead = (obj->attack & 0x2) ? 1 : 4;
                hitside = 8;
            }
            po->touch = (obj->attack & 0xC) ? 2 : hitside;
            if (po->dead == 0) {
                continue;
            }

            dpos.x = (obj->pos.x + cpos.x) * 0.5f;
            dpos.z = (obj->pos.z + cpos.z) * 0.5f;
            if (obj->attack & 0xC) {
                dpos.y = obj->pos.y + obj->bot * obj->SCALE;
            } else {
                dpos.y = (obj->pos.y + (obj->bot + obj->top) * obj->SCALE * 0.5f +
                          (po->objbot + po->objtop) * 0.5f) * 0.5f;
            }
            AddGameDebris(0x82, &dpos);
            if (obj->attack & 0x2) {
                if (po->oldobjtop - D_0062D7C0 <= obj->oldobjbot) {
                    obj->mom.y = 0.0f;
                } else if (obj->oldobjtop <= po->oldobjbot + D_0062D7C0) {
                    obj->mom.y = 0.0f;
                } else {
                    obj->mom.x = obj->mom.x * D_0062D7C4;
                    obj->mom.z = obj->mom.z * D_0062D7C4;
                }
            }
            goto kill_tail_72C;
        }

        /* physical overlap */
        dir = 0;
        if (obj->objtop < po->objbot || po->objtop < obj->objbot) {
            dir = 1;
            goto topbot;
        }
        if (VEHICLECONTROL == 2 ||
            (VEHICLECONTROL == 1 && obj->vehicle == 0x20)) {
            if (obj->attack & (po->vulnerable & 0x82)) {
                po->dead = 0x15;
            } else if (maskflag == 0) {
                obj->dead = 1;
            } else if (mask_weak) {
                lose = 1;
                po->dead = 0x15;
            } else {
                obj->dead = 1;
            }
            NuVecScale(&obj->mom, &obj->mom, D_0062D7C8);
            goto kill_tail_724;
        }

        if (po->oldobjtop - D_0062D7CC <= obj->oldobjbot) {
            s32 a1 = 0;
            s32 a2 = 0;
            ctype = 2;
            if (obj->attack & (po->vulnerable & 0x8E)) {
                po->dead = (obj->attack & 0x8C) ? 4 : 1;
                if (obj->attack & 0x2) {
                    obj->mom.y = 0.0f;
                }
            } else if (obj->attack & (po->vulnerable & 0x20)) {
                po->dead = 4;
                a2 = 1;
            } else if ((obj->attack & 0x2) && (po->attack & 0x2)) {
                a2 = 1;
            } else {
                if (po->attack == 0) {
                    if ((po->flags & 0x100) == 0) {
                        a1 = 1;
                        goto push_up;
                    }
                }
                if (maskflag == 0) {
                    obj->dead = 1;
                } else if (mask_weak) {
                    lose = 1;
                    po->dead = 4;
                    goto push_up;
                } else {
                    obj->dead = 1;
                }
                a1 = 1;
                a2 = (po->attack & 0x2) ? 1 : 0;
            }
        push_up:
            if (a2 == 0) {
                a2 = (po->flags & 0x40) ? 1 : 0;
                if (a2 == 0) {
                    a2 = (po->flags & 0x80) ? 2 : 0;
                    if (a2 == 0 && a1 == 0) {
                        dir = 2;
                        goto push_up_done;
                    }
                }
            }
            obj->mom.y = 0.0f;
            obj->ground = 2;
            obj->pos.y = po->objtop - obj->bot * obj->SCALE;
            obj->shadow = po->objtop;
            obj->surface_type = 0xF;
            obj->got_shadow = 1;
            obj->vSN = v010;
            if (a2 != 0) {
                obj->boing = obj->boing | a2;
                GameSfx(2, &obj->pos);
                NewRumble(&player->rumble, 0x7F);
                NewBuzz(&player->rumble, 0xA);
                dir = 2;
            } else if (a1 != 0) {
                obj->pos.x = obj->pos.x + po->mom.x;
                obj->pos.z = obj->pos.z + po->mom.z;
                if (po->dyrot != 0) {
                    f32 mdx;
                    f32 mdz;
                    f32 dist;
                    s32 base;
                    s32 na;
                    obj->hdg = obj->hdg + po->dyrot;
                    mdx = obj->pos.x - po->pos.x;
                    mdz = obj->pos.z - po->pos.z;
                    base = NuAtan2D(mdx, mdz);
                    dist = NuFsqrt(mdx * mdx + mdz * mdz);
                    na = base + po->dyrot;
                    obj->pos.x = obj->pos.x +
                                 (NuTrigTable[(u16)na] * dist -
                                  NuTrigTable[(u16)base] * dist);
                    obj->pos.z = obj->pos.z +
                                 (NuTrigTable[(u16)(na + 0x4000)] * dist -
                                  NuTrigTable[(u16)(base + 0x4000)] * dist);
                }
                dir = 2;
            }
        push_up_done:
            po->touch = dir;
            goto kill_tail_724;
        }

        if (obj->oldobjtop <= po->oldobjbot + D_0062D7CC) {
            s32 a1 = 0;
            ctype = 4;
            if (obj->attack & (po->vulnerable & 0x8E)) {
                po->dead = (obj->attack & 0x8C) ? 4 : 1;
                if (obj->attack & 0x2) {
                    obj->mom.y = 0.0f;
                    if (obj->flags & 0x1) {
                        if (player->jump != 0) {
                            player->jump = 6;
                            player->jump_type = 4;
                            player->jump_frame = player->jump_frames;
                        }
                    }
                }
            } else if (obj->attack & (po->vulnerable & 0x40)) {
                po->dead = 4;
                a1 = 1;
            } else if ((obj->attack & 0x2) && (po->attack & 0x2)) {
                /* nothing */
            } else if (po->attack == 0) {
                a1 = 1;
            } else if (maskflag == 0) {
                obj->dead = 1;
                a1 = 1;
            } else if (mask_weak) {
                lose = 1;
                po->dead = 4;
            } else {
                obj->dead = 1;
                a1 = 1;
            }
            if (a1 != 0) {
                obj->pos.y = po->objbot - obj->top * obj->SCALE;
                obj->mom.y = 0.0f;
            }
            po->touch = 4;
            goto kill_tail_724;
        }

    topbot:
        if (dir == 0) {
            s32 a1 = 0;
            ctype = 1;
            if (obj->attack & (po->vulnerable & 0x9E)) {
                po->dead = (obj->attack & 0x80) ? 4 : 1;
            } else if ((obj->attack & 0x2) && (po->attack & 0x2)) {
                a1 = 1;
            } else if (po->attack == 0) {
                a1 = 1;
            } else if (maskflag == 0) {
                obj->dead = 1;
                a1 = 1;
            } else if (mask_weak) {
                lose = 1;
                po->dead = 4;
            } else {
                obj->dead = 1;
                a1 = 1;
            }
            if (a1 != 0) {
                if (po->character == 0x39) {
                    gamesfx_effect_volume = 0x9FFD;
                    GameSfx(0x49, &obj->pos);
                }
                if (is_cuboid) {
                    ang = (s32)temp_cuboid_bounce_angle;
                } else {
                    ang = NuAtan2D(obj->pos.x - po->pos.x,
                                   obj->pos.z - po->pos.z);
                }
                {
                    f32 bf = (po->attack & 0x2) ? D_0062D7D4 : D_0062D7D0;
                    obj->mom.x = NuTrigTable[(u16)ang] * bf;
                    obj->mom.z = NuTrigTable[(u16)(ang + 0x4000)] * bf;
                    temp_xzmomset = 1;
                }
            }
            po->touch = side;
            goto kill_tail_724;
        } else {
            f32 objmid = (obj->objtop + obj->objbot) * 0.5f;
            if (!(po->objtop < objmid)) {
                continue;
            }
            if (obj->got_shadow == 0) {
                obj->shadow = po->objtop;
            } else if (obj->shadow < po->objtop) {
                obj->shadow = po->objtop;
            } else {
                continue;
            }
            obj->got_shadow = 1;
            obj->surface_type = 0;
            obj->vSN = v010;
            continue;
        }

    kill_tail_724:
        po->contact = obj;
        po->contact_type = ctype;
    kill_tail_72C:
        if (po->dead != 0) {
            dpos.x = (obj->pos.x + cpos.x) * 0.5f;
            dpos.z = (obj->pos.z + cpos.z) * 0.5f;
            if (ctype == 2) {
                dpos.y = obj->pos.y + obj->bot * obj->SCALE;
            } else if (ctype == 4) {
                dpos.y = obj->pos.y + obj->top * obj->SCALE;
            } else {
                dpos.y = (obj->pos.y + (obj->bot + obj->top) * obj->SCALE * 0.5f +
                          (po->objbot + po->objtop) * 0.5f) * 0.5f;
            }
            AddGameDebris(0x82, &dpos);
            NewRumble(&player->rumble, 0x7F);
            NewBuzz(&player->rumble, 0x5);
            if ((po->flags & 0x40000) && !(obj->attack & 0x80)) {
                po->kill_contact = 1;
                po->dead = 0;
                continue;
            }
            if (po->dead == 1) {
                ang = NuAtan2D(po->pos.x - obj->pos.x, po->pos.z - obj->pos.z);
                po->mom.x = 0.0f;
                po->mom.y = 0.0f;
                po->mom.z = D_0062D7D8;
                NuVecRotateX(&po->mom, &po->mom, -0x400);
                NuVecRotateY(&po->mom, &po->mom, (u16)ang);
                GameSfx(0x37, &po->pos);
            }
            KillGameObject(po, po->dead);
            continue;
        }
        if (obj->dead == 0) {
            continue;
        }
        goto obj_death;
    }

    if (lose != 0) {
        if (obj->invincible == 0) {
            LoseMask(obj);
        }
        goto after_death;
    }
    if (obj->dead == 0) {
        goto after_death;
    }
obj_death:
    if (obj->invincible != 0) {
        obj->dead = 0;
    } else {
        if (((s8 *)po->parent)[0x21C] == -1) {
            killanim = GetDieAnim(obj, -1);
        } else {
            killanim = ((s8 *)po->parent)[0x21C];
        }
        KillGameObject(obj, killanim);
    }
after_death:
    if (VEHICLECONTROL == 1 && obj->vehicle == 0x53) {
        ObjectToAtlas(obj, player);
    }
}


void GetTopBot(struct creature_s *c) {
    if (c == player && VEHICLECONTROL == 1 && c->obj.vehicle != -1) {
        c->obj.bot = CData[c->obj.vehicle].min.y;
        c->obj.top = CData[c->obj.vehicle].max.y;
    } else if (c == player && VEHICLECONTROL == 2) {
        c->obj.bot = CData[115].min.y;
        c->obj.top = CData[115].max.y;
    } else if (c->obj.flags & 0x10000) {
        c->obj.bot = -c->obj.max.y;
        c->obj.top = -c->obj.min.y;
    } else if (c->obj.dangle != 0) {
        c->obj.top = c->obj.max.y;
        if (c->obj.dangle == 2) {
            c->obj.bot = (c->obj.min.y + c->obj.top) * 0.5f;
        } else {
            c->obj.bot = c->obj.min.y;
        }
    } else {
        c->obj.bot = c->obj.min.y;
        if (((c->obj.ground != 0 || c->obj.old_ground != 0) &&
             (c->crawl != 0 || (c->slide != 0 && c->obj.character != 1) ||
              c->slam_wait != 0)) ||
            (c->obj.ground == 0 && c->slam != 0 && c->slam < 3)) {
            c->obj.top = (c->obj.min.y + c->obj.max.y) * 0.5f;
        } else {
            c->obj.top = c->obj.max.y;
        }
    }
}

void PickupItem(struct obj_s *obj) {
    struct creature_s *c;
    s32 mask;
    s32 power;

    if (new_mode == -1 && new_level == -1) {
        switch (obj->character) {
        case 0x75:
            plr_crystal.count = 1;
            plr_crystal.frame = 0x19;
            plr_items |= 1;
            GameSfx(0x26, 0);
            AddPanelDebris(0.0f, -0.7f, 6, 0.125f, 0x10);
            ClockOff();
            GameSfx(0x26, 0);
            break;
        case 0x76:
            StartTimeTrial(&obj->pos, 0);
            break;
        case 0x77:
            plr_crategem.count = 1;
            plr_crategem.frame = 0x19;
            plr_items |= 2;
            GameSfx(0x26, 0);
            AddPanelDebris(-0.2f, -0.7f, 6, 0.125f, 0x10);
            ClockOff();
            GameSfx(0x26, 0);
            break;
        case 0x78:
        case 0x79:
        case 0x7A:
        case 0x7B:
        case 0x7C:
        case 0x7D:
            if (obj->character == 0x79) {
                mask = 0x8;
            } else if (obj->character == 0x7A) {
                mask = 0x20;
            } else if (obj->character == 0x7B) {
                mask = 0x10;
            } else if (obj->character == 0x7C) {
                mask = 0x40;
            } else {
                mask = (obj->character == 0x7D) ? 0x80 : 0x4;
            }
            plr_items |= mask;
            plr_bonusgem.type = mask;
            plr_bonusgem.count = 1;
            plr_bonusgem.frame = 0x19;
            GameSfx(0x26, 0);
            AddPanelDebris(0.2f, -0.7f, 6, 0.125f, 0x10);
            GameSfx(0x26, 0);
            break;
        case 0xA2:
        case 0xA3:
        case 0xA4:
        case 0xA5:
        case 0xA6:
        case 0xA7:
            if (LBIT & 0x3E00000) {
                boss_dead = 2;
            }
            power = obj->character;
            switch (power) {
            case 0xA7:
                new_power = 0;
                break;
            case 0xA5:
                new_power = 1;
                break;
            case 0xA6:
                new_power = 2;
                break;
            case 0xA2:
                new_power = 3;
                break;
            case 0xA4:
                new_power = 4;
                break;
            case 0xA3:
                new_power = 5;
                break;
            }
            Game[0x406] |= 1 << new_power;
            NewMenu(Cursor, 0x1F, -1, -1);
            GameSfx(0x26, 0);
            if (Level != 0x15 && Level != 0x18) {
                player->obj.mom.x = 0.0f;
                player->obj.mom.z = 0.0f;
                player->slide = 0;
            }
            break;
        }
    }
    c = (struct creature_s *)obj->parent;
    obj->dead = 1;
    c->on = 0;
    c->off_wait = 2;
}


s32 GetDieAnim(struct obj_s *obj, s32 anim) {
    if (VEHICLECONTROL == 1 && obj->vehicle != -1) {
        if (anim == 6 && obj->vehicle == 0x99) {
            return 0xC;
        }
        return 0xB;
    }
    if (VEHICLECONTROL == 2) {
        return 0xE;
    }
    if (anim == -1) {
        if (Level == 0x17 || qrand() <= 0x7FFF) {
            return 2;
        }
        return 3;
    }
    return anim;
}


s32 KillPlayer(struct obj_s *obj, s32 anim)
{
    struct mask_s *mask;

    if (obj->dead) {
        return 0;
    }
    if (obj->finished) {
        return 0;
    }
    if (obj->invincible) {
        return 0;
    }
    if (vtog_time < vtog_duration && vtog_blend != 0) {
        return 0;
    }
    mask = obj->mask;
    if (mask != 0 && mask->active != 0 && (LDATA->flags & 0xE00) == 0) {
        if (mask->active < 3) {
            LoseMask(obj);
        }
        return 0;
    }
    return KillGameObject(obj, anim);
}

void PickupCrystal(void) {
    plr_crystal.count = 1;
    plr_crystal.frame = 0x19;
    plr_items |= 1;
    GameSfx(0x26, 0);
    AddPanelDebris(0.0f, D_0062D904, 6, 0.125f, 0x10);
}

void PickupCrateGem(void) {
    plr_crategem.count = 1;
    plr_crategem.frame = 0x19;
    plr_items |= 2;
    GameSfx(0x26, 0);
    AddPanelDebris(D_0062D908, D_0062D90C, 6, 0.125f, 0x10);
}

void PickupBonusGem(s32 mask) {
    plr_items |= mask;
    plr_bonusgem.type = mask;
    plr_bonusgem.count = 1;
    plr_bonusgem.frame = 0x19;
    GameSfx(0x26, 0);
    AddPanelDebris(0.2f, -0.7f, 6, 0.125f, 0x10);
}

void PickupPower(s32 type) {
    switch (type) {
    case 0xA2:
        new_power = 0;
        break;
    case 0xA3:
        new_power = 1;
        break;
    case 0xA4:
        new_power = 2;
        break;
    case 0xA5:
        new_power = 3;
        break;
    case 0xA6:
        new_power = 4;
        break;
    case 0xA7:
        new_power = 5;
        break;
    }
    Game[0x406] |= 1 << new_power;
    NewMenu(Cursor, 0x1F, -1, -1);
    GameSfx(0x26, 0);
    if (Level != 0x15 && Level != 0x18) {
        player->obj.mom.x = 0.0f;
        player->obj.mom.z = 0.0f;
        player->slide = 0;
    }
}


s32 HitItems(struct obj_s *obj) {
    s32 i;
    struct obj_s *item;

    if (level_part_2 != 0) {
        return 0;
    }
    for (i = 0; i < 64; i++) {
        item = pObj[i];
        if (item != 0 && item->dead == 0 && item->invisible == 0 &&
            (item->flags & 0x10) && GameObjectOverlap(obj, item, 0)) {
            PickupItem(item);
            return 1;
        }
    }
    return 0;
}

void ClearGameObjects(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        pObj[i] = 0;
    }
}

inline void CountGameObjects(void) {
    s32 i;

    for (i = 64; i > 0; i--) {
        if (pObj[i - 1] != 0) {
            break;
        }
    }
    GAMEOBJECTCOUNT = i;
}

s32 CylinderCylinderOverlapXZ(struct nuvec_s *p0, f32 r0, struct nuvec_s *p1,
                              f32 r1) {
    f32 dz;
    f32 dx;

    dx = p1->x - p0->x;
    dz = p1->z - p0->z;
    if (dx * dx + dz * dz <= r0 * r0 + r1 * r1) {
        return 1;
    }
    return 0;
}

void NewTopBot(struct obj_s *obj) {
    obj->objbot = obj->pos.y + obj->bot * obj->SCALE;
    obj->objtop = obj->pos.y + obj->top * obj->SCALE;
}

void OldTopBot(struct obj_s *obj) {
    obj->oldobjbot = obj->objbot;
    obj->oldobjtop = obj->objtop;
}


void FindAnglesZX(struct nuvec_s *v) {
    struct nuvec_s rotated;
    u16 xrot;

    xrot = NuAtan2D(v->z, v->y);
    temp_xrot = xrot;
    NuVecRotateX(&rotated, v, -xrot);
    temp_zrot = -NuAtan2D(rotated.x, rotated.y);
}


void ScaleFlatShadow(struct nuvec_s *s, f32 y, f32 shadow, f32 scale) {
    f32 dy;

    dy = y - shadow;
    if (dy <= 0.0f) {
        s->x = scale;
    } else if (dy >= 2.0f) {
        s->x = scale * 0.5f;
    } else {
        s->x = scale - scale * 0.5f * dy * 0.5f;
    }
    s->y = 0.0f;
    s->z = s->x;
}

void KillItem(struct obj_s *obj) {
    struct creature_s *c;

    c = (struct creature_s *)obj->parent;
    obj->dead = 1;
    c->on = 0;
    c->off_wait = 2;
}

void ResetGameObject(struct obj_s *obj) {
    memset(obj, 0, 0x188);
    obj->reflect_y = 2000000.0f;
    obj->SCALE = 1.0f;
    obj->RPos.iRAIL = -1;
    obj->RPos.iALONG = -1;
    obj->layer_type = -1;
    obj->roof_type = -1;
    obj->scale = 1.0f;
}

s32 AddGameObject(struct obj_s *obj, struct obj_s *parent) {
    s32 i;
    s32 added;

    memset(obj, 0, 0x188);
    obj->SCALE = 1.0f;
    obj->scale = 1.0f;
    obj->RPos.iRAIL = -1;
    obj->RPos.iALONG = -1;
    obj->layer_type = -1;
    obj->roof_type = -1;
    obj->reflect_y = 2000000.0f;

    for (i = 0; i < 64; i++) {
        if (pObj[i] == 0) {
            break;
        }
    }
    if (i < 64) {
        pObj[i] = obj;
        obj->parent = parent;
        obj->i = i;
        added = 1;
    } else {
        added = 0;
    }
    CountGameObjects();
    return added;
}

void RemoveGameObject(struct obj_s *obj) {
    s32 i;

    for (i = 0; i < 64; i++) {
        if (pObj[i] == obj) {
            pObj[i] = 0;
            i = 64;
        }
    }
    CountGameObjects();
}

void ResetProjectiles(void) {
    s32 i;
    s32 j;
    struct projectile_s *p;

    p = Projectile;
    for (i = 0; i < 16; i++) {
        if (p->active) {
            for (j = 0; j < 64; j++) {
                if (pObj[j] == &p->obj) {
                    pObj[j] = 0;
                    j = 64;
                }
            }
            CountGameObjects();
            p->active = 0;
        }
        p++;
    }
    jonglow = 0;
    jonglowsize = 0;
}

f32 GameObjectRadius(struct obj_s *obj) {
    f32 r;

    r = obj->radius * obj->SCALE;
    if ((obj->attack & 8) != 0) {
        return r + 0.5f;
    }
    if ((obj->attack & 4) != 0) {
        return r + 0.25f;
    }
    if ((obj->attack & 2) != 0) {
        if ((obj->character == 1) &&
            (*(short *)((int)&obj->parent[8].startpos.y + 2) != 0)) {
            return r * 3.0f;
        } else {
            return r + r;
        }
    }
    return r;
}

void FlyGameObject(struct obj_s *obj, u16 yrot) {
    obj->mom.x = 0.0f;
    obj->mom.y = 0.0f;
    obj->mom.z = 0.1999999881f;
    NuVecRotateX(&obj->mom, &obj->mom, -0x400);
    NuVecRotateY(&obj->mom, &obj->mom, yrot);
    GameSfx(0x37, &obj->pos);
}


void PickupRelic(s32 relic) {
    plr_crystal.type = relic;
    plr_crystal.count = 1;
    plr_crystal.frame = 0x19;
    GameSfx(0x26, 0);
}
