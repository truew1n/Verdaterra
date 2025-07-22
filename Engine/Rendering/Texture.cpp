#include "Texture.h"
#include <iostream>

#include "Utils/Logger.h"

uint32_t CTexture::GetNextUnit()
{
	uint32_t MaxUnit;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, (GLint*) &MaxUnit);
	MMaxUnit = (MMaxUnit + 1) % MaxUnit;
	return MMaxUnit;
}

void CTexture::Create()
{
	MUsage = ETextureUsage::None;
	MUnit = GetNextUnit();

	glGenTextures(1, &MId);
}

void CTexture::Create(const char *Filepath)
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

	glGenTextures(1, &MId);
	glActiveTexture(GL_TEXTURE0 + MUnit);
	glBindTexture(GL_TEXTURE_2D, MId);

	SetTextureParameter(ETextureParameter::MinFilter, ETextureParameterValue::LinearMipMapLinear);
	SetTextureParameter(ETextureParameter::MagFilter, ETextureParameterValue::Linear);

	SetTextureParameter(ETextureParameter::WrapU, ETextureParameterValue::Repeat);
	SetTextureParameter(ETextureParameter::WrapV, ETextureParameterValue::Repeat);

	switch (MChannels) {
		case 4: {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureWidth, TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
			break;
		}
		case 3: {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureWidth, TextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
			break;
		}
		case 1: {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, TextureWidth, TextureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, TextureData);
			break;
		}
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(TextureData);
}

void CTexture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + MUnit);
	glBindTexture(GL_TEXTURE_2D, MId);
}

void CTexture::SetTextureParameter(ETextureParameter Type, ETextureParameterValue Value)
{
	 
	glTexParameteri(GL_TEXTURE_2D, static_cast<GLenum>(Type), static_cast<GLint>(Value));
}

ETextureUsage CTexture::GetUsage() const
{
	return MUsage;
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
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CTexture::Destroy()
{
	glDeleteTextures(1, &MId);
}