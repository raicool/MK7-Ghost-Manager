#include "pch.h"
#include "texture.h"

#include "external/miiconv.h"
#include "common/log.h"
#include "common/type.h"
#include "mii.h"
#include <queue>

extern SDL_Renderer* g_renderer;

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
	driver[CHAR_SHY_GUY]      = texture::add_texture("res/tex/Driver/sh.png", "shy_guy");
	driver[CHAR_TOAD]         = texture::add_texture("res/tex/Driver/td.png", "toad");
	driver[CHAR_WARIO]        = texture::add_texture("res/tex/Driver/wr.png", "wario");
	driver[CHAR_WIGGLER]      = texture::add_texture("res/tex/Driver/wig.png", "wiggler");
	driver[CHAR_YOSHI]        = texture::add_texture("res/tex/Driver/ys.png", "yoshi");
	LOG_TRACE("character textures loaded");
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
	LOG_TRACE("kart textures loaded");
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
	LOG_TRACE("tire textures loaded");
	// gliders
	wing[WING_SUPER_GLIDER]  = texture::add_texture("res/tex/Kart/Wing/wing_std.png", "super_glider");
	wing[WING_PARAGLIDER]    = texture::add_texture("res/tex/Kart/Wing/wing_para.png", "paraglider");
	wing[WING_PEACH_PARASOL] = texture::add_texture("res/tex/Kart/Wing/wing_umb.png", "peach_parasol");
	wing[WING_FLOWER_GLIDER] = texture::add_texture("res/tex/Kart/Wing/wing_flower.png", "flower_glider");
	wing[WING_SWOOPER]       = texture::add_texture("res/tex/Kart/Wing/wing_basa.png", "swooper");
	wing[WING_BEAST_GLIDER]  = texture::add_texture("res/tex/Kart/Wing/wing_met.png", "beast_glider");
	wing[WING_GOLD_GLIDER]   = texture::add_texture("res/tex/Kart/Wing/wing_gold.png", "gold_glider");
	LOG_TRACE("glider textures loaded");
	// symbols
	symbol[0] = texture::add_texture("res/tex/Symbol/first_person.png", "first_person_badge");
	symbol[1] = texture::add_texture("res/tex/Symbol/flag_atlas.png", "flag_atlas");
	symbol[2] = texture::add_texture("res/tex/Symbol/mii_unknown.png", "mii_unknown");
	LOG_TRACE("extra textures loaded");
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

	LOG_DEBUG("texture {} loaded at address {}", dir, (void*)textures[id]);

	SDL_DestroySurface(surface);

	texture_count++;
	return textures[id];
}

struct queue_data
{
	mii* mii_raw_data;
	async_texture* texture;
};

std::mutex mlock;
std::queue<queue_data> texture_queue;
std::condition_variable cv;

void __async_job_fetch_texture()
{
	std::unique_lock<std::mutex> lock(mlock);

	while (1)
	{
		cv.wait(lock, []{ return texture_queue.size() > 0; });

		auto job = texture_queue.front();
		texture_queue.pop();

		//https://mii-unsecure.ariankordi.net/miis/image.png?data=000f165c66757884939e959897999f9ca0b3b9bfbdc4c8cfced2dce0eff5c3ecf3fafeedf6ecf3f401080f0a11181a&resourceType=low&shaderType=ffliconwithbody&bodyType=3ds&characterYRotate=333&lightEnable=true
		httplib::Client client = httplib::Client("https://mii-unsecure.ariankordi.net");
		const std::string mii_url = "/miis/image.png?data={}&width=128&resourceType=low&shaderType=ffliconwithbody&bodyType=3ds&characterYRotate=333&lightEnable=true";

		thread_local char s[(sizeof(mii) * 2) + 1];
		s[sizeof(mii)] = '\00';

		char* p = s;
		for (size_t i = 0; i < sizeof(mii); i++)
		{
			uint8_t data = *((uint8_t*)job.mii_raw_data + i);
			p += sprintf(p, "%.2x", data);
		}

		const std::string mii_data_string_hexadecimal(s);

		const std::string formatted = std::vformat(mii_url, std::make_format_args(mii_data_string_hexadecimal));

		SDL_IOStream* img = nullptr;
		std::stringstream __dummy_stream;

		httplib::Result res = client.Get(formatted,
			[&](const char* data, size_t data_length)
			{
				__dummy_stream.write(data, data_length);
				LOG_TRACE("data_length: {}", data_length);
				return true;
			}
		);

		auto view = __dummy_stream.view();
		img = SDL_IOFromMem((void*)view.data(), view.size());

		job.texture->surface = IMG_Load_IO(img, true);
		job.texture->status = SURFACE_CREATED;
	}
}

std::thread t(__async_job_fetch_texture);

void* texture::request_mii_texture(mii* mii_raw_data)
{
#if BUILD_WITH_SSL_SUPPORT
	const uint16_t crc16 = mii_raw_data->crc16;

	if (mii_textures.contains(crc16))
	{
		if (mii_textures.at(crc16).status == SURFACE_CREATED)
		{
			void* data = SDL_CreateTextureFromSurface(g_renderer, mii_textures.at(crc16).surface);
			LOG_DEBUG("new mii texture loaded at address {}", data);
			SDL_DestroySurface(mii_textures.at(crc16).surface);

			mii_textures.at(crc16).surface = nullptr;
			mii_textures.at(crc16).data = (SDL_Texture*)data;
			mii_textures.at(crc16).status = OK;
		}

		return mii_textures.at(crc16).data;
	}
	else
	{
		mii_textures[crc16] = { .status = WORKING, .data = nullptr };

		texture_queue.push({ mii_raw_data, &mii_textures[crc16] });
		cv.notify_all();

		// return placeholder texture
		return nullptr;
	}
#else
	return nullptr;
#endif

}