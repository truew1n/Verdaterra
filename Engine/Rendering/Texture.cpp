#include "Texture.h"
#include <iostream>

#include "Util/Logger.h"

uint32_t CTexture::GetNextUnit()
{
	uint32_t MaxUnit;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, (GLint*) &MaxUnit);
	MMaxUnit = (MMaxUnit + 1) % MaxUnit;
	return MMaxUnit;
}

void CTexture::Create()
{
	Create(ETextureType::Texture2D);
}

void CTexture::Create(ETextureType Type)
{
	MUsage = ETextureUsage::None;
	MUnit = GetNextUnit();

	glCreateTextures(static_cast<uint32_t>(Type), 1, &MId);
}

void CTexture::Create(const char *Filepath)
{
	Create(Filepath, ETextureType::Texture2D);
}

void CTexture::Create(const char *Filepath, ETextureType Type)
{
	MUsage = ETextureUsage::None;
	int32_t TextureWidth, TextureHeight;
	stbi_set_flip_vertically_on_load(true);
	uint8_t *TextureData = stbi_load(Filepath, &TextureWidth, &TextureHeight, &MChannels, 0);

	if (!TextureData) {
		LOG_ERROR_FORMAT(
			"Failed to load texture: %s",
			Filepath
		);
		return;
	}

	MUnit = GetNextUnit();

	glCreateTextures(static_cast<uint32_t>(Type), 1, &MId);

	SetMinFilter(ETextureMinFilter::LinearMipMapLinear);
	SetMagFilter(ETextureMagFilter::Linear);

	SetHorizontalWrap(ETextureWrap::Repeat);
	SetVerticalWrap(ETextureWrap::Repeat);

	switch (MChannels) {
		case 4: {
			glTextureStorage2D(MId, 1, GL_RGBA, TextureWidth, TextureHeight);
			glTextureSubImage2D(MId, 0, 0, 0, TextureWidth, TextureHeight, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
			break;
		}
		case 3: {
			glTextureStorage2D(MId, 1, GL_RGB, TextureWidth, TextureHeight);
			glTextureSubImage2D(MId, 0, 0, 0, TextureWidth, TextureHeight, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
			break;
		}
		case 1: {
			glTextureStorage2D(MId, 1, GL_RED, TextureWidth, TextureHeight);
			glTextureSubImage2D(MId, 0, 0, 0, TextureWidth, TextureHeight, GL_RED, GL_UNSIGNED_BYTE, TextureData);
			break;
		}
	}

	glGenerateTextureMipmap(MId);

	stbi_image_free(TextureData);
}

void CTexture::Bind()
{
	glBindTextureUnit(MUnit, MId);
}

void CTexture::SetMinFilter(ETextureMinFilter Value)
{
	glTextureParameteri(MId, GL_TEXTURE_MIN_FILTER, static_cast<uint32_t>(Value));
}

void CTexture::SetMagFilter(ETextureMagFilter Value)
{
	glTextureParameteri(MId, GL_TEXTURE_MAG_FILTER, static_cast<uint32_t>(Value));
}

void CTexture::SetHorizontalWrap(ETextureWrap Value)
{
	glTextureParameteri(MId, GL_TEXTURE_WRAP_S, static_cast<uint32_t>(Value));
}

void CTexture::SetVerticalWrap(ETextureWrap Value)
{
	glTextureParameteri(MId, GL_TEXTURE_WRAP_T, static_cast<uint32_t>(Value));
}

ETextureUsage CTexture::GetUsage() const
{
	return MUsage;
}

ETextureFormat CTexture::GetFormat() const
{
	return MFormat;
}

void CTexture::SetUsage(ETextureUsage Usage)
{
	MUsage = Usage;
}

uint32_t CTexture::GetUnit() const
{
	return MUnit;
}

uint32_t CTexture::GetChannels() const
{
	return MChannels;
}

void CTexture::Unbind()
{
	glBindTextureUnit(MUnit, 0);
}

void CTexture::Destroy()
{
	glDeleteTextures(1, &MId);
}