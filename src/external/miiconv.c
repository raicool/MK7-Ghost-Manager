// The functions from this file are decompiled and
// translated into Fusion code for MiiToStudio.
//
// gcc-15 -std=c99 -nostdlib -Wall -Wextra -Wconversion -Wpedantic mii-to-studio-conv.c -Os -g -c
// After compiling - which requires GCC for correct Wii big-endian reads - pass the .o into Ghidra.
#include <stdint.h>
#include <stdbool.h>

#include "external/miiconv.h"

bool IsValid(const StudioCharInfo* info) {
	if (info->beardColor > 99) return false;
	if (info->beardType > 5) return false;
	if (info->build > 127) return false;
	if (info->eyeAspect > 6) return false;
	if (info->eyeColor > 99) return false;
	if (info->eyeRotate > 7) return false;
	if (info->eyeScale > 7) return false;
	if (info->eyeType > 59) return false;
	if (info->eyeX > 12) return false;
	if (info->eyeY > 18) return false;

	if (info->eyebrowAspect > 6) return false;
	if (info->eyebrowColor > 99) return false;
	if (info->eyebrowRotate > 11) return false;
	if (info->eyebrowScale > 8) return false;
	if (info->eyebrowType > 23) return false;
	if (info->eyebrowX > 12) return false;
	// if (info->eyebrowY < 3 || info->eyebrowY > 18) return false; // same reason
	if (info->eyebrowY < 3) return false;
	if (info->eyebrowY > 18) return false;

	if (info->facelineColor > 9) return false;
	if (info->facelineMake > 11) return false;
	if (info->facelineType > 11) return false;
	if (info->facelineWrinkle > 11) return false;

	if (info->favoriteColor > 11) return false;
	if (info->gender > 1) return false;

	if (info->glassColor > 99) return false;
	if (info->glassScale > 7) return false;
	if (info->glassType > 19) return false;
	if (info->glassY > 20) return false;

	if (info->hairColor > 99) return false;
	if (info->hairFlip > 1) return false;
	if (info->hairType > 131) return false;

	if (info->height > 127) return false;

	if (info->moleScale > 8) return false;
	if (info->moleType > 1) return false;
	if (info->moleX > 16) return false;
	if (info->moleY > 30) return false;

	if (info->mouthAspect > 6) return false;
	if (info->mouthColor > 99) return false;
	if (info->mouthScale > 8) return false;
	if (info->mouthType > 35) return false;
	if (info->mouthY > 18) return false;

	if (info->mustacheScale > 8) return false;
	if (info->mustacheType > 5) return false;
	if (info->mustacheY > 16) return false;

	if (info->noseScale > 8) return false;
	if (info->noseType > 17) return false;
	if (info->noseY > 18) return false;

	return true;
}

void ConvFromVer3MiiDataCore(StudioCharInfo* dst, const uint8_t* data) {
	/// Packed structure representing 3DS/Wii U Mii data.
	typedef struct __attribute__((packed, scalar_storage_order("little-endian"))) Ver3MiiDataCore {
		// Packed attribute is not needed.
		// Alignment of 4 can be used. It just makes
		// the compiler only access one byte at a time, which
		// is useful when decompiling the getter/setters

		// 00/0x00 - Lower personal fields.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint32_t miiVersion : 8;  // (LSB)
		uint32_t copyable : 1;
		uint32_t ngWord : 1;
		uint32_t regionMove : 2;
		uint32_t fontRegion : 2;
		uint32_t reserved_0 : 2;
		uint32_t roomIndex : 4;  ///< Maximum = 9
		uint32_t positionInRoom : 4;  ///< Maximum = 9
		uint32_t authorType : 4;  ///< Logically unused.
		uint32_t birthPlatform : 3;
		uint32_t reserved_1 : 1;  // (MSB)
#else
		uint32_t reserved_1 : 1;  // (MSB)
		uint32_t birthPlatform : 3;
		uint32_t authorType : 4;
		uint32_t positionInRoom : 4;
		uint32_t roomIndex : 4;
		uint32_t reserved_0 : 2;
		uint32_t fontRegion : 2;
		uint32_t regionMove : 2;
		uint32_t ngWord : 1;
		uint32_t copyable : 1;
		uint32_t miiVersion : 8;  // (LSB)
#endif

		// 04/0x04 - 8 byte value containing the transferable ID.
		uint8_t authorId[8];

		// 12/0x0C - 10 byte value identifying the character.
		uint8_t createId[10]; // Renamed (as well as above) to Id instead of ID
		uint8_t reserved_2[2];

		// 24/0x18 - Higher personal fields.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint16_t gender : 1;
		uint16_t birthMonth : 4;
		uint16_t birthDay : 5;
		uint16_t favoriteColor : 4;
		uint16_t favorite : 1;
		uint16_t padding_0 : 1;
#else
		uint16_t padding_0 : 1;
		uint16_t favorite : 1;
		uint16_t favoriteColor : 4;
		uint16_t birthDay : 5;
		uint16_t birthMonth : 4;
		uint16_t gender : 1;
#endif

		// 26/0x1A - 10-character name in UTF-16.
		uint16_t name[10];

		// 46/0x2E - Body parameters.
		uint8_t height;
		uint8_t build;

		// 48/0x30 - Faceline fields + localonly.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint16_t localonly : 1;
		uint16_t faceType : 4;
		uint16_t faceColor : 3;
		uint16_t faceTex : 4;
		uint16_t faceMake : 4;
#else
		uint16_t faceMake : 4;
		uint16_t faceTex : 4;
		uint16_t faceColor : 3;
		uint16_t faceType : 4;
		uint16_t localonly : 1;
#endif

		// 50/0x32 - Hair fields.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint16_t hairType : 8;
		uint16_t hairColor : 3;
		uint16_t hairFlip : 1;
		uint16_t padding_1 : 4;
#else
		uint16_t padding_1 : 4;
		uint16_t hairFlip : 1;
		uint16_t hairColor : 3;
		uint16_t hairType : 8;
#endif

		// 52/0x34 - Eye fields part 1.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint16_t eyeType : 6;
		uint16_t eyeColor : 3;
		uint16_t eyeScale : 4;
		uint16_t eyeAspect : 3;
#else
		uint16_t eyeAspect : 3;
		uint16_t eyeScale : 4;
		uint16_t eyeColor : 3;
		uint16_t eyeType : 6;
#endif

		// 54/0x36 - Eye fields part 2.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint16_t eyeRotate : 5;
		uint16_t eyeX : 4;
		uint16_t eyeY : 5;
		uint16_t padding_2 : 2;
#else
		uint16_t padding_2 : 2;
		uint16_t eyeY : 5;
		uint16_t eyeX : 4;
		uint16_t eyeRotate : 5;
#endif

		// 56/0x38 - Eyebrow fields part 1.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint16_t eyebrowType : 5;
		uint16_t eyebrowColor : 3;
		uint16_t eyebrowScale : 4;
		uint16_t eyebrowAspect : 3;
		uint16_t padding_3 : 1;
#else
		uint16_t padding_3 : 1;
		uint16_t eyebrowAspect : 3;
		uint16_t eyebrowScale : 4;
		uint16_t eyebrowColor : 3;
		uint16_t eyebrowType : 5;
#endif

		// 58/0x3A - Eyebrow fields part 2.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint16_t eyebrowRotate : 5;
		uint16_t eyebrowX : 4;
		uint16_t eyebrowY : 5;
		uint16_t padding_4 : 2;
#else
		uint16_t padding_4 : 2;
		uint16_t eyebrowY : 5;
		uint16_t eyebrowX : 4;
		uint16_t eyebrowRotate : 5;
#endif

		// 60/0x3C - Nose fields.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint16_t noseType : 5;
		uint16_t noseScale : 4;
		uint16_t noseY : 5;
		uint16_t padding_5 : 2;
#else
		uint16_t padding_5 : 2;
		uint16_t noseY : 5;
		uint16_t noseScale : 4;
		uint16_t noseType : 5;
#endif

		// 62/0x3E - Mouth fields part 1.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint16_t mouthType : 6;
		uint16_t mouthColor : 3;
		uint16_t mouthScale : 4;
		uint16_t mouthAspect : 3;
#else
		uint16_t mouthAspect : 3;
		uint16_t mouthScale : 4;
		uint16_t mouthColor : 3;
		uint16_t mouthType : 6;
#endif

		// 64/0x40 - Mouth fields part 2 + mustache type.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint16_t mouthY : 5;
		uint16_t mustacheType : 3;
		uint16_t padding_6 : 8;
#else
		uint16_t padding_6 : 8;
		uint16_t mustacheType : 3;
		uint16_t mouthY : 5;
#endif

		// 66/0x42 - Beard fields.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint16_t beardType : 3;
		uint16_t beardColor : 3;
		uint16_t beardScale : 4;
		uint16_t beardY : 5;
		uint16_t padding_7 : 1;
#else
		uint16_t padding_7 : 1;
		uint16_t beardY : 5;
		uint16_t beardScale : 4;
		uint16_t beardColor : 3;
		uint16_t beardType : 3;
#endif

		// 68/0x44 - Glass fields.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint16_t glassType : 4;
		uint16_t glassColor : 3;
		uint16_t glassScale : 4;
		uint16_t glassY : 5;
#else
		uint16_t glassY : 5;
		uint16_t glassScale : 4;
		uint16_t glassColor : 3;
		uint16_t glassType : 4;
#endif

		// 70/0x46 - Mole fields.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint16_t moleType : 1;
		uint16_t moleScale : 4;
		uint16_t moleX : 5;
		uint16_t moleY : 5;
		uint16_t padding_8 : 1;
#else
		uint16_t padding_8 : 1;
		uint16_t moleY : 5;
		uint16_t moleX : 5;
		uint16_t moleScale : 4;
		uint16_t moleType : 1;
#endif
		// 72/0x48 - End of core.
	} Ver3MiiDataCore;
	// Color indices need to be converted using Ver3 tables: https://github.com/Genwald/MiiPort/blob/4ee38bbb8aa68a2365e9c48d59d7709f760f9b5d/include/convert_mii.h#L8
	// A shortcut that is equivalent to the tables is used in this snippet.

	const Ver3MiiDataCore* src = (const Ver3MiiDataCore*)(data);

	dst->beardColor = src->beardColor; // Convert to CommonColor
	dst->beardType = src->beardType;
	dst->build = src->build;
	dst->eyeAspect = src->eyeAspect;
	dst->eyeColor = src->eyeColor; // Convert to CommonColor
	dst->eyeRotate = src->eyeRotate;
	dst->eyeScale = src->eyeScale;
	dst->eyeType = src->eyeType;
	dst->eyeX = src->eyeX;
	dst->eyeY = src->eyeY;
	dst->eyebrowAspect = src->eyebrowAspect;
	dst->eyebrowColor = src->eyebrowColor; // Convert to CommonColor
	dst->eyebrowRotate = src->eyebrowRotate;
	dst->eyebrowScale = src->eyebrowScale;
	dst->eyebrowType = src->eyebrowType;
	dst->eyebrowX = src->eyebrowX;
	dst->eyebrowY = src->eyebrowY;

	// All faceline fields are named differently.
	dst->facelineColor = src->faceColor; // No up conversion needed
	dst->facelineMake = src->faceMake;
	dst->facelineType = src->faceType;
	dst->facelineWrinkle = src->faceTex;

	dst->favoriteColor = src->favoriteColor; // Unchanged
	dst->gender = src->gender;
	dst->glassColor = src->glassColor; // Convert to CommonColor
	dst->glassScale = src->glassScale;
	dst->glassType = src->glassType; // No up conversion needed
	dst->glassY = src->glassY;
	dst->hairColor = src->hairColor; // Convert to CommonColor
	dst->hairFlip = src->hairFlip;
	dst->hairType = src->hairType;
	dst->height = src->height;
	dst->moleScale = src->moleScale;
	dst->moleType = src->moleType;
	dst->moleX = src->moleX;
	dst->moleY = src->moleY;
	dst->mouthAspect = src->mouthAspect;
	dst->mouthColor = src->mouthColor; // Convert to CommonColor
	dst->mouthScale = src->mouthScale;
	dst->mouthType = src->mouthType;
	dst->mouthY = src->mouthY;

	// Beard fields are named differently.
	dst->mustacheScale = src->beardScale;
	dst->mustacheType = src->mustacheType;
	dst->mustacheY = src->beardY;

	dst->noseScale = src->noseScale;
	dst->noseType = src->noseType;
	dst->noseY = src->noseY;
}

void ConvFromRflMiiDataCore(StudioCharInfo* dst, const uint8_t* data) {
	// RFLiHiddenCharData, CFLiRFLMiiDataCore, FFLiMiiDataCoreRFL
	typedef struct __attribute__((packed, scalar_storage_order("big-endian"))) RFLMiiDataCore {

#if 0 //
		uint16_t favorite : 1;
		uint16_t favoriteColor : 4;
		uint16_t birthDay : 5;
		uint16_t birthMonth : 4;
		uint16_t gender : 1;
		uint16_t padding0 : 1;
#else
		uint16_t padding0 : 1;
		uint16_t gender : 1; // Renamed from "sex"
		// Renamed from "birth_padding" in RFLiHiddenCharData - not present there?
		// uint16_t birth_padding : 9;
		uint16_t birthMonth : 4;
		uint16_t birthDay : 5;
		// Above are renamed from "birth_month"/"birth_day"
		uint16_t favoriteColor : 4;
		uint16_t favorite : 1;
#endif

		uint16_t name[10];
		uint8_t height;
		uint8_t build;
		uint8_t createId[8]; // Renamed from "createID"

#if 0 //
		uint16_t type : 2;
		uint16_t localonly : 1;
		uint16_t padding2 : 3;
		uint16_t faceTex : 4;
		uint16_t faceColor : 3;
		uint16_t faceType : 3;
#else
		uint16_t faceType : 3;
		uint16_t faceColor : 3;
		uint16_t faceTex : 4;
		uint16_t padding2 : 3;
		uint16_t localonly : 1;
		uint16_t type : 2;
#endif

#if 0 //
		uint16_t padding3 : 5;
		uint16_t hairFlip : 1;
		uint16_t hairColor : 3;
		uint16_t hairType : 7;
#else
		uint16_t hairType : 7;
		uint16_t hairColor : 3;
		uint16_t hairFlip : 1;
		uint16_t padding3 : 5;
#endif

#if 0 //
		uint16_t padding4 : 6;
		uint16_t eyebrowRotate : 5;
		uint16_t eyebrowType : 5;
#else
		uint16_t eyebrowType : 5;
		uint16_t eyebrowRotate : 5;
		uint16_t padding4 : 6;
#endif

#if 0 //
		uint16_t eyebrowX : 4;
		uint16_t eyebrowY : 5;
		uint16_t eyebrowScale : 4;
		uint16_t eyebrowColor : 3;
#else
		uint16_t eyebrowColor : 3;
		uint16_t eyebrowScale : 4;
		uint16_t eyebrowY : 5;
		uint16_t eyebrowX : 4;
#endif

#if 0 //
		uint16_t eyeY : 5;
		uint16_t eyeRotate : 5;
		uint16_t eyeType : 6;
#else
		uint16_t eyeType : 6;
		uint16_t eyeRotate : 5;
		uint16_t eyeY : 5;
#endif

#if 0 //
		uint16_t padding5 : 5;
		uint16_t eyeX : 4;
		uint16_t eyeScale : 4;
		uint16_t eyeColor : 3;
#else
		uint16_t eyeColor : 3;
		uint16_t eyeScale : 4;
		uint16_t eyeX : 4;
		uint16_t padding5 : 5;
#endif

#if 0 //
		uint16_t padding6 : 3;
		uint16_t noseY : 5;
		uint16_t noseScale : 4;
		uint16_t noseType : 4;
#else
		uint16_t noseType : 4;
		uint16_t noseScale : 4;
		uint16_t noseY : 5;
		uint16_t padding6 : 3;
#endif

#if 0 //
		uint16_t mouthY : 5;
		uint16_t mouthScale : 4;
		uint16_t mouthColor : 2;
		uint16_t mouthType : 5;
#else
		uint16_t mouthType : 5;
		uint16_t mouthColor : 2;
		uint16_t mouthScale : 4;
		uint16_t mouthY : 5;
#endif

#if 0 //
		uint16_t glassY : 5;
		uint16_t glassScale : 4;
		uint16_t glassColor : 3;
		uint16_t glassType : 4;
#else
		uint16_t glassType : 4;
		uint16_t glassColor : 3;
		uint16_t glassScale : 4;
		uint16_t glassY : 5;
#endif

#if 0 //
		uint16_t beardY : 5;
		uint16_t beardScale : 4;
		uint16_t beardColor : 3;
		uint16_t beardType : 2;
		uint16_t mustacheType : 2;
#else
		uint16_t mustacheType : 2;
		uint16_t beardType : 2;
		uint16_t beardColor : 3;
		uint16_t beardScale : 4;
		uint16_t beardY : 5;
#endif

#if 0 //
		uint16_t padding8 : 1;
		uint16_t moleX : 5;
		uint16_t moleY : 5;
		uint16_t moleScale : 4;
		uint16_t moleType : 1;
#else
		uint16_t moleType : 1;
		uint16_t moleScale : 4;
		uint16_t moleY : 5;
		uint16_t moleX : 5;
		uint16_t padding8 : 1;
#endif

		//uint8_t padding9[10];
		// ^^ in RFLiHiddenCharData, but... why...
	} RFLMiiDataCore;

	// Faceline makeup/wrinkle are derived from
	// the single "faceTex" property, from this table:
	const uint8_t faceTexTable[12][2] = {
		{ 0, 0 },
		{ 0, 1 },
		{ 0, 6 },
		{ 0, 9 },
		{ 5, 0 },
		{ 2, 0 },
		{ 3, 0 },
		{ 7, 0 },
		{ 8, 0 },
		{ 0, 10 },
		{ 9, 0 },
		{ 11, 0 }
	};

	const RFLMiiDataCore* src = (const RFLMiiDataCore*)(data);

	dst->beardColor = src->beardColor; // Convert to CommonColor
	dst->beardType = src->beardType;
	dst->build = src->build;
	dst->eyeAspect = 3; // Aspect field default = 3.
	dst->eyeColor = src->eyeColor; // Convert to CommonColor
	dst->eyeRotate = src->eyeRotate;
	dst->eyeScale = src->eyeScale;
	dst->eyeType = src->eyeType;
	dst->eyeX = src->eyeX;
	dst->eyeY = src->eyeY;
	dst->eyebrowAspect = 3;
	dst->eyebrowColor = src->eyebrowColor; // Convert to CommonColor
	dst->eyebrowRotate = src->eyebrowRotate;
	dst->eyebrowScale = src->eyebrowScale;
	dst->eyebrowType = src->eyebrowType;
	dst->eyebrowX = src->eyebrowX;
	dst->eyebrowY = src->eyebrowY;

	// All faceline fields are named differently.
	dst->facelineColor = src->faceColor; // No up conversion needed
	// The faceTexTable looks up make/wrinkle from faceTex.
	dst->facelineMake = faceTexTable[src->faceTex][1];
	dst->facelineType = src->faceType;
	dst->facelineWrinkle = faceTexTable[src->faceTex][0];

	dst->favoriteColor = src->favoriteColor; // Unchanged
	dst->gender = src->gender;
	dst->glassColor = src->glassColor; // Convert to CommonColor
	dst->glassScale = src->glassScale;
	dst->glassType = src->glassType; // No up conversion needed
	dst->glassY = src->glassY;
	dst->hairColor = src->hairColor; // Convert to CommonColor
	dst->hairFlip = src->hairFlip;
	dst->hairType = src->hairType;
	dst->height = src->height;
	dst->moleScale = src->moleScale;
	dst->moleType = src->moleType;
	dst->moleX = src->moleX;
	dst->moleY = src->moleY;
	dst->mouthAspect = 3;
	dst->mouthColor = src->mouthColor; // Convert to CommonColor
	dst->mouthScale = src->mouthScale;
	dst->mouthType = src->mouthType;
	dst->mouthY = src->mouthY;

	// Beard fields are named differently.
	dst->mustacheScale = src->beardScale;
	dst->mustacheType = src->mustacheType;
	dst->mustacheY = src->beardY;

	dst->noseScale = src->noseScale;
	dst->noseType = src->noseType;
	dst->noseY = src->noseY;
}

void ConvFromNxCharInfo(StudioCharInfo* dst, const uint8_t* data) {
	typedef struct NxCharInfo { // mii_CharInfoRaw.h : 40
		uint8_t createId[16];
		uint16_t nickname[11];
		uint8_t fontRegion;
		uint8_t favoriteColor;
		uint8_t gender;
		uint8_t height;
		uint8_t build;
		uint8_t type;
		uint8_t regionMove;
		uint8_t facelineType;
		uint8_t facelineColor;
		uint8_t facelineWrinkle;
		uint8_t facelineMake;
		uint8_t hairType;
		uint8_t hairColor;
		uint8_t hairFlip;
		uint8_t eyeType;
		uint8_t eyeColor;
		uint8_t eyeScale;
		uint8_t eyeAspect;
		uint8_t eyeRotate;
		uint8_t eyeX;
		uint8_t eyeY;
		uint8_t eyebrowType;
		uint8_t eyebrowColor;
		uint8_t eyebrowScale;
		uint8_t eyebrowAspect;
		uint8_t eyebrowRotate;
		uint8_t eyebrowX;
		uint8_t eyebrowY;
		uint8_t noseType;
		uint8_t noseScale;
		uint8_t noseY;
		uint8_t mouthType;
		uint8_t mouthColor;
		uint8_t mouthScale;
		uint8_t mouthAspect;
		uint8_t mouthY;
		uint8_t beardColor;
		uint8_t beardType;
		uint8_t mustacheType;
		uint8_t mustacheScale;
		uint8_t mustacheY;
		uint8_t glassType;
		uint8_t glassColor;
		uint8_t glassScale;
		uint8_t glassY;
		uint8_t moleType;
		uint8_t moleScale;
		uint8_t moleX;
		uint8_t moleY;
		uint8_t pad;
	} NxCharInfo;

	const NxCharInfo* src = (const NxCharInfo*)(data);

	dst->beardColor = src->beardColor;
	dst->beardType = src->beardType;
	dst->build = src->build;
	dst->eyeAspect = src->eyeAspect;
	dst->eyeColor = src->eyeColor;
	dst->eyeRotate = src->eyeRotate;
	dst->eyeScale = src->eyeScale;
	dst->eyeType = src->eyeType;
	dst->eyeX = src->eyeX;
	dst->eyeY = src->eyeY;
	dst->eyebrowAspect = src->eyebrowAspect;
	dst->eyebrowColor = src->eyebrowColor;
	dst->eyebrowRotate = src->eyebrowRotate;
	dst->eyebrowScale = src->eyebrowScale;
	dst->eyebrowType = src->eyebrowType;
	dst->eyebrowX = src->eyebrowX;
	dst->eyebrowY = src->eyebrowY;
	dst->facelineColor = src->facelineColor;
	dst->facelineMake = src->facelineMake;
	dst->facelineType = src->facelineType;
	dst->facelineWrinkle = src->facelineWrinkle;
	dst->favoriteColor = src->favoriteColor;
	dst->gender = src->gender;
	dst->glassColor = src->glassColor;
	dst->glassScale = src->glassScale;
	dst->glassType = src->glassType;
	dst->glassY = src->glassY;
	dst->hairColor = src->hairColor;
	dst->hairFlip = src->hairFlip;
	dst->hairType = src->hairType;
	dst->height = src->height;
	dst->moleScale = src->moleScale;
	dst->moleType = src->moleType;
	dst->moleX = src->moleX;
	dst->moleY = src->moleY;
	dst->mouthAspect = src->mouthAspect;
	dst->mouthColor = src->mouthColor;
	dst->mouthScale = src->mouthScale;
	dst->mouthType = src->mouthType;
	dst->mouthY = src->mouthY;
	dst->mustacheScale = src->mustacheScale;
	dst->mustacheType = src->mustacheType;
	dst->mustacheY = src->mustacheY;
	dst->noseScale = src->noseScale;
	dst->noseType = src->noseType;
	dst->noseY = src->noseY;
}

void ConvFieldsVer3ToNx(StudioCharInfo* info) {
	// Convert Ver3 colors to common colors.
	if (info->hairColor == 0) info->hairColor = 8; // Map 0 to 8.
	// Beard and eyebrow color are treated like hair color.
	if (info->beardColor == 0) info->beardColor = 8;
	if (info->eyebrowColor == 0) info->eyebrowColor = 8;

	info->mouthColor += 19; // Offset mouth color by 19.
	info->eyeColor += 8; // Offset eye color by 8.

	// Convert glass color.
	if (info->glassColor == 0)  info->glassColor = 8;
	else if (info->glassColor < 6) info->glassColor += 13;

	// Clamp build and height from Ver3 maximum of 128.
	if (info->build > 127) info->build = 127;
	if (info->height > 127) info->height = 127;
}
