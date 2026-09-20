#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct StudioCharInfo {
	uint8_t beardColor;
	uint8_t beardType;
	uint8_t build;
	uint8_t eyeAspect;
	uint8_t eyeColor;
	uint8_t eyeRotate;
	uint8_t eyeScale;
	uint8_t eyeType;
	uint8_t eyeX;
	uint8_t eyeY;
	uint8_t eyebrowAspect;
	uint8_t eyebrowColor;
	uint8_t eyebrowRotate;
	uint8_t eyebrowScale;
	uint8_t eyebrowType;
	uint8_t eyebrowX;
	uint8_t eyebrowY;
	uint8_t facelineColor;
	uint8_t facelineMake;
	uint8_t facelineType;
	uint8_t facelineWrinkle;
	uint8_t favoriteColor;
	uint8_t gender;
	uint8_t glassColor;
	uint8_t glassScale;
	uint8_t glassType;
	uint8_t glassY;
	uint8_t hairColor;
	uint8_t hairFlip;
	uint8_t hairType;
	uint8_t height;
	uint8_t moleScale;
	uint8_t moleType;
	uint8_t moleX;
	uint8_t moleY;
	uint8_t mouthAspect;
	uint8_t mouthColor;
	uint8_t mouthScale;
	uint8_t mouthType;
	uint8_t mouthY;
	uint8_t mustacheScale;
	uint8_t mustacheType;
	uint8_t mustacheY;
	uint8_t noseScale;
	uint8_t noseType;
	uint8_t noseY;
} StudioCharInfo;

void ConvFromVer3MiiDataCore(StudioCharInfo* dst, const uint8_t* data);
void ConvFieldsVer3ToNx(StudioCharInfo* info);
bool IsValid(const StudioCharInfo* info);

#ifdef __cplusplus
}
#endif