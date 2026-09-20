#include "pch.h"
#include "texture.h"

#include "external/miiconv.h"
#include "common/log.h"
#include "common/type.h"
#include "cfl.h"

#include <queue>

extern SDL_Renderer* g_renderer;

void TextureManager::load_ghost_textures()
{
	// driver portraits
	driver[CHAR_BOWSER]       = this->add_texture("res/tex/Driver/bw.png", "bowser");
	driver[CHAR_DAISY]        = this->add_texture("res/tex/Driver/ds.png", "daisy");
	driver[CHAR_DONKEY_KONG]  = this->add_texture("res/tex/Driver/dk.png", "donkey_kong");
	driver[CHAR_HONEY_QUEEN]  = this->add_texture("res/tex/Driver/hq.png", "honey_queen");
	driver[CHAR_KOOPA_TROOPA] = this->add_texture("res/tex/Driver/kt.png", "koopa_troopa");
	driver[CHAR_LAKITU]       = this->add_texture("res/tex/Driver/lk.png", "lakitu");
	driver[CHAR_LUIGI]        = this->add_texture("res/tex/Driver/lg.png", "luigi");
	driver[CHAR_MARIO]        = this->add_texture("res/tex/Driver/mr.png", "mario");
	driver[CHAR_METAL_MARIO]  = this->add_texture("res/tex/Driver/mtl.png", "metal_mario");
	driver[CHAR_MII_MALE]     = this->add_texture("res/tex/Driver/mim.png", "mii_male");
	driver[CHAR_MII_FEMALE]   = this->add_texture("res/tex/Driver/mif.png", "mii_female");
	driver[CHAR_PEACH]        = this->add_texture("res/tex/Driver/pc.png", "peach");
	driver[CHAR_ROSALINA]     = this->add_texture("res/tex/Driver/rs.png", "rosalina");
	driver[CHAR_SHY_GUY]      = this->add_texture("res/tex/Driver/sh.png", "shy_guy");
	driver[CHAR_TOAD]         = this->add_texture("res/tex/Driver/td.png", "toad");
	driver[CHAR_WARIO]        = this->add_texture("res/tex/Driver/wr.png", "wario");
	driver[CHAR_WIGGLER]      = this->add_texture("res/tex/Driver/wig.png", "wiggler");
	driver[CHAR_YOSHI]        = this->add_texture("res/tex/Driver/ys.png", "yoshi");
	LOG_TRACE("character textures loaded");
	// karts
	body[KART_STANDARD]      = this->add_texture("res/tex/Kart/Body/body_std.png", "standard");
	body[KART_BOLT_BUGGY]    = this->add_texture("res/tex/Kart/Body/body_rally.png", "bolt_buggy");
	body[KART_BIRTHDAY_GIRL] = this->add_texture("res/tex/Kart/Body/body_rbn.png", "birthday_girl");
	body[KART_EGG_1]         = this->add_texture("res/tex/Kart/Body/body_egg.png", "egg_1");
	body[KART_BDASHER]       = this->add_texture("res/tex/Kart/Body/body_dsh.png", "b_dasher");
	body[KART_ZUCCHINI]      = this->add_texture("res/tex/Kart/Body/body_cuc.png", "zucchini");
	body[KART_KOOPA_CLOWN]   = this->add_texture("res/tex/Kart/Body/body_kpc.png", "koopa_clown");
	body[KART_TINY_TUG]      = this->add_texture("res/tex/Kart/Body/body_boat.png", "tiny_tug");
	body[KART_BUMBLE_V]      = this->add_texture("res/tex/Kart/Body/body_hny.png", "bumble_v");
	body[KART_CACT_X]        = this->add_texture("res/tex/Kart/Body/body_sabo.png", "cact-x");
	body[KART_BRUISER]       = this->add_texture("res/tex/Kart/Body/body_gng.png", "bruiser");
	body[KART_PIPE_FRAME]    = this->add_texture("res/tex/Kart/Body/body_pipe.png", "pipe_frame");
	body[KART_BARREL_TRAIN]  = this->add_texture("res/tex/Kart/Body/body_trn.png", "barrel_train");
	body[KART_CLOUD_9]       = this->add_texture("res/tex/Kart/Body/body_cld.png", "cloud_9");
	body[KART_BLUE_SEVEN]    = this->add_texture("res/tex/Kart/Body/body_race.png", "blue_seven");
	body[KART_SODA_JET]      = this->add_texture("res/tex/Kart/Body/body_jet.png", "soda_jet");
	body[KART_GOLD_STANDARD] = this->add_texture("res/tex/Kart/Body/body_gold.png", "gold_standard");
	LOG_TRACE("kart textures loaded");
	// tires
	tire[TIRE_STANDARD]    = this->add_texture("res/tex/Kart/Tire/tire_std.png", "standard");
	tire[TIRE_MONSTER]     = this->add_texture("res/tex/Kart/Tire/tire_big.png", "monster");
	tire[TIRE_ROLLER]      = this->add_texture("res/tex/Kart/Tire/tire_small.png", "roller");
	tire[TIRE_SLICK]       = this->add_texture("res/tex/Kart/Tire/tire_race.png", "slick");
	tire[TIRE_SLIM]        = this->add_texture("res/tex/Kart/Tire/tire_classic.png", "slim");
	tire[TIRE_SPONGE]      = this->add_texture("res/tex/Kart/Tire/tire_sponge.png", "sponge");
	tire[TIRE_GOLD_TIRES]  = this->add_texture("res/tex/Kart/Tire/tire_gold.png", "gold_tires");
	tire[TIRE_WOOD]        = this->add_texture("res/tex/Kart/Tire/tire_wood.png", "wood");
	tire[TIRE_RED_MONSTER] = this->add_texture("res/tex/Kart/Tire/tire_bigRed.png", "red_monster");
	tire[TIRE_MUSHROOM]    = this->add_texture("res/tex/Kart/Tire/tire_mush.png", "mushroom");
	LOG_TRACE("tire textures loaded");
	// gliders
	wing[WING_SUPER_GLIDER]  = this->add_texture("res/tex/Kart/Wing/wing_std.png", "super_glider");
	wing[WING_PARAGLIDER]    = this->add_texture("res/tex/Kart/Wing/wing_para.png", "paraglider");
	wing[WING_PEACH_PARASOL] = this->add_texture("res/tex/Kart/Wing/wing_umb.png", "peach_parasol");
	wing[WING_FLOWER_GLIDER] = this->add_texture("res/tex/Kart/Wing/wing_flower.png", "flower_glider");
	wing[WING_SWOOPER]       = this->add_texture("res/tex/Kart/Wing/wing_basa.png", "swooper");
	wing[WING_BEAST_GLIDER]  = this->add_texture("res/tex/Kart/Wing/wing_met.png", "beast_glider");
	wing[WING_GOLD_GLIDER]   = this->add_texture("res/tex/Kart/Wing/wing_gold.png", "gold_glider");
	LOG_TRACE("glider textures loaded");
	// symbols
	symbol[0] = this->add_texture("res/tex/Symbol/first_person.png", "first_person_badge");
	symbol[1] = this->add_texture("res/tex/Symbol/flag_atlas.png", "flag_atlas");
	symbol[2] = this->add_texture("res/tex/Symbol/mii_unknown.png", "mii_unknown");
	LOG_TRACE("extra textures loaded");
}


void* TextureManager::add_texture(const char* dir, std::string_view id)
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

struct TextureThreadQueueData
{
	CFLStoreData* mii_raw_data;
	AsyncTextureData* texture;
};

std::mutex mlock;
std::queue<TextureThreadQueueData> texture_queue;
std::condition_variable cv;

void async_job_fetch_texture()
{
	std::unique_lock<std::mutex> lock(mlock);

	while (1)
	{
		cv.wait(lock, []{ return texture_queue.size() > 0; });

		auto job = texture_queue.front();
		texture_queue.pop();

		httplib::Client client = httplib::Client("https://mii-unsecure.ariankordi.net");
		const std::string mii_url = "/miis/image.png?data={}&width=128&resourceType=low&shaderType=ffliconwithbody&bodyType=3ds&characterYRotate=333&lightEnable=true";

		static char s[(sizeof(CFLStoreData) * 2) + 1];
		s[sizeof(CFLStoreData)] = '\00';

		char* p = s;
		for (size_t i = 0; i < sizeof(CFLStoreData); i++)
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
				return true;
			}
		);

		auto view = __dummy_stream.view();
		img = SDL_IOFromMem((void*)view.data(), view.size());

		job.texture->surface = IMG_Load_IO(img, true);
		job.texture->status = SURFACE_CREATED;
	}
}
std::thread t(async_job_fetch_texture);

void* TextureManager::request_mii_texture(CFLStoreData* mii_raw_data)
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
			mii_textures.at(crc16).status = READY;
		}

		return mii_textures.at(crc16).data;
	}
	else
	{
		mii_textures[crc16] = { .status = WORKING, .data = nullptr };

		texture_queue.push({ mii_raw_data, &mii_textures[crc16] });
		cv.notify_all();

		return nullptr;
	}
#else
	const uint8_t gender = mii_raw_data->misc_detail_0 & 0x01;

	if (gender)
	{
		return driver[CHAR_MII_MALE];
	}
	else
	{
		return driver[CHAR_MII_FEMALE];
	}
#endif
}