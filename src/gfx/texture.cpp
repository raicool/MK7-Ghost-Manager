#include "pch.h"
#include "texture.h"

#include "common/log.h"
#include "common/type.h"

void texture::load_ghost_textures()
{
	// driver portraits
	driver[CHAR_BOWSER]       = texture::add_texture("res/tex/Driver/bw.png", "bowser");
	driver[CHAR_DAISY]        = texture::add_texture("res/tex/Driver/ds.png", "daisy");
	driver[CHAR_DONKEY_KONG]  = texture::add_texture("res/tex/Driver/dk.png", "donkey_kong");
	driver[CHAR_HONEY_QUEEN]  = texture::add_texture("res/tex/Driver/hq.png", "honey_queen");
	driver[CHAR_KOOPA_TROOPA] = texture::add_texture("res/tex/Driver/kt.png", "koopa_troopa");
	driver[CHAR_LAKITU]       = texture::add_texture("res/tex/Driver/lk.png", "lakitu");
	driver[CHAR_LUIGI]        = texture::add_texture("res/tex/Driver/lg.png", "luigi");
	driver[CHAR_MARIO]        = texture::add_texture("res/tex/Driver/mr.png", "mario");
	driver[CHAR_METAL_MARIO]  = texture::add_texture("res/tex/Driver/mtl.png", "metal_mario");
	driver[CHAR_MII_MALE]     = texture::add_texture("res/tex/Driver/mim.png", "mii_male");
	driver[CHAR_MII_FEMALE]   = texture::add_texture("res/tex/Driver/mif.png", "mii_female");
	driver[CHAR_PEACH]        = texture::add_texture("res/tex/Driver/pc.png", "peach");
	driver[CHAR_ROSALINA]     = texture::add_texture("res/tex/Driver/rs.png", "rosalina");
	driver[CHAR_SHY_GUY]      = texture::add_texture("res/tex/Driver/mim.png", "shy_guy");
	driver[CHAR_TOAD]         = texture::add_texture("res/tex/Driver/td.png", "toad");
	driver[CHAR_WARIO]        = texture::add_texture("res/tex/Driver/wr.png", "wario");
	driver[CHAR_WIGGLER]      = texture::add_texture("res/tex/Driver/wig.png", "wiggler");
	driver[CHAR_YOSHI]        = texture::add_texture("res/tex/Driver/ys.png", "yoshi");
	LOG_INFO("character textures loaded");
	// karts
	body[KART_STANDARD]      = texture::add_texture("res/tex/Kart/Body/body_std.png", "standard");
	body[KART_BOLT_BUGGY]    = texture::add_texture("res/tex/Kart/Body/body_rally.png", "bolt_buggy");
	body[KART_BIRTHDAY_GIRL] = texture::add_texture("res/tex/Kart/Body/body_rbn.png", "birthday_girl");
	body[KART_EGG_1]         = texture::add_texture("res/tex/Kart/Body/body_egg.png", "egg_1");
	body[KART_BDASHER]       = texture::add_texture("res/tex/Kart/Body/body_dsh.png", "b_dasher");
	body[KART_ZUCCHINI]      = texture::add_texture("res/tex/Kart/Body/body_cuc.png", "zucchini");
	body[KART_KOOPA_CLOWN]   = texture::add_texture("res/tex/Kart/Body/body_kpc.png", "koopa_clown");
	body[KART_TINY_TUG]      = texture::add_texture("res/tex/Kart/Body/body_boat.png", "tiny_tug");
	body[KART_BUMBLE_V]      = texture::add_texture("res/tex/Kart/Body/body_hny.png", "bumble_v");
	body[KART_CACT_X]        = texture::add_texture("res/tex/Kart/Body/body_sabo.png", "cact-x");
	body[KART_BRUISER]       = texture::add_texture("res/tex/Kart/Body/body_gng.png", "bruiser");
	body[KART_PIPE_FRAME]    = texture::add_texture("res/tex/Kart/Body/body_pipe.png", "pipe_frame");
	body[KART_BARREL_TRAIN]  = texture::add_texture("res/tex/Kart/Body/body_trn.png", "barrel_train");
	body[KART_CLOUD_9]       = texture::add_texture("res/tex/Kart/Body/body_cld.png", "cloud_9");
	body[KART_BLUE_SEVEN]    = texture::add_texture("res/tex/Kart/Body/body_race.png", "blue_seven");
	body[KART_SODA_JET]      = texture::add_texture("res/tex/Kart/Body/body_jet.png", "soda_jet");
	body[KART_GOLD_STANDARD] = texture::add_texture("res/tex/Kart/Body/body_gold.png", "gold_standard");
	LOG_INFO("kart textures loaded");
	// tires
	tire[TIRE_STANDARD]    = texture::add_texture("res/tex/Kart/Tire/tire_std.png", "standard");
	tire[TIRE_MONSTER]     = texture::add_texture("res/tex/Kart/Tire/tire_big.png", "monster");
	tire[TIRE_ROLLER]      = texture::add_texture("res/tex/Kart/Tire/tire_small.png", "roller");
	tire[TIRE_SLICK]       = texture::add_texture("res/tex/Kart/Tire/tire_race.png", "slick");
	tire[TIRE_SLIM]        = texture::add_texture("res/tex/Kart/Tire/tire_classic.png", "slim");
	tire[TIRE_SPONGE]      = texture::add_texture("res/tex/Kart/Tire/tire_sponge.png", "sponge");
	tire[TIRE_GOLD_TIRES]  = texture::add_texture("res/tex/Kart/Tire/tire_gold.png", "gold_tires");
	tire[TIRE_WOOD]        = texture::add_texture("res/tex/Kart/Tire/tire_wood.png", "wood");
	tire[TIRE_RED_MONSTER] = texture::add_texture("res/tex/Kart/Tire/tire_bigRed.png", "red_monster");
	tire[TIRE_MUSHROOM]    = texture::add_texture("res/tex/Kart/Tire/tire_mush.png", "mushroom");
	LOG_INFO("tire textures loaded");
	// gliders
	wing[WING_SUPER_GLIDER]  = texture::add_texture("res/tex/Kart/Wing/wing_std.png", "super_glider");
	wing[WING_PARAGLIDER]    = texture::add_texture("res/tex/Kart/Wing/wing_para.png", "paraglider");
	wing[WING_PEACH_PARASOL] = texture::add_texture("res/tex/Kart/Wing/wing_umb.png", "peach_parasol");
	wing[WING_FLOWER_GLIDER] = texture::add_texture("res/tex/Kart/Wing/wing_flower.png", "flower_glider");
	wing[WING_SWOOPER]       = texture::add_texture("res/tex/Kart/Wing/wing_basa.png", "swooper");
	wing[WING_BEAST_GLIDER]  = texture::add_texture("res/tex/Kart/Wing/wing_met.png", "beast_glider");
	wing[WING_GOLD_GLIDER]   = texture::add_texture("res/tex/Kart/Wing/wing_gold.png", "gold_glider");
	LOG_INFO("glider textures loaded");
	// symbols
	symbol[0] = texture::add_texture("res/tex/Symbol/first_person.png", "first_person_badge");
	symbol[1] = texture::add_texture("res/tex/Symbol/flag_atlas.png", "flag_atlas");
	LOG_INFO("extra textures loaded");
}


void* texture::add_texture(const char* dir, std::string_view id)
{
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);

	SDL_Surface* surface = IMG_Load(dir);

	if (!surface)
	{
		LOG_ERROR("Error creating texture {} : {}", id, SDL_GetError());
		return 0;
	}

	textures[id] = SDL_CreateTextureFromSurface(current_renderer, surface);

	//LOG_TRACE("texture {} created at address {}", dir, (void*)textures[id]);

	SDL_DestroySurface(surface);

	texture_count++;
	return textures[id];
}